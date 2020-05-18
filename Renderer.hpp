//
//  Renderer.hpp
//  walls3d
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

// abstract base class for BspRenderer and Raycaster, hosting some common functions
class Renderer
{
public:
    Renderer(Graphics& g,
             const Camera& camera,
             const std::vector<Line>& worldBounds,
             const std::vector<Surface>& textures);
    ~Renderer() = default;
    
    virtual void RenderScene() = 0;
    virtual void RenderMap() = 0;
    
    void ToggleBrightnessAdjustment() { brightnessAdjustment = !brightnessAdjustment; }
    void ToggleDrawTextures() { drawTextures = !drawTextures; }
    void ShowDrawing() { showDrawing = true; }

protected:
    void BeginRender();
    void EndRender();
    double GetColumnHeightByDistance(double dist, double height);
    void RenderColumn(uint32_t screenX, double height, Color c, uint32_t textureNum, double textureXPercentage);
    uint32_t MapPercentageToRange(double percentage, uint32_t rangeHigh);
    
    void RenderMapBounds();
    void RenderMapCamera();
    void RenderMapLine(const Line& line, const Color& c);

    Graphics& g;
    const Camera& camera;
    const std::vector<Line>& worldBounds;
    const std::vector<Surface>& textures;
    
    bool brightnessAdjustment {true};
    bool drawTextures {true};
    bool showDrawing {false};
    
    const uint32_t HalfScreenWidth;
    const uint32_t HalfScreenHeight;
    
    // this is a simplification of a "z buffer" - currently, we don't care about
    // z-depth per column - we only care about whether or not a column was drawn
    std::unique_ptr<bool[]> pDrawnBuffer;
};

#endif /* Renderer_hpp */
