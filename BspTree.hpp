//
//  BspTree.hpp
//  walls3d
//
//  Created by Brian Dolan on 5/9/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

// TODO:
// add split function to Wall class so it can handle texture splitting, etc.
// could even templatize on wall and point types (e.g. for expanding to real 3d)

#ifndef BspTree_hpp
#define BspTree_hpp

#include <vector>
#include "Vec2.hpp"
#include "Wall.hpp"
#include "Color.hpp"

class BspTree
{
public:
    class BspNodeDebugInfo
    {
    public:
        BspNodeDebugInfo(uint32_t nodeIndex);
        void ExtendMapLineToSectionBounds(const Line& line, const std::vector<Line>& sectionBounds);
        
        uint32_t nodeIndex;
        Color mapColor;
        Line extendedLineForMapDrawingForward;
        Line extendedLineForMapDrawingBackward;
    };

    using RenderFuncType = std::function<void(const Wall&)>;
    using DebugFuncType = std::function<void(const Wall& wall, const BspNodeDebugInfo&)>;

private:
    class BspNode
    {
    public:
        BspNode(BspTree* pOwnerTree, const Wall& wall, const std::vector<Wall>& surroundingWalls,
                const std::vector<Line>& sectionBounds, uint32_t index);
        ~BspNode() = default;
        
        int GetIndex() { return debugInfo.nodeIndex; }
        void Print();
        int32_t Find(const Vec2& p);
        void TraverseRender(const Vec2& cameraLoc);
        void TraverseDebug(DebugFuncType debugFunc);

    private:
        void ExtendMapLineToSectionBounds(const std::vector<Line>& sectionBounds);

        Wall wall;
        std::unique_ptr<BspNode> pBackNode {nullptr};
        std::unique_ptr<BspNode> pFrontNode {nullptr};
        BspNodeDebugInfo debugInfo;
        
        BspTree* pOwnerTree;
    };

public:
    BspTree(const std::vector<Wall>& walls, const std::vector<Line>& sectionBounds, RenderFuncType renderFunc);
    ~BspTree() = default;
    
    void Print();
    int32_t Find(const Vec2& p);
    void TraverseRender(const Vec2& cameraLoc);
    void TraverseDebug(DebugFuncType debugFunc);
    
private:
    // for "compiling" tree
    std::unique_ptr<BspNode> CreateNode(const std::vector<Wall>& walls, const std::vector<Line>& sectionBounds);
    static void SplitWalls(const Line& splitterLine, const std::vector<Wall>& walls, std::vector<Wall>& backWalls, std::vector<Wall>& frontWalls);
    static size_t FindBestSplitterWallIndex(const std::vector<Wall>& walls);
    
    uint32_t numNodes;
    std::unique_ptr<BspNode> pRootNode;
    RenderFuncType renderFunc;
    
    static const std::vector<Color> mapColors;
};

#endif /* BspTree_hpp */
