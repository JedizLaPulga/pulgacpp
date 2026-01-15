// pulgacpp::geometry - Core concepts and CRTP base for shapes
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_GEOMETRY_SHAPE_HPP
#define PULGACPP_GEOMETRY_SHAPE_HPP

#include "../optional/optional.hpp"

#include <cmath>
#include <concepts>
#include <type_traits>
#include <numbers>

namespace pulgacpp {

// ==================== Type Traits ====================

/// Concept to detect pulgacpp safe numeric types (i8, i32, etc.)
template <typename T>
concept SafeNumeric = requires {
    typename T::underlying_type;
    { T::MIN } -> std::convertible_to<typename T::underlying_type>;
    { T::MAX } -> std::convertible_to<typename T::underlying_type>;
    { std::declval<T>().get() } -> std::convertible_to<typename T::underlying_type>;
};

/// Concept for any numeric type usable in geometry
template <typename T>
concept Numeric = std::is_arithmetic_v<T> || SafeNumeric<T>;

/// Concept for floating point types (native or pulgacpp f32/f64 when available)
template <typename T>
concept FloatingPoint = std::floating_point<T>;

/// Concept for integral types (native or pulgacpp)
template <typename T>
concept Integral = std::integral<T> || (SafeNumeric<T> && std::integral<typename T::underlying_type>);

// ==================== Helper Functions ====================

/// Extract raw numeric value from pulgacpp or native type
template <Numeric T>
[[nodiscard]] constexpr auto raw(T value) noexcept {
    if constexpr (SafeNumeric<T>) {
        return value.get();
    } else {
        return value;
    }
}

/// Convert to double for calculations
template <Numeric T>
[[nodiscard]] constexpr double to_double(T value) noexcept {
    return static_cast<double>(raw(value));
}

/// Mathematical constants
namespace constants {
    inline constexpr double PI = std::numbers::pi;
    inline constexpr double TAU = 2.0 * std::numbers::pi;
    inline constexpr double E = std::numbers::e;
    inline constexpr double SQRT2 = std::numbers::sqrt2;
}

// ==================== Safe Arithmetic ====================

/// Safe addition - returns Optional<T> for types that can overflow
template <Numeric T>
[[nodiscard]] constexpr auto checked_add(T a, T b) noexcept {
    if constexpr (SafeNumeric<T>) {
        return a.checked_add(b);
    } else if constexpr (std::floating_point<T>) {
        return Some(a + b);  // Floating point doesn't overflow the same way
    } else {
        // Native integer - check for overflow
        if (b > 0 && a > std::numeric_limits<T>::max() - b) {
            return Optional<T>{None};  // Overflow
        }
        if (b < 0 && a < std::numeric_limits<T>::min() - b) {
            return Optional<T>{None};  // Underflow
        }
        return Some(a + b);
    }
}

/// Safe subtraction
template <Numeric T>
[[nodiscard]] constexpr auto checked_sub(T a, T b) noexcept {
    if constexpr (SafeNumeric<T>) {
        return a.checked_sub(b);
    } else if constexpr (std::floating_point<T>) {
        return Some(a - b);
    } else {
        if (b < 0 && a > std::numeric_limits<T>::max() + b) {
            return Optional<T>{None};
        }
        if (b > 0 && a < std::numeric_limits<T>::min() + b) {
            return Optional<T>{None};
        }
        return Some(a - b);
    }
}

/// Safe multiplication
template <Numeric T>
[[nodiscard]] constexpr auto checked_mul(T a, T b) noexcept {
    if constexpr (SafeNumeric<T>) {
        return a.checked_mul(b);
    } else if constexpr (std::floating_point<T>) {
        return Some(a * b);
    } else {
        // Simplified overflow check for native integers
        if (a == 0 || b == 0) return Some(static_cast<T>(0));
        T result = a * b;
        if (a != 0 && result / a != b) {
            return Optional<T>{None};  // Overflow occurred
        }
        return Some(result);
    }
}

// ==================== CRTP Base Classes ====================

/// Base class for all shapes (CRTP pattern)
template <typename Derived>
class Shape {
public:
    /// Returns the name of the shape
    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return Derived::NAME;
    }
    
    /// Returns the dimensionality (2 for 2D, 3 for 3D)
    [[nodiscard]] static constexpr unsigned dimensions() noexcept {
        return Derived::DIMENSIONS;
    }
    
    /// Type trait to identify shapes
    static constexpr bool is_shape = true;

protected:
    // CRTP helper to get derived class
    [[nodiscard]] constexpr const Derived& derived() const noexcept {
        return static_cast<const Derived&>(*this);
    }
    
    [[nodiscard]] constexpr Derived& derived() noexcept {
        return static_cast<Derived&>(*this);
    }
};

/// Base class for 2D shapes
template <typename Derived, Numeric T>
class Shape2D : public Shape<Derived> {
public:
    using value_type = T;
    static constexpr unsigned DIMENSIONS = 2;
    
    /// Returns the area of the shape
    [[nodiscard]] constexpr auto area() const noexcept {
        return this->derived().area_impl();
    }
    
    /// Returns the perimeter of the shape
    [[nodiscard]] constexpr auto perimeter() const noexcept {
        return this->derived().perimeter_impl();
    }
};

// ==================== Type Traits for Shapes ====================

/// Check if a type is a shape
template <typename T>
concept IsShape = requires {
    { T::is_shape } -> std::convertible_to<bool>;
    T::is_shape == true;
};

/// Check if a shape is 2D
template <typename T>
concept IsShape2D = IsShape<T> && T::DIMENSIONS == 2;

} // namespace pulgacpp

#endif // PULGACPP_GEOMETRY_SHAPE_HPP
