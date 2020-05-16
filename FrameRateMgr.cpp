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
    printFps{printFps},
    firstMark{false},
    frameTimeSecs{0.017f}, // (~60fps - something reasonable before the first frame is rendered)
    lastClockTicks{0},
    secsUntilPrint{PrintEverySecs},
    framesDrawnSincePrint{0}
{
}

void FrameRateMgr::Mark()
{
    Uint32 nowClockTicks = SDL_GetTicks();
    
    if (!firstMark)
        frameTimeSecs = static_cast<float>(nowClockTicks - lastClockTicks) / 1000.0f;
    
    lastClockTicks = nowClockTicks;

    // print the frame rate
    if (printFps && !firstMark)
    {
        secsUntilPrint -= frameTimeSecs;
        framesDrawnSincePrint++;
        if (secsUntilPrint <= 0)
        {
            // we could simply print frameTimeSecs, but that represents only the frame
            // it is better if we show an average since the last print
            std::cout << "FPS: " << static_cast<int>(static_cast<float>(framesDrawnSincePrint) / PrintEverySecs) << " frames/s" << std::endl;

            secsUntilPrint = PrintEverySecs;
            framesDrawnSincePrint = 0;
        }
    }
    
    firstMark = false;
}