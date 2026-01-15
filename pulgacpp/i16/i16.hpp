// pulgacpp::i16 - Type-safe signed 16-bit integer
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_I16_HPP
#define PULGACPP_I16_HPP

#include "../core/safe_int.hpp"

namespace pulgacpp {

/// Type-safe signed 16-bit integer with Rust-like semantics.
/// No implicit conversions. Checked arithmetic returns Optional<i16>.
using i16 = detail::SafeInt<std::int16_t, std::int32_t, 16, true>;

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
