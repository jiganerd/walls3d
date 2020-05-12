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
    
    static bool IsPointToRightOfLine(const Line& l, const Vec2& p);
    static bool IsLineSegToRightOfLine(const Line& line, const Line& lineSeg);
    
    static bool FindRayLineSegIntersection(const Line& ray, const Line& seg, Vec2& intersection, double& u);
    static bool FindLineLineSegIntersection(const Line& line, const Line& seg, Vec2& intersection, double& t, double& u);
    static bool FindRayLineIntersection(const Line& ray, const Line& line, Vec2& intersection, double& t);
    
private:
    static bool FindRayLineSegIntersection(bool rayIsActuallyLine, bool lineSegIsActuallyLine, const Line& ray,
                                           const Line& seg, Vec2& intersection, double& t, double& u);
};

#endif /* GeomUtils_hpp */
