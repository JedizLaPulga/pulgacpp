<div align="center">

<img src="img/pulgacpp.png" alt="pulgacpp logo" width="400"/>

# 🦗 pulgacpp

### *Type Safety, Reimagined for C++*

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg?style=for-the-badge&logo=cplusplus)](https://en.cppreference.com/w/cpp/23)
[![Header Only](https://img.shields.io/badge/Header-Only-green.svg?style=for-the-badge)](https://github.com/JedizLaPulga/pulgacpp)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/MIT)

**Modern • Safe • Explicit • Zero Overhead**

[Getting Started](#-getting-started) •
[Features](#-features) •
[Types](#-available-types) •
[Examples](#-examples) •
[Philosophy](#-philosophy)

</div>

---

## 🎯 What is pulgacpp?

**pulgacpp** is a modern C++23 library that brings **Rust-style type safety** to C++. No more silent integer overflows. No more implicit conversions gone wrong. No more undefined behavior hiding in your arithmetic.

> 💡 *"pulga"* means "flea" in Spanish — small but mighty! 🦗

```cpp
#include <pulgacpp/pulgacpp.hpp>

using namespace pulgacpp;
using namespace pulgacpp::literals;

auto a = 100_i8;
auto b = 50_i8;

// ❌ This won't compile — we don't hide overflow!
// auto c = a + b;

// ✅ You must choose your overflow behavior explicitly
auto c = a.checked_add(b);      // Returns Optional<i8> — None on overflow
auto d = a.saturating_add(b);   // Clamps to MAX (127)
auto e = a.wrapping_add(b);     // Wraps around like C (but defined!)
```

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| 🚫 **No Implicit Conversions** | Types are explicit. `i8 x = 42;` won't compile. |
| 🔒 **Checked Arithmetic** | `checked_add()` returns `Optional<T>` — overflow is visible |
| 📊 **Saturating Arithmetic** | `saturating_add()` clamps to MIN/MAX |
| 🔄 **Wrapping Arithmetic** | `wrapping_add()` wraps like C, but defined |
| ⚡ **Overflowing Arithmetic** | `overflowing_add()` returns `(result, did_overflow)` |
| 🎭 **Optional<T>** | Rust-inspired with `.unwrap()`, `.expect()`, `.map()` |
| 🔗 **STL Compatible** | Works with `std::vector`, `std::set`, `std::unordered_map` |
| 0️⃣ **Zero Dependencies** | Header-only, standard library only |

---

## 📦 Available Types

### Signed Integers
| Type | Bits | Range | Literal |
|------|------|-------|---------|
| `i8` | 8 | -128 to 127 | `42_i8` |
| `i16` | 16 | -32,768 to 32,767 | `1000_i16` |
| `i32` | 32 | -2.1B to 2.1B | `1000000_i32` |
| `i64` | 64 | -9.2Q to 9.2Q | `1000000000_i64` |

### Unsigned Integers
| Type | Bits | Range | Literal |
|------|------|-------|---------|
| `u8` | 8 | 0 to 255 | `200_u8` |
| `u16` | 16 | 0 to 65,535 | `50000_u16` |
| `u32` | 32 | 0 to 4.2B | `3000000000_u32` |
| `u64` | 64 | 0 to 18.4Q | `10000000000_u64` |

### Core Types
| Type | Description |
|------|-------------|
| `Optional<T>` | Rust-style optional with rich API |

---

## 🚀 Getting Started

### Installation

**pulgacpp** is header-only. Just copy the `pulgacpp` folder to your project:

```
your_project/
├── pulgacpp/          ← Copy this folder
│   ├── pulgacpp.hpp
│   └── pulgacpp/
│       ├── core/
│       ├── i8/
│       ├── i16/
│       └── ...
└── main.cpp
```

### Usage

```cpp
// Include everything
#include <pulgacpp/pulgacpp.hpp>

// Or include only what you need
#include <pulgacpp/i8/i8.hpp>
#include <pulgacpp/i32/i32.hpp>
```

### Compile

```bash
# MSVC
cl /std:c++latest /EHsc main.cpp

# GCC
g++ -std=c++23 main.cpp -o main

# Clang
clang++ -std=c++23 main.cpp -o main
```

---

## 📖 Examples

### Checked Arithmetic — Catch Overflow!

```cpp
auto a = 100_i8;
auto result = a.checked_add(50_i8);  // 150 > 127, overflow!

if (result.is_none()) {
    std::cout << "Overflow detected!\n";
} else {
    std::cout << "Result: " << result.unwrap() << "\n";
}

// Or use expect() for quick prototyping
auto value = 50_i8.checked_add(10_i8).expect("math failed");
```

### Saturating Arithmetic — Clamp to Bounds

```cpp
auto a = 100_i8;
auto b = a.saturating_add(100_i8);  // Would be 200, but clamps to 127
std::cout << b;  // Output: 127
```

### Type Conversions — Safe and Explicit

```cpp
auto small = 50_i8;

// Widen: always safe
auto bigger = small.widen<i32>();  // i32(50)

// Narrow: checked, returns Optional
auto back = bigger.narrow<i8>();   // Some(i8(50))
auto fail = 1000_i32.narrow<i8>(); // None (overflow!)

// Cast: unchecked, like Rust's `as`
auto wrapped = i8(-1).cast<u8>();  // u8(255)
```

### STL Integration

```cpp
#include <vector>
#include <algorithm>

std::vector<i32> numbers = {50_i32, 30_i32, 70_i32, 10_i32};
std::sort(numbers.begin(), numbers.end());

// Use with algorithms
auto sum = std::accumulate(numbers.begin(), numbers.end(), 0_i32,
    [](i32 a, i32 b) { return a.saturating_add(b); });
```

### Optional<T> — Rust-Inspired API

```cpp
auto result = 100_i8.checked_mul(2_i8)
    .map([](i8 v) { return v.widen<i32>(); })
    .unwrap_or(0_i32);
```

---

## 🧠 Philosophy

### Why pulgacpp?

C++ gives you power. Sometimes too much power. Integer overflow? Undefined behavior. Implicit narrowing? Silent bugs. pulgacpp takes inspiration from Rust to make these errors **visible** and **explicit**.

| C++ (dangerous) | pulgacpp (safe) |
|-----------------|-----------------|
| `int8_t x = 200;` ⚠️ | `i8::from(200)` → `None` ✅ |
| `a + b` (might overflow) | `a.checked_add(b)` → `Optional` ✅ |
| `uint8_t x = -1;` → 255 😱 | `i8(-1).narrow<u8>()` → `None` ✅ |

### Design Principles

1. **Explicit over Implicit** — No silent conversions or hidden overflow
2. **Errors are Values** — Use `Optional<T>` instead of exceptions
3. **Zero Cost When You Choose** — `wrapping_*` and `cast<T>()` are as fast as raw types
4. **STL Compatible** — Works with containers, algorithms, and idioms you know

---

## 📁 Project Structure

```
pulgacpp/
├── pulgacpp.hpp                 # Include everything
└── pulgacpp/
    ├── core/
    │   └── safe_int.hpp         # Template base (the magic ✨)
    ├── optional/
    │   └── optional.hpp         # Rust-style Optional<T>
    ├── i8/   ├── i16/   ├── i32/   ├── i64/
    │   └── i8.hpp, i8doc.md, main.cpp (tests)
    └── u8/   ├── u16/   ├── u32/   └── u64/
```

---

## 🛣️ Roadmap

- [x] Signed integers: `i8`, `i16`, `i32`, `i64`
- [x] Unsigned integers: `u8`, `u16`, `u32`, `u64`
- [x] `Optional<T>` with Rust-style API
- [x] Inter-type conversions: `widen`, `narrow`, `cast`
- [x] STL container compatibility
- [ ] `isize` / `usize` (pointer-sized integers)
- [ ] `Result<T, E>` for richer error handling
- [ ] Fixed-point and decimal types
- [ ] SIMD-friendly type wrappers
- [ ] `String` and `Slice<T>` types

---

## 🤝 Contributing

Contributions welcome! Whether it's new types, documentation improvements, or bug fixes — open an issue or PR.

---

## 📄 License

MIT License — use it freely, just keep the attribution.

---

<div align="center">

**Made with 🦗 and C++23**

*Small but mighty type safety*

</div>
