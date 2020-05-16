//
//  GeomUtils.hpp
//  BspTree
//
//  Created by Brian Dolan on 5/11/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef GeomUtils_hpp
#define GeomUtils_hpp

#include "Vec2.hpp"
#include "Line.hpp"

class GeomUtils
{
public:
    GeomUtils() = delete;
    ~GeomUtils() = delete;
    
    static bool IsPointInFrontOfLine(const Line& l, const Vec2& p);
    static bool IsLineSegInFrontOfLine(const Line& line, const Line& lineSeg);
    
    static bool FindRayLineSegIntersection(const Line& ray, const Line& seg, Vec2& intersection, double& u);
    static bool FindLineLineSegIntersection(const Line& line, const Line& seg, Vec2& intersection, double& t, double& u);
    static bool FindRayLineIntersection(const Line& ray, const Line& line, Vec2& intersection, double& t);
    static bool FindLineLineIntersection(const Line& l1, const Line& l2, Vec2& intersection);
    
    static double AngleBetween(const Vec2& v1, const Vec2& v2);
    static double AngleBetweenNorm(const Vec2& v1, const Vec2& v2);
    
private:
    static bool FindRayLineSegIntersection(bool rayIsActuallyLine, bool lineSegIsActuallyLine, const Line& ray,
                                           const Line& seg, Vec2& intersection, double& t, double& u);
    static double GetSlope(const Vec2& a, const Vec2& b);
    static double GetSlope(const Line& l);
    static double GetYIntercept(double slope, const Vec2& a);
};

#endif /* GeomUtils_hpp */
