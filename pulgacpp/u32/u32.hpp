// pulgacpp::u32 - Type-safe unsigned 32-bit integer
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_U32_HPP
#define PULGACPP_U32_HPP

#include "../core/safe_int.hpp"

namespace pulgacpp {

/// Type-safe unsigned 32-bit integer with Rust-like semantics.
/// No implicit conversions. Checked arithmetic returns Optional<u32>.
using u32 = detail::SafeInt<std::uint32_t, std::uint64_t, 32, false>;

// Literal suffix for u32 (e.g., 3000000000_u32)
namespace literals {
    [[nodiscard]] constexpr u32 operator""_u32(unsigned long long value) {
        if (value > static_cast<unsigned long long>(u32::MAX)) {
            panic("u32 literal out of range");
        }
        return u32(static_cast<u32::underlying_type>(value));
    }
} // namespace literals

} // namespace pulgacpp

// std::hash specialization for unordered containers
template <>
struct std::hash<pulgacpp::u32> {
    [[nodiscard]] std::size_t operator()(pulgacpp::u32 value) const noexcept {
        return std::hash<pulgacpp::u32::underlying_type>{}(value.get());
    }
};

#endif // PULGACPP_U32_HPP
