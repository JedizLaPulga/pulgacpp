// pulgacpp::Vector2 - 2D vector type
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_GEOMETRY_VECTOR2_HPP
#define PULGACPP_GEOMETRY_VECTOR2_HPP

#include "shape.hpp"
#include "point.hpp"
#include <cmath>
#include <tuple>

namespace pulgacpp {

/// A 2D vector with components (x, y)
template <Numeric T>
class Vector2 {
public:
    using value_type = T;
    static constexpr std::string_view NAME = "Vector2";
    static constexpr unsigned DIMENSIONS = 2;

private:
    T m_x;
    T m_y;

    constexpr Vector2(T x, T y) noexcept : m_x(x), m_y(y) {}

public:
    // ==================== Construction ====================

    /// Default: zero vector
    constexpr Vector2() noexcept : m_x{}, m_y{} {}

    /// Factory: create from components
    [[nodiscard]] static constexpr Vector2 from(T x, T y) noexcept {
        return Vector2(x, y);
    }

    /// Factory: zero vector
    [[nodiscard]] static constexpr Vector2 zero() noexcept {
        return Vector2{};
    }

    /// Factory: from angle (radians) and magnitude
    [[nodiscard]] static constexpr Vector2<double> from_angle(double angle, double magnitude = 1.0) noexcept {
        return Vector2<double>::from(
            magnitude * std::cos(angle),
            magnitude * std::sin(angle)
        );
    }

    // ==================== Accessors ====================

    [[nodiscard]] constexpr T x() const noexcept { return m_x; }
    [[nodiscard]] constexpr T y() const noexcept { return m_y; }

    [[nodiscard]] constexpr Vector2 with_x(T new_x) const noexcept {
        return Vector2(new_x, m_y);
    }

    [[nodiscard]] constexpr Vector2 with_y(T new_y) const noexcept {
        return Vector2(m_x, new_y);
    }

    // ==================== Magnitude ====================

    /// Length of vector (magnitude)
    [[nodiscard]] constexpr double magnitude() const noexcept {
        return std::sqrt(magnitude_squared());
    }

    /// Squared magnitude (faster, avoids sqrt)
    [[nodiscard]] constexpr double magnitude_squared() const noexcept {
        double x = to_double(m_x);
        double y = to_double(m_y);
        return x * x + y * y;
    }

    /// Check if zero vector
    [[nodiscard]] constexpr bool is_zero() const noexcept {
        return raw(m_x) == 0 && raw(m_y) == 0;
    }

    // ==================== Normalization ====================

    /// Normalize to unit vector
    /// Returns (success, normalized_vector)
    [[nodiscard]] constexpr std::pair<bool, Vector2<double>> try_normalized() const noexcept {
        double mag = magnitude();
        if (mag == 0.0) {
            return {false, Vector2<double>::zero()};
        }
        return {true, Vector2<double>::from(
            to_double(m_x) / mag,
            to_double(m_y) / mag
        )};
    }

    /// Normalize assuming non-zero (caller must check is_zero first)
    [[nodiscard]] constexpr Vector2<double> normalized_unchecked() const noexcept {
        double mag = magnitude();
        return Vector2<double>::from(
            to_double(m_x) / mag,
            to_double(m_y) / mag
        );
    }

    // ==================== Arithmetic (Checked) ====================

    /// Add vectors, returns Optional on overflow
    [[nodiscard]] constexpr Optional<Vector2> checked_add(Vector2 other) const noexcept {
        auto new_x = pulgacpp::checked_add(m_x, other.m_x);
        if (new_x.is_none()) return None;
        
        auto new_y = pulgacpp::checked_add(m_y, other.m_y);
        if (new_y.is_none()) return None;
        
        return Some(Vector2(new_x.unwrap(), new_y.unwrap()));
    }

    /// Subtract vectors
    [[nodiscard]] constexpr Optional<Vector2> checked_sub(Vector2 other) const noexcept {
        auto new_x = pulgacpp::checked_sub(m_x, other.m_x);
        if (new_x.is_none()) return None;
        
        auto new_y = pulgacpp::checked_sub(m_y, other.m_y);
        if (new_y.is_none()) return None;
        
        return Some(Vector2(new_x.unwrap(), new_y.unwrap()));
    }

    /// Scale by factor
    [[nodiscard]] constexpr Optional<Vector2> checked_scale(T factor) const noexcept {
        auto new_x = pulgacpp::checked_mul(m_x, factor);
        if (new_x.is_none()) return None;
        
        auto new_y = pulgacpp::checked_mul(m_y, factor);
        if (new_y.is_none()) return None;
        
        return Some(Vector2(new_x.unwrap(), new_y.unwrap()));
    }

    /// Negate vector
    [[nodiscard]] constexpr Vector2<double> negated() const noexcept {
        return Vector2<double>::from(-to_double(m_x), -to_double(m_y));
    }

    // ==================== Vector Operations ====================

