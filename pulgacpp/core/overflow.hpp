// pulgacpp::detail::overflow - Cross-platform overflow detection for 64-bit
// arithmetic SPDX-License-Identifier: MIT
//
// This provides compiler-intrinsic-based overflow detection for cases where
// no wider integer type is available (i.e., 64-bit types on MSVC).

#ifndef PULGACPP_CORE_OVERFLOW_HPP
#define PULGACPP_CORE_OVERFLOW_HPP

#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>


// Platform-specific intrinsics
#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#define PULGACPP_MSVC_INTRINSICS 1
#else
#define PULGACPP_MSVC_INTRINSICS 0
#endif

namespace pulgacpp {
namespace detail {

// ============================================================
// Type trait to detect if we need intrinsic-based overflow detection
// This is true when Underlying == Wider (no wider type available)
// ============================================================
template <typename Underlying, typename Wider>
constexpr bool needs_intrinsic_overflow_v = std::is_same_v<Underlying, Wider>;

// ============================================================
// Signed 64-bit overflow detection
// ============================================================

/// Checked addition for signed 64-bit integers.
/// Returns {result, overflowed}
[[nodiscard]] inline std::pair<std::int64_t, bool>
checked_add_i64(std::int64_t a, std::int64_t b) noexcept {
#if defined(__has_builtin)
#if __has_builtin(__builtin_add_overflow)
  std::int64_t result = 0;
  bool overflow = __builtin_add_overflow(a, b, &result);
  return {result, overflow};
#endif
#endif
#if PULGACPP_MSVC_INTRINSICS
  // MSVC: Manual overflow detection for signed addition
  // Overflow occurs if signs of operands are same but result sign differs
  std::int64_t result = static_cast<std::int64_t>(
      static_cast<std::uint64_t>(a) + static_cast<std::uint64_t>(b));
  bool overflow = ((a ^ result) & (b ^ result)) < 0;
  return {result, overflow};
#elif defined(__SIZEOF_INT128__)
  // Use 128-bit arithmetic where available
  __int128 wide = static_cast<__int128>(a) + static_cast<__int128>(b);
  bool overflow = wide < INT64_MIN || wide > INT64_MAX;
  return {static_cast<std::int64_t>(wide), overflow};
#else
  // Portable fallback
  std::int64_t result = static_cast<std::int64_t>(
      static_cast<std::uint64_t>(a) + static_cast<std::uint64_t>(b));
  bool overflow = ((a ^ result) & (b ^ result)) < 0;
  return {result, overflow};
#endif
}

/// Checked subtraction for signed 64-bit integers.
/// Returns {result, overflowed}
[[nodiscard]] inline std::pair<std::int64_t, bool>
checked_sub_i64(std::int64_t a, std::int64_t b) noexcept {
#if defined(__has_builtin)
#if __has_builtin(__builtin_sub_overflow)
  std::int64_t result = 0;
  bool overflow = __builtin_sub_overflow(a, b, &result);
  return {result, overflow};
#endif
#endif
#if PULGACPP_MSVC_INTRINSICS
  // Overflow occurs if operands have different signs and result sign differs
  // from a
  std::int64_t result = static_cast<std::int64_t>(
      static_cast<std::uint64_t>(a) - static_cast<std::uint64_t>(b));
  bool overflow = ((a ^ b) & (a ^ result)) < 0;
  return {result, overflow};
#elif defined(__SIZEOF_INT128__)
  __int128 wide = static_cast<__int128>(a) - static_cast<__int128>(b);
  bool overflow = wide < INT64_MIN || wide > INT64_MAX;
  return {static_cast<std::int64_t>(wide), overflow};
#else
  std::int64_t result = static_cast<std::int64_t>(
      static_cast<std::uint64_t>(a) - static_cast<std::uint64_t>(b));
  bool overflow = ((a ^ b) & (a ^ result)) < 0;
  return {result, overflow};
#endif
}

/// Checked multiplication for signed 64-bit integers.
/// Returns {result, overflowed}
[[nodiscard]] inline std::pair<std::int64_t, bool>
checked_mul_i64(std::int64_t a, std::int64_t b) noexcept {
#if defined(__has_builtin)
#if __has_builtin(__builtin_mul_overflow)
  std::int64_t result = 0;
  bool overflow = __builtin_mul_overflow(a, b, &result);
  return {result, overflow};
#endif
#endif
#if PULGACPP_MSVC_INTRINSICS
  // Use __mulh to get high 64 bits of 128-bit signed product
  std::int64_t result = a * b;
  std::int64_t high = __mulh(a, b);
  // No overflow if high bits are all sign extension of result
  bool overflow = (high != (result >> 63));
  return {result, overflow};
#elif defined(__SIZEOF_INT128__)
  __int128 wide = static_cast<__int128>(a) * static_cast<__int128>(b);
  bool overflow = wide < INT64_MIN || wide > INT64_MAX;
  return {static_cast<std::int64_t>(wide), overflow};
#else
  // Portable fallback: careful manual check
  std::int64_t result = a * b;
  bool overflow = false;
  if (a != 0) {
    if (a == -1 && b == (std::numeric_limits<std::int64_t>::min)()) {
      overflow = true;
    } else if (b == -1 && a == (std::numeric_limits<std::int64_t>::min)()) {
      overflow = true;
    } else {
      overflow = (result / a != b);
    }
  }
  return {result, overflow};
#endif
}

// ============================================================
// Unsigned 64-bit overflow detection
// ============================================================

/// Checked addition for unsigned 64-bit integers.
/// Returns {result, overflowed}
[[nodiscard]] inline std::pair<std::uint64_t, bool>
checked_add_u64(std::uint64_t a, std::uint64_t b) noexcept {
#if defined(__has_builtin)
#if __has_builtin(__builtin_add_overflow)
  std::uint64_t result = 0;
  bool overflow = __builtin_add_overflow(a, b, &result);
  return {result, overflow};
#endif
#endif
#if PULGACPP_MSVC_INTRINSICS
  // Use _addcarry_u64 intrinsic
  std::uint64_t result = 0;
  unsigned char carry = _addcarry_u64(0, a, b, &result);
  return {result, carry != 0};
#elif defined(__SIZEOF_INT128__)
  unsigned __int128 wide =
      static_cast<unsigned __int128>(a) + static_cast<unsigned __int128>(b);
  bool overflow = wide > UINT64_MAX;
  return {static_cast<std::uint64_t>(wide), overflow};
#else
  // Portable: overflow if result < either operand
  std::uint64_t result = a + b;
  bool overflow = result < a;
  return {result, overflow};
#endif
}

/// Checked subtraction for unsigned 64-bit integers.
/// Returns {result, underflowed}
[[nodiscard]] inline std::pair<std::uint64_t, bool>
checked_sub_u64(std::uint64_t a, std::uint64_t b) noexcept {
#if defined(__has_builtin)
#if __has_builtin(__builtin_sub_overflow)
  std::uint64_t result = 0;
  bool overflow = __builtin_sub_overflow(a, b, &result);
  return {result, overflow};
#endif
#endif
#if PULGACPP_MSVC_INTRINSICS
  // Use _subborrow_u64 intrinsic
  std::uint64_t result = 0;
  unsigned char borrow = _subborrow_u64(0, a, b, &result);
  return {result, borrow != 0};
#elif defined(__SIZEOF_INT128__)
  bool overflow = b > a;
  return {static_cast<std::uint64_t>(a - b), overflow};
#else
  // Portable: underflow if b > a
  bool overflow = b > a;
  return {a - b, overflow};
#endif
}

/// Checked multiplication for unsigned 64-bit integers.
/// Returns {result, overflowed}
[[nodiscard]] inline std::pair<std::uint64_t, bool>
checked_mul_u64(std::uint64_t a, std::uint64_t b) noexcept {
#if defined(__has_builtin)
#if __has_builtin(__builtin_mul_overflow)
  std::uint64_t result = 0;
  bool overflow = __builtin_mul_overflow(a, b, &result);
  return {result, overflow};
#endif
#endif
#if PULGACPP_MSVC_INTRINSICS
  // Use __umulh to get high 64 bits of 128-bit unsigned product
  std::uint64_t result = a * b;
  std::uint64_t high = __umulh(a, b);
  // Overflow if any high bits are set
  bool overflow = (high != 0);
  return {result, overflow};
#elif defined(__SIZEOF_INT128__)
  unsigned __int128 wide =
      static_cast<unsigned __int128>(a) * static_cast<unsigned __int128>(b);
  bool overflow = wide > UINT64_MAX;
  return {static_cast<std::uint64_t>(wide), overflow};
#else
  // Portable fallback
  std::uint64_t result = a * b;
  bool overflow = (a != 0) && (result / a != b);
  return {result, overflow};
#endif
}

} // namespace detail
} // namespace pulgacpp

#endif // PULGACPP_CORE_OVERFLOW_HPP
