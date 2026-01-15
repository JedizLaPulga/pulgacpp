// Test suite for pulgacpp geometry types
// Compile: cl /std:c++latest /EHsc /W4 main.cpp

#include "geometry.hpp"
#include <iostream>
#include <cmath>

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

bool approx_eq(double a, double b, double tol = 1e-9) {
    return std::abs(a - b) < tol;
}

int main() {
    std::cout << "=== pulgacpp Geometry Test Suite ===\n\n";

    // ==================== Point Tests ====================
    std::cout << "--- Point<T> ---\n";
    
    auto p1 = Point<double>::from(3.0, 4.0);
    auto p2 = Point<double>::from(0.0, 0.0);
    
    test(p1.x() == 3.0, "Point x() accessor");
    test(p1.y() == 4.0, "Point y() accessor");
    test(approx_eq(p1.distance_to(p2), 5.0), "Point distance (3-4-5 triangle)");
    test(approx_eq(p1.magnitude(), 5.0), "Point magnitude");
    
    auto mid = p1.midpoint(p2);
    test(approx_eq(mid.x(), 1.5) && approx_eq(mid.y(), 2.0), "Point midpoint");
    
    auto translated = p2.checked_translate(5.0, 10.0);
    test(translated.is_some(), "Point translate returns Some");
    test(translated.unwrap().x() == 5.0, "Point translate x correct");
    
    auto origin = Point<double>::origin();
    test(origin.x() == 0.0 && origin.y() == 0.0, "Point::origin()");

    // Point with integer type
    auto pi = Point<int>::from(10, 20);
    test(pi.x() == 10 && pi.y() == 20, "Point<int> works");

    // ==================== Vector2 Tests ====================
    std::cout << "\n--- Vector2<T> ---\n";
    
    auto v1 = Vector2<double>::from(3.0, 4.0);
    auto v2 = Vector2<double>::from(1.0, 0.0);
    
    test(approx_eq(v1.magnitude(), 5.0), "Vector2 magnitude");
    test(approx_eq(v1.magnitude_squared(), 25.0), "Vector2 magnitude_squared");
    
    auto normalized = v1.normalized();
    test(normalized.is_some(), "Vector2 normalize returns Some");
    test(approx_eq(normalized.unwrap().magnitude(), 1.0), "Normalized vector has magnitude 1");
    
    test(approx_eq(v1.dot(v2), 3.0), "Vector2 dot product");
    
    auto perp = v2.perpendicular();
    test(approx_eq(perp.x(), 0.0) && approx_eq(perp.y(), 1.0), "Vector2 perpendicular");
    
    auto zero = Vector2<double>::zero();
    test(zero.is_zero(), "Vector2::zero() is_zero");
    test(zero.normalized().is_none(), "Cannot normalize zero vector");

    auto angle = v2.angle();
    test(approx_eq(angle, 0.0), "Vector2 angle from x-axis");
    
    auto rotated = v2.rotated(constants::PI / 2.0);  // 90 degrees
    test(approx_eq(rotated.x(), 0.0, 1e-6) && approx_eq(rotated.y(), 1.0, 1e-6), "Vector2 rotate 90°");

    // ==================== Circle Tests ====================
    std::cout << "\n--- Circle<T> ---\n";
    
    auto center = Point<double>::from(0.0, 0.0);
    auto circle = Circle<double>::from(center, 5.0);
    test(circle.is_some(), "Circle::from with positive radius returns Some");
    
    auto c = circle.unwrap();
    test(approx_eq(c.area(), constants::PI * 25.0), "Circle area = π*r²");
    test(approx_eq(c.circumference(), 2.0 * constants::PI * 5.0), "Circle circumference = 2πr");
    test(approx_eq(c.diameter(), 10.0), "Circle diameter = 2r");
    
    test(c.contains(Point<double>::from(3.0, 4.0)), "Circle contains point on boundary");
    test(c.contains(Point<double>::from(0.0, 0.0)), "Circle contains center");
    test(!c.contains(Point<double>::from(6.0, 0.0)), "Circle does not contain distant point");
    test(c.strictly_contains(Point<double>::from(1.0, 1.0)), "Circle strictly contains interior point");
    
    auto neg_radius = Circle<double>::from(center, -5.0);
    test(neg_radius.is_none(), "Circle with negative radius returns None");

    // Circle intersection
    auto c2 = Circle<double>::from(Point<double>::from(8.0, 0.0), 5.0).unwrap();
    test(c.intersects(c2), "Circles with touching boundaries intersect");
    
    auto c3 = Circle<double>::from(Point<double>::from(20.0, 0.0), 5.0).unwrap();
    test(!c.overlaps(c3), "Distant circles do not overlap");

    // Circle from three points
    auto circle_3pt = Circle<double>::from_points(
        Point<double>::from(0.0, 5.0),
        Point<double>::from(5.0, 0.0),
        Point<double>::from(-5.0, 0.0)
    );
    test(circle_3pt.is_some(), "Circle from 3 points returns Some");
    test(approx_eq(circle_3pt.unwrap().radius(), 5.0, 1e-6), "Circle from 3 points has correct radius");

    // ==================== Rectangle Tests ====================
    std::cout << "\n--- Rectangle<T> ---\n";
    
    auto rect = Rectangle<double>::from_corner(Point<double>::from(0.0, 0.0), 10.0, 5.0);
    test(rect.is_some(), "Rectangle::from_corner returns Some");
    
    auto r = rect.unwrap();
    test(approx_eq(r.area(), 50.0), "Rectangle area = w*h");
    test(approx_eq(r.perimeter(), 30.0), "Rectangle perimeter = 2*(w+h)");
    test(r.width() == 10.0 && r.height() == 5.0, "Rectangle dimensions");
    
    auto rect_center = r.center();
    test(approx_eq(rect_center.x(), 5.0) && approx_eq(rect_center.y(), 2.5), "Rectangle center");
    
    test(r.contains(Point<double>::from(5.0, 2.5)), "Rectangle contains center point");
    test(r.contains(Point<double>::from(0.0, 0.0)), "Rectangle contains corner");
    test(!r.contains(Point<double>::from(11.0, 0.0)), "Rectangle does not contain outside point");
    
    auto neg_rect = Rectangle<double>::from_corner(Point<double>::origin(), -10.0, 5.0);
    test(neg_rect.is_none(), "Rectangle with negative width returns None");

    // Rectangle from corners
    auto r2 = Rectangle<double>::from_corners(
        Point<double>::from(5.0, 5.0),
        Point<double>::from(0.0, 0.0)
    );
    test(r2.width() == 5.0 && r2.height() == 5.0, "Rectangle from_corners normalizes corners");
    test(r2.is_square(), "5x5 rectangle is square");
    
    // Rectangle intersection
    auto r3 = Rectangle<double>::from_corner(Point<double>::from(5.0, 0.0), 10.0, 5.0).unwrap();
    test(r.intersects(r3), "Overlapping rectangles intersect");
    
    auto [has_intersection, intersection_rect] = r.intersection(r3);
    test(has_intersection, "Intersection exists");
    test(approx_eq(intersection_rect.width(), 5.0), "Intersection width correct");

    auto corners = r.corners();
    test(corners.size() == 4, "Rectangle has 4 corners");

    // ==================== CRTP / Shape traits ====================
    std::cout << "\n--- Shape Traits ---\n";
    
    test(Circle<double>::is_shape, "Circle is_shape = true");
    test(Rectangle<double>::is_shape, "Rectangle is_shape = true");
    test(Circle<double>::DIMENSIONS == 2, "Circle dimensions = 2");
    test(Rectangle<double>::DIMENSIONS == 2, "Rectangle dimensions = 2");
    test(Circle<double>::name() == "Circle", "Circle::name() = 'Circle'");
    test(Rectangle<double>::name() == "Rectangle", "Rectangle::name() = 'Rectangle'");

    // ==================== With pulgacpp types ====================
    std::cout << "\n--- With pulgacpp Integer Types ---\n";
    
    // Using raw int instead of i32 since we need checked_add from namespace
    auto pi32 = Point<int>::from(100, 200);
    test(pi32.x() == 100 && pi32.y() == 200, "Point<int> creation");
    
    auto translated32 = pi32.checked_translate(50, 50);
    test(translated32.is_some(), "Point<int> translate works");
    test(translated32.unwrap().x() == 150, "Point<int> translate value correct");

    // ==================== Summary ====================
    std::cout << "\n=== Test Summary ===\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";

    return failed > 0 ? 1 : 0;
}
