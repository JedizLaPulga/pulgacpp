// pulgacpp::LineSegment - 2D line segment type
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_GEOMETRY_LINESEGMENT_HPP
#define PULGACPP_GEOMETRY_LINESEGMENT_HPP

#include "point.hpp"
#include "shape.hpp"
#include "vector2.hpp"
#include <algorithm>
#include <cmath>
#include <ostream>


namespace pulgacpp {

// Forward declaration for intersection result
template <Numeric T> class Rectangle;

/// A 2D line segment defined by two endpoints
template <Numeric T> class LineSegment : public Shape2D<LineSegment<T>> {
public:
  using value_type = T;
  static constexpr std::string_view NAME = "LineSegment";
  static constexpr unsigned DIMENSIONS = 2;
  static constexpr bool is_shape = true;

private:
  Point<T> m_start;
  Point<T> m_end;

  constexpr LineSegment(Point<T> start, Point<T> end) noexcept
      : m_start(start), m_end(end) {}

public:
  // ==================== Construction ====================

  /// Default: zero-length segment at origin
  constexpr LineSegment() noexcept
      : m_start(Point<T>::origin()), m_end(Point<T>::origin()) {}

  /// Create from two endpoints
  [[nodiscard]] static constexpr LineSegment from(Point<T> start,
                                                  Point<T> end) noexcept {
    return LineSegment(start, end);
  }

  /// Create from start point and direction vector
  [[nodiscard]] static constexpr LineSegment
  from_direction(Point<T> start, Vector2<T> direction) noexcept {
    return LineSegment(start, Point<T>::from(start.x() + direction.x(),
                                             start.y() + direction.y()));
  }

  /// Create a horizontal segment
  [[nodiscard]] static constexpr LineSegment horizontal(T y, T x1,
                                                        T x2) noexcept {
    T min_x = x1 < x2 ? x1 : x2;
    T max_x = x1 > x2 ? x1 : x2;
    return LineSegment(Point<T>::from(min_x, y), Point<T>::from(max_x, y));
  }

  /// Create a vertical segment
  [[nodiscard]] static constexpr LineSegment vertical(T x, T y1,
                                                      T y2) noexcept {
    T min_y = y1 < y2 ? y1 : y2;
    T max_y = y1 > y2 ? y1 : y2;
    return LineSegment(Point<T>::from(x, min_y), Point<T>::from(x, max_y));
  }

  /// Create unit segment from origin along x-axis
  [[nodiscard]] static constexpr LineSegment unit_x() noexcept {
    if constexpr (std::is_floating_point_v<T>) {
      return LineSegment(Point<T>::origin(), Point<T>::from(T(1), T(0)));
    } else {
      return LineSegment(Point<T>::origin(), Point<T>::from(T(1), T(0)));
    }
  }

  /// Create unit segment from origin along y-axis
  [[nodiscard]] static constexpr LineSegment unit_y() noexcept {
    if constexpr (std::is_floating_point_v<T>) {
      return LineSegment(Point<T>::origin(), Point<T>::from(T(0), T(1)));
    } else {
      return LineSegment(Point<T>::origin(), Point<T>::from(T(0), T(1)));
    }
  }

  // ==================== Accessors ====================

  [[nodiscard]] constexpr Point<T> start() const noexcept { return m_start; }
  [[nodiscard]] constexpr Point<T> end() const noexcept { return m_end; }

  /// Get start point (alias)
  [[nodiscard]] constexpr Point<T> p1() const noexcept { return m_start; }

  /// Get end point (alias)
  [[nodiscard]] constexpr Point<T> p2() const noexcept { return m_end; }

  /// Create with new start point
  [[nodiscard]] constexpr LineSegment
  with_start(Point<T> new_start) const noexcept {
    return LineSegment(new_start, m_end);
  }

  /// Create with new end point
  [[nodiscard]] constexpr LineSegment
  with_end(Point<T> new_end) const noexcept {
    return LineSegment(m_start, new_end);
  }

  /// Swap start and end points (reverse direction)
  [[nodiscard]] constexpr LineSegment reversed() const noexcept {
    return LineSegment(m_end, m_start);
  }

  // ==================== Length & Midpoint ====================

  /// Length of the segment
  [[nodiscard]] constexpr double length() const noexcept {
    return std::sqrt(length_squared());
  }

  /// Squared length (faster, avoids sqrt)
  [[nodiscard]] constexpr double length_squared() const noexcept {
    double dx = to_double(m_end.x()) - to_double(m_start.x());
    double dy = to_double(m_end.y()) - to_double(m_start.y());
    return dx * dx + dy * dy;
  }

  /// Check if segment is degenerate (zero length)
  [[nodiscard]] constexpr bool is_degenerate() const noexcept {
    return raw(m_start.x()) == raw(m_end.x()) &&
           raw(m_start.y()) == raw(m_end.y());
  }

  /// Midpoint of the segment
  [[nodiscard]] constexpr Point<double> midpoint() const noexcept {
    return Point<double>::from(
        (to_double(m_start.x()) + to_double(m_end.x())) / 2.0,
        (to_double(m_start.y()) + to_double(m_end.y())) / 2.0);
  }

  /// Point at parameter t along the segment (t=0: start, t=1: end)
  [[nodiscard]] constexpr Point<double> point_at(double t) const noexcept {
    return Point<double>::from(
        to_double(m_start.x()) +
            t * (to_double(m_end.x()) - to_double(m_start.x())),
        to_double(m_start.y()) +
            t * (to_double(m_end.y()) - to_double(m_start.y())));
  }

  /// Linear interpolation (alias for point_at)
  [[nodiscard]] constexpr Point<double> lerp(double t) const noexcept {
    return point_at(t);
  }

  // ==================== Direction & Vectors ====================

  /// Direction vector from start to end
  [[nodiscard]] constexpr Vector2<double> direction() const noexcept {
    return Vector2<double>::from(to_double(m_end.x()) - to_double(m_start.x()),
                                 to_double(m_end.y()) - to_double(m_start.y()));
  }

  /// Normalized direction vector (Optional, None if degenerate)
  [[nodiscard]] constexpr Optional<Vector2<double>>
  unit_direction() const noexcept {
    return vec_normalized(direction());
  }

  /// Normal vector (perpendicular to direction, left-handed)
  [[nodiscard]] constexpr Vector2<double> normal() const noexcept {
    return vec_perpendicular(direction());
  }

  /// Normalized normal vector
  [[nodiscard]] constexpr Optional<Vector2<double>>
  unit_normal() const noexcept {
    return vec_normalized(normal());
  }

  /// Angle of the segment from positive x-axis (radians)
  [[nodiscard]] constexpr double angle() const noexcept {
    return direction().angle();
  }

  // ==================== Distance & Closest Point ====================

  /// Closest point on segment to a given point
  [[nodiscard]] constexpr Point<double>
  closest_point_to(Point<T> point) const noexcept {
    if (is_degenerate()) {
      return Point<double>::from(to_double(m_start.x()),
                                 to_double(m_start.y()));
    }

    double px = to_double(point.x()) - to_double(m_start.x());
    double py = to_double(point.y()) - to_double(m_start.y());
    double dx = to_double(m_end.x()) - to_double(m_start.x());
    double dy = to_double(m_end.y()) - to_double(m_start.y());

    double t = (px * dx + py * dy) / length_squared();

    // Clamp t to [0, 1] to stay on segment
    if (t < 0.0)
      t = 0.0;
    if (t > 1.0)
      t = 1.0;

    return point_at(t);
  }

  /// Distance from a point to the segment
  [[nodiscard]] constexpr double
  distance_to_point(Point<T> point) const noexcept {
    auto closest = closest_point_to(point);
    double dx = to_double(point.x()) - closest.x();
    double dy = to_double(point.y()) - closest.y();
    return std::sqrt(dx * dx + dy * dy);
  }

  /// Squared distance from a point to the segment
  [[nodiscard]] constexpr double
  distance_squared_to_point(Point<T> point) const noexcept {
    auto closest = closest_point_to(point);
    double dx = to_double(point.x()) - closest.x();
    double dy = to_double(point.y()) - closest.y();
    return dx * dx + dy * dy;
  }

  /// Parameter t for projection of point onto infinite line
  /// Returns t where t=0 is start, t=1 is end
  /// May be outside [0,1] if point projects outside segment
  [[nodiscard]] constexpr double project_t(Point<T> point) const noexcept {
    if (is_degenerate())
      return 0.0;

    double px = to_double(point.x()) - to_double(m_start.x());
    double py = to_double(point.y()) - to_double(m_start.y());
    double dx = to_double(m_end.x()) - to_double(m_start.x());
    double dy = to_double(m_end.y()) - to_double(m_start.y());

    return (px * dx + py * dy) / length_squared();
  }

  // ==================== Containment ====================

  /// Check if a point lies on the segment (within tolerance)
  [[nodiscard]] constexpr bool
  contains(Point<T> point, double tolerance = 1e-9) const noexcept {
    return distance_to_point(point) < tolerance;
  }

  /// Check if point is strictly between start and end (not at endpoints)
  [[nodiscard]] constexpr bool
  strictly_contains(Point<T> point, double tolerance = 1e-9) const noexcept {
    double t = project_t(point);
    if (t <= 0.0 + tolerance || t >= 1.0 - tolerance)
      return false;
    return distance_to_point(point) < tolerance;
  }

  // ==================== Intersection ====================

  /// Check if two segments intersect
  [[nodiscard]] constexpr bool
  intersects(const LineSegment &other) const noexcept {
    return intersection(other).is_some();
  }

  /// Find intersection point of two segments (None if parallel or
  /// non-intersecting)
  [[nodiscard]] constexpr Optional<Point<double>>
  intersection(const LineSegment &other) const noexcept {
    double x1 = to_double(m_start.x()), y1 = to_double(m_start.y());
    double x2 = to_double(m_end.x()), y2 = to_double(m_end.y());
    double x3 = to_double(other.m_start.x()), y3 = to_double(other.m_start.y());
    double x4 = to_double(other.m_end.x()), y4 = to_double(other.m_end.y());

    double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (std::abs(denom) < 1e-12) {
      // Lines are parallel
      return None;
    }

    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
    double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom;

    if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
      return Some(Point<double>::from(x1 + t * (x2 - x1), y1 + t * (y2 - y1)));
    }

    return None;
  }

  /// Check if this segment intersects with a circle
  template <typename U>
  [[nodiscard]] constexpr bool intersects_circle(Point<U> center,
                                                 U radius) const noexcept {
    double dist = distance_to_point(Point<T>::from(
        static_cast<T>(raw(center.x())), static_cast<T>(raw(center.y()))));
    return dist <= to_double(radius);
  }

  // ==================== Transformations ====================

  /// Translate segment
  [[nodiscard]] constexpr Optional<LineSegment>
  checked_translate(T dx, T dy) const noexcept {
    auto new_start = m_start.checked_translate(dx, dy);
    if (new_start.is_none())
      return None;

    auto new_end = m_end.checked_translate(dx, dy);
    if (new_end.is_none())
      return None;

    return Some(LineSegment(new_start.unwrap(), new_end.unwrap()));
  }

  /// Extend segment by a factor from center
  [[nodiscard]] constexpr LineSegment extended(double factor) const noexcept {
    auto mid = midpoint();
    double half_len = length() / 2.0 * factor;
    auto dir = unit_direction();

    if (dir.is_none()) {
      return *this;
    }

    auto d = dir.unwrap();
    return LineSegment(
        Point<T>::from(static_cast<T>(mid.x() - half_len * d.x()),
                       static_cast<T>(mid.y() - half_len * d.y())),
        Point<T>::from(static_cast<T>(mid.x() + half_len * d.x()),
                       static_cast<T>(mid.y() + half_len * d.y())));
  }

  /// Extend segment from both ends by given lengths
  [[nodiscard]] constexpr LineSegment
  extended_by(double start_extend, double end_extend) const noexcept {
    auto dir = unit_direction();
    if (dir.is_none())
      return *this;

    auto d = dir.unwrap();
    return LineSegment(
        Point<T>::from(
            static_cast<T>(to_double(m_start.x()) - start_extend * d.x()),
            static_cast<T>(to_double(m_start.y()) - start_extend * d.y())),
        Point<T>::from(
            static_cast<T>(to_double(m_end.x()) + end_extend * d.x()),
            static_cast<T>(to_double(m_end.y()) + end_extend * d.y())));
  }

  // ==================== Bounding Box ====================

  /// Get minimum x coordinate
  [[nodiscard]] constexpr T min_x() const noexcept {
    return m_start.x() < m_end.x() ? m_start.x() : m_end.x();
  }

  /// Get maximum x coordinate
  [[nodiscard]] constexpr T max_x() const noexcept {
    return m_start.x() > m_end.x() ? m_start.x() : m_end.x();
  }

  /// Get minimum y coordinate
  [[nodiscard]] constexpr T min_y() const noexcept {
    return m_start.y() < m_end.y() ? m_start.y() : m_end.y();
  }

  /// Get maximum y coordinate
  [[nodiscard]] constexpr T max_y() const noexcept {
    return m_start.y() > m_end.y() ? m_start.y() : m_end.y();
  }

  /// Get bounding box corners
  [[nodiscard]] constexpr std::pair<Point<T>, Point<T>>
  bounding_box() const noexcept {
    return {Point<T>::from(min_x(), min_y()), Point<T>::from(max_x(), max_y())};
  }

  // ==================== Shape2D Interface ====================

  /// "Perimeter" of segment is 2x length (there and back)
  [[nodiscard]] constexpr double perimeter() const noexcept {
    return 2.0 * length();
  }

  /// Area is 0 for a line segment
  [[nodiscard]] constexpr double area() const noexcept { return 0.0; }

  // ==================== Comparison ====================

  [[nodiscard]] constexpr bool
  operator==(const LineSegment &other) const noexcept {
    return m_start == other.m_start && m_end == other.m_end;
  }

