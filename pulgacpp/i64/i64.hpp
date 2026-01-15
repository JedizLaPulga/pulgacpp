// pulgacpp::i64 - Type-safe signed 64-bit integer
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_I64_HPP
#define PULGACPP_I64_HPP

#include "../core/safe_int.hpp"

namespace pulgacpp {

/// Type-safe signed 64-bit integer with Rust-like semantics.
/// No implicit conversions. Checked arithmetic returns Optional<i64>.
/// Note: Uses __int128 for wider type if available, otherwise uses overflow detection.
#if defined(__SIZEOF_INT128__)
using i64 = detail::SafeInt<std::int64_t, __int128, 64, true>;
#else
// Fallback: use int64_t as wider type (limited overflow detection for some ops)
using i64 = detail::SafeInt<std::int64_t, std::int64_t, 64, true>;
#endif

// Literal suffix for i64 (e.g., 1000000000_i64)
namespace literals {
    [[nodiscard]] constexpr i64 operator""_i64(unsigned long long value) {
        if (value > static_cast<unsigned long long>(i64::MAX)) {
            panic("i64 literal out of range");
        }
        return i64(static_cast<i64::underlying_type>(value));
    }
} // namespace literals

} // namespace pulgacpp

// std::hash specialization for unordered containers
template <>
struct std::hash<pulgacpp::i64> {
    [[nodiscard]] std::size_t operator()(pulgacpp::i64 value) const noexcept {
        return std::hash<pulgacpp::i64::underlying_type>{}(value.get());
    }
};

#endif // PULGACPP_I64_HPP
