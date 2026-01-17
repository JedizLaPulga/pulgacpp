// Test suite for pulgacpp Angle<T>
// Compile: cl /std:c++latest /EHsc /W4 /I. test_angle.cpp

#include "pulgacpp.hpp"
#include <cmath>
#include <iostream>


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

bool approx_eq(double a, double b, double tol = 1e-9) {
  return std::abs(a - b) < tol;
}

int main() {
  std::cout << "=== Angle<T> Test Suite ===\n\n";

  // ==================== Construction ====================
  std::cout << "--- Construction ---\n";

  auto zero = Angle<>::zero();
  test(zero.radians() == 0.0, "Angle::zero() = 0");

  auto from_deg = Angle<>::from_degrees(180.0);
  test(approx_eq(from_deg.radians(), Angle<>::PI), "from_degrees(180) = π");

  auto from_rad = Angle<>::from_radians(Angle<>::PI);
  test(approx_eq(from_rad.degrees(), 180.0), "from_radians(π) = 180°");

  auto from_turns = Angle<>::from_turns(0.5);
  test(approx_eq(from_turns.degrees(), 180.0), "from_turns(0.5) = 180°");

  auto full = Angle<>::full_turn();
  test(approx_eq(full.degrees(), 360.0), "full_turn() = 360°");

  auto half = Angle<>::half_turn();
  test(approx_eq(half.degrees(), 180.0), "half_turn() = 180°");

  auto quarter = Angle<>::quarter_turn();
  test(approx_eq(quarter.degrees(), 90.0), "quarter_turn() = 90°");

  // ==================== Accessors ====================
  std::cout << "\n--- Accessors ---\n";

  auto angle_90 = Angle<>::from_degrees(90.0);
  test(approx_eq(angle_90.degrees(), 90.0), "degrees() accessor");
  test(approx_eq(angle_90.radians(), Angle<>::PI / 2.0), "radians() accessor");
  test(approx_eq(angle_90.turns(), 0.25), "turns() accessor");

  // ==================== Trigonometry ====================
  std::cout << "\n--- Trigonometry ---\n";

  auto zero_angle = Angle<>::from_degrees(0.0);
  test(approx_eq(zero_angle.sin(), 0.0), "sin(0°) = 0");
  test(approx_eq(zero_angle.cos(), 1.0), "cos(0°) = 1");
  test(approx_eq(zero_angle.tan(), 0.0), "tan(0°) = 0");

  test(approx_eq(angle_90.sin(), 1.0), "sin(90°) = 1");
  test(approx_eq(angle_90.cos(), 0.0, 1e-9), "cos(90°) ≈ 0");

  auto angle_45 = Angle<>::from_degrees(45.0);
  test(approx_eq(angle_45.sin(), std::sqrt(2.0) / 2.0), "sin(45°) = √2/2");
  test(approx_eq(angle_45.cos(), std::sqrt(2.0) / 2.0), "cos(45°) = √2/2");
  test(approx_eq(angle_45.tan(), 1.0), "tan(45°) = 1");

  // Inverse trig
  auto asin_result = Angle<>::asin(1.0);
  test(approx_eq(asin_result.degrees(), 90.0), "asin(1) = 90°");

  auto acos_result = Angle<>::acos(0.0);
  test(approx_eq(acos_result.degrees(), 90.0), "acos(0) = 90°");

  auto atan_result = Angle<>::atan(1.0);
  test(approx_eq(atan_result.degrees(), 45.0), "atan(1) = 45°");

  auto atan2_result = Angle<>::atan2(1.0, 1.0);
  test(approx_eq(atan2_result.degrees(), 45.0), "atan2(1,1) = 45°");

  // ==================== Normalization ====================
  std::cout << "\n--- Normalization ---\n";

  auto over_360 = Angle<>::from_degrees(450.0);
  auto normalized = over_360.normalized();
  test(approx_eq(normalized.degrees(), 90.0), "normalize 450° = 90°");

  auto negative = Angle<>::from_degrees(-90.0);
  auto neg_normalized = negative.normalized();
  test(approx_eq(neg_normalized.degrees(), 270.0), "normalize -90° = 270°");

  auto neg_signed = negative.normalized_signed();
  test(approx_eq(neg_signed.degrees(), -90.0), "normalize_signed -90° = -90°");

  auto big_negative = Angle<>::from_degrees(-450.0);
  auto big_neg_norm = big_negative.normalized();
  test(approx_eq(big_neg_norm.degrees(), 270.0), "normalize -450° = 270°");

  test(angle_90.is_normalized(), "90° is normalized");
  test(!over_360.is_normalized(), "450° is not normalized");

  // ==================== Arithmetic ====================
  std::cout << "\n--- Arithmetic ---\n";

  auto a1 = Angle<>::from_degrees(45.0);
  auto a2 = Angle<>::from_degrees(45.0);

  auto sum = a1.checked_add(a2);
  test(sum.is_some(), "checked_add returns Some");
  test(approx_eq(sum.unwrap().degrees(), 90.0), "45° + 45° = 90°");

  auto diff = angle_90.checked_sub(a1);
  test(diff.is_some() && approx_eq(diff.unwrap().degrees(), 45.0),
       "90° - 45° = 45°");

  auto scaled = a1.checked_mul(2.0);
  test(scaled.is_some() && approx_eq(scaled.unwrap().degrees(), 90.0),
       "45° * 2 = 90°");

  auto divided = angle_90.checked_div(2.0);
  test(divided.is_some() && approx_eq(divided.unwrap().degrees(), 45.0),
       "90° / 2 = 45°");

  auto div_zero = angle_90.checked_div(0.0);
  test(div_zero.is_none(), "divide by zero returns None");

  auto negated = -angle_90;
  test(approx_eq(negated.degrees(), -90.0), "-90° works");

  // ==================== Comparison ====================
  std::cout << "\n--- Comparison ---\n";

  test(a1 == a2, "45° == 45°");
  test(a1 != angle_90, "45° != 90°");
  test(a1 < angle_90, "45° < 90°");
  test(angle_90 > a1, "90° > 45°");
  test(a1 <= a2, "45° <= 45°");
  test(a1 >= a2, "45° >= 45°");

  auto slightly_off = Angle<>::from_degrees(45.00000001);
  test(a1.approx_eq(slightly_off, 1e-6), "approximate equality");

  // ==================== Utilities ====================
  std::cout << "\n--- Utilities ---\n";

  test(zero.is_zero(), "0° is_zero");
  test(angle_45.is_acute(), "45° is acute");
  test(!angle_90.is_acute(), "90° is not acute");
  test(angle_90.is_right(), "90° is right");

  auto obtuse = Angle<>::from_degrees(120.0);
  test(obtuse.is_obtuse(), "120° is obtuse");

  auto straight = Angle<>::from_degrees(180.0);
  test(straight.is_straight(), "180° is straight");

  auto supp = angle_90.supplementary();
  test(approx_eq(supp.degrees(), 90.0), "supplementary of 90° = 90°");

  auto comp = angle_45.complementary();
  test(approx_eq(comp.degrees(), 45.0), "complementary of 45° = 45°");

  // Shortest distance
  auto a_10 = Angle<>::from_degrees(10.0);
  auto a_350 = Angle<>::from_degrees(350.0);
  auto shortest = a_10.shortest_distance_to(a_350);
  test(approx_eq(std::abs(shortest.degrees()), 20.0),
       "shortest 10° to 350° = 20°");

  // ==================== Literals ====================
  std::cout << "\n--- Literals ---\n";

  auto deg_literal = 90_deg;
  test(approx_eq(deg_literal.degrees(), 90.0), "90_deg literal");

  auto rad_literal = 3.14159265358979_rad;
  test(approx_eq(rad_literal.degrees(), 180.0, 1e-5), "π_rad literal ≈ 180°");

  // ==================== Type Aliases ====================
  std::cout << "\n--- Type Aliases ---\n";

  Angled ad = Angled::from_degrees(45.0);
  test(approx_eq(ad.degrees(), 45.0), "Angled alias works");

  Anglef af = Anglef::from_degrees(45.0f);
  test(af.degrees() == 45.0f, "Anglef alias works");

  // ==================== Summary ====================
  std::cout << "\n=== Test Summary ===\n";
  std::cout << "Passed: " << passed << "\n";
  std::cout << "Failed: " << failed << "\n";

  return failed > 0 ? 1 : 0;
}
