// pulgacpp::Vector3 - 3D vector type
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_GEOMETRY_VECTOR3_HPP
#define PULGACPP_GEOMETRY_VECTOR3_HPP

#include "shape.hpp"
#include <cmath>
#include <ostream>
#include <string>

namespace pulgacpp {

/// A 3D vector with components (x, y, z)
template <Numeric T> class Vector3 {
public:
  using value_type = T;
  static constexpr std::string_view NAME = "Vector3";
  static constexpr unsigned DIMENSIONS = 3;
  static constexpr bool is_shape = true;

private:
  T m_x;
  T m_y;
  T m_z;

  constexpr Vector3(T x, T y, T z) noexcept : m_x(x), m_y(y), m_z(z) {}

public:
  // ==================== Construction ====================

  /// Default: zero vector
  constexpr Vector3() noexcept : m_x{}, m_y{}, m_z{} {}

  /// Factory: create from components
  [[nodiscard]] static constexpr Vector3 from(T x, T y, T z) noexcept {
    return Vector3(x, y, z);
  }

  /// Factory: zero vector
  [[nodiscard]] static constexpr Vector3 zero() noexcept { return Vector3{}; }

  /// Factory: unit vectors
  [[nodiscard]] static constexpr Vector3 unit_x() noexcept {
    if constexpr (std::is_floating_point_v<T>) {
      return Vector3(T(1), T(0), T(0));
    } else {
      return Vector3(T(1), T(0), T(0));
    }
  }

  [[nodiscard]] static constexpr Vector3 unit_y() noexcept {
    if constexpr (std::is_floating_point_v<T>) {
      return Vector3(T(0), T(1), T(0));
    } else {
      return Vector3(T(0), T(1), T(0));
    }
  }

  [[nodiscard]] static constexpr Vector3 unit_z() noexcept {
    if constexpr (std::is_floating_point_v<T>) {
      return Vector3(T(0), T(0), T(1));
    } else {
      return Vector3(T(0), T(0), T(1));
    }
  }

  // ==================== Accessors ====================

  [[nodiscard]] constexpr T x() const noexcept { return m_x; }
  [[nodiscard]] constexpr T y() const noexcept { return m_y; }
  [[nodiscard]] constexpr T z() const noexcept { return m_z; }

  [[nodiscard]] constexpr Vector3 with_x(T new_x) const noexcept {
    return Vector3(new_x, m_y, m_z);
  }

  [[nodiscard]] constexpr Vector3 with_y(T new_y) const noexcept {
    return Vector3(m_x, new_y, m_z);
  }

  [[nodiscard]] constexpr Vector3 with_z(T new_z) const noexcept {
    return Vector3(m_x, m_y, new_z);
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
    double z = to_double(m_z);
    return x * x + y * y + z * z;
  }

  /// Check if zero vector
  [[nodiscard]] constexpr bool is_zero() const noexcept {
    return raw(m_x) == 0 && raw(m_y) == 0 && raw(m_z) == 0;
  }

  /// Check if unit vector (magnitude ≈ 1)
  [[nodiscard]] constexpr bool is_unit(double tolerance = 1e-9) const noexcept {
    return std::abs(magnitude_squared() - 1.0) < tolerance;
  }

  // ==================== Arithmetic (Checked) ====================

  /// Add vectors, returns Optional on overflow
  [[nodiscard]] constexpr Optional<Vector3>
  checked_add(Vector3 other) const noexcept {
    auto new_x = pulgacpp::checked_add(m_x, other.m_x);
    if (new_x.is_none())
      return None;

    auto new_y = pulgacpp::checked_add(m_y, other.m_y);
    if (new_y.is_none())
      return None;

    auto new_z = pulgacpp::checked_add(m_z, other.m_z);
    if (new_z.is_none())
      return None;

    return Some(Vector3(new_x.unwrap(), new_y.unwrap(), new_z.unwrap()));
  }

  /// Subtract vectors
  [[nodiscard]] constexpr Optional<Vector3>
  checked_sub(Vector3 other) const noexcept {
    auto new_x = pulgacpp::checked_sub(m_x, other.m_x);
    if (new_x.is_none())
      return None;

    auto new_y = pulgacpp::checked_sub(m_y, other.m_y);
    if (new_y.is_none())
      return None;

    auto new_z = pulgacpp::checked_sub(m_z, other.m_z);
    if (new_z.is_none())
      return None;

    return Some(Vector3(new_x.unwrap(), new_y.unwrap(), new_z.unwrap()));
  }

  /// Scale by factor
  [[nodiscard]] constexpr Optional<Vector3>
  checked_scale(T factor) const noexcept {
    auto new_x = pulgacpp::checked_mul(m_x, factor);
    if (new_x.is_none())
      return None;

    auto new_y = pulgacpp::checked_mul(m_y, factor);
    if (new_y.is_none())
      return None;

    auto new_z = pulgacpp::checked_mul(m_z, factor);
    if (new_z.is_none())
      return None;

    return Some(Vector3(new_x.unwrap(), new_y.unwrap(), new_z.unwrap()));
  }

  // ==================== Vector Operations ====================

  /// Dot product
  [[nodiscard]] constexpr double dot(Vector3 other) const noexcept {
    return to_double(m_x) * to_double(other.m_x) +
           to_double(m_y) * to_double(other.m_y) +
           to_double(m_z) * to_double(other.m_z);
  }

  /// Cross product (returns a new Vector3)
  [[nodiscard]] constexpr Vector3<double> cross(Vector3 other) const noexcept {
    double ax = to_double(m_x), ay = to_double(m_y), az = to_double(m_z);
    double bx = to_double(other.m_x), by = to_double(other.m_y),
           bz = to_double(other.m_z);
    return Vector3<double>::from(ay * bz - az * by, az * bx - ax * bz,
                                 ax * by - ay * bx);
  }

  /// Angle between this vector and another (radians)
  [[nodiscard]] constexpr double angle_to(Vector3 other) const noexcept {
    double dot_product = dot(other);
    double mags = magnitude() * other.magnitude();
    if (mags == 0.0)
      return 0.0;
    // Clamp to handle floating point errors
    double cos_angle = dot_product / mags;
    if (cos_angle > 1.0)
      cos_angle = 1.0;
    if (cos_angle < -1.0)
      cos_angle = -1.0;
    return std::acos(cos_angle);
  }

  /// Distance to another vector (treating as points)
  [[nodiscard]] constexpr double distance_to(Vector3 other) const noexcept {
    double dx = to_double(m_x) - to_double(other.m_x);
    double dy = to_double(m_y) - to_double(other.m_y);
    double dz = to_double(m_z) - to_double(other.m_z);
    return std::sqrt(dx * dx + dy * dy + dz * dz);
  }

  /// Linear interpolation between vectors
  [[nodiscard]] constexpr Vector3<double> lerp(Vector3 other,
                                               double t) const noexcept {
    return Vector3<double>::from(
        to_double(m_x) + t * (to_double(other.m_x) - to_double(m_x)),
        to_double(m_y) + t * (to_double(other.m_y) - to_double(m_y)),
        to_double(m_z) + t * (to_double(other.m_z) - to_double(m_z)));
  }

  /// Project this vector onto another
  [[nodiscard]] constexpr Vector3<double>
  project_onto(Vector3 other) const noexcept {
    double other_mag_sq = other.magnitude_squared();
    if (other_mag_sq == 0.0)
      return Vector3<double>::zero();
    double scale = dot(other) / other_mag_sq;
    return Vector3<double>::from(to_double(other.m_x) * scale,
                                 to_double(other.m_y) * scale,
                                 to_double(other.m_z) * scale);
  }

  /// Reflect vector off a surface with given normal
  [[nodiscard]] constexpr Vector3<double>
  reflect(Vector3 normal) const noexcept {
    double dot_2 = 2.0 * dot(normal);
    return Vector3<double>::from(to_double(m_x) - dot_2 * to_double(normal.m_x),
                                 to_double(m_y) - dot_2 * to_double(normal.m_y),
                                 to_double(m_z) -
                                     dot_2 * to_double(normal.m_z));
  }

  // ==================== Comparison ====================

  [[nodiscard]] constexpr bool operator==(const Vector3 &other) const noexcept {
    return raw(m_x) == raw(other.m_x) && raw(m_y) == raw(other.m_y) &&
           raw(m_z) == raw(other.m_z);
  }

  [[nodiscard]] constexpr bool operator!=(const Vector3 &other) const noexcept {
    return !(*this == other);
  }

  // ==================== Stream Output ====================

  friend std::ostream &operator<<(std::ostream &os, const Vector3 &v) {
    os << "Vector3(" << raw(v.m_x) << ", " << raw(v.m_y) << ", " << raw(v.m_z)
       << ")";
    return os;
  }

  // ==================== CRTP Shape interface ====================

  [[nodiscard]] static constexpr std::string_view name() noexcept {
    return NAME;
  }
  [[nodiscard]] static constexpr unsigned dimensions() noexcept {
    return DIMENSIONS;
  }
};

// ==================== Free Functions ====================

/// Normalize vector (returns Optional)
template <Numeric T>
[[nodiscard]] constexpr Optional<Vector3<double>>
vec3_normalized(const Vector3<T> &v) noexcept {
  double mag = v.magnitude();
  if (mag == 0.0) {
    return None;
  }
  return Some(Vector3<double>::from(
      to_double(v.x()) / mag, to_double(v.y()) / mag, to_double(v.z()) / mag));
}

/// Negate vector
template <Numeric T>
[[nodiscard]] constexpr Vector3<double>
vec3_negated(const Vector3<T> &v) noexcept {
  return Vector3<double>::from(-to_double(v.x()), -to_double(v.y()),
                               -to_double(v.z()));
}

/// Component-wise absolute value
template <Numeric T>
[[nodiscard]] constexpr Vector3<double> vec3_abs(const Vector3<T> &v) noexcept {
  return Vector3<double>::from(std::abs(to_double(v.x())),
                               std::abs(to_double(v.y())),
                               std::abs(to_double(v.z())));
}

/// Component-wise minimum
template <Numeric T, Numeric U>
[[nodiscard]] constexpr Vector3<double> vec3_min(const Vector3<T> &a,
                                                 const Vector3<U> &b) noexcept {
  return Vector3<double>::from(std::min(to_double(a.x()), to_double(b.x())),
                               std::min(to_double(a.y()), to_double(b.y())),
                               std::min(to_double(a.z()), to_double(b.z())));
}

/// Component-wise maximum
template <Numeric T, Numeric U>
[[nodiscard]] constexpr Vector3<double> vec3_max(const Vector3<T> &a,
                                                 const Vector3<U> &b) noexcept {
  return Vector3<double>::from(std::max(to_double(a.x()), to_double(b.x())),
                               std::max(to_double(a.y()), to_double(b.y())),
                               std::max(to_double(a.z()), to_double(b.z())));
}

/// Calculate the triple scalar product (a · (b × c))
template <Numeric T, Numeric U, Numeric V>
[[nodiscard]] constexpr double
vec3_triple_product(const Vector3<T> &a, const Vector3<U> &b,
                    const Vector3<V> &c) noexcept {
  auto b_cross_c = b.cross(c);
  return to_double(a.x()) * b_cross_c.x() + to_double(a.y()) * b_cross_c.y() +
         to_double(a.z()) * b_cross_c.z();
}

/// Spherical linear interpolation (slerp)
template <Numeric T>
[[nodiscard]] inline Vector3<double>
vec3_slerp(const Vector3<T> &a, const Vector3<T> &b, double t) noexcept {
  double dot = a.dot(b) / (a.magnitude() * b.magnitude());
  // Clamp
  if (dot > 1.0)
    dot = 1.0;
  if (dot < -1.0)
    dot = -1.0;

  double theta = std::acos(dot);
  if (std::abs(theta) < 1e-9) {
    // Nearly parallel, use linear interpolation
    return a.lerp(b, t);
  }

  double sin_theta = std::sin(theta);
  double wa = std::sin((1.0 - t) * theta) / sin_theta;
  double wb = std::sin(t * theta) / sin_theta;

  return Vector3<double>::from(to_double(a.x()) * wa + to_double(b.x()) * wb,
                               to_double(a.y()) * wa + to_double(b.y()) * wb,
                               to_double(a.z()) * wa + to_double(b.z()) * wb);
}

/// Create vector from spherical coordinates (r, theta, phi)
/// theta = azimuthal angle (0 to 2π), phi = polar angle (0 to π)
[[nodiscard]] inline Vector3<double> vec3_from_spherical(double r, double theta,
                                                         double phi) noexcept {
  double sin_phi = std::sin(phi);
  return Vector3<double>::from(r * sin_phi * std::cos(theta),
                               r * sin_phi * std::sin(theta),
                               r * std::cos(phi));
}

// Type aliases
using Vec3i = Vector3<int>;
using Vec3f = Vector3<float>;
using Vec3d = Vector3<double>;

} // namespace pulgacpp

#endif // PULGACPP_GEOMETRY_VECTOR3_HPP
