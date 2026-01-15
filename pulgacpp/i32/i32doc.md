# pulgacpp::i32 Documentation

A type-safe signed 32-bit integer with Rust-like semantics. No implicit conversions. Checked arithmetic returns `Optional<i32>`.

## Header

```cpp
#include <pulgacpp/i32/i32.hpp>

using namespace pulgacpp;
using namespace pulgacpp::literals;  // For _i32 suffix
```

---

## Constants

| Name | Value | Description |
|------|-------|-------------|
| `i32::MIN` | `-2,147,483,648` | Minimum representable value |
| `i32::MAX` | `2,147,483,647` | Maximum representable value |
| `i32::underlying_type` | `std::int32_t` | The underlying primitive type |
| `i32::BITS` | `32` | Number of bits |

---

## Construction

| Method | Description |
|--------|-------------|
| `i32()` | Default: initializes to `0` |
| `i32(std::int32_t)` | **Explicit** construction — no implicit conversions allowed |
| `i32::from<T>(val)` | Returns `Optional<i32>` — `None` if out of range |
| `i32::saturating_from<T>(val)` | Clamps to `MIN`/`MAX` if out of range |
| `1000000_i32` | Literal suffix |

---

## Arithmetic Operations

All operations force explicit handling of overflow.

### Checked (returns `Optional<i32>`)
- `checked_add(i32)`, `checked_sub(i32)`, `checked_mul(i32)`
- `checked_div(i32)`, `checked_rem(i32)`
- `checked_neg()`, `checked_abs()`

### Saturating (clamps to bounds)
- `saturating_add(i32)`, `saturating_sub(i32)`, `saturating_mul(i32)`

### Wrapping (wraps around)
- `wrapping_add(i32)`, `wrapping_sub(i32)`, `wrapping_mul(i32)`, `wrapping_neg()`

### Overflowing (returns `pair<i32, bool>`)
- `overflowing_add(i32)`, `overflowing_sub(i32)`, `overflowing_mul(i32)`

---

## Type Conversions

### To Built-in Types
| Method | Returns | Description |
|--------|---------|-------------|
| `to<T>()` | `Optional<T>` | Safe — returns `None` if value doesn't fit |
| `as<T>()` | `T` | Unchecked — static_cast |
| `get()` | `std::int32_t` | Returns underlying value |

### To Other pulgacpp Types
| Method | Returns | Description |
|--------|---------|-------------|
| `widen<i64>()` | `i64` | Safe widening |
| `narrow<i16>()` | `Optional<i16>` | Checked narrowing |
| `cast<u32>()` | `u32` | Unchecked (wraps) |

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
| `count_zeros()` | `unsigned` | 32 − popcount |
| `leading_zeros()` | `unsigned` | Leading zero bits |
| `trailing_zeros()` | `unsigned` | Trailing zero bits |

---

## STL Compatibility

Works with `std::vector`, `std::set`, `std::unordered_set`, `std::map`, and all standard algorithms.
