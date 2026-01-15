// pulgacpp::Circle - 2D circle shape
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_GEOMETRY_CIRCLE_HPP
#define PULGACPP_GEOMETRY_CIRCLE_HPP

#include "shape.hpp"
#include "point.hpp"
#include <cmath>

namespace pulgacpp {

// Forward declaration
template <Numeric T> class Rectangle;

/// A circle defined by center point and radius
template <Numeric T>
class Circle : public Shape2D<Circle<T>, T> {
public:
    using value_type = T;
    static constexpr std::string_view NAME = "Circle";

private:
    Point<T> m_center;
    T m_radius;

    constexpr Circle(Point<T> center, T radius) noexcept 
        : m_center(center), m_radius(radius) {}

public:
    // ==================== Construction ====================

    /// Default: unit circle at origin
    constexpr Circle() noexcept : m_center{}, m_radius{} {
        if constexpr (std::is_arithmetic_v<T>) {
            m_radius = static_cast<T>(1);
        }
    }

    /// Factory: create from center and radius
    /// Returns None if radius is negative
    [[nodiscard]] static constexpr Optional<Circle> from(Point<T> center, T radius) noexcept {
        if (raw(radius) < 0) {
            return None;
        }
        return Some(Circle(center, radius));
    }

    /// Factory: create unit circle at origin
    [[nodiscard]] static constexpr Circle unit() noexcept {
        if constexpr (std::is_arithmetic_v<T>) {
            return Circle(Point<T>::origin(), static_cast<T>(1));
        } else {
            // For pulgacpp types, caller must provide proper value
            return Circle{};
        }
    }

    /// Factory: create from three points on circumference
    [[nodiscard]] static constexpr Optional<Circle<double>> from_points(
        Point<T> p1, Point<T> p2, Point<T> p3) noexcept 
    {
        // Calculate circumcenter using determinants
        double ax = to_double(p1.x()), ay = to_double(p1.y());
        double bx = to_double(p2.x()), by = to_double(p2.y());
        double cx = to_double(p3.x()), cy = to_double(p3.y());

        double d = 2.0 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
        if (std::abs(d) < 1e-10) {
            return None;  // Points are collinear
        }

        double ux = ((ax * ax + ay * ay) * (by - cy) + 
                     (bx * bx + by * by) * (cy - ay) + 
                     (cx * cx + cy * cy) * (ay - by)) / d;
        double uy = ((ax * ax + ay * ay) * (cx - bx) + 
                     (bx * bx + by * by) * (ax - cx) + 
                     (cx * cx + cy * cy) * (bx - ax)) / d;

        auto center = Point<double>::from(ux, uy);
        double radius = center.distance_to(Point<double>::from(ax, ay));
        
        return Circle<double>::from(center, radius);
    }

    // ==================== Accessors ====================

    [[nodiscard]] constexpr Point<T> center() const noexcept { return m_center; }
    [[nodiscard]] constexpr T radius() const noexcept { return m_radius; }

    /// Diameter (2 * radius)
    [[nodiscard]] constexpr double diameter() const noexcept {
        return 2.0 * to_double(m_radius);
    }

    // ==================== Geometry (Shape2D interface) ====================

    /// Area = π * r²
    [[nodiscard]] constexpr double area_impl() const noexcept {
        double r = to_double(m_radius);
        return constants::PI * r * r;
    }

    /// Circumference = 2 * π * r
    [[nodiscard]] constexpr double perimeter_impl() const noexcept {
        return constants::TAU * to_double(m_radius);
    }

    /// Alias for perimeter
    [[nodiscard]] constexpr double circumference() const noexcept {
        return this->perimeter();
    }

    // ==================== Containment ====================

    /// Check if point is inside or on the circle
    [[nodiscard]] constexpr bool contains(Point<T> p) const noexcept {
        double dist_sq = m_center.distance_squared(p);
        double r = to_double(m_radius);
        return dist_sq <= r * r;
    }

    /// Check if point is strictly inside (not on boundary)
    [[nodiscard]] constexpr bool strictly_contains(Point<T> p) const noexcept {
        double dist_sq = m_center.distance_squared(p);
        double r = to_double(m_radius);
        return dist_sq < r * r;
    }