    /// Dot product
    [[nodiscard]] constexpr double dot(Vector2 other) const noexcept {
        return to_double(m_x) * to_double(other.m_x) + 
               to_double(m_y) * to_double(other.m_y);
    }

    /// Cross product (returns z-component of 3D cross product)
    [[nodiscard]] constexpr double cross(Vector2 other) const noexcept {
        return to_double(m_x) * to_double(other.m_y) - 
               to_double(m_y) * to_double(other.m_x);
    }

    /// Angle from positive x-axis (radians)
    [[nodiscard]] constexpr double angle() const noexcept {
        return std::atan2(to_double(m_y), to_double(m_x));
    }

    /// Perpendicular vector (90° counter-clockwise)
    [[nodiscard]] constexpr Vector2<double> perpendicular() const noexcept {
        return Vector2<double>::from(-to_double(m_y), to_double(m_x));
    }

    // ==================== Rotation ====================

    /// Rotate by angle (radians)
    [[nodiscard]] constexpr Vector2<double> rotated(double angle) const noexcept {
        double cos_a = std::cos(angle);
        double sin_a = std::sin(angle);
        double x = to_double(m_x);
        double y = to_double(m_y);
        return Vector2<double>::from(
            x * cos_a - y * sin_a,
            x * sin_a + y * cos_a
        );
    }

    // ==================== Comparison ====================

    [[nodiscard]] constexpr bool operator==(const Vector2& other) const noexcept {
        return raw(m_x) == raw(other.m_x) && raw(m_y) == raw(other.m_y);
    }

    [[nodiscard]] constexpr bool operator!=(const Vector2& other) const noexcept {
        return !(*this == other);
    }

    // ==================== Conversion ====================

    /// Convert to Point
    [[nodiscard]] constexpr Point<T> to_point() const noexcept {
        return Point<T>::from(m_x, m_y);
    }

    // ==================== Stream Output ====================

    friend std::ostream& operator<<(std::ostream& os, const Vector2& v) {
        os << "Vector2(" << raw(v.m_x) << ", " << raw(v.m_y) << ")";
        return os;
    }
};

// ==================== Free Functions ====================

/// Create vector from two points (end - start)
template <Numeric T>
[[nodiscard]] constexpr Vector2<double> vector_from_points(Point<T> start, Point<T> end) noexcept {
    return Vector2<double>::from(
        to_double(end.x()) - to_double(start.x()),
        to_double(end.y()) - to_double(start.y())
    );
}

/// Normalize vector (returns Optional)
template <Numeric T>
[[nodiscard]] constexpr Optional<Vector2<double>> vec_normalized(const Vector2<T>& v) noexcept {
    double mag = v.magnitude();
    if (mag == 0.0) {
        return None;
    }
    return Some(Vector2<double>::from(
        to_double(v.x()) / mag,
        to_double(v.y()) / mag
    ));
}

/// Set magnitude of vector
template <Numeric T>
[[nodiscard]] constexpr Optional<Vector2<double>> vec_with_magnitude(const Vector2<T>& v, double new_mag) noexcept {
    auto norm = vec_normalized(v);
    if (norm.is_none()) return None;
    auto n = norm.unwrap();
    return Some(Vector2<double>::from(n.x() * new_mag, n.y() * new_mag));
}

/// Project vector a onto vector b
template <Numeric T, Numeric U>
[[nodiscard]] constexpr Optional<Vector2<double>> vec_project(const Vector2<T>& a, const Vector2<U>& b) noexcept {
    double b_mag_sq = b.magnitude_squared();
    if (b_mag_sq == 0.0) return None;
    double scalar = a.dot(b) / b_mag_sq;
    return Some(Vector2<double>::from(
        to_double(b.x()) * scalar,
        to_double(b.y()) * scalar
    ));
}

/// Reflect vector across a normal
template <Numeric T, Numeric U>
[[nodiscard]] constexpr Optional<Vector2<double>> vec_reflect(const Vector2<T>& v, const Vector2<U>& normal) noexcept {
    auto norm = vec_normalized(normal);
    if (norm.is_none()) return None;
    auto n = norm.unwrap();
    double d = 2.0 * v.dot(n);
    return Some(Vector2<double>::from(
        to_double(v.x()) - d * n.x(),
        to_double(v.y()) - d * n.y()
    ));
}

/// Angle between two vectors
template <Numeric T, Numeric U>
[[nodiscard]] constexpr Optional<double> vec_angle_between(const Vector2<T>& a, const Vector2<U>& b) noexcept {
    double mag_product = a.magnitude() * b.magnitude();
    if (mag_product == 0.0) return None;
    double cos_angle = a.dot(b) / mag_product;
    cos_angle = std::max(-1.0, std::min(1.0, cos_angle));
    return Some(std::acos(cos_angle));
}

// Type aliases
using Vec2i = Vector2<int>;
using Vec2f = Vector2<float>;
using Vec2d = Vector2<double>;

} // namespace pulgacpp

#endif // PULGACPP_GEOMETRY_VECTOR2_HPP
