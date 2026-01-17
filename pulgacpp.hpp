// pulgacpp - Modern C++ library for safe, versatile types
// SPDX-License-Identifier: MIT
//
// Usage:
//   #include <pulgacpp/pulgacpp.hpp>  // Include everything
//   #include <pulgacpp/i8/i8.hpp>     // Include only i8
//   #include <pulgacpp/i16/i16.hpp>   // Include only i16
//   #include <pulgacpp/i32/i32.hpp>   // Include only i32
//   #include <pulgacpp/i64/i64.hpp>   // Include only i64
//   #include <pulgacpp/isize/isize.hpp>  // Include only isize
//   #include <pulgacpp/u8/u8.hpp>     // Include only u8
//   #include <pulgacpp/u16/u16.hpp>   // Include only u16
//   #include <pulgacpp/u32/u32.hpp>   // Include only u32
//   #include <pulgacpp/u64/u64.hpp>   // Include only u64
//   #include <pulgacpp/usize/usize.hpp>  // Include only usize
//   #include <pulgacpp/result/result.hpp>  // Include only Result<T,E>
//   #include <pulgacpp/geometry/geometry.hpp>  // Include geometry types

#ifndef PULGACPP_HPP
#define PULGACPP_HPP

// Core types
#include "pulgacpp/optional/optional.hpp"
#include "pulgacpp/result/result.hpp"

// Signed integers
#include "pulgacpp/i16/i16.hpp"
#include "pulgacpp/i32/i32.hpp"
#include "pulgacpp/i64/i64.hpp"
#include "pulgacpp/i8/i8.hpp"
#include "pulgacpp/isize/isize.hpp"


// Unsigned integers
#include "pulgacpp/u16/u16.hpp"
#include "pulgacpp/u32/u32.hpp"
#include "pulgacpp/u64/u64.hpp"
#include "pulgacpp/u8/u8.hpp"
#include "pulgacpp/usize/usize.hpp"


// Geometry (2D/3D shapes and angles)
#include "pulgacpp/geometry/geometry.hpp"

// Scientific constants
#include "pulgacpp/constants/constants.hpp"

#endif // PULGACPP_HPP
