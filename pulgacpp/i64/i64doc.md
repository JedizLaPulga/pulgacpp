# pulgacpp::i64 Documentation

A type-safe signed 64-bit integer with Rust-like semantics. No implicit conversions. Checked arithmetic returns `Optional<i64>`.

## Header

```cpp
#include <pulgacpp/i64/i64.hpp>

using namespace pulgacpp;
using namespace pulgacpp::literals;  // For _i64 suffix
```

---

## Constants

| Name | Value | Description |
|------|-------|-------------|
| `i64::MIN` | `-9,223,372,036,854,775,808` | Minimum representable value |
| `i64::MAX` | `9,223,372,036,854,775,807` | Maximum representable value |
| `i64::underlying_type` | `std::int64_t` | The underlying primitive type |
| `i64::BITS` | `64` | Number of bits |

---

## Construction

| Method | Description |
|--------|-------------|
| `i64()` | Default: initializes to `0` |
| `i64(std::int64_t)` | **Explicit** construction — no implicit conversions allowed |
| `i64::from<T>(val)` | Returns `Optional<i64>` — `None` if out of range |
| `i64::saturating_from<T>(val)` | Clamps to `MIN`/`MAX` if out of range |
| `1000000000_i64` | Literal suffix |

---

## Arithmetic Operations

All operations force explicit handling of overflow.

### Checked (returns `Optional<i64>`)
- `checked_add(i64)`, `checked_sub(i64)`, `checked_mul(i64)`
- `checked_div(i64)`, `checked_rem(i64)`
- `checked_neg()`, `checked_abs()`

### Saturating (clamps to bounds)
- `saturating_add(i64)`, `saturating_sub(i64)`, `saturating_mul(i64)`

### Wrapping (wraps around)
- `wrapping_add(i64)`, `wrapping_sub(i64)`, `wrapping_mul(i64)`, `wrapping_neg()`

### Overflowing (returns `pair<i64, bool>`)
- `overflowing_add(i64)`, `overflowing_sub(i64)`, `overflowing_mul(i64)`

---

## Type Conversions

### To Built-in Types
| Method | Returns | Description |
|--------|---------|-------------|
| `to<T>()` | `Optional<T>` | Safe — returns `None` if value doesn't fit |
| `as<T>()` | `T` | Unchecked — static_cast |
| `get()` | `std::int64_t` | Returns underlying value |

### To Other pulgacpp Types
| Method | Returns | Description |
|--------|---------|-------------|
| `narrow<i32>()` | `Optional<i32>` | Checked narrowing |
| `narrow<i16>()` | `Optional<i16>` | Checked narrowing |
| `narrow<i8>()` | `Optional<i8>` | Checked narrowing |
| `cast<u64>()` | `u64` | Unchecked (wraps) |

---

## Bitwise Operations

`~`, `&`, `|`, `^`, `<<`, `>>` and compound assignments.

---

## Comparison Operators

`==`, `!=`, `<`, `<=`, `>`, `>=`, `<=>` (spaceship)

---

## Utility Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `is_positive()` | `bool` | `true` if > 0 |
| `is_negative()` | `bool` | `true` if < 0 |
| `is_zero()` | `bool` | `true` if == 0 |
| `signum()` | `int` | Returns -1, 0, or 1 |
| `count_ones()` | `unsigned` | Population count |
| `count_zeros()` | `unsigned` | 64 − popcount |
| `leading_zeros()` | `unsigned` | Leading zero bits |
| `trailing_zeros()` | `unsigned` | Trailing zero bits |

---

## STL Compatibility

Works with `std::vector`, `std::set`, `std::unordered_set`, `std::map`, and all standard algorithms.

---

## Note on Overflow Detection

For `i64`, the wider type for overflow detection may use `__int128` on platforms that support it (GCC/Clang on 64-bit). On other platforms, some overflow detection may be limited.
