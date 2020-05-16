//
//  Camera.cpp
//  BspTree
//
//  Created by Brian Dolan on 5/12/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include <cmath>
#include "Camera.hpp"
#include "Mat2.hpp"
#include "Utils.hpp"
#include "GeomUtils.hpp"

Camera::Camera(const Vec2& location):
    location{location}
{
    // cache the normalized versions of the vectors
    updateNormalizedVectors();
    
    // compute angles of extremities of viewable area
    // angles are defined as positive going to the right of the camera direction,
    // and negative going to the left
    rightmostVisibleAngle = atan((viewPlaneWidth / 2.0f) / viewPlaneDist);
    leftmostVisibleAngle = -rightmostVisibleAngle;
    
    updateViewPlaneVectors();
}

void Camera::rotate(double angleRad)
{
    Mat2 rotMat {Mat2::Rot(angleRad)};
    dir *= rotMat;
    halfViewPlane *= rotMat;
    updateNormalizedVectors();
    updateViewPlaneVectors();
};

void Camera::moveForward(double distance)
{
    location += (dirN * distance);
    updateViewPlaneVectors();
}

void Camera::strafe(double distanceToRight)
{
    location += (halfViewPlaneN * distanceToRight);
    updateViewPlaneVectors();
}

bool Camera::IsBehind(const Vec2 &p) const
{
    return GeomUtils::IsPointInFrontOfLine({location, location + halfViewPlane}, p);
}

bool Camera::IsBehind(const Line &l) const
{
    return (IsBehind(l.p1) && IsBehind(l.p2));
}

void Camera::updateNormalizedVectors()
{
    dirN = dir.Norm();
    halfViewPlaneN = halfViewPlane.Norm();
}

void Camera::updateViewPlaneVectors()
{
    viewPlaneMiddle = location + dir;
    leftmostViewPlaneEnd = viewPlaneMiddle - halfViewPlane;
    rightmostViewPlaneEnd = viewPlaneMiddle + halfViewPlane;
}