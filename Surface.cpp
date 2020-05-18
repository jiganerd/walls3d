//
//  Surface.cpp
//  engine3d
//
//  Created by Brian Dolan on 4/27/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include <cassert>
#include "Surface.hpp"
#include "Color.hpp"
#include "Utils.hpp"

void Surface::FillXorPattern()
{
    uint32_t offset {0};
    for (uint32_t x {0}; x < w; x++)
    {
        for (uint32_t y {0}; y < h; y++)
        {
            pPixelBuffer[offset++] = Color(x ^ y, (h - 1) - y, (w - 1) - x);
        }
    }
}

Color Surface::GetPixel(int32_t x, int32_t y) const
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < w);
    assert(y < h);
    
    return pPixelBuffer[y * h + x];
}

Color Surface::GetPixelUV(double u, double v) const
{
    assert(u >= 0.0f);
    assert(v >= 0.0f);
    assert(u <= 1.0f);
    assert(v <= 1.0f);
    
    uint32_t x {static_cast<uint32_t>(u * static_cast<double>(w))};
    uint32_t y {static_cast<uint32_t>(v * static_cast<double>(h))};

    // as u/v values approach 1.0f above, we will correctly get the last pixel in the
    // surface - but if the u/v values are *exactly* 1.0f, we will get an x/y out
    // of range (unless we do something)
    Utils::Clamp(x, 0u, w - 1);
    Utils::Clamp(y, 0u, h - 1);
    
    return GetPixel(x, y);
}

void Surface::PutPixel(int32_t x, int32_t y, const Color& c)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < w);
    assert(y < h);
    pPixelBuffer[y * w + x] = c;
}