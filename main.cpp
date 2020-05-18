//
//  main.cpp
//  walls3d
//
//  Created by Brian Dolan on 5/9/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

// TODO:
// clipping on map drawing
// deal with split wall texturing
// use "horizontal center of column" for x scans in both bsp and raycast modes and "pre-step" into angles

#include <iostream>
#include "Game.hpp"

int main(int argc, const char * argv[])
{
    Game g;
    
    bool quit {false};
    while (!quit)
    {
        quit = g.ProcessFrame();
    }
    
    return 0;
}