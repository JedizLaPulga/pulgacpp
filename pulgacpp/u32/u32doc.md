# pulgacpp::u32 Documentation

A type-safe unsigned 32-bit integer with Rust-like semantics. No implicit conversions. Checked arithmetic returns `Optional<u32>`.

## Header

```cpp
#include <pulgacpp/u32/u32.hpp>

using namespace pulgacpp;
using namespace pulgacpp::literals;  // For _u32 suffix
```

---

## Constants

| Name | Value | Description |
|------|-------|-------------|
| `u32::MIN` | `0` | Minimum representable value |
| `u32::MAX` | `4,294,967,295` | Maximum representable value |
| `u32::underlying_type` | `std::uint32_t` | The underlying primitive type |
| `u32::BITS` | `32` | Number of bits |

---

## Construction

| Method | Description |
|--------|-------------|
| `u32()` | Default: initializes to `0` |
| `u32(std::uint32_t)` | **Explicit** construction — no implicit conversions allowed |
| `u32::from<T>(val)` | Returns `Optional<u32>` — `None` if out of range |
| `u32::saturating_from<T>(val)` | Clamps to `MIN`/`MAX` if out of range |
| `3000000000_u32` | Literal suffix |

---

## Arithmetic Operations

All operations force explicit handling of overflow.

### Checked (returns `Optional<u32>`)
- `checked_add(u32)`, `checked_sub(u32)`, `checked_mul(u32)`
- `checked_div(u32)`, `checked_rem(u32)`

### Saturating (clamps to bounds)
- `saturating_add(u32)`, `saturating_sub(u32)`, `saturating_mul(u32)`

### Wrapping (wraps around)
- `wrapping_add(u32)`, `wrapping_sub(u32)`, `wrapping_mul(u32)`, `wrapping_neg()`

### Overflowing (returns `pair<u32, bool>`)
- `overflowing_add(u32)`, `overflowing_sub(u32)`, `overflowing_mul(u32)`

---

## Type Conversions

### To Built-in Types
| Method | Returns | Description |
|--------|---------|-------------|
| `to<T>()` | `Optional<T>` | Safe — returns `None` if value doesn't fit |
| `as<T>()` | `T` | Unchecked — static_cast |
| `get()` | `std::uint32_t` | Returns underlying value |

### To Other pulgacpp Types
| Method | Returns | Description |
|--------|---------|-------------|
| `widen<u64>()` | `u64` | Safe widening |
| `narrow<u16>()` | `Optional<u16>` | Checked narrowing |
| `narrow<u8>()` | `Optional<u8>` | Checked narrowing |
| `narrow<i32>()` | `Optional<i32>` | Checked (fails if > 2.1B) |
| `cast<i32>()` | `i32` | Unchecked (wraps) |

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
| `count_zeros()` | `unsigned` | 32 − popcount |
| `leading_zeros()` | `unsigned` | Leading zero bits |
| `trailing_zeros()` | `unsigned` | Trailing zero bits |

**Note:** Unsigned types do not have `is_negative()` or `signum()`.

---

## STL Compatibility

Works with `std::vector`, `std::set`, `std::unordered_set`, `std::map`, and all standard algorithms.
