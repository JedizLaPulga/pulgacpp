// pulgacpp::i8 - Type-safe signed 8-bit integer
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_I8_HPP
#define PULGACPP_I8_HPP

#include "../core/safe_int.hpp"

namespace pulgacpp {

/// Type-safe signed 8-bit integer with Rust-like semantics.
/// No implicit conversions. Checked arithmetic returns Optional<i8>.
using i8 = detail::SafeInt<std::int8_t, std::int16_t, 8, true>;

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

// std::hash specialization for unordered containers
template <>
struct std::hash<pulgacpp::i8> {
    [[nodiscard]] std::size_t operator()(pulgacpp::i8 value) const noexcept {
        return std::hash<pulgacpp::i8::underlying_type>{}(value.get());
    }
};

#endif // PULGACPP_I8_HPP
