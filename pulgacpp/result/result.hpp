// pulgacpp::Result - Rust-style Result type for error handling
// SPDX-License-Identifier: MIT

#ifndef PULGACPP_RESULT_HPP
#define PULGACPP_RESULT_HPP

#include "../optional/optional.hpp"  // For panic() and Optional<T>

#include <variant>
#include <utility>
#include <functional>
#include <type_traits>

namespace pulgacpp {

// Forward declarations
template <typename T, typename E>
class Result;

// Tag types for Ok and Err
template <typename T>
struct OkType {
    T value;
    constexpr explicit OkType(T&& v) : value(std::forward<T>(v)) {}
    constexpr explicit OkType(const T& v) : value(v) {}
};

template <typename E>
struct ErrType {
    E error;
    constexpr explicit ErrType(E&& e) : error(std::forward<E>(e)) {}
    constexpr explicit ErrType(const E& e) : error(e) {}
};

// Factory functions
template <typename T>
[[nodiscard]] constexpr OkType<std::decay_t<T>> Ok(T&& value) {
    return OkType<std::decay_t<T>>(std::forward<T>(value));
}

template <typename E>
[[nodiscard]] constexpr ErrType<std::decay_t<E>> Err(E&& error) {
    return ErrType<std::decay_t<E>>(std::forward<E>(error));
}

/// Result<T, E> - A type that represents either success (Ok) or failure (Err)
/// 
/// This is the pulgacpp equivalent of Rust's Result<T, E>.
/// Use it for operations that can fail and need to communicate error information.
///
/// Example:
///   Result<int, std::string> parse(const char* s);
///   auto r = parse("42");
///   if (r.is_ok()) { use(r.unwrap()); }
///   else { log(r.unwrap_err()); }
template <typename T, typename E>
class Result {
public:
    using value_type = T;
    using error_type = E;

private:
    // Internal storage: index 0 = Ok, index 1 = Err
    std::variant<T, E> m_storage;
    
    // Private constructor from variant
    explicit constexpr Result(std::variant<T, E>&& storage) : m_storage(std::move(storage)) {}

public:
    // Constructors from Ok and Err tags
    constexpr Result(OkType<T> ok) : m_storage(std::in_place_index<0>, std::move(ok.value)) {}
    constexpr Result(ErrType<E> err) : m_storage(std::in_place_index<1>, std::move(err.error)) {}

    // Copy and move
    constexpr Result(const Result&) = default;
    constexpr Result(Result&&) noexcept = default;
    constexpr Result& operator=(const Result&) = default;
    constexpr Result& operator=(Result&&) noexcept = default;

    // ==================== State queries ====================

    /// Returns true if the result is Ok
    [[nodiscard]] constexpr bool is_ok() const noexcept {
        return m_storage.index() == 0;
    }

    /// Returns true if the result is Err
    [[nodiscard]] constexpr bool is_err() const noexcept {
        return m_storage.index() == 1;
    }

    /// Converts to bool (true if Ok)
    [[nodiscard]] constexpr explicit operator bool() const noexcept {
        return is_ok();
    }

    // ==================== Value access ====================

    /// Returns the Ok value, panics if Err
    [[nodiscard]] constexpr T unwrap() const& {
        if (is_err()) {
            panic("called unwrap() on an Err value");
        }
        return std::get<0>(m_storage);
    }

    [[nodiscard]] constexpr T unwrap() && {
        if (is_err()) {
            panic("called unwrap() on an Err value");
        }
        return std::get<0>(std::move(m_storage));
    }

    /// Returns the Ok value, panics with message if Err
    [[nodiscard]] constexpr T expect(std::string_view message) const& {
        if (is_err()) {
            panic(message);
        }
        return std::get<0>(m_storage);
    }

    [[nodiscard]] constexpr T expect(std::string_view message) && {
        if (is_err()) {
            panic(message);
        }
        return std::get<0>(std::move(m_storage));
    }

    /// Returns the Err value, panics if Ok
    [[nodiscard]] constexpr E unwrap_err() const& {
        if (is_ok()) {
            panic("called unwrap_err() on an Ok value");
        }
        return std::get<1>(m_storage);
    }

    [[nodiscard]] constexpr E unwrap_err() && {
        if (is_ok()) {
            panic("called unwrap_err() on an Ok value");
        }
        return std::get<1>(std::move(m_storage));
    }

    /// Returns the Err value, panics with message if Ok
    [[nodiscard]] constexpr E expect_err(std::string_view message) const& {
        if (is_ok()) {
            panic(message);
        }
        return std::get<1>(m_storage);
    }

    // ==================== Default values ====================

    /// Returns the Ok value, or a default if Err
    [[nodiscard]] constexpr T unwrap_or(T default_value) const& {
        if (is_ok()) {
            return std::get<0>(m_storage);
        }
        return default_value;
    }

    /// Returns the Ok value, or computes a default from the error
    template <typename F>
        requires std::invocable<F, const E&> && std::convertible_to<std::invoke_result_t<F, const E&>, T>
    [[nodiscard]] constexpr T unwrap_or_else(F&& f) const& {
        if (is_ok()) {
            return std::get<0>(m_storage);
        }
        return std::invoke(std::forward<F>(f), std::get<1>(m_storage));
    }

    // ==================== Transformations ====================

    /// Maps the Ok value using function f, leaving Err unchanged
    template <typename F>
        requires std::invocable<F, const T&>
    [[nodiscard]] constexpr auto map(F&& f) const& -> Result<std::invoke_result_t<F, const T&>, E> {
        using U = std::invoke_result_t<F, const T&>;
        if (is_ok()) {
            return Ok(std::invoke(std::forward<F>(f), std::get<0>(m_storage)));
        }
        return Err(std::get<1>(m_storage));
    }

    /// Maps the Err value using function f, leaving Ok unchanged
    template <typename F>
        requires std::invocable<F, const E&>
    [[nodiscard]] constexpr auto map_err(F&& f) const& -> Result<T, std::invoke_result_t<F, const E&>> {
        using F2 = std::invoke_result_t<F, const E&>;
        if (is_err()) {
            return Err(std::invoke(std::forward<F>(f), std::get<1>(m_storage)));
        }
        return Ok(std::get<0>(m_storage));
    }

    /// Returns res if Ok, otherwise returns the Err value
    template <typename U>
    [[nodiscard]] constexpr Result<U, E> and_result(Result<U, E> res) const& {
        if (is_ok()) {
            return res;
        }
        return Err(std::get<1>(m_storage));
    }

    /// Calls f with the Ok value and returns its result, otherwise returns Err
    template <typename F>
        requires std::invocable<F, const T&>
    [[nodiscard]] constexpr auto and_then(F&& f) const& -> std::invoke_result_t<F, const T&> {
        using ResultType = std::invoke_result_t<F, const T&>;
        if (is_ok()) {
            return std::invoke(std::forward<F>(f), std::get<0>(m_storage));
        }
        return Err(std::get<1>(m_storage));
    }

    /// Returns res if Err, otherwise returns the Ok value
    template <typename F2>
    [[nodiscard]] constexpr Result<T, F2> or_result(Result<T, F2> res) const& {
        if (is_err()) {
            return res;
        }
        return Ok(std::get<0>(m_storage));
    }

    /// Calls f with the Err value and returns its result, otherwise returns Ok
    template <typename F>
        requires std::invocable<F, const E&>
    [[nodiscard]] constexpr auto or_else(F&& f) const& -> std::invoke_result_t<F, const E&> {
        using ResultType = std::invoke_result_t<F, const E&>;
        if (is_err()) {
            return std::invoke(std::forward<F>(f), std::get<1>(m_storage));
        }
        return Ok(std::get<0>(m_storage));
    }

    // ==================== Conversion to Optional ====================

    /// Converts to Optional<T>, discarding the error
    [[nodiscard]] constexpr Optional<T> ok() const& {
        if (is_ok()) {
            return Some(std::get<0>(m_storage));
        }
        return None;
    }

    /// Converts to Optional<E>, discarding the Ok value
    [[nodiscard]] constexpr Optional<E> err() const& {
        if (is_err()) {
            return Some(std::get<1>(m_storage));
        }
        return None;
    }

    // ==================== Comparison ====================

    [[nodiscard]] constexpr bool operator==(const Result& other) const {
        return m_storage == other.m_storage;
    }

    [[nodiscard]] constexpr bool operator!=(const Result& other) const {
        return m_storage != other.m_storage;
    }
};

// Specialization for void Ok type (Result<void, E>)
// This is useful for operations that succeed with no value
template <typename E>
class Result<void, E> {
public:
    using value_type = void;
    using error_type = E;

private:
    std::optional<E> m_error;  // nullopt = Ok, has_value = Err

public:
    // Ok constructor (no value)
    struct OkVoidTag {};
    constexpr Result(OkVoidTag) : m_error(std::nullopt) {}
    
    // Err constructor
    constexpr Result(ErrType<E> err) : m_error(std::move(err.error)) {}

    // Factory for void Ok
    [[nodiscard]] static constexpr Result ok() { return Result(OkVoidTag{}); }

    [[nodiscard]] constexpr bool is_ok() const noexcept { return !m_error.has_value(); }
    [[nodiscard]] constexpr bool is_err() const noexcept { return m_error.has_value(); }
    [[nodiscard]] constexpr explicit operator bool() const noexcept { return is_ok(); }

    constexpr void unwrap() const {
        if (is_err()) {
            panic("called unwrap() on an Err value");
        }
    }

    constexpr void expect(std::string_view message) const {
        if (is_err()) {
            panic(message);
        }
    }

    [[nodiscard]] constexpr E unwrap_err() const {
        if (is_ok()) {
            panic("called unwrap_err() on an Ok value");
        }
        return *m_error;
    }

    [[nodiscard]] constexpr Optional<E> err() const {
        if (is_err()) {
            return Some(*m_error);
        }
        return None;
    }
};

} // namespace pulgacpp

#endif // PULGACPP_RESULT_HPP
