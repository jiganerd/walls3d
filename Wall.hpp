//
//  Wall.hpp
//  walls3d
//
//  Created by Brian Dolan on 5/11/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Wall_hpp
#define Wall_hpp

#include <ostream>
#include "Line.hpp"
#include "Color.hpp"

class Wall
{
public:
    Wall(const Line& seg, double height, const Color& c, int32_t textureNum):
        seg{seg},
        height{height},
        c{c},
        textureNum{textureNum}
    {
    }
    Wall(const Line& seg):
        seg{seg},
        height{10.0f},
        c{Colors::Cyan},
        textureNum{0}
    {
    }
    ~Wall() = default;
    void Serialize(std::ostream& strm) const
    {
        strm << seg;
    }
    
    Line seg;
    double height;
    Color c;
    int32_t textureNum;
};

std::ostream& operator<<(std::ostream& s, const Wall& w);

#endif /* Wall_hpp */
