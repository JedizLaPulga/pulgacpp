// Test program for pulgacpp::i8
// Compile with C++23 modules support:
//   MSVC:  cl /std:c++latest /EHsc /experimental:module main.cpp
//   Clang: clang++ -std=c++23 -fmodules main.cpp
//   GCC:   g++ -std=c++23 -fmodules-ts main.cpp

import pulgacpp.i8;

#include <iostream>
#include <string_view>

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
    std::cout << "=== pulgacpp::i8 Test Suite ===\n\n";

    // --- Construction Tests ---
    std::cout << "--- Construction ---\n";
    {
        i8 a;
        test(a.get() == 0, "Default construction is zero");

        i8 b(static_cast<std::int8_t>(42));
        test(b.get() == 42, "Explicit construction from int8_t");

        auto c = 100_i8;
        test(c.get() == 100, "Literal suffix _i8");
    }

    // --- from() Factory ---
    std::cout << "\n--- from() Factory ---\n";
    {
        auto valid = i8::from(50);
        test(valid.is_some(), "from(50) returns Some");
        test(valid.unwrap().get() == 50, "from(50) contains 50");

        auto too_big = i8::from(200);
        test(too_big.is_none(), "from(200) returns None (overflow)");

        auto too_small = i8::from(-200);
        test(too_small.is_none(), "from(-200) returns None (underflow)");

        auto negative = i8::from(-100);
        test(negative.is_some() && negative.unwrap().get() == -100, "from(-100) works");
    }

    // --- saturating_from() Factory ---
    std::cout << "\n--- saturating_from() Factory ---\n";
    {
        auto clamped_high = i8::saturating_from(500);
        test(clamped_high.get() == i8::MAX, "saturating_from(500) clamps to MAX (127)");

        auto clamped_low = i8::saturating_from(-500);
        test(clamped_low.get() == i8::MIN, "saturating_from(-500) clamps to MIN (-128)");

        auto in_range = i8::saturating_from(50);
        test(in_range.get() == 50, "saturating_from(50) returns 50");
    }

    // --- Checked Arithmetic ---
    std::cout << "\n--- Checked Arithmetic ---\n";
    {
        auto a = 100_i8;
        auto b = 50_i8;

        // checked_add
        auto add_ok = a.checked_add(20_i8);
        test(add_ok.is_some() && add_ok.unwrap().get() == 120, "100 + 20 = 120");

        auto add_overflow = a.checked_add(b);
        test(add_overflow.is_none(), "100 + 50 overflows (returns None)");

        // checked_sub
        auto sub_ok = a.checked_sub(50_i8);
        test(sub_ok.is_some() && sub_ok.unwrap().get() == 50, "100 - 50 = 50");

        auto neg = i8(static_cast<std::int8_t>(-100));
        auto sub_underflow = neg.checked_sub(50_i8);
        test(sub_underflow.is_none(), "-100 - 50 underflows (returns None)");

        // checked_mul
        auto mul_ok = 10_i8.checked_mul(10_i8);
        test(mul_ok.is_some() && mul_ok.unwrap().get() == 100, "10 * 10 = 100");

        auto mul_overflow = 50_i8.checked_mul(10_i8);
        test(mul_overflow.is_none(), "50 * 10 overflows (returns None)");

        // checked_div
        auto div_ok = 100_i8.checked_div(10_i8);
        test(div_ok.is_some() && div_ok.unwrap().get() == 10, "100 / 10 = 10");

        auto div_by_zero = 100_i8.checked_div(0_i8);
        test(div_by_zero.is_none(), "100 / 0 returns None");

        auto div_min = i8(i8::MIN).checked_div(i8(static_cast<std::int8_t>(-1)));
        test(div_min.is_none(), "MIN / -1 overflows (returns None)");

        // checked_rem
        auto rem_ok = 100_i8.checked_rem(30_i8);
        test(rem_ok.is_some() && rem_ok.unwrap().get() == 10, "100 % 30 = 10");

        auto rem_by_zero = 100_i8.checked_rem(0_i8);
        test(rem_by_zero.is_none(), "100 % 0 returns None");

        // checked_neg
        auto neg_ok = 50_i8.checked_neg();
        test(neg_ok.is_some() && neg_ok.unwrap().get() == -50, "-50 negation works");

        auto neg_min = i8(i8::MIN).checked_neg();
        test(neg_min.is_none(), "-MIN overflows (returns None)");

        // checked_abs
        auto abs_neg = i8(static_cast<std::int8_t>(-50)).checked_abs();
        test(abs_neg.is_some() && abs_neg.unwrap().get() == 50, "abs(-50) = 50");

        auto abs_min = i8(i8::MIN).checked_abs();
        test(abs_min.is_none(), "abs(MIN) overflows (returns None)");
    }

    // --- Saturating Arithmetic ---
    std::cout << "\n--- Saturating Arithmetic ---\n";
    {
        auto a = 100_i8;

        auto sat_add = a.saturating_add(100_i8);
        test(sat_add.get() == i8::MAX, "100 saturating_add 100 = MAX");

        auto neg = i8(static_cast<std::int8_t>(-100));
        auto sat_sub = neg.saturating_sub(100_i8);
        test(sat_sub.get() == i8::MIN, "-100 saturating_sub 100 = MIN");

        auto sat_mul = 50_i8.saturating_mul(10_i8);
        test(sat_mul.get() == i8::MAX, "50 saturating_mul 10 = MAX");
    }

    // --- Wrapping Arithmetic ---
    std::cout << "\n--- Wrapping Arithmetic ---\n";
    {
        auto max_val = i8(i8::MAX);
        auto wrap_add = max_val.wrapping_add(1_i8);
        test(wrap_add.get() == i8::MIN, "MAX wrapping_add 1 = MIN");

        auto min_val = i8(i8::MIN);
        auto wrap_sub = min_val.wrapping_sub(1_i8);
        test(wrap_sub.get() == i8::MAX, "MIN wrapping_sub 1 = MAX");
    }

    // --- Overflowing Arithmetic ---
    std::cout << "\n--- Overflowing Arithmetic ---\n";
    {
        auto [result, overflow] = i8(i8::MAX).overflowing_add(1_i8);
        test(overflow == true, "MAX + 1 sets overflow flag");
        test(result.get() == i8::MIN, "MAX + 1 wraps to MIN");

        auto [result2, no_overflow] = 50_i8.overflowing_add(10_i8);
        test(no_overflow == false, "50 + 10 does not overflow");
        test(result2.get() == 60, "50 + 10 = 60");
    }

    // --- Bitwise Operations ---
    std::cout << "\n--- Bitwise Operations ---\n";
    {
        auto a = i8(static_cast<std::int8_t>(0b00001111));
        auto b = i8(static_cast<std::int8_t>(0b11110000));

        test((a & b).get() == 0, "0x0F & 0xF0 = 0x00");
        test((a | b).get() == static_cast<std::int8_t>(0xFF), "0x0F | 0xF0 = 0xFF");
        test((a ^ b).get() == static_cast<std::int8_t>(0xFF), "0x0F ^ 0xF0 = 0xFF");
        test((a << 2).get() == 0b00111100, "0x0F << 2 = 0x3C");
        test((a >> 2).get() == 0b00000011, "0x0F >> 2 = 0x03");
    }

    // --- Comparison ---
    std::cout << "\n--- Comparison ---\n";
    {
        auto a = 50_i8;
        auto b = 100_i8;
        auto c = 50_i8;

        test(a < b, "50 < 100");
        test(b > a, "100 > 50");
        test(a == c, "50 == 50");
        test(a != b, "50 != 100");
        test(a <= c, "50 <= 50");
        test(b >= a, "100 >= 50");
    }

    // --- Utility Methods ---
    std::cout << "\n--- Utility Methods ---\n";
    {
        auto pos = 50_i8;
        auto neg = i8(static_cast<std::int8_t>(-50));
        auto zero = 0_i8;

        test(pos.is_positive(), "50 is positive");
        test(neg.is_negative(), "-50 is negative");
        test(zero.is_zero(), "0 is zero");
        test(pos.signum() == 1, "signum(50) = 1");
        test(neg.signum() == -1, "signum(-50) = -1");
        test(zero.signum() == 0, "signum(0) = 0");
    }

    // --- Bit Counting ---
    std::cout << "\n--- Bit Counting ---\n";
    {
        auto val = i8(static_cast<std::int8_t>(0b00001111));
        test(val.count_ones() == 4, "0x0F has 4 ones");
        test(val.count_zeros() == 4, "0x0F has 4 zeros");
        test(val.leading_zeros() == 4, "0x0F has 4 leading zeros");
        test(val.trailing_zeros() == 0, "0x0F has 0 trailing zeros");

        auto val2 = i8(static_cast<std::int8_t>(0b00010000));
        test(val2.trailing_zeros() == 4, "0x10 has 4 trailing zeros");
    }

    // --- Optional Methods ---
    std::cout << "\n--- Optional Methods ---\n";
    {
        auto some_val = 50_i8.checked_add(10_i8);
        test(some_val.expect("should work").get() == 60, "expect() returns value");

        auto default_val = i8::from(999).unwrap_or(0_i8);
        test(default_val.get() == 0, "unwrap_or returns default on None");

        auto mapped = 50_i8.checked_add(10_i8).map([](i8 v) { return v.get() * 2; });
        test(mapped.is_some() && mapped.unwrap() == 120, "map() transforms value");
    }

    // --- Summary ---
    std::cout << "\n=== Test Summary ===\n";
    std::cout << "Passed: " << g_tests_passed << '\n';
    std::cout << "Failed: " << g_tests_failed << '\n';

    return g_tests_failed > 0 ? 1 : 0;
}
