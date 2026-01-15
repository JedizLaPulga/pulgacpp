// Quick test to verify all types compile and work
// Compile: cl /std:c++latest /EHsc /W4 /I.. test_all.cpp

#include "../pulgacpp.hpp"
#include <iostream>
#include <string>

using namespace pulgacpp;
using namespace pulgacpp::literals;

int main() {
    std::cout << "=== pulgacpp All Types Test ===\n\n";

    // Test all signed types
    auto a8  = 100_i8;
    auto a16 = 10000_i16;
    auto a32 = 1000000_i32;
    auto a64 = 1000000000_i64;

    std::cout << "i8:  " << a8 << " (MIN=" << i8::MIN << ", MAX=" << i8::MAX << ")\n";
    std::cout << "i16: " << a16 << " (MIN=" << i16::MIN << ", MAX=" << i16::MAX << ")\n";
    std::cout << "i32: " << a32 << " (MIN=" << i32::MIN << ", MAX=" << i32::MAX << ")\n";
    std::cout << "i64: " << a64 << " (MIN=" << i64::MIN << ", MAX=" << i64::MAX << ")\n";

    // Test all unsigned types
    auto b8  = 200_u8;
    auto b16 = 50000_u16;
    auto b32 = 3000000000_u32;
    auto b64 = 10000000000_u64;

    std::cout << "\nu8:  " << b8 << " (MIN=" << u8::MIN << ", MAX=" << u8::MAX << ")\n";
    std::cout << "u16: " << b16 << " (MIN=" << u16::MIN << ", MAX=" << u16::MAX << ")\n";
    std::cout << "u32: " << b32 << " (MIN=" << u32::MIN << ", MAX=" << u32::MAX << ")\n";
    std::cout << "u64: " << b64 << " (MIN=" << u64::MIN << ", MAX=" << u64::MAX << ")\n";

    // Test checked arithmetic on each
    std::cout << "\n--- Checked Arithmetic ---\n";
    
    auto r8 = a8.checked_add(10_i8);
    std::cout << "i8: 100 + 10 = " << (r8.is_some() ? std::to_string(r8.unwrap().get()) : "None") << "\n";
    
    auto r16 = a16.checked_add(5000_i16);
    std::cout << "i16: 10000 + 5000 = " << (r16.is_some() ? std::to_string(r16.unwrap().get()) : "None") << "\n";
    
    auto r32 = a32.checked_mul(1000_i32);
    std::cout << "i32: 1000000 * 1000 = " << (r32.is_some() ? std::to_string(r32.unwrap().get()) : "None") << "\n";
    
    auto r64 = a64.checked_add(1000000000_i64);
    std::cout << "i64: 1000000000 + 1000000000 = " << (r64.is_some() ? std::to_string(r64.unwrap().get()) : "None") << "\n";

    // Test overflow detection
    std::cout << "\n--- Overflow Detection ---\n";
    
    auto overflow8 = 127_i8.checked_add(1_i8);
    std::cout << "i8: 127 + 1 = " << (overflow8.is_some() ? "Some" : "None (overflow!)") << "\n";
    
    auto overflow_u8 = 255_u8.checked_add(1_u8);
    std::cout << "u8: 255 + 1 = " << (overflow_u8.is_some() ? "Some" : "None (overflow!)") << "\n";

    // Test wrapping
    std::cout << "\n--- Wrapping Arithmetic ---\n";
    auto wrapped = 127_i8.wrapping_add(1_i8);
    std::cout << "i8: 127 wrapping_add 1 = " << wrapped << " (wrapped to MIN)\n";

    auto wrapped_u = 255_u8.wrapping_add(1_u8);
    std::cout << "u8: 255 wrapping_add 1 = " << wrapped_u << " (wrapped to 0)\n";

    std::cout << "\n=== All Types Working! ===\n";
    return 0;
}
