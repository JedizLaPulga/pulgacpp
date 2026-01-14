# pulgacpp::i8 Documentation

A type-safe signed 8-bit integer with Rust-like semantics. No implicit conversions. Checked arithmetic returns `Optional<i8>`.

## Header

```cpp
#include <pulgacpp/i8/i8.hpp>

using namespace pulgacpp;
using namespace pulgacpp::literals;  // For _i8 suffix
```

---

## Constants

| Name | Value | Description |
|------|-------|-------------|
| `i8::MIN` | `-128` | Minimum representable value |
| `i8::MAX` | `127` | Maximum representable value |
| `i8::underlying_type` | `std::int8_t` | The underlying primitive type |

---

## Construction

| Method | Description |
|--------|-------------|
| `i8()` | Default: initializes to `0` |
| `i8(std::int8_t)` | **Explicit** construction — no implicit conversions allowed |
| `i8::from<T>(val)` | Returns `Optional<i8>` — `None` if out of range |
| `i8::saturating_from<T>(val)` | Clamps to `MIN`/`MAX` if out of range |
| `42_i8` | Literal suffix (requires `using namespace pulgacpp::literals;`) |

### Examples

```cpp
i8 a;                                    // a = 0
i8 b(static_cast<std::int8_t>(42));      // b = 42
auto c = 100_i8;                         // c = 100

auto d = i8::from(200);                  // d = None (overflow)
auto e = i8::from(50);                   // e = Some(50)
auto f = i8::saturating_from(500);       // f = 127 (clamped to MAX)
```

---

## Checked Arithmetic

All checked operations return `Optional<i8>`. Returns `None` on overflow, underflow, or division by zero.

| Method | Description |
|--------|-------------|
| `checked_add(i8)` | Returns `None` on overflow |
| `checked_sub(i8)` | Returns `None` on underflow |
| `checked_mul(i8)` | Returns `None` on overflow |
| `checked_div(i8)` | Returns `None` on division by zero or `MIN / -1` |
| `checked_rem(i8)` | Returns `None` on division by zero |
| `checked_neg()` | Returns `None` if negating `MIN` (would overflow) |
| `checked_abs()` | Returns `None` if `abs(MIN)` (would overflow) |

### Examples

```cpp
auto a = 100_i8;
auto result = a.checked_add(50_i8);  // None (overflow: 150 > 127)

auto b = 50_i8;
auto sum = b.checked_add(10_i8);     // Some(60)
auto value = sum.expect("overflow"); // 60

auto div = 100_i8.checked_div(0_i8); // None (division by zero)
```

---

## Saturating Arithmetic

Operations clamp the result to `MIN` or `MAX` instead of overflowing.

| Method | Description |
|--------|-------------|
| `saturating_add(i8)` | Clamps result to `MIN`/`MAX` |
| `saturating_sub(i8)` | Clamps result to `MIN`/`MAX` |
| `saturating_mul(i8)` | Clamps result to `MIN`/`MAX` |

### Examples

```cpp
auto a = 100_i8;
auto b = a.saturating_add(100_i8);  // b = 127 (MAX)

auto c = i8(i8::MIN);
auto d = c.saturating_sub(1_i8);    // d = -128 (MIN, unchanged)
```

---

## Wrapping Arithmetic

Operations wrap around on overflow (like C's undefined behavior, but defined).

| Method | Description |
|--------|-------------|
| `wrapping_add(i8)` | Wraps around on overflow |
| `wrapping_sub(i8)` | Wraps around on underflow |
| `wrapping_mul(i8)` | Wraps around on overflow |
| `wrapping_neg()` | Wraps around if negating `MIN` |

### Examples

```cpp
auto max = i8(i8::MAX);              // 127
auto wrapped = max.wrapping_add(1_i8); // -128 (wraps to MIN)

auto min = i8(i8::MIN);              // -128
auto wrapped2 = min.wrapping_sub(1_i8); // 127 (wraps to MAX)
```

---

## Overflowing Arithmetic

Returns a `std::pair<i8, bool>` containing the wrapped result and a flag indicating if overflow occurred.

| Method | Returns |
|--------|---------|
| `overflowing_add(i8)` | `(wrapped_result, did_overflow)` |
| `overflowing_sub(i8)` | `(wrapped_result, did_overflow)` |
| `overflowing_mul(i8)` | `(wrapped_result, did_overflow)` |

### Examples

```cpp
auto [result, overflow] = i8(i8::MAX).overflowing_add(1_i8);
// result = -128, overflow = true

auto [result2, overflow2] = 50_i8.overflowing_add(10_i8);
// result2 = 60, overflow2 = false
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
auto a = i8(static_cast<std::int8_t>(0b00001111));
auto b = i8(static_cast<std::int8_t>(0b11110000));

auto c = a & b;   // 0b00000000
auto d = a | b;   // 0b11111111
auto e = a ^ b;   // 0b11111111
auto f = a << 2;  // 0b00111100
auto g = a >> 2;  // 0b00000011
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
auto a = 50_i8;
auto b = 100_i8;

bool eq = (a == b);   // false
bool lt = (a < b);    // true
bool ge = (b >= a);   // true
```

---

## Utility Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `get()` | `std::int8_t` | Returns the underlying value |
| `is_positive()` | `bool` | `true` if value > 0 |
| `is_negative()` | `bool` | `true` if value < 0 |
| `is_zero()` | `bool` | `true` if value == 0 |
| `signum()` | `int` | Returns `-1`, `0`, or `1` |
| `count_ones()` | `unsigned` | Population count (number of 1 bits) |
| `count_zeros()` | `unsigned` | Number of 0 bits (8 − popcount) |
| `leading_zeros()` | `unsigned` | Number of leading zero bits |
| `trailing_zeros()` | `unsigned` | Number of trailing zero bits |

### Examples

```cpp
auto a = 50_i8;
auto b = i8(static_cast<std::int8_t>(-50));
auto c = 0_i8;

a.is_positive();  // true
b.is_negative();  // true
c.is_zero();      // true

a.signum();       // 1
b.signum();       // -1
c.signum();       // 0

auto val = i8(static_cast<std::int8_t>(0b00001111));
val.count_ones();      // 4
val.count_zeros();     // 4
val.leading_zeros();   // 4
val.trailing_zeros();  // 0
```

---

## Explicit Conversion

| Method | Description |
|--------|-------------|
| `static_cast<std::int8_t>(a)` | Explicit conversion to underlying type |
| `a.get()` | Same as above, but clearer |

---

## Stream Output

```cpp
std::cout << my_i8;  // Prints as integer (e.g., "42"), not as ASCII char
```

---

## Overloaded Operators Summary

### Explicitly Defined: 15 operators

| Category | Operators | Count |
|----------|-----------|-------|
| Bitwise | `~`, `&`, `\|`, `^`, `<<`, `>>` | 6 |
| Compound Bitwise | `&=`, `\|=`, `^=`, `<<=`, `>>=` | 5 |
| Comparison | `==`, `<=>` | 2 |
| Conversion | `explicit operator std::int8_t()` | 1 |
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

All checked operations return `Optional<i8>`. Key methods:

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
auto result = 50_i8.checked_add(10_i8)
    .map([](i8 v) { return v.get() * 2; })
    .unwrap_or(0);  // result = 120
```
