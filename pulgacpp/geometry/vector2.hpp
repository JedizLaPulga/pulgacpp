// pulgacpp::Vector2 - 2D vector type
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_GEOMETRY_VECTOR2_HPP
#define PULGACPP_GEOMETRY_VECTOR2_HPP

#include "shape.hpp"
#include "point.hpp"
#include <cmath>

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

    /// Factory: unit vector in X direction
    [[nodiscard]] static constexpr Vector2<double> unit_x() noexcept {
        return Vector2<double>::from(1.0, 0.0);
    }

    /// Factory: unit vector in Y direction
    [[nodiscard]] static constexpr Vector2<double> unit_y() noexcept {
        return Vector2<double>::from(0.0, 1.0);
    }

    /// Factory: from two points (end - start)
    [[nodiscard]] static constexpr Vector2<double> from_points(Point<T> start, Point<T> end) noexcept {
        return Vector2<double>::from(
            to_double(end.x()) - to_double(start.x()),
            to_double(end.y()) - to_double(start.y())
        );
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

    /// Normalize to unit vector, returns None if zero vector
    [[nodiscard]] constexpr Optional<Vector2<double>> normalized() const noexcept {
        double mag = magnitude();
        if (mag == 0.0) {
            return None;
        }
        return Some(Vector2<double>::from(
            to_double(m_x) / mag,
            to_double(m_y) / mag
        ));
    }

    /// Set magnitude to given value
    [[nodiscard]] constexpr Optional<Vector2<double>> with_magnitude(double new_mag) const noexcept {
        auto norm = normalized();
        if (norm.is_none()) return None;
        auto n = norm.unwrap();
        return Some(Vector2<double>::from(n.x() * new_mag, n.y() * new_mag));
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

    /// Angle between vectors (radians)
    [[nodiscard]] constexpr Optional<double> angle_to(Vector2 other) const noexcept {
        double mag_product = magnitude() * other.magnitude();
        if (mag_product == 0.0) return None;
        double cos_angle = dot(other) / mag_product;
        // Clamp to [-1, 1] to handle floating point errors
        cos_angle = std::max(-1.0, std::min(1.0, cos_angle));
        return Some(std::acos(cos_angle));
    }

    /// Angle from positive x-axis (radians)
    [[nodiscard]] constexpr double angle() const noexcept {
        return std::atan2(to_double(m_y), to_double(m_x));
    }

    /// Project this vector onto another
    [[nodiscard]] constexpr Optional<Vector2<double>> project_onto(Vector2 other) const noexcept {
        double other_mag_sq = other.magnitude_squared();
        if (other_mag_sq == 0.0) return None;
        double scalar = dot(other) / other_mag_sq;
        return Some(Vector2<double>::from(
            to_double(other.m_x) * scalar,
            to_double(other.m_y) * scalar
        ));
    }

    /// Perpendicular vector (90° counter-clockwise)
    [[nodiscard]] constexpr Vector2<double> perpendicular() const noexcept {
        return Vector2<double>::from(-to_double(m_y), to_double(m_x));
    }

    /// Reflect across a normal vector
    [[nodiscard]] constexpr Optional<Vector2<double>> reflect(Vector2 normal) const noexcept {
        auto norm = normal.normalized();
        if (norm.is_none()) return None;
        auto n = norm.unwrap();
        double d = 2.0 * dot(Vector2<double>::from(n.x(), n.y()));
        return Some(Vector2<double>::from(
            to_double(m_x) - d * n.x(),
            to_double(m_y) - d * n.y()
        ));
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
        return os << "Vector2(" << raw(v.m_x) << ", " << raw(v.m_y) << ")";
    }
};

// Type aliases
using Vec2i = Vector2<int>;
using Vec2f = Vector2<float>;
using Vec2d = Vector2<double>;

} // namespace pulgacpp

#endif // PULGACPP_GEOMETRY_VECTOR2_HPP
