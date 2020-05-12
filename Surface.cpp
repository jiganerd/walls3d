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
    int offset = 0;
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            pPixelBuffer[offset++] = Color(x ^ y, (h - 1) - y, (w - 1) - x);
        }
    }
}

Color Surface::GetPixel(int x, int y) const
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < w);
    assert(y < h);
    return pPixelBuffer[y * h + x];
}

Color Surface::GetPixelUV(float u, float v) const
{
    assert(u >= 0.0f);
    assert(v >= 0.0f);
    assert(u <= 1.0f);
    assert(v <= 1.0f);
    
    int x = static_cast<int>(u * static_cast<float>(w));
    int y = static_cast<int>(v * static_cast<float>(h));

    // as u/v values approach 1.0f above, we will correctly get the last pixel in the
    // surface - but if the u/v values are *exactly* 1.0f, we will get an x/y out
    // of range (unless we do something)
    Utils::Clamp(x, 0, w - 1);
    Utils::Clamp(y, 0, h - 1);
    
    return GetPixel(x, y);
}

void Surface::PutPixel(int x, int y, const Color& c)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < w);
    assert(y < h);
    pPixelBuffer[y * w + x] = c;
}