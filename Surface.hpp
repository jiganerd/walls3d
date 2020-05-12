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
#include "Color.hpp"

class Surface
{
public:
    Surface(int w, int h) :
        w(w),
        h(h),
        pPixelBuffer(new unsigned int[w * h])
    {}
    Surface(Surface&) = delete;
    Surface(Surface&& s) = default;
    int Width() const { return w; };
    int Height() const { return h; };
    unsigned int* GetPixelBuffer() const { return pPixelBuffer.get(); }
    Color GetPixel(int x, int y) const;
    Color GetPixelUV(float u, float v) const;
    void PutPixel(int x, int y, const Color& c);
    void FillXorPattern();
    ~Surface() = default;
    
private:
    int w;
    int h;
    std::unique_ptr<unsigned int[]> pPixelBuffer;
};

#endif /* Surface_hpp */
