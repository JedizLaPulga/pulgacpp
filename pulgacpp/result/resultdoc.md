# pulgacpp::Result<T, E> Documentation

A Rust-style Result type for explicit error handling. Represents either success (`Ok`) with a value of type `T`, or failure (`Err`) with an error of type `E`.

## Header

```cpp
#include <pulgacpp/result/result.hpp>

using namespace pulgacpp;
```

---

## Why Result?

| Approach | Problem |
|----------|---------|
| Exceptions | Hidden control flow, performance cost, easy to ignore |
| Error codes | Easy to forget checking, mixes data with errors |
| `Optional<T>` | No error info — only knows *something* failed |
| **`Result<T, E>`** ✅ | Forces handling, carries error details, no exceptions |

---

## Creating Results

```cpp
// Ok - success with a value
Result<int, std::string> success = Ok(42);

// Err - failure with error info
Result<int, std::string> failure = Err(std::string("something went wrong"));

// From a function
Result<i32, std::string> parse_number(const std::string& s) {
    try {
        return Ok(i32(std::stoi(s)));
    } catch (...) {
        return Err(std::string("invalid number: " + s));
    }
}
```

---

## State Queries

| Method | Returns | Description |
|--------|---------|-------------|
| `is_ok()` | `bool` | `true` if success |
| `is_err()` | `bool` | `true` if failure |
| `operator bool()` | `bool` | Same as `is_ok()` |

```cpp
auto result = divide(10_i32, 2_i32);

if (result.is_ok()) {
    // Handle success
}

if (result) {  // Also works
    // Handle success
}
```

---

## Extracting Values

| Method | Returns | On Wrong State |
|--------|---------|----------------|
| `unwrap()` | `T` | Panics |
| `expect("msg")` | `T` | Panics with message |
| `unwrap_err()` | `E` | Panics |
| `expect_err("msg")` | `E` | Panics with message |
| `unwrap_or(default)` | `T` | Returns default |
| `unwrap_or_else(fn)` | `T` | Calls `fn(error)` for default |

```cpp
auto result = parse_number("42");

// Unsafe but quick for prototyping
int value = result.unwrap();

// With custom panic message
int value = result.expect("failed to parse");

// Safe with default
int value = result.unwrap_or(0);

// Safe with computed default
int value = result.unwrap_or_else([](const auto& err) {
    log("Error: " + err);
    return -1;
});
```

---

## Transformations

### `map(fn)` — Transform the success value

```cpp
Result<int, std::string> r = Ok(21);
Result<int, std::string> doubled = r.map([](int x) { return x * 2; });
// doubled = Ok(42)

Result<int, std::string> err = Err(std::string("error"));
Result<int, std::string> still_err = err.map([](int x) { return x * 2; });
// still_err = Err("error")
```

### `map_err(fn)` — Transform the error value

```cpp
Result<int, int> r = Err(404);
Result<int, std::string> with_msg = r.map_err([](int code) {
    return "Error code: " + std::to_string(code);
});
// with_msg = Err("Error code: 404")
```

---

## Chaining Operations

### `and_then(fn)` — Chain operations that can fail

```cpp
Result<int, std::string> parse(const std::string& s);
Result<int, std::string> validate(int n);

auto result = parse("42")
    .and_then(validate)
    .map([](int n) { return n * 2; });

// If any step fails, the error propagates
```

### `or_else(fn)` — Try alternative on failure

```cpp
Result<int, std::string> parse_int(const std::string& s);
Result<int, std::string> parse_hex(const std::string& s);

// Try parsing as int, if that fails try hex
auto result = parse_int(input).or_else([&](const auto&) {
    return parse_hex(input);
});
```

---

## Converting to Optional

| Method | Returns | Description |
|--------|---------|-------------|
| `ok()` | `Optional<T>` | Discards error, keeps value |
| `err()` | `Optional<E>` | Discards value, keeps error |

```cpp
Result<int, std::string> r = Ok(42);

Optional<int> maybe_value = r.ok();      // Some(42)
Optional<std::string> maybe_err = r.err(); // None
```

---

## Void Results

For operations that succeed with no value:

```cpp
Result<void, std::string> write_file(const std::string& path, const std::string& content) {
    if (/* success */) {
        return Result<void, std::string>::ok();
    }
    return Err(std::string("failed to write"));
}

auto result = write_file("test.txt", "hello");
result.unwrap();  // Panics if error, does nothing if ok
```

---

## Complete Example

```cpp
#include <pulgacpp/pulgacpp.hpp>
#include <pulgacpp/result/result.hpp>

using namespace pulgacpp;
using namespace pulgacpp::literals;

enum class MathError { DivisionByZero, Overflow };

Result<i32, MathError> safe_divide(i32 a, i32 b) {
    if (b.is_zero()) {
        return Err(MathError::DivisionByZero);
    }
    if (a.get() == i32::MIN && b.get() == -1) {
        return Err(MathError::Overflow);
    }
    return Ok(i32(a.get() / b.get()));
}

int main() {
    auto result = safe_divide(100_i32, 0_i32);
    
    if (result.is_err()) {
        switch (result.unwrap_err()) {
            case MathError::DivisionByZero:
                std::cout << "Cannot divide by zero\n";
                break;
            case MathError::Overflow:
                std::cout << "Result would overflow\n";
                break;
        }
    } else {
        std::cout << "Result: " << result.unwrap() << "\n";
    }
    
    // Chaining example
    auto chained = safe_divide(100_i32, 5_i32)
        .and_then([](i32 v) { return safe_divide(v, 2_i32); })
        .map([](i32 v) { return v.get() * 10; })
        .unwrap_or(-1);
    
    std::cout << "Chained: " << chained << "\n";  // Output: 100
    
    return 0;
}
```

---

## API Summary

| Category | Methods |
|----------|---------|
| **State** | `is_ok()`, `is_err()`, `operator bool()` |
| **Extract** | `unwrap()`, `expect()`, `unwrap_err()`, `expect_err()` |
| **Defaults** | `unwrap_or()`, `unwrap_or_else()` |
| **Transform** | `map()`, `map_err()` |
| **Chain** | `and_then()`, `or_else()`, `and_result()`, `or_result()` |
| **Convert** | `ok()`, `err()` |
