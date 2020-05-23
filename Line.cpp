//
//  Line.cpp
//  walls3d
//
//  Created by Brian Dolan on 5/22/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include "Line.hpp"

std::ostream& operator<<(std::ostream& s, const Line& l)
{
    l.Serialize(s);
    return s;
}
