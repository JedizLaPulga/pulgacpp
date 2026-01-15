// pulgacpp::usize - Type-safe unsigned pointer-sized integer
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_USIZE_HPP
#define PULGACPP_USIZE_HPP

#include "../core/safe_int.hpp"

namespace pulgacpp {

/// Type-safe unsigned pointer-sized integer with Rust-like semantics.
/// This is equivalent to std::size_t, used for sizes and indexing.
/// On 64-bit platforms: 64 bits. On 32-bit platforms: 32 bits.
#if UINTPTR_MAX == UINT64_MAX
    // 64-bit platform
    #if defined(__SIZEOF_INT128__)
        using usize = detail::SafeInt<std::uintptr_t, unsigned __int128, 64, false>;
    #else
        using usize = detail::SafeInt<std::uintptr_t, std::uintptr_t, 64, false>;
    #endif
    constexpr unsigned USIZE_BITS = 64;
#else
    // 32-bit platform
    using usize = detail::SafeInt<std::uintptr_t, std::uint64_t, 32, false>;
    constexpr unsigned USIZE_BITS = 32;
#endif

// Literal suffix for usize (e.g., 1000_usize)
namespace literals {
    [[nodiscard]] constexpr usize operator""_usize(unsigned long long value) {
        // All unsigned long long values fit in usize on 64-bit
        // On 32-bit, check the range
        #if UINTPTR_MAX != UINT64_MAX
        if (value > static_cast<unsigned long long>(usize::MAX)) {
            panic("usize literal out of range");
        }
        #endif
        return usize(static_cast<usize::underlying_type>(value));
    }
} // namespace literals

} // namespace pulgacpp

// std::hash specialization for unordered containers
template <>
struct std::hash<pulgacpp::usize> {
    [[nodiscard]] std::size_t operator()(pulgacpp::usize value) const noexcept {
        return std::hash<pulgacpp::usize::underlying_type>{}(value.get());
    }
};

#endif // PULGACPP_USIZE_HPP
