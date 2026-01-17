// pulgacpp::Box - 3D axis-aligned box type
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_GEOMETRY_BOX_HPP
#define PULGACPP_GEOMETRY_BOX_HPP

#include "shape.hpp"
#include "vector3.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <ostream>


namespace pulgacpp {

// Forward declaration
template <Numeric T> class Sphere;

/// A 3D axis-aligned bounding box (AABB)
template <Numeric T> class Box {
public:
  using value_type = T;
  static constexpr std::string_view NAME = "Box";
  static constexpr unsigned DIMENSIONS = 3;
  static constexpr bool is_shape = true;

private:
  Vector3<T> m_min; // Minimum corner
  Vector3<T> m_max; // Maximum corner

  constexpr Box(Vector3<T> min_corner, Vector3<T> max_corner) noexcept
      : m_min(min_corner), m_max(max_corner) {}

public:
  // ==================== Construction ====================

  /// Create box from min and max corners (None if invalid)
  [[nodiscard]] static constexpr Optional<Box>
  from_corners(Vector3<T> min_corner, Vector3<T> max_corner) noexcept {
    if (raw(min_corner.x()) > raw(max_corner.x()) ||
        raw(min_corner.y()) > raw(max_corner.y()) ||
        raw(min_corner.z()) > raw(max_corner.z())) {
      return None;
    }
    return Some(Box(min_corner, max_corner));
  }

  /// Create box from any two opposite corners (auto-sorts)
  [[nodiscard]] static constexpr Box from_points(Vector3<T> a,
                                                 Vector3<T> b) noexcept {
    T min_x = raw(a.x()) < raw(b.x()) ? a.x() : b.x();
    T min_y = raw(a.y()) < raw(b.y()) ? a.y() : b.y();
    T min_z = raw(a.z()) < raw(b.z()) ? a.z() : b.z();
    T max_x = raw(a.x()) > raw(b.x()) ? a.x() : b.x();
    T max_y = raw(a.y()) > raw(b.y()) ? a.y() : b.y();
    T max_z = raw(a.z()) > raw(b.z()) ? a.z() : b.z();
    return Box(Vector3<T>::from(min_x, min_y, min_z),
               Vector3<T>::from(max_x, max_y, max_z));
  }

  /// Create box from center and half-extents
  [[nodiscard]] static constexpr Optional<Box>
  from_center_extents(Vector3<T> center, T half_width, T half_height,
                      T half_depth) noexcept {
    if (raw(half_width) < 0 || raw(half_height) < 0 || raw(half_depth) < 0) {
      return None;
    }
    return Some(
        Box(Vector3<T>::from(center.x() - half_width, center.y() - half_height,
                             center.z() - half_depth),
            Vector3<T>::from(center.x() + half_width, center.y() + half_height,
                             center.z() + half_depth)));
  }

  /// Create cube from center and half-size
  [[nodiscard]] static constexpr Optional<Box> cube(Vector3<T> center,
                                                    T half_size) noexcept {
    return from_center_extents(center, half_size, half_size, half_size);
  }

  /// Create unit box (0,0,0 to 1,1,1)
  [[nodiscard]] static constexpr Box unit() noexcept {
    if constexpr (std::is_floating_point_v<T>) {
      return Box(Vector3<T>::zero(), Vector3<T>::from(T(1), T(1), T(1)));
    } else {
      return Box(Vector3<T>::zero(), Vector3<T>::from(T(1), T(1), T(1)));
    }
  }

  // ==================== Accessors ====================

  [[nodiscard]] constexpr Vector3<T> min() const noexcept { return m_min; }
  [[nodiscard]] constexpr Vector3<T> max() const noexcept { return m_max; }
  [[nodiscard]] constexpr Vector3<T> min_corner() const noexcept {
    return m_min;
  }
  [[nodiscard]] constexpr Vector3<T> max_corner() const noexcept {
    return m_max;
  }

  /// Get dimensions
  [[nodiscard]] constexpr double width() const noexcept {
    return to_double(m_max.x()) - to_double(m_min.x());
  }
  [[nodiscard]] constexpr double height() const noexcept {
    return to_double(m_max.y()) - to_double(m_min.y());
  }
  [[nodiscard]] constexpr double depth() const noexcept {
    return to_double(m_max.z()) - to_double(m_min.z());
  }

  /// Get half-extents
  [[nodiscard]] constexpr Vector3<double> half_extents() const noexcept {
    return Vector3<double>::from(width() / 2.0, height() / 2.0, depth() / 2.0);
  }

  /// Get center
  [[nodiscard]] constexpr Vector3<double> center() const noexcept {
    return Vector3<double>::from(
        (to_double(m_min.x()) + to_double(m_max.x())) / 2.0,
        (to_double(m_min.y()) + to_double(m_max.y())) / 2.0,
        (to_double(m_min.z()) + to_double(m_max.z())) / 2.0);
  }

  /// Get size as vector
  [[nodiscard]] constexpr Vector3<double> size() const noexcept {
    return Vector3<double>::from(width(), height(), depth());
  }

  // ==================== Geometry ====================

  /// Volume
  [[nodiscard]] constexpr double volume() const noexcept {
    return width() * height() * depth();
  }

  /// Surface area
  [[nodiscard]] constexpr double surface_area() const noexcept {
    double w = width(), h = height(), d = depth();
    return 2.0 * (w * h + h * d + d * w);
  }

  /// Diagonal length
  [[nodiscard]] constexpr double diagonal() const noexcept {
    double w = width(), h = height(), d = depth();
    return std::sqrt(w * w + h * h + d * d);
  }

  /// Check if box is a cube (all sides equal)
  [[nodiscard]] constexpr bool is_cube(double tolerance = 1e-9) const noexcept {
    double w = width(), h = height(), d = depth();
    return std::abs(w - h) < tolerance && std::abs(h - d) < tolerance;
  }

  // ==================== Corners ====================

  /// Get all 8 corners
  [[nodiscard]] constexpr std::array<Vector3<T>, 8> corners() const noexcept {
    return {{
        Vector3<T>::from(m_min.x(), m_min.y(), m_min.z()), // 0: ---
        Vector3<T>::from(m_max.x(), m_min.y(), m_min.z()), // 1: +--
        Vector3<T>::from(m_min.x(), m_max.y(), m_min.z()), // 2: -+-
        Vector3<T>::from(m_max.x(), m_max.y(), m_min.z()), // 3: ++-
        Vector3<T>::from(m_min.x(), m_min.y(), m_max.z()), // 4: --+
        Vector3<T>::from(m_max.x(), m_min.y(), m_max.z()), // 5: +-+
        Vector3<T>::from(m_min.x(), m_max.y(), m_max.z()), // 6: -++
        Vector3<T>::from(m_max.x(), m_max.y(), m_max.z()), // 7: +++
    }};
  }

  /// Get specific corner by index (0-7)
  [[nodiscard]] constexpr Vector3<T> corner(unsigned index) const noexcept {
    return Vector3<T>::from((index & 1) ? m_max.x() : m_min.x(),
                            (index & 2) ? m_max.y() : m_min.y(),
                            (index & 4) ? m_max.z() : m_min.z());
  }

  // ==================== Containment ====================

  /// Check if point is inside or on box
  [[nodiscard]] constexpr bool contains(Vector3<T> point) const noexcept {
    return raw(point.x()) >= raw(m_min.x()) &&
           raw(point.x()) <= raw(m_max.x()) &&
           raw(point.y()) >= raw(m_min.y()) &&
           raw(point.y()) <= raw(m_max.y()) &&
           raw(point.z()) >= raw(m_min.z()) && raw(point.z()) <= raw(m_max.z());
  }

  /// Check if point is strictly inside
  [[nodiscard]] constexpr bool
  strictly_contains(Vector3<T> point) const noexcept {
    return raw(point.x()) > raw(m_min.x()) && raw(point.x()) < raw(m_max.x()) &&
           raw(point.y()) > raw(m_min.y()) && raw(point.y()) < raw(m_max.y()) &&
           raw(point.z()) > raw(m_min.z()) && raw(point.z()) < raw(m_max.z());
  }

  /// Check if this box completely contains another
  [[nodiscard]] constexpr bool contains_box(const Box &other) const noexcept {
    return contains(other.m_min) && contains(other.m_max);
  }

  /// Closest point on box surface to external point
  [[nodiscard]] constexpr Vector3<double>
  closest_point_to(Vector3<T> point) const noexcept {
    auto clamp = [](double val, double min_v, double max_v) {
      if (val < min_v)
        return min_v;
      if (val > max_v)
        return max_v;
      return val;
    };

    return Vector3<double>::from(
        clamp(to_double(point.x()), to_double(m_min.x()), to_double(m_max.x())),
        clamp(to_double(point.y()), to_double(m_min.y()), to_double(m_max.y())),
        clamp(to_double(point.z()), to_double(m_min.z()),
              to_double(m_max.z())));
  }

  /// Distance from point to box (0 if inside)
  [[nodiscard]] constexpr double
  distance_to_point(Vector3<T> point) const noexcept {
    auto closest = closest_point_to(point);
    double dx = to_double(point.x()) - closest.x();
    double dy = to_double(point.y()) - closest.y();
    double dz = to_double(point.z()) - closest.z();
    return std::sqrt(dx * dx + dy * dy + dz * dz);
  }

  // ==================== Box Relationships ====================

  /// Check if boxes intersect
  [[nodiscard]] constexpr bool intersects(const Box &other) const noexcept {
    return raw(m_min.x()) <= raw(other.m_max.x()) &&
           raw(m_max.x()) >= raw(other.m_min.x()) &&
           raw(m_min.y()) <= raw(other.m_max.y()) &&
           raw(m_max.y()) >= raw(other.m_min.y()) &&
           raw(m_min.z()) <= raw(other.m_max.z()) &&
           raw(m_max.z()) >= raw(other.m_min.z());
  }

  /// Get intersection of two boxes (None if no overlap)
  [[nodiscard]] constexpr Optional<Box>
  intersection(const Box &other) const noexcept {
    if (!intersects(other))
      return None;

    T min_x =
        raw(m_min.x()) > raw(other.m_min.x()) ? m_min.x() : other.m_min.x();
    T min_y =
        raw(m_min.y()) > raw(other.m_min.y()) ? m_min.y() : other.m_min.y();
    T min_z =
        raw(m_min.z()) > raw(other.m_min.z()) ? m_min.z() : other.m_min.z();
    T max_x =
        raw(m_max.x()) < raw(other.m_max.x()) ? m_max.x() : other.m_max.x();
    T max_y =
        raw(m_max.y()) < raw(other.m_max.y()) ? m_max.y() : other.m_max.y();
    T max_z =
        raw(m_max.z()) < raw(other.m_max.z()) ? m_max.z() : other.m_max.z();

    return Some(Box(Vector3<T>::from(min_x, min_y, min_z),
                    Vector3<T>::from(max_x, max_y, max_z)));
  }

  /// Get bounding box that contains both boxes (union)
  [[nodiscard]] constexpr Box merged_with(const Box &other) const noexcept {
    T min_x =
        raw(m_min.x()) < raw(other.m_min.x()) ? m_min.x() : other.m_min.x();
    T min_y =
        raw(m_min.y()) < raw(other.m_min.y()) ? m_min.y() : other.m_min.y();
    T min_z =
        raw(m_min.z()) < raw(other.m_min.z()) ? m_min.z() : other.m_min.z();
    T max_x =
        raw(m_max.x()) > raw(other.m_max.x()) ? m_max.x() : other.m_max.x();
    T max_y =
        raw(m_max.y()) > raw(other.m_max.y()) ? m_max.y() : other.m_max.y();
    T max_z =
        raw(m_max.z()) > raw(other.m_max.z()) ? m_max.z() : other.m_max.z();

    return Box(Vector3<T>::from(min_x, min_y, min_z),
               Vector3<T>::from(max_x, max_y, max_z));
  }

  // ==================== Transformations ====================

  /// Translate box
  [[nodiscard]] constexpr Optional<Box> checked_translate(T dx, T dy,
                                                          T dz) const noexcept {
    auto new_min = m_min.checked_add(Vector3<T>::from(dx, dy, dz));
    if (new_min.is_none())
      return None;

    auto new_max = m_max.checked_add(Vector3<T>::from(dx, dy, dz));
    if (new_max.is_none())
      return None;

    return Some(Box(new_min.unwrap(), new_max.unwrap()));
  }

  /// Scale box from center
  [[nodiscard]] constexpr Optional<Box> scaled(double factor) const noexcept {
    if (factor < 0)
      return None;

    auto c = center();
    double hw = width() / 2.0 * factor;
    double hh = height() / 2.0 * factor;
    double hd = depth() / 2.0 * factor;

    return Some(Box(
        Vector3<T>::from(static_cast<T>(c.x() - hw), static_cast<T>(c.y() - hh),
                         static_cast<T>(c.z() - hd)),
        Vector3<T>::from(static_cast<T>(c.x() + hw), static_cast<T>(c.y() + hh),
                         static_cast<T>(c.z() + hd))));
  }

  /// Expand box by delta in all directions
  [[nodiscard]] constexpr Optional<Box> expanded(double delta) const noexcept {
    double new_width = width() + 2 * delta;
    double new_height = height() + 2 * delta;
    double new_depth = depth() + 2 * delta;

    if (new_width < 0 || new_height < 0 || new_depth < 0)
      return None;

    return Some(
        Box(Vector3<T>::from(static_cast<T>(to_double(m_min.x()) - delta),
                             static_cast<T>(to_double(m_min.y()) - delta),
                             static_cast<T>(to_double(m_min.z()) - delta)),
            Vector3<T>::from(static_cast<T>(to_double(m_max.x()) + delta),
                             static_cast<T>(to_double(m_max.y()) + delta),
                             static_cast<T>(to_double(m_max.z()) + delta))));
  }

  // ==================== CRTP / Static ====================

  [[nodiscard]] static constexpr std::string_view name() noexcept {
    return NAME;
  }
  [[nodiscard]] static constexpr unsigned dimensions() noexcept {
    return DIMENSIONS;
  }

  // ==================== Comparison ====================

  [[nodiscard]] constexpr bool operator==(const Box &other) const noexcept {
    return m_min == other.m_min && m_max == other.m_max;
  }

  [[nodiscard]] constexpr bool operator!=(const Box &other) const noexcept {
    return !(*this == other);
  }

  // ==================== Stream Output ====================

  friend std::ostream &operator<<(std::ostream &os, const Box &b) {
    os << "Box(" << b.m_min << " to " << b.m_max << ")";
    return os;
  }
};

// ==================== Free Functions ====================

/// Create bounding box from array of points
template <Numeric T, std::size_t N>
[[nodiscard]] constexpr Box<T>
bounding_box(const std::array<Vector3<T>, N> &points) noexcept {
  static_assert(N > 0, "Need at least one point");

  T min_x = points[0].x(), max_x = points[0].x();
  T min_y = points[0].y(), max_y = points[0].y();
  T min_z = points[0].z(), max_z = points[0].z();

  for (std::size_t i = 1; i < N; ++i) {
    if (raw(points[i].x()) < raw(min_x))
      min_x = points[i].x();
    if (raw(points[i].x()) > raw(max_x))
      max_x = points[i].x();
    if (raw(points[i].y()) < raw(min_y))
      min_y = points[i].y();
    if (raw(points[i].y()) > raw(max_y))
      max_y = points[i].y();
    if (raw(points[i].z()) < raw(min_z))
      min_z = points[i].z();
    if (raw(points[i].z()) > raw(max_z))
      max_z = points[i].z();
  }

  return Box<T>::from_points(Vector3<T>::from(min_x, min_y, min_z),
                             Vector3<T>::from(max_x, max_y, max_z));
}

// Type aliases
using Boxi = Box<int>;
using Boxf = Box<float>;
using Boxd = Box<double>;
using AABB = Box<double>;
using AABBf = Box<float>;
using AABBi = Box<int>;

} // namespace pulgacpp

#endif // PULGACPP_GEOMETRY_BOX_HPP
