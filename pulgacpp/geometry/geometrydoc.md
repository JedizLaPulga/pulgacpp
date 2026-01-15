# pulgacpp::geometry - 2D Geometry Types

> Safe, flexible, and compile-time geometry primitives for modern C++

## Quick Start

```cpp
#include <pulgacpp/geometry/geometry.hpp>
using namespace pulgacpp;

// Create a point
auto p = Point<double>::from(3.0, 4.0);

// Create a vector
auto v = Vector2<double>::from(1.0, 0.0);

// Create shapes
auto circle = Circle<double>::from(p, 10.0).unwrap();
auto rect = Rectangle<double>::from_corner(Point<double>::origin(), 100.0, 50.0).unwrap();

// Geometry operations
double dist = p.distance_to(Point<double>::origin());  // 5.0
double area = circle.area();                            // π * 100
bool inside = rect.contains(p);                         // true
```

---

## Table of Contents

1. [Design Principles](#design-principles)
2. [Point\<T\>](#pointt)
3. [Vector2\<T\>](#vector2t)
4. [Circle\<T\>](#circlet)
5. [Rectangle\<T\>](#rectanglet)
6. [Free Functions](#free-functions)
7. [Type Traits & CRTP](#type-traits--crtp)
8. [Numeric Type Flexibility](#numeric-type-flexibility)

---

## Design Principles

### Safety First
- **Checked arithmetic**: Operations that can overflow return `Optional<T>`
- **Invalid state prevention**: Cannot create Circle with negative radius
- **Immutability**: All operations return new objects, never mutate

### Flexibility
- Works with **native types** (`int`, `double`, `float`) for performance
- Works with **pulgacpp safe types** (`i32`, `i64`) for maximum safety
- All types are `constexpr`-compatible where possible

### CRTP Inheritance
```
Shape<Derived>
    └── Shape2D<Derived, T>
            ├── Circle<T>
            └── Rectangle<T>
```

---

## Point\<T\>

A 2D point with coordinates (x, y).

### Construction

| Factory | Description |
|---------|-------------|
| `Point<T>::from(x, y)` | Create from coordinates |
| `Point<T>::origin()` | Create point at (0, 0) |

```cpp
auto p1 = Point<double>::from(10.0, 20.0);
auto p2 = Point<int>::from(5, 10);
auto origin = Point<double>::origin();
```

### Accessors

| Method | Returns | Description |
|--------|---------|-------------|
| `x()` | `T` | X coordinate |
| `y()` | `T` | Y coordinate |
| `with_x(T)` | `Point<T>` | New point with different x |
| `with_y(T)` | `Point<T>` | New point with different y |

```cpp
auto p = Point<double>::from(3.0, 4.0);
double x = p.x();           // 3.0
auto moved = p.with_x(10.0); // Point(10.0, 4.0) - original unchanged
```

### Arithmetic (Checked)

| Method | Returns | Description |
|--------|---------|-------------|
| `checked_translate(dx, dy)` | `Optional<Point<T>>` | Move by delta |
| `checked_add(Point)` | `Optional<Point<T>>` | Add points |
| `checked_sub(Point)` | `Optional<Point<T>>` | Subtract points |
| `checked_scale(T)` | `Optional<Point<T>>` | Scale coordinates |

```cpp
auto p = Point<double>::from(10.0, 20.0);

auto moved = p.checked_translate(5.0, -10.0);
if (moved.is_some()) {
    // moved.unwrap() = Point(15.0, 10.0)
}

// Overflow detection with integers
auto big = Point<int>::from(2147483640, 0);
auto overflow = big.checked_translate(100, 0);
// overflow.is_none() == true (would overflow)
```

### Distance

| Method | Returns | Description |
|--------|---------|-------------|
| `distance_to(Point)` | `double` | Euclidean distance |
| `distance_squared(Point)` | `double` | Squared distance (faster) |
| `manhattan_distance(Point)` | `auto` | Manhattan (L1) distance |
| `magnitude()` | `double` | Distance from origin |

```cpp
auto a = Point<double>::from(0.0, 0.0);
auto b = Point<double>::from(3.0, 4.0);

double d = a.distance_to(b);      // 5.0 (3-4-5 triangle)
double d2 = a.distance_squared(b); // 25.0
auto manhattan = a.manhattan_distance(b); // 7
double mag = b.magnitude();       // 5.0
```

### Geometry

| Method | Returns | Description |
|--------|---------|-------------|
| `midpoint(Point)` | `Point<double>` | Midpoint between two points |
| `lerp(Point, t)` | `Point<double>` | Linear interpolation |
| `rotate(angle)` | `Point<double>` | Rotate around origin |
| `rotate_around(center, angle)` | `Point<double>` | Rotate around a point |

```cpp
auto a = Point<double>::from(0.0, 0.0);
auto b = Point<double>::from(10.0, 10.0);

auto mid = a.midpoint(b);       // Point(5.0, 5.0)
auto quarter = a.lerp(b, 0.25); // Point(2.5, 2.5)

auto p = Point<double>::from(1.0, 0.0);
auto rotated = p.rotate(constants::PI / 2.0); // Point(0.0, 1.0)
```

### Comparison

```cpp
auto p1 = Point<double>::from(1.0, 2.0);
auto p2 = Point<double>::from(1.0, 2.0);
auto p3 = Point<double>::from(1.0, 3.0);

p1 == p2; // true
p1 != p3; // true
```

### Type Aliases

| Alias | Definition |
|-------|------------|
| `Point32` | `Point<std::int32_t>` |
| `Point64` | `Point<std::int64_t>` |
| `PointF` | `Point<float>` |
| `PointD` | `Point<double>` |

---

## Vector2\<T\>

A 2D vector with components (x, y).

### Construction

| Factory | Description |
|---------|-------------|
| `Vector2<T>::from(x, y)` | Create from components |
| `Vector2<T>::zero()` | Zero vector (0, 0) |
| `vec_from_angle(angle, mag)` | From angle and magnitude (free function) |

```cpp
auto v = Vector2<double>::from(3.0, 4.0);
auto zero = Vector2<double>::zero();
auto unit_right = vec_from_angle(0.0);      // (1, 0)
auto unit_up = vec_from_angle(constants::PI / 2.0); // (0, 1)
```

### Accessors

| Method | Returns | Description |
|--------|---------|-------------|
| `x()` | `T` | X component |
| `y()` | `T` | Y component |
| `with_x(T)` | `Vector2<T>` | New vector with different x |
| `with_y(T)` | `Vector2<T>` | New vector with different y |

### Magnitude

| Method | Returns | Description |
|--------|---------|-------------|
| `magnitude()` | `double` | Vector length |
| `magnitude_squared()` | `double` | Squared length (faster) |
| `is_zero()` | `bool` | True if zero vector |

```cpp
auto v = Vector2<double>::from(3.0, 4.0);
double len = v.magnitude();         // 5.0
double len_sq = v.magnitude_squared(); // 25.0
bool zero = v.is_zero();            // false
```

### Normalization (Free Functions)

| Function | Returns | Description |
|----------|---------|-------------|
| `vec_normalized(v)` | `Optional<Vector2<double>>` | Unit vector |

```cpp
auto v = Vector2<double>::from(3.0, 4.0);
auto norm = vec_normalized(v);
if (norm.is_some()) {
    // norm.unwrap().magnitude() == 1.0
    // norm.unwrap() == Vector2(0.6, 0.8)
}

auto zero = Vector2<double>::zero();
vec_normalized(zero).is_none(); // true - cannot normalize zero
```

### Arithmetic (Checked)

| Method | Returns | Description |
|--------|---------|-------------|
| `checked_add(Vector2)` | `Optional<Vector2<T>>` | Add vectors |
| `checked_sub(Vector2)` | `Optional<Vector2<T>>` | Subtract vectors |
| `checked_scale(T)` | `Optional<Vector2<T>>` | Scale by factor |

```cpp
auto a = Vector2<double>::from(1.0, 2.0);
auto b = Vector2<double>::from(3.0, 4.0);

auto sum = a.checked_add(b).unwrap();   // (4.0, 6.0)
auto diff = b.checked_sub(a).unwrap();  // (2.0, 2.0)
auto scaled = a.checked_scale(3.0).unwrap(); // (3.0, 6.0)
```

### Vector Operations

| Method | Returns | Description |
|--------|---------|-------------|
| `dot(Vector2)` | `double` | Dot product |
| `cross(Vector2)` | `double` | Cross product (z-component) |
| `angle()` | `double` | Angle from positive x-axis |

```cpp
auto x_axis = Vector2<double>::from(1.0, 0.0);
auto y_axis = Vector2<double>::from(0.0, 1.0);

double dot = x_axis.dot(y_axis);   // 0.0 (perpendicular)
double cross = x_axis.cross(y_axis); // 1.0 (right-hand rule)
double angle = x_axis.angle();     // 0.0 radians
```

### Transformations (Free Functions)

| Function | Returns | Description |
|----------|---------|-------------|
| `vec_perpendicular(v)` | `Vector2<double>` | 90° counter-clockwise |
| `vec_negated(v)` | `Vector2<double>` | Flip direction |
| `vec_rotated(v, angle)` | `Vector2<double>` | Rotate by angle |
| `vector_from_points(start, end)` | `Vector2<double>` | Direction between points |

```cpp
auto v = Vector2<double>::from(1.0, 0.0);

auto perp = vec_perpendicular(v);    // (0.0, 1.0)
auto neg = vec_negated(v);           // (-1.0, 0.0)
auto rot = vec_rotated(v, constants::PI / 2.0); // (0.0, 1.0)

auto a = Point<double>::from(0.0, 0.0);
auto b = Point<double>::from(3.0, 4.0);
auto dir = vector_from_points(a, b); // (3.0, 4.0)
```

### Conversion

```cpp
auto v = Vector2<double>::from(5.0, 10.0);
auto p = v.to_point(); // Point(5.0, 10.0)
```

### Type Aliases

| Alias | Definition |
|-------|------------|
| `Vec2i` | `Vector2<int>` |
| `Vec2f` | `Vector2<float>` |
| `Vec2d` | `Vector2<double>` |

---

## Circle\<T\>

A circle defined by center point and radius.

### Construction

| Factory | Returns | Description |
|---------|---------|-------------|
| `Circle<T>::from(center, radius)` | `Optional<Circle<T>>` | Create circle (None if radius < 0) |
| `Circle<T>::unit()` | `Circle<T>` | Unit circle at origin |

```cpp
auto center = Point<double>::from(5.0, 5.0);
auto circle = Circle<double>::from(center, 10.0);

if (circle.is_some()) {
    auto c = circle.unwrap();
    // Use the circle
}

// Negative radius returns None
Circle<double>::from(center, -5.0).is_none(); // true

auto unit = Circle<double>::unit(); // center=(0,0), radius=1
```

### Accessors

| Method | Returns | Description |
|--------|---------|-------------|
| `center()` | `Point<T>` | Center point |
| `radius()` | `T` | Radius |
| `diameter()` | `double` | 2 × radius |

### Geometry (Shape2D Interface)

| Method | Returns | Description |
|--------|---------|-------------|
| `area()` | `double` | π × r² |
| `perimeter()` | `double` | 2 × π × r |
| `circumference()` | `double` | Alias for perimeter |

```cpp
auto c = Circle<double>::from(Point<double>::origin(), 5.0).unwrap();

double area = c.area();           // π × 25 ≈ 78.54
double perimeter = c.perimeter(); // 2π × 5 ≈ 31.42
double circum = c.circumference(); // same as perimeter
double diam = c.diameter();       // 10.0
```

### Containment

| Method | Returns | Description |
|--------|---------|-------------|
| `contains(Point)` | `bool` | Point inside or on boundary |
| `strictly_contains(Point)` | `bool` | Point strictly inside |
| `on_boundary(Point, tol)` | `bool` | Point on boundary |

```cpp
auto c = Circle<double>::from(Point<double>::origin(), 10.0).unwrap();

c.contains(Point<double>::from(5.0, 0.0));          // true (inside)
c.contains(Point<double>::from(10.0, 0.0));         // true (on boundary)
c.strictly_contains(Point<double>::from(10.0, 0.0)); // false (boundary)
c.on_boundary(Point<double>::from(10.0, 0.0));       // true
```

### Intersection & Overlap

| Method | Returns | Description |
|--------|---------|-------------|
| `overlaps(Circle)` | `bool` | Circles overlap (share area) |
| `intersects(Circle)` | `bool` | Circles touch or overlap |
| `contains_circle(Circle)` | `bool` | This contains other entirely |

```cpp
auto c1 = Circle<double>::from(Point<double>::origin(), 5.0).unwrap();
auto c2 = Circle<double>::from(Point<double>::from(8.0, 0.0), 5.0).unwrap();  // Overlapping
auto c3 = Circle<double>::from(Point<double>::from(10.0, 0.0), 5.0).unwrap(); // Tangent
auto c4 = Circle<double>::from(Point<double>::from(20.0, 0.0), 5.0).unwrap(); // Distant

c1.overlaps(c2);    // true
c1.intersects(c3);  // true (tangent counts as intersecting)
c1.overlaps(c3);    // false (tangent doesn't overlap)
c1.intersects(c4);  // false
```

### Transformations

| Method | Returns | Description |
|--------|---------|-------------|
| `checked_translate(dx, dy)` | `Optional<Circle<T>>` | Move center |
| `checked_scale(factor)` | `Optional<Circle<T>>` | Scale radius |
| `with_center(Point)` | `Circle<T>` | New center |
| `with_radius(T)` | `Optional<Circle<T>>` | New radius |

```cpp
auto c = Circle<double>::from(Point<double>::origin(), 5.0).unwrap();

auto moved = c.checked_translate(10.0, 20.0).unwrap();
auto scaled = c.checked_scale(2.0).unwrap(); // radius = 10

auto relocated = c.with_center(Point<double>::from(100.0, 100.0));
auto resized = c.with_radius(50.0).unwrap();
```

### Special Points

| Method | Returns | Description |
|--------|---------|-------------|
| `point_at_angle(angle)` | `Point<double>` | Point on circumference |
| `top()` | `Point<double>` | Northernmost point |
| `bottom()` | `Point<double>` | Southernmost point |
| `left()` | `Point<double>` | Westernmost point |
| `right()` | `Point<double>` | Easternmost point |

```cpp
auto c = Circle<double>::from(Point<double>::origin(), 5.0).unwrap();

auto rightmost = c.right();  // (5, 0)
auto topmost = c.top();      // (0, 5)
auto at_45deg = c.point_at_angle(constants::PI / 4.0);
```

### Circle from 3 Points (Free Function)

```cpp
auto p1 = Point<double>::from(5.0, 0.0);
auto p2 = Point<double>::from(0.0, 5.0);
auto p3 = Point<double>::from(-5.0, 0.0);

auto result = circle_from_points(p1, p2, p3);
if (result.is_some()) {
    auto c = result.unwrap();
    // c.radius() ≈ 5.0, c.center() ≈ (0, 0)
}

// Returns None for collinear points
auto collinear = circle_from_points(
    Point<double>::from(0.0, 0.0),
    Point<double>::from(1.0, 1.0),
    Point<double>::from(2.0, 2.0)
);
collinear.is_none(); // true
```

### Type Aliases

| Alias | Definition |
|-------|------------|
| `Circle32` | `Circle<std::int32_t>` |
| `Circle64` | `Circle<std::int64_t>` |
| `CircleF` | `Circle<float>` |
| `CircleD` | `Circle<double>` |

---

## Rectangle\<T\>

An axis-aligned rectangle defined by minimum corner and dimensions.

### Construction

| Factory | Returns | Description |
|---------|---------|-------------|
| `Rectangle<T>::from_corner(min, w, h)` | `Optional<Rectangle<T>>` | From corner + dimensions |
| `Rectangle<T>::from_corners(p1, p2)` | `Rectangle<T>` | From any two corners |
| `Rectangle<T>::unit()` | `Rectangle<T>` | 1×1 at origin |

```cpp
auto min = Point<double>::from(10.0, 20.0);
auto rect = Rectangle<double>::from_corner(min, 100.0, 50.0);

if (rect.is_some()) {
    auto r = rect.unwrap();
}

// Negative dimensions return None
Rectangle<double>::from_corner(min, -10.0, 50.0).is_none(); // true

// from_corners auto-normalizes
auto r = Rectangle<double>::from_corners(
    Point<double>::from(100.0, 100.0),  // top-right
    Point<double>::from(0.0, 0.0)       // bottom-left
);
// r.min_corner() = (0, 0), r.width() = 100, r.height() = 100
```

### Accessors

| Method | Returns | Description |
|--------|---------|-------------|
| `width()` | `T` | Width |
| `height()` | `T` | Height |
| `min_corner()` | `Point<T>` | Bottom-left corner |
| `max_corner()` | `Point<double>` | Top-right corner |
| `center()` | `Point<double>` | Center point |
| `left()` | `T` | Left edge x |
| `right()` | `double` | Right edge x |
| `bottom()` | `T` | Bottom edge y |
| `top()` | `double` | Top edge y |
| `corners()` | `std::array<Point<double>, 4>` | All four corners |

```cpp
auto r = Rectangle<double>::from_corner(
    Point<double>::from(10.0, 20.0), 30.0, 40.0
).unwrap();

r.width();      // 30.0
r.height();     // 40.0
r.left();       // 10.0
r.right();      // 40.0
r.bottom();     // 20.0
r.top();        // 60.0
r.center();     // Point(25.0, 40.0)

auto corners = r.corners();
// [0] = bottom-left, [1] = bottom-right
// [2] = top-right,   [3] = top-left
```

### Geometry (Shape2D Interface)

| Method | Returns | Description |
|--------|---------|-------------|
| `area()` | `double` | width × height |
| `perimeter()` | `double` | 2 × (width + height) |
| `diagonal()` | `double` | √(w² + h²) |
| `aspect_ratio()` | `Optional<double>` | width / height |
| `is_square()` | `bool` | width == height |

```cpp
auto r = Rectangle<double>::from_corner(
    Point<double>::origin(), 10.0, 20.0
).unwrap();

r.area();         // 200.0
r.perimeter();    // 60.0
r.diagonal();     // √500 ≈ 22.36
r.aspect_ratio(); // Some(0.5)
r.is_square();    // false

// Zero height
auto zero_h = Rectangle<double>::from_corner(
    Point<double>::origin(), 10.0, 0.0
).unwrap();
zero_h.aspect_ratio().is_none(); // true
```

### Containment

| Method | Returns | Description |
|--------|---------|-------------|
| `contains(Point)` | `bool` | Point inside or on boundary |
| `strictly_contains(Point)` | `bool` | Point strictly inside |

```cpp
auto r = Rectangle<double>::from_corner(
    Point<double>::origin(), 100.0, 100.0
).unwrap();

r.contains(Point<double>::from(50.0, 50.0));         // true (interior)
r.contains(Point<double>::origin());                  // true (corner)
r.strictly_contains(Point<double>::origin());         // false (boundary)
r.contains(Point<double>::from(150.0, 50.0));        // false (outside)
```

### Intersection

| Method | Returns | Description |
|--------|---------|-------------|
| `intersects(Rectangle)` | `bool` | Rectangles overlap |
| `contains_rect(Rectangle)` | `bool` | This contains other entirely |

```cpp
auto r1 = Rectangle<double>::from_corner(
    Point<double>::origin(), 100.0, 100.0
).unwrap();
auto r2 = Rectangle<double>::from_corner(
    Point<double>::from(50.0, 50.0), 100.0, 100.0
).unwrap();

r1.intersects(r2);    // true
r1.contains_rect(r2); // false (only partial overlap)
```

### Transformations

| Method | Returns | Description |
|--------|---------|-------------|
| `checked_translate(dx, dy)` | `Optional<Rectangle<T>>` | Move |
| `checked_scale(factor)` | `Optional<Rectangle<T>>` | Scale dimensions |

```cpp
auto r = Rectangle<double>::from_corner(
    Point<double>::from(10.0, 10.0), 20.0, 30.0
).unwrap();

auto moved = r.checked_translate(5.0, -5.0).unwrap();
auto scaled = r.checked_scale(2.0).unwrap();  // 40×60
```

### Type Aliases

| Alias | Definition |
|-------|------------|
| `Rect32` | `Rectangle<std::int32_t>` |
| `Rect64` | `Rectangle<std::int64_t>` |
| `RectF` | `Rectangle<float>` |
| `RectD` | `Rectangle<double>` |

---

## Free Functions

### Rectangle Operations

| Function | Returns | Description |
|----------|---------|-------------|
| `rect_intersection(r1, r2)` | `Optional<Rectangle<double>>` | Intersection rectangle |
| `rect_from_center(center, w, h)` | `Optional<Rectangle<double>>` | Create from center |
| `rect_expanded(r, amount)` | `Optional<Rectangle<double>>` | Expand/shrink |

```cpp
auto r1 = Rectangle<double>::from_corner(Point<double>::origin(), 100.0, 100.0).unwrap();
auto r2 = Rectangle<double>::from_corner(Point<double>::from(50.0, 50.0), 100.0, 100.0).unwrap();

// Intersection
auto inter = rect_intersection(r1, r2);
if (inter.is_some()) {
    auto i = inter.unwrap();
    // 50×50 intersection rectangle
}

// From center
auto centered = rect_from_center(Point<double>::from(50.0, 50.0), 20.0, 30.0);

// Expand (positive) or shrink (negative)
auto expanded = rect_expanded(r1, 10.0);  // 120×120
auto shrunk = rect_expanded(r1, -10.0);   // 80×80
```

### Circle Operations

| Function | Returns | Description |
|----------|---------|-------------|
| `circle_from_points(p1, p2, p3)` | `Optional<Circle<double>>` | Circumscribed circle |

### Vector Operations

| Function | Returns | Description |
|----------|---------|-------------|
| `vec_normalized(v)` | `Optional<Vector2<double>>` | Unit vector |
| `vec_perpendicular(v)` | `Vector2<double>` | 90° rotation |
| `vec_negated(v)` | `Vector2<double>` | Flip direction |
| `vec_rotated(v, angle)` | `Vector2<double>` | Rotate by angle |
| `vec_from_angle(angle, mag)` | `Vector2<double>` | From polar coords |
| `vector_from_points(start, end)` | `Vector2<double>` | Direction vector |

---

## Type Traits & CRTP

All geometry types expose compile-time properties:

```cpp
// Static properties
Point<double>::NAME;       // "Point"
Circle<double>::NAME;      // "Circle"
Rectangle<double>::NAME;   // "Rectangle"

Point<double>::DIMENSIONS;     // 2
Circle<double>::DIMENSIONS;    // 2

Circle<double>::is_shape;      // true
Rectangle<double>::is_shape;   // true

// CRTP methods
Circle<double>::name();        // "Circle"
Circle<double>::dimensions();  // 2
```

### Shape2D Interface

Circles and Rectangles inherit from `Shape2D<Derived, T>`:

```cpp
// Both provide these methods via CRTP:
circle.area();      // π × r²
circle.perimeter(); // 2π × r

rect.area();        // w × h
rect.perimeter();   // 2(w + h)
```

---

## Numeric Type Flexibility

All geometry types work with both native and pulgacpp safe types:

### Native Types (Performance)
```cpp
Circle<double> c1;   // Fast, no overflow checks
Circle<float> c2;    // Lower precision, faster
Circle<int> c3;      // Integer coordinates
```

### pulgacpp Types (Safety)
```cpp
#include <pulgacpp/i32/i32.hpp>

Circle<i32> safe_circle = Circle<i32>::from(
    Point<i32>::from(10_i32, 20_i32),
    100_i32
).unwrap();

// Overflow is detected by checked operations
auto result = safe_circle.checked_scale(1000000_i32);
// result.is_none() if overflow occurs
```

### When to Use Which

| Use Case | Recommended Type |
|----------|-----------------|
| Game graphics | `double` or `float` |
| Financial/precise calculations | pulgacpp safe types |
| Pixel coordinates | `int` or `i32` |
| Scientific computing | `double` |

---

## Mathematical Constants

The geometry module provides these constants:

```cpp
pulgacpp::constants::PI;   // 3.14159265358979323846
pulgacpp::constants::TAU;  // 2π = 6.28318530717958647692
```

---

## Complete Example: Collision Detection System

```cpp
#include <pulgacpp/geometry/geometry.hpp>
#include <vector>

using namespace pulgacpp;

struct Entity {
    Circle<double> hitbox;
    Vector2<double> velocity;
};

class CollisionSystem {
    Rectangle<double> world_bounds;
    
public:
    CollisionSystem(double width, double height)
        : world_bounds(Rectangle<double>::from_corner(
            Point<double>::origin(), width, height
          ).unwrap()) {}
    
    bool isInWorld(const Entity& e) const {
        return world_bounds.contains(e.hitbox.center());
    }
    
    bool checkCollision(const Entity& a, const Entity& b) const {
        return a.hitbox.overlaps(b.hitbox);
    }
    
    void moveEntity(Entity& e, double dt) {
        double dx = e.velocity.x() * dt;
        double dy = e.velocity.y() * dt;
        
        auto new_hitbox = e.hitbox.checked_translate(dx, dy);
        if (new_hitbox.is_some()) {
            e.hitbox = new_hitbox.unwrap();
        }
    }
    
    Vector2<double> directionTo(const Entity& from, const Entity& to) const {
        auto dir = vector_from_points(
            from.hitbox.center(),
            to.hitbox.center()
        );
        return vec_normalized(dir).unwrap_or(Vector2<double>::zero());
    }
};
```

---

## See Also

- [Optional\<T\> Documentation](../optional/optionaldoc.md)
- [Result\<T, E\> Documentation](../result/resultdoc.md)
- [Safe Integer Types](../i32/i32doc.md)