  [[nodiscard]] constexpr bool
  operator!=(const LineSegment &other) const noexcept {
    return !(*this == other);
  }

  /// Check if segments are equal regardless of direction
  [[nodiscard]] constexpr bool
  equivalent_to(const LineSegment &other) const noexcept {
    return (*this == other) || (*this == other.reversed());
  }

  // ==================== Stream Output ====================

  friend std::ostream &operator<<(std::ostream &os, const LineSegment &seg) {
    os << "LineSegment(" << seg.m_start << " -> " << seg.m_end << ")";
    return os;
  }
};

// ==================== Free Functions ====================

/// Create segment between two points
template <Numeric T>
[[nodiscard]] constexpr LineSegment<double>
segment_between(Point<T> a, Point<T> b) noexcept {
  return LineSegment<double>::from(
      Point<double>::from(to_double(a.x()), to_double(a.y())),
      Point<double>::from(to_double(b.x()), to_double(b.y())));
}

/// Find the shortest segment connecting two segments
template <Numeric T, Numeric U>
[[nodiscard]] constexpr Optional<LineSegment<double>>
shortest_segment_between(const LineSegment<T> &a,
                         const LineSegment<U> &b) noexcept {
  // Check if they intersect
  auto intersection = a.intersection(
      LineSegment<T>::from(Point<T>::from(static_cast<T>(raw(b.start().x())),
                                          static_cast<T>(raw(b.start().y()))),
                           Point<T>::from(static_cast<T>(raw(b.end().x())),
                                          static_cast<T>(raw(b.end().y())))));

  if (intersection.is_some()) {
    // They intersect, shortest distance is 0
    auto p = intersection.unwrap();
    return Some(LineSegment<double>::from(p, p));
  }

  // Find closest points between the two segments
  // Check all 4 endpoint-to-segment combinations
  auto a_start_to_b = b.closest_point_to(Point<U>::from(
      static_cast<U>(raw(a.start().x())), static_cast<U>(raw(a.start().y()))));
  auto a_end_to_b = b.closest_point_to(Point<U>::from(
      static_cast<U>(raw(a.end().x())), static_cast<U>(raw(a.end().y()))));
  auto b_start_to_a = a.closest_point_to(Point<T>::from(
      static_cast<T>(raw(b.start().x())), static_cast<T>(raw(b.start().y()))));
  auto b_end_to_a = a.closest_point_to(Point<T>::from(
      static_cast<T>(raw(b.end().x())), static_cast<T>(raw(b.end().y()))));

  // Calculate distances
  auto dist_sq = [](Point<double> p1, Point<double> p2) {
    double dx = p1.x() - p2.x();
    double dy = p1.y() - p2.y();
    return dx * dx + dy * dy;
  };

  Point<double> a_start_d =
      Point<double>::from(to_double(a.start().x()), to_double(a.start().y()));
  Point<double> a_end_d =
      Point<double>::from(to_double(a.end().x()), to_double(a.end().y()));
  Point<double> b_start_d =
      Point<double>::from(to_double(b.start().x()), to_double(b.start().y()));
  Point<double> b_end_d =
      Point<double>::from(to_double(b.end().x()), to_double(b.end().y()));

  double d1 = dist_sq(a_start_d, a_start_to_b);
  double d2 = dist_sq(a_end_d, a_end_to_b);
  double d3 = dist_sq(b_start_d, b_start_to_a);
  double d4 = dist_sq(b_end_d, b_end_to_a);

  double min_d = d1;
  Point<double> p1 = a_start_d, p2 = a_start_to_b;

  if (d2 < min_d) {
    min_d = d2;
    p1 = a_end_d;
    p2 = a_end_to_b;
  }
  if (d3 < min_d) {
    min_d = d3;
    p1 = b_start_to_a;
    p2 = b_start_d;
  }
  if (d4 < min_d) {
    min_d = d4;
    p1 = b_end_to_a;
    p2 = b_end_d;
  }

  return Some(LineSegment<double>::from(p1, p2));
}

/// Calculate distance between two segments
template <Numeric T, Numeric U>
[[nodiscard]] constexpr double
segment_distance(const LineSegment<T> &a, const LineSegment<U> &b) noexcept {
  auto shortest = shortest_segment_between(a, b);
  if (shortest.is_none())
    return 0.0;
  return shortest.unwrap().length();
}

// Type aliases
using LineSegmenti = LineSegment<int>;
using LineSegmentf = LineSegment<float>;
using LineSegmentd = LineSegment<double>;
using Line2i = LineSegment<int>;
using Line2f = LineSegment<float>;
using Line2d = LineSegment<double>;

} // namespace pulgacpp

#endif // PULGACPP_GEOMETRY_LINESEGMENT_HPP
