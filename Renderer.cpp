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
#include <vector>
#include "Renderer.hpp"
#include "GeomUtils.hpp"
#include "Utils.hpp"

using std::vector;

Renderer::Renderer(Graphics& g,
                   const Camera& camera,
                   const vector<Line>& worldBounds,
                   const std::vector<Surface>& textures):
    g{g},
    camera{camera},
    worldBounds{worldBounds},
    textures{textures},
    HalfScreenWidth{g.ScreenWidth/2},
    HalfScreenHeight{g.ScreenHeight/2},
    pDrawnBuffer{new bool[Graphics::ScreenWidth]}
{
}

void Renderer::BeginRender()
{
    memset(pDrawnBuffer.get(), 0, Graphics::ScreenWidth * sizeof(bool));
}

void Renderer::EndRender()
{
    showDrawing = false;
}

double Renderer::getColumnHeightByDistance(double dist, double height)
{
    return (3.0f * height / dist * static_cast<double>(g.ScreenHeight));
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
    const Surface* pTexture {nullptr};
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

void Renderer::RenderMapBounds()
{
    for (const auto& line : worldBounds)
        RenderMapLine(line, Colors::Gray);
}

void Renderer::RenderMapCamera()
{
    RenderMapLine({camera.location, camera.leftmostViewPlaneEnd}, Colors::Red);
    RenderMapLine({camera.rightmostViewPlaneEnd, camera.location}, Colors::Red);
    RenderMapLine({camera.leftmostViewPlaneEnd, camera.rightmostViewPlaneEnd}, Colors::Cyan);
}

void Renderer::RenderMapLine(const Line& line, const Color& c)
{
    Vec2 transVec {10.0f, 10.0f};
    g.DrawLine(line.p1 + transVec, line.p2 + transVec, c);
}