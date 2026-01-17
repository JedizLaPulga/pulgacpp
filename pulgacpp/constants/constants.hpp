// pulgacpp::constants - Scientific and mathematical constants
// SPDX-License-Identifier: MIT
//
// Comprehensive collection of fundamental constants organized by discipline.
// All values use SI units unless otherwise noted.
// Naming follows Rust convention: UPPER_SNAKE_CASE for constants.
//
// Usage:
//   #include <pulgacpp/constants/constants.hpp>
//   using namespace pulgacpp::constants::physics;
//   double c = SPEED_OF_LIGHT;

#ifndef PULGACPP_CONSTANTS_HPP
#define PULGACPP_CONSTANTS_HPP

#include <cstdint>
#include <numbers>


namespace pulgacpp {
namespace constants {

// ============================================================
// Mathematical Constants
// ============================================================
namespace math {

/// π (pi) - ratio of circumference to diameter
inline constexpr double PI = 3.14159265358979323846;

/// τ (tau) - 2π, full circle in radians
inline constexpr double TAU = 2.0 * PI;

/// e (Euler's number) - base of natural logarithm
inline constexpr double E = 2.71828182845904523536;

/// φ (phi) - golden ratio (1 + √5) / 2
inline constexpr double PHI = 1.61803398874989484820;

/// √2 - square root of 2
inline constexpr double SQRT_2 = 1.41421356237309504880;

/// √3 - square root of 3
inline constexpr double SQRT_3 = 1.73205080756887729353;

/// √5 - square root of 5
inline constexpr double SQRT_5 = 2.23606797749978969641;

/// 1/√2 - inverse square root of 2
inline constexpr double INV_SQRT_2 = 0.70710678118654752440;

/// ln(2) - natural log of 2
inline constexpr double LN_2 = 0.69314718055994530942;

/// ln(10) - natural log of 10
inline constexpr double LN_10 = 2.30258509299404568402;

/// log₁₀(e) - common log of e
inline constexpr double LOG10_E = 0.43429448190325182765;

/// log₂(e) - binary log of e
inline constexpr double LOG2_E = 1.44269504088896340736;

/// γ (Euler-Mascheroni constant)
inline constexpr double EULER_GAMMA = 0.57721566490153286060;

/// Degrees per radian
inline constexpr double DEG_PER_RAD = 180.0 / PI;

/// Radians per degree
inline constexpr double RAD_PER_DEG = PI / 180.0;

} // namespace math

// ============================================================
// Physics Constants (SI units)
// ============================================================
namespace physics {

// -------------------- Fundamental Constants --------------------

/// Speed of light in vacuum (m/s)
inline constexpr double SPEED_OF_LIGHT = 299792458.0;
inline constexpr double C = SPEED_OF_LIGHT; // Alias

/// Planck constant (J⋅s)
inline constexpr double PLANCK = 6.62607015e-34;
inline constexpr double H = PLANCK; // Alias

/// Reduced Planck constant ℏ = h/(2π) (J⋅s)
inline constexpr double HBAR = 1.054571817e-34;

/// Gravitational constant (m³/(kg⋅s²))
inline constexpr double GRAVITATIONAL = 6.67430e-11;
inline constexpr double G = GRAVITATIONAL; // Alias

/// Elementary charge (C)
inline constexpr double ELEMENTARY_CHARGE = 1.602176634e-19;
inline constexpr double Q_E = ELEMENTARY_CHARGE; // Alias

/// Electron mass (kg)
inline constexpr double ELECTRON_MASS = 9.1093837015e-31;
inline constexpr double M_E = ELECTRON_MASS; // Alias

/// Proton mass (kg)
inline constexpr double PROTON_MASS = 1.67262192369e-27;
inline constexpr double M_P = PROTON_MASS; // Alias

/// Neutron mass (kg)
inline constexpr double NEUTRON_MASS = 1.67492749804e-27;
inline constexpr double M_N = NEUTRON_MASS; // Alias

/// Atomic mass unit (kg)
inline constexpr double ATOMIC_MASS_UNIT = 1.66053906660e-27;
inline constexpr double U = ATOMIC_MASS_UNIT; // Alias

/// Boltzmann constant (J/K)
inline constexpr double BOLTZMANN = 1.380649e-23;
inline constexpr double K_B = BOLTZMANN; // Alias

/// Vacuum permittivity ε₀ (F/m)
inline constexpr double VACUUM_PERMITTIVITY = 8.8541878128e-12;
inline constexpr double EPSILON_0 = VACUUM_PERMITTIVITY; // Alias

/// Vacuum permeability μ₀ (H/m)
inline constexpr double VACUUM_PERMEABILITY = 1.25663706212e-6;
inline constexpr double MU_0 = VACUUM_PERMEABILITY; // Alias

/// Fine-structure constant α ≈ 1/137 (dimensionless)
inline constexpr double FINE_STRUCTURE = 7.2973525693e-3;
inline constexpr double ALPHA = FINE_STRUCTURE; // Alias

/// Rydberg constant (1/m)
inline constexpr double RYDBERG = 10973731.568160;

/// Bohr radius (m)
inline constexpr double BOHR_RADIUS = 5.29177210903e-11;
inline constexpr double A_0 = BOHR_RADIUS; // Alias

/// Stefan-Boltzmann constant (W/(m²⋅K⁴))
inline constexpr double STEFAN_BOLTZMANN = 5.670374419e-8;
inline constexpr double SIGMA = STEFAN_BOLTZMANN; // Alias

/// Wien's displacement constant (m⋅K)
inline constexpr double WIEN = 2.897771955e-3;

// -------------------- Electromagnetic --------------------

/// Coulomb constant k = 1/(4πε₀) (N⋅m²/C²)
inline constexpr double COULOMB = 8.9875517923e9;
inline constexpr double K_E = COULOMB; // Alias

/// Magnetic flux quantum Φ₀ (Wb)
inline constexpr double MAGNETIC_FLUX_QUANTUM = 2.067833848e-15;

/// Conductance quantum (S)
inline constexpr double CONDUCTANCE_QUANTUM = 7.748091729e-5;

/// Electron volt (J)
inline constexpr double ELECTRON_VOLT = 1.602176634e-19;
inline constexpr double EV = ELECTRON_VOLT; // Alias

// -------------------- Thermodynamics --------------------

/// Absolute zero (°C)
inline constexpr double ABSOLUTE_ZERO_CELSIUS = -273.15;

/// Standard temperature (K)
inline constexpr double STANDARD_TEMPERATURE = 273.15;

/// Standard pressure (Pa)
inline constexpr double STANDARD_PRESSURE = 101325.0;

/// Standard atmosphere (Pa)
inline constexpr double ATM = 101325.0;

} // namespace physics

// ============================================================
// Chemistry Constants (SI units)
// ============================================================
namespace chemistry {

/// Avogadro constant (1/mol)
inline constexpr double AVOGADRO = 6.02214076e23;
inline constexpr double N_A = AVOGADRO; // Alias

/// Molar gas constant R (J/(mol⋅K))
inline constexpr double GAS_CONSTANT = 8.314462618;
inline constexpr double R = GAS_CONSTANT; // Alias

/// Faraday constant F = N_A × e (C/mol)
inline constexpr double FARADAY = 96485.33212;
inline constexpr double F = FARADAY; // Alias

/// Molar Planck constant (J⋅s/mol)
inline constexpr double MOLAR_PLANCK = 3.990312712e-10;

/// Standard molar volume of ideal gas at STP (m³/mol)
inline constexpr double MOLAR_VOLUME_STP = 0.022413969;

/// First radiation constant c₁ = 2πhc² (W⋅m²)
inline constexpr double FIRST_RADIATION = 3.741771852e-16;

/// Second radiation constant c₂ = hc/k_B (m⋅K)
inline constexpr double SECOND_RADIATION = 0.01438776877;

// -------------------- Atomic Masses (in atomic mass units)
// --------------------

/// Hydrogen atomic mass (u)
inline constexpr double MASS_HYDROGEN = 1.00794;

/// Carbon-12 atomic mass (u) - by definition
inline constexpr double MASS_CARBON_12 = 12.0;

/// Oxygen atomic mass (u)
inline constexpr double MASS_OXYGEN = 15.999;

/// Nitrogen atomic mass (u)
inline constexpr double MASS_NITROGEN = 14.007;

/// Helium atomic mass (u)
inline constexpr double MASS_HELIUM = 4.002602;

/// Iron atomic mass (u)
inline constexpr double MASS_IRON = 55.845;

/// Gold atomic mass (u)
inline constexpr double MASS_GOLD = 196.96657;

/// Uranium-238 atomic mass (u)
inline constexpr double MASS_URANIUM_238 = 238.02891;

} // namespace chemistry

// ============================================================
// Astronomy Constants (SI units)
// ============================================================
namespace astronomy {

/// Astronomical unit AU (m)
inline constexpr double ASTRONOMICAL_UNIT = 1.495978707e11;
inline constexpr double AU = ASTRONOMICAL_UNIT; // Alias

/// Light year (m)
inline constexpr double LIGHT_YEAR = 9.4607304725808e15;
inline constexpr double LY = LIGHT_YEAR; // Alias

/// Parsec (m)
inline constexpr double PARSEC = 3.0856775814913673e16;
inline constexpr double PC = PARSEC; // Alias

/// Solar mass (kg)
inline constexpr double SOLAR_MASS = 1.98892e30;
inline constexpr double M_SUN = SOLAR_MASS; // Alias

/// Solar radius (m)
inline constexpr double SOLAR_RADIUS = 6.9634e8;
inline constexpr double R_SUN = SOLAR_RADIUS; // Alias

/// Solar luminosity (W)
inline constexpr double SOLAR_LUMINOSITY = 3.828e26;
inline constexpr double L_SUN = SOLAR_LUMINOSITY; // Alias

/// Earth mass (kg)
inline constexpr double EARTH_MASS = 5.97237e24;
inline constexpr double M_EARTH = EARTH_MASS; // Alias

/// Earth radius (equatorial, m)
inline constexpr double EARTH_RADIUS = 6.3781e6;
inline constexpr double R_EARTH = EARTH_RADIUS; // Alias

/// Earth surface gravity (m/s²)
inline constexpr double EARTH_GRAVITY = 9.80665;
inline constexpr double G_EARTH = EARTH_GRAVITY; // Alias

/// Moon mass (kg)
inline constexpr double MOON_MASS = 7.342e22;
inline constexpr double M_MOON = MOON_MASS; // Alias

/// Moon radius (m)
inline constexpr double MOON_RADIUS = 1.7374e6;
inline constexpr double R_MOON = MOON_RADIUS; // Alias

/// Jupiter mass (kg)
inline constexpr double JUPITER_MASS = 1.8982e27;
inline constexpr double M_JUPITER = JUPITER_MASS; // Alias

/// Hubble constant H₀ (km/s/Mpc) - approximate, subject to revision
inline constexpr double HUBBLE = 67.4;
inline constexpr double H_0 = HUBBLE; // Alias

/// Age of universe (seconds) - approximate
inline constexpr double UNIVERSE_AGE = 4.35e17;

/// Cosmic microwave background temperature (K)
inline constexpr double CMB_TEMPERATURE = 2.72548;

} // namespace astronomy

// ============================================================
// Engineering Constants
// ============================================================
namespace engineering {

/// Standard gravity (m/s²)
inline constexpr double STANDARD_GRAVITY = 9.80665;
inline constexpr double G_N = STANDARD_GRAVITY; // Alias

/// Speed of sound in air at 20°C (m/s)
inline constexpr double SPEED_OF_SOUND_AIR = 343.0;

/// Density of water at 4°C (kg/m³)
inline constexpr double WATER_DENSITY = 1000.0;

/// Density of air at STP (kg/m³)
inline constexpr double AIR_DENSITY_STP = 1.2754;

/// Specific heat of water (J/(kg⋅K))
inline constexpr double WATER_SPECIFIC_HEAT = 4184.0;

/// Thermal conductivity of copper (W/(m⋅K))
inline constexpr double COPPER_CONDUCTIVITY = 401.0;

/// Thermal conductivity of aluminum (W/(m⋅K))
inline constexpr double ALUMINUM_CONDUCTIVITY = 237.0;

/// Young's modulus of steel (Pa)
inline constexpr double STEEL_YOUNGS_MODULUS = 2.0e11;

/// Yield strength of mild steel (Pa)
inline constexpr double STEEL_YIELD_STRENGTH = 2.5e8;

// -------------------- Unit Conversions --------------------

/// Feet to meters
inline constexpr double FEET_TO_METERS = 0.3048;

/// Inches to meters
inline constexpr double INCHES_TO_METERS = 0.0254;

/// Miles to meters
inline constexpr double MILES_TO_METERS = 1609.344;

/// Pounds to kilograms
inline constexpr double POUNDS_TO_KG = 0.45359237;

/// Horsepower to watts
inline constexpr double HP_TO_WATTS = 745.69987158227022;

/// PSI to Pascals
inline constexpr double PSI_TO_PA = 6894.757293168361;

/// Bar to Pascals
inline constexpr double BAR_TO_PA = 100000.0;

/// Calorie to Joules
inline constexpr double CAL_TO_J = 4.184;

/// BTU to Joules
inline constexpr double BTU_TO_J = 1055.05585;

} // namespace engineering

// ============================================================
// Computer Science Constants
// ============================================================
namespace computing {

/// Bytes in a kilobyte (base-10, SI)
inline constexpr std::uint64_t KILOBYTE = 1000ULL;
inline constexpr std::uint64_t KB = KILOBYTE;

/// Bytes in a megabyte (base-10, SI)
inline constexpr std::uint64_t MEGABYTE = 1000000ULL;
inline constexpr std::uint64_t MB = MEGABYTE;

/// Bytes in a gigabyte (base-10, SI)
inline constexpr std::uint64_t GIGABYTE = 1000000000ULL;
inline constexpr std::uint64_t GB = GIGABYTE;

/// Bytes in a terabyte (base-10, SI)
inline constexpr std::uint64_t TERABYTE = 1000000000000ULL;
inline constexpr std::uint64_t TB = TERABYTE;

/// Bytes in a kibibyte (base-2, IEC)
inline constexpr std::uint64_t KIBIBYTE = 1024ULL;
inline constexpr std::uint64_t KIB = KIBIBYTE;

/// Bytes in a mebibyte (base-2, IEC)
inline constexpr std::uint64_t MEBIBYTE = 1048576ULL;
inline constexpr std::uint64_t MIB = MEBIBYTE;

/// Bytes in a gibibyte (base-2, IEC)
inline constexpr std::uint64_t GIBIBYTE = 1073741824ULL;
inline constexpr std::uint64_t GIB = GIBIBYTE;

/// Bytes in a tebibyte (base-2, IEC)
inline constexpr std::uint64_t TEBIBYTE = 1099511627776ULL;
inline constexpr std::uint64_t TIB = TEBIBYTE;

/// Bits in a byte
inline constexpr unsigned BITS_PER_BYTE = 8;

/// Maximum value of signed 8-bit integer
inline constexpr std::int8_t INT8_MAX_VAL = 127;

/// Maximum value of signed 16-bit integer
inline constexpr std::int16_t INT16_MAX_VAL = 32767;

/// Maximum value of signed 32-bit integer
inline constexpr std::int32_t INT32_MAX_VAL = 2147483647;

/// Maximum value of signed 64-bit integer
inline constexpr std::int64_t INT64_MAX_VAL = 9223372036854775807LL;

/// Maximum value of unsigned 8-bit integer
inline constexpr std::uint8_t UINT8_MAX_VAL = 255;

/// Maximum value of unsigned 16-bit integer
inline constexpr std::uint16_t UINT16_MAX_VAL = 65535;

/// Maximum value of unsigned 32-bit integer
inline constexpr std::uint32_t UINT32_MAX_VAL = 4294967295U;

/// Maximum value of unsigned 64-bit integer
inline constexpr std::uint64_t UINT64_MAX_VAL = 18446744073709551615ULL;

} // namespace computing

// ============================================================
// Time Constants
// ============================================================
namespace time {

/// Seconds in a minute
inline constexpr std::int64_t SECONDS_PER_MINUTE = 60;

/// Seconds in an hour
inline constexpr std::int64_t SECONDS_PER_HOUR = 3600;

/// Seconds in a day
inline constexpr std::int64_t SECONDS_PER_DAY = 86400;

/// Seconds in a week
inline constexpr std::int64_t SECONDS_PER_WEEK = 604800;

/// Seconds in a year (365 days)
inline constexpr std::int64_t SECONDS_PER_YEAR = 31536000;

/// Seconds in a leap year (366 days)
inline constexpr std::int64_t SECONDS_PER_LEAP_YEAR = 31622400;

/// Milliseconds in a second
inline constexpr std::int64_t MS_PER_SECOND = 1000;

/// Microseconds in a second
inline constexpr std::int64_t US_PER_SECOND = 1000000;

/// Nanoseconds in a second
inline constexpr std::int64_t NS_PER_SECOND = 1000000000;

/// Days in a week
inline constexpr int DAYS_PER_WEEK = 7;

/// Months in a year
inline constexpr int MONTHS_PER_YEAR = 12;

/// Average days in a month (approximate)
inline constexpr double AVG_DAYS_PER_MONTH = 30.436875;

/// Average days in a year (accounting for leap years)
inline constexpr double AVG_DAYS_PER_YEAR = 365.2425;

/// Julian year in seconds
inline constexpr double JULIAN_YEAR = 31557600.0;

} // namespace time

} // namespace constants
} // namespace pulgacpp

#endif // PULGACPP_CONSTANTS_HPP
