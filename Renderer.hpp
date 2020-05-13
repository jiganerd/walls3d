//
//  Renderer.hpp
//  BspTree
//
//  Created by Brian Dolan on 5/12/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include "Graphics.hpp"
#include "Wall.hpp"
#include "Camera.hpp"

class Renderer
{
public:
    Renderer(Graphics& g, const Camera& camera):
        g{g},
        camera{camera},
        HalfScreenWidth{g.ScreenWidth/2},
        HalfScreenHeight{g.ScreenHeight/2}
    {}
    void RenderWall(const Wall& wall);
    
private:
    uint32_t getColumnHeightByDistance(double dist, double height);
    double getPerpendicularDistanceFromCamera(const Vec2& point, double percentWidth);
    // (angleFromCamera could be figured out, but it is passed in for efficiency, as it has already been calculated)
    double getPerpendicularDistanceFromCameraByAngle(const Vec2& point, double angleFromCamera);
    double getAngleFromCamera(const Vec2& location);
    uint32_t getScreenXFromAngle(double angle);
    bool isOnScreen(uint32_t screenX);
    int32_t unsignedSub(uint32_t n1, uint32_t n2);
    
    uint32_t RenderColumn(uint32_t screenX, uint32_t height, Color c, uint32_t textureNum, double textureXPercentage);
    
    Graphics& g;
    const Camera& camera;
    
    bool affineTextureMapping {false};
    bool brightnessAdjustment {true};
    
    const uint32_t HalfScreenWidth;
    const uint32_t HalfScreenHeight;
};

#endif /* Renderer_hpp */
