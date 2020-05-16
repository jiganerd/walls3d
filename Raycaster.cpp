//
//  Raycaster.cpp
//  BspTree
//
//  Created by Brian Dolan on 5/15/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include <limits>
#include "Raycaster.hpp"
#include "GeomUtils.hpp"

using std::vector;

Raycaster::Raycaster(Graphics& g,
                     const Camera& camera,
                     const vector<Wall>& walls,
                     const vector<Line>& worldBounds,
                     const vector<Surface>& textures):
    Renderer(g, camera, worldBounds, textures),
    walls{walls}
{
}

void Raycaster::RenderScene()
{
    BeginRender();
    
    // do this once, as it is actually a pretty costly call
    static constexpr double infinity {std::numeric_limits<double>::infinity()};

    // -1 to 1 as we draw across the screen
    double percentWidth = -1.0f;
    const double percentWidthIncrement = 2.0f / g.ScreenWidth;
    
    // loop through columns on the screen
    for (uint32_t column = 0; column < g.ScreenWidth; column++)
    {
        // for this current column, figure out the corresponding point
        // on the view plane
        // TODO: optimize performance - don't have to do multiplication here every pass through loop
        Vec2 vectorToRayPoint {camera.halfViewPlane * percentWidth};
        Vec2 viewPlanePoint {camera.viewPlaneMiddle + vectorToRayPoint};

        Line ray {camera.location, viewPlanePoint};
        
        // find intersections with all walls, and find the closest wall
        const Wall* closestWallPtr {nullptr};
        double closestWallDistance {infinity};
        Vec2 closestWallIntersection;
        double closestWallU {0.0f};

        for (const Wall& wall : walls)
        {
            Vec2 intersection;
            double u;
            if (GeomUtils::FindRayLineSegIntersection(ray, wall.seg, intersection, u))
            {
                double distanceToIntersection {getPerpendicularDistanceFromCamera(intersection, percentWidth)};
                
                // draw this wall intersection only if it is closer than any other
                // wall intersections drawn so far
                // (prevent overdrawing)
                // (this method works because all walls are the same height, and
                // closer walls will always obscure farther walls entirely)
                if (distanceToIntersection < closestWallDistance)
                {
                    // save off information for this particular (closest) wall intersection
                    closestWallDistance = distanceToIntersection;
                    closestWallU = u;
                    closestWallPtr = &wall;
                    closestWallIntersection = intersection;
                }
            }
        }
        
        // draw the closest wall
        if (closestWallPtr)
            RenderColumn(column,
                         getColumnHeightByDistance(closestWallDistance, closestWallPtr->height),
                         closestWallPtr->c,
                         closestWallPtr->textureNum,
                         closestWallU);
        
        percentWidth += percentWidthIncrement;
    }
    
    EndRender();
}

void Raycaster::RenderMap()
{
    RenderMapBounds();
    
    // draw walls
    for (const Wall& wall : walls)
        RenderMapLine(wall.seg, wall.c);
    
    RenderMapCamera();
}

double Raycaster::getPerpendicularDistanceFromCamera(const Vec2& point, double percentWidth)
{
    // see my notebook notes for the math here
    // this "perpendicular distance" equation prevents the fisheye effect that
    // would be seen with the Euclidean (real) distance
    
    Vec2 cameraToIntersection {point - camera.location};
    double opp {percentWidth * camera.viewPlaneWidth / 2.0f};
    double adj {camera.viewPlaneDist};
    double ratio {opp / adj};
    return sqrt((cameraToIntersection.x * cameraToIntersection.x + cameraToIntersection.y * cameraToIntersection.y) / (1 + ratio * ratio));
}