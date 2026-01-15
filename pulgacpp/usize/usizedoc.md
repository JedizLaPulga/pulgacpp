# pulgacpp::usize Documentation

A type-safe unsigned pointer-sized integer with Rust-like semantics. The size matches the platform's pointer size:
- **64-bit platforms**: 64 bits (equivalent to `std::uint64_t`)
- **32-bit platforms**: 32 bits (equivalent to `std::uint32_t`)

This is the pulgacpp equivalent of `std::size_t`.

No implicit conversions. Checked arithmetic returns `Optional<usize>`.

## Header

```cpp
#include <pulgacpp/usize/usize.hpp>

using namespace pulgacpp;
using namespace pulgacpp::literals;  // For _usize suffix
```

---

## When to Use

Use `usize` for:
- **Container sizes** and capacities
- **Array/vector indexing**
- **Memory sizes** and byte counts
- **Loop counters** for collections

Use `isize` for:
- **Pointer arithmetic** differences
- **Signed indices** (when negative values are meaningful)

---

## Constants

| Name | Value (64-bit) | Value (32-bit) |
|------|----------------|----------------|
| `usize::MIN` | 0 | 0 |
| `usize::MAX` | 18,446,744,073,709,551,615 | 4,294,967,295 |
| `usize::BITS` | 64 | 32 |
| `usize::underlying_type` | `std::uintptr_t` | `std::uintptr_t` |

---

## Construction

| Method | Description |
|--------|-------------|
| `usize()` | Default: initializes to `0` |
| `usize(std::uintptr_t)` | **Explicit** construction |
| `usize::from<T>(val)` | Returns `Optional<usize>` — `None` if out of range |
| `usize::saturating_from<T>(val)` | Clamps to `MIN`/`MAX` if out of range |
| `1000_usize` | Literal suffix |

---

## Arithmetic Operations

All operations force explicit handling of overflow.

### Checked (returns `Optional<usize>`)
- `checked_add(usize)`, `checked_sub(usize)`, `checked_mul(usize)`
- `checked_div(usize)`, `checked_rem(usize)`

### Saturating (clamps to bounds)
- `saturating_add(usize)`, `saturating_sub(usize)`, `saturating_mul(usize)`

### Wrapping (wraps around)
- `wrapping_add(usize)`, `wrapping_sub(usize)`, `wrapping_mul(usize)`, `wrapping_neg()`

### Overflowing (returns `pair<usize, bool>`)
- `overflowing_add(usize)`, `overflowing_sub(usize)`, `overflowing_mul(usize)`

---

## Type Conversions

### To Other pulgacpp Types
| Method | Returns | Description |
|--------|---------|-------------|
| `narrow<u32>()` | `Optional<u32>` | Checked narrowing |
| `narrow<u16>()` | `Optional<u16>` | Checked narrowing |
| `narrow<u8>()` | `Optional<u8>` | Checked narrowing |
| `narrow<isize>()` | `Optional<isize>` | Checked (fails if > isize::MAX) |
| `cast<isize>()` | `isize` | Unchecked (wraps) |

---

## Utility Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `is_positive()` | `bool` | `true` if > 0 |
| `is_zero()` | `bool` | `true` if == 0 |
| `count_ones()` | `unsigned` | Population count |
| `count_zeros()` | `unsigned` | BITS − popcount |
| `leading_zeros()` | `unsigned` | Leading zero bits |
| `trailing_zeros()` | `unsigned` | Trailing zero bits |

**Note:** Unsigned types do not have `is_negative()` or `signum()`.

---

## Common Use Cases

### Safe Indexing
```cpp
auto index = 5_usize;
auto len = container.size();

if (index.get() < len) {
    auto& item = container[index.get()];
}
```

### Safe Size Calculations
```cpp
auto width = 1920_usize;
auto height = 1080_usize;
auto total = width.checked_mul(height);  // Optional<usize>

if (total.is_some()) {
    allocate(total.unwrap().get());
}
```

---

## STL Compatibility

Works with `std::vector`, `std::set`, `std::unordered_set`, `std::map`, and all standard algorithms.

---

## Platform Detection

```cpp
// Check the platform at compile time
static_assert(usize::BITS == 64);  // On 64-bit
static_assert(usize::BITS == 32);  // On 32-bit
```
