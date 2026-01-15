// pulgacpp::u16 - Type-safe unsigned 16-bit integer
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_U16_HPP
#define PULGACPP_U16_HPP

#include "../core/safe_int.hpp"

namespace pulgacpp {

/// Type-safe unsigned 16-bit integer with Rust-like semantics.
/// No implicit conversions. Checked arithmetic returns Optional<u16>.
using u16 = detail::SafeInt<std::uint16_t, std::uint32_t, 16, false>;

// Literal suffix for u16 (e.g., 50000_u16)
namespace literals {
    [[nodiscard]] constexpr u16 operator""_u16(unsigned long long value) {
        if (value > static_cast<unsigned long long>(u16::MAX)) {
            panic("u16 literal out of range");
        }
        return u16(static_cast<u16::underlying_type>(value));
    }
} // namespace literals

} // namespace pulgacpp

// std::hash specialization for unordered containers
template <>
struct std::hash<pulgacpp::u16> {
    [[nodiscard]] std::size_t operator()(pulgacpp::u16 value) const noexcept {
        return std::hash<pulgacpp::u16::underlying_type>{}(value.get());
    }
};

#endif // PULGACPP_U16_HPP
