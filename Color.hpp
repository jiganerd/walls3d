//
//  Color.hpp
//  engine3d
//
//  Created by Brian Dolan on 4/24/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Color_hpp
#define Color_hpp

#include <cstdint>

class Color
{
public:
    constexpr Color(uint8_t r, uint8_t g, uint8_t b):
        argb(0x00000000 |
             static_cast<uint32_t>(r) << 16 |
             static_cast<uint32_t>(g) << 8 |
             static_cast<uint32_t>(b))
    { }
    constexpr Color(uint32_t argb):
        argb(argb)
    { }
    constexpr operator uint32_t() const
    {
        return argb;
    }
    constexpr Color operator*(float n) const
    {
        return Color(R()*n, G()*n, B()*n);
    }
    Color& operator*=(float n)
    {
        *this = *this * n;
        return *this;
    }
    void SetRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        argb = 0x00000000 |
                static_cast<uint32_t>(r) << 16 |
                static_cast<uint32_t>(g) << 8 |
                static_cast<uint32_t>(b);
    }
    constexpr uint8_t R() const
    {
        return ((argb & 0x00FF0000) >> 16);
    }
    constexpr uint8_t G() const
    {
        return ((argb & 0x0000FF00) >> 8);
    }
    constexpr uint8_t B() const
    {
        return ((argb & 0x000000FF));
    }
    void SetR(uint8_t r)
    {
        argb = ((argb & 0xFF00FFFF) | (static_cast<uint32_t>(r) << 16));
    }
    void SetG(uint8_t g)
    {
        argb = ((argb & 0xFFFF00FF) | (static_cast<uint32_t>(g) << 8));
    }
    void SetB(uint8_t b)
    {
        argb = ((argb & 0xFFFFFF00) | static_cast<uint32_t>(b));
    }
public:
    uint32_t argb;
};

// based on standard HTML colors
namespace Colors
{
    static constexpr Color Black   {Color(  0,   0,   0)};
    static constexpr Color White   {Color(255, 255, 255)};
    
    static constexpr Color Red     {Color(255,   0,   0)};
    static constexpr Color Lime    {Color(  0, 255,   0)};
    static constexpr Color Blue    {Color(  0,   0, 255)};
    
    static constexpr Color Yellow  {Color(255, 255,   0)};
    static constexpr Color Cyan    {Color(  0, 255, 255)};
    static constexpr Color Magenta {Color(255,   0, 255)};

    static constexpr Color Silver  {Color(192, 192, 192)};
    static constexpr Color Gray    {Color(128, 128, 128)};
    static constexpr Color Maroon  {Color(128,   0,   0)};
    static constexpr Color Olive   {Color(128, 128,   0)};
    static constexpr Color Green   {Color(  0, 128,   0)};
    static constexpr Color Purple  {Color(128,   0, 128)};
    static constexpr Color Teal    {Color(  0, 128, 128)};
    static constexpr Color Navy    {Color(  0,   0, 128)};
}

#endif /* Color_hpp */
