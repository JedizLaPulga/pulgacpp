// pulgacpp::detail::SafeInt - Template base for type-safe integers
// SPDX-License-Identifier: MIT
//
// This is an internal implementation detail. Users should include
// specific type headers like <pulgacpp/i8/i8.hpp> instead.

#ifndef PULGACPP_CORE_SAFE_INT_HPP
#define PULGACPP_CORE_SAFE_INT_HPP

#include "../optional/optional.hpp"
#include "overflow.hpp"

#include <bit>
#include <concepts>
#include <cstdint>
#include <limits>
#include <ostream>
#include <type_traits>
#include <utility>


namespace pulgacpp::detail {

/// Template base class for type-safe integers with Rust-like semantics.
/// @tparam Underlying The underlying primitive type (e.g., std::int8_t)
/// @tparam Wider A wider type for intermediate calculations (e.g., std::int16_t
/// for i8)
/// @tparam Bits Number of bits (e.g., 8, 16, 32, 64)
/// @tparam IsSigned Whether the type is signed
template <typename Underlying, typename Wider, unsigned Bits, bool IsSigned>
class SafeInt {
public:
  using underlying_type = Underlying;
  using wider_type = Wider;
  using unsigned_type = std::make_unsigned_t<Underlying>;

  static constexpr underlying_type MIN =
      std::numeric_limits<underlying_type>::min();
  static constexpr underlying_type MAX =
      std::numeric_limits<underlying_type>::max();
  static constexpr unsigned BITS = Bits;

  // Constructors - explicit only, no implicit conversions
  constexpr SafeInt() noexcept : m_value(0) {}
  constexpr explicit SafeInt(underlying_type value) noexcept : m_value(value) {}

  // Delete implicit conversions from other types
  template <typename T>
    requires std::integral<T> && (!std::same_as<T, underlying_type>)
  SafeInt(T) = delete;

  // Factory methods for safe construction
  /// Creates a SafeInt from a value, returning None if out of range.
  template <std::integral T>
  [[nodiscard]] static constexpr Optional<SafeInt> from(T value) noexcept {
    // Use the wider type for safe comparison
    auto wide_value = static_cast<wider_type>(value);
    if (wide_value < static_cast<wider_type>(MIN) ||
        wide_value > static_cast<wider_type>(MAX)) {
      return None;
    }
    return Some(SafeInt(static_cast<underlying_type>(value)));
  }

  /// Creates a SafeInt from a value, saturating at MIN/MAX if out of range.
  template <std::integral T>
  [[nodiscard]] static constexpr SafeInt saturating_from(T value) noexcept {
    auto wide_value = static_cast<wider_type>(value);
    if (wide_value < static_cast<wider_type>(MIN)) {
      return SafeInt(MIN);
    }
    if (wide_value > static_cast<wider_type>(MAX)) {
      return SafeInt(MAX);
    }
    return SafeInt(static_cast<underlying_type>(value));
  }

  // Accessors
  [[nodiscard]] constexpr underlying_type get() const noexcept {
    return m_value;
  }
  [[nodiscard]] constexpr explicit operator underlying_type() const noexcept {
    return m_value;
  }

  // Type conversions
  /// Converts to type T, returning None if the value doesn't fit.
  template <std::integral T>
  [[nodiscard]] constexpr Optional<T> to() const noexcept {
    if constexpr (std::is_signed_v<T>) {
      if (m_value < std::numeric_limits<T>::min() ||
          m_value > std::numeric_limits<T>::max()) {
        return None;
      }
    } else {
      if constexpr (IsSigned) {
        if (m_value < 0) {
          return None;
        }
      }
      if (static_cast<std::make_unsigned_t<underlying_type>>(m_value) >
          std::numeric_limits<T>::max()) {
        return None;
      }
    }
    return Some(static_cast<T>(m_value));
  }

  /// Unchecked conversion to type T (like Rust's `as` keyword).
  template <std::integral T> [[nodiscard]] constexpr T as() const noexcept {
    return static_cast<T>(m_value);
  }

  // ==================== Inter-type conversions ====================

  /// Concept to detect a pulgacpp SafeInt type
  template <typename T>
  static constexpr bool is_safe_int_v = requires {
    typename T::underlying_type;
    typename T::wider_type;
    { T::BITS } -> std::convertible_to<unsigned>;
    { T::MIN } -> std::convertible_to<typename T::underlying_type>;
    { T::MAX } -> std::convertible_to<typename T::underlying_type>;
  };

