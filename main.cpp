//
//  main.cpp
//  BspTree
//
//  Created by Brian Dolan on 5/9/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include <iostream>
#include <vector>
#include "BspTree.hpp"
#include "Wall.hpp"

void RenderWall(const Wall& wall)
{
    std::cout << "rendering wall from "
        << "{" << wall.seg.p1.x << ", " << wall.seg.p1.y << "}"
        << " to "
        << "{" << wall.seg.p2.x << ", " << wall.seg.p2.y << "}"
        << std::endl;
}

int main(int argc, const char * argv[])
{
    const Vec2 worldMax{300, 300};

    std::vector<Line> worldBounds =
    {
        { { 0 , 0 },{ worldMax.x, 0 } },
        { { worldMax.x , 0 }, worldMax },
        { worldMax, { 0, worldMax.y } },
        { { 0, worldMax.y },{ 0, 0 } }
    };

    std::vector<Wall> walls = {
        // world bounding box
        { { {   0,   0 }, { 200,   0 } }, 10, {   0,   0, 255, 255 }, 2 },
        { { { 200,   0 }, { 200, 200 } }, 10, {   0,   0, 255, 255 }, 2 },
        { { { 200, 200 }, {   0, 200 } }, 10, {   0,   0, 255, 255 }, 2 },
        { { {   0, 200 }, {   0,   0 } }, 10, {   0,   0, 255, 255 }, 2 },
        // red wall
        { { { 130, 160 }, { 180, 180 } }, 10, { 255,   0,   0, 255 }, 3 },
        // purple/textured wall
        { { {  10,  50 }, {  30,  80 } }, 10, { 255,   0, 255, 255 }, 1 },
        { { {  30,  80 }, {  60,  60 } }, 10, { 255,   0, 255, 255 }, 1 },
        { { {  60,  60 }, {  40,  30 } }, 10, { 255,   0, 255, 255 }, 1 },
        { { {  40,  30 }, {  10,  50 } }, 10, { 255,   0, 255, 255 }, 1 },
    };

    BspTree t(walls, worldBounds, RenderWall);
    t.Print();
    
    Vec2 p;
    t.Find(p);
    t.Render({0,0});
    
    return 0;
}