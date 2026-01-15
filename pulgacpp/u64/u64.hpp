// pulgacpp::u64 - Type-safe unsigned 64-bit integer
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_U64_HPP
#define PULGACPP_U64_HPP

#include "../core/safe_int.hpp"

namespace pulgacpp {

/// Type-safe unsigned 64-bit integer with Rust-like semantics.
/// No implicit conversions. Checked arithmetic returns Optional<u64>.
#if defined(__SIZEOF_INT128__)
using u64 = detail::SafeInt<std::uint64_t, unsigned __int128, 64, false>;
#else
// Fallback: use uint64_t as wider type (limited overflow detection for some ops)
using u64 = detail::SafeInt<std::uint64_t, std::uint64_t, 64, false>;
#endif

// Literal suffix for u64 (e.g., 10000000000_u64)
namespace literals {
    [[nodiscard]] constexpr u64 operator""_u64(unsigned long long value) {
        // All unsigned long long values fit in u64
        return u64(static_cast<u64::underlying_type>(value));
    }
} // namespace literals

} // namespace pulgacpp

// std::hash specialization for unordered containers
template <>
struct std::hash<pulgacpp::u64> {
    [[nodiscard]] std::size_t operator()(pulgacpp::u64 value) const noexcept {
        return std::hash<pulgacpp::u64::underlying_type>{}(value.get());
    }
};

#endif // PULGACPP_U64_HPP
