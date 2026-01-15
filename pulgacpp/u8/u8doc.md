# pulgacpp::u8 Documentation

A type-safe unsigned 8-bit integer with Rust-like semantics. No implicit conversions. Checked arithmetic returns `Optional<u8>`.

## Header

```cpp
#include <pulgacpp/u8/u8.hpp>

using namespace pulgacpp;
using namespace pulgacpp::literals;  // For _u8 suffix
```

---

## Constants

| Name | Value | Description |
|------|-------|-------------|
| `u8::MIN` | `0` | Minimum representable value |
| `u8::MAX` | `255` | Maximum representable value |
| `u8::underlying_type` | `std::uint8_t` | The underlying primitive type |
| `u8::BITS` | `8` | Number of bits |

---

## Construction

| Method | Description |
|--------|-------------|
| `u8()` | Default: initializes to `0` |
| `u8(std::uint8_t)` | **Explicit** construction — no implicit conversions allowed |
| `u8::from<T>(val)` | Returns `Optional<u8>` — `None` if out of range |
| `u8::saturating_from<T>(val)` | Clamps to `MIN`/`MAX` if out of range |
| `200_u8` | Literal suffix |

---

## Arithmetic Operations

All operations force explicit handling of overflow.

### Checked (returns `Optional<u8>`)
- `checked_add(u8)`, `checked_sub(u8)`, `checked_mul(u8)`
- `checked_div(u8)`, `checked_rem(u8)`

### Saturating (clamps to bounds)
- `saturating_add(u8)`, `saturating_sub(u8)`, `saturating_mul(u8)`

### Wrapping (wraps around)
- `wrapping_add(u8)`, `wrapping_sub(u8)`, `wrapping_mul(u8)`, `wrapping_neg()`

### Overflowing (returns `pair<u8, bool>`)
- `overflowing_add(u8)`, `overflowing_sub(u8)`, `overflowing_mul(u8)`

---

## Type Conversions

### To Built-in Types
| Method | Returns | Description |
|--------|---------|-------------|
| `to<T>()` | `Optional<T>` | Safe — returns `None` if value doesn't fit |
| `as<T>()` | `T` | Unchecked — static_cast |
| `get()` | `std::uint8_t` | Returns underlying value |

### To Other pulgacpp Types
| Method | Returns | Description |
|--------|---------|-------------|
| `widen<u16>()` | `u16` | Safe widening |
| `widen<u32>()` | `u32` | Safe widening |
| `widen<u64>()` | `u64` | Safe widening |
| `narrow<i8>()` | `Optional<i8>` | Checked (fails if > 127) |
| `cast<i8>()` | `i8` | Unchecked (wraps) |

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
| `is_zero()` | `bool` | `true` if == 0 |
| `count_ones()` | `unsigned` | Population count |
| `count_zeros()` | `unsigned` | 8 − popcount |
| `leading_zeros()` | `unsigned` | Leading zero bits |
| `trailing_zeros()` | `unsigned` | Trailing zero bits |

**Note:** Unsigned types do not have `is_negative()` or `signum()`.

---

## STL Compatibility

Works with `std::vector`, `std::set`, `std::unordered_set`, `std::map`, and all standard algorithms.
