// pulgacpp::i16 - Type-safe signed 16-bit integer with checked arithmetic
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_I16_HPP
#define PULGACPP_I16_HPP

#include "../optional/optional.hpp"

#include <cstdint>
#include <limits>
#include <utility>
#include <bit>
#include <ostream>
#include <concepts>

namespace pulgacpp {

/// A type-safe signed 16-bit integer with Rust-like semantics.
/// No implicit conversions. Checked arithmetic returns Optional<i16>.
class i16 {
public:
    using underlying_type = std::int16_t;
    static constexpr underlying_type MIN = std::numeric_limits<underlying_type>::min(); // -32768
    static constexpr underlying_type MAX = std::numeric_limits<underlying_type>::max(); // 32767

    // Constructors - explicit only, no implicit conversions
    constexpr i16() noexcept : m_value(0) {}
    constexpr explicit i16(underlying_type value) noexcept : m_value(value) {}

    // Delete implicit conversions from other types
    template <typename T>
        requires std::integral<T> && (!std::same_as<T, underlying_type>)
    i16(T) = delete;

    // Factory methods for safe construction
    /// Creates an i16 from a value, returning None if out of range.
    template <std::integral T>
    [[nodiscard]] static constexpr Optional<i16> from(T value) noexcept {
        // Widen to int32_t for safe comparison
        auto wide_value = static_cast<std::int32_t>(value);
        if (wide_value < static_cast<std::int32_t>(MIN) || wide_value > static_cast<std::int32_t>(MAX)) {
            return None;
        }
        return Some(i16(static_cast<underlying_type>(value)));
    }

    /// Creates an i16 from a value, saturating at MIN/MAX if out of range.
    template <std::integral T>
    [[nodiscard]] static constexpr i16 saturating_from(T value) noexcept {
        if constexpr (std::is_signed_v<T>) {
            if (value < static_cast<T>(MIN)) {
                return i16(MIN);
            }
            if (value > static_cast<T>(MAX)) {
                return i16(MAX);
            }
        } else {
            if (value > static_cast<std::make_unsigned_t<underlying_type>>(MAX)) {
                return i16(MAX);
            }
        }
        return i16(static_cast<underlying_type>(value));
    }

    // Accessors
    [[nodiscard]] constexpr underlying_type get() const noexcept { return m_value; }
    [[nodiscard]] constexpr explicit operator underlying_type() const noexcept { return m_value; }

    // Type conversions
    /// Converts to type T, returning None if the value doesn't fit.
    template <std::integral T>
    [[nodiscard]] constexpr Optional<T> to() const noexcept {
        if constexpr (std::is_signed_v<T>) {
            if (m_value < std::numeric_limits<T>::min() || m_value > std::numeric_limits<T>::max()) {
                return None;
            }
        } else {
            if (m_value < 0) {
                return None;
            }
            if (static_cast<std::make_unsigned_t<underlying_type>>(m_value) > std::numeric_limits<T>::max()) {
                return None;
            }
        }
        return Some(static_cast<T>(m_value));
    }

    /// Unchecked conversion to type T (like Rust's `as` keyword).
    template <std::integral T>
    [[nodiscard]] constexpr T as() const noexcept {
        return static_cast<T>(m_value);
    }

    // Checked arithmetic - returns Optional<i16>
    [[nodiscard]] constexpr Optional<i16> checked_add(i16 rhs) const noexcept {
        std::int32_t result = static_cast<std::int32_t>(m_value) + static_cast<std::int32_t>(rhs.m_value);
        if (result < MIN || result > MAX) {
            return None;
        }
        return Some(i16(static_cast<underlying_type>(result)));
    }

    [[nodiscard]] constexpr Optional<i16> checked_sub(i16 rhs) const noexcept {
        std::int32_t result = static_cast<std::int32_t>(m_value) - static_cast<std::int32_t>(rhs.m_value);
        if (result < MIN || result > MAX) {
            return None;
        }
        return Some(i16(static_cast<underlying_type>(result)));
    }

    [[nodiscard]] constexpr Optional<i16> checked_mul(i16 rhs) const noexcept {
        std::int32_t result = static_cast<std::int32_t>(m_value) * static_cast<std::int32_t>(rhs.m_value);
        if (result < MIN || result > MAX) {
            return None;
        }
        return Some(i16(static_cast<underlying_type>(result)));
    }

    [[nodiscard]] constexpr Optional<i16> checked_div(i16 rhs) const noexcept {
        if (rhs.m_value == 0) {
            return None;
        }
        // Handle overflow case: MIN / -1 = 32768, which overflows
        if (m_value == MIN && rhs.m_value == -1) {
            return None;
        }
        return Some(i16(static_cast<underlying_type>(m_value / rhs.m_value)));
    }

    [[nodiscard]] constexpr Optional<i16> checked_rem(i16 rhs) const noexcept {
        if (rhs.m_value == 0) {
            return None;
        }
        return Some(i16(static_cast<underlying_type>(m_value % rhs.m_value)));
    }

    [[nodiscard]] constexpr Optional<i16> checked_neg() const noexcept {
        if (m_value == MIN) {
            return None;
        }
        return Some(i16(static_cast<underlying_type>(-m_value)));
    }

    [[nodiscard]] constexpr Optional<i16> checked_abs() const noexcept {
        if (m_value == MIN) {
            return None;
        }
        return Some(i16(static_cast<underlying_type>(m_value < 0 ? -m_value : m_value)));
    }

    // Saturating arithmetic - clamps to MIN/MAX
    [[nodiscard]] constexpr i16 saturating_add(i16 rhs) const noexcept {
        std::int32_t result = static_cast<std::int32_t>(m_value) + static_cast<std::int32_t>(rhs.m_value);
        if (result < MIN) return i16(MIN);
        if (result > MAX) return i16(MAX);
        return i16(static_cast<underlying_type>(result));
    }

    [[nodiscard]] constexpr i16 saturating_sub(i16 rhs) const noexcept {
        std::int32_t result = static_cast<std::int32_t>(m_value) - static_cast<std::int32_t>(rhs.m_value);
        if (result < MIN) return i16(MIN);
        if (result > MAX) return i16(MAX);
        return i16(static_cast<underlying_type>(result));
    }

    [[nodiscard]] constexpr i16 saturating_mul(i16 rhs) const noexcept {
        std::int32_t result = static_cast<std::int32_t>(m_value) * static_cast<std::int32_t>(rhs.m_value);
        if (result < MIN) return i16(MIN);
        if (result > MAX) return i16(MAX);
        return i16(static_cast<underlying_type>(result));
    }

    // Wrapping arithmetic - wraps around on overflow
    [[nodiscard]] constexpr i16 wrapping_add(i16 rhs) const noexcept {
        return i16(static_cast<underlying_type>(
            static_cast<std::uint16_t>(m_value) + static_cast<std::uint16_t>(rhs.m_value)));
    }

    [[nodiscard]] constexpr i16 wrapping_sub(i16 rhs) const noexcept {
        return i16(static_cast<underlying_type>(
            static_cast<std::uint16_t>(m_value) - static_cast<std::uint16_t>(rhs.m_value)));
    }

    [[nodiscard]] constexpr i16 wrapping_mul(i16 rhs) const noexcept {
        return i16(static_cast<underlying_type>(
            static_cast<std::uint16_t>(m_value) * static_cast<std::uint16_t>(rhs.m_value)));
    }

    [[nodiscard]] constexpr i16 wrapping_neg() const noexcept {
        return i16(static_cast<underlying_type>(-static_cast<std::uint16_t>(m_value)));
    }

    // Overflowing arithmetic - returns (result, did_overflow)
    [[nodiscard]] constexpr std::pair<i16, bool> overflowing_add(i16 rhs) const noexcept {
        std::int32_t result = static_cast<std::int32_t>(m_value) + static_cast<std::int32_t>(rhs.m_value);
        bool overflow = result < MIN || result > MAX;
        return {wrapping_add(rhs), overflow};
    }

    [[nodiscard]] constexpr std::pair<i16, bool> overflowing_sub(i16 rhs) const noexcept {
        std::int32_t result = static_cast<std::int32_t>(m_value) - static_cast<std::int32_t>(rhs.m_value);
        bool overflow = result < MIN || result > MAX;
        return {wrapping_sub(rhs), overflow};
    }

    [[nodiscard]] constexpr std::pair<i16, bool> overflowing_mul(i16 rhs) const noexcept {
        std::int32_t result = static_cast<std::int32_t>(m_value) * static_cast<std::int32_t>(rhs.m_value);
        bool overflow = result < MIN || result > MAX;
        return {wrapping_mul(rhs), overflow};
    }

    // Bitwise operations
    [[nodiscard]] constexpr i16 operator~() const noexcept {
        return i16(static_cast<underlying_type>(~m_value));
    }

    [[nodiscard]] constexpr i16 operator&(i16 rhs) const noexcept {
        return i16(static_cast<underlying_type>(m_value & rhs.m_value));
    }

    [[nodiscard]] constexpr i16 operator|(i16 rhs) const noexcept {
        return i16(static_cast<underlying_type>(m_value | rhs.m_value));
    }

    [[nodiscard]] constexpr i16 operator^(i16 rhs) const noexcept {
        return i16(static_cast<underlying_type>(m_value ^ rhs.m_value));
    }

    [[nodiscard]] constexpr i16 operator<<(int shift) const noexcept {
        return i16(static_cast<underlying_type>(m_value << shift));
    }

    [[nodiscard]] constexpr i16 operator>>(int shift) const noexcept {
        return i16(static_cast<underlying_type>(m_value >> shift));
    }

    // Compound assignment for bitwise
    constexpr i16& operator&=(i16 rhs) noexcept { m_value &= rhs.m_value; return *this; }
    constexpr i16& operator|=(i16 rhs) noexcept { m_value |= rhs.m_value; return *this; }
    constexpr i16& operator^=(i16 rhs) noexcept { m_value ^= rhs.m_value; return *this; }
    constexpr i16& operator<<=(int shift) noexcept { m_value <<= shift; return *this; }
    constexpr i16& operator>>=(int shift) noexcept { m_value >>= shift; return *this; }

    // Comparison operators
    [[nodiscard]] constexpr auto operator<=>(const i16& other) const noexcept = default;
    [[nodiscard]] constexpr bool operator==(const i16& other) const noexcept = default;

    // Utility methods
    [[nodiscard]] constexpr bool is_positive() const noexcept { return m_value > 0; }
    [[nodiscard]] constexpr bool is_negative() const noexcept { return m_value < 0; }
    [[nodiscard]] constexpr bool is_zero() const noexcept { return m_value == 0; }

    [[nodiscard]] constexpr int signum() const noexcept {
        if (m_value > 0) return 1;
        if (m_value < 0) return -1;
        return 0;
    }

    [[nodiscard]] constexpr unsigned int count_ones() const noexcept {
        return static_cast<unsigned int>(std::popcount(static_cast<std::uint16_t>(m_value)));
    }

    [[nodiscard]] constexpr unsigned int count_zeros() const noexcept {
        return 16u - count_ones();
    }

    [[nodiscard]] constexpr unsigned int leading_zeros() const noexcept {
        return static_cast<unsigned int>(std::countl_zero(static_cast<std::uint16_t>(m_value)));
    }

    [[nodiscard]] constexpr unsigned int trailing_zeros() const noexcept {
        return static_cast<unsigned int>(std::countr_zero(static_cast<std::uint16_t>(m_value)));
    }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, i16 value) {
        return os << static_cast<int>(value.m_value);
    }

private:
    underlying_type m_value;
};

// Literal suffix for i16 (e.g., 1000_i16)
namespace literals {
    [[nodiscard]] constexpr i16 operator""_i16(unsigned long long value) {
        if (value > static_cast<unsigned long long>(i16::MAX)) {
            panic("i16 literal out of range");
        }
        return i16(static_cast<i16::underlying_type>(value));
    }
} // namespace literals

} // namespace pulgacpp

// std::hash specialization for unordered containers
template <>
struct std::hash<pulgacpp::i16> {
    [[nodiscard]] std::size_t operator()(pulgacpp::i16 value) const noexcept {
        return std::hash<pulgacpp::i16::underlying_type>{}(value.get());
    }
};

#endif // PULGACPP_I16_HPP
