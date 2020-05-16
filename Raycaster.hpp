//
//  Raycaster.hpp
//  BspTree
//
//  Created by Brian Dolan on 5/15/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Raycaster_hpp
#define Raycaster_hpp

#include <vector>
#include "Renderer.hpp"

class Raycaster : public Renderer
{
public:
    Raycaster(Graphics& g,
              const Camera& camera,
              const std::vector<Wall>& walls,
              const std::vector<Line>& worldBounds,
              const std::vector<Surface>& textures);
    ~Raycaster() = default;

    void RenderScene() override;
    void RenderMap() override;

private:
    double getPerpendicularDistanceFromCamera(const Vec2& point, double percentWidth);
    
    const std::vector<Wall>& walls;
};

#endif /* Raycaster_hpp */
