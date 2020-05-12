//
//  FrameRateMgr.cpp
//  engine3d
//
//  Created by Brian Dolan on 5/8/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include <iostream>
#include "FrameRateMgr.hpp"

FrameRateMgr::FrameRateMgr():
    lastClockTicks(clock())
{
}

void FrameRateMgr::Mark()
{
    clock_t nowClockTicks = clock();
    
    frameTimeSecs = static_cast<float>(nowClockTicks - lastClockTicks) / CLOCKS_PER_SEC;
    lastClockTicks = nowClockTicks;
    
    // print the frame rate
    secsUntilPrint -= frameTimeSecs;
    if (secsUntilPrint <= 0)
    {
        secsUntilPrint = PrintEverySecs;
        std::cout << "FPS: " << static_cast<int>(1.0f / frameTimeSecs) << " frames/s" << std::endl;
    }
}