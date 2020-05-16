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
    FrameRateMgr(bool printFps = false);
    ~FrameRateMgr() = default;
    void Mark();
    float GetFrameTimeSecs() { return frameTimeSecs; }

    static constexpr float PrintEverySecs {1.0f};

private:
    const bool printFps;
    bool firstMark;
    // time it took to render the last frame (only!), in secs
    float frameTimeSecs;
    Uint32 lastClockTicks;
    float secsUntilPrint;
    uint32_t framesDrawnSincePrint;
};

#endif /* FrameRateMgr_hpp */
