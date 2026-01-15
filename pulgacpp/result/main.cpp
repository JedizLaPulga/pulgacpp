// Test suite for pulgacpp::Result<T, E>
// Compile: cl /std:c++latest /EHsc /W4 main.cpp

#include "../optional/optional.hpp"
#include "result.hpp"
#include <iostream>
#include <string>

using namespace pulgacpp;

// Test utilities
int passed = 0;
int failed = 0;

void test(bool condition, const char* name) {
    if (condition) {
        std::cout << "[PASS] " << name << "\n";
        ++passed;
    } else {
        std::cout << "[FAIL] " << name << "\n";
        ++failed;
    }
}

// Sample error type
enum class MathError { DivisionByZero, Overflow, Underflow };

Result<int, MathError> divide(int a, int b) {
    if (b == 0) return Err(MathError::DivisionByZero);
    return Ok(a / b);
}

Result<int, std::string> parse_int(const std::string& s) {
    if (s.empty()) return Err(std::string("empty string"));
    try {
        return Ok(std::stoi(s));
    } catch (...) {
        return Err(std::string("invalid: " + s));
    }
}

int main() {
    std::cout << "=== pulgacpp::Result<T, E> Test Suite ===\n\n";

    // --- Creation ---
    std::cout << "--- Creation ---\n";
    
    auto ok_result = Ok(42);
    Result<int, std::string> r1 = ok_result;
    test(r1.is_ok(), "Ok() creates Ok result");
    test(!r1.is_err(), "Ok result is not Err");
    
    auto err_result = Err(std::string("error"));
    Result<int, std::string> r2 = err_result;
    test(r2.is_err(), "Err() creates Err result");
    test(!r2.is_ok(), "Err result is not Ok");

    // --- Boolean conversion ---
    std::cout << "\n--- Boolean Conversion ---\n";
    
    Result<int, std::string> ok_val = Ok(1);
    Result<int, std::string> err_val = Err(std::string("e"));
    test(static_cast<bool>(ok_val) == true, "Ok converts to true");
    test(static_cast<bool>(err_val) == false, "Err converts to false");

    // --- unwrap() ---
    std::cout << "\n--- unwrap() ---\n";
    
    Result<int, std::string> r3 = Ok(100);
    test(r3.unwrap() == 100, "unwrap() returns Ok value");

    // --- unwrap_err() ---
    std::cout << "\n--- unwrap_err() ---\n";
    
    Result<int, std::string> r4 = Err(std::string("my error"));
    test(r4.unwrap_err() == "my error", "unwrap_err() returns Err value");

    // --- unwrap_or() ---
    std::cout << "\n--- unwrap_or() ---\n";
    
    Result<int, std::string> r5 = Ok(42);
    Result<int, std::string> r6 = Err(std::string("err"));
    test(r5.unwrap_or(0) == 42, "unwrap_or() returns Ok value if Ok");
    test(r6.unwrap_or(0) == 0, "unwrap_or() returns default if Err");

    // --- unwrap_or_else() ---
    std::cout << "\n--- unwrap_or_else() ---\n";
    
    Result<int, std::string> r7 = Err(std::string("error message"));
    int computed = r7.unwrap_or_else([](const std::string& e) { 
        return static_cast<int>(e.length()); 
    });
    test(computed == 13, "unwrap_or_else() computes default from error");

    // --- map() ---
    std::cout << "\n--- map() ---\n";
    
    Result<int, std::string> r8 = Ok(21);
    auto r9 = r8.map([](int x) { return x * 2; });
    test(r9.unwrap() == 42, "map() transforms Ok value");
    
    Result<int, std::string> r10 = Err(std::string("e"));
    auto r11 = r10.map([](int x) { return x * 2; });
    test(r11.is_err(), "map() leaves Err unchanged");
    test(r11.unwrap_err() == "e", "map() preserves error");

    // --- map_err() ---
    std::cout << "\n--- map_err() ---\n";
    
    Result<int, int> r12 = Err(404);
    auto r13 = r12.map_err([](int code) { return std::string("code: ") + std::to_string(code); });
    test(r13.unwrap_err() == "code: 404", "map_err() transforms error");
    
    Result<int, int> r14 = Ok(1);
    auto r15 = r14.map_err([](int code) { return std::string("code: ") + std::to_string(code); });
    test(r15.is_ok(), "map_err() leaves Ok unchanged");

    // --- and_then() ---
    std::cout << "\n--- and_then() ---\n";
    
    auto chain_result = divide(100, 5)
        .and_then([](int v) { return divide(v, 2); });
    test(chain_result.is_ok(), "and_then() chains Ok results");
    test(chain_result.unwrap() == 10, "and_then() value is correct");
    
    auto chain_fail = divide(100, 0)
        .and_then([](int v) { return divide(v, 2); });
    test(chain_fail.is_err(), "and_then() short-circuits on Err");

    // --- or_else() ---
    std::cout << "\n--- or_else() ---\n";
    
    auto fallback = divide(10, 0).or_else([](MathError) { 
        return divide(10, 2); 
    });
    test(fallback.is_ok(), "or_else() recovers from Err");
    test(fallback.unwrap() == 5, "or_else() recovery value is correct");
    
    auto no_fallback = divide(10, 2).or_else([](MathError) { 
        return divide(0, 1); 
    });
    test(no_fallback.unwrap() == 5, "or_else() ignores fallback on Ok");

    // --- ok() conversion ---
    std::cout << "\n--- ok() Conversion ---\n";
    
    Result<int, std::string> r16 = Ok(42);
    Result<int, std::string> r17 = Err(std::string("e"));
    test(r16.ok().is_some(), "ok() returns Some for Ok result");
    test(r16.ok().unwrap() == 42, "ok() value is correct");
    test(r17.ok().is_none(), "ok() returns None for Err result");

    // --- err() conversion ---
    std::cout << "\n--- err() Conversion ---\n";
    
    test(r17.err().is_some(), "err() returns Some for Err result");
    test(r17.err().unwrap() == "e", "err() value is correct");
    test(r16.err().is_none(), "err() returns None for Ok result");

    // --- Real-world example ---
    std::cout << "\n--- Real-World Example ---\n";
    
    auto parsed = parse_int("123");
    test(parsed.is_ok(), "parse_int(\"123\") succeeds");
    test(parsed.unwrap() == 123, "parse_int(\"123\") = 123");
    
    auto invalid = parse_int("abc");
    test(invalid.is_err(), "parse_int(\"abc\") fails");
    test(invalid.unwrap_err().find("invalid") != std::string::npos, "error message contains 'invalid'");

    // --- Comparison ---
    std::cout << "\n--- Comparison ---\n";
    
    Result<int, std::string> a1 = Ok(42);
    Result<int, std::string> a2 = Ok(42);
    Result<int, std::string> a3 = Ok(100);
    Result<int, std::string> a4 = Err(std::string("e"));
    
    test(a1 == a2, "Ok(42) == Ok(42)");
    test(a1 != a3, "Ok(42) != Ok(100)");
    test(a1 != a4, "Ok(42) != Err(\"e\")");

    // --- Summary ---
    std::cout << "\n=== Test Summary ===\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";

    return failed > 0 ? 1 : 0;
}
