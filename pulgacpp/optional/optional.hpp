// pulgacpp::optional - Rust-inspired Optional type with no exceptions
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_OPTIONAL_HPP
#define PULGACPP_OPTIONAL_HPP

#include <optional>
#include <string_view>
#include <functional>
#include <concepts>
#include <utility>
#include <cstdlib>
#include <cstdio>

namespace pulgacpp {

/// Terminates the program with an error message (no exceptions).
[[noreturn]] inline void panic(std::string_view message) noexcept {
    std::fprintf(stderr, "panic: %.*s\n", static_cast<int>(message.size()), message.data());
    std::abort();
}

/// A Rust-inspired Optional type wrapping std::optional with .expect() and .unwrap().
template <typename T>
class Optional {
public:
    // Constructors
    constexpr Optional() noexcept : m_value(std::nullopt) {}
    constexpr Optional(std::nullopt_t) noexcept : m_value(std::nullopt) {}
    constexpr Optional(T value) noexcept : m_value(std::move(value)) {}
    constexpr Optional(const Optional&) = default;
    constexpr Optional(Optional&&) noexcept = default;
    constexpr Optional& operator=(const Optional&) = default;
    constexpr Optional& operator=(Optional&&) noexcept = default;

    // Observers
    [[nodiscard]] constexpr bool has_value() const noexcept { return m_value.has_value(); }
    [[nodiscard]] constexpr explicit operator bool() const noexcept { return has_value(); }
    [[nodiscard]] constexpr bool is_some() const noexcept { return has_value(); }
    [[nodiscard]] constexpr bool is_none() const noexcept { return !has_value(); }

    /// Returns the contained value, or panics with the provided message.
    [[nodiscard]] constexpr T expect(std::string_view message) const& {
        if (!has_value()) {
            panic(message);
        }
        return *m_value;
    }

    [[nodiscard]] constexpr T expect(std::string_view message) && {
        if (!has_value()) {
            panic(message);
        }
        return std::move(*m_value);
    }

    /// Returns the contained value, or panics with a generic message.
    [[nodiscard]] constexpr T unwrap() const& {
        return expect("called unwrap() on a None value");
    }

    [[nodiscard]] constexpr T unwrap() && {
        return std::move(*this).expect("called unwrap() on a None value");
    }

    /// Returns the contained value, or the provided default.
    [[nodiscard]] constexpr T unwrap_or(T default_value) const& noexcept {
        return has_value() ? *m_value : std::move(default_value);
    }

    [[nodiscard]] constexpr T unwrap_or(T default_value) && noexcept {
        return has_value() ? std::move(*m_value) : std::move(default_value);
    }

    /// Returns the contained value, or computes it from the provided function.
    template <typename F>
        requires std::invocable<F> && std::convertible_to<std::invoke_result_t<F>, T>
    [[nodiscard]] constexpr T unwrap_or_else(F&& f) const& {
        return has_value() ? *m_value : std::invoke(std::forward<F>(f));
    }

    template <typename F>
        requires std::invocable<F> && std::convertible_to<std::invoke_result_t<F>, T>
    [[nodiscard]] constexpr T unwrap_or_else(F&& f) && {
        return has_value() ? std::move(*m_value) : std::invoke(std::forward<F>(f));
    }

    /// Maps the contained value using the provided function.
    template <typename F>
        requires std::invocable<F, T>
    [[nodiscard]] constexpr auto map(F&& f) const& -> Optional<std::invoke_result_t<F, T>> {
        if (has_value()) {
            return Optional<std::invoke_result_t<F, T>>(std::invoke(std::forward<F>(f), *m_value));
        }
        return std::nullopt;
    }

    template <typename F>
        requires std::invocable<F, T>
    [[nodiscard]] constexpr auto map(F&& f) && -> Optional<std::invoke_result_t<F, T>> {
        if (has_value()) {
            return Optional<std::invoke_result_t<F, T>>(std::invoke(std::forward<F>(f), std::move(*m_value)));
        }
        return std::nullopt;
    }

    /// Returns None if this is None, otherwise returns the provided optional.
    template <typename U>
    [[nodiscard]] constexpr Optional<U> and_then(Optional<U> other) const noexcept {
        return has_value() ? other : std::nullopt;
    }

    /// Returns this if it contains a value, otherwise returns the provided optional.
    [[nodiscard]] constexpr Optional or_else(Optional other) const noexcept {
        return has_value() ? *this : other;
    }

    // Comparison operators
    [[nodiscard]] constexpr bool operator==(const Optional& other) const noexcept = default;
    [[nodiscard]] constexpr bool operator==(std::nullopt_t) const noexcept { return is_none(); }

    template <typename U>
        requires std::equality_comparable_with<T, U>
    [[nodiscard]] constexpr bool operator==(const U& value) const noexcept {
        return has_value() && *m_value == value;
    }

private:
    std::optional<T> m_value;
};

// Deduction guide
template <typename T>
Optional(T) -> Optional<T>;

/// Factory function for creating a Some value.
template <typename T>
[[nodiscard]] constexpr Optional<std::decay_t<T>> Some(T&& value) {
    return Optional<std::decay_t<T>>(std::forward<T>(value));
}

/// Factory constant for None.
inline constexpr std::nullopt_t None = std::nullopt;

} // namespace pulgacpp

#endif // PULGACPP_OPTIONAL_HPP
