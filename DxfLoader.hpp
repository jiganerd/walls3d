//
//  DxfLoader.hpp
//  walls3d
//
//  Created by Brian Dolan on 5/15/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef DxfLoader_hpp
#define DxfLoader_hpp

#include <memory>
#include "dxflib/dl_dxf.h"
#include "dxflib/dl_creationadapter.h"
#include "Line.hpp"

class DxfLoader
{
public:
    DxfLoader(std::string filename, std::vector<Line>& lines);
    
private:
    class DxfFilter : public DL_CreationAdapter
    {
    public:
        DxfFilter(std::vector<Line>& lines);
        
        virtual void addLine(const DL_LineData& d);
        
    private:
        std::vector<Line>& lines;
    };
    
    DxfFilter f;
    std::unique_ptr<DL_Dxf> pDxf;
};

#endif /* DxfLoader_hpp */
