// Test suite for pulgacpp::constants
// Compile: cl /std:c++latest /EHsc /W4 /I. test_constants.cpp

#include "pulgacpp.hpp"
#include <cmath>
#include <iostream>


using namespace pulgacpp::constants;

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

bool approx_eq(double a, double b, double rel_tol = 1e-6) {
  return std::abs(a - b) <= rel_tol * std::max(std::abs(a), std::abs(b));
}

int main() {
  std::cout << "=== Constants Test Suite ===\n\n";

  // ==================== Math Constants ====================
  std::cout << "--- math ---\n";

  test(approx_eq(math::PI, 3.14159265358979), "PI ≈ 3.14159...");
  test(approx_eq(math::TAU, 2.0 * math::PI), "TAU = 2π");
  test(approx_eq(math::E, 2.71828182845904), "E ≈ 2.71828...");
  test(approx_eq(math::PHI, 1.61803398874989), "PHI ≈ 1.61803...");
  test(approx_eq(math::SQRT_2, std::sqrt(2.0)), "SQRT_2 = √2");
  test(approx_eq(math::LN_2, std::log(2.0)), "LN_2 = ln(2)");
  test(approx_eq(math::DEG_PER_RAD * math::RAD_PER_DEG, 1.0),
       "DEG_PER_RAD * RAD_PER_DEG = 1");

  // ==================== Physics Constants ====================
  std::cout << "\n--- physics ---\n";

  test(physics::SPEED_OF_LIGHT == 299792458.0,
       "SPEED_OF_LIGHT = 299792458 m/s");
  test(physics::C == physics::SPEED_OF_LIGHT, "C alias works");
  test(approx_eq(physics::PLANCK, 6.62607015e-34), "PLANCK ≈ 6.626e-34");
  test(physics::H == physics::PLANCK, "H alias works");
  test(approx_eq(physics::ELECTRON_MASS, 9.1093837015e-31),
       "ELECTRON_MASS ≈ 9.109e-31");
  test(physics::M_E == physics::ELECTRON_MASS, "M_E alias works");
  test(approx_eq(physics::BOLTZMANN, 1.380649e-23), "BOLTZMANN ≈ 1.38e-23");
  test(physics::STANDARD_PRESSURE == 101325.0, "STANDARD_PRESSURE = 101325 Pa");

  // ==================== Chemistry Constants ====================
  std::cout << "\n--- chemistry ---\n";

  test(approx_eq(chemistry::AVOGADRO, 6.02214076e23), "AVOGADRO ≈ 6.022e23");
  test(chemistry::N_A == chemistry::AVOGADRO, "N_A alias works");
  test(approx_eq(chemistry::GAS_CONSTANT, 8.314462618), "GAS_CONSTANT ≈ 8.314");
  test(chemistry::R == chemistry::GAS_CONSTANT, "R alias works");
  test(approx_eq(chemistry::FARADAY, 96485.33212), "FARADAY ≈ 96485");
  test(chemistry::MASS_CARBON_12 == 12.0,
       "MASS_CARBON_12 = 12 (by definition)");

  // ==================== Astronomy Constants ====================
  std::cout << "\n--- astronomy ---\n";

  test(approx_eq(astronomy::ASTRONOMICAL_UNIT, 1.495978707e11),
       "AU ≈ 1.496e11 m");
  test(astronomy::AU == astronomy::ASTRONOMICAL_UNIT, "AU alias works");
  test(approx_eq(astronomy::LIGHT_YEAR, 9.4607304725808e15),
       "LIGHT_YEAR ≈ 9.46e15 m");
  test(astronomy::LY == astronomy::LIGHT_YEAR, "LY alias works");
  test(approx_eq(astronomy::SOLAR_MASS, 1.98892e30),
       "SOLAR_MASS ≈ 1.989e30 kg");
  test(astronomy::EARTH_GRAVITY == 9.80665, "EARTH_GRAVITY = 9.80665 m/s²");

  // ==================== Engineering Constants ====================
  std::cout << "\n--- engineering ---\n";

  test(engineering::STANDARD_GRAVITY == 9.80665, "STANDARD_GRAVITY = 9.80665");
  test(engineering::WATER_DENSITY == 1000.0, "WATER_DENSITY = 1000 kg/m³");
  test(engineering::FEET_TO_METERS == 0.3048, "FEET_TO_METERS = 0.3048");
  test(approx_eq(engineering::HP_TO_WATTS, 745.69987158227022),
       "HP_TO_WATTS ≈ 745.7");

  // ==================== Computing Constants ====================
  std::cout << "\n--- computing ---\n";

  test(computing::KILOBYTE == 1000, "KILOBYTE = 1000");
  test(computing::KIBIBYTE == 1024, "KIBIBYTE = 1024");
  test(computing::MEGABYTE == 1000000, "MEGABYTE = 1000000");
  test(computing::MEBIBYTE == 1048576, "MEBIBYTE = 1048576");
  test(computing::BITS_PER_BYTE == 8, "BITS_PER_BYTE = 8");
  test(computing::INT8_MAX_VAL == 127, "INT8_MAX_VAL = 127");
  test(computing::UINT64_MAX_VAL == 18446744073709551615ULL,
       "UINT64_MAX_VAL correct");

  // ==================== Time Constants ====================
  std::cout << "\n--- time ---\n";

  test(time::SECONDS_PER_MINUTE == 60, "SECONDS_PER_MINUTE = 60");
  test(time::SECONDS_PER_HOUR == 3600, "SECONDS_PER_HOUR = 3600");
  test(time::SECONDS_PER_DAY == 86400, "SECONDS_PER_DAY = 86400");
  test(time::MS_PER_SECOND == 1000, "MS_PER_SECOND = 1000");
  test(time::NS_PER_SECOND == 1000000000, "NS_PER_SECOND = 1000000000");
  test(time::DAYS_PER_WEEK == 7, "DAYS_PER_WEEK = 7");

  // ==================== Cross-check Physics ====================
  std::cout << "\n--- cross-checks ---\n";

  // Verify Faraday = N_A * e
  double computed_faraday = chemistry::AVOGADRO * physics::ELEMENTARY_CHARGE;
  test(approx_eq(computed_faraday, chemistry::FARADAY, 1e-4), "F = N_A × e");

  // Verify Stefan-Boltzmann relation
  // σ = (2 * π^5 * k^4) / (15 * h^3 * c^2)
  // This is a complex formula, just check it's in the right ballpark
  test(physics::STEFAN_BOLTZMANN > 5.6e-8 && physics::STEFAN_BOLTZMANN < 5.7e-8,
       "STEFAN_BOLTZMANN in expected range");

  // ==================== Summary ====================
  std::cout << "\n=== Test Summary ===\n";
  std::cout << "Passed: " << passed << "\n";
  std::cout << "Failed: " << failed << "\n";

  return failed > 0 ? 1 : 0;
}
