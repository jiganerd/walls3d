//
//  Game.hpp
//  walls3d
//
//  Created by Brian Dolan on 5/12/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <vector>
#include "Surface.hpp"
#include "Graphics.hpp"
#include "Wall.hpp"
#include "Line.hpp"
#include "FrameRateMgr.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "BspRenderer.hpp"
#include "Raycaster.hpp"
#include "DxfLoader.hpp"

class Game
{
public:
    Game();
    bool ProcessFrame();
    
private:
    void HandleKeys();
    
    static constexpr bool LoadFromFile {true};

    const Vec2 worldMax;
    const std::vector<Line> worldBounds;
    std::vector<Wall> walls;
    std::vector<Surface> textures;
    Graphics g;
    Camera camera;
    Input i;
    FrameRateMgr frm;
    BspRenderer bspr;
    Raycaster rc;
    Renderer* pActiveRenderer;
};

#endif /* Game_hpp */
