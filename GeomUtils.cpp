//
//  GeomUtils.cpp
//  walls3d
//
//  Created by Brian Dolan on 5/11/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include <cmath>
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
    return FindRayLineSegIntersection(false, true, ray, line, intersection, t, dummy);
}

bool GeomUtils::FindLineLineIntersection(const Line& l1, const Line& l2, Vec2& intersection)
{
    bool intersectionFound {true};
    
    // handle undefined slope for line 1
    if (l1.p1.x == l1.p2.x)
    {
        // parallel, possibly overlapping, lines - no clear intersection
        if (l2.p1.x == l2.p2.x)
        {
            intersectionFound = false;
        }
        else
        {
            // we cannot calculate the (undefined) slope, so do something else
            intersection.x = l1.p1.x;
            
            // get l2 into the form: y = m2*x + b2
            double m2 {GetSlope(l2)};
            double b2 {GetYIntercept(m2, l2.p1)};
            
            // y = mx + b for line 2
            intersection.y = m2 * intersection.x + b2;
        }
    }
    // handle undefined slope for line 1
    else if (l2.p1.x == l2.p2.x)
    {
        // (note that we've already checked the case for both line 1 and line 2
        // being parallel lines)
        
        // we cannot calculate the (undefined) slope, so do something else
        intersection.x = l2.p1.x;
        
        // get l1 into the form: y = m1*x + b1
        double m1 {GetSlope(l1)};
        double b1 {GetYIntercept(m1, l1.p1)};
        
        // y = mx + b for line 1
        intersection.y = m1 * intersection.x + b1;
    }
    // handle the regular case
    else
    {
        // get l1 into the form: y = m1*x + b1
        double m1 {GetSlope(l1)};
        double b1 {GetYIntercept(m1, l1.p1)};
        
        // get l2 into the form: y = m2*x + b2
        double m2 {GetSlope(l2)};
        double b2 {GetYIntercept(m2, l2.p1)};
        
        // parallel, possibly overlapping, lines - no clear intersection
        if (m1 == m2)
        {
            intersectionFound = false;
        }
        else
        {
            // find the intersection of the lines with this equation:
            //           y1 = y2
            //    m1*x + b1 = m2*x + b2 (and there is no distinction between x1 and x2)
            //  m1*x - m2*x = b2 - b1
            //            x = (b2 - b1) / (m1 - m2)
            intersection.x = (b2 - b1) / (m1 - m2);
            // now plugging in x for line 1: y = m1*x + b1
            intersection.y = m1 * intersection.x + b1;
        }
    }
    
    return intersectionFound;
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
    bool intersectionFound {false};
    
    // (see ray/line segment intersection notes in my "engineering notebook"
    // for the math here...)
    
    Vec2 r {ray.p2 - ray.p1};
    Vec2 s {seg.p2 - seg.p1};
    
    // t = ((seg.p1 - ray.p1) x s) / (r x s)
    // u = ((seg.p1 - ray.p1) x r) / (r x s)
    
    // just some things that appear in both equations
    // and can be calculated just once:
    Vec2 segStartMinusRayStart {seg.p1 - ray.p1};
    double rCrossS {r.cross(s)};
    
    if (fabs(rCrossS) > fpNegligible)
    {
        t = segStartMinusRayStart.cross(s) / rCrossS;
        u = segStartMinusRayStart.cross(r) / rCrossS;
        
        // for a ray/line segment intersection, the following must be
        // true (see notes)
        // 0 <= t, 0 <= u <= 1
        if (rayIsActuallyLine || (t > fpNegligible)) // ...but don't intersect at the very base of the ray
        {
            if (lineSegIsActuallyLine || ((u >= 0.0f) && (u <= 1.0f)))
            {
                intersectionFound = true;
                
                // now, figure out the location of the intersection
                // by plugging in t to the original equation
                intersection = ray.p1 + (r * t);
            }
        }
    }
    // else parallel lines
    
    return intersectionFound;
}

double GeomUtils::GetSlope(const Vec2& a, const Vec2& b)
{
    return ((b.y - a.y) / (b.x - a.x));
}

double GeomUtils::GetSlope(const Line& l)
{
    return GetSlope(l.p1, l.p2);
}

double GeomUtils::GetYIntercept(double slope, const Vec2& a)
{
    return (a.y - slope * a.x);
}