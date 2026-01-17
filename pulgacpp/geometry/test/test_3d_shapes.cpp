// Test suite for pulgacpp Sphere<T> and Box<T>
// Compile: cl /std:c++latest /EHsc /W4 /I. test_3d_shapes.cpp

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

bool approx_eq(double a, double b, double tol = 1e-6) {
  return std::abs(a - b) < tol;
}

int main() {
  std::cout << "=== Sphere<T> & Box<T> Test Suite ===\n\n";

  // ============================================================
  // SPHERE TESTS
  // ============================================================
  std::cout << "============ SPHERE TESTS ============\n\n";

  // ==================== Construction ====================
  std::cout << "--- Sphere Construction ---\n";

  auto center = Vector3<double>::from(1.0, 2.0, 3.0);
  auto sphere = Sphere<double>::from(center, 5.0);
  test(sphere.is_some(), "Sphere::from() with positive radius returns Some");
  test(sphere.unwrap().radius() == 5.0, "radius() correct");
  test(sphere.unwrap().center() == center, "center() correct");

  auto invalid = Sphere<double>::from(center, -1.0);
  test(invalid.is_none(), "Sphere::from() with negative radius returns None");

  auto unit = Sphere<double>::unit();
  test(unit.radius() == 1.0, "unit() has radius 1");
  test(unit.center().is_zero(), "unit() centered at origin");

  // ==================== Geometry ====================
  std::cout << "\n--- Sphere Geometry ---\n";

  auto s = Sphere<double>::from(Vector3<double>::zero(), 2.0).unwrap();

  test(approx_eq(s.diameter(), 4.0), "diameter = 2r");
  test(approx_eq(s.circumference(), 2.0 * constants::math::PI * 2.0),
       "circumference = 2πr");
  test(approx_eq(s.surface_area(), 4.0 * constants::math::PI * 4.0),
       "surface_area = 4πr²");
  test(approx_eq(s.volume(), (4.0 / 3.0) * constants::math::PI * 8.0),
       "volume = 4/3 πr³");

  // ==================== Containment ====================
  std::cout << "\n--- Sphere Containment ---\n";

  auto s2 = Sphere<double>::from(Vector3<double>::zero(), 5.0).unwrap();

  auto inside = Vector3<double>::from(1.0, 1.0, 1.0);
  auto on_surface = Vector3<double>::from(5.0, 0.0, 0.0);
  auto outside = Vector3<double>::from(10.0, 0.0, 0.0);

  test(s2.contains(inside), "contains() true for interior point");
  test(s2.strictly_contains(inside), "strictly_contains() true for interior");
  test(s2.contains(on_surface), "contains() true for surface point");
  test(!s2.strictly_contains(on_surface),
       "strictly_contains() false for surface");
  test(!s2.contains(outside), "contains() false for exterior point");
  test(s2.on_surface(on_surface), "on_surface() true for boundary");

  // ==================== Sphere Relationships ====================
  std::cout << "\n--- Sphere Relationships ---\n";

  auto sphere1 = Sphere<double>::from(Vector3<double>::zero(), 5.0).unwrap();
  auto sphere2 =
      Sphere<double>::from(Vector3<double>::from(3.0, 0.0, 0.0), 5.0).unwrap();
  auto sphere3 =
      Sphere<double>::from(Vector3<double>::from(20.0, 0.0, 0.0), 2.0).unwrap();
  auto sphere4 =
      Sphere<double>::from(Vector3<double>::from(1.0, 0.0, 0.0), 2.0).unwrap();

  test(sphere1.overlaps(sphere2), "overlapping spheres overlap");
  test(sphere1.intersects(sphere2), "overlapping spheres intersect");
  test(!sphere1.overlaps(sphere3), "distant spheres don't overlap");
  test(!sphere1.intersects(sphere3), "distant spheres don't intersect");
  test(sphere1.contains_sphere(sphere4), "large sphere contains small");
  test(!sphere4.contains_sphere(sphere1), "small doesn't contain large");

  test(approx_eq(sphere1.distance_between_centers(sphere2), 3.0),
       "distance between centers");
  test(sphere1.distance_to_sphere(sphere3) > 0,
       "distance to distant sphere > 0");

  // ==================== Transformations ====================
  std::cout << "\n--- Sphere Transformations ---\n";

  auto translated = sphere1.checked_translate(5.0, 0.0, 0.0);
  test(translated.is_some(), "checked_translate returns Some");
  test(approx_eq(translated.unwrap().center().x(), 5.0),
       "translated x correct");

  auto scaled = sphere1.checked_scale(2.0);
  test(scaled.is_some() && approx_eq(scaled.unwrap().radius(), 10.0),
       "scaled radius doubled");

  auto expanded = sphere1.expanded(1.0);
  test(expanded.is_some() && approx_eq(expanded.unwrap().radius(), 6.0),
       "expanded by 1");

  // ==================== Points on Sphere ====================
  std::cout << "\n--- Points on Sphere ---\n";

  auto north = sphere1.north_pole();
  test(approx_eq(north.z(), 5.0), "north_pole z = radius");

  auto south = sphere1.south_pole();
  test(approx_eq(south.z(), -5.0), "south_pole z = -radius");

  auto at_equator = sphere1.point_at(0.0, constants::math::PI / 2.0);
  test(approx_eq(at_equator.x(), 5.0), "point_at equator x = radius");

  // ============================================================
  // BOX TESTS
  // ============================================================
  std::cout << "\n============ BOX TESTS ============\n\n";

  // ==================== Construction ====================
  std::cout << "--- Box Construction ---\n";

  auto min_c = Vector3<double>::from(0.0, 0.0, 0.0);
  auto max_c = Vector3<double>::from(10.0, 20.0, 30.0);
  auto box = Box<double>::from_corners(min_c, max_c);

  test(box.is_some(), "from_corners with valid corners returns Some");
  test(box.unwrap().min() == min_c, "min() correct");
  test(box.unwrap().max() == max_c, "max() correct");

  auto invalid_box = Box<double>::from_corners(max_c, min_c);
  test(invalid_box.is_none(), "from_corners with swapped corners returns None");

  auto from_pts = Box<double>::from_points(max_c, min_c);
  test(from_pts.min() == min_c, "from_points auto-sorts min");
  test(from_pts.max() == max_c, "from_points auto-sorts max");

  auto unit_box = Box<double>::unit();
  test(approx_eq(unit_box.width(), 1.0), "unit() width = 1");

  auto cube = Box<double>::cube(Vector3<double>::zero(), 5.0);
  test(cube.is_some() && cube.unwrap().is_cube(), "cube() creates cube");

  // ==================== Accessors ====================
  std::cout << "\n--- Box Accessors ---\n";

  auto b = box.unwrap();

  test(approx_eq(b.width(), 10.0), "width() = 10");
  test(approx_eq(b.height(), 20.0), "height() = 20");
  test(approx_eq(b.depth(), 30.0), "depth() = 30");

  auto c = b.center();
  test(approx_eq(c.x(), 5.0) && approx_eq(c.y(), 10.0) &&
           approx_eq(c.z(), 15.0),
       "center() correct");

  auto sz = b.size();
  test(approx_eq(sz.x(), 10.0) && approx_eq(sz.y(), 20.0) &&
           approx_eq(sz.z(), 30.0),
       "size() correct");

  // ==================== Geometry ====================
  std::cout << "\n--- Box Geometry ---\n";

  test(approx_eq(b.volume(), 10.0 * 20.0 * 30.0), "volume = w*h*d");
  test(approx_eq(b.surface_area(),
                 2.0 * (10.0 * 20.0 + 20.0 * 30.0 + 30.0 * 10.0)),
       "surface_area correct");
  test(approx_eq(b.diagonal(),
                 std::sqrt(10.0 * 10.0 + 20.0 * 20.0 + 30.0 * 30.0)),
       "diagonal correct");
  test(!b.is_cube(), "10x20x30 is not a cube");

  // ==================== Corners ====================
  std::cout << "\n--- Box Corners ---\n";

  auto corners = b.corners();
  test(corners.size() == 8, "8 corners");
  test(corners[0] == b.min(), "corner[0] is min");
  test(corners[7] == b.max(), "corner[7] is max");

  auto corner3 = b.corner(3); // ++-
  test(approx_eq(corner3.x(), 10.0) && approx_eq(corner3.y(), 20.0) &&
           approx_eq(corner3.z(), 0.0),
       "corner(3) = ++-");

  // ==================== Containment ====================
  std::cout << "\n--- Box Containment ---\n";

  auto inside_pt = Vector3<double>::from(5.0, 10.0, 15.0);
  auto on_face = Vector3<double>::from(0.0, 10.0, 15.0);
  auto outside_pt = Vector3<double>::from(50.0, 10.0, 15.0);

  test(b.contains(inside_pt), "contains() true for interior");
  test(b.strictly_contains(inside_pt), "strictly_contains() true for interior");
  test(b.contains(on_face), "contains() true for face point");
  test(!b.strictly_contains(on_face),
       "strictly_contains() false for face point");
  test(!b.contains(outside_pt), "contains() false for exterior");

  auto smaller = Box<double>::from_corners(Vector3<double>::from(1.0, 1.0, 1.0),
                                           Vector3<double>::from(5.0, 5.0, 5.0))
                     .unwrap();
  test(b.contains_box(smaller), "large box contains smaller");

  // ==================== Box Relationships ====================
  std::cout << "\n--- Box Relationships ---\n";

  auto box1 = Box<double>::from_corners(Vector3<double>::from(0.0, 0.0, 0.0),
                                        Vector3<double>::from(10.0, 10.0, 10.0))
                  .unwrap();
  auto box2 = Box<double>::from_corners(Vector3<double>::from(5.0, 5.0, 5.0),
                                        Vector3<double>::from(15.0, 15.0, 15.0))
                  .unwrap();
  auto box3 =
      Box<double>::from_corners(Vector3<double>::from(100.0, 100.0, 100.0),
                                Vector3<double>::from(110.0, 110.0, 110.0))
          .unwrap();

  test(box1.intersects(box2), "overlapping boxes intersect");
  test(!box1.intersects(box3), "distant boxes don't intersect");

  auto inter = box1.intersection(box2);
  test(inter.is_some(), "intersection returns Some for overlapping");
  test(approx_eq(inter.unwrap().volume(), 5.0 * 5.0 * 5.0),
       "intersection volume = 125");

  auto merged = box1.merged_with(box3);
  test(approx_eq(merged.min().x(), 0.0), "merged min x");
  test(approx_eq(merged.max().x(), 110.0), "merged max x");

  // ==================== Transformations ====================
  std::cout << "\n--- Box Transformations ---\n";

  auto box_trans = box1.checked_translate(10.0, 0.0, 0.0);
  test(box_trans.is_some(), "checked_translate returns Some");
  test(approx_eq(box_trans.unwrap().min().x(), 10.0), "translated min x");

  auto box_scaled = box1.scaled(2.0);
  test(box_scaled.is_some() && approx_eq(box_scaled.unwrap().width(), 20.0),
       "scaled width doubled");

  auto box_exp = box1.expanded(1.0);
  test(box_exp.is_some() && approx_eq(box_exp.unwrap().width(), 12.0),
       "expanded by 1 each side");

  // ==================== Integer Types ====================
  std::cout << "\n--- Integer Types ---\n";

  auto int_sphere = Sphere<int>::from(Vector3<int>::from(0, 0, 0), 5);
  test(int_sphere.is_some() && int_sphere.unwrap().radius() == 5,
       "Sphere<int> works");

  auto int_box = Box<int>::from_corners(Vector3<int>::from(0, 0, 0),
                                        Vector3<int>::from(10, 10, 10));
  test(int_box.is_some() && int_box.unwrap().width() == 10.0, "Box<int> works");

  // ==================== Type Aliases ====================
  std::cout << "\n--- Type Aliases ---\n";

  Sphered sd = Sphered::unit();
  test(sd.radius() == 1.0, "Sphered alias works");

  Boxd bd = Boxd::unit();
  test(approx_eq(bd.volume(), 1.0), "Boxd alias works");

  AABB aabb = AABB::unit();
  test(approx_eq(aabb.volume(), 1.0), "AABB alias works");

  // ==================== Summary ====================
  std::cout << "\n=== Test Summary ===\n";
  std::cout << "Passed: " << passed << "\n";
  std::cout << "Failed: " << failed << "\n";

  return failed > 0 ? 1 : 0;
}
