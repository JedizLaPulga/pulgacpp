// ============================================================================
// pulgacpp Geometry - Comprehensive Integration Tests
// ============================================================================
// This file contains a real-world simulation that exercises ALL geometry types
// and their methods. It simulates a 2D game world with entities, collision
// detection, physics, and spatial algorithms.
//
// Compile: cl /std:c++latest /EHsc /W4 /I../.. game_world_test.cpp
// ============================================================================

#include "../geometry.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <random>
#include <algorithm>

using namespace pulgacpp;

// ============================================================================
// UTILITY: Test Framework
// ============================================================================

namespace test {
    int total_tests = 0;
    int passed_tests = 0;
    int failed_tests = 0;
    
    void check(bool condition, const std::string& test_name) {
        ++total_tests;
        if (condition) {
            ++passed_tests;
            std::cout << "  [PASS] " << test_name << "\n";
        } else {
            ++failed_tests;
            std::cout << "  [FAIL] " << test_name << "\n";
        }
    }
    
    bool approx(double a, double b, double tol = 1e-6) {
        return std::abs(a - b) < tol;
    }
    
    void section(const std::string& name) {
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << " " << name << "\n";
        std::cout << std::string(60, '=') << "\n";
    }
    
    void subsection(const std::string& name) {
        std::cout << "\n--- " << name << " ---\n";
    }
    
    void summary() {
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << " TEST SUMMARY\n";
        std::cout << std::string(60, '=') << "\n";
        std::cout << " Total:  " << total_tests << "\n";
        std::cout << " Passed: " << passed_tests << "\n";
        std::cout << " Failed: " << failed_tests << "\n";
        std::cout << std::string(60, '=') << "\n";
    }
}

// ============================================================================
// PART 1: Point<T> - Comprehensive Tests
// ============================================================================
// Testing: Construction, accessors, arithmetic, distance, geometry, comparison

void test_point_comprehensive() {
    test::section("Point<T> Comprehensive Tests");
    
    // ------------------------------------
    // 1.1 Construction Methods
    // ------------------------------------
    test::subsection("Construction");
    
    // Factory: from(x, y)
    auto p1 = Point<double>::from(3.0, 4.0);
    test::check(p1.x() == 3.0, "Point::from() creates with correct x");
    test::check(p1.y() == 4.0, "Point::from() creates with correct y");
    
    // Factory: origin()
    auto origin = Point<double>::origin();
    test::check(origin.x() == 0.0 && origin.y() == 0.0, "Point::origin() is (0, 0)");
    
    // With different numeric types
    auto p_int = Point<int>::from(10, 20);
    test::check(p_int.x() == 10 && p_int.y() == 20, "Point<int> works correctly");
    
    auto p_float = Point<float>::from(1.5f, 2.5f);
    test::check(p_float.x() == 1.5f, "Point<float> works correctly");
    
    // ------------------------------------
    // 1.2 Immutable Accessors (with_x, with_y)
    // ------------------------------------
    test::subsection("Immutable Modification");
    
    auto original = Point<double>::from(5.0, 10.0);
    auto modified_x = original.with_x(100.0);
    auto modified_y = original.with_y(200.0);
    
    // Original unchanged (immutability)
    test::check(original.x() == 5.0, "with_x() doesn't modify original");
    test::check(original.y() == 10.0, "with_y() doesn't modify original");
    
    // New points have changes
    test::check(modified_x.x() == 100.0 && modified_x.y() == 10.0, "with_x() creates correct new point");
    test::check(modified_y.x() == 5.0 && modified_y.y() == 200.0, "with_y() creates correct new point");
    
    // ------------------------------------
    // 1.3 Checked Arithmetic (translate, scale)
    // ------------------------------------
    test::subsection("Checked Arithmetic");
    
    auto base = Point<double>::from(10.0, 20.0);
    
    // checked_translate success
    auto translated = base.checked_translate(5.0, -10.0);
    test::check(translated.is_some(), "checked_translate() returns Some on valid input");
    test::check(translated.unwrap().x() == 15.0, "checked_translate() correct x");
    test::check(translated.unwrap().y() == 10.0, "checked_translate() correct y");
    
    // checked_scale success
    auto scaled = base.checked_scale(2.0);
    test::check(scaled.is_some(), "checked_scale() returns Some on valid input");
    test::check(scaled.unwrap().x() == 20.0, "checked_scale() correct x");
    test::check(scaled.unwrap().y() == 40.0, "checked_scale() correct y");
    
    // checked_add (point addition)
    auto delta = Point<double>::from(1.0, 2.0);
    auto added = base.checked_add(delta);
    test::check(added.is_some(), "checked_add() returns Some");
    test::check(added.unwrap().x() == 11.0 && added.unwrap().y() == 22.0, "checked_add() correct result");
    
    // checked_sub (point subtraction)
    auto subtracted = base.checked_sub(delta);
    test::check(subtracted.is_some(), "checked_sub() returns Some");
    test::check(subtracted.unwrap().x() == 9.0 && subtracted.unwrap().y() == 18.0, "checked_sub() correct result");
    
    // Integer overflow detection with native int
    auto int_max_pt = Point<int>::from(2147483640, 100);
    auto overflow_result = int_max_pt.checked_translate(100, 0);
    test::check(overflow_result.is_none(), "checked_translate() detects integer overflow");
    
    // ------------------------------------
    // 1.4 Distance Calculations
    // ------------------------------------
    test::subsection("Distance Calculations");
    
    auto a = Point<double>::from(0.0, 0.0);
    auto b = Point<double>::from(3.0, 4.0);  // Classic 3-4-5 right triangle
    
    // Euclidean distance
    test::check(test::approx(a.distance_to(b), 5.0), "distance_to() is 5 for (0,0) to (3,4)");
    test::check(test::approx(b.distance_to(a), 5.0), "distance_to() is symmetric");
    test::check(test::approx(a.distance_to(a), 0.0), "distance_to() self is 0");
    
    // Squared distance (faster)
    test::check(test::approx(a.distance_squared(b), 25.0), "distance_squared() is 25");
    
    // Manhattan distance
    auto manhattan = a.manhattan_distance(b);
    test::check(manhattan == 7, "manhattan_distance() is 3+4=7");
    
    // Magnitude (distance from origin)
    test::check(test::approx(b.magnitude(), 5.0), "magnitude() is 5 for (3,4)");
    test::check(test::approx(origin.magnitude(), 0.0), "magnitude() of origin is 0");
    
    // ------------------------------------
    // 1.5 Geometric Operations
    // ------------------------------------
    test::subsection("Geometric Operations");
    
    auto g1 = Point<double>::from(0.0, 0.0);
    auto g2 = Point<double>::from(10.0, 10.0);
    
    // Midpoint
    auto mid = g1.midpoint(g2);
    test::check(test::approx(mid.x(), 5.0) && test::approx(mid.y(), 5.0), "midpoint() is (5, 5)");
    
    // Linear interpolation (lerp)
    auto lerp_0 = g1.lerp(g2, 0.0);   // Should be g1
    auto lerp_1 = g1.lerp(g2, 1.0);   // Should be g2
    auto lerp_half = g1.lerp(g2, 0.5); // Should be midpoint
    
    test::check(test::approx(lerp_0.x(), 0.0) && test::approx(lerp_0.y(), 0.0), "lerp(0) is start point");
    test::check(test::approx(lerp_1.x(), 10.0) && test::approx(lerp_1.y(), 10.0), "lerp(1) is end point");
    test::check(test::approx(lerp_half.x(), 5.0) && test::approx(lerp_half.y(), 5.0), "lerp(0.5) is midpoint");
    
    // Rotation around origin
    auto to_rotate = Point<double>::from(1.0, 0.0);
    auto rotated_90 = to_rotate.rotate(constants::PI / 2.0);  // 90 degrees
    test::check(test::approx(rotated_90.x(), 0.0) && test::approx(rotated_90.y(), 1.0), "rotate(90°) from (1,0) is (0,1)");
    
    auto rotated_180 = to_rotate.rotate(constants::PI);  // 180 degrees
    test::check(test::approx(rotated_180.x(), -1.0) && test::approx(rotated_180.y(), 0.0, 1e-6), "rotate(180°) from (1,0) is (-1,0)");
    
    // Rotation around a center point
    auto center = Point<double>::from(5.0, 5.0);
    auto point_on_circle = Point<double>::from(10.0, 5.0);  // 5 units right of center
    auto rotated_around = point_on_circle.rotate_around(center, constants::PI / 2.0);
    test::check(test::approx(rotated_around.x(), 5.0) && test::approx(rotated_around.y(), 10.0), 
                "rotate_around() maintains distance from center");
    
    // ------------------------------------
    // 1.6 Comparison Operators
    // ------------------------------------
    test::subsection("Comparison Operators");
    
    auto eq1 = Point<double>::from(1.0, 2.0);
    auto eq2 = Point<double>::from(1.0, 2.0);
    auto neq = Point<double>::from(1.0, 3.0);
    
    test::check(eq1 == eq2, "operator== returns true for equal points");
    test::check(!(eq1 == neq), "operator== returns false for different points");
    test::check(eq1 != neq, "operator!= returns true for different points");
    test::check(!(eq1 != eq2), "operator!= returns false for equal points");
}

