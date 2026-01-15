// pulgacpp::Point - 2D point type
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_GEOMETRY_POINT_HPP
#define PULGACPP_GEOMETRY_POINT_HPP

#include "shape.hpp"
#include <cmath>

namespace pulgacpp {

/// A 2D point with coordinates (x, y)
template <Numeric T>
class Point {
public:
    using value_type = T;
    static constexpr std::string_view NAME = "Point";
    static constexpr unsigned DIMENSIONS = 2;
    static constexpr bool is_shape = true;  // Points are degenerate shapes

private:
    T m_x;
    T m_y;

    // Private constructor
    constexpr Point(T x, T y) noexcept : m_x(x), m_y(y) {}

public:
    // ==================== Construction ====================

    /// Default: origin (0, 0)
    constexpr Point() noexcept : m_x{}, m_y{} {}

    /// Factory: create from coordinates
    [[nodiscard]] static constexpr Point from(T x, T y) noexcept {
        return Point(x, y);
    }

    /// Factory: create origin point
    [[nodiscard]] static constexpr Point origin() noexcept {
        return Point{};
    }

    // ==================== Accessors ====================

    [[nodiscard]] constexpr T x() const noexcept { return m_x; }
    [[nodiscard]] constexpr T y() const noexcept { return m_y; }

    /// Set x coordinate (returns new point - immutable)
    [[nodiscard]] constexpr Point with_x(T new_x) const noexcept {
        return Point(new_x, m_y);
    }

    /// Set y coordinate (returns new point - immutable)
    [[nodiscard]] constexpr Point with_y(T new_y) const noexcept {
        return Point(m_x, new_y);
    }

    // ==================== Arithmetic (Checked) ====================

    /// Translate by (dx, dy), returns Optional on overflow
    [[nodiscard]] constexpr Optional<Point> checked_translate(T dx, T dy) const noexcept {
        auto new_x = checked_add(m_x, dx);
        if (new_x.is_none()) return None;
        
        auto new_y = checked_add(m_y, dy);
        if (new_y.is_none()) return None;
        
        return Some(Point(new_x.unwrap(), new_y.unwrap()));
    }

    /// Add another point (vector addition), returns Optional on overflow
    [[nodiscard]] constexpr Optional<Point> checked_add(Point other) const noexcept {
        return checked_translate(other.m_x, other.m_y);
    }

    /// Subtract another point, returns Optional on overflow
    [[nodiscard]] constexpr Optional<Point> checked_sub(Point other) const noexcept {
        auto new_x = pulgacpp::checked_sub(m_x, other.m_x);
        if (new_x.is_none()) return None;
        
        auto new_y = pulgacpp::checked_sub(m_y, other.m_y);
        if (new_y.is_none()) return None;
        
        return Some(Point(new_x.unwrap(), new_y.unwrap()));
    }

    /// Scale by factor, returns Optional on overflow
    [[nodiscard]] constexpr Optional<Point> checked_scale(T factor) const noexcept {
        auto new_x = pulgacpp::checked_mul(m_x, factor);
        if (new_x.is_none()) return None;
        
        auto new_y = pulgacpp::checked_mul(m_y, factor);
        if (new_y.is_none()) return None;
        
        return Some(Point(new_x.unwrap(), new_y.unwrap()));
    }

    // ==================== Distance ====================

    /// Euclidean distance to another point (always returns double)
    [[nodiscard]] constexpr double distance_to(Point other) const noexcept {
        double dx = to_double(m_x) - to_double(other.m_x);
        double dy = to_double(m_y) - to_double(other.m_y);
        return std::sqrt(dx * dx + dy * dy);
    }

    /// Squared distance (faster, avoids sqrt)
    [[nodiscard]] constexpr double distance_squared(Point other) const noexcept {
        double dx = to_double(m_x) - to_double(other.m_x);
        double dy = to_double(m_y) - to_double(other.m_y);
        return dx * dx + dy * dy;
    }

    /// Manhattan distance
    [[nodiscard]] constexpr auto manhattan_distance(Point other) const noexcept {
        auto dx = raw(m_x) - raw(other.m_x);
        auto dy = raw(m_y) - raw(other.m_y);
        return (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);
    }

    /// Distance from origin
    [[nodiscard]] constexpr double magnitude() const noexcept {
        return distance_to(Point::origin());
    }

    // ==================== Geometry ====================

    /// Midpoint between this and another point
    [[nodiscard]] constexpr Point<double> midpoint(Point other) const noexcept {
        double mx = (to_double(m_x) + to_double(other.m_x)) / 2.0;
        double my = (to_double(m_y) + to_double(other.m_y)) / 2.0;
        return Point<double>::from(mx, my);
    }

    /// Linear interpolation: this + t * (other - this)
    [[nodiscard]] constexpr Point<double> lerp(Point other, double t) const noexcept {
        double lx = to_double(m_x) + t * (to_double(other.m_x) - to_double(m_x));
        double ly = to_double(m_y) + t * (to_double(other.m_y) - to_double(m_y));
        return Point<double>::from(lx, ly);
    }

    /// Rotate around origin by angle (radians)
    [[nodiscard]] constexpr Point<double> rotate(double angle) const noexcept {
        double cos_a = std::cos(angle);
        double sin_a = std::sin(angle);
        double x = to_double(m_x);
        double y = to_double(m_y);
        return Point<double>::from(
            x * cos_a - y * sin_a,
            x * sin_a + y * cos_a
        );
    }

    /// Rotate around a center point by angle (radians)
    [[nodiscard]] constexpr Point<double> rotate_around(Point center, double angle) const noexcept {
        double dx = to_double(m_x) - to_double(center.m_x);
        double dy = to_double(m_y) - to_double(center.m_y);
        double cos_a = std::cos(angle);
        double sin_a = std::sin(angle);
        return Point<double>::from(
            to_double(center.m_x) + dx * cos_a - dy * sin_a,
            to_double(center.m_y) + dx * sin_a + dy * cos_a
        );
    }

    // ==================== Comparison ====================

    [[nodiscard]] constexpr bool operator==(const Point& other) const noexcept {
        return raw(m_x) == raw(other.m_x) && raw(m_y) == raw(other.m_y);
    }

    [[nodiscard]] constexpr bool operator!=(const Point& other) const noexcept {
        return !(*this == other);
    }

    // ==================== Stream Output ====================

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "Point(" << raw(p.m_x) << ", " << raw(p.m_y) << ")";
    }
};

// Type aliases for common use cases
using Point32 = Point<std::int32_t>;
using Point64 = Point<std::int64_t>;
using PointF = Point<float>;
using PointD = Point<double>;

} // namespace pulgacpp

#endif // PULGACPP_GEOMETRY_POINT_HPP
