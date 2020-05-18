//
//  Graphics.hpp
//  walls3d
//
//  Created by Brian Dolan on 4/24/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Graphics_hpp
#define Graphics_hpp

#include <string>
#include <cmath>
#include <cstdint>
#include <SDL2/SDL.h>
#include "Color.hpp"
#include "Vec2.hpp"
#include "Surface.hpp"

class Graphics
{
public:
    class Exception
    {
    public:
        virtual std::string GetMsg() const = 0;
    };
    
    Graphics();
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    void BeginFrame();
    void EndFrame();
    static Surface LoadTexture(std::string filename, bool sideways = false);
    void PutPixel(int32_t x, int32_t y, int32_t r, int32_t g, int32_t b);
    void PutPixel(int32_t x, int32_t y, const Color& c);
    // this follows triangle rasterization rules described at
    // https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-rasterizer-stage-rules
    inline static int32_t Rast(double n) { return static_cast<int32_t>(ceil(n - 0.5f)); };
    void DrawLine(const Vec2& v1, const Vec2& v2, const Color& c);
    void DrawLine(double x1, double y1, double x2, double y2, const Color& c);
    uint32_t* GetScreenBuffer();
    ~Graphics();
    
private:
    class SDLException : public Exception
    {
    public:
        SDLException(std::string msg);
        std::string GetMsg() const override;
    private:
        std::string error;
        std::string msg;
    };

    static Color GetSDLSurfaceColor(const SDL_Surface& surface, uint32_t x, uint32_t y);

    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;
    SDL_Texture* pScreenTexture;
    Surface screen;
    
public:
    static constexpr uint32_t ScreenWidth {800u};
    static constexpr uint32_t ScreenHeight {600u};
};

#endif /* Graphics_hpp */
