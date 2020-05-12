//
//  Game.hpp
//  BspTree
//
//  Created by Brian Dolan on 5/12/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <vector>
#include "Graphics.hpp"
#include "Wall.hpp"
#include "Line.hpp"
#include "Wall.hpp"
#include "BspTree.hpp"
#include "FrameRateMgr.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"

class Game
{
public:
    Game();
    bool ProcessFrame();
    
private:
    void HandleKeys();
    void DrawMap();
    void DrawMapBsp1(const Wall& wall, const BspTree::BspNodeDebugInfo& debugInfo);
    void DrawMapBsp2(const Wall& wall, const BspTree::BspNodeDebugInfo& debugInfo);
    void DrawMapLine(const Line& line, const Color&c);

    const Vec2 worldMax;
    std::vector<Line> worldBounds;
    std::vector<Wall> walls;
    Camera camera;
    
    Graphics g;
    Input i;
    FrameRateMgr frm;
    Renderer r;
    
    BspTree bspTree;
};

#endif /* Game_hpp */
