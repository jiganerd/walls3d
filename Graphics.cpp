//
//  Graphics.cpp
//  walls3d
//
//  Created by Brian Dolan on 4/24/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include <assert.h>
#include <algorithm>
#include "Graphics.hpp"

Graphics::SDLException::SDLException(std::string msg):
    error(SDL_GetError()),
    msg(msg)
{
}

std::string Graphics::SDLException::GetMsg() const
{
    return "SDLException: " + msg + ": " + error;
}

Graphics::Graphics() :
    screen(ScreenWidth, ScreenHeight)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw SDLException("Error initializating SDL");
    
    SDL_CreateWindowAndRenderer(ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN, &pWindow, &pRenderer);
    if (pWindow == NULL)
        throw SDLException("Window could not be created");

    pScreenTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, ScreenWidth, ScreenHeight);
    if (pScreenTexture == NULL)
        throw SDLException("Could not create screen texture");
    
    SDL_RaiseWindow(pWindow);
}

Graphics::~Graphics()
{
    SDL_DestroyTexture(pScreenTexture);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
}

void Graphics::BeginFrame()
{
    memset(screen.GetPixelBuffer(), 0, ScreenWidth * ScreenHeight * sizeof(uint32_t)/sizeof(uint8_t));
}

void Graphics::EndFrame()
{
    if (SDL_UpdateTexture(pScreenTexture, NULL, screen.GetPixelBuffer(), ScreenWidth * sizeof(uint32_t)) < 0)
        throw SDLException("Could not update screen texture");
    
    if (SDL_RenderCopy(pRenderer, pScreenTexture, NULL, NULL) < 0)
        throw SDLException("Could not render screen copy");
    
    SDL_RenderPresent(pRenderer);
}

Surface Graphics::LoadTexture(std::string filename, bool sideways)
{
    SDL_Surface* pSurf {SDL_LoadBMP(filename.c_str())};
    if (!pSurf)
        throw SDLException("Could not load texture");
   
    Surface s(pSurf->w, pSurf->h);
    
    // load image into pixels array in ARGB format
    // (no problem if this is slow)
    auto pPixelBuffer = s.GetPixelBuffer();
    uint32_t offset {0};
    for (uint32_t y {0}; y < pSurf->h; y++)
    {
        for (uint32_t x {0}; x < pSurf->w; x++)
        {
            pPixelBuffer[offset++] = GetSDLSurfaceColor(*pSurf, (sideways ? y : x), (sideways ? x : y));
        }
    }
    
    return s;
}

void Graphics::PutPixel(int32_t x, int32_t y, int32_t r, int32_t g, int32_t b)
{
    PutPixel(x, y, Color(r, g, b));
}

void Graphics::PutPixel(int32_t x, int32_t y, const Color& c)
{
    screen.PutPixel(x, y, c);
}

void Graphics::DrawLine(const Vec2& v1, const Vec2& v2, const Color& c)
{
    DrawLine(v1.x, v1.y, v2.x, v2.y, c);
}

// taken from CS480 homework! (it's not perfect...)
void Graphics::DrawLine(double x1, double y1, double x2, double y2, const Color& c)
{
    // a lazy man's clipping
    if ((x1 < 0) || (x1 > ScreenWidth - 1) ||
        (x2 < 0) || (x2 > ScreenWidth - 1) ||
        (y1 < 0) || (y1 > ScreenHeight - 1) ||
        (y2 < 0) || (y2 > ScreenHeight - 1))
        return; // f it!
    
    // if we've been passed a single point, don't bother drawing a line
    if (x1 == x2 && y1 == y2)
    {
        PutPixel(Rast(x1), Rast(y1), c);
    }
    else
    {
        double x {x1}, y {y1};
        
        uint32_t numPix; // the number of times to iterate
        int32_t distX {Rast(x2) - Rast(x1)}; // the dimensions of the line
        int32_t distY {Rast(y2) - Rast(y1)};
        
        // should we scan horizontally or vertically?
        if (abs(distX) > abs(distY)) numPix = abs(distX);
        else numPix = abs(distY);
        
        // calculate the incremental x and y values (can be pos. or neg.)
        double deltaX {static_cast<double>(distX) / static_cast<double>(numPix)};
        double deltaY {static_cast<double>(distY) / static_cast<double>(numPix)};
        
        // draw the line
        for (uint32_t i {0}; i < numPix; i++)
        {
            PutPixel(Rast(x), Rast(y), c);
            x += deltaX;
            y += deltaY;
        }
        PutPixel(Rast(x), Rast(y), c);
    }
}

uint32_t* Graphics::GetScreenBuffer()
{
    return screen.GetPixelBuffer();
}

Color Graphics::GetSDLSurfaceColor(const SDL_Surface& surface, uint32_t x, uint32_t y)
{
    const SDL_PixelFormat* pPixelFormat {surface.format};
    uint8_t *pPixels {static_cast<uint8_t*>(surface.pixels)};
    
    uint8_t* pPixel {&pPixels[((y * surface.w) + x) * pPixelFormat->BytesPerPixel]};
    
    Uint32 pixel;
    switch (pPixelFormat->BytesPerPixel)
    {
        case 1:
            pixel = *pPixel;
            break;
            
        case 2:
            pixel = *(Uint16 *)pPixel;
            break;
            
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                pixel = pPixel[0] << 16 | pPixel[1] << 8 | pPixel[2];
            else
                pixel = pPixel[0] | pPixel[1] << 8 | pPixel[2] << 16;
            break;
            
        case 4:
            pixel = *(Uint32 *)pPixel;
            break;
            
        default:
            pixel = 0;
    }

    Uint8 r, g, b;
    SDL_GetRGB(pixel, pPixelFormat, &r, &g, &b);
    return Color(r, g, b);
}