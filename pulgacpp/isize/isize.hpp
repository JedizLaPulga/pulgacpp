// pulgacpp::isize - Type-safe signed pointer-sized integer
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_ISIZE_HPP
#define PULGACPP_ISIZE_HPP

#include "../core/safe_int.hpp"
#include <cstdint>

namespace pulgacpp {

/// Type-safe signed pointer-sized integer with Rust-like semantics.
/// This is the signed equivalent of size_t, used for pointer arithmetic and indexing.
/// On 64-bit platforms: 64 bits. On 32-bit platforms: 32 bits.

#if INTPTR_MAX == INT64_MAX
    // 64-bit platform: isize is effectively i64
    // We use a distinct type tag to differentiate from i64
    namespace detail {
        struct isize_tag {};
    }
    #if defined(__SIZEOF_INT128__)
        using isize = detail::SafeInt<std::int64_t, __int128, 64, true>;
    #else
        using isize = detail::SafeInt<std::int64_t, std::int64_t, 64, true>;
    #endif
    constexpr unsigned ISIZE_BITS = 64;
    // On 64-bit, isize IS i64, so no separate hash needed
    constexpr bool ISIZE_IS_DISTINCT = false;
#elif INTPTR_MAX == INT32_MAX
    // 32-bit platform: isize is effectively i32
    using isize = detail::SafeInt<std::int32_t, std::int64_t, 32, true>;
    constexpr unsigned ISIZE_BITS = 32;
    constexpr bool ISIZE_IS_DISTINCT = false;
#else
    #error "Unknown pointer size"
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

// Note: std::hash for isize is provided by the corresponding fixed-width type
// (i64 on 64-bit, i32 on 32-bit), since isize is a type alias to that type.

#endif // PULGACPP_ISIZE_HPP
