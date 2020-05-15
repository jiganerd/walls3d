//
//  Renderer.cpp
//  BspTree
//
//  Created by Brian Dolan on 5/12/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include <cmath>
#include <cassert>
#include <string.h>
#include "Renderer.hpp"
#include "GeomUtils.hpp"
#include "Utils.hpp"

Renderer::Renderer(Graphics& g, const Camera& camera):
    g{g},
    camera{camera},
    HalfScreenWidth{g.ScreenWidth/2},
    HalfScreenHeight{g.ScreenHeight/2},
    pDrawnBuffer{new bool[Graphics::ScreenWidth]}
{
}

void Renderer::BindTexture(Surface&& texture)
{
    textures.push_back(std::move(texture));
}

void Renderer::BeginRender()
{
    memset(pDrawnBuffer.get(), 0, Graphics::ScreenWidth * sizeof(bool));
}

void Renderer::RenderWall(const Wall &wall)
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
        
        bool p1IsOnScreen = false, p2IsOnScreen = false;
        uint32_t screenXP1, screenXP2;
        double distP1, distP2;
        double textureXPercentageP1 = 0.0f, textureXPercentageP2 = 1.0f;
        
        // get properties for screen x, texture x percentage, and distance for each vertex, with clipping
        p1IsOnScreen = ClipAndGetAttributes(true, wall.seg, screenXP1, distP1, textureXPercentageP1);
        p2IsOnScreen = ClipAndGetAttributes(false, wall.seg, screenXP2, distP2, textureXPercentageP2);
        
        // if both (clipped) vertices are on screen, fill in the middle
        if (p1IsOnScreen && p2IsOnScreen)
        {
            assert(screenXP1 <= screenXP2);
            
            uint32_t columnHeightP1 = getColumnHeightByDistance(distP1, wall.height);
            uint32_t columnHeightP2 = getColumnHeightByDistance(distP2, wall.height);

            double columnHeight = columnHeightP1;
            uint32_t screenXDifference = screenXP2 - screenXP1;
            double columnHeightIncrement = static_cast<double>(unsignedSub(columnHeightP2, columnHeightP1) / static_cast<double>(screenXDifference));
            
            // (U/Z) / (1/Z) = U/Z * Z = U
            // (U/Z) and (1/Z) can increment linearly as we go across screen X coordinates
            
            // distance does not increment linearly!
            // but 1/distance does!
            // (1/Z)
            const double inverseDistStart = 1.0 / distP1;
            const double inverseDistEnd = 1.0 / distP2;
            const double inverseDistIncrement = (inverseDistEnd - inverseDistStart) / static_cast<double>(screenXDifference);
            double inverseDist = inverseDistStart;
            
            // (U/Z)
            const double textureXPercentageOverDistStart = textureXPercentageP1 / distP1;
            const double textureXPercentageOverDistEnd = textureXPercentageP2 / distP2;
            const double textureXPercentageOverDistIncrement = (textureXPercentageOverDistEnd - textureXPercentageOverDistStart) / static_cast<double>(screenXDifference);
            double textureXPercentageOverDist = textureXPercentageOverDistStart;
            
            // this stuff is for affine texture mapping (bad) only
            double textureXPercentage = 0;
            double textureXPercentageIncrement = 0;
            if ((wall.textureNum != -1) && affineTextureMapping)
            {
                textureXPercentage = textureXPercentageP1;
                textureXPercentageIncrement = (textureXPercentageP2 - textureXPercentageP1) / static_cast<double>(screenXDifference);
            }
            
            for (uint32_t screenX = screenXP1; screenX </*=?*/ screenXP2; screenX++)
            {
                if (!affineTextureMapping)
                {
                    // accurate, but division in loop
                    // U = U/Z * Z = (U/Z) / (1/Z) 
                    textureXPercentage = textureXPercentageOverDist / inverseDist;
                }
                
                if (!pDrawnBuffer[screenX])
                {
                    RenderColumn(screenX, columnHeight, wall.c, wall.textureNum, textureXPercentage);
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

uint32_t Renderer::getColumnHeightByDistance(double dist, double height)
{
    return static_cast<uint32_t>(3.0 * height / dist * static_cast<double>(g.ScreenHeight) + 0.5);
}

double Renderer::getPerpendicularDistanceFromCamera(const Vec2& point, double percentWidth)
{
    // see my notebook notes for the math here
    // this "perpendicular distance" equation prevents the fisheye effect that
    // would be seen with the Euclidean (real) distance
    
    Vec2 cameraToIntersection = point - camera.location;
    double opp = percentWidth * camera.viewPlaneWidth / 2;
    const double adj = camera.viewPlaneDist;
    double ratio = opp / adj;
    return sqrt((cameraToIntersection.x * cameraToIntersection.x + cameraToIntersection.y * cameraToIntersection.y) / (1 + ratio * ratio));
}

// (angleFromCamera could be figured out, but it is passed in for efficiency, as it has already been calculated)
double Renderer::getPerpendicularDistanceFromCameraByAngle(const Vec2& point, double angleFromCamera)
{
    double euclideanDistance = (point - camera.location).Mag();
    return euclideanDistance * cos(angleFromCamera);
}

double Renderer::getAngleFromCamera(const Vec2& location)
{
    Vec2 diffVectN = (location - camera.location).Norm();
    double absAngleFromCameraDir = GeomUtils::AngleBetweenNorm(camera.dirN, diffVectN);
    
    // we need to do the cross product in order to tell if this angle
    // is to the left of or the right of the camera's direction
    return (camera.dirN.cross(diffVectN) > 0 ? absAngleFromCameraDir : -absAngleFromCameraDir);
}

uint32_t Renderer::getScreenXFromAngle(double angle)
{
    // (see notebook for the math)
    double opp = camera.viewPlaneDist * tan(angle);
    double percentWidth = opp / (camera.viewPlaneWidth / 2);
    uint32_t screenX = static_cast<uint32_t>(percentWidth * static_cast<double>(HalfScreenWidth)) + HalfScreenWidth;

    assert(screenX < g.ScreenWidth);
    
    return screenX;
}

// this is kind of a silly function, but it must exist when dealing with unsigned numbers...
int32_t Renderer::unsignedSub(uint32_t n1, uint32_t n2)
{
    return (static_cast<int32_t>(n1) - static_cast<int32_t>(n2));
}

void Renderer::RenderColumn(uint32_t screenX, double height, Color c, uint32_t textureNum, double textureXPercentage)
{
    double clippedHeight = (height <= g.ScreenHeight ? height : g.ScreenHeight);
    double y1Float = static_cast<double>(HalfScreenHeight) - (clippedHeight / 2.0f);
    
    uint32_t y1 = Rast(y1Float);
    uint32_t y2 = Rast(y1Float + clippedHeight);
    
    // figure out where in the pixel buffer to start, and only do
    // additions from there (avoid multiplication in the drawing loop)
    uint32_t pixelBufferOffset = y1 * g.ScreenWidth + screenX;
    unsigned int* pPixelBuffer = g.GetScreenBuffer();
    
    double brightness = static_cast<double>(clippedHeight) / static_cast<double>(g.ScreenHeight);

    // if texturing...
    Surface* pTexture {nullptr};
    uint32_t texturePixelX {0};
    double texturePixelY {0.0f};
    double texturePixelYIncrement {0.0f};
    double texturePixelBufferOffset {0.0f};
    uint32_t texturePixelBufferLimit {0};
    if (drawTextures && textureNum != -1)
    {
        pTexture = &textures[textureNum];

        // we want to map height to a range of 0 to 1 (u/v coordinates), for use in sampling
        // the texture
        double textureYPercentageIncrement = 1.0f / height;
        
        // figure out the initial y location to start sampling the texture
        // in the simple case, this is 0, but if the camera is very close to the
        // texture, and it actually "starts" above the screen, we have to account
        // for that
        double heightClippedAboveScreen = (height - clippedHeight) / 2.0f;
        double textureYPercentage = textureYPercentageIncrement * heightClippedAboveScreen;
        
        // this logic may not be intuitive - we need to "bump" the initial texture lookup value a bit:
        // we actually should be calculating the lookup value based on the y value for the *vertical center*
        // of the first pixel in the column (not the upper edge)
        textureYPercentage += textureYPercentageIncrement * (static_cast<double>(y1) + 0.5f - y1Float);
        
        // map textureXPercentage/textureYPercentage to pixel coordinates within the texture
        texturePixelX = mapPercentageToRange(textureXPercentage, pTexture->Width() - 1);
        
        texturePixelYIncrement = textureYPercentageIncrement * static_cast<double>(pTexture->Height());
        texturePixelY = textureYPercentage * static_cast<double>(pTexture->Height());
        
        // (reminder that the texture pixel buffer is formatted as an array of *vertical* strips)
        texturePixelBufferOffset = static_cast<double>(texturePixelX * pTexture->Height()) + texturePixelY;
        
        // we perform clamping with this to handle floating point accumulation errors
        // (don't attempt to sample the texture past its height!)
        texturePixelBufferLimit = ((texturePixelX + 1) * pTexture->Height()) - 1;
    }
    else
    {
        if (brightnessAdjustment)
            c *= brightness;
    }
    
    for (uint32_t y = y1; y < y2; y++)
    {
        if (drawTextures && textureNum != -1)
        {
            c = pTexture->GetPixelBuffer()[static_cast<uint32_t>(texturePixelBufferOffset)];
            
            if (brightnessAdjustment)
                c *= brightness;
        }

        pPixelBuffer[pixelBufferOffset] = c;
        
        pixelBufferOffset += g.ScreenWidth;
        
        if (drawTextures && textureNum != -1)
        {
            texturePixelBufferOffset += texturePixelYIncrement;
            if (texturePixelBufferOffset > texturePixelBufferLimit)
                texturePixelBufferOffset = texturePixelBufferLimit;
        }
    }
    
    if (showDrawing)
        g.EndFrame();
}

bool Renderer::ClipAndGetAttributes(bool leftSide, const Line& wallSeg, uint32_t& screenX, double& dist, double& textureXPercentage)
{
    bool pIsOnScreen = false;
    Vec2 p = (leftSide ? wallSeg.p1 : wallSeg.p2);
    double angle = getAngleFromCamera(p);
    
    // TODO: is it accurate enough to use camera.leftmostVisibleAngle, vs. the angle
    // from a line drawn from camera.location through the "middle" of the first pixel on the
    // left side of the screen?
    
    // if point is in the field of view
    if (angle >= camera.leftmostVisibleAngle && angle <= camera.rightmostVisibleAngle)
    {
        pIsOnScreen = true;
        screenX = getScreenXFromAngle(angle);
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
        dist = getPerpendicularDistanceFromCameraByAngle(p, angle);
    
    return pIsOnScreen;
}

// maps a range of [0.0, 1.0] to [0, rangeHigh]
// clamps at rangeHigh to account for floating point error
uint32_t Renderer::mapPercentageToRange(double percentage, uint32_t rangeHigh)
{
    uint32_t retVal = static_cast<uint32_t>(percentage * static_cast<double>(rangeHigh + 1));
    if (retVal > rangeHigh) retVal = rangeHigh;
    return retVal;
}

// rasterization rule for how a subpixel (floating point) coordinate
// maps to a pixel (integer) coordinate
inline uint32_t Renderer::Rast(double n)
{
    return static_cast<uint32_t>(ceil(n - 0.5f));
}

