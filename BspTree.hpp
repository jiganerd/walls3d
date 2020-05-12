//
//  BspTree.hpp
//  walls3d
//
//  Created by Brian Dolan on 5/9/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

// TODO:
// right/left terminology -> front/back?
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
private:
    class BspNode
    {
    public:
        BspNode(BspTree* pOwnerTree, const Wall& wall, const std::vector<Wall>& surroundingWalls,
                const std::vector<Line>& sectionBounds, uint32_t index);
        ~BspNode() = default;
        
        int GetIndex() { return index; }
        void Print();
        int32_t Find(const Vec2& p);
        void Render(const Vec2& cameraLoc, bool drawFrontToBack);

    private:
        void ExtendMapLineToSectionBounds(const std::vector<Line>& sectionBounds);
        static Color GenerateRandomColor();

        Wall wall;
        std::unique_ptr<BspNode> pLeftNode {nullptr};
        std::unique_ptr<BspNode> pRightNode {nullptr};
        
        // for debugging
        uint32_t index;
        Color randomColorForMapDrawing;
        Line extendedLineForMapDrawingForward;
        Line extendedLineForMapDrawingBackward;
        
        BspTree* pOwnerTree;
    };

public:
    BspTree(const std::vector<Wall>& walls, const std::vector<Line>& sectionBounds, std::function<void(const Wall&)> renderFunc);
    ~BspTree() = default;
    
    void Print();
    int32_t Find(const Vec2& p);
    void Render(const Vec2& cameraLoc, bool drawFrontToBack = true);
    
private:
    // for "compiling" tree
    std::unique_ptr<BspNode> CreateNode(const std::vector<Wall>& walls, const std::vector<Line>& sectionBounds);
    static void SplitWalls(const Line& splitterLine, const std::vector<Wall>& walls, std::vector<Wall>& leftWalls, std::vector<Wall>& rightWalls);
    static size_t FindBestSplitterWallIndex(const std::vector<Wall>& walls);
    
    std::function<void(const Wall&)> renderFunc;
    std::unique_ptr<BspNode> pRootNode {nullptr};
    int32_t numNodes {0};
};

#endif /* BspTree_hpp */
