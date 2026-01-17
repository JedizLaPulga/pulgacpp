// Test suite for pulgacpp Vector3<T>
// Compile: cl /std:c++latest /EHsc /W4 /I. test_vector3.cpp

#include "pulgacpp.hpp"
#include <cmath>
#include <iostream>


using namespace pulgacpp;

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
  std::cout << "=== Vector3<T> Test Suite ===\n\n";

  // ==================== Construction ====================
  std::cout << "--- Construction ---\n";

  auto v1 = Vector3<double>::from(1.0, 2.0, 3.0);
  test(v1.x() == 1.0 && v1.y() == 2.0 && v1.z() == 3.0,
       "Vector3::from() works");

  auto zero = Vector3<double>::zero();
  test(zero.is_zero(), "Vector3::zero() creates zero vector");

  auto unit_x = Vector3<double>::unit_x();
  test(unit_x.x() == 1.0 && unit_x.y() == 0.0 && unit_x.z() == 0.0,
       "unit_x() correct");

  auto unit_y = Vector3<double>::unit_y();
  test(unit_y.x() == 0.0 && unit_y.y() == 1.0 && unit_y.z() == 0.0,
       "unit_y() correct");

  auto unit_z = Vector3<double>::unit_z();
  test(unit_z.x() == 0.0 && unit_z.y() == 0.0 && unit_z.z() == 1.0,
       "unit_z() correct");

  // ==================== Accessors ====================
  std::cout << "\n--- Accessors ---\n";

  auto v2 = v1.with_x(10.0);
  test(v2.x() == 10.0 && v2.y() == 2.0 && v2.z() == 3.0, "with_x() works");

  auto v3 = v1.with_y(20.0);
  test(v3.x() == 1.0 && v3.y() == 20.0 && v3.z() == 3.0, "with_y() works");

  auto v4 = v1.with_z(30.0);
  test(v4.x() == 1.0 && v4.y() == 2.0 && v4.z() == 30.0, "with_z() works");

  // ==================== Magnitude ====================
  std::cout << "\n--- Magnitude ---\n";

  auto v345 = Vector3<double>::from(3.0, 4.0, 0.0);
  test(approx_eq(v345.magnitude(), 5.0), "magnitude() = 5 for (3,4,0)");
  test(approx_eq(v345.magnitude_squared(), 25.0), "magnitude_squared() = 25");

  // 3D Pythagorean: sqrt(1^2 + 2^2 + 2^2) = 3
  auto v122 = Vector3<double>::from(1.0, 2.0, 2.0);
  test(approx_eq(v122.magnitude(), 3.0), "magnitude() = 3 for (1,2,2)");

  test(unit_x.is_unit(), "unit_x is_unit() = true");
  test(!v1.is_unit(), "(1,2,3) is_unit() = false");

  // ==================== Dot Product ====================
  std::cout << "\n--- Dot Product ---\n";

  auto a = Vector3<double>::from(1.0, 2.0, 3.0);
  auto b = Vector3<double>::from(4.0, 5.0, 6.0);
  // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
  test(approx_eq(a.dot(b), 32.0), "dot product (1,2,3) · (4,5,6) = 32");

  // Perpendicular vectors have dot = 0
  test(approx_eq(unit_x.dot(unit_y), 0.0), "unit_x · unit_y = 0");
  test(approx_eq(unit_x.dot(unit_z), 0.0), "unit_x · unit_z = 0");

  // ==================== Cross Product ====================
  std::cout << "\n--- Cross Product ---\n";

  // x × y = z
  auto x_cross_y = unit_x.cross(unit_y);
  test(approx_eq(x_cross_y.x(), 0.0) && approx_eq(x_cross_y.y(), 0.0) &&
           approx_eq(x_cross_y.z(), 1.0),
       "unit_x × unit_y = unit_z");

  // y × x = -z
  auto y_cross_x = unit_y.cross(unit_x);
  test(approx_eq(y_cross_x.z(), -1.0), "unit_y × unit_x = -unit_z");

  // y × z = x
  auto y_cross_z = unit_y.cross(unit_z);
  test(approx_eq(y_cross_z.x(), 1.0), "unit_y × unit_z = unit_x");

  // Parallel vectors: cross product = 0
  auto self_cross = unit_x.cross(unit_x);
  test(approx_eq(self_cross.magnitude(), 0.0), "unit_x × unit_x = 0");

  // ==================== Angle Between ====================
  std::cout << "\n--- Angle Between ---\n";

  double angle_xy = unit_x.angle_to(unit_y);
  test(approx_eq(angle_xy, constants::PI / 2.0), "angle between x and y = π/2");

  double angle_same = unit_x.angle_to(unit_x);
  test(approx_eq(angle_same, 0.0), "angle between same vector = 0");

  auto neg_x = Vector3<double>::from(-1.0, 0.0, 0.0);
  double angle_opposite = unit_x.angle_to(neg_x);
  test(approx_eq(angle_opposite, constants::PI), "angle between opposite = π");

  // ==================== Distance ====================
  std::cout << "\n--- Distance ---\n";

  auto p1 = Vector3<double>::from(0.0, 0.0, 0.0);
  auto p2 = Vector3<double>::from(1.0, 0.0, 0.0);
  test(approx_eq(p1.distance_to(p2), 1.0), "distance (0,0,0) to (1,0,0) = 1");

  auto p3 = Vector3<double>::from(1.0, 1.0, 1.0);
  test(approx_eq(p1.distance_to(p3), std::sqrt(3.0)),
       "distance to (1,1,1) = √3");

  // ==================== Lerp ====================
  std::cout << "\n--- Linear Interpolation ---\n";

  auto start = Vector3<double>::from(0.0, 0.0, 0.0);
  auto end = Vector3<double>::from(10.0, 20.0, 30.0);

  auto mid = start.lerp(end, 0.5);
  test(approx_eq(mid.x(), 5.0) && approx_eq(mid.y(), 10.0) &&
           approx_eq(mid.z(), 15.0),
       "lerp(0.5) is midpoint");

  auto quarter = start.lerp(end, 0.25);
  test(approx_eq(quarter.x(), 2.5), "lerp(0.25) x = 2.5");

  // ==================== Normalize ====================
  std::cout << "\n--- Normalization ---\n";

  auto normalized = vec3_normalized(Vector3<double>::from(3.0, 0.0, 0.0));
  test(normalized.is_some(), "normalize non-zero returns Some");
  test(approx_eq(normalized.unwrap().magnitude(), 1.0),
       "normalized vector has magnitude 1");
  test(approx_eq(normalized.unwrap().x(), 1.0), "normalized (3,0,0) = (1,0,0)");

  auto zero_norm = vec3_normalized(Vector3<double>::zero());
  test(zero_norm.is_none(), "normalize zero vector returns None");

  // ==================== Projection ====================
  std::cout << "\n--- Projection ---\n";

  auto v_proj = Vector3<double>::from(1.0, 1.0, 0.0);
  auto onto = Vector3<double>::from(1.0, 0.0, 0.0);
  auto proj = v_proj.project_onto(onto);
  test(approx_eq(proj.x(), 1.0) && approx_eq(proj.y(), 0.0),
       "project (1,1,0) onto x-axis = (1,0,0)");

  // ==================== Reflection ====================
  std::cout << "\n--- Reflection ---\n";

  auto incoming = Vector3<double>::from(1.0, -1.0, 0.0);
  auto normal = Vector3<double>::from(0.0, 1.0, 0.0);
  auto reflected = incoming.reflect(normal);
  // Reflecting (1,-1,0) off horizontal surface -> (1,1,0)
  test(approx_eq(reflected.x(), 1.0) && approx_eq(reflected.y(), 1.0),
       "reflection off horizontal surface");

  // ==================== Triple Product ====================
  std::cout << "\n--- Triple Product ---\n";

  // a · (b × c) for unit vectors = 1 (volume of unit cube)
  double triple = vec3_triple_product(unit_x, unit_y, unit_z);
  test(approx_eq(triple, 1.0), "triple product of unit basis = 1");

  // ==================== Spherical Coordinates ====================
  std::cout << "\n--- Spherical Coordinates ---\n";

  // r=1, theta=0, phi=π/2 should give (1,0,0)
  auto from_spherical = vec3_from_spherical(1.0, 0.0, constants::PI / 2.0);
  test(approx_eq(from_spherical.x(), 1.0, 1e-6), "spherical (1,0,π/2) x ≈ 1");
  test(approx_eq(from_spherical.z(), 0.0, 1e-6), "spherical (1,0,π/2) z ≈ 0");

  // ==================== Integer Type ====================
  std::cout << "\n--- Integer Type ---\n";

  auto vi = Vector3<int>::from(1, 2, 3);
  test(vi.x() == 1 && vi.y() == 2 && vi.z() == 3, "Vector3<int> works");

  auto vi_add = vi.checked_add(Vector3<int>::from(10, 20, 30));
  test(vi_add.is_some(), "Vector3<int> checked_add returns Some");
  test(vi_add.unwrap().x() == 11, "checked_add result correct");

  // ==================== Type Aliases ====================
  std::cout << "\n--- Type Aliases ---\n";

  Vec3d vd = Vec3d::from(1.0, 2.0, 3.0);
  test(vd.x() == 1.0, "Vec3d alias works");

  Vec3f vf = Vec3f::from(1.0f, 2.0f, 3.0f);
  test(vf.x() == 1.0f, "Vec3f alias works");

  Vec3i vi2 = Vec3i::from(1, 2, 3);
  test(vi2.x() == 1, "Vec3i alias works");

  // ==================== Summary ====================
  std::cout << "\n=== Test Summary ===\n";
  std::cout << "Passed: " << passed << "\n";
  std::cout << "Failed: " << failed << "\n";

  return failed > 0 ? 1 : 0;
}
