//
//  FrameRateMgr.cpp
//  engine3d
//
//  Created by Brian Dolan on 5/8/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include <iostream>
#include <SDL2/SDL.h>
#include "FrameRateMgr.hpp"

FrameRateMgr::FrameRateMgr(bool printFps):
    printFps(printFps),
    lastClockTicks(SDL_GetTicks())
{
}

void FrameRateMgr::Mark()
{
    Uint32 nowClockTicks = SDL_GetTicks();
    
    frameTimeSecs = static_cast<float>(nowClockTicks - lastClockTicks) / 1000.0f;
    lastClockTicks = nowClockTicks;

    // print the frame rate
    if (printFps)
    {
        secsUntilPrint -= frameTimeSecs;
        if (secsUntilPrint <= 0)
        {
            secsUntilPrint = PrintEverySecs;
            std::cout << "FPS: " << static_cast<int>(1.0f / frameTimeSecs) << " frames/s" << std::endl;
        }
    }
}