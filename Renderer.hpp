//
//  Renderer.hpp
//  BspTree
//
//  Created by Brian Dolan on 5/12/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <vector>
#include "Graphics.hpp"
#include "Wall.hpp"
#include "Camera.hpp"
#include "Surface.hpp"

class Renderer
{
public:
    Renderer(Graphics& g, const Camera& camera);
    ~Renderer() = default;
    
    void BindTexture(Surface&& texture);
    void BeginRender();
    void RenderWall(const Wall& wall);
    
private:
    uint32_t getColumnHeightByDistance(double dist, double height);
    double getPerpendicularDistanceFromCamera(const Vec2& point, double percentWidth);
    // (angleFromCamera could be figured out, but it is passed in for efficiency, as it has already been calculated)
    double getPerpendicularDistanceFromCameraByAngle(const Vec2& point, double angleFromCamera);
    double getAngleFromCamera(const Vec2& location);
    uint32_t getScreenXFromAngle(double angle);
    int32_t unsignedSub(uint32_t n1, uint32_t n2);
    uint32_t RenderColumn(uint32_t screenX, uint32_t height, Color c, uint32_t textureNum, double textureXPercentage);
    bool ClipAndGetAttributes(bool leftSide, const Line& wallSeg, uint32_t& screenX, double& dist, double& textureXPercentage);
    uint32_t mapPercentageToRange(double percentage, uint32_t rangeHigh);

    Graphics& g;
    const Camera& camera;
    
    bool affineTextureMapping {false};
    bool brightnessAdjustment {true};
    bool drawTextures {true};
    bool showDrawing {false};
    
    // this is a simplification of a "z buffer" - currently, we don't care about
    // z-depth per column - we only care about whether or not a column was drawn
    std::unique_ptr<bool[]> pDrawnBuffer;
    
    const uint32_t HalfScreenWidth;
    const uint32_t HalfScreenHeight;
    
    std::vector<Surface> textures;
};

#endif /* Renderer_hpp */