  /// Safe widening conversion to a larger SafeInt type.
  /// The target type must be able to hold all values of this type.
  /// Returns the target type directly (always succeeds for valid widening).
  template <typename Target>
    requires is_safe_int_v<Target> && (Target::BITS >= BITS)
  [[nodiscard]] constexpr Target widen() const noexcept {
    return Target(static_cast<typename Target::underlying_type>(m_value));
  }

  /// Checked narrowing conversion to a smaller SafeInt type.
  /// Returns Optional<Target> - None if the value doesn't fit.
  template <typename Target>
    requires is_safe_int_v<Target>
  [[nodiscard]] constexpr Optional<Target> narrow() const noexcept {
    // Check if value fits in target range
    using target_underlying = typename Target::underlying_type;

    // Handle signed -> unsigned conversion
    if constexpr (!std::is_signed_v<target_underlying> && IsSigned) {
      if (m_value < 0) {
        return None;
      }
    }

    // Check bounds
    if constexpr (std::is_signed_v<target_underlying> &&
                  std::is_signed_v<underlying_type>) {
      // Both signed
      if (m_value < static_cast<underlying_type>(Target::MIN) ||
          m_value > static_cast<underlying_type>(Target::MAX)) {
        return None;
      }
    } else if constexpr (!std::is_signed_v<target_underlying> &&
                         !std::is_signed_v<underlying_type>) {
      // Both unsigned
      if (m_value > static_cast<underlying_type>(Target::MAX)) {
        return None;
      }
    } else if constexpr (std::is_signed_v<target_underlying> &&
                         !std::is_signed_v<underlying_type>) {
      // Unsigned -> signed
      if (m_value > static_cast<underlying_type>(Target::MAX)) {
        return None;
      }
    } else {
      // Signed -> unsigned (already checked for negative above)
      if (static_cast<std::make_unsigned_t<underlying_type>>(m_value) >
          static_cast<std::make_unsigned_t<underlying_type>>(Target::MAX)) {
        return None;
      }
    }

    return Some(Target(static_cast<target_underlying>(m_value)));
  }

  /// Unchecked conversion to another SafeInt type (like Rust's `as`).
  /// Performs a static_cast without bounds checking.
  template <typename Target>
    requires is_safe_int_v<Target>
  [[nodiscard]] constexpr Target cast() const noexcept {
    return Target(static_cast<typename Target::underlying_type>(m_value));
  }

  // Checked arithmetic - returns Optional<SafeInt>
  [[nodiscard]] constexpr Optional<SafeInt>
  checked_add(SafeInt rhs) const noexcept {
    if constexpr (needs_intrinsic_overflow_v<underlying_type, wider_type> &&
                  Bits == 64) {
      // Use intrinsic-based overflow detection for 64-bit types
      if constexpr (IsSigned) {
        auto [result, overflow] = checked_add_i64(m_value, rhs.m_value);
        if (overflow)
          return None;
        return Some(SafeInt(result));
      } else {
        auto [result, overflow] = checked_add_u64(m_value, rhs.m_value);
        if (overflow)
          return None;
        return Some(SafeInt(result));
      }
    } else {
      wider_type result = static_cast<wider_type>(m_value) +
                          static_cast<wider_type>(rhs.m_value);
      if (result < static_cast<wider_type>(MIN) ||
          result > static_cast<wider_type>(MAX)) {
        return None;
      }
      return Some(SafeInt(static_cast<underlying_type>(result)));
    }
  }

  [[nodiscard]] constexpr Optional<SafeInt>
  checked_sub(SafeInt rhs) const noexcept {
    if constexpr (needs_intrinsic_overflow_v<underlying_type, wider_type> &&
                  Bits == 64) {
      if constexpr (IsSigned) {
        auto [result, overflow] = checked_sub_i64(m_value, rhs.m_value);
        if (overflow)
          return None;
        return Some(SafeInt(result));
      } else {
        auto [result, overflow] = checked_sub_u64(m_value, rhs.m_value);
        if (overflow)
          return None;
        return Some(SafeInt(result));
      }
    } else {
      wider_type result = static_cast<wider_type>(m_value) -
                          static_cast<wider_type>(rhs.m_value);
      if (result < static_cast<wider_type>(MIN) ||
          result > static_cast<wider_type>(MAX)) {
        return None;
      }
      return Some(SafeInt(static_cast<underlying_type>(result)));
    }
  }

