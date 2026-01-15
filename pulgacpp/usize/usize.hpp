// pulgacpp::usize - Type-safe unsigned pointer-sized integer
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_USIZE_HPP
#define PULGACPP_USIZE_HPP

#include "../core/safe_int.hpp"
#include <cstdint>

namespace pulgacpp {

/// Type-safe unsigned pointer-sized integer with Rust-like semantics.
/// This is equivalent to std::size_t, used for sizes and indexing.
/// On 64-bit platforms: 64 bits. On 32-bit platforms: 32 bits.

#if UINTPTR_MAX == UINT64_MAX
    // 64-bit platform: usize is effectively u64
    #if defined(__SIZEOF_INT128__)
        using usize = detail::SafeInt<std::uint64_t, unsigned __int128, 64, false>;
    #else
        using usize = detail::SafeInt<std::uint64_t, std::uint64_t, 64, false>;
    #endif
    constexpr unsigned USIZE_BITS = 64;
#elif UINTPTR_MAX == UINT32_MAX
    // 32-bit platform: usize is effectively u32
    using usize = detail::SafeInt<std::uint32_t, std::uint64_t, 32, false>;
    constexpr unsigned USIZE_BITS = 32;
#else
    #error "Unknown pointer size"
#endif

// Literal suffix for usize (e.g., 1000_usize)
namespace literals {
    [[nodiscard]] constexpr usize operator""_usize(unsigned long long value) {
        // On 64-bit, all values fit; on 32-bit, check range
        #if UINTPTR_MAX != UINT64_MAX
        if (value > static_cast<unsigned long long>(usize::MAX)) {
            panic("usize literal out of range");
        }
        #endif
        return usize(static_cast<usize::underlying_type>(value));
    }
} // namespace literals

} // namespace pulgacpp

// Note: std::hash for usize is provided by the corresponding fixed-width type
// (u64 on 64-bit, u32 on 32-bit), since usize is a type alias to that type.

#endif // PULGACPP_USIZE_HPP