// ============================================================================
// PART 2: Vector2<T> - Comprehensive Tests
// ============================================================================
// Testing: Construction, magnitude, normalization, operations, arithmetic

void test_vector2_comprehensive() {
    test::section("Vector2<T> Comprehensive Tests");
    
    // ------------------------------------
    // 2.1 Construction Methods
    // ------------------------------------
    test::subsection("Construction");
    
    auto v1 = Vector2<double>::from(3.0, 4.0);
    test::check(v1.x() == 3.0, "Vector2::from() correct x");
    test::check(v1.y() == 4.0, "Vector2::from() correct y");
    
    auto zero = Vector2<double>::zero();
    test::check(zero.x() == 0.0 && zero.y() == 0.0, "Vector2::zero() is (0, 0)");
    
    // From angle (free function)
    auto unit_right = vec_from_angle(0.0);  // 0 radians = pointing right
    test::check(test::approx(unit_right.x(), 1.0) && test::approx(unit_right.y(), 0.0), 
                "vec_from_angle(0) is (1, 0)");
    
    auto unit_up = vec_from_angle(constants::PI / 2.0);  // 90 degrees = pointing up
    test::check(test::approx(unit_up.x(), 0.0) && test::approx(unit_up.y(), 1.0), 
                "vec_from_angle(π/2) is (0, 1)");
    
    // From angle with custom magnitude
    auto double_right = vec_from_angle(0.0, 2.0);
    test::check(test::approx(double_right.x(), 2.0), "vec_from_angle with magnitude 2");
    
    // Vector from points (free function)
    auto start = Point<double>::from(1.0, 2.0);
    auto end = Point<double>::from(4.0, 6.0);
    auto from_pts = vector_from_points(start, end);
    test::check(test::approx(from_pts.x(), 3.0) && test::approx(from_pts.y(), 4.0), 
                "vector_from_points() calculates difference correctly");
    
    // With different types
    auto v_int = Vector2<int>::from(5, 12);
    test::check(v_int.x() == 5 && v_int.y() == 12, "Vector2<int> works");
    
    // ------------------------------------
    // 2.2 Magnitude Operations
    // ------------------------------------
    test::subsection("Magnitude");
    
    auto v_345 = Vector2<double>::from(3.0, 4.0);  // 3-4-5 triangle
    
    test::check(test::approx(v_345.magnitude(), 5.0), "magnitude() is 5 for (3, 4)");
    test::check(test::approx(v_345.magnitude_squared(), 25.0), "magnitude_squared() is 25");
    
    test::check(!v_345.is_zero(), "is_zero() false for non-zero vector");
    test::check(zero.is_zero(), "is_zero() true for zero vector");
    
    // ------------------------------------
    // 2.3 Normalization (Free Functions)
    // ------------------------------------
    test::subsection("Normalization");
    
    auto normalized = vec_normalized(v_345);
    test::check(normalized.is_some(), "vec_normalized() returns Some for non-zero vector");
    test::check(test::approx(normalized.unwrap().magnitude(), 1.0), "normalized vector has magnitude 1");
    test::check(test::approx(normalized.unwrap().x(), 0.6), "normalized (3,4) has x=0.6");
    test::check(test::approx(normalized.unwrap().y(), 0.8), "normalized (3,4) has y=0.8");
    
    // Cannot normalize zero vector
    auto zero_norm = vec_normalized(zero);
    test::check(zero_norm.is_none(), "vec_normalized() returns None for zero vector");
    
    // ------------------------------------
    // 2.4 Checked Arithmetic
    // ------------------------------------
    test::subsection("Checked Arithmetic");
    
    auto va = Vector2<double>::from(1.0, 2.0);
    auto vb = Vector2<double>::from(3.0, 4.0);
    
    // Addition
    auto added = va.checked_add(vb);
    test::check(added.is_some(), "checked_add() returns Some");
    test::check(added.unwrap().x() == 4.0 && added.unwrap().y() == 6.0, "checked_add() result correct");
    
    // Subtraction
    auto subbed = vb.checked_sub(va);
    test::check(subbed.is_some(), "checked_sub() returns Some");
    test::check(subbed.unwrap().x() == 2.0 && subbed.unwrap().y() == 2.0, "checked_sub() result correct");
    
    // Scaling
    auto scaled = va.checked_scale(3.0);
    test::check(scaled.is_some(), "checked_scale() returns Some");
    test::check(scaled.unwrap().x() == 3.0 && scaled.unwrap().y() == 6.0, "checked_scale() result correct");
    
    // Integer overflow detection
    auto big_vec = Vector2<int>::from(2000000000, 100);
    auto overflow = big_vec.checked_scale(2);
    test::check(overflow.is_none(), "checked_scale() detects integer overflow");
    
    // ------------------------------------
    // 2.5 Vector Operations
    // ------------------------------------
    test::subsection("Vector Operations");
    
    auto x_axis = Vector2<double>::from(1.0, 0.0);
    auto y_axis = Vector2<double>::from(0.0, 1.0);
    auto diagonal = Vector2<double>::from(1.0, 1.0);
    
    // Dot product
    test::check(test::approx(x_axis.dot(y_axis), 0.0), "dot product of perpendicular vectors is 0");
    test::check(test::approx(x_axis.dot(x_axis), 1.0), "dot product of unit vector with itself is 1");
    test::check(test::approx(diagonal.dot(diagonal), 2.0), "dot product of (1,1) with itself is 2");
    
    // Cross product (z-component of 3D cross)
    test::check(test::approx(x_axis.cross(y_axis), 1.0), "cross(x, y) = 1 (right-hand rule)");
    test::check(test::approx(y_axis.cross(x_axis), -1.0), "cross(y, x) = -1 (opposite direction)");
    test::check(test::approx(x_axis.cross(x_axis), 0.0), "cross of parallel vectors is 0");
    
    // Angle from x-axis
    test::check(test::approx(x_axis.angle(), 0.0), "angle of (1,0) is 0");
    test::check(test::approx(y_axis.angle(), constants::PI / 2.0), "angle of (0,1) is π/2");
    
    auto neg_x = Vector2<double>::from(-1.0, 0.0);
    test::check(test::approx(neg_x.angle(), constants::PI), "angle of (-1,0) is π");
    
    // ------------------------------------
    // 2.6 Transformations (Free Functions)
    // ------------------------------------
    test::subsection("Transformations");
    
    // Perpendicular
    auto perp = vec_perpendicular(x_axis);
    test::check(test::approx(perp.x(), 0.0) && test::approx(perp.y(), 1.0), 
                "vec_perpendicular(1,0) is (0,1)");
    
    // Negation
    auto negated = vec_negated(diagonal);
    test::check(test::approx(negated.x(), -1.0) && test::approx(negated.y(), -1.0), 
                "vec_negated() flips signs");
    
    // Rotation
    auto rotated = vec_rotated(x_axis, constants::PI / 2.0);
    test::check(test::approx(rotated.x(), 0.0) && test::approx(rotated.y(), 1.0), 
                "vec_rotated(x_axis, 90°) is y_axis");
    
    // ------------------------------------
    // 2.7 Conversion
    // ------------------------------------
    test::subsection("Conversion");
    
    auto vec = Vector2<double>::from(7.0, 8.0);
    auto as_point = vec.to_point();
    test::check(as_point.x() == 7.0 && as_point.y() == 8.0, "to_point() converts correctly");
    
    // ------------------------------------
    // 2.8 Comparison
    // ------------------------------------
    test::subsection("Comparison");
    
    auto veq1 = Vector2<double>::from(1.5, 2.5);
    auto veq2 = Vector2<double>::from(1.5, 2.5);
    auto vneq = Vector2<double>::from(1.5, 3.5);
    
    test::check(veq1 == veq2, "Vector2 == works for equal vectors");
    test::check(veq1 != vneq, "Vector2 != works for different vectors");
}