  [[nodiscard]] constexpr Optional<SafeInt>
  checked_mul(SafeInt rhs) const noexcept {
    if constexpr (needs_intrinsic_overflow_v<underlying_type, wider_type> &&
                  Bits == 64) {
      if constexpr (IsSigned) {
        auto [result, overflow] = checked_mul_i64(m_value, rhs.m_value);
        if (overflow)
          return None;
        return Some(SafeInt(result));
      } else {
        auto [result, overflow] = checked_mul_u64(m_value, rhs.m_value);
        if (overflow)
          return None;
        return Some(SafeInt(result));
      }
    } else {
      wider_type result = static_cast<wider_type>(m_value) *
                          static_cast<wider_type>(rhs.m_value);
      if (result < static_cast<wider_type>(MIN) ||
          result > static_cast<wider_type>(MAX)) {
        return None;
      }
      return Some(SafeInt(static_cast<underlying_type>(result)));
    }
  }

  [[nodiscard]] constexpr Optional<SafeInt>
  checked_div(SafeInt rhs) const noexcept {
    if (rhs.m_value == 0) {
      return None;
    }
    // Handle overflow case for signed: MIN / -1 overflows
    if constexpr (IsSigned) {
      if (m_value == MIN && rhs.m_value == static_cast<underlying_type>(-1)) {
        return None;
      }
    }
    return Some(SafeInt(static_cast<underlying_type>(m_value / rhs.m_value)));
  }

  [[nodiscard]] constexpr Optional<SafeInt>
  checked_rem(SafeInt rhs) const noexcept {
    if (rhs.m_value == 0) {
      return None;
    }
    return Some(SafeInt(static_cast<underlying_type>(m_value % rhs.m_value)));
  }

  [[nodiscard]] constexpr Optional<SafeInt> checked_neg() const noexcept
    requires IsSigned
  {
    if (m_value == MIN) {
      return None;
    }
    return Some(SafeInt(static_cast<underlying_type>(-m_value)));
  }

  [[nodiscard]] constexpr Optional<SafeInt> checked_abs() const noexcept
    requires IsSigned
  {
    if (m_value == MIN) {
      return None;
    }
    return Some(SafeInt(
        static_cast<underlying_type>(m_value < 0 ? -m_value : m_value)));
  }

  // Saturating arithmetic - clamps to MIN/MAX
  [[nodiscard]] constexpr SafeInt saturating_add(SafeInt rhs) const noexcept {
    if constexpr (needs_intrinsic_overflow_v<underlying_type, wider_type> &&
                  Bits == 64) {
      if constexpr (IsSigned) {
        auto [result, overflow] = checked_add_i64(m_value, rhs.m_value);
        if (overflow) {
          // Determine direction: positive overflow -> MAX, negative -> MIN
          return SafeInt((m_value > 0 && rhs.m_value > 0) ? MAX : MIN);
        }
        return SafeInt(result);
      } else {
        auto [result, overflow] = checked_add_u64(m_value, rhs.m_value);
        if (overflow)
          return SafeInt(MAX);
        return SafeInt(result);
      }
    } else {
      wider_type result = static_cast<wider_type>(m_value) +
                          static_cast<wider_type>(rhs.m_value);
      if (result < static_cast<wider_type>(MIN))
        return SafeInt(MIN);
      if (result > static_cast<wider_type>(MAX))
        return SafeInt(MAX);
      return SafeInt(static_cast<underlying_type>(result));
    }
  }

  [[nodiscard]] constexpr SafeInt saturating_sub(SafeInt rhs) const noexcept {
    if constexpr (needs_intrinsic_overflow_v<underlying_type, wider_type> &&
                  Bits == 64) {
      if constexpr (IsSigned) {
        auto [result, overflow] = checked_sub_i64(m_value, rhs.m_value);
        if (overflow) {
          // Determine direction
          return SafeInt((m_value > 0 && rhs.m_value < 0) ? MAX : MIN);
        }
        return SafeInt(result);
      } else {
        auto [result, overflow] = checked_sub_u64(m_value, rhs.m_value);
        if (overflow)
          return SafeInt(MIN); // Underflow for unsigned
        return SafeInt(result);
      }
    } else {
      wider_type result = static_cast<wider_type>(m_value) -
                          static_cast<wider_type>(rhs.m_value);
      if (result < static_cast<wider_type>(MIN))
        return SafeInt(MIN);
      if (result > static_cast<wider_type>(MAX))
        return SafeInt(MAX);
      return SafeInt(static_cast<underlying_type>(result));
    }
  }