    /// Check if point is on the boundary (within tolerance)
    [[nodiscard]] constexpr bool on_boundary(Point<T> p, double tolerance = 1e-9) const noexcept {
        double dist = m_center.distance_to(p);
        double r = to_double(m_radius);
        return std::abs(dist - r) <= tolerance;
    }

    // ==================== Intersection ====================

    /// Check if this circle intersects another
    [[nodiscard]] constexpr bool intersects(Circle other) const noexcept {
        double dist = m_center.distance_to(other.m_center);
        double r1 = to_double(m_radius);
        double r2 = to_double(other.m_radius);
        // Intersects if distance between centers <= sum of radii
        // and >= absolute difference of radii (not one inside the other without touching)
        return dist <= r1 + r2 && dist >= std::abs(r1 - r2);
    }

    /// Check if this circle overlaps another (includes one containing the other)
    [[nodiscard]] constexpr bool overlaps(Circle other) const noexcept {
        double dist = m_center.distance_to(other.m_center);
        double r1 = to_double(m_radius);
        double r2 = to_double(other.m_radius);
        return dist < r1 + r2;
    }

    /// Check if this circle contains another entirely
    [[nodiscard]] constexpr bool contains_circle(Circle other) const noexcept {
        double dist = m_center.distance_to(other.m_center);
        double r1 = to_double(m_radius);
        double r2 = to_double(other.m_radius);
        return dist + r2 <= r1;
    }

    // ==================== Transformations ====================

    /// Move circle by delta
    [[nodiscard]] constexpr Optional<Circle> checked_translate(T dx, T dy) const noexcept {
        auto new_center = m_center.checked_translate(dx, dy);
        if (new_center.is_none()) return None;
        return Some(Circle(new_center.unwrap(), m_radius));
    }

    /// Scale radius by factor
    [[nodiscard]] constexpr Optional<Circle> checked_scale(T factor) const noexcept {
        if (raw(factor) < 0) return None;
        auto new_radius = pulgacpp::checked_mul(m_radius, factor);
        if (new_radius.is_none()) return None;
        return Some(Circle(m_center, new_radius.unwrap()));
    }

    /// Move center to new location
    [[nodiscard]] constexpr Circle with_center(Point<T> new_center) const noexcept {
        return Circle(new_center, m_radius);
    }

    /// Set new radius
    [[nodiscard]] constexpr Optional<Circle> with_radius(T new_radius) const noexcept {
        if (raw(new_radius) < 0) return None;
        return Some(Circle(m_center, new_radius));
    }

    // ==================== Special Points ====================

    /// Point on circle at given angle (radians)
    [[nodiscard]] constexpr Point<double> point_at_angle(double angle) const noexcept {
        double r = to_double(m_radius);
        return Point<double>::from(
            to_double(m_center.x()) + r * std::cos(angle),
            to_double(m_center.y()) + r * std::sin(angle)
        );
    }

    /// Northernmost point (top)
    [[nodiscard]] constexpr Point<double> top() const noexcept {
        return point_at_angle(constants::PI / 2.0);
    }

    /// Southernmost point (bottom)
    [[nodiscard]] constexpr Point<double> bottom() const noexcept {
        return point_at_angle(-constants::PI / 2.0);
    }

    /// Easternmost point (right)
    [[nodiscard]] constexpr Point<double> right() const noexcept {
        return point_at_angle(0.0);
    }

    /// Westernmost point (left)
    [[nodiscard]] constexpr Point<double> left() const noexcept {
        return point_at_angle(constants::PI);
    }

    // ==================== Comparison ====================

    [[nodiscard]] constexpr bool operator==(const Circle& other) const noexcept {
        return m_center == other.m_center && raw(m_radius) == raw(other.m_radius);
    }

    [[nodiscard]] constexpr bool operator!=(const Circle& other) const noexcept {
        return !(*this == other);
    }

    // ==================== Stream Output ====================

    friend std::ostream& operator<<(std::ostream& os, const Circle& c) {
        return os << "Circle(center=" << c.m_center 
                  << ", radius=" << raw(c.m_radius) << ")";
    }
};

// Type aliases
using Circle32 = Circle<std::int32_t>;
using Circle64 = Circle<std::int64_t>;
using CircleF = Circle<float>;
using CircleD = Circle<double>;

} // namespace pulgacpp

#endif // PULGACPP_GEOMETRY_CIRCLE_HPP
