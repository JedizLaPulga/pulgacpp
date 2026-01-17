// Test for 64-bit overflow detection fix
// Compile: cl /std:c++latest /EHsc /W4 /I. test_64bit_overflow.cpp

#include "pulgacpp.hpp"
#include <iostream>
#include <string>

using namespace pulgacpp;
using namespace pulgacpp::literals;

int passed = 0;
int failed = 0;

void test(bool condition, const char *name) {
  if (condition) {
    std::cout << "[PASS] " << name << "\n";
    ++passed;
  } else {
    std::cout << "[FAIL] " << name << "\n";
    ++failed;
  }
}

int main() {
  std::cout << "=== 64-bit Overflow Detection Test ===\n\n";

  // ===================== i64 Addition Overflow =====================
  std::cout << "--- i64 checked_add ---\n";

  // MAX + 1 should overflow
  auto i64_max = i64(i64::MAX);
  auto i64_one = 1_i64;
  auto add_overflow = i64_max.checked_add(i64_one);
  test(add_overflow.is_none(), "i64::MAX + 1 overflows (checked_add)");

  // MAX + 0 should work
  auto add_zero = i64_max.checked_add(0_i64);
  test(add_zero.is_some() && add_zero.unwrap().get() == i64::MAX,
       "i64::MAX + 0 works");

  // MIN + (-1) should overflow
  auto i64_min = i64(i64::MIN);
  auto neg_one = i64(static_cast<std::int64_t>(-1));
  auto add_underflow = i64_min.checked_add(neg_one);
  test(add_underflow.is_none(), "i64::MIN + (-1) underflows");

  // Large positive + large positive = overflow
  auto half_max = i64(i64::MAX / 2 + 1);
  auto large_add = half_max.checked_add(half_max);
  test(large_add.is_none(), "Half MAX + Half MAX overflows");

  // Normal addition
  auto normal_add = 1000000000_i64.checked_add(2000000000_i64);
  test(normal_add.is_some() && normal_add.unwrap().get() == 3000000000,
       "Normal i64 add works");

  // ===================== i64 Subtraction Overflow =====================
  std::cout << "\n--- i64 checked_sub ---\n";

  // MIN - 1 should overflow
  auto sub_underflow = i64_min.checked_sub(i64_one);
  test(sub_underflow.is_none(), "i64::MIN - 1 underflows");

  // MAX - (-1) should overflow (MAX + 1)
  auto sub_overflow = i64_max.checked_sub(neg_one);
  test(sub_overflow.is_none(), "i64::MAX - (-1) overflows");

  // Normal subtraction
  auto normal_sub = 5000000000_i64.checked_sub(2000000000_i64);
  test(normal_sub.is_some() && normal_sub.unwrap().get() == 3000000000,
       "Normal i64 sub works");

  // ===================== i64 Multiplication Overflow =====================
  std::cout << "\n--- i64 checked_mul ---\n";

  // MAX * 2 should overflow
  auto mul_overflow = i64_max.checked_mul(2_i64);
  test(mul_overflow.is_none(), "i64::MAX * 2 overflows");

  // Large * large = overflow
  auto large = i64(static_cast<std::int64_t>(3037000500LL)); // sqrt(MAX) approx
  auto large_mul = large.checked_mul(large);
  test(large_mul.is_none(), "~sqrt(MAX) * ~sqrt(MAX) overflows");

  // MIN * 2 should overflow
  auto min_mul = i64_min.checked_mul(2_i64);
  test(min_mul.is_none(), "i64::MIN * 2 overflows");

  // Normal multiplication
  auto normal_mul = 1000000_i64.checked_mul(1000000_i64);
  test(normal_mul.is_some() && normal_mul.unwrap().get() == 1000000000000LL,
       "1M * 1M = 1T works");

  // ===================== u64 Addition Overflow =====================
  std::cout << "\n--- u64 checked_add ---\n";

  auto u64_max = u64(u64::MAX);
  auto u64_one = 1_u64;

  // MAX + 1 should overflow
  auto u64_add_overflow = u64_max.checked_add(u64_one);
  test(u64_add_overflow.is_none(), "u64::MAX + 1 overflows");

  // MAX + 0 should work
  auto u64_add_zero = u64_max.checked_add(0_u64);
  test(u64_add_zero.is_some() && u64_add_zero.unwrap().get() == u64::MAX,
       "u64::MAX + 0 works");

  // Normal addition
  auto u64_normal = 10000000000_u64.checked_add(5000000000_u64);
  test(u64_normal.is_some() && u64_normal.unwrap().get() == 15000000000ULL,
       "Normal u64 add works");

  // ===================== u64 Subtraction Underflow =====================
  std::cout << "\n--- u64 checked_sub ---\n";

  auto u64_zero = 0_u64;

  // 0 - 1 should underflow
  auto u64_sub_underflow = u64_zero.checked_sub(u64_one);
  test(u64_sub_underflow.is_none(), "u64: 0 - 1 underflows");

  // Normal subtraction
  auto u64_sub_normal = 10000000000_u64.checked_sub(5000000000_u64);
  test(u64_sub_normal.is_some() &&
           u64_sub_normal.unwrap().get() == 5000000000ULL,
       "Normal u64 sub works");

  // ===================== u64 Multiplication Overflow =====================
  std::cout << "\n--- u64 checked_mul ---\n";

  // MAX * 2 should overflow
  auto u64_mul_overflow = u64_max.checked_mul(2_u64);
  test(u64_mul_overflow.is_none(), "u64::MAX * 2 overflows");

  // Large * large = overflow (sqrt of MAX is about 4.3 billion)
  auto u64_large = u64(static_cast<std::uint64_t>(4294967296ULL)); // 2^32
  auto u64_large_mul = u64_large.checked_mul(u64_large);
  test(u64_large_mul.is_none(), "2^32 * 2^32 overflows (would be 2^64)");

  // Normal multiplication
  auto u64_normal_mul = 1000000_u64.checked_mul(1000000_u64);
  test(u64_normal_mul.is_some() &&
           u64_normal_mul.unwrap().get() == 1000000000000ULL,
       "1M * 1M = 1T works");

  // ===================== Saturating Arithmetic =====================
  std::cout << "\n--- saturating_* for 64-bit ---\n";

  auto i64_sat_add = i64_max.saturating_add(100_i64);
  test(i64_sat_add.get() == i64::MAX, "i64 saturating_add clamps to MAX");

  auto i64_sat_sub = i64_min.saturating_sub(100_i64);
  test(i64_sat_sub.get() == i64::MIN, "i64 saturating_sub clamps to MIN");

  auto u64_sat_add = u64_max.saturating_add(100_u64);
  test(u64_sat_add.get() == u64::MAX, "u64 saturating_add clamps to MAX");

  auto u64_sat_sub = u64_zero.saturating_sub(100_u64);
  test(u64_sat_sub.get() == u64::MIN, "u64 saturating_sub clamps to MIN (0)");

  // ===================== Overflowing Arithmetic =====================
  std::cout << "\n--- overflowing_* for 64-bit ---\n";

  auto [i64_of_result, i64_of_overflow] = i64_max.overflowing_add(1_i64);
  test(i64_of_overflow, "i64 overflowing_add detects overflow");

  auto [u64_of_result, u64_of_overflow] = u64_max.overflowing_add(1_u64);
  test(u64_of_overflow, "u64 overflowing_add detects overflow");

  auto [i64_mul_of_result, i64_mul_of_overflow] =
      i64_max.overflowing_mul(2_i64);
  test(i64_mul_of_overflow, "i64 overflowing_mul detects overflow");

  auto [u64_mul_of_result, u64_mul_of_overflow] =
      u64_max.overflowing_mul(2_u64);
  test(u64_mul_of_overflow, "u64 overflowing_mul detects overflow");

  // ===================== Summary =====================
  std::cout << "\n=== Test Summary ===\n";
  std::cout << "Passed: " << passed << "\n";
  std::cout << "Failed: " << failed << "\n";

  if (failed == 0) {
    std::cout << "\n*** 64-BIT OVERFLOW DETECTION WORKING! ***\n";
  }

  return failed > 0 ? 1 : 0;
}
