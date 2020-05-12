//
//  FrameRateMgr.hpp
//  engine3d
//
//  Created by Brian Dolan on 5/8/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef FrameRateMgr_hpp
#define FrameRateMgr_hpp

#include <ctime>

class FrameRateMgr
{
public:
    FrameRateMgr();
    ~FrameRateMgr() = default;
    void Mark();
    float GetFrameTimeSecs() { return frameTimeSecs; }

    static constexpr float PrintEverySecs = 1.0f;

private:
    // time it took to render the last frame, in secs
    float frameTimeSecs = 0.100; // (something reasonable before the first frame is rendered)
    clock_t lastClockTicks;
    float secsUntilPrint = PrintEverySecs;
};

#endif /* FrameRateMgr_hpp */
