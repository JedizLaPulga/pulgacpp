// pulgacpp::Rectangle - Axis-aligned 2D rectangle
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_GEOMETRY_RECTANGLE_HPP
#define PULGACPP_GEOMETRY_RECTANGLE_HPP

#include "shape.hpp"
#include "point.hpp"
#include <algorithm>
#include <array>

namespace pulgacpp {

/// An axis-aligned rectangle defined by min corner and dimensions
template <Numeric T>
class Rectangle : public Shape2D<Rectangle<T>, T> {
public:
    using value_type = T;
    static constexpr std::string_view NAME = "Rectangle";

private:
    Point<T> m_min;   // Bottom-left corner (minimum x,y)
    T m_width;
    T m_height;

    constexpr Rectangle(Point<T> min, T width, T height) noexcept 
        : m_min(min), m_width(width), m_height(height) {}

public:
    // ==================== Construction ====================

    /// Default: unit square at origin
    constexpr Rectangle() noexcept : m_min{}, m_width{}, m_height{} {
        if constexpr (std::is_arithmetic_v<T>) {
            m_width = static_cast<T>(1);
            m_height = static_cast<T>(1);
        }
    }

    /// Factory: create from min corner and dimensions
    /// Returns None if width or height is negative
    [[nodiscard]] static constexpr Optional<Rectangle> from_corner(
        Point<T> min_corner, T width, T height) noexcept 
    {
        if (raw(width) < 0 || raw(height) < 0) {
            return None;
        }
        return Some(Rectangle(min_corner, width, height));
    }

    /// Factory: create from two opposite corners
    [[nodiscard]] static constexpr Rectangle from_corners(Point<T> p1, Point<T> p2) noexcept {
        auto x1 = raw(p1.x()), y1 = raw(p1.y());
        auto x2 = raw(p2.x()), y2 = raw(p2.y());
        
        auto min_x = std::min(x1, x2);
        auto min_y = std::min(y1, y2);
        auto max_x = std::max(x1, x2);
        auto max_y = std::max(y1, y2);
        
        if constexpr (SafeNumeric<T>) {
            return Rectangle(
                Point<T>::from(T(static_cast<typename T::underlying_type>(min_x)), 
                               T(static_cast<typename T::underlying_type>(min_y))),
                T(static_cast<typename T::underlying_type>(max_x - min_x)),
                T(static_cast<typename T::underlying_type>(max_y - min_y))
            );
        } else {
            return Rectangle(
                Point<T>::from(static_cast<T>(min_x), static_cast<T>(min_y)),
                static_cast<T>(max_x - min_x),
                static_cast<T>(max_y - min_y)
            );
        }
    }

    /// Factory: create from center and dimensions
    [[nodiscard]] static constexpr Optional<Rectangle<double>> from_center(
        Point<T> center, T width, T height) noexcept 
    {
        if (raw(width) < 0 || raw(height) < 0) {
            return None;
        }
        double half_w = to_double(width) / 2.0;
        double half_h = to_double(height) / 2.0;
        auto min = Point<double>::from(
            to_double(center.x()) - half_w,
            to_double(center.y()) - half_h
        );
        return Rectangle<double>::from_corner(min, to_double(width), to_double(height));
    }

    /// Factory: unit square at origin
    [[nodiscard]] static constexpr Rectangle unit() noexcept {
        if constexpr (std::is_arithmetic_v<T>) {
            return Rectangle(Point<T>::origin(), static_cast<T>(1), static_cast<T>(1));
        } else {
            return Rectangle{};
        }
    }

    // ==================== Accessors ====================

    [[nodiscard]] constexpr T width() const noexcept { return m_width; }
    [[nodiscard]] constexpr T height() const noexcept { return m_height; }

    [[nodiscard]] constexpr Point<T> min_corner() const noexcept { return m_min; }
    
    [[nodiscard]] constexpr Point<double> max_corner() const noexcept {
        return Point<double>::from(
            to_double(m_min.x()) + to_double(m_width),
            to_double(m_min.y()) + to_double(m_height)
        );
    }

    [[nodiscard]] constexpr Point<double> center() const noexcept {
        return Point<double>::from(
            to_double(m_min.x()) + to_double(m_width) / 2.0,
            to_double(m_min.y()) + to_double(m_height) / 2.0
        );
    }

    /// Left edge x-coordinate
    [[nodiscard]] constexpr T left() const noexcept { return m_min.x(); }
    
    /// Right edge x-coordinate
    [[nodiscard]] constexpr double right() const noexcept { 
        return to_double(m_min.x()) + to_double(m_width); 
    }
    
    /// Bottom edge y-coordinate
    [[nodiscard]] constexpr T bottom() const noexcept { return m_min.y(); }
    
    /// Top edge y-coordinate
    [[nodiscard]] constexpr double top() const noexcept { 
        return to_double(m_min.y()) + to_double(m_height); 
    }

    /// Get all four corners [bottom-left, bottom-right, top-right, top-left]
    [[nodiscard]] constexpr std::array<Point<double>, 4> corners() const noexcept {
        double x1 = to_double(m_min.x());
        double y1 = to_double(m_min.y());
        double x2 = x1 + to_double(m_width);
        double y2 = y1 + to_double(m_height);
        return {
            Point<double>::from(x1, y1),  // bottom-left
            Point<double>::from(x2, y1),  // bottom-right
            Point<double>::from(x2, y2),  // top-right
            Point<double>::from(x1, y2)   // top-left
        };
    }

    // ==================== Geometry (Shape2D interface) ====================

    /// Area = width * height
    [[nodiscard]] constexpr double area_impl() const noexcept {
        return to_double(m_width) * to_double(m_height);
    }

    /// Perimeter = 2 * (width + height)
    [[nodiscard]] constexpr double perimeter_impl() const noexcept {
        return 2.0 * (to_double(m_width) + to_double(m_height));
    }

    /// Diagonal length
    [[nodiscard]] constexpr double diagonal() const noexcept {
        double w = to_double(m_width);
        double h = to_double(m_height);
        return std::sqrt(w * w + h * h);
    }

    /// Aspect ratio (width / height)
    [[nodiscard]] constexpr Optional<double> aspect_ratio() const noexcept {
        if (raw(m_height) == 0) return None;
        return Some(to_double(m_width) / to_double(m_height));
    }

    /// Check if square
    [[nodiscard]] constexpr bool is_square() const noexcept {
        return raw(m_width) == raw(m_height);
    }

    // ==================== Containment ====================

    /// Check if point is inside or on the rectangle
    [[nodiscard]] constexpr bool contains(Point<T> p) const noexcept {
        double px = to_double(p.x());
        double py = to_double(p.y());
        double x1 = to_double(m_min.x());
        double y1 = to_double(m_min.y());
        double x2 = x1 + to_double(m_width);
        double y2 = y1 + to_double(m_height);
        return px >= x1 && px <= x2 && py >= y1 && py <= y2;
    }

    /// Check if point is strictly inside
    [[nodiscard]] constexpr bool strictly_contains(Point<T> p) const noexcept {
        double px = to_double(p.x());
        double py = to_double(p.y());
        double x1 = to_double(m_min.x());
        double y1 = to_double(m_min.y());
        double x2 = x1 + to_double(m_width);
        double y2 = y1 + to_double(m_height);
        return px > x1 && px < x2 && py > y1 && py < y2;
    }

    // ==================== Intersection ====================

    /// Check if this rectangle intersects another
    [[nodiscard]] constexpr bool intersects(Rectangle other) const noexcept {
        double ax1 = to_double(m_min.x());
        double ay1 = to_double(m_min.y());
        double ax2 = ax1 + to_double(m_width);
        double ay2 = ay1 + to_double(m_height);
        
        double bx1 = to_double(other.m_min.x());
        double by1 = to_double(other.m_min.y());
        double bx2 = bx1 + to_double(other.m_width);
        double by2 = by1 + to_double(other.m_height);
        
        return ax1 <= bx2 && ax2 >= bx1 && ay1 <= by2 && ay2 >= by1;
    }

    /// Get intersection rectangle, returns None if no intersection
    [[nodiscard]] constexpr Optional<Rectangle<double>> intersection(Rectangle other) const noexcept {
        double ax1 = to_double(m_min.x());
        double ay1 = to_double(m_min.y());
        double ax2 = ax1 + to_double(m_width);
        double ay2 = ay1 + to_double(m_height);
        
        double bx1 = to_double(other.m_min.x());
        double by1 = to_double(other.m_min.y());
        double bx2 = bx1 + to_double(other.m_width);
        double by2 = by1 + to_double(other.m_height);
        
        double ix1 = std::max(ax1, bx1);
        double iy1 = std::max(ay1, by1);
        double ix2 = std::min(ax2, bx2);
        double iy2 = std::min(ay2, by2);
        
        if (ix1 > ix2 || iy1 > iy2) {
            return None;
        }
        
        return Rectangle<double>::from_corner(
            Point<double>::from(ix1, iy1),
            ix2 - ix1,
            iy2 - iy1
        );
    }

    /// Check if this rectangle contains another entirely
    [[nodiscard]] constexpr bool contains_rect(Rectangle other) const noexcept {
        double ax1 = to_double(m_min.x());
        double ay1 = to_double(m_min.y());
        double ax2 = ax1 + to_double(m_width);
        double ay2 = ay1 + to_double(m_height);
        
        double bx1 = to_double(other.m_min.x());
        double by1 = to_double(other.m_min.y());
        double bx2 = bx1 + to_double(other.m_width);
        double by2 = by1 + to_double(other.m_height);
        
        return ax1 <= bx1 && ay1 <= by1 && ax2 >= bx2 && ay2 >= by2;
    }

    // ==================== Transformations ====================

    /// Move rectangle by delta
    [[nodiscard]] constexpr Optional<Rectangle> checked_translate(T dx, T dy) const noexcept {
        auto new_min = m_min.checked_translate(dx, dy);
        if (new_min.is_none()) return None;
        return Some(Rectangle(new_min.unwrap(), m_width, m_height));
    }

    /// Scale dimensions by factor
    [[nodiscard]] constexpr Optional<Rectangle> checked_scale(T factor) const noexcept {
        if (raw(factor) < 0) return None;
        auto new_w = pulgacpp::checked_mul(m_width, factor);
        if (new_w.is_none()) return None;
        auto new_h = pulgacpp::checked_mul(m_height, factor);
        if (new_h.is_none()) return None;
        return Some(Rectangle(m_min, new_w.unwrap(), new_h.unwrap()));
    }

    /// Expand rectangle by amount on all sides
    [[nodiscard]] constexpr Optional<Rectangle<double>> expanded(double amount) const noexcept {
        double new_w = to_double(m_width) + 2.0 * amount;
        double new_h = to_double(m_height) + 2.0 * amount;
        if (new_w < 0 || new_h < 0) return None;
        return Rectangle<double>::from_corner(
            Point<double>::from(
                to_double(m_min.x()) - amount,
                to_double(m_min.y()) - amount
            ),
            new_w, new_h
        );
    }

    // ==================== Comparison ====================

    [[nodiscard]] constexpr bool operator==(const Rectangle& other) const noexcept {
        return m_min == other.m_min && 
               raw(m_width) == raw(other.m_width) && 
               raw(m_height) == raw(other.m_height);
    }

    [[nodiscard]] constexpr bool operator!=(const Rectangle& other) const noexcept {
        return !(*this == other);
    }

    // ==================== Stream Output ====================

    friend std::ostream& operator<<(std::ostream& os, const Rectangle& r) {
        return os << "Rectangle(min=" << r.m_min 
                  << ", width=" << raw(r.m_width) 
                  << ", height=" << raw(r.m_height) << ")";
    }
};

// Type aliases
using Rect32 = Rectangle<std::int32_t>;
using Rect64 = Rectangle<std::int64_t>;
using RectF = Rectangle<float>;
using RectD = Rectangle<double>;

} // namespace pulgacpp

#endif // PULGACPP_GEOMETRY_RECTANGLE_HPP