// ============================================================================
// PART 3: Circle<T> - Comprehensive Tests
// ============================================================================

void test_circle_comprehensive() {
    test::section("Circle<T> Comprehensive Tests");
    
    // ------------------------------------
    // 3.1 Construction
    // ------------------------------------
    test::subsection("Construction");
    
    auto center = Point<double>::from(5.0, 5.0);
    auto circle_opt = Circle<double>::from(center, 10.0);
    
    test::check(circle_opt.is_some(), "Circle::from() with positive radius returns Some");
    auto circle = circle_opt.unwrap();
    test::check(circle.center() == center, "Circle center is correct");
    test::check(circle.radius() == 10.0, "Circle radius is correct");
    
    // Negative radius should fail
    auto neg_circle = Circle<double>::from(center, -5.0);
    test::check(neg_circle.is_none(), "Circle::from() with negative radius returns None");
    
    // Unit circle
    auto unit = Circle<double>::unit();
    test::check(unit.radius() == 1.0, "Circle::unit() has radius 1");
    
    // Circle with integer type
    auto int_center = Point<int>::from(0, 0);
    auto int_circle = Circle<int>::from(int_center, 5);
    test::check(int_circle.is_some(), "Circle<int> works");
    
    // ------------------------------------
    // 3.2 Geometry (Shape2D Interface)
    // ------------------------------------
    test::subsection("Geometry");
    
    auto r5_circle = Circle<double>::from(Point<double>::origin(), 5.0).unwrap();
    
    // Area = π * r²
    test::check(test::approx(r5_circle.area(), constants::PI * 25.0), "area() = πr² = 78.54...");
    
    // Perimeter (circumference) = 2πr
    test::check(test::approx(r5_circle.perimeter(), constants::TAU * 5.0), "perimeter() = 2πr = 31.42...");
    test::check(test::approx(r5_circle.circumference(), r5_circle.perimeter()), "circumference() = perimeter()");
    
    // Diameter
    test::check(test::approx(r5_circle.diameter(), 10.0), "diameter() = 2r = 10");
    
    // ------------------------------------
    // 3.3 Containment
    // ------------------------------------
    test::subsection("Containment");
    
    auto c = Circle<double>::from(Point<double>::origin(), 10.0).unwrap();
    
    // Center is inside
    test::check(c.contains(Point<double>::origin()), "contains() true for center");
    test::check(c.strictly_contains(Point<double>::origin()), "strictly_contains() true for center");
    
    // Point on boundary
    auto on_boundary = Point<double>::from(10.0, 0.0);
    test::check(c.contains(on_boundary), "contains() true for boundary point");
    test::check(!c.strictly_contains(on_boundary), "strictly_contains() false for boundary");
    test::check(c.on_boundary(on_boundary), "on_boundary() true for boundary point");
    
    // Interior point
    auto interior = Point<double>::from(3.0, 4.0);  // Distance = 5 < 10
    test::check(c.contains(interior), "contains() true for interior point");
    test::check(c.strictly_contains(interior), "strictly_contains() true for interior");
    test::check(!c.on_boundary(interior), "on_boundary() false for interior");
    
    // Exterior point
    auto exterior = Point<double>::from(20.0, 0.0);
    test::check(!c.contains(exterior), "contains() false for exterior point");
    test::check(!c.strictly_contains(exterior), "strictly_contains() false for exterior");
    
    // ------------------------------------
    // 3.4 Intersection & Overlap
    // ------------------------------------
    test::subsection("Intersection & Overlap");
    
    auto c1 = Circle<double>::from(Point<double>::origin(), 5.0).unwrap();
    auto c2 = Circle<double>::from(Point<double>::from(8.0, 0.0), 5.0).unwrap();  // Overlapping
    auto c3 = Circle<double>::from(Point<double>::from(20.0, 0.0), 5.0).unwrap(); // Distant
    auto c4 = Circle<double>::from(Point<double>::from(10.0, 0.0), 5.0).unwrap(); // Touching
    auto c_small = Circle<double>::from(Point<double>::origin(), 2.0).unwrap();   // Inside c1
    
    // Overlapping circles
    test::check(c1.overlaps(c2), "overlaps() true for overlapping circles");
    test::check(c1.intersects(c2), "intersects() true for overlapping circles");
    
    // Touching circles (tangent)
    test::check(c1.intersects(c4), "intersects() true for tangent circles");
    test::check(!c1.overlaps(c4), "overlaps() false for tangent circles (just touching)");
    
    // Distant circles
    test::check(!c1.overlaps(c3), "overlaps() false for distant circles");
    test::check(!c1.intersects(c3), "intersects() false for distant circles");
    
    // One contains the other
    test::check(c1.contains_circle(c_small), "contains_circle() true when one is inside");
    test::check(!c_small.contains_circle(c1), "contains_circle() false for smaller containing larger");
    
    // ------------------------------------
    // 3.5 Transformations
    // ------------------------------------
    test::subsection("Transformations");
    
    auto base_circle = Circle<double>::from(Point<double>::from(5.0, 5.0), 3.0).unwrap();
    
    // checked_translate
    auto moved = base_circle.checked_translate(10.0, -5.0);
    test::check(moved.is_some(), "checked_translate() returns Some");
    test::check(moved.unwrap().center().x() == 15.0, "checked_translate() x correct");
    test::check(moved.unwrap().center().y() == 0.0, "checked_translate() y correct");
    test::check(moved.unwrap().radius() == 3.0, "checked_translate() preserves radius");
    
    // checked_scale
    auto scaled = base_circle.checked_scale(2.0);
    test::check(scaled.is_some(), "checked_scale() returns Some");
    test::check(scaled.unwrap().radius() == 6.0, "checked_scale() doubles radius");
    test::check(scaled.unwrap().center() == base_circle.center(), "checked_scale() preserves center");
    
    // Negative scale fails
    auto neg_scale = base_circle.checked_scale(-1.0);
    test::check(neg_scale.is_none(), "checked_scale() with negative factor returns None");
    
    // with_center / with_radius
    auto new_center = Point<double>::from(100.0, 100.0);
    auto relocated = base_circle.with_center(new_center);
    test::check(relocated.center() == new_center, "with_center() sets new center");
    test::check(relocated.radius() == 3.0, "with_center() preserves radius");
    
    auto resized = base_circle.with_radius(50.0);
    test::check(resized.is_some(), "with_radius() with positive value returns Some");
    test::check(resized.unwrap().radius() == 50.0, "with_radius() sets new radius");
    
    auto neg_radius = base_circle.with_radius(-10.0);
    test::check(neg_radius.is_none(), "with_radius() with negative returns None");
    
    // ------------------------------------
    // 3.6 Special Points
    // ------------------------------------
    test::subsection("Special Points");
    
    auto centered = Circle<double>::from(Point<double>::origin(), 5.0).unwrap();
    
    // point_at_angle
    auto right = centered.point_at_angle(0.0);
    test::check(test::approx(right.x(), 5.0) && test::approx(right.y(), 0.0), "point_at_angle(0) is right");
    
    auto top = centered.point_at_angle(constants::PI / 2.0);
    test::check(test::approx(top.x(), 0.0) && test::approx(top.y(), 5.0), "point_at_angle(π/2) is top");
    
    // Cardinal points
    auto t = centered.top();
    auto b = centered.bottom();
    auto l = centered.left();
    auto rr = centered.right();
    
    test::check(test::approx(t.y(), 5.0), "top() y = radius");
    test::check(test::approx(b.y(), -5.0), "bottom() y = -radius");
    test::check(test::approx(l.x(), -5.0), "left() x = -radius");
    test::check(test::approx(rr.x(), 5.0), "right() x = radius");
    
    // ------------------------------------
    // 3.7 Circle from 3 Points (Free Function)
    // ------------------------------------
    test::subsection("Circle from 3 Points");
    
    // Three points on a circle of radius 5 centered at origin
    auto p1 = Point<double>::from(5.0, 0.0);
    auto p2 = Point<double>::from(0.0, 5.0);
    auto p3 = Point<double>::from(-5.0, 0.0);
    
    auto from3 = circle_from_points(p1, p2, p3);
    test::check(from3.is_some(), "circle_from_points() returns Some for valid triangle");
    test::check(test::approx(from3.unwrap().radius(), 5.0), "circle_from_points() calculates correct radius");
    test::check(test::approx(from3.unwrap().center().x(), 0.0), "circle_from_points() center x correct");
    test::check(test::approx(from3.unwrap().center().y(), 0.0), "circle_from_points() center y correct");
    
    // Collinear points should fail
    auto col1 = Point<double>::from(0.0, 0.0);
    auto col2 = Point<double>::from(1.0, 1.0);
    auto col3 = Point<double>::from(2.0, 2.0);
    auto collinear = circle_from_points(col1, col2, col3);
    test::check(collinear.is_none(), "circle_from_points() returns None for collinear points");
}

