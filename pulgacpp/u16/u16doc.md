# pulgacpp::u16 Documentation

A type-safe unsigned 16-bit integer with Rust-like semantics. No implicit conversions. Checked arithmetic returns `Optional<u16>`.

## Header

```cpp
#include <pulgacpp/u16/u16.hpp>

using namespace pulgacpp;
using namespace pulgacpp::literals;  // For _u16 suffix
```

---

## Constants

| Name | Value | Description |
|------|-------|-------------|
| `u16::MIN` | `0` | Minimum representable value |
| `u16::MAX` | `65,535` | Maximum representable value |
| `u16::underlying_type` | `std::uint16_t` | The underlying primitive type |
| `u16::BITS` | `16` | Number of bits |

---

## Construction

| Method | Description |
|--------|-------------|
| `u16()` | Default: initializes to `0` |
| `u16(std::uint16_t)` | **Explicit** construction — no implicit conversions allowed |
| `u16::from<T>(val)` | Returns `Optional<u16>` — `None` if out of range |
| `u16::saturating_from<T>(val)` | Clamps to `MIN`/`MAX` if out of range |
| `50000_u16` | Literal suffix |

---

## Arithmetic Operations

All operations force explicit handling of overflow.

### Checked (returns `Optional<u16>`)
- `checked_add(u16)`, `checked_sub(u16)`, `checked_mul(u16)`
- `checked_div(u16)`, `checked_rem(u16)`

### Saturating (clamps to bounds)
- `saturating_add(u16)`, `saturating_sub(u16)`, `saturating_mul(u16)`

### Wrapping (wraps around)
- `wrapping_add(u16)`, `wrapping_sub(u16)`, `wrapping_mul(u16)`, `wrapping_neg()`

### Overflowing (returns `pair<u16, bool>`)
- `overflowing_add(u16)`, `overflowing_sub(u16)`, `overflowing_mul(u16)`

---

## Type Conversions

### To Built-in Types
| Method | Returns | Description |
|--------|---------|-------------|
| `to<T>()` | `Optional<T>` | Safe — returns `None` if value doesn't fit |
| `as<T>()` | `T` | Unchecked — static_cast |
| `get()` | `std::uint16_t` | Returns underlying value |

### To Other pulgacpp Types
| Method | Returns | Description |
|--------|---------|-------------|
| `widen<u32>()` | `u32` | Safe widening |
| `widen<u64>()` | `u64` | Safe widening |
| `narrow<u8>()` | `Optional<u8>` | Checked narrowing |
| `narrow<i16>()` | `Optional<i16>` | Checked (fails if > 32767) |
| `cast<i16>()` | `i16` | Unchecked (wraps) |

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
| `count_zeros()` | `unsigned` | 16 − popcount |
| `leading_zeros()` | `unsigned` | Leading zero bits |
| `trailing_zeros()` | `unsigned` | Trailing zero bits |

**Note:** Unsigned types do not have `is_negative()` or `signum()`.

---

## STL Compatibility

Works with `std::vector`, `std::set`, `std::unordered_set`, `std::map`, and all standard algorithms.
