// pulgacpp::isize - Type-safe signed pointer-sized integer
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_ISIZE_HPP
#define PULGACPP_ISIZE_HPP

#include "../core/safe_int.hpp"

namespace pulgacpp {

/// Type-safe signed pointer-sized integer with Rust-like semantics.
/// This is the signed equivalent of size_t, used for pointer arithmetic and indexing.
/// On 64-bit platforms: 64 bits. On 32-bit platforms: 32 bits.
#if INTPTR_MAX == INT64_MAX
    // 64-bit platform
    #if defined(__SIZEOF_INT128__)
        using isize = detail::SafeInt<std::intptr_t, __int128, 64, true>;
    #else
        using isize = detail::SafeInt<std::intptr_t, std::intptr_t, 64, true>;
    #endif
    constexpr unsigned ISIZE_BITS = 64;
#else
    // 32-bit platform
    using isize = detail::SafeInt<std::intptr_t, std::int64_t, 32, true>;
    constexpr unsigned ISIZE_BITS = 32;
#endif

// Literal suffix for isize (e.g., 1000_isize)
namespace literals {
    [[nodiscard]] constexpr isize operator""_isize(unsigned long long value) {
        if (value > static_cast<unsigned long long>(isize::MAX)) {
            panic("isize literal out of range");
        }
        return isize(static_cast<isize::underlying_type>(value));
    }
} // namespace literals

} // namespace pulgacpp

// std::hash specialization for unordered containers
template <>
struct std::hash<pulgacpp::isize> {
    [[nodiscard]] std::size_t operator()(pulgacpp::isize value) const noexcept {
        return std::hash<pulgacpp::isize::underlying_type>{}(value.get());
    }
};

#endif // PULGACPP_ISIZE_HPP
