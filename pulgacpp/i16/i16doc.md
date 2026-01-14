# pulgacpp::i16 Documentation

A type-safe signed 16-bit integer with Rust-like semantics. No implicit conversions. Checked arithmetic returns `Optional<i16>`.

## Header

```cpp
#include <pulgacpp/i16/i16.hpp>

using namespace pulgacpp;
using namespace pulgacpp::literals;  // For _i16 suffix
```

---

## Constants

| Name | Value | Description |
|------|-------|-------------|
| `i16::MIN` | `-32768` | Minimum representable value |
| `i16::MAX` | `32767` | Maximum representable value |
| `i16::underlying_type` | `std::int16_t` | The underlying primitive type |

---

## Construction

| Method | Description |
|--------|-------------|
| `i16()` | Default: initializes to `0` |
| `i16(std::int16_t)` | **Explicit** construction — no implicit conversions allowed |
| `i16::from<T>(val)` | Returns `Optional<i16>` — `None` if out of range |
| `i16::saturating_from<T>(val)` | Clamps to `MIN`/`MAX` if out of range |
| `1000_i16` | Literal suffix (requires `using namespace pulgacpp::literals;`) |

### Examples

```cpp
i16 a;                                     // a = 0
i16 b(static_cast<std::int16_t>(1000));    // b = 1000
auto c = 30000_i16;                        // c = 30000

auto d = i16::from(50000);                 // d = None (overflow)
auto e = i16::from(10000);                 // e = Some(10000)
auto f = i16::saturating_from(100000);     // f = 32767 (clamped to MAX)
```

---

## Checked Arithmetic

All checked operations return `Optional<i16>`. Returns `None` on overflow, underflow, or division by zero.

| Method | Description |
|--------|-------------|
| `checked_add(i16)` | Returns `None` on overflow |
| `checked_sub(i16)` | Returns `None` on underflow |
| `checked_mul(i16)` | Returns `None` on overflow |
| `checked_div(i16)` | Returns `None` on division by zero or `MIN / -1` |
| `checked_rem(i16)` | Returns `None` on division by zero |
| `checked_neg()` | Returns `None` if negating `MIN` (would overflow) |
| `checked_abs()` | Returns `None` if `abs(MIN)` (would overflow) |

### Examples

```cpp
auto a = 30000_i16;
auto result = a.checked_add(10000_i16);  // None (overflow: 40000 > 32767)

auto b = 10000_i16;
auto sum = b.checked_add(5000_i16);      // Some(15000)
auto value = sum.expect("overflow");     // 15000

auto div = 10000_i16.checked_div(0_i16); // None (division by zero)
```

---

## Saturating Arithmetic

Operations clamp the result to `MIN` or `MAX` instead of overflowing.

| Method | Description |
|--------|-------------|
| `saturating_add(i16)` | Clamps result to `MIN`/`MAX` |
| `saturating_sub(i16)` | Clamps result to `MIN`/`MAX` |
| `saturating_mul(i16)` | Clamps result to `MIN`/`MAX` |

### Examples

```cpp
auto a = 30000_i16;
auto b = a.saturating_add(30000_i16);   // b = 32767 (MAX)

auto c = i16(i16::MIN);
auto d = c.saturating_sub(1_i16);       // d = -32768 (MIN, unchanged)
```

---

## Wrapping Arithmetic

Operations wrap around on overflow (like C's undefined behavior, but defined).

| Method | Description |
|--------|-------------|
| `wrapping_add(i16)` | Wraps around on overflow |
| `wrapping_sub(i16)` | Wraps around on underflow |
| `wrapping_mul(i16)` | Wraps around on overflow |
| `wrapping_neg()` | Wraps around if negating `MIN` |

### Examples

```cpp
auto max = i16(i16::MAX);              // 32767
auto wrapped = max.wrapping_add(1_i16); // -32768 (wraps to MIN)

auto min = i16(i16::MIN);              // -32768
auto wrapped2 = min.wrapping_sub(1_i16); // 32767 (wraps to MAX)
```

---

## Overflowing Arithmetic

Returns a `std::pair<i16, bool>` containing the wrapped result and a flag indicating if overflow occurred.

| Method | Returns |
|--------|---------|
| `overflowing_add(i16)` | `(wrapped_result, did_overflow)` |
| `overflowing_sub(i16)` | `(wrapped_result, did_overflow)` |
| `overflowing_mul(i16)` | `(wrapped_result, did_overflow)` |

### Examples

```cpp
auto [result, overflow] = i16(i16::MAX).overflowing_add(1_i16);
// result = -32768, overflow = true

auto [result2, overflow2] = 1000_i16.overflowing_add(1000_i16);
// result2 = 2000, overflow2 = false
```

---

## Bitwise Operations

| Operator | Description |
|----------|-------------|
| `~a` | Bitwise NOT |
| `a & b` | Bitwise AND |
| `a \| b` | Bitwise OR |
| `a ^ b` | Bitwise XOR |
| `a << n` | Left shift by `n` bits |
| `a >> n` | Right shift by `n` bits |
| `a &= b` | Compound AND |
| `a \|= b` | Compound OR |
| `a ^= b` | Compound XOR |
| `a <<= n` | Compound left shift |
| `a >>= n` | Compound right shift |

### Examples

```cpp
auto a = i16(static_cast<std::int16_t>(0x00FF));
auto b = i16(static_cast<std::int16_t>(0xFF00));

auto c = a & b;   // 0x0000
auto d = a | b;   // 0xFFFF
auto e = a ^ b;   // 0xFFFF
auto f = a << 4;  // 0x0FF0
auto g = a >> 4;  // 0x000F
```

---

## Comparison Operators

| Operator | Description |
|----------|-------------|
| `a == b` | Equality |
| `a != b` | Inequality (auto-generated) |
| `a < b` | Less than (auto-generated from `<=>`) |
| `a <= b` | Less than or equal (auto-generated) |
| `a > b` | Greater than (auto-generated) |
| `a >= b` | Greater than or equal (auto-generated) |
| `a <=> b` | Three-way comparison (C++20 spaceship) |

### Examples

```cpp
auto a = 5000_i16;
auto b = 10000_i16;

bool eq = (a == b);   // false
bool lt = (a < b);    // true
bool ge = (b >= a);   // true
```

---

## Utility Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `get()` | `std::int16_t` | Returns the underlying value |
| `is_positive()` | `bool` | `true` if value > 0 |
| `is_negative()` | `bool` | `true` if value < 0 |
| `is_zero()` | `bool` | `true` if value == 0 |
| `signum()` | `int` | Returns `-1`, `0`, or `1` |
| `count_ones()` | `unsigned` | Population count (number of 1 bits) |
| `count_zeros()` | `unsigned` | Number of 0 bits (16 − popcount) |
| `leading_zeros()` | `unsigned` | Number of leading zero bits |
| `trailing_zeros()` | `unsigned` | Number of trailing zero bits |

### Examples

```cpp
auto a = 5000_i16;
auto b = i16(static_cast<std::int16_t>(-5000));
auto c = 0_i16;

a.is_positive();  // true
b.is_negative();  // true
c.is_zero();      // true

a.signum();       // 1
b.signum();       // -1
c.signum();       // 0

auto val = i16(static_cast<std::int16_t>(0x00FF));  // 8 ones in low byte
val.count_ones();      // 8
val.count_zeros();     // 8
val.leading_zeros();   // 8
val.trailing_zeros();  // 0
```

---

## Type Conversions

### Safe Conversion: `to<T>()`

Converts to type `T`, returning `Optional<T>`. Returns `None` if the value doesn't fit in the target type.

| Target Type | Behavior |
|-------------|----------|
| `int8_t` | Returns `None` if value outside [-128, 127] |
| Larger signed (`int32_t`, `int64_t`) | Always succeeds (widening) |
| Unsigned (`uint8_t`, `uint16_t`, etc.) | Returns `None` if value is negative |

```cpp
auto a = 5000_i16;

// Widening - always succeeds
auto i32_val = a.to<std::int32_t>();  // Some(5000)

// Narrowing - may fail
auto i8_val = a.to<std::int8_t>();    // None (5000 > 127)

auto small = 50_i16;
auto i8_small = small.to<std::int8_t>();  // Some(50)

// To unsigned - fails for negative
auto neg = i16(static_cast<std::int16_t>(-100));
auto u16_neg = neg.to<std::uint16_t>();   // None
```

### Unchecked Conversion: `as<T>()`

Performs an unchecked `static_cast` to type `T` (like Rust's `as` keyword). Use when you know the conversion is safe or want C-style wrapping behavior.

```cpp
auto a = 5000_i16;

// Widening
int x = a.as<int>();  // 5000

// Negative to unsigned - wraps (like C)
auto neg = i16(static_cast<std::int16_t>(-1));
std::uint16_t u = neg.as<std::uint16_t>();  // 65535
```

### Legacy Accessors

| Method | Description |
|--------|-------------|
| `get()` | Returns the underlying `std::int16_t` value |
| `static_cast<std::int16_t>(a)` | Explicit conversion operator |

---

## Stream Output

```cpp
std::cout << my_i16;  // Prints as integer (e.g., "5000")
```

---

## Overloaded Operators Summary

### Explicitly Defined: 15 operators

| Category | Operators | Count |
|----------|-----------|-------|
| Bitwise | `~`, `&`, `\|`, `^`, `<<`, `>>` | 6 |
| Compound Bitwise | `&=`, `\|=`, `^=`, `<<=`, `>>=` | 5 |
| Comparison | `==`, `<=>` | 2 |
| Conversion | `explicit operator std::int16_t()` | 1 |
| Stream | `operator<<` (friend) | 1 |

### Effective: 19 operators

The `<=>` operator auto-generates `<`, `<=`, `>`, `>=`, and `==` auto-generates `!=`.

---

## Intentionally NOT Overloaded

| Operator | Reason |
|----------|--------|
| `+`, `-`, `*`, `/`, `%` | Use `checked_*`, `saturating_*`, or `wrapping_*` methods — forces explicit overflow handling |
| `++`, `--` | Same reason — forces explicit choice about overflow behavior |
| Implicit conversions | Deleted — prevents silent narrowing/widening bugs |

**Design philosophy**: Arithmetic must be explicit about overflow behavior, following Rust's approach to integer safety.

---

## Optional<T> Integration

All checked operations return `Optional<i16>`. Key methods:

| Method | Description |
|--------|-------------|
| `.expect("msg")` | Returns value or panics with message |
| `.unwrap()` | Returns value or panics with generic message |
| `.unwrap_or(default)` | Returns value or the provided default |
| `.unwrap_or_else(fn)` | Returns value or computes from function |
| `.map(fn)` | Transforms the contained value |
| `.is_some()` | Returns `true` if contains a value |
| `.is_none()` | Returns `true` if empty |

### Example

```cpp
auto result = 10000_i16.checked_add(5000_i16)
    .map([](i16 v) { return v.get() * 2; })
    .unwrap_or(0);  // result = 30000
```

---

## STL Container Compatibility

`i16` works seamlessly with all STL containers:

```cpp
#include <vector>
#include <set>
#include <unordered_set>

std::vector<i16> vec = {1000_i16, 2000_i16, 3000_i16};
std::set<i16> s = {5000_i16, 3000_i16, 7000_i16};           // Uses operator<=>
std::unordered_set<i16> us = {1000_i16, 2000_i16};          // Uses std::hash<i16>

std::sort(vec.begin(), vec.end());                          // Works
auto it = std::find(vec.begin(), vec.end(), 2000_i16);      // Works
```
