//
//  FrameRateMgr.hpp
//  engine3d
//
//  Created by Brian Dolan on 5/8/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef FrameRateMgr_hpp
#define FrameRateMgr_hpp

class FrameRateMgr
{
public:
    FrameRateMgr(bool printFps = false);
    ~FrameRateMgr() = default;
    void Mark();
    double GetFrameTimeSecs() { return frameTimeSecs; }

    static constexpr double PrintEverySecs {1.0f};

private:
    const bool printFps;
    bool firstMark;
    // time it took to render the last frame (only!), in secs
    double frameTimeSecs;
    Uint32 lastClockTicks;
    double secsUntilPrint;
    uint32_t framesDrawnSincePrint;
};

#endif /* FrameRateMgr_hpp */
