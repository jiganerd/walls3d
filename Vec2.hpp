//
//  Vec2.hpp
//  engine3d
//
//  Created by Brian Dolan on 4/24/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Vec2_hpp
#define Vec2_hpp

#include <math.h>

template<typename T>
class _Vec2
{
public:
    _Vec2() = default;
    _Vec2(T x, T y):
        x(x),
        y(y)
    {}
    _Vec2 operator+(const _Vec2& rhs) const
    {
        return _Vec2(x + rhs.x, y + rhs.y);
    }
    _Vec2& operator+=(const _Vec2& rhs)
    {
        *this = *this + rhs;
        return *this;
    }
    _Vec2 operator-(const _Vec2& rhs) const
    {
        return _Vec2(x - rhs.x, y - rhs.y);
    }
    _Vec2& operator-=(const _Vec2& rhs)
    {
        *this = *this - rhs;
        return *this;
    }
    _Vec2& operator-() const
    {
        return (*this * static_cast<T>(-1));
    }
    _Vec2 operator*(const T& rhs) const
    {
        return _Vec2(x * rhs, y * rhs);
    }
    _Vec2& operator*=(const _Vec2& rhs)
    {
        *this = *this * rhs;
        return *this;
    }
    _Vec2 operator/(const T& rhs) const
    {
        return _Vec2(x / rhs, y / rhs);
    }
    _Vec2& operator/=(const _Vec2& rhs)
    {
        *this = *this / rhs;
        return *this;
    }
    T MagSq() const
    {
        return (x*x + y*y);
    }
    T Mag() const
    {
        return sqrt(MagSq());
    }
    _Vec2 Norm() const
    {
        T mag {Mag()};
        return _Vec2(x/mag, y/mag);
    }
    T operator*(const _Vec2& rhs) const // dot product
    {
        return (x * rhs.x + y * rhs.y);
    }
    T cross(const _Vec2& rhs) const // cross product
    {
        return (x * rhs.y - y * rhs.x);
    }
    _Vec2 InterpTo(const _Vec2& rhs, T percent) const
    {
        return (*this + (rhs - *this) * percent);
    }
    ~_Vec2() = default;

public:
    T x, y;
};

typedef _Vec2<double> Vec2;

#endif /* Vec2_hpp */
