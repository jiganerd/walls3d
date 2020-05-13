//
//  main.cpp
//  BspTree
//
//  Created by Brian Dolan on 5/9/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

// TODO:
// floats vs. doubles
// fix turnaround problem at start
// fix frame rate manager
// clean up render wall func
// add Raycaster class w/ dependency to Renderer class for column
// add texturing
// use uniform init., capitalization of funcs, stdint vs. other style, X.0f style for floating point
// fix brightness adjustment in Color mult?

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