//
//  Game.cpp
//  walls3d
//
//  Created by Brian Dolan on 5/12/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include <iostream>
#include "Game.hpp"

Game::Game():
    worldMax{300, 300},
    worldBounds
    {
        {{      0.0f, 0.0f}, {worldMax.x, 0.0f}},
        {{worldMax.x, 0.0f},           worldMax},
        {          worldMax, {0.0f, worldMax.y}},
        {{0.0f, worldMax.y}, {0.0f,       0.0f}}
    },
    camera({60.0f, 15.0f}),
    frm(true),
    bspr(g, camera, worldBounds, textures),
    rc(g, camera, walls, worldBounds, textures),
    pActiveRenderer{&bspr}
{
    // load textures
    
    textures.push_back(Graphics::LoadTexture("brick.bmp", true));
    textures.push_back(Graphics::LoadTexture("brick_markedup.bmp", true));
    textures.push_back(Graphics::LoadTexture("stripes.bmp", true));
    
    Surface xorSurface(256, 256);
    xorSurface.FillXorPattern();
    textures.push_back(std::move(xorSurface));
    
    // load walls
    
    if (LoadFromFile)
    {
        std::vector<Line> lines;
        DxfLoader dxfLoader("walls.dxf", lines);
        for (const Line& l : lines)
            walls.push_back({l, 10.0f, Colors::Cyan, 0});
    }
    else
    {
        walls =
        {
            // area bounding box
            { { {  10.0f,  10.0f }, { 210.0f,  10.0f } }, 10.0f,    Colors::Blue, 2 },
            { { { 210.0f,  10.0f }, { 210.0f, 210.0f } }, 10.0f,    Colors::Blue, 2 },
            { { { 210.0f, 210.0f }, {  10.0f, 210.0f } }, 10.0f,    Colors::Blue, 2 },
            { { {  10.0f, 210.0f }, {  10.0f,  10.0f } }, 10.0f,    Colors::Blue, 2 },
            // standalone wall
            { { { 190.0f, 190.0f }, { 140.0f, 170.0f } }, 10.0f,     Colors::Red, 3 },
            // interior "box"
            { { {  20.0f,  60.0f }, {  40.0f,  90.0f } }, 10.0f, Colors::Magenta, 1 },
            { { {  40.0f,  90.0f }, {  70.0f,  70.0f } }, 10.0f, Colors::Magenta, 1 },
            { { {  70.0f,  70.0f }, {  50.0f,  40.0f } }, 10.0f, Colors::Magenta, 1 },
            { { {  50.0f,  40.0f }, {  20.0f,  60.0f } }, 10.0f, Colors::Magenta, 1 },
        };
    }
    
    bspr.ProcessWalls(walls);
}

bool Game::ProcessFrame()
{
    bool quit {i.ProcessKeys()};
    
    if (!quit)
    {
        HandleKeys();
        
        g.BeginFrame();
        
        pActiveRenderer->RenderScene();
        pActiveRenderer->RenderMap();

        g.EndFrame();
        
        frm.Mark();
    }
    
    return quit;
}

void Game::HandleKeys()
{
    double moveSpeed {frm.GetFrameTimeSecs() * 75.0f};
    double rotSpeed {frm.GetFrameTimeSecs() * M_PI / 2.0f};

    if (i.GetMoveForward())  camera.MoveForward(moveSpeed);
    if (i.GetMoveBackward()) camera.MoveForward(-moveSpeed);
    if (i.GetRotateLeft())   camera.Rotate(-rotSpeed);
    if (i.GetRotateRight())  camera.Rotate(rotSpeed);
    if (i.GetStrafeLeft())   camera.Strafe(-moveSpeed);
    if (i.GetStrafeRight())  camera.Strafe(moveSpeed);
    
    // swap renderers when tab pressed
    if (i.GetTabFirstPressed())
    {
        if (pActiveRenderer == &bspr)
            pActiveRenderer = &rc;
        else
            pActiveRenderer = &bspr;
    }
    
    if (i.GetSFirstPressed())
    {
        pActiveRenderer->ShowDrawing();
    }
    if (i.GetTFirstPressed())
    {
        bspr.ToggleDrawTextures();
        rc.ToggleDrawTextures();
    }
    if (i.GetAFirstPressed())
    {
        bspr.ToggleAffineTextureMapping();
    }
    if (i.GetBFirstPressed())
    {
        bspr.ToggleBrightnessAdjustment();
        rc.ToggleBrightnessAdjustment();
    }
}