//
//  BspRenderer.cpp
//  walls3d
//
//  Created by Brian Dolan on 5/15/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include <vector>
#include <cassert>
#include <iostream>
#include "BspRenderer.hpp"
#include "GeomUtils.hpp"

using std::vector;

BspRenderer::BspRenderer(Graphics& g,
                         const Camera& camera,
                         const std::vector<Line>& worldBounds,
                         const vector<Surface>& textures):
    Renderer(g, camera, worldBounds, textures),
    cameraNodeIndex{-1}
{
}

void BspRenderer::ProcessWalls(const std::vector<Wall>& walls)
{
    bspTree.ProcessWalls(walls, worldBounds);
    
    bspTree.Print();
    cameraNodeIndex = bspTree.Find(camera.location);
    PrintCameraNodeIndex();
}

void BspRenderer::RenderScene()
{
    BeginRender();
    
    bspTree.TraverseRender(camera.location, std::bind(&BspRenderer::RenderWall, this, std::placeholders::_1, std::placeholders::_2));
    
    EndRender();
    
    // technically this is not "rendering", but... sue me
    int32_t newCameraNodeIndex {bspTree.Find(camera.location)};
    if (newCameraNodeIndex != cameraNodeIndex)
    {
        cameraNodeIndex = newCameraNodeIndex;
        PrintCameraNodeIndex();
    }
}

void BspRenderer::RenderWall(const Wall &wall, const BspTree::BspNodeDebugInfo& debugInfo)
{
    // the BSP tree should be ensuring that we only get to this function
    // if this wall is facing the camera
    assert(GeomUtils::IsPointInFrontOfLine(wall.seg, camera.location));
    
    // but this doesn't mean that the wall is "in front of" the camera
    // as per the camera's view direction
    // cull any walls that are entirely behind the camera to reduce processing
    if (!camera.IsBehind(wall.seg))
    {
        // at this point, we can safely assume that the p2 vertex of the wall is to the right
        // of the p1 vertex in screen coordinates!
        
        bool p1IsOnScreen {false}, p2IsOnScreen {false};
        uint32_t screenXP1, screenXP2;
        double distP1, distP2;
        double textureXPercentageP1 {0.0f}, textureXPercentageP2 {1.0f};
        
        // get properties for screen x, texture x percentage, and distance for each vertex, with clipping
        p1IsOnScreen = ClipAndGetAttributes(true, wall.seg, screenXP1, distP1, textureXPercentageP1);
        p2IsOnScreen = ClipAndGetAttributes(false, wall.seg, screenXP2, distP2, textureXPercentageP2);
        
        // if both (clipped) vertices are on screen, fill in the middle
        if (p1IsOnScreen && p2IsOnScreen)
        {
            assert(screenXP1 <= screenXP2);
            
            double columnHeightP1 {GetColumnHeightByDistance(distP1, wall.height)};
            double columnHeightP2 {GetColumnHeightByDistance(distP2, wall.height)};
            
            double columnHeight {columnHeightP1};
            uint32_t screenXDifference {screenXP2 - screenXP1};
            double columnHeightIncrement {(columnHeightP2 - columnHeightP1) / static_cast<double>(screenXDifference)};
            
            // (U/Z) / (1/Z) = U/Z * Z = U
            // (U/Z) and (1/Z) can increment linearly as we go across screen X coordinates
            
            // distance does not increment linearly!
            // but 1/distance does!
            // (1/Z)
            const double inverseDistStart {1.0f / distP1};
            const double inverseDistEnd {1.0f / distP2};
            const double inverseDistIncrement {(inverseDistEnd - inverseDistStart) / static_cast<double>(screenXDifference)};
            double inverseDist {inverseDistStart};
            
            // (U/Z)
            const double textureXPercentageOverDistStart {textureXPercentageP1 / distP1};
            const double textureXPercentageOverDistEnd {textureXPercentageP2 / distP2};
            const double textureXPercentageOverDistIncrement {(textureXPercentageOverDistEnd - textureXPercentageOverDistStart) / static_cast<double>(screenXDifference)};
            double textureXPercentageOverDist {textureXPercentageOverDistStart};
            
            // this stuff is for affine texture mapping (bad) only
            double textureXPercentage {0.0f};
            double textureXPercentageIncrement {0.0f};
            if ((wall.textureNum != -1) && affineTextureMapping)
            {
                textureXPercentage = textureXPercentageP1;
                textureXPercentageIncrement = (textureXPercentageP2 - textureXPercentageP1) / static_cast<double>(screenXDifference);
            }
            
            for (uint32_t screenX = screenXP1; screenX <= screenXP2; screenX++)
            {
                if (!affineTextureMapping)
                {
                    // accurate, but division in loop
                    // U = U/Z * Z = (U/Z) / (1/Z)
                    textureXPercentage = textureXPercentageOverDist / inverseDist;
                }
                
                if (!pDrawnBuffer[screenX])
                {
                    RenderColumn(screenX, columnHeight, debugInfo.mapColor, wall.textureNum, textureXPercentage);
                    pDrawnBuffer[screenX] = true;
                }
                
                columnHeight += columnHeightIncrement;
                
                if (affineTextureMapping)
                {
                    // inaccurate, but quick (simple addition)
                    textureXPercentage += textureXPercentageIncrement;
                }
                else
                {
                    textureXPercentageOverDist += textureXPercentageOverDistIncrement;
                    inverseDist += inverseDistIncrement;
                }
            }
        }
        // else the wall is in front of the camera, but entirely outside the field of view to the right
        // or the left
    }
}

// (angleFromCamera could be figured out, but it is passed in for efficiency, as it has already been calculated)
double BspRenderer::GetPerpendicularDistanceFromCameraByAngle(const Vec2& point, double angleFromCamera)
{
    double euclideanDistance {(point - camera.location).Mag()};
    return euclideanDistance * cos(angleFromCamera);
}

double BspRenderer::GetAngleFromCamera(const Vec2& location)
{
    Vec2 diffVectN {(location - camera.location).Norm()};
    double absAngleFromCameraDir {GeomUtils::AngleBetweenNorm(camera.dirN, diffVectN)};
    
    // we need to do the cross product in order to tell if this angle
    // is to the left of or the right of the camera's direction
    return (camera.dirN.cross(diffVectN) > 0 ? absAngleFromCameraDir : -absAngleFromCameraDir);
}

uint32_t BspRenderer::GetScreenXFromAngle(double angle)
{
    // (see notebook for the math)
    double opp {camera.viewPlaneDist * tan(angle)};
    double percentWidth {opp / (camera.viewPlaneWidth / 2.0f)};
    uint32_t screenX {static_cast<uint32_t>(percentWidth * static_cast<double>(HalfScreenWidth)) + HalfScreenWidth};
    
    assert(screenX < g.ScreenWidth);
    
    return screenX;
}

// this is kind of a silly function, but it must exist when dealing with unsigned numbers...
int32_t BspRenderer::UnsignedSub(uint32_t n1, uint32_t n2)
{
    return (static_cast<int32_t>(n1) - static_cast<int32_t>(n2));
}

bool BspRenderer::ClipAndGetAttributes(bool leftSide, const Line& wallSeg, uint32_t& screenX, double& dist, double& textureXPercentage)
{
    bool pIsOnScreen {false};
    Vec2 p {(leftSide ? wallSeg.p1 : wallSeg.p2)};
    double angle {GetAngleFromCamera(p)};
    
    // TODO: is it accurate enough to use camera.leftmostVisibleAngle, vs. the angle
    // from a line drawn from camera.location through the "middle" of the first pixel on the
    // left side of the screen?
    
    // if point is in the field of view
    if (angle >= camera.leftmostVisibleAngle && angle <= camera.rightmostVisibleAngle)
    {
        pIsOnScreen = true;
        screenX = GetScreenXFromAngle(angle);
        textureXPercentage = (leftSide ? 0.0f : 1.0f);
    }
    else
    {
        // perform clipping if necessary
        Line ray;
        if (leftSide) // clip to the left of the camera's field of view
        {
            angle = camera.leftmostVisibleAngle;
            ray = {camera.location, camera.leftmostViewPlaneEnd};
        }
        else // clip to the right of the camera's field of view
        {
            angle = camera.rightmostVisibleAngle;
            ray = {camera.location, camera.rightmostViewPlaneEnd};
        }
        
        if (GeomUtils::FindRayLineSegIntersection(ray, wallSeg, p, textureXPercentage))
        {
            pIsOnScreen = true;
            screenX = (leftSide ? 0 : (g.ScreenWidth - 1));
        }
        // else no clipping was performed - the point is out of the field of
        // view and the wall does not lie in the path of the edges of the field
        // of view
    }
    
    if (pIsOnScreen)
        dist = GetPerpendicularDistanceFromCameraByAngle(p, angle);
    
    return pIsOnScreen;
}

void BspRenderer::RenderMap()
{
    RenderMapBounds();
    
    // draw BSP split lines and darkened versions of *all* walls
    bspTree.TraverseDebug(std::bind(&BspRenderer::RenderMapDivision, this, std::placeholders::_1, std::placeholders::_2));
    bspTree.TraverseDebug(std::bind(&BspRenderer::RenderMapWallDark, this, std::placeholders::_1, std::placeholders::_2));
    
    // draw bright versions of the walls which are not culled by the BSP tree render traversal
    bspTree.TraverseRender(camera.location, std::bind(&BspRenderer::RenderMapWallBright, this, std::placeholders::_1, std::placeholders::_2));

    RenderMapCamera();
}

void BspRenderer::RenderMapDivision(const Wall& wall, const BspTree::BspNodeDebugInfo& debugInfo)
{
    RenderMapLine(debugInfo.extendedLineForMapDrawingBackward, Colors::Gray);
    RenderMapLine(debugInfo.extendedLineForMapDrawingForward, Colors::Gray);
}

void BspRenderer::RenderMapWallDark(const Wall& wall, const BspTree::BspNodeDebugInfo& debugInfo)
{
    RenderMapLine(wall.seg, debugInfo.mapColor * 0.5f);
}

void BspRenderer::RenderMapWallBright(const Wall& wall, const BspTree::BspNodeDebugInfo& debugInfo)
{
    RenderMapLine(wall.seg, debugInfo.mapColor);
}

void BspRenderer::PrintCameraNodeIndex()
{
    std::cout << "Camera BSP node index: " << cameraNodeIndex << std::endl;
}