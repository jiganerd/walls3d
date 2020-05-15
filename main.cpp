//
//  main.cpp
//  BspTree
//
//  Created by Brian Dolan on 5/9/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

// TODO:
// fix frame rate manager
// consistent style: use uniform init., capitalization of funcs, stdint vs. other style, floats vs. doubles, X.0f style for floating point
// clipping on map drawing
// show BSP visible walls in map

#include <iostream>
#include "Game.hpp"

int main(int argc, const char * argv[])
{
    Game g;
    
    bool quit = false;
    while (!quit)
    {
        quit = g.ProcessFrame();
    }
    
    return 0;
}