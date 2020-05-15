//
//  Game.cpp
//  BspTree
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
    walls
    {
        // area bounding box
        { { {  10.0f,  10.0f }, { 210.0f,  10.0f } }, 10.0f, {   0,   0, 255 }, 2 },
        { { { 210.0f,  10.0f }, { 210.0f, 210.0f } }, 10.0f, {   0,   0, 255 }, 2 },
        { { { 210.0f, 210.0f }, {  10.0f, 210.0f } }, 10.0f, {   0,   0, 255 }, 2 },
        { { {  10.0f, 210.0f }, {  10.0f,  10.0f } }, 10.0f, {   0,   0, 255 }, 2 },
        // red wall
        { { { 190.0f, 190.0f }, { 140.0f, 170.0f } }, 10.0f, { 255,   0,   0 }, 3 },
        // purple/textured wall
        { { {  20.0f,  60.0f }, {  40.0f,  90.0f } }, 10.0f, { 255,   0, 255 }, 1 },
        { { {  40.0f,  90.0f }, {  70.0f,  70.0f } }, 10.0f, { 255,   0, 255 }, 1 },
        { { {  70.0f,  70.0f }, {  50.0f,  40.0f } }, 10.0f, { 255,   0, 255 }, 1 },
        { { {  50.0f,  40.0f }, {  20.0f,  60.0f } }, 10.0f, { 255,   0, 255 }, 1 },
    },
    camera({60.0f, 15.0f}),
    frm(true),
    bspr(g, camera, walls, worldBounds, textures),
    rc(g, camera, walls, worldBounds, textures),
    pActiveRenderer{&bspr}
{
    textures.push_back(Graphics::LoadTexture("brick.bmp", true));
    textures.push_back(Graphics::LoadTexture("brick_markedup.bmp", true));
    textures.push_back(Graphics::LoadTexture("stripes.bmp", true));
    
    Surface xorSurface(256, 256);
    xorSurface.FillXorPattern();
    textures.push_back(std::move(xorSurface));
}

bool Game::ProcessFrame()
{
    bool quit = i.ProcessKeys();
    
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
    double moveSpeed = frm.GetFrameTimeSecs() * 75.0f;
    double rotSpeed = frm.GetFrameTimeSecs() * M_PI / 2.0f;

    if (i.GetMoveForward())  camera.moveForward(moveSpeed);
    if (i.GetMoveBackward()) camera.moveForward(-moveSpeed);
    if (i.GetRotateLeft())   camera.rotate(-rotSpeed);
    if (i.GetRotateRight())  camera.rotate(rotSpeed);
    if (i.GetStrafeLeft())   camera.strafe(-moveSpeed);
    if (i.GetStrafeRight())  camera.strafe(moveSpeed);
    
    // swap renderers when tab pressed
    if (i.GetTabFirstPressed())
    {
        if (pActiveRenderer == &bspr)
            pActiveRenderer = &rc;
        else
            pActiveRenderer = &bspr;
    }
}