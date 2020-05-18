//
//  Camera.hpp
//  BspTree
//
//  Created by Brian Dolan on 5/12/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include "Vec2.hpp"
#include "Line.hpp"

//              * location  -
//             /|\          ^
//            / | \         |
//           /  |  \        | viewPlaneDist
//          /   |   \       |
//         /    |    \      |
//        /     |dir  \     v
//       <-----*v------     -
// halfViewPlane
//
//       |<----------->|
//       viewPlaneWidth
//
class Camera
{
public:
    Camera(const Vec2& location);
    ~Camera() = default;
    
    void Rotate(double angleRad);
    void MoveForward(double distance);
    void Strafe(double distanceToRight);
    bool IsBehind(const Vec2& p) const;
    bool IsBehind(const Line& l) const;

	const double viewPlaneWidth {5.0f}; // maps to screen width
	const double viewPlaneDist {5.0f};

    Vec2 location;

	// relative to location
	// magnitude represents distance to view plane
	Vec2 dir {0.0f, viewPlaneDist};

	// a vector that starts at the end of the "dir" vector,
	// and is always perpendicular to it
	Vec2 halfViewPlane {-viewPlaneWidth / 2.0f, 0.0f};

	// normalized versions of the above angles, just as an optimization
	Vec2 dirN, halfViewPlaneN;

	// (cached upfront during creation of camera so this doesn't have to be
	// computed over and over in BSP rendering algorithm)
	double leftmostVisibleAngle, rightmostVisibleAngle;
	Vec2 leftmostViewPlaneEnd, rightmostViewPlaneEnd, viewPlaneMiddle;

private:
    void UpdateNormalizedVectors();
    void UpdateViewPlaneVectors();
};

#endif /* Camera_hpp */