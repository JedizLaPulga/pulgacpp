// pulgacpp::u8 - Type-safe unsigned 8-bit integer
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_U8_HPP
#define PULGACPP_U8_HPP

#include "../core/safe_int.hpp"

namespace pulgacpp {

/// Type-safe unsigned 8-bit integer with Rust-like semantics.
/// No implicit conversions. Checked arithmetic returns Optional<u8>.
using u8 = detail::SafeInt<std::uint8_t, std::uint16_t, 8, false>;

// Literal suffix for u8 (e.g., 200_u8)
namespace literals {
    [[nodiscard]] constexpr u8 operator""_u8(unsigned long long value) {
        if (value > static_cast<unsigned long long>(u8::MAX)) {
            panic("u8 literal out of range");
        }
        return u8(static_cast<u8::underlying_type>(value));
    }
} // namespace literals

} // namespace pulgacpp

// std::hash specialization for unordered containers
template <>
struct std::hash<pulgacpp::u8> {
    [[nodiscard]] std::size_t operator()(pulgacpp::u8 value) const noexcept {
        return std::hash<pulgacpp::u8::underlying_type>{}(value.get());
    }
};

#endif // PULGACPP_U8_HPP