// ============================================================================
// PART 4: Rectangle<T> - Comprehensive Tests
// ============================================================================

void test_rectangle_comprehensive() {
    test::section("Rectangle<T> Comprehensive Tests");
    
    // ------------------------------------
    // 4.1 Construction
    // ------------------------------------
    test::subsection("Construction");
    
    // from_corner
    auto min_corner = Point<double>::from(10.0, 20.0);
    auto rect_opt = Rectangle<double>::from_corner(min_corner, 30.0, 40.0);
    test::check(rect_opt.is_some(), "Rectangle::from_corner() with positive dims returns Some");
    
    auto rect = rect_opt.unwrap();
    test::check(rect.width() == 30.0, "Rectangle width is correct");
    test::check(rect.height() == 40.0, "Rectangle height is correct");
    test::check(rect.min_corner() == min_corner, "Rectangle min_corner is correct");
    
    // Negative dimensions fail
    auto neg_width = Rectangle<double>::from_corner(min_corner, -10.0, 40.0);
    test::check(neg_width.is_none(), "from_corner() with negative width returns None");
    
    auto neg_height = Rectangle<double>::from_corner(min_corner, 10.0, -40.0);
    test::check(neg_height.is_none(), "from_corner() with negative height returns None");
    
    // from_corners (auto-normalizes)
    auto p1 = Point<double>::from(50.0, 60.0);  // top-right
    auto p2 = Point<double>::from(10.0, 20.0);  // bottom-left
    auto from_corners = Rectangle<double>::from_corners(p1, p2);
    test::check(from_corners.width() == 40.0, "from_corners() calculates width correctly");
    test::check(from_corners.height() == 40.0, "from_corners() calculates height correctly");
    test::check(from_corners.min_corner().x() == 10.0, "from_corners() finds min x");
    test::check(from_corners.min_corner().y() == 20.0, "from_corners() finds min y");
    
    // Unit rectangle
    auto unit = Rectangle<double>::unit();
    test::check(unit.width() == 1.0 && unit.height() == 1.0, "Rectangle::unit() is 1x1");
    
    // Integer rectangle
    auto int_rect = Rectangle<int>::from_corner(Point<int>::from(0, 0), 100, 200);
    test::check(int_rect.is_some(), "Rectangle<int> works");
    
    // ------------------------------------
    // 4.2 Accessors
    // ------------------------------------
    test::subsection("Accessors");
    
    auto r = Rectangle<double>::from_corner(Point<double>::from(10.0, 20.0), 30.0, 40.0).unwrap();
    
    // Edges
    test::check(r.left() == 10.0, "left() returns min x");
    test::check(test::approx(r.right(), 40.0), "right() returns max x");
    test::check(r.bottom() == 20.0, "bottom() returns min y");
    test::check(test::approx(r.top(), 60.0), "top() returns max y");
    
    // Corners
    auto min_c = r.min_corner();
    auto max_c = r.max_corner();
    test::check(min_c.x() == 10.0 && min_c.y() == 20.0, "min_corner() correct");
    test::check(test::approx(max_c.x(), 40.0) && test::approx(max_c.y(), 60.0), "max_corner() correct");
    
    // Center
    auto ctr = r.center();
    test::check(test::approx(ctr.x(), 25.0), "center() x is midpoint");
    test::check(test::approx(ctr.y(), 40.0), "center() y is midpoint");
    
    // All four corners
    auto corners = r.corners();
    test::check(corners.size() == 4, "corners() returns 4 points");
    test::check(test::approx(corners[0].x(), 10.0) && test::approx(corners[0].y(), 20.0), "corners[0] is bottom-left");
    test::check(test::approx(corners[2].x(), 40.0) && test::approx(corners[2].y(), 60.0), "corners[2] is top-right");
    
    // ------------------------------------
    // 4.3 Geometry (Shape2D Interface)
    // ------------------------------------
    test::subsection("Geometry");
    
    auto r10x20 = Rectangle<double>::from_corner(Point<double>::origin(), 10.0, 20.0).unwrap();
    
    // Area
    test::check(test::approx(r10x20.area(), 200.0), "area() = width * height = 200");
    
    // Perimeter
    test::check(test::approx(r10x20.perimeter(), 60.0), "perimeter() = 2*(w+h) = 60");
    
    // Diagonal
    test::check(test::approx(r10x20.diagonal(), std::sqrt(500.0)), "diagonal() = √(w²+h²)");
    
    // Aspect ratio
    auto aspect = r10x20.aspect_ratio();
    test::check(aspect.is_some(), "aspect_ratio() returns Some for non-zero height");
    test::check(test::approx(aspect.unwrap(), 0.5), "aspect_ratio() = w/h = 0.5");
    
    // Zero height aspect ratio
    auto zero_h = Rectangle<double>::from_corner(Point<double>::origin(), 10.0, 0.0).unwrap();
    test::check(zero_h.aspect_ratio().is_none(), "aspect_ratio() returns None for zero height");
    
    // is_square
    auto square = Rectangle<double>::from_corner(Point<double>::origin(), 15.0, 15.0).unwrap();
    test::check(square.is_square(), "is_square() true for equal dimensions");
    test::check(!r10x20.is_square(), "is_square() false for non-square");
    
    // ------------------------------------
    // 4.4 Containment
    // ------------------------------------
    test::subsection("Containment");
    
    auto container = Rectangle<double>::from_corner(Point<double>::origin(), 100.0, 100.0).unwrap();
    
    // Interior point
    auto inside = Point<double>::from(50.0, 50.0);
    test::check(container.contains(inside), "contains() true for interior");
    test::check(container.strictly_contains(inside), "strictly_contains() true for interior");
    
    // Corner point
    auto corner = Point<double>::origin();
    test::check(container.contains(corner), "contains() true for corner");
    test::check(!container.strictly_contains(corner), "strictly_contains() false for corner");
    
    // Edge point
    auto edge = Point<double>::from(50.0, 0.0);
    test::check(container.contains(edge), "contains() true for edge");
    test::check(!container.strictly_contains(edge), "strictly_contains() false for edge");
    
    // Outside
    auto outside = Point<double>::from(150.0, 50.0);
    test::check(!container.contains(outside), "contains() false for outside");
    
    // ------------------------------------
    // 4.5 Intersection
    // ------------------------------------
    test::subsection("Intersection");
    
    auto r1 = Rectangle<double>::from_corner(Point<double>::origin(), 100.0, 100.0).unwrap();
    auto r2 = Rectangle<double>::from_corner(Point<double>::from(50.0, 50.0), 100.0, 100.0).unwrap();  // Overlapping
    auto r3 = Rectangle<double>::from_corner(Point<double>::from(200.0, 0.0), 50.0, 50.0).unwrap();   // Distant
    auto r_small = Rectangle<double>::from_corner(Point<double>::from(25.0, 25.0), 50.0, 50.0).unwrap(); // Inside r1
    
    // Overlapping
    test::check(r1.intersects(r2), "intersects() true for overlapping rectangles");
    
    // Get intersection (free function)
    auto intersection = rect_intersection(r1, r2);
    test::check(intersection.is_some(), "rect_intersection() returns Some for overlapping");
    auto inter_rect = intersection.unwrap();
    test::check(test::approx(inter_rect.width(), 50.0), "intersection width correct");
    test::check(test::approx(inter_rect.height(), 50.0), "intersection height correct");
    
    // Distant rectangles
    test::check(!r1.intersects(r3), "intersects() false for distant rectangles");
    auto no_intersect = rect_intersection(r1, r3);
    test::check(no_intersect.is_none(), "rect_intersection() returns None for distant");
    
    // Contains rect
    test::check(r1.contains_rect(r_small), "contains_rect() true when one is inside");
    test::check(!r_small.contains_rect(r1), "contains_rect() false for smaller containing larger");
    
    // ------------------------------------
    // 4.6 Transformations
    // ------------------------------------
    test::subsection("Transformations");
    
    auto base = Rectangle<double>::from_corner(Point<double>::from(10.0, 10.0), 20.0, 30.0).unwrap();
    
    // checked_translate
    auto moved = base.checked_translate(5.0, -5.0);
    test::check(moved.is_some(), "checked_translate() returns Some");
    test::check(moved.unwrap().min_corner().x() == 15.0, "checked_translate() x correct");
    test::check(moved.unwrap().width() == 20.0, "checked_translate() preserves width");
    
    // checked_scale
    auto scaled = base.checked_scale(2.0);
    test::check(scaled.is_some(), "checked_scale() returns Some");
    test::check(scaled.unwrap().width() == 40.0, "checked_scale() doubles width");
    test::check(scaled.unwrap().height() == 60.0, "checked_scale() doubles height");
    
    // Negative scale fails
    auto neg_scale = base.checked_scale(-1.0);
    test::check(neg_scale.is_none(), "checked_scale() with negative returns None");
    
    // rect_expanded (free function)
    auto expanded = rect_expanded(base, 5.0);
    test::check(expanded.is_some(), "rect_expanded() returns Some");
    test::check(test::approx(expanded.unwrap().width(), 30.0), "rect_expanded() increases width by 10");
    test::check(test::approx(expanded.unwrap().height(), 40.0), "rect_expanded() increases height by 10");
    
    // Shrinking too much
    auto over_shrunk = rect_expanded(base, -15.0);
    test::check(over_shrunk.is_none(), "rect_expanded() with excessive shrink returns None");
    
    // rect_from_center (free function)
    auto from_center = rect_from_center(Point<double>::from(50.0, 50.0), 20.0, 30.0);
    test::check(from_center.is_some(), "rect_from_center() returns Some");
    test::check(test::approx(from_center.unwrap().center().x(), 50.0), "rect_from_center() center x correct");
}

// ============================================================================
// PART 5: Shape Traits & CRTP Tests
// ============================================================================

void test_shape_traits() {
    test::section("Shape Traits & CRTP");
    
    // ------------------------------------
    // 5.1 Static Properties
    // ------------------------------------
    test::subsection("Static Properties");
    
    // NAME
    test::check(Point<double>::NAME == "Point", "Point::NAME is 'Point'");
    test::check(Vector2<double>::NAME == "Vector2", "Vector2::NAME is 'Vector2'");
    test::check(Circle<double>::NAME == "Circle", "Circle::NAME is 'Circle'");
    test::check(Rectangle<double>::NAME == "Rectangle", "Rectangle::NAME is 'Rectangle'");
    
    // DIMENSIONS
    test::check(Point<double>::DIMENSIONS == 2, "Point::DIMENSIONS is 2");
    test::check(Vector2<double>::DIMENSIONS == 2, "Vector2::DIMENSIONS is 2");
    test::check(Circle<double>::DIMENSIONS == 2, "Circle::DIMENSIONS is 2");
    test::check(Rectangle<double>::DIMENSIONS == 2, "Rectangle::DIMENSIONS is 2");
    
    // is_shape
    test::check(Point<double>::is_shape == true, "Point::is_shape is true");
    test::check(Circle<double>::is_shape == true, "Circle::is_shape is true");
    test::check(Rectangle<double>::is_shape == true, "Rectangle::is_shape is true");
    
    // ------------------------------------
    // 5.2 CRTP Methods
    // ------------------------------------
    test::subsection("CRTP Methods");
    
    // name() via CRTP
    test::check(Circle<double>::name() == "Circle", "Circle::name() returns 'Circle' via CRTP");
    test::check(Rectangle<double>::name() == "Rectangle", "Rectangle::name() returns 'Rectangle' via CRTP");
    
    // dimensions() via CRTP
    test::check(Circle<double>::dimensions() == 2, "Circle::dimensions() returns 2 via CRTP");
    test::check(Rectangle<double>::dimensions() == 2, "Rectangle::dimensions() returns 2 via CRTP");
    
    // area() and perimeter() via Shape2D
    auto circle = Circle<double>::from(Point<double>::origin(), 10.0).unwrap();
    auto rect = Rectangle<double>::from_corner(Point<double>::origin(), 10.0, 5.0).unwrap();
    
    test::check(circle.area() > 0, "Circle area() callable via CRTP");
    test::check(circle.perimeter() > 0, "Circle perimeter() callable via CRTP");
    test::check(rect.area() > 0, "Rectangle area() callable via CRTP");
    test::check(rect.perimeter() > 0, "Rectangle perimeter() callable via CRTP");
}

// ============================================================================
// PART 6: Real-World Simulation - 2D Game Physics
// ============================================================================
// This section simulates a simple 2D game world with collision detection

void test_game_world_simulation() {
    test::section("Real-World Simulation: 2D Game World");
    
    // ------------------------------------
    // 6.1 Entity System with Positions
    // ------------------------------------
    test::subsection("Entity Positioning");
    
    // Define game entities as circles (players, enemies, bullets)
    struct Entity {
        std::string name;
        Circle<double> bounds;
        Vector2<double> velocity;
    };
    
    std::vector<Entity> entities;
    
    // Create player at center
    auto player_pos = Point<double>::from(400.0, 300.0);
    auto player = Entity{
        "Player",
        Circle<double>::from(player_pos, 20.0).unwrap(),
        Vector2<double>::zero()
    };
    entities.push_back(player);
    
    // Create enemies around the player
    auto enemy1 = Entity{
        "Enemy1",
        Circle<double>::from(Point<double>::from(200.0, 300.0), 15.0).unwrap(),
        Vector2<double>::from(2.0, 0.0)
    };
    entities.push_back(enemy1);
    
    auto enemy2 = Entity{
        "Enemy2",
        Circle<double>::from(Point<double>::from(600.0, 300.0), 15.0).unwrap(),
        Vector2<double>::from(-2.0, 0.0)
    };
    entities.push_back(enemy2);
    
    test::check(entities.size() == 3, "Created 3 game entities");
    
    // ------------------------------------
    // 6.2 Movement & Collision Detection
    // ------------------------------------
    test::subsection("Movement & Collision");
    
    // Simulate movement
    auto move_entity = [](Entity& e, double dt) {
        auto dx = e.velocity.x() * dt;
        auto dy = e.velocity.y() * dt;
        auto new_bounds = e.bounds.checked_translate(dx, dy);
        if (new_bounds.is_some()) {
            e.bounds = new_bounds.unwrap();
        }
    };
    
    // Move enemy1 towards player
    double dt = 1.0;  // 1 second
    auto initial_pos = entities[1].bounds.center().x();
    move_entity(entities[1], dt);
    auto new_pos = entities[1].bounds.center().x();
    
    test::check(new_pos > initial_pos, "Entity movement increases x position");
    test::check(test::approx(new_pos - initial_pos, 2.0), "Entity moved by velocity * dt");
    
    // Check collision between entities
    auto check_collision = [](const Entity& a, const Entity& b) {
        return a.bounds.overlaps(b.bounds);
    };
    
    bool player_enemy1_collision = check_collision(entities[0], entities[1]);
    test::check(!player_enemy1_collision, "No collision between distant entities");
    
    // Move enemy close to player
    entities[1].bounds = Circle<double>::from(
        Point<double>::from(410.0, 300.0), 15.0
    ).unwrap();
    
    player_enemy1_collision = check_collision(entities[0], entities[1]);
    test::check(player_enemy1_collision, "Collision detected for overlapping entities");
    
    // ------------------------------------
    // 6.3 World Boundaries (Rectangle)
    // ------------------------------------
    test::subsection("World Boundaries");
    
    auto world_bounds = Rectangle<double>::from_corner(
        Point<double>::origin(), 800.0, 600.0
    ).unwrap();
    
    // Check if entities are within world
    auto in_world = [&world_bounds](const Entity& e) {
        return world_bounds.contains(e.bounds.center());
    };
    
    test::check(in_world(entities[0]), "Player is within world bounds");
    test::check(in_world(entities[1]), "Enemy1 is within world bounds");
    
    // Entity outside world
    auto out_of_bounds = Entity{
        "OutOfBounds",
        Circle<double>::from(Point<double>::from(1000.0, 500.0), 10.0).unwrap(),
        Vector2<double>::zero()
    };
    
    test::check(!in_world(out_of_bounds), "Entity outside world detected");
    
    // ------------------------------------
    // 6.4 Spatial Queries
    // ------------------------------------
    test::subsection("Spatial Queries");
    
    // Find all entities within a search radius
    auto search_center = player.bounds.center();
    auto search_radius = 50.0;
    auto search_area = Circle<double>::from(search_center, search_radius).unwrap();
    
    std::vector<std::string> nearby;
    for (const auto& e : entities) {
        if (e.name != "Player" && search_area.overlaps(e.bounds)) {
            nearby.push_back(e.name);
        }
    }
    
    test::check(nearby.size() == 1, "Found 1 enemy in search radius");
    test::check(nearby[0] == "Enemy1", "Nearby enemy is Enemy1");
    
    // Distance-based sorting
    auto distance_to_player = [&player](const Entity& e) {
        return player.bounds.center().distance_to(e.bounds.center());
    };
    
    std::vector<Entity> sorted_enemies = {entities[1], entities[2]};
    std::sort(sorted_enemies.begin(), sorted_enemies.end(), 
        [&](const Entity& a, const Entity& b) {
            return distance_to_player(a) < distance_to_player(b);
        });
    
    test::check(sorted_enemies[0].name == "Enemy1", "Sorted by distance: closest first");
    
    // ------------------------------------
    // 6.5 Vector Operations for AI
    // ------------------------------------
    test::subsection("AI Vector Math");
    
    // Calculate direction from enemy to player
    auto enemy_pos = entities[2].bounds.center();  // Enemy2
    auto player_target = player.bounds.center();
    
    auto direction_vec = vector_from_points(enemy_pos, player_target);
    auto normalized = vec_normalized(direction_vec);
    
    test::check(normalized.is_some(), "Direction vector normalized successfully");
    test::check(test::approx(normalized.unwrap().magnitude(), 1.0), "Normalized direction has magnitude 1");
    
    // Calculate angle for enemy rotation
    auto angle_to_player = direction_vec.angle();
    test::check(angle_to_player != 0.0, "Angle to player calculated");
    
    // Predict interception point (simple linear prediction)
    auto player_velocity = Vector2<double>::from(1.0, 0.5);
    double prediction_time = 2.0;
    
    auto future_x = player_target.x() + player_velocity.x() * prediction_time;
    auto future_y = player_target.y() + player_velocity.y() * prediction_time;
    auto predicted_position = Point<double>::from(future_x, future_y);
    
    test::check(predicted_position.x() > player_target.x(), "Predicted position is ahead of current");
    
    // ------------------------------------
    // 6.6 Rectangle-based UI/HUD
    // ------------------------------------
    test::subsection("UI Rectangles");
    
    // Health bar
    auto health_bar_bg = Rectangle<double>::from_corner(
        Point<double>::from(10.0, 560.0), 200.0, 30.0
    ).unwrap();
    
    double health_percent = 0.75;  // 75% health
    auto health_bar_fill = Rectangle<double>::from_corner(
        health_bar_bg.min_corner(),
        health_bar_bg.width() * health_percent,
        health_bar_bg.height()
    ).unwrap();
    
    test::check(test::approx(health_bar_fill.width(), 150.0), "Health bar fill at 75%");
    test::check(health_bar_bg.contains_rect(health_bar_fill), "Health fill within background");
    
    // Mini-map (scaled world view)
    double map_scale = 0.1;  // 10% size
    auto minimap = Rectangle<double>::from_corner(
        Point<double>::from(650.0, 450.0),
        world_bounds.width() * map_scale,
        world_bounds.height() * map_scale
    ).unwrap();
    
    test::check(test::approx(minimap.width(), 80.0), "Minimap width is 10% of world");
    test::check(test::approx(minimap.height(), 60.0), "Minimap height is 10% of world");
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║     pulgacpp GEOMETRY - COMPREHENSIVE INTEGRATION TESTS      ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    
    // Run all test sections
    test_point_comprehensive();
    test_vector2_comprehensive();
    test_circle_comprehensive();
    test_rectangle_comprehensive();
    test_shape_traits();
    test_game_world_simulation();
    
    // Final summary
    test::summary();
    
    return test::failed_tests > 0 ? 1 : 0;
}
