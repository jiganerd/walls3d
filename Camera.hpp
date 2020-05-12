//
//  Camera.hpp
//  BspTree
//
//  Created by Brian Dolan on 5/12/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <cmath>
#include "Vec2.hpp"
#include "Mat2.hpp"

class Camera
{
public:
    Camera(const Vec2& location);
    ~Camera() = default;
    
    void rotate(double angleRad);
    void moveForward(double distance);
    void strafe(double distanceToRight);

	const float viewPlaneWidth {5}; // maps to screen width
	const float viewPlaneDist {5};

    Vec2 location;

	// relative to location
	// magnitude represents distance to view plane
	Vec2 dir {0, viewPlaneDist};

	// a vector that starts at the end of the "dir" vector,
	// and is always perpendicular to it
	Vec2 halfViewPlane {-viewPlaneWidth / 2, 0};

	// normalized versions of the above angles, just as an optimization
	Vec2 dirN, halfViewPlaneN;

	// (cached upfront during creation of camera so this doesn't have to be
	// computed over and over in BSP rendering algorithm)
	float leftmostVisibleAngle, rightmostVisibleAngle;
	Vec2 leftmostViewPlaneEnd, rightmostViewPlaneEnd, viewPlaneMiddle;

private:
    void updateNormalizedVectors();
    void updateViewPlaneVectors();
};

#endif /* Camera_hpp */