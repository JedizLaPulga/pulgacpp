# pulgacpp::isize Documentation

A type-safe signed pointer-sized integer with Rust-like semantics. The size matches the platform's pointer size:
- **64-bit platforms**: 64 bits (equivalent to `std::int64_t`)
- **32-bit platforms**: 32 bits (equivalent to `std::int32_t`)

No implicit conversions. Checked arithmetic returns `Optional<isize>`.

## Header

```cpp
#include <pulgacpp/isize/isize.hpp>

using namespace pulgacpp;
using namespace pulgacpp::literals;  // For _isize suffix
```

---

## When to Use

Use `isize` for:
- **Pointer arithmetic** differences
- **Signed indices** (when negative values are meaningful)
- **Interop with C APIs** that use `ptrdiff_t` or `ssize_t`

Use `usize` for:
- **Container sizes** and capacities
- **Array/vector indexing**
- **Memory sizes**

---

## Constants

| Name | Value (64-bit) | Value (32-bit) |
|------|----------------|----------------|
| `isize::MIN` | −9,223,372,036,854,775,808 | −2,147,483,648 |
| `isize::MAX` | 9,223,372,036,854,775,807 | 2,147,483,647 |
| `isize::BITS` | 64 | 32 |
| `isize::underlying_type` | `std::intptr_t` | `std::intptr_t` |

---

## Construction

| Method | Description |
|--------|-------------|
| `isize()` | Default: initializes to `0` |
| `isize(std::intptr_t)` | **Explicit** construction |
| `isize::from<T>(val)` | Returns `Optional<isize>` — `None` if out of range |
| `isize::saturating_from<T>(val)` | Clamps to `MIN`/`MAX` if out of range |
| `1000_isize` | Literal suffix |

---

## Arithmetic Operations

All operations force explicit handling of overflow.

### Checked (returns `Optional<isize>`)
- `checked_add(isize)`, `checked_sub(isize)`, `checked_mul(isize)`
- `checked_div(isize)`, `checked_rem(isize)`
- `checked_neg()`, `checked_abs()`

### Saturating (clamps to bounds)
- `saturating_add(isize)`, `saturating_sub(isize)`, `saturating_mul(isize)`

### Wrapping (wraps around)
- `wrapping_add(isize)`, `wrapping_sub(isize)`, `wrapping_mul(isize)`, `wrapping_neg()`

### Overflowing (returns `pair<isize, bool>`)
- `overflowing_add(isize)`, `overflowing_sub(isize)`, `overflowing_mul(isize)`

---

## Type Conversions

### To Other pulgacpp Types
| Method | Returns | Description |
|--------|---------|-------------|
| `narrow<i32>()` | `Optional<i32>` | Checked narrowing |
| `narrow<i16>()` | `Optional<i16>` | Checked narrowing |
| `narrow<i8>()` | `Optional<i8>` | Checked narrowing |
| `cast<usize>()` | `usize` | Unchecked (wraps) |

---

## Utility Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `is_positive()` | `bool` | `true` if > 0 |
| `is_negative()` | `bool` | `true` if < 0 |
| `is_zero()` | `bool` | `true` if == 0 |
| `signum()` | `int` | Returns -1, 0, or 1 |
| `count_ones()` | `unsigned` | Population count |
| `leading_zeros()` | `unsigned` | Leading zero bits |
| `trailing_zeros()` | `unsigned` | Trailing zero bits |

---

## STL Compatibility

Works with `std::vector`, `std::set`, `std::unordered_set`, `std::map`, and all standard algorithms.

---

## Platform Detection

```cpp
// Check the platform at compile time
static_assert(isize::BITS == 64);  // On 64-bit
static_assert(isize::BITS == 32);  // On 32-bit
```
