// Test suite for pulgacpp LineSegment<T>
// Compile: cl /std:c++latest /EHsc /W4 /I. test_linesegment.cpp

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
  std::cout << "=== LineSegment<T> Test Suite ===\n\n";

  // ==================== Construction ====================
  std::cout << "--- Construction ---\n";

  auto p1 = Point<double>::from(0.0, 0.0);
  auto p2 = Point<double>::from(3.0, 4.0);
  auto seg = LineSegment<double>::from(p1, p2);

  test(seg.start() == p1, "start() accessor");
  test(seg.end() == p2, "end() accessor");
  test(seg.p1() == p1, "p1() alias");
  test(seg.p2() == p2, "p2() alias");

  auto horiz = LineSegment<double>::horizontal(5.0, 0.0, 10.0);
  test(approx_eq(horiz.start().y(), 5.0), "horizontal() y correct");
  test(approx_eq(horiz.start().x(), 0.0), "horizontal() min x correct");

  auto vert = LineSegment<double>::vertical(3.0, 0.0, 10.0);
  test(approx_eq(vert.start().x(), 3.0), "vertical() x correct");

  auto unit_x = LineSegment<double>::unit_x();
  test(approx_eq(unit_x.length(), 1.0), "unit_x() has length 1");

  // ==================== Length & Midpoint ====================
  std::cout << "\n--- Length & Midpoint ---\n";

  // 3-4-5 triangle
  test(approx_eq(seg.length(), 5.0), "length() = 5 for (0,0)-(3,4)");
  test(approx_eq(seg.length_squared(), 25.0), "length_squared() = 25");

  auto mid = seg.midpoint();
  test(approx_eq(mid.x(), 1.5) && approx_eq(mid.y(), 2.0),
       "midpoint() correct");

  auto degen = LineSegment<double>::from(p1, p1);
  test(degen.is_degenerate(), "is_degenerate() true for zero-length");
  test(!seg.is_degenerate(), "is_degenerate() false for normal segment");

  // Point at parameter
  auto quarter = seg.point_at(0.25);
  test(approx_eq(quarter.x(), 0.75) && approx_eq(quarter.y(), 1.0),
       "point_at(0.25) correct");

  auto lerp_mid = seg.lerp(0.5);
  test(approx_eq(lerp_mid.x(), mid.x()), "lerp(0.5) == midpoint");

  // ==================== Direction & Vectors ====================
  std::cout << "\n--- Direction & Vectors ---\n";

  auto dir = seg.direction();
  test(approx_eq(dir.x(), 3.0) && approx_eq(dir.y(), 4.0),
       "direction() correct");

  auto unit_dir = seg.unit_direction();
  test(unit_dir.is_some(), "unit_direction() returns Some");
  test(approx_eq(unit_dir.unwrap().magnitude(), 1.0),
       "unit_direction() has magnitude 1");

  auto normal = seg.normal();
  test(approx_eq(normal.dot(dir), 0.0), "normal is perpendicular to direction");

  test(degen.unit_direction().is_none(),
       "degenerate segment has None unit_direction");

  // Angle
  auto horiz_seg = LineSegment<double>::from(Point<double>::origin(),
                                             Point<double>::from(1.0, 0.0));
  test(approx_eq(horiz_seg.angle(), 0.0), "horizontal segment angle = 0");

  // ==================== Closest Point ====================
  std::cout << "\n--- Closest Point ---\n";

  // Segment from (0,0) to (10,0)
  auto h_seg = LineSegment<double>::from(Point<double>::origin(),
                                         Point<double>::from(10.0, 0.0));

  // Point directly above middle
  auto test_point = Point<double>::from(5.0, 5.0);
  auto closest = h_seg.closest_point_to(test_point);
  test(approx_eq(closest.x(), 5.0) && approx_eq(closest.y(), 0.0),
       "closest_point_to for point above segment");

  // Point before start
  auto before = Point<double>::from(-5.0, 3.0);
  auto closest_before = h_seg.closest_point_to(before);
  test(approx_eq(closest_before.x(), 0.0) && approx_eq(closest_before.y(), 0.0),
       "closest_point clamps to start");

  // Point after end
  auto after = Point<double>::from(15.0, 3.0);
  auto closest_after = h_seg.closest_point_to(after);
  test(approx_eq(closest_after.x(), 10.0) && approx_eq(closest_after.y(), 0.0),
       "closest_point clamps to end");

  // ==================== Distance ====================
  std::cout << "\n--- Distance ---\n";

  auto dist = h_seg.distance_to_point(test_point);
  test(approx_eq(dist, 5.0), "distance_to_point = 5 for point 5 units above");

  auto on_segment = Point<double>::from(5.0, 0.0);
  test(approx_eq(h_seg.distance_to_point(on_segment), 0.0),
       "distance = 0 for point on segment");

  // ==================== Containment ====================
  std::cout << "\n--- Containment ---\n";

  test(h_seg.contains(on_segment), "contains() true for point on segment");
  test(!h_seg.contains(test_point), "contains() false for point above segment");

  auto endpoint = Point<double>::from(0.0, 0.0);
  test(h_seg.contains(endpoint), "contains() true for endpoint");
  test(!h_seg.strictly_contains(endpoint),
       "strictly_contains() false for endpoint");
  test(h_seg.strictly_contains(on_segment),
       "strictly_contains() true for interior point");

  // ==================== Intersection ====================
  std::cout << "\n--- Intersection ---\n";

  // Cross pattern
  auto seg1 = LineSegment<double>::from(Point<double>::from(0.0, 5.0),
                                        Point<double>::from(10.0, 5.0));
  auto seg2 = LineSegment<double>::from(Point<double>::from(5.0, 0.0),
                                        Point<double>::from(5.0, 10.0));

  test(seg1.intersects(seg2), "perpendicular segments intersect");

  auto inter = seg1.intersection(seg2);
  test(inter.is_some(), "intersection returns Some");
  test(approx_eq(inter.unwrap().x(), 5.0) && approx_eq(inter.unwrap().y(), 5.0),
       "intersection point is (5,5)");

  // Parallel segments
  auto parallel1 = LineSegment<double>::from(Point<double>::from(0.0, 0.0),
                                             Point<double>::from(10.0, 0.0));
  auto parallel2 = LineSegment<double>::from(Point<double>::from(0.0, 5.0),
                                             Point<double>::from(10.0, 5.0));

  test(!parallel1.intersects(parallel2), "parallel segments don't intersect");
  test(parallel1.intersection(parallel2).is_none(),
       "parallel intersection is None");

  // Non-intersecting (miss)
  auto miss1 = LineSegment<double>::from(Point<double>::from(0.0, 0.0),
                                         Point<double>::from(1.0, 0.0));
  auto miss2 = LineSegment<double>::from(Point<double>::from(0.0, 1.0),
                                         Point<double>::from(1.0, 2.0));
  test(!miss1.intersects(miss2), "non-overlapping segments don't intersect");

  // ==================== Transformations ====================
  std::cout << "\n--- Transformations ---\n";

  auto reversed = seg.reversed();
  test(reversed.start() == seg.end() && reversed.end() == seg.start(),
       "reversed() swaps points");

  auto translated = h_seg.checked_translate(5.0, 5.0);
  test(translated.is_some(), "checked_translate returns Some");
  test(approx_eq(translated.unwrap().start().x(), 5.0), "translated start x");
  test(approx_eq(translated.unwrap().start().y(), 5.0), "translated start y");

  // Extended
  auto extended = h_seg.extended(2.0); // Double the length
  test(approx_eq(extended.length(), 20.0), "extended(2.0) doubles length");

  auto extended_by =
      h_seg.extended_by(2.0, 3.0); // Extend 2 from start, 3 from end
  test(approx_eq(extended_by.length(), 15.0),
       "extended_by(2,3) adds 5 to length");

  // ==================== Bounding Box ====================
  std::cout << "\n--- Bounding Box ---\n";

  auto diag = LineSegment<double>::from(Point<double>::from(5.0, 10.0),
                                        Point<double>::from(-3.0, 2.0));

  test(approx_eq(diag.min_x(), -3.0), "min_x() correct");
  test(approx_eq(diag.max_x(), 5.0), "max_x() correct");
  test(approx_eq(diag.min_y(), 2.0), "min_y() correct");
  test(approx_eq(diag.max_y(), 10.0), "max_y() correct");

  auto [bb_min, bb_max] = diag.bounding_box();
  test(approx_eq(bb_min.x(), -3.0) && approx_eq(bb_min.y(), 2.0),
       "bounding_box min");
  test(approx_eq(bb_max.x(), 5.0) && approx_eq(bb_max.y(), 10.0),
       "bounding_box max");

  // ==================== Comparison ====================
  std::cout << "\n--- Comparison ---\n";

  auto seg_copy = LineSegment<double>::from(p1, p2);
  test(seg == seg_copy, "== works for equal segments");
  test(!(seg == reversed), "== false for reversed segment");
  test(seg.equivalent_to(reversed), "equivalent_to() true for reversed");
  test(seg != reversed, "!= works");

  // ==================== Integer Type ====================
  std::cout << "\n--- Integer Type ---\n";

  auto int_seg =
      LineSegment<int>::from(Point<int>::from(0, 0), Point<int>::from(3, 4));
  test(approx_eq(int_seg.length(), 5.0), "LineSegment<int> length works");

  auto int_mid = int_seg.midpoint();
  test(approx_eq(int_mid.x(), 1.5), "LineSegment<int> midpoint x");

  // ==================== Type Aliases ====================
  std::cout << "\n--- Type Aliases ---\n";

  LineSegmentd ld = LineSegmentd::from(Point<double>::origin(),
                                       Point<double>::from(1.0, 1.0));
  test(approx_eq(ld.length(), std::sqrt(2.0)), "LineSegmentd alias works");

  Line2d l2d =
      Line2d::from(Point<double>::origin(), Point<double>::from(1.0, 0.0));
  test(approx_eq(l2d.length(), 1.0), "Line2d alias works");

  // ==================== Summary ====================
  std::cout << "\n=== Test Summary ===\n";
  std::cout << "Passed: " << passed << "\n";
  std::cout << "Failed: " << failed << "\n";

  return failed > 0 ? 1 : 0;
}
