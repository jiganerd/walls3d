//
//  Line.hpp
//  walls3d
//
//  Created by Brian Dolan on 5/11/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Line_hpp
#define Line_hpp

#include <ostream>
#include "Vec2.hpp"
#include "Serializer.hpp"

class Line
{
public:
    Line():
        p1{0.0f, 0.0f},
        p2{0.0f, 0.0f}
    {
    }
    Line(const Vec2& p1, const Vec2& p2):
        p1{p1},
        p2{p2}
    {
    }
    Line(const uint8_t* bytes, size_t& offset):
        p1{Serializer::DeSerDouble(bytes, offset), Serializer::DeSerDouble(bytes, offset)},
        p2{Serializer::DeSerDouble(bytes, offset), Serializer::DeSerDouble(bytes, offset)}
    {
    }
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
    double Mag() const
    {
        return ((p2 - p1).Mag());
    }
    void Serialize(std::ostream& strm) const
    {
        strm << Serializer::Ser(p1.x)
             << Serializer::Ser(p1.y)
             << Serializer::Ser(p2.x)
             << Serializer::Ser(p2.y);
    }
    
    Vec2 p1;
    Vec2 p2;
};

std::ostream& operator<<(std::ostream& s, const Line& l);

#endif /* Line_hpp */
