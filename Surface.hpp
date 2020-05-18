//
//  Surface.hpp
//  engine3d
//
//  Created by Brian Dolan on 4/27/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Surface_hpp
#define Surface_hpp

#include <iostream>
#include <memory>
#include <cstdint>
#include "Color.hpp"

class Surface
{
public:
    Surface(uint32_t w, uint32_t h) :
        w(w),
        h(h),
        pPixelBuffer(new uint32_t[w * h])
    {}
    Surface(Surface&) = delete;
    Surface(Surface&& s) = default;
    uint32_t Width() const { return w; };
    uint32_t Height() const { return h; };
    uint32_t* GetPixelBuffer() const { return pPixelBuffer.get(); }
    Color GetPixel(int32_t x, int32_t y) const;
    Color GetPixelUV(double u, double v) const;
    void PutPixel(int32_t x, int32_t y, const Color& c);
    void FillXorPattern();
    ~Surface() = default;
    
private:
    uint32_t w;
    uint32_t h;
    std::unique_ptr<uint32_t[]> pPixelBuffer;
};

#endif /* Surface_hpp */
