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
        { { 0 , 0 },{ worldMax.x, 0 } },
        { { worldMax.x , 0 }, worldMax },
        { worldMax, { 0, worldMax.y } },
        { { 0, worldMax.y },{ 0, 0 } }
    },
    walls
    {
        // area bounding box
        { { {   0,   0 }, { 200,   0 } }, 10, {   0,   0, 255 }, 2 },
        { { { 200,   0 }, { 200, 200 } }, 10, {   0,   0, 255 }, 2 },
        { { { 200, 200 }, {   0, 200 } }, 10, {   0,   0, 255 }, 2 },
        { { {   0, 200 }, {   0,   0 } }, 10, {   0,   0, 255 }, 2 },
        // red wall
        { { { 180, 180 }, { 130, 160 } }, 10, { 255,   0,   0 }, 3 },
        // purple/textured wall
        { { {  10,  50 }, {  30,  80 } }, 10, { 255,   0, 255 }, 1 },
        { { {  30,  80 }, {  60,  60 } }, 10, { 255,   0, 255 }, 1 },
        { { {  60,  60 }, {  40,  30 } }, 10, { 255,   0, 255 }, 1 },
        { { {  40,  30 }, {  10,  50 } }, 10, { 255,   0, 255 }, 1 },
    },
    camera({50, 5}),
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
    double moveSpeed = frm.GetFrameTimeSecs() * 75;
    double rotSpeed = frm.GetFrameTimeSecs() * M_PI / 2;

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