//
//  GeomUtils.cpp
//  BspTree
//
//  Created by Brian Dolan on 5/11/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include "GeomUtils.hpp"

// follows this convention:
//
//         * l.p2
//        /|\
/          |
//    back | front
//         |
//         * l.p1
//
// this convention has the benefit of natural "left to right" screen drawing for a line/wall when the
// camera is in "front" of the line/wall
bool GeomUtils::IsPointInFrontOfLine(const Line& l, const Vec2& p)
{
    return ((p - l.p1).cross(l.p2 - l.p1) < 0);
}

bool GeomUtils::IsLineSegInFrontOfLine(const Line& line, const Line& lineSeg)
{
    return (IsPointInFrontOfLine(line, lineSeg.p1) && IsPointInFrontOfLine(line, lineSeg.p2));
}

bool GeomUtils::FindRayLineSegIntersection(const Line& ray, const Line& seg, Vec2& intersection, double& u)
{
    double dummy;
    return FindRayLineSegIntersection(false, false, ray, seg, intersection, dummy, u);
}

bool GeomUtils::FindLineLineSegIntersection(const Line& line, const Line& seg, Vec2& intersection, double& t, double& u)
{
    return FindRayLineSegIntersection(true, false, line, seg, intersection, t, u);
}

bool GeomUtils::FindRayLineIntersection(const Line& ray, const Line& line, Vec2& intersection, double& t)
{
    double dummy;
    return FindRayLineSegIntersection(true, false, ray, line, intersection, t, dummy);
}

double GeomUtils::AngleBetween(const Vec2& v1, const Vec2& v2)
{
    // could also be asin(v1.cross(v2) / (v1.Mag() * v2.Mag()))
    return acos((v1 * v2) / (v1.Mag() * v2.Mag()));
}

// a faster version of angleBetweenVectors, if the vectors have already been normalized
double GeomUtils::AngleBetweenNorm(const Vec2& v1, const Vec2& v2)
{
    // could also be asin(v1 * v2)
    return acos(v1 * v2);
}

// finds the point of intersection of a ray (line w/ starting point and no end), starting
// at ray.p1, and a line segment (line with both starting point and ending point
//
// argument "rayIsAtuallyLine" is kind of a hack - set this to true if you actually
// want to do a line/line segment intersection instead of a ray/line segment intersection
//
// argument "lineSegIsActuallyLine" is kind of a hack - set this to true if you actually
// want to do a ray/line intersection instead of a ray/line segment intersection
bool GeomUtils::FindRayLineSegIntersection(bool rayIsActuallyLine, bool lineSegIsActuallyLine, const Line& ray, const Line& seg, Vec2& intersection, double& t, double& u)
{
    bool intersectionFound = false;
    
    // (see ray/line segment intersection notes in my "engineering notebook"
    // for the math here...)
    
    Vec2 r = ray.p2 - ray.p1;
    Vec2 s = seg.p2 - seg.p1;
    
    // t = ((seg.p1 - ray.p1) x s) / (r x s)
    // u = ((seg.p1 - ray.p1) x r) / (r x s)
    
    // just some things that appear in both equations
    // and can be calculated just once:
    Vec2 segStartMinusRayStart = seg.p1 - ray.p1;
    double rCrossS = r.cross(s);
    
    t = segStartMinusRayStart.cross(s) / rCrossS;
    u = segStartMinusRayStart.cross(r) / rCrossS;
    
    // for a ray/line segment intersection, the following must be
    // true (see notes)
    // 0 <= t, 0 <= u <= 1
    if (rayIsActuallyLine || (t >= 0))
    {
        if (lineSegIsActuallyLine || ((u >= 0) && (u <= 1)))
        {
            intersectionFound = true;
            
            // now, figure out the location of the intersection
            // by plugging in t to the original equation
            intersection = ray.p1 + (r * t);
        }
    }
    
    return intersectionFound;
}