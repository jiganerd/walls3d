//
//  BspRenderer.hpp
//  BspTree
//
//  Created by Brian Dolan on 5/15/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef BspRenderer_hpp
#define BspRenderer_hpp

#include <vector>
#include "Renderer.hpp"
#include "BspTree.hpp"

class BspRenderer : public Renderer
{
public:
    BspRenderer(Graphics& g,
                const Camera& camera,
                const std::vector<Line>& worldBounds,
                const std::vector<Surface>& textures);
    ~BspRenderer() = default;

    void ProcessWalls(const std::vector<Wall>& walls);
    
    void RenderScene() override;
    void RenderMap() override;
    
    void ToggleAffineTextureMapping() { affineTextureMapping = !affineTextureMapping; }
    
private:
    
    void RenderWall(const Wall& wall, const BspTree::BspNodeDebugInfo& debugInfo);
    double GetPerpendicularDistanceFromCameraByAngle(const Vec2& point, double angleFromCamera);
    double GetAngleFromCamera(const Vec2& location);
    uint32_t GetScreenXFromAngle(double angle);
    int32_t UnsignedSub(uint32_t n1, uint32_t n2);
    bool ClipAndGetAttributes(bool leftSide, const Line& wallSeg, uint32_t& screenX, double& dist, double& textureXPercentage);

    void RenderMapDivision(const Wall& wall, const BspTree::BspNodeDebugInfo& debugInfo);
    void RenderMapWallDark(const Wall& wall, const BspTree::BspNodeDebugInfo& debugInfo);
    void RenderMapWallBright(const Wall& wall, const BspTree::BspNodeDebugInfo& debugInfo);
    
    void PrintCameraNodeIndex();

    bool affineTextureMapping {false};
    
    BspTree bspTree;
    int32_t cameraNodeIndex;
};

#endif /* BspRenderer_hpp */
