// pulgacpp::Angle - Type-safe angle representation
// SPDX-License-Identifier: MIT
//
// Prevents the common bug of mixing radians and degrees.
// All internal storage is in radians.

#ifndef PULGACPP_GEOMETRY_ANGLE_HPP
#define PULGACPP_GEOMETRY_ANGLE_HPP

#include "shape.hpp"
#include <cmath>
#include <numbers>
#include <ostream>


namespace pulgacpp {

/// Type-safe angle representation.
/// Stores angles in radians internally but provides convenient
/// conversions from/to degrees.
template <typename T = double>
  requires std::floating_point<T>
class Angle {
public:
  using value_type = T;
  static constexpr std::string_view NAME = "Angle";

private:
  T m_radians;

  constexpr explicit Angle(T radians) noexcept : m_radians(radians) {}

public:
  // ==================== Constants ====================

  /// π (pi)
  static constexpr T PI = std::numbers::pi_v<T>;

  /// τ (tau = 2π)
  static constexpr T TAU = T(2) * PI;

  /// Degrees per radian
  static constexpr T DEG_PER_RAD = T(180) / PI;

  /// Radians per degree
  static constexpr T RAD_PER_DEG = PI / T(180);

  // ==================== Construction ====================

  /// Default: zero angle
  constexpr Angle() noexcept : m_radians(T(0)) {}

  /// Create angle from radians
  [[nodiscard]] static constexpr Angle from_radians(T radians) noexcept {
    return Angle(radians);
  }

  /// Create angle from degrees
  [[nodiscard]] static constexpr Angle from_degrees(T degrees) noexcept {
    return Angle(degrees * RAD_PER_DEG);
  }

  /// Create angle from turns (1 turn = 360°)
  [[nodiscard]] static constexpr Angle from_turns(T turns) noexcept {
    return Angle(turns * TAU);
  }

  /// Zero angle
  [[nodiscard]] static constexpr Angle zero() noexcept { return Angle(T(0)); }

  /// Full turn (360° = 2π)
  [[nodiscard]] static constexpr Angle full_turn() noexcept {
    return Angle(TAU);
  }

  /// Half turn (180° = π)
  [[nodiscard]] static constexpr Angle half_turn() noexcept {
    return Angle(PI);
  }

  /// Quarter turn (90° = π/2)
  [[nodiscard]] static constexpr Angle quarter_turn() noexcept {
    return Angle(PI / T(2));
  }

  /// Right angle (90°)
  [[nodiscard]] static constexpr Angle right() noexcept {
    return quarter_turn();
  }

  // ==================== Accessors ====================

  /// Get angle in radians
  [[nodiscard]] constexpr T radians() const noexcept { return m_radians; }

  /// Get angle in degrees
  [[nodiscard]] constexpr T degrees() const noexcept {
    return m_radians * DEG_PER_RAD;
  }

  /// Get angle in turns (1 turn = 360°)
  [[nodiscard]] constexpr T turns() const noexcept { return m_radians / TAU; }

  // ==================== Trigonometry ====================

  /// Sine of angle
  [[nodiscard]] constexpr T sin() const noexcept { return std::sin(m_radians); }

  /// Cosine of angle
  [[nodiscard]] constexpr T cos() const noexcept { return std::cos(m_radians); }

  /// Tangent of angle
  [[nodiscard]] constexpr T tan() const noexcept { return std::tan(m_radians); }

  /// Create angle from arc sine
  [[nodiscard]] static constexpr Angle asin(T value) noexcept {
    return Angle(std::asin(value));
  }

  /// Create angle from arc cosine
  [[nodiscard]] static constexpr Angle acos(T value) noexcept {
    return Angle(std::acos(value));
  }

  /// Create angle from arc tangent
  [[nodiscard]] static constexpr Angle atan(T value) noexcept {
    return Angle(std::atan(value));
  }

  /// Create angle from atan2 (y, x)
  [[nodiscard]] static constexpr Angle atan2(T y, T x) noexcept {
    return Angle(std::atan2(y, x));
  }

  // ==================== Normalization ====================

  /// Normalize to [0, 2π)
  [[nodiscard]] constexpr Angle normalized() const noexcept {
    T result = std::fmod(m_radians, TAU);
    if (result < T(0)) {
      result += TAU;
    }
    return Angle(result);
  }

  /// Normalize to [-π, π)
  [[nodiscard]] constexpr Angle normalized_signed() const noexcept {
    T result = std::fmod(m_radians + PI, TAU);
    if (result < T(0)) {
      result += TAU;
    }
    return Angle(result - PI);
  }

  /// Check if angle is normalized [0, 2π)
  [[nodiscard]] constexpr bool is_normalized() const noexcept {
    return m_radians >= T(0) && m_radians < TAU;
  }

  // ==================== Arithmetic ====================

  /// Add angles, returns Optional on overflow (for float precision)
  [[nodiscard]] constexpr Optional<Angle>
  checked_add(Angle other) const noexcept {
    T result = m_radians + other.m_radians;
    if (!std::isfinite(result))
      return None;
    return Some(Angle(result));
  }

