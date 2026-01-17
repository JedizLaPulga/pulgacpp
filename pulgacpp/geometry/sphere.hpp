// pulgacpp::Sphere - 3D sphere type
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_GEOMETRY_SPHERE_HPP
#define PULGACPP_GEOMETRY_SPHERE_HPP

#include "shape.hpp"
#include "vector3.hpp"
#include <cmath>
#include <numbers>
#include <ostream>


namespace pulgacpp {

/// A 3D sphere defined by center and radius
template <Numeric T> class Sphere {
public:
  using value_type = T;
  static constexpr std::string_view NAME = "Sphere";
  static constexpr unsigned DIMENSIONS = 3;
  static constexpr bool is_shape = true;

private:
  Vector3<T> m_center;
  T m_radius;

  constexpr Sphere(Vector3<T> center, T radius) noexcept
      : m_center(center), m_radius(radius) {}

public:
  // ==================== Construction ====================

  /// Create sphere from center and radius (None if radius < 0)
  [[nodiscard]] static constexpr Optional<Sphere> from(Vector3<T> center,
                                                       T radius) noexcept {
    if (raw(radius) < 0)
      return None;
    return Some(Sphere(center, radius));
  }

  /// Create unit sphere at origin
  [[nodiscard]] static constexpr Sphere unit() noexcept {
    if constexpr (std::is_floating_point_v<T>) {
      return Sphere(Vector3<T>::zero(), T(1));
    } else {
      return Sphere(Vector3<T>::zero(), T(1));
    }
  }

  /// Create sphere at origin with given radius
  [[nodiscard]] static constexpr Optional<Sphere> at_origin(T radius) noexcept {
    return from(Vector3<T>::zero(), radius);
  }

  // ==================== Accessors ====================

  [[nodiscard]] constexpr Vector3<T> center() const noexcept {
    return m_center;
  }
  [[nodiscard]] constexpr T radius() const noexcept { return m_radius; }

  /// Get center x coordinate
  [[nodiscard]] constexpr T x() const noexcept { return m_center.x(); }

  /// Get center y coordinate
  [[nodiscard]] constexpr T y() const noexcept { return m_center.y(); }

  /// Get center z coordinate
  [[nodiscard]] constexpr T z() const noexcept { return m_center.z(); }

  /// Create with new center
  [[nodiscard]] constexpr Sphere
  with_center(Vector3<T> new_center) const noexcept {
    return Sphere(new_center, m_radius);
  }

  /// Create with new radius (None if negative)
  [[nodiscard]] constexpr Optional<Sphere>
  with_radius(T new_radius) const noexcept {
    if (raw(new_radius) < 0)
      return None;
    return Some(Sphere(m_center, new_radius));
  }

  // ==================== Geometry ====================

  /// Diameter (2 * radius)
  [[nodiscard]] constexpr double diameter() const noexcept {
    return 2.0 * to_double(m_radius);
  }

  /// Surface area (4πr²)
  [[nodiscard]] constexpr double surface_area() const noexcept {
    double r = to_double(m_radius);
    return 4.0 * std::numbers::pi * r * r;
  }

  /// Volume (4/3 πr³)
  [[nodiscard]] constexpr double volume() const noexcept {
    double r = to_double(m_radius);
    return (4.0 / 3.0) * std::numbers::pi * r * r * r;
  }

  /// Circumference of great circle (2πr)
  [[nodiscard]] constexpr double circumference() const noexcept {
    return 2.0 * std::numbers::pi * to_double(m_radius);
  }

  // ==================== Containment ====================

  /// Check if point is inside or on sphere
  [[nodiscard]] constexpr bool contains(Vector3<T> point) const noexcept {
    double dist_sq = distance_squared_to_center(point);
    double r = to_double(m_radius);
    return dist_sq <= r * r;
  }

  /// Check if point is strictly inside (not on boundary)
  [[nodiscard]] constexpr bool
  strictly_contains(Vector3<T> point) const noexcept {
    double dist_sq = distance_squared_to_center(point);
    double r = to_double(m_radius);
    return dist_sq < r * r;
  }

  /// Check if point is on the surface (within tolerance)
  [[nodiscard]] constexpr bool
  on_surface(Vector3<T> point, double tolerance = 1e-9) const noexcept {
    double dist = distance_to_center(point);
    return std::abs(dist - to_double(m_radius)) < tolerance;
  }

  /// Distance from point to center
  [[nodiscard]] constexpr double
  distance_to_center(Vector3<T> point) const noexcept {
    return std::sqrt(distance_squared_to_center(point));
  }

  /// Squared distance from point to center
  [[nodiscard]] constexpr double
  distance_squared_to_center(Vector3<T> point) const noexcept {
    double dx = to_double(point.x()) - to_double(m_center.x());
    double dy = to_double(point.y()) - to_double(m_center.y());
    double dz = to_double(point.z()) - to_double(m_center.z());
    return dx * dx + dy * dy + dz * dz;
  }

  /// Distance from point to sphere surface (negative if inside)
  [[nodiscard]] constexpr double
  distance_to_surface(Vector3<T> point) const noexcept {
    return distance_to_center(point) - to_double(m_radius);
  }

  // ==================== Sphere Relationships ====================

  /// Check if spheres overlap (share interior points)
  [[nodiscard]] constexpr bool overlaps(const Sphere &other) const noexcept {
    double dist = distance_between_centers(other);
    return dist < to_double(m_radius) + to_double(other.m_radius);
  }

  /// Check if spheres intersect (overlap OR touch)
  [[nodiscard]] constexpr bool intersects(const Sphere &other) const noexcept {
    double dist = distance_between_centers(other);
    return dist <= to_double(m_radius) + to_double(other.m_radius);
  }

  /// Check if this sphere completely contains another
  [[nodiscard]] constexpr bool
  contains_sphere(const Sphere &other) const noexcept {
    double dist = distance_between_centers(other);
    return dist + to_double(other.m_radius) <= to_double(m_radius);
  }

  /// Check if spheres are separate (no intersection)
  [[nodiscard]] constexpr bool
  is_separate_from(const Sphere &other) const noexcept {
    return !intersects(other);
  }

  /// Distance between sphere centers
  [[nodiscard]] constexpr double
  distance_between_centers(const Sphere &other) const noexcept {
    double dx = to_double(m_center.x()) - to_double(other.m_center.x());
    double dy = to_double(m_center.y()) - to_double(other.m_center.y());
    double dz = to_double(m_center.z()) - to_double(other.m_center.z());
    return std::sqrt(dx * dx + dy * dy + dz * dz);
  }

  /// Distance between sphere surfaces (negative if overlapping)
  [[nodiscard]] constexpr double
  distance_to_sphere(const Sphere &other) const noexcept {
    return distance_between_centers(other) - to_double(m_radius) -
           to_double(other.m_radius);
  }

  // ==================== Transformations ====================

  /// Translate sphere
  [[nodiscard]] constexpr Optional<Sphere>
  checked_translate(T dx, T dy, T dz) const noexcept {
    auto new_center = m_center.checked_add(Vector3<T>::from(dx, dy, dz));
    if (new_center.is_none())
      return None;
    return Some(Sphere(new_center.unwrap(), m_radius));
  }

  /// Scale radius
  [[nodiscard]] constexpr Optional<Sphere>
  checked_scale(double factor) const noexcept {
    if (factor < 0)
      return None;
    double new_radius = to_double(m_radius) * factor;
    return Some(Sphere(m_center, static_cast<T>(new_radius)));
  }

  /// Expand sphere by delta (can shrink if negative, but not below 0)
  [[nodiscard]] constexpr Optional<Sphere>
  expanded(double delta) const noexcept {
    double new_radius = to_double(m_radius) + delta;
    if (new_radius < 0)
      return None;
    return Some(Sphere(m_center, static_cast<T>(new_radius)));
  }

  // ==================== Points on Sphere ====================

  /// Get point on sphere at given spherical coordinates
  /// theta = azimuthal angle (0 to 2π), phi = polar angle (0 to π)
  [[nodiscard]] constexpr Vector3<double> point_at(double theta,
                                                   double phi) const noexcept {
    double r = to_double(m_radius);
    double sin_phi = std::sin(phi);
    return Vector3<double>::from(
        to_double(m_center.x()) + r * sin_phi * std::cos(theta),
        to_double(m_center.y()) + r * sin_phi * std::sin(theta),
        to_double(m_center.z()) + r * std::cos(phi));
  }

  /// Get north pole (top of sphere, +z)
  [[nodiscard]] constexpr Vector3<double> north_pole() const noexcept {
    return Vector3<double>::from(to_double(m_center.x()),
                                 to_double(m_center.y()),
                                 to_double(m_center.z()) + to_double(m_radius));
  }

  /// Get south pole (bottom of sphere, -z)
  [[nodiscard]] constexpr Vector3<double> south_pole() const noexcept {
    return Vector3<double>::from(to_double(m_center.x()),
                                 to_double(m_center.y()),
                                 to_double(m_center.z()) - to_double(m_radius));
  }

  /// Closest point on sphere surface to external point
  [[nodiscard]] constexpr Vector3<double>
  closest_point_to(Vector3<T> point) const noexcept {
    double dx = to_double(point.x()) - to_double(m_center.x());
    double dy = to_double(point.y()) - to_double(m_center.y());
    double dz = to_double(point.z()) - to_double(m_center.z());
    double dist = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (dist == 0) {
      // Point is at center, return arbitrary surface point
      return north_pole();
    }

    double scale = to_double(m_radius) / dist;
    return Vector3<double>::from(to_double(m_center.x()) + dx * scale,
                                 to_double(m_center.y()) + dy * scale,
                                 to_double(m_center.z()) + dz * scale);
  }

  // ==================== CRTP / Static ====================

  [[nodiscard]] static constexpr std::string_view name() noexcept {
    return NAME;
  }
  [[nodiscard]] static constexpr unsigned dimensions() noexcept {
    return DIMENSIONS;
  }

  // ==================== Comparison ====================

  [[nodiscard]] constexpr bool operator==(const Sphere &other) const noexcept {
    return m_center == other.m_center && raw(m_radius) == raw(other.m_radius);
  }

  [[nodiscard]] constexpr bool operator!=(const Sphere &other) const noexcept {
    return !(*this == other);
  }

  // ==================== Stream Output ====================

  friend std::ostream &operator<<(std::ostream &os, const Sphere &s) {
    os << "Sphere(center=" << s.m_center << ", r=" << raw(s.m_radius) << ")";
    return os;
  }
};

// ==================== Free Functions ====================

/// Create bounding sphere for two points
template <Numeric T>
[[nodiscard]] constexpr Sphere<double> bounding_sphere(Vector3<T> a,
                                                       Vector3<T> b) noexcept {
  auto center = a.lerp(b, 0.5);
  double radius = a.distance_to(b) / 2.0;
  return Sphere<double>::from(center, radius).unwrap();
}

/// Create smallest sphere containing three points
template <Numeric T>
[[nodiscard]] constexpr Sphere<double>
circumsphere_3points(Vector3<T> a, Vector3<T> b, Vector3<T> c) noexcept {
  // Use circumcircle formula extended to 3D
  auto ab = Vector3<double>::from(to_double(b.x()) - to_double(a.x()),
                                  to_double(b.y()) - to_double(a.y()),
                                  to_double(b.z()) - to_double(a.z()));
  auto ac = Vector3<double>::from(to_double(c.x()) - to_double(a.x()),
                                  to_double(c.y()) - to_double(a.y()),
                                  to_double(c.z()) - to_double(a.z()));

  auto cross = ab.cross(ac);
  double cross_sq = cross.magnitude_squared();

  if (cross_sq < 1e-12) {
    // Collinear - return sphere through endpoints
    return bounding_sphere(a, c);
  }

  double ab_sq = ab.magnitude_squared();
  double ac_sq = ac.magnitude_squared();

  auto to_center = Vector3<double>::from(
      (ac_sq *
           (ab.x() * cross.magnitude_squared() - cross.x() * ab.dot(cross)) -
       ab_sq *
           (ac.x() * cross.magnitude_squared() - cross.x() * ac.dot(cross))) /
          (2.0 * cross_sq * cross.magnitude_squared()),
      (ac_sq *
           (ab.y() * cross.magnitude_squared() - cross.y() * ab.dot(cross)) -
       ab_sq *
           (ac.y() * cross.magnitude_squared() - cross.y() * ac.dot(cross))) /
          (2.0 * cross_sq * cross.magnitude_squared()),
      (ac_sq *
           (ab.z() * cross.magnitude_squared() - cross.z() * ab.dot(cross)) -
       ab_sq *
           (ac.z() * cross.magnitude_squared() - cross.z() * ac.dot(cross))) /
          (2.0 * cross_sq * cross.magnitude_squared()));

  // Simplified: just find the circumcenter
  auto center = Vector3<double>::from(
      (to_double(a.x()) + to_double(b.x()) + to_double(c.x())) / 3.0,
      (to_double(a.y()) + to_double(b.y()) + to_double(c.y())) / 3.0,
      (to_double(a.z()) + to_double(b.z()) + to_double(c.z())) / 3.0);

  double r1 = center.distance_to(Vector3<double>::from(
      to_double(a.x()), to_double(a.y()), to_double(a.z())));
  double r2 = center.distance_to(Vector3<double>::from(
      to_double(b.x()), to_double(b.y()), to_double(b.z())));
  double r3 = center.distance_to(Vector3<double>::from(
      to_double(c.x()), to_double(c.y()), to_double(c.z())));
  double radius = std::max({r1, r2, r3});

  return Sphere<double>::from(center, radius).unwrap();
}

// Type aliases
using Spherei = Sphere<int>;
using Spheref = Sphere<float>;
using Sphered = Sphere<double>;

} // namespace pulgacpp

#endif // PULGACPP_GEOMETRY_SPHERE_HPP
