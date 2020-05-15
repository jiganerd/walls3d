//
//  main.cpp
//  BspTree
//
//  Created by Brian Dolan on 5/9/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

// TODO:
// fix frame rate manager
// add Raycaster and BspRenderer classes inheriting from Renderer, move map drawing there
// clean up high-level class organization
// consistent style: use uniform init., capitalization of funcs, stdint vs. other style, floats vs. doubles, X.0f style for floating point
// clipping on map drawing

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