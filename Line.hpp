//
//  Line.hpp
//  walls3d
//
//  Created by Brian Dolan on 5/11/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Line_hpp
#define Line_hpp

#include "Vec2.hpp"

class Line
{
public:
    Line operator+(const Vec2& rhs) const
    {
        return {p1 + rhs, p2 + rhs};
    }
    Line& operator+=(const Vec2& rhs)
    {
        *this = *this + rhs;
        return *this;
    }
    Line operator-(const Vec2& rhs) const
    {
        return {p1 - rhs, p2 - rhs};
    }
    Line& operator-=(const Vec2& rhs)
    {
        *this = *this - rhs;
        return *this;
    }
    
    Vec2 p1;
    Vec2 p2;
};

#endif /* Line_hpp */
