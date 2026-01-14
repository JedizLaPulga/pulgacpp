// pulgacpp::i8 - Type-safe signed 8-bit integer with checked arithmetic
// SPDX-License-Identifier: MIT

export module pulgacpp.i8;

export import pulgacpp.optional;

import std;

export namespace pulgacpp {

/// A type-safe signed 8-bit integer with Rust-like semantics.
/// No implicit conversions. Checked arithmetic returns Optional<i8>.
class i8 {
public:
    using underlying_type = std::int8_t;
    static constexpr underlying_type MIN = std::numeric_limits<underlying_type>::min(); // -128
    static constexpr underlying_type MAX = std::numeric_limits<underlying_type>::max(); // 127

    // Constructors - explicit only, no implicit conversions
    constexpr i8() noexcept : m_value(0) {}
    constexpr explicit i8(underlying_type value) noexcept : m_value(value) {}

    // Delete implicit conversions from larger types
    template <typename T>
        requires std::integral<T> && (!std::same_as<T, underlying_type>)
    i8(T) = delete;

    // Factory methods for safe construction
    /// Creates an i8 from a value, returning None if out of range.
    template <std::integral T>
    [[nodiscard]] static constexpr Optional<i8> from(T value) noexcept {
        if (value < static_cast<T>(MIN) || value > static_cast<T>(MAX)) {
            return None;
        }
        return Some(i8(static_cast<underlying_type>(value)));
    }

    /// Creates an i8 from a value, saturating at MIN/MAX if out of range.
    template <std::integral T>
    [[nodiscard]] static constexpr i8 saturating_from(T value) noexcept {
        if (value < static_cast<T>(MIN)) {
            return i8(MIN);
        }
        if (value > static_cast<T>(MAX)) {
            return i8(MAX);
        }
        return i8(static_cast<underlying_type>(value));
    }

    // Accessors
    [[nodiscard]] constexpr underlying_type get() const noexcept { return m_value; }
    [[nodiscard]] constexpr explicit operator underlying_type() const noexcept { return m_value; }

    // Checked arithmetic - returns Optional<i8>
    [[nodiscard]] constexpr Optional<i8> checked_add(i8 rhs) const noexcept {
        std::int16_t result = static_cast<std::int16_t>(m_value) + static_cast<std::int16_t>(rhs.m_value);
        if (result < MIN || result > MAX) {
            return None;
        }
        return Some(i8(static_cast<underlying_type>(result)));
    }

    [[nodiscard]] constexpr Optional<i8> checked_sub(i8 rhs) const noexcept {
        std::int16_t result = static_cast<std::int16_t>(m_value) - static_cast<std::int16_t>(rhs.m_value);
        if (result < MIN || result > MAX) {
            return None;
        }
        return Some(i8(static_cast<underlying_type>(result)));
    }

    [[nodiscard]] constexpr Optional<i8> checked_mul(i8 rhs) const noexcept {
        std::int16_t result = static_cast<std::int16_t>(m_value) * static_cast<std::int16_t>(rhs.m_value);
        if (result < MIN || result > MAX) {
            return None;
        }
        return Some(i8(static_cast<underlying_type>(result)));
    }

    [[nodiscard]] constexpr Optional<i8> checked_div(i8 rhs) const noexcept {
        if (rhs.m_value == 0) {
            return None;
        }
        // Handle overflow case: MIN / -1 = 128, which overflows
        if (m_value == MIN && rhs.m_value == -1) {
            return None;
        }
        return Some(i8(static_cast<underlying_type>(m_value / rhs.m_value)));
    }

    [[nodiscard]] constexpr Optional<i8> checked_rem(i8 rhs) const noexcept {
        if (rhs.m_value == 0) {
            return None;
        }
        return Some(i8(static_cast<underlying_type>(m_value % rhs.m_value)));
    }

    [[nodiscard]] constexpr Optional<i8> checked_neg() const noexcept {
        // -MIN overflows
        if (m_value == MIN) {
            return None;
        }
        return Some(i8(static_cast<underlying_type>(-m_value)));
    }

    [[nodiscard]] constexpr Optional<i8> checked_abs() const noexcept {
        if (m_value == MIN) {
            return None; // abs(MIN) = 128, overflow
        }
        return Some(i8(static_cast<underlying_type>(m_value < 0 ? -m_value : m_value)));
    }

    // Saturating arithmetic - clamps to MIN/MAX
    [[nodiscard]] constexpr i8 saturating_add(i8 rhs) const noexcept {
        std::int16_t result = static_cast<std::int16_t>(m_value) + static_cast<std::int16_t>(rhs.m_value);
        if (result < MIN) return i8(MIN);
        if (result > MAX) return i8(MAX);
        return i8(static_cast<underlying_type>(result));
    }

    [[nodiscard]] constexpr i8 saturating_sub(i8 rhs) const noexcept {
        std::int16_t result = static_cast<std::int16_t>(m_value) - static_cast<std::int16_t>(rhs.m_value);
        if (result < MIN) return i8(MIN);
        if (result > MAX) return i8(MAX);
        return i8(static_cast<underlying_type>(result));
    }

    [[nodiscard]] constexpr i8 saturating_mul(i8 rhs) const noexcept {
        std::int16_t result = static_cast<std::int16_t>(m_value) * static_cast<std::int16_t>(rhs.m_value);
        if (result < MIN) return i8(MIN);
        if (result > MAX) return i8(MAX);
        return i8(static_cast<underlying_type>(result));
    }

    // Wrapping arithmetic - wraps around on overflow
    [[nodiscard]] constexpr i8 wrapping_add(i8 rhs) const noexcept {
        return i8(static_cast<underlying_type>(
            static_cast<std::uint8_t>(m_value) + static_cast<std::uint8_t>(rhs.m_value)));
    }

    [[nodiscard]] constexpr i8 wrapping_sub(i8 rhs) const noexcept {
        return i8(static_cast<underlying_type>(
            static_cast<std::uint8_t>(m_value) - static_cast<std::uint8_t>(rhs.m_value)));
    }

    [[nodiscard]] constexpr i8 wrapping_mul(i8 rhs) const noexcept {
        return i8(static_cast<underlying_type>(
            static_cast<std::uint8_t>(m_value) * static_cast<std::uint8_t>(rhs.m_value)));
    }

    [[nodiscard]] constexpr i8 wrapping_neg() const noexcept {
        return i8(static_cast<underlying_type>(-static_cast<std::uint8_t>(m_value)));
    }

    // Overflowing arithmetic - returns (result, did_overflow)
    [[nodiscard]] constexpr std::pair<i8, bool> overflowing_add(i8 rhs) const noexcept {
        std::int16_t result = static_cast<std::int16_t>(m_value) + static_cast<std::int16_t>(rhs.m_value);
        bool overflow = result < MIN || result > MAX;
        return {wrapping_add(rhs), overflow};
    }

    [[nodiscard]] constexpr std::pair<i8, bool> overflowing_sub(i8 rhs) const noexcept {
        std::int16_t result = static_cast<std::int16_t>(m_value) - static_cast<std::int16_t>(rhs.m_value);
        bool overflow = result < MIN || result > MAX;
        return {wrapping_sub(rhs), overflow};
    }

    [[nodiscard]] constexpr std::pair<i8, bool> overflowing_mul(i8 rhs) const noexcept {
        std::int16_t result = static_cast<std::int16_t>(m_value) * static_cast<std::int16_t>(rhs.m_value);
        bool overflow = result < MIN || result > MAX;
        return {wrapping_mul(rhs), overflow};
    }

    // Bitwise operations
    [[nodiscard]] constexpr i8 operator~() const noexcept {
        return i8(static_cast<underlying_type>(~m_value));
    }

    [[nodiscard]] constexpr i8 operator&(i8 rhs) const noexcept {
        return i8(static_cast<underlying_type>(m_value & rhs.m_value));
    }

    [[nodiscard]] constexpr i8 operator|(i8 rhs) const noexcept {
        return i8(static_cast<underlying_type>(m_value | rhs.m_value));
    }

    [[nodiscard]] constexpr i8 operator^(i8 rhs) const noexcept {
        return i8(static_cast<underlying_type>(m_value ^ rhs.m_value));
    }

    [[nodiscard]] constexpr i8 operator<<(int shift) const noexcept {
        return i8(static_cast<underlying_type>(m_value << shift));
    }

    [[nodiscard]] constexpr i8 operator>>(int shift) const noexcept {
        return i8(static_cast<underlying_type>(m_value >> shift));
    }

    // Compound assignment for bitwise
    constexpr i8& operator&=(i8 rhs) noexcept { m_value &= rhs.m_value; return *this; }
    constexpr i8& operator|=(i8 rhs) noexcept { m_value |= rhs.m_value; return *this; }
    constexpr i8& operator^=(i8 rhs) noexcept { m_value ^= rhs.m_value; return *this; }
    constexpr i8& operator<<=(int shift) noexcept { m_value <<= shift; return *this; }
    constexpr i8& operator>>=(int shift) noexcept { m_value >>= shift; return *this; }

    // Comparison operators
    [[nodiscard]] constexpr auto operator<=>(const i8& other) const noexcept = default;
    [[nodiscard]] constexpr bool operator==(const i8& other) const noexcept = default;

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
        return static_cast<unsigned int>(std::popcount(static_cast<std::uint8_t>(m_value)));
    }

    [[nodiscard]] constexpr unsigned int count_zeros() const noexcept {
        return 8u - count_ones();
    }

    [[nodiscard]] constexpr unsigned int leading_zeros() const noexcept {
        return static_cast<unsigned int>(std::countl_zero(static_cast<std::uint8_t>(m_value)));
    }

    [[nodiscard]] constexpr unsigned int trailing_zeros() const noexcept {
        return static_cast<unsigned int>(std::countr_zero(static_cast<std::uint8_t>(m_value)));
    }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, i8 value) {
        return os << static_cast<int>(value.m_value);
    }

private:
    underlying_type m_value;
};

// Literal suffix for i8 (e.g., 42_i8)
namespace literals {
    [[nodiscard]] constexpr i8 operator""_i8(unsigned long long value) {
        if (value > static_cast<unsigned long long>(i8::MAX)) {
            panic("i8 literal out of range");
        }
        return i8(static_cast<i8::underlying_type>(value));
    }
} // namespace literals

} // namespace pulgacpp