  [[nodiscard]] constexpr SafeInt saturating_mul(SafeInt rhs) const noexcept {
    if constexpr (needs_intrinsic_overflow_v<underlying_type, wider_type> &&
                  Bits == 64) {
      if constexpr (IsSigned) {
        auto [result, overflow] = checked_mul_i64(m_value, rhs.m_value);
        if (overflow) {
          // Same sign -> positive overflow, different sign -> negative overflow
          bool same_sign = (m_value >= 0) == (rhs.m_value >= 0);
          return SafeInt(same_sign ? MAX : MIN);
        }
        return SafeInt(result);
      } else {
        auto [result, overflow] = checked_mul_u64(m_value, rhs.m_value);
        if (overflow)
          return SafeInt(MAX);
        return SafeInt(result);
      }
    } else {
      wider_type result = static_cast<wider_type>(m_value) *
                          static_cast<wider_type>(rhs.m_value);
      if (result < static_cast<wider_type>(MIN))
        return SafeInt(MIN);
      if (result > static_cast<wider_type>(MAX))
        return SafeInt(MAX);
      return SafeInt(static_cast<underlying_type>(result));
    }
  }

  // Wrapping arithmetic - wraps around on overflow
  [[nodiscard]] constexpr SafeInt wrapping_add(SafeInt rhs) const noexcept {
    return SafeInt(
        static_cast<underlying_type>(static_cast<unsigned_type>(m_value) +
                                     static_cast<unsigned_type>(rhs.m_value)));
  }

  [[nodiscard]] constexpr SafeInt wrapping_sub(SafeInt rhs) const noexcept {
    return SafeInt(
        static_cast<underlying_type>(static_cast<unsigned_type>(m_value) -
                                     static_cast<unsigned_type>(rhs.m_value)));
  }

  [[nodiscard]] constexpr SafeInt wrapping_mul(SafeInt rhs) const noexcept {
    return SafeInt(
        static_cast<underlying_type>(static_cast<unsigned_type>(m_value) *
                                     static_cast<unsigned_type>(rhs.m_value)));
  }

  [[nodiscard]] constexpr SafeInt wrapping_neg() const noexcept {
    return SafeInt(
        static_cast<underlying_type>(-static_cast<unsigned_type>(m_value)));
  }

  // Overflowing arithmetic - returns (result, did_overflow)
  [[nodiscard]] constexpr std::pair<SafeInt, bool>
  overflowing_add(SafeInt rhs) const noexcept {
    if constexpr (needs_intrinsic_overflow_v<underlying_type, wider_type> &&
                  Bits == 64) {
      if constexpr (IsSigned) {
        auto [result, overflow] = checked_add_i64(m_value, rhs.m_value);
        return {SafeInt(result), overflow};
      } else {
        auto [result, overflow] = checked_add_u64(m_value, rhs.m_value);
        return {SafeInt(result), overflow};
      }
    } else {
      wider_type result = static_cast<wider_type>(m_value) +
                          static_cast<wider_type>(rhs.m_value);
      bool overflow = result < static_cast<wider_type>(MIN) ||
                      result > static_cast<wider_type>(MAX);
      return {wrapping_add(rhs), overflow};
    }
  }

  [[nodiscard]] constexpr std::pair<SafeInt, bool>
  overflowing_sub(SafeInt rhs) const noexcept {
    if constexpr (needs_intrinsic_overflow_v<underlying_type, wider_type> &&
                  Bits == 64) {
      if constexpr (IsSigned) {
        auto [result, overflow] = checked_sub_i64(m_value, rhs.m_value);
        return {SafeInt(result), overflow};
      } else {
        auto [result, overflow] = checked_sub_u64(m_value, rhs.m_value);
        return {SafeInt(result), overflow};
      }
    } else {
      wider_type result = static_cast<wider_type>(m_value) -
                          static_cast<wider_type>(rhs.m_value);
      bool overflow = result < static_cast<wider_type>(MIN) ||
                      result > static_cast<wider_type>(MAX);
      return {wrapping_sub(rhs), overflow};
    }
  }

