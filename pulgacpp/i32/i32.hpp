// pulgacpp::i32 - Type-safe signed 32-bit integer
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_I32_HPP
#define PULGACPP_I32_HPP

#include "../core/safe_int.hpp"

namespace pulgacpp {

/// Type-safe signed 32-bit integer with Rust-like semantics.
/// No implicit conversions. Checked arithmetic returns Optional<i32>.
using i32 = detail::SafeInt<std::int32_t, std::int64_t, 32, true>;

// Literal suffix for i32 (e.g., 100000_i32)
namespace literals {
    [[nodiscard]] constexpr i32 operator""_i32(unsigned long long value) {
        if (value > static_cast<unsigned long long>(i32::MAX)) {
            panic("i32 literal out of range");
        }
        return i32(static_cast<i32::underlying_type>(value));
    }
} // namespace literals

} // namespace pulgacpp

// std::hash specialization for unordered containers
template <>
struct std::hash<pulgacpp::i32> {
    [[nodiscard]] std::size_t operator()(pulgacpp::i32 value) const noexcept {
        return std::hash<pulgacpp::i32::underlying_type>{}(value.get());
    }
};

#endif // PULGACPP_I32_HPP