  /// Subtract angles
  [[nodiscard]] constexpr Optional<Angle>
  checked_sub(Angle other) const noexcept {
    T result = m_radians - other.m_radians;
    if (!std::isfinite(result))
      return None;
    return Some(Angle(result));
  }

  /// Scale angle
  [[nodiscard]] constexpr Optional<Angle> checked_mul(T factor) const noexcept {
    T result = m_radians * factor;
    if (!std::isfinite(result))
      return None;
    return Some(Angle(result));
  }

  /// Divide angle
  [[nodiscard]] constexpr Optional<Angle>
  checked_div(T divisor) const noexcept {
    if (divisor == T(0))
      return None;
    T result = m_radians / divisor;
    if (!std::isfinite(result))
      return None;
    return Some(Angle(result));
  }

  /// Negate angle
  [[nodiscard]] constexpr Angle operator-() const noexcept {
    return Angle(-m_radians);
  }

  // ==================== Comparison ====================

  [[nodiscard]] constexpr bool operator==(const Angle &other) const noexcept {
    return m_radians == other.m_radians;
  }

  [[nodiscard]] constexpr bool operator!=(const Angle &other) const noexcept {
    return m_radians != other.m_radians;
  }

  [[nodiscard]] constexpr bool operator<(const Angle &other) const noexcept {
    return m_radians < other.m_radians;
  }

  [[nodiscard]] constexpr bool operator<=(const Angle &other) const noexcept {
    return m_radians <= other.m_radians;
  }

  [[nodiscard]] constexpr bool operator>(const Angle &other) const noexcept {
    return m_radians > other.m_radians;
  }

  [[nodiscard]] constexpr bool operator>=(const Angle &other) const noexcept {
    return m_radians >= other.m_radians;
  }

  /// Approximate equality with tolerance
  [[nodiscard]] constexpr bool approx_eq(Angle other,
                                         T tolerance = T(1e-9)) const noexcept {
    return std::abs(m_radians - other.m_radians) < tolerance;
  }

  // ==================== Utilities ====================

  /// Check if angle is zero
  [[nodiscard]] constexpr bool is_zero() const noexcept {
    return m_radians == T(0);
  }

  /// Check if angle is acute (< 90°)
  [[nodiscard]] constexpr bool is_acute() const noexcept {
    T abs_rad = std::abs(normalized_signed().m_radians);
    return abs_rad > T(0) && abs_rad < PI / T(2);
  }

  /// Check if angle is right (= 90°)
  [[nodiscard]] constexpr bool is_right(T tolerance = T(1e-9)) const noexcept {
    T abs_rad = std::abs(normalized_signed().m_radians);
    return std::abs(abs_rad - PI / T(2)) < tolerance;
  }

  /// Check if angle is obtuse (> 90° and < 180°)
  [[nodiscard]] constexpr bool is_obtuse() const noexcept {
    T abs_rad = std::abs(normalized_signed().m_radians);
    return abs_rad > PI / T(2) && abs_rad < PI;
  }

  /// Check if angle is straight (= 180°)
  [[nodiscard]] constexpr bool
  is_straight(T tolerance = T(1e-9)) const noexcept {
    T abs_rad = std::abs(normalized_signed().m_radians);
    return std::abs(abs_rad - PI) < tolerance;
  }

  /// Get the supplementary angle (180° - this)
  [[nodiscard]] constexpr Angle supplementary() const noexcept {
    return Angle(PI - m_radians);
  }

  /// Get the complementary angle (90° - this)
  [[nodiscard]] constexpr Angle complementary() const noexcept {
    return Angle(PI / T(2) - m_radians);
  }

  /// Shortest angular distance to another angle
  [[nodiscard]] constexpr Angle
  shortest_distance_to(Angle other) const noexcept {
    T diff = other.m_radians - m_radians;
    diff = std::fmod(diff + PI, TAU);
    if (diff < T(0))
      diff += TAU;
    return Angle(diff - PI);
  }

  // ==================== Stream Output ====================

  friend std::ostream &operator<<(std::ostream &os, const Angle &a) {
    os << a.degrees() << "°";
    return os;
  }
};

// ==================== Literal Operators ====================

namespace literals {

/// Degree literal: 90_deg
[[nodiscard]] constexpr Angle<double>
operator""_deg(long double degrees) noexcept {
  return Angle<double>::from_degrees(static_cast<double>(degrees));
}

[[nodiscard]] constexpr Angle<double>
operator""_deg(unsigned long long degrees) noexcept {
  return Angle<double>::from_degrees(static_cast<double>(degrees));
}

/// Radian literal: 3.14_rad
[[nodiscard]] constexpr Angle<double>
operator""_rad(long double radians) noexcept {
  return Angle<double>::from_radians(static_cast<double>(radians));
}

[[nodiscard]] constexpr Angle<double>
operator""_rad(unsigned long long radians) noexcept {
  return Angle<double>::from_radians(static_cast<double>(radians));
}

} // namespace literals

// ==================== Type Aliases ====================

using Anglef = Angle<float>;
using Angled = Angle<double>;

} // namespace pulgacpp

#endif // PULGACPP_GEOMETRY_ANGLE_HPP