  [[nodiscard]] constexpr std::pair<SafeInt, bool>
  overflowing_mul(SafeInt rhs) const noexcept {
    if constexpr (needs_intrinsic_overflow_v<underlying_type, wider_type> &&
                  Bits == 64) {
      if constexpr (IsSigned) {
        auto [result, overflow] = checked_mul_i64(m_value, rhs.m_value);
        return {SafeInt(result), overflow};
      } else {
        auto [result, overflow] = checked_mul_u64(m_value, rhs.m_value);
        return {SafeInt(result), overflow};
      }
    } else {
      wider_type result = static_cast<wider_type>(m_value) *
                          static_cast<wider_type>(rhs.m_value);
      bool overflow = result < static_cast<wider_type>(MIN) ||
                      result > static_cast<wider_type>(MAX);
      return {wrapping_mul(rhs), overflow};
    }
  }

  // Bitwise operations
  [[nodiscard]] constexpr SafeInt operator~() const noexcept {
    return SafeInt(static_cast<underlying_type>(~m_value));
  }

  [[nodiscard]] constexpr SafeInt operator&(SafeInt rhs) const noexcept {
    return SafeInt(static_cast<underlying_type>(m_value & rhs.m_value));
  }

  [[nodiscard]] constexpr SafeInt operator|(SafeInt rhs) const noexcept {
    return SafeInt(static_cast<underlying_type>(m_value | rhs.m_value));
  }

  [[nodiscard]] constexpr SafeInt operator^(SafeInt rhs) const noexcept {
    return SafeInt(static_cast<underlying_type>(m_value ^ rhs.m_value));
  }

  [[nodiscard]] constexpr SafeInt operator<<(int shift) const noexcept {
    return SafeInt(static_cast<underlying_type>(m_value << shift));
  }

  [[nodiscard]] constexpr SafeInt operator>>(int shift) const noexcept {
    return SafeInt(static_cast<underlying_type>(m_value >> shift));
  }

  // Compound assignment for bitwise
  constexpr SafeInt &operator&=(SafeInt rhs) noexcept {
    m_value &= rhs.m_value;
    return *this;
  }
  constexpr SafeInt &operator|=(SafeInt rhs) noexcept {
    m_value |= rhs.m_value;
    return *this;
  }
  constexpr SafeInt &operator^=(SafeInt rhs) noexcept {
    m_value ^= rhs.m_value;
    return *this;
  }
  constexpr SafeInt &operator<<=(int shift) noexcept {
    m_value <<= shift;
    return *this;
  }
  constexpr SafeInt &operator>>=(int shift) noexcept {
    m_value >>= shift;
    return *this;
  }

  // Comparison operators
  [[nodiscard]] constexpr auto
  operator<=>(const SafeInt &other) const noexcept = default;
  [[nodiscard]] constexpr bool
  operator==(const SafeInt &other) const noexcept = default;

  // Utility methods
  [[nodiscard]] constexpr bool is_positive() const noexcept {
    return m_value > 0;
  }
  [[nodiscard]] constexpr bool is_negative() const noexcept
    requires IsSigned
  {
    return m_value < 0;
  }
  [[nodiscard]] constexpr bool is_zero() const noexcept { return m_value == 0; }

  [[nodiscard]] constexpr int signum() const noexcept
    requires IsSigned
  {
    if (m_value > 0)
      return 1;
    if (m_value < 0)
      return -1;
    return 0;
  }

  [[nodiscard]] constexpr unsigned int count_ones() const noexcept {
    return static_cast<unsigned int>(
        std::popcount(static_cast<unsigned_type>(m_value)));
  }

  [[nodiscard]] constexpr unsigned int count_zeros() const noexcept {
    return Bits - count_ones();
  }

  [[nodiscard]] constexpr unsigned int leading_zeros() const noexcept {
    return static_cast<unsigned int>(
        std::countl_zero(static_cast<unsigned_type>(m_value)));
  }

  [[nodiscard]] constexpr unsigned int trailing_zeros() const noexcept {
    return static_cast<unsigned int>(
        std::countr_zero(static_cast<unsigned_type>(m_value)));
  }

  // Stream output
  friend std::ostream &operator<<(std::ostream &os, SafeInt value) {
    if constexpr (Bits <= 8) {
      // Print as int to avoid char interpretation
      return os << static_cast<int>(value.m_value);
    } else {
      return os << value.m_value;
    }
  }

private:
  underlying_type m_value;
};

} // namespace pulgacpp::detail

#endif // PULGACPP_CORE_SAFE_INT_HPP
