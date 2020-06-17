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
#include <functional>
#include <memory>
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
        bool extendedLinesValid;
        Line extendedLineForMapDrawingForward;
        Line extendedLineForMapDrawingBackward;
    };

    using TraversalCbType = std::function<void(const Wall&, const BspNodeDebugInfo&)>;

private:
    class BspNode
    {
    public:
        BspNode(BspTree* pOwnerTree, const Wall& wall, const std::vector<Wall>& surroundingWalls,
                const std::vector<Line>& sectionBounds, uint32_t index);
        BspNode(BspTree* pOwnerTree, const uint8_t* bytes, size_t& offset, uint32_t index);
        ~BspNode() = default;
        
        uint32_t GetIndex() { return debugInfo.nodeIndex; }
        void Print();
        void ExportPrint();
        int32_t Find(const Vec2& p);
        void TraverseRender(const Vec2& cameraLoc, TraversalCbType renderFunc);
        void TraverseDebug(TraversalCbType debugFunc);

    private:
        void ExtendMapLineToSectionBounds(const std::vector<Line>& sectionBounds);

        BspTree* pOwnerTree;
        Wall wall;
        BspNodeDebugInfo debugInfo;
        std::unique_ptr<BspNode> pBackNode;
        std::unique_ptr<BspNode> pFrontNode;
    };

public:
    BspTree();
    ~BspTree() = default;

    void ProcessWalls(const std::vector<Wall>& walls, const std::vector<Line>& sectionBounds);
    void LoadBin(const uint8_t* bytes);

    void Print();
    void ExportPrint();
    int32_t Find(const Vec2& p);
    void TraverseRender(const Vec2& cameraLoc, TraversalCbType renderFunc);
    void TraverseDebug(TraversalCbType debugFunc);

private:
    // for "compiling" tree
    std::unique_ptr<BspNode> CreateNode(const std::vector<Wall>& walls, const std::vector<Line>& sectionBounds);
    static void SplitWalls(const Line& splitterLine, const std::vector<Wall>& walls, std::vector<Wall>& backWalls, std::vector<Wall>& frontWalls);
    static size_t FindBestSplitterWallIndex(const std::vector<Wall>& walls);
    
    // for loading from a binary
    std::unique_ptr<BspNode> ParseNode(const uint8_t* bytes, size_t& offset);
    
    void ExportPrintNode(const std::unique_ptr<BspNode>& pNode);
    
    uint32_t numNodes;
    std::unique_ptr<BspNode> pRootNode;

    static const std::vector<Color> mapColors;
    
    // this value represents a "null node" in the serialized data, and is chosen to
    // not conflict with any reasonable value for what is otherwise a fixed-point
    // representation of an x coordinate in the case of a "real" node
    static constexpr int32_t SerNullNode {static_cast<int32_t>(0x7FFFFFFF)};
};

#endif /* BspTree_hpp */
