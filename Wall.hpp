//
//  Wall.hpp
//  BspTree
//
//  Created by Brian Dolan on 5/11/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Wall_hpp
#define Wall_hpp

#include "Line.hpp"
#include "Color.hpp"

class Wall
{
public:
    Line seg;
    double height;
    Color c;
    int32_t textureNum;
};

#endif /* Wall_hpp */
