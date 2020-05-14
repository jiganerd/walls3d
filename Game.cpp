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
    r(g, camera),
    bspTree{walls, worldBounds, std::bind(&Renderer::RenderWall, &r, std::placeholders::_1)}
{
    bspTree.Print();
}

bool Game::ProcessFrame()
{
    bool quit = i.ProcessKeys();
    
    if (!quit)
    {
        HandleKeys();
        
        // TODO: organize this a bit
        g.BeginFrame();
        r.BeginRender();
        bspTree.Render(camera.location);
        DrawMap();
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
}

void Game::DrawMap()
{
    // draw world bounds
    for (const auto& line : worldBounds)
        DrawMapLine(line, Colors::Blue);
    
    // draw walls and BSP split lines
    bspTree.DebugTraverse(std::bind(&Game::DrawMapBsp1, this, std::placeholders::_1, std::placeholders::_2));
    bspTree.DebugTraverse(std::bind(&Game::DrawMapBsp2, this, std::placeholders::_1, std::placeholders::_2));
    
    // draw the camera's viewing frustrum
    DrawMapLine({camera.location, camera.leftmostViewPlaneEnd}, Colors::Red);
    DrawMapLine({camera.rightmostViewPlaneEnd, camera.location}, Colors::Red);
    DrawMapLine({camera.leftmostViewPlaneEnd, camera.rightmostViewPlaneEnd}, Colors::Cyan);
}

void Game::DrawMapBsp1(const Wall& wall, const BspTree::BspNodeDebugInfo& debugInfo)
{
    DrawMapLine(debugInfo.extendedLineForMapDrawingBackward, Colors::Gray);
    DrawMapLine(debugInfo.extendedLineForMapDrawingForward, Colors::Gray);
}

void Game::DrawMapBsp2(const Wall& wall, const BspTree::BspNodeDebugInfo& debugInfo)
{
    DrawMapLine(wall.seg, debugInfo.randomColorForMapDrawing);
}

void Game::DrawMapLine(const Line& line, const Color&c)
{
    Vec2 transVec {10.0f, 10.0f};
    g.DrawLine(line.p1 + transVec, line.p2 + transVec, c);
}