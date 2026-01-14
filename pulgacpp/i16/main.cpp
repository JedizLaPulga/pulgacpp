// Test program for pulgacpp::i16
// Compile: cl /std:c++latest /EHsc /W4 main.cpp
//      or: g++ -std=c++23 -Wall main.cpp -o test_i16
//      or: clang++ -std=c++23 -Wall main.cpp -o test_i16

#include "i16.hpp"
#include <iostream>
#include <string_view>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <numeric>

using namespace pulgacpp;
using namespace pulgacpp::literals;

// Simple test helper
int g_tests_passed = 0;
int g_tests_failed = 0;

void test(bool condition, std::string_view name) {
    if (condition) {
        std::cout << "[PASS] " << name << '\n';
        ++g_tests_passed;
    } else {
        std::cout << "[FAIL] " << name << '\n';
        ++g_tests_failed;
    }
}

int main() {
    std::cout << "=== pulgacpp::i16 Test Suite ===\n\n";

    // --- Constants ---
    std::cout << "--- Constants ---\n";
    {
        test(i16::MIN == -32768, "MIN == -32768");
        test(i16::MAX == 32767, "MAX == 32767");
        test(sizeof(i16) == 2, "sizeof(i16) == 2");
    }

    // --- Construction Tests ---
    std::cout << "\n--- Construction ---\n";
    {
        i16 a;
        test(a.get() == 0, "Default construction is zero");

        i16 b(static_cast<std::int16_t>(1000));
        test(b.get() == 1000, "Explicit construction from int16_t");

        auto c = 30000_i16;
        test(c.get() == 30000, "Literal suffix _i16");
    }

    // --- from() Factory ---
    std::cout << "\n--- from() Factory ---\n";
    {
        auto valid = i16::from(10000);
        test(valid.is_some(), "from(10000) returns Some");
        test(valid.unwrap().get() == 10000, "from(10000) contains 10000");

        auto too_big = i16::from(50000);
        test(too_big.is_none(), "from(50000) returns None (overflow)");

        auto too_small = i16::from(-50000);
        test(too_small.is_none(), "from(-50000) returns None (underflow)");

        auto negative = i16::from(-20000);
        test(negative.is_some() && negative.unwrap().get() == -20000, "from(-20000) works");

        // From i8 range - should always work
        auto from_small = i16::from(static_cast<std::int8_t>(100));
        test(from_small.is_some() && from_small.unwrap().get() == 100, "from(int8_t) works");
    }

    // --- saturating_from() Factory ---
    std::cout << "\n--- saturating_from() Factory ---\n";
    {
        auto clamped_high = i16::saturating_from(100000);
        test(clamped_high.get() == i16::MAX, "saturating_from(100000) clamps to MAX");

        auto clamped_low = i16::saturating_from(-100000);
        test(clamped_low.get() == i16::MIN, "saturating_from(-100000) clamps to MIN");

        auto in_range = i16::saturating_from(5000);
        test(in_range.get() == 5000, "saturating_from(5000) returns 5000");
    }

    // --- Checked Arithmetic ---
    std::cout << "\n--- Checked Arithmetic ---\n";
    {
        auto a = 30000_i16;
        auto b = 10000_i16;

        // checked_add
        auto add_ok = a.checked_add(2000_i16);
        test(add_ok.is_some() && add_ok.unwrap().get() == 32000, "30000 + 2000 = 32000");

        auto add_overflow = a.checked_add(b);
        test(add_overflow.is_none(), "30000 + 10000 overflows (returns None)");

        // checked_sub
        auto sub_ok = a.checked_sub(10000_i16);
        test(sub_ok.is_some() && sub_ok.unwrap().get() == 20000, "30000 - 10000 = 20000");

        auto neg = i16(static_cast<std::int16_t>(-30000));
        auto sub_underflow = neg.checked_sub(10000_i16);
        test(sub_underflow.is_none(), "-30000 - 10000 underflows (returns None)");

        // checked_mul
        auto mul_ok = 100_i16.checked_mul(100_i16);
        test(mul_ok.is_some() && mul_ok.unwrap().get() == 10000, "100 * 100 = 10000");

        auto mul_overflow = 1000_i16.checked_mul(100_i16);
        test(mul_overflow.is_none(), "1000 * 100 overflows (returns None)");

        // checked_div
        auto div_ok = 10000_i16.checked_div(100_i16);
        test(div_ok.is_some() && div_ok.unwrap().get() == 100, "10000 / 100 = 100");

        auto div_by_zero = 10000_i16.checked_div(0_i16);
        test(div_by_zero.is_none(), "10000 / 0 returns None");

        auto div_min = i16(i16::MIN).checked_div(i16(static_cast<std::int16_t>(-1)));
        test(div_min.is_none(), "MIN / -1 overflows (returns None)");

        // checked_neg
        auto neg_ok = 1000_i16.checked_neg();
        test(neg_ok.is_some() && neg_ok.unwrap().get() == -1000, "negation works");

        auto neg_min = i16(i16::MIN).checked_neg();
        test(neg_min.is_none(), "-MIN overflows (returns None)");

        // checked_abs
        auto abs_neg = i16(static_cast<std::int16_t>(-5000)).checked_abs();
        test(abs_neg.is_some() && abs_neg.unwrap().get() == 5000, "abs(-5000) = 5000");

        auto abs_min = i16(i16::MIN).checked_abs();
        test(abs_min.is_none(), "abs(MIN) overflows (returns None)");
    }

    // --- Saturating Arithmetic ---
    std::cout << "\n--- Saturating Arithmetic ---\n";
    {
        auto a = 30000_i16;

        auto sat_add = a.saturating_add(30000_i16);
        test(sat_add.get() == i16::MAX, "30000 saturating_add 30000 = MAX");

        auto neg = i16(static_cast<std::int16_t>(-30000));
        auto sat_sub = neg.saturating_sub(30000_i16);
        test(sat_sub.get() == i16::MIN, "-30000 saturating_sub 30000 = MIN");

        auto sat_mul = 1000_i16.saturating_mul(1000_i16);
        test(sat_mul.get() == i16::MAX, "1000 saturating_mul 1000 = MAX");
    }

    // --- Wrapping Arithmetic ---
    std::cout << "\n--- Wrapping Arithmetic ---\n";
    {
        auto max_val = i16(i16::MAX);
        auto wrap_add = max_val.wrapping_add(1_i16);
        test(wrap_add.get() == i16::MIN, "MAX wrapping_add 1 = MIN");

        auto min_val = i16(i16::MIN);
        auto wrap_sub = min_val.wrapping_sub(1_i16);
        test(wrap_sub.get() == i16::MAX, "MIN wrapping_sub 1 = MAX");
    }

    // --- Overflowing Arithmetic ---
    std::cout << "\n--- Overflowing Arithmetic ---\n";
    {
        auto [result, overflow] = i16(i16::MAX).overflowing_add(1_i16);
        test(overflow == true, "MAX + 1 sets overflow flag");
        test(result.get() == i16::MIN, "MAX + 1 wraps to MIN");

        auto [result2, no_overflow] = 1000_i16.overflowing_add(1000_i16);
        test(no_overflow == false, "1000 + 1000 does not overflow");
        test(result2.get() == 2000, "1000 + 1000 = 2000");
    }

    // --- Bitwise Operations ---
    std::cout << "\n--- Bitwise Operations ---\n";
    {
        auto a = i16(static_cast<std::int16_t>(0x00FF));
        auto b = i16(static_cast<std::int16_t>(0xFF00));

        test((a & b).get() == 0, "0x00FF & 0xFF00 = 0x0000");
        test((a | b).get() == static_cast<std::int16_t>(0xFFFF), "0x00FF | 0xFF00 = 0xFFFF");
        test((a ^ b).get() == static_cast<std::int16_t>(0xFFFF), "0x00FF ^ 0xFF00 = 0xFFFF");
        test((a << 4).get() == static_cast<std::int16_t>(0x0FF0), "0x00FF << 4 = 0x0FF0");
        test((a >> 4).get() == 0x000F, "0x00FF >> 4 = 0x000F");
    }

    // --- Comparison ---
    std::cout << "\n--- Comparison ---\n";
    {
        auto a = 5000_i16;
        auto b = 10000_i16;
        auto c = 5000_i16;

        test(a < b, "5000 < 10000");
        test(b > a, "10000 > 5000");
        test(a == c, "5000 == 5000");
        test(a != b, "5000 != 10000");
        test(a <= c, "5000 <= 5000");
        test(b >= a, "10000 >= 5000");
    }

    // --- Utility Methods ---
    std::cout << "\n--- Utility Methods ---\n";
    {
        auto pos = 5000_i16;
        auto neg = i16(static_cast<std::int16_t>(-5000));
        auto zero = 0_i16;

        test(pos.is_positive(), "5000 is positive");
        test(neg.is_negative(), "-5000 is negative");
        test(zero.is_zero(), "0 is zero");
        test(pos.signum() == 1, "signum(5000) = 1");
        test(neg.signum() == -1, "signum(-5000) = -1");
        test(zero.signum() == 0, "signum(0) = 0");
    }

    // --- Bit Counting ---
    std::cout << "\n--- Bit Counting ---\n";
    {
        auto val = i16(static_cast<std::int16_t>(0x00FF));  // 8 ones in low byte
        test(val.count_ones() == 8, "0x00FF has 8 ones");
        test(val.count_zeros() == 8, "0x00FF has 8 zeros");
        test(val.leading_zeros() == 8, "0x00FF has 8 leading zeros");
        test(val.trailing_zeros() == 0, "0x00FF has 0 trailing zeros");

        auto val2 = i16(static_cast<std::int16_t>(0x0100));  // bit 8 set
        test(val2.trailing_zeros() == 8, "0x0100 has 8 trailing zeros");
    }

    // --- Type Conversions ---
    std::cout << "\n--- Type Conversions ---\n";
    {
        auto a = 1000_i16;

        // to<T>() - safe conversion
        auto to_i32 = a.to<std::int32_t>();
        test(to_i32.is_some() && to_i32.unwrap() == 1000, "to<int32_t>() works");

        auto to_i8 = a.to<std::int8_t>();
        test(to_i8.is_none(), "to<int8_t>() returns None for 1000 (overflow)");

        auto small = 50_i16;
        auto small_to_i8 = small.to<std::int8_t>();
        test(small_to_i8.is_some() && small_to_i8.unwrap() == 50, "to<int8_t>() works for 50");

        // as<T>() - unchecked conversion
        test(a.as<int>() == 1000, "as<int>() works");
        test(a.as<std::int32_t>() == 1000, "as<int32_t>() works");

        auto neg = i16(static_cast<std::int16_t>(-1));
        test(neg.as<std::uint16_t>() == 65535, "as<uint16_t>() wraps -1 to 65535");
    }

    // --- STL Container Usage ---
    std::cout << "\n--- STL Container Usage ---\n";
    {
        std::vector<i16> vec = {1000_i16, 3000_i16, 2000_i16};
        std::sort(vec.begin(), vec.end());
        test(vec[0].get() == 1000 && vec[2].get() == 3000, "vector: sort works");

        std::set<i16> s = {5000_i16, 3000_i16, 7000_i16};
        test(s.contains(5000_i16), "set: contains() works");

        std::unordered_set<i16> us = {1000_i16, 2000_i16, 3000_i16};
        test(us.contains(2000_i16), "unordered_set: hash works");

        auto sum = std::accumulate(vec.begin(), vec.end(), 0_i16,
            [](i16 a, i16 b) { return a.saturating_add(b); });
        test(sum.get() == 6000, "accumulate: sum = 6000");
    }

    // --- Summary ---
    std::cout << "\n=== Test Summary ===\n";
    std::cout << "Passed: " << g_tests_passed << '\n';
    std::cout << "Failed: " << g_tests_failed << '\n';

    return g_tests_failed > 0 ? 1 : 0;
}
