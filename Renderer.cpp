//
//  Renderer.cpp
//  BspTree
//
//  Created by Brian Dolan on 5/12/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include "Renderer.hpp"
#include "GeomUtils.hpp"

void Renderer::RenderWall(const Wall &wall)
{
    // I'm going to guess that this is wildly inefficient, and that I'm
    // being dumb about doing it this way... but for now, this is
    // mathematically correct
    
    // find the angles and distances to the wall's endpoints
    
    bool p1IsOnScreen = false, p2IsOnScreen = false;
    uint32_t screenXP1, screenXP2;
    double distP1, distP2;
    uint32_t columnHeightP1, columnHeightP2;
    double textureXPercentageP1, textureXPercentageP2;
    
    // TODO: is it accurate enough to use camera.leftmostVisibleAngle, vs. the angle
    // from a line drawn from camera.location through the "middle" of the first pixel on the
    // left side of the screen?
    
    double angleP1 = getAngleFromCamera(wall.seg.p1);
    if (angleP1 >= camera.leftmostVisibleAngle && angleP1 <= camera.rightmostVisibleAngle)
    {
        screenXP1 = getScreenXFromAngle(angleP1);
        //if (isOnScreen(screenXP1))
        //{
        p1IsOnScreen = true;
        distP1 = getPerpendicularDistanceFromCameraByAngle(wall.seg.p1, angleP1);
        columnHeightP1 = getColumnHeightByDistance(distP1, wall.height);
        textureXPercentageP1 = 0;
        //}
    }
    else if (angleP1 > camera.rightmostVisibleAngle)
    {
        Line ray{ camera.location, camera.rightmostViewPlaneEnd };
        Vec2 intersection;
        double u;
        if (GeomUtils::FindRayLineSegIntersection(ray, wall.seg, intersection, u) == true)
        {
            screenXP1 = g.ScreenWidth - 1;
            p1IsOnScreen = true;
            distP1 = getPerpendicularDistanceFromCameraByAngle(intersection, camera.rightmostVisibleAngle);
            columnHeightP1 = getColumnHeightByDistance(distP1, wall.height);
            textureXPercentageP1 = u;
        }
    }
    else if (angleP1 < camera.leftmostVisibleAngle)
    {
        Line ray{ camera.location, camera.leftmostViewPlaneEnd };
        Vec2 intersection;
        double u;
        if (GeomUtils::FindRayLineSegIntersection(ray, wall.seg, intersection, u) == true)
        {
            screenXP1 = 0;
            p1IsOnScreen = true;
            distP1 = getPerpendicularDistanceFromCameraByAngle(intersection, camera.leftmostVisibleAngle);
            columnHeightP1 = getColumnHeightByDistance(distP1, wall.height);
            textureXPercentageP1 = u;
        }
    }
    
    double angleP2 = getAngleFromCamera(wall.seg.p2);
    if (angleP2 >= camera.leftmostVisibleAngle && angleP2 <= camera.rightmostVisibleAngle)
    {
        screenXP2 = getScreenXFromAngle(angleP2);
        //if (isOnScreen(screenXP2))
        //{
        p2IsOnScreen = true;
        distP2 = getPerpendicularDistanceFromCameraByAngle(wall.seg.p2, angleP2);
        columnHeightP2 = getColumnHeightByDistance(distP2, wall.height);
        textureXPercentageP2 = 1.0;
        //}
    }
    else if (angleP2 > camera.rightmostVisibleAngle)
    {
        Line ray{ camera.location, camera.rightmostViewPlaneEnd };
        Vec2 intersection;
        double u;
        if (GeomUtils::FindRayLineSegIntersection(ray, wall.seg, intersection, u) == true)
        {
            screenXP2 = g.ScreenWidth - 1;
            p2IsOnScreen = true;
            distP2 = getPerpendicularDistanceFromCameraByAngle(intersection, camera.rightmostVisibleAngle);
            columnHeightP2 = getColumnHeightByDistance(distP2, wall.height);
            textureXPercentageP2 = u;
        }
    }
    else if (angleP2 < camera.leftmostVisibleAngle)
    {
        Line ray{ camera.location, camera.leftmostViewPlaneEnd };
        Vec2 intersection;
        double u;
        if (GeomUtils::FindRayLineSegIntersection(ray, wall.seg, intersection, u) == true)
        {
            screenXP2 = 0;
            p2IsOnScreen = true;
            distP2 = getPerpendicularDistanceFromCameraByAngle(intersection, camera.leftmostVisibleAngle);
            columnHeightP2 = getColumnHeightByDistance(distP2, wall.height);
            textureXPercentageP2 = u;
        }
    }
    
    // fill in the middle
    if (p1IsOnScreen && p2IsOnScreen)
    {
        // potentially swap things around so that p1 is always to the left of p2,
        // so that we can draw left to right
        if ((screenXP2 < screenXP1) && !backfaceCulling)
        {
            std::swap(distP1, distP2);
            std::swap(screenXP1, screenXP2);
            std::swap(columnHeightP1, columnHeightP2);
            std::swap(textureXPercentageP1, textureXPercentageP2);
        }
        
        // if doing backface culling and this wall is facing the wrong way, don't draw it
        if (screenXP1 <= screenXP2)
        {
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
            
            // TODO: textureXPercentage can't increase linearly as we go from left to right across the screen
            // it's not that simple... this works only if the wall is facing the camera "head on"
            
            for (uint32_t screenX = screenXP1; screenX </*=?*/ screenXP2; screenX++)
            {
                if (!affineTextureMapping)
                {
                    // accurate, but division in loop
                    // U = U/Z * Z = (U/Z) / (1/Z) 
                    textureXPercentage = textureXPercentageOverDist / inverseDist;
                }
                
                RenderColumn(screenX, static_cast<uint32_t>(columnHeight + 0.5), wall.c, wall.textureNum, textureXPercentage);
                
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
    }
}

#include <cmath>

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
    double absAngleFromCameraDir = angleBetweenNormalizedVectors(camera.dirN, diffVectN);
    
    // we need to do the cross product in order to tell if this angle
    // is to the left of or the right of the camera's direction
    return (camera.dirN.cross(diffVectN) > 0 ? absAngleFromCameraDir : -absAngleFromCameraDir);
}

uint32_t Renderer::getScreenXFromAngle(double angle)
{
    // (see notebook for the math)
    double opp = camera.viewPlaneDist * tan(angle);
    double percentWidth = opp / (camera.viewPlaneWidth / 2);
    uint32_t screenX = static_cast<uint32_t>(percentWidth * static_cast<double>(HalfScreenWidth) + 0.5) + HalfScreenWidth;
    return screenX;
}

// TODO: code calling this function should probably be improved such
// that this function need not exist...
bool Renderer::isOnScreen(uint32_t screenX)
{
    //	return true;
    return (screenX >= 0 /* always will be, because it's unsigned... */
            && screenX < g.ScreenWidth);
}

// this is kind of a silly function, but it must exist when dealing with unsigned numbers...
int32_t Renderer::unsignedSub(uint32_t n1, uint32_t n2)
{
    return (static_cast<int32_t>(n1) - static_cast<int32_t>(n2));
}

double Renderer::angleBetweenVectors(const Vec2& v1, const Vec2& v2)
{
    return acos((v1 * v2) / (v1.Mag() * v2.Mag()));
    //	return asin(v1.cross(v2) / (v1.Mag() * v2.Mag()));
}

// a faster version of angleBetweenVectors, if the vectors have already been normalized
double Renderer::angleBetweenNormalizedVectors(const Vec2& v1, const Vec2& v2)
{
    return acos(v1 * v2);
}

uint32_t Renderer::RenderColumn(uint32_t screenX, uint32_t height, Color c, uint32_t textureNum, double textureXPercentage)
{
    uint32_t clippedHeight = (height <= g.ScreenHeight ? height : g.ScreenHeight);
    
    uint32_t halfHeight = clippedHeight / 2;
    uint32_t y1 = HalfScreenHeight - halfHeight;
    
    // figure out where in the pixel buffer to start, and only do
    // additions from there (avoid multiplication in the drawing loop)
    uint32_t pixelBufferOffset = y1 * g.ScreenWidth + screenX;
    
    double brightness = static_cast<double>(clippedHeight) / static_cast<double>(g.ScreenHeight);
    
    unsigned int* pPixelBuffer = g.GetScreenBuffer();
    
    // draw a solid, brightness-adjusted color all the way through the column
    
    if (brightnessAdjustment)
        c *= brightness;

    for (uint32_t i = 0; i < clippedHeight; i++)
    {
        pPixelBuffer[pixelBufferOffset] = c;
        
        pixelBufferOffset += g.ScreenWidth;
    }

    return y1 + clippedHeight;
}
