# pulgacpp::u64 Documentation

A type-safe unsigned 64-bit integer with Rust-like semantics. No implicit conversions. Checked arithmetic returns `Optional<u64>`.

## Header

```cpp
#include <pulgacpp/u64/u64.hpp>

using namespace pulgacpp;
using namespace pulgacpp::literals;  // For _u64 suffix
```

---

## Constants

| Name | Value | Description |
|------|-------|-------------|
| `u64::MIN` | `0` | Minimum representable value |
| `u64::MAX` | `18,446,744,073,709,551,615` | Maximum representable value |
| `u64::underlying_type` | `std::uint64_t` | The underlying primitive type |
| `u64::BITS` | `64` | Number of bits |

---

## Construction

| Method | Description |
|--------|-------------|
| `u64()` | Default: initializes to `0` |
| `u64(std::uint64_t)` | **Explicit** construction — no implicit conversions allowed |
| `u64::from<T>(val)` | Returns `Optional<u64>` — `None` if out of range |
| `u64::saturating_from<T>(val)` | Clamps to `MIN`/`MAX` if out of range |
| `10000000000_u64` | Literal suffix |

---

## Arithmetic Operations

All operations force explicit handling of overflow.

### Checked (returns `Optional<u64>`)
- `checked_add(u64)`, `checked_sub(u64)`, `checked_mul(u64)`
- `checked_div(u64)`, `checked_rem(u64)`

### Saturating (clamps to bounds)
- `saturating_add(u64)`, `saturating_sub(u64)`, `saturating_mul(u64)`

### Wrapping (wraps around)
- `wrapping_add(u64)`, `wrapping_sub(u64)`, `wrapping_mul(u64)`, `wrapping_neg()`

### Overflowing (returns `pair<u64, bool>`)
- `overflowing_add(u64)`, `overflowing_sub(u64)`, `overflowing_mul(u64)`

---

## Type Conversions

### To Built-in Types
| Method | Returns | Description |
|--------|---------|-------------|
| `to<T>()` | `Optional<T>` | Safe — returns `None` if value doesn't fit |
| `as<T>()` | `T` | Unchecked — static_cast |
| `get()` | `std::uint64_t` | Returns underlying value |

### To Other pulgacpp Types
| Method | Returns | Description |
|--------|---------|-------------|
| `narrow<u32>()` | `Optional<u32>` | Checked narrowing |
| `narrow<u16>()` | `Optional<u16>` | Checked narrowing |
| `narrow<u8>()` | `Optional<u8>` | Checked narrowing |
| `narrow<i64>()` | `Optional<i64>` | Checked (fails if > 9.2Q) |
| `cast<i64>()` | `i64` | Unchecked (wraps) |

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
| `count_zeros()` | `unsigned` | 64 − popcount |
| `leading_zeros()` | `unsigned` | Leading zero bits |
| `trailing_zeros()` | `unsigned` | Trailing zero bits |

**Note:** Unsigned types do not have `is_negative()` or `signum()`.

---

## STL Compatibility

Works with `std::vector`, `std::set`, `std::unordered_set`, `std::map`, and all standard algorithms.

---

## Note on Overflow Detection

For `u64`, the wider type for overflow detection may use `unsigned __int128` on platforms that support it (GCC/Clang on 64-bit). On other platforms, some overflow detection may be limited.
