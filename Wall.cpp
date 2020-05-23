//
//  Wall.cpp
//  walls3d
//
//  Created by Brian Dolan on 5/22/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include "Wall.hpp"

std::ostream& operator<<(std::ostream& s, const Wall& w)
{
    w.Serialize(s);
    return s;
}
