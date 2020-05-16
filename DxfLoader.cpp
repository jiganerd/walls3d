//
//  DxfLoader.cpp
//  BspTree
//
//  Created by Brian Dolan on 5/15/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include <iostream>
#include "DxfLoader.hpp"

DxfLoader::DxfFilter::DxfFilter(std::vector<Line>& lines):
    lines{lines}
{
}

void DxfLoader::DxfFilter::addLine(const DL_LineData& d)
{
    std::cout << " line: {" << d.x1 << ", " << d.y1 << "}, {" << d.x2 << ", " << d.y2 << "}" << std::endl;
    lines.push_back({{static_cast<float>(d.x1), static_cast<float>(d.y1)}, {static_cast<float>(d.x2), static_cast<float>(d.y2)}});
}

DxfLoader::DxfLoader(std::string filename, std::vector<Line>& lines):
    f{lines},
    pDxf{new DL_Dxf()}
{
    std::cout << "Loading " << filename << "..." << std::endl;
    if (!pDxf->in(filename.c_str(), &f))
        std::cerr << " error." << std::endl;
}