//
//  BspTree.cpp
//  BspTree
//
//  Created by Brian Dolan on 5/9/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include <iostream>
#include <random>
#include <cmath>
#include "BspTree.hpp"
#include "GeomUtils.hpp"

using std::vector;
using std::cout;
using std::endl;
using std::unique_ptr;

BspTree::BspNodeDebugInfo::BspNodeDebugInfo(uint32_t nodeIndex):
    nodeIndex(nodeIndex),
    randomColorForMapDrawing{Color(64 + rand() % 192, rand() % 192, 64 + rand() % 192)}
{
}

// the order of sectionBounds is important - will be traversed in reverse order for the
// first (!) intersections
void BspTree::BspNodeDebugInfo::ExtendMapLineToSectionBounds(const Line& line, const vector<Line>& sectionBounds)
{
    bool forwardIntersectionFound {false};
    bool backwardIntersectionFound {false};
    for (auto rit = sectionBounds.rbegin(); rit != sectionBounds.rend(); rit++)
    {
        const Line& bound {*rit};
        
        Vec2 intersection;
        double t;
        if (GeomUtils::FindRayLineIntersection(line, bound, intersection, t))
        {
            if (t > 0)
            {
                if (!forwardIntersectionFound)
                {
                    forwardIntersectionFound = true;
                    extendedLineForMapDrawingForward = {line.p2, intersection};
                }
            }
            else// if (t <= 0)
            {
                if (!backwardIntersectionFound)
                {
                    backwardIntersectionFound = true;
                    extendedLineForMapDrawingBackward = {line.p1, intersection};
                }
            }
            
            if (forwardIntersectionFound && backwardIntersectionFound) break;
        }
    }
}

BspTree::BspNode::BspNode(BspTree* pOwnerTree, const Wall& wall, const std::vector<Wall>& surroundingWalls,
                          const std::vector<Line>& sectionBounds, uint32_t index):
    wall{wall},
    pOwnerTree{pOwnerTree},
    debugInfo{BspNodeDebugInfo(index)}
{
    // use the section boundaries for drawing/debugging the BSP tree on a map
    debugInfo.ExtendMapLineToSectionBounds(wall.seg, sectionBounds);
    
    // append the current wall's line to the boundaries of the children nodes
    vector<Line> sectionBoundsForChildren = sectionBounds;
    sectionBoundsForChildren.push_back(wall.seg);
    
    // now do the split for realsies
    vector<Wall> backWalls, frontWalls;
    BspTree::SplitWalls(wall.seg, surroundingWalls, backWalls, frontWalls);
    
    // now process any/all walls that are in back
    if (backWalls.size() > 0)
        pBackNode = pOwnerTree->CreateNode(backWalls, sectionBoundsForChildren);
    
    // ... and any/all walls that are in front
    if (frontWalls.size() > 0)
        pFrontNode = pOwnerTree->CreateNode(frontWalls, sectionBoundsForChildren);
}

void BspTree::BspNode::Print()
{
    cout << "Node: " << GetIndex() << " ";
    if (pBackNode)
        cout << "Back: " << pBackNode->GetIndex() << " ";
    if (pFrontNode)
        cout << "Front: " << pFrontNode->GetIndex() << " ";
    cout << endl;
    
    if (pBackNode)
        pBackNode->Print();
    if (pFrontNode)
        pFrontNode->Print();
}

int32_t BspTree::BspNode::Find(const Vec2& p)
{
    if (GeomUtils::GeomUtils::IsPointInFrontOfLine(wall.seg, p))
        return (pFrontNode ? pFrontNode->Find(p) : -1);
    else
        return (pBackNode ? pBackNode->Find(p) : -1);
}

void BspTree::BspNode::Render(const Vec2& cameraLoc, bool drawFrontToBack)
{
    // if this is a leaf node, render this node's wall
    if (!pBackNode && !pFrontNode)
    {
        pOwnerTree->renderFunc(wall);
    }
    // if this is not a leaf, and the camera is in front of this node's wall,
    // render the nodes in front
    // TODO: clean up logic / do culling here?
    else if (GeomUtils::GeomUtils::IsPointInFrontOfLine(wall.seg, cameraLoc))
    {
        if (!drawFrontToBack)
        {
            if (pBackNode) pBackNode->Render(cameraLoc, drawFrontToBack);
            pOwnerTree->renderFunc(wall);
            if (pFrontNode) pFrontNode->Render(cameraLoc, drawFrontToBack);
        }
        else
        {
            if (pFrontNode) pFrontNode->Render(cameraLoc, drawFrontToBack);
            pOwnerTree->renderFunc(wall);
            if (pBackNode) pBackNode->Render(cameraLoc, drawFrontToBack);
        }
    }
    // ... or in back
    else
    {
        if (!drawFrontToBack)
        {
            if (pFrontNode) pFrontNode->Render(cameraLoc, drawFrontToBack);
            //pOwnerTree->renderFunc(wall); // cull here
            if (pBackNode) pBackNode->Render(cameraLoc, drawFrontToBack);
        }
        else
        {
            if (pBackNode) pBackNode->Render(cameraLoc, drawFrontToBack);
            //pOwnerTree->renderFunc(wall); // cull here
            if (pFrontNode) pFrontNode->Render(cameraLoc, drawFrontToBack);
        }
    }
    
    // TODO: handle if camera is exactly on this node's line
}

void BspTree::BspNode::DebugTraverse(BspTree::DebugFuncType debugFunc)
{
    if (pBackNode) pBackNode->DebugTraverse(debugFunc);
    debugFunc(wall, debugInfo);
    if (pFrontNode) pFrontNode->DebugTraverse(debugFunc);
}

BspTree::BspTree(const vector<Wall>& walls, const vector<Line>& sectionBounds, RenderFuncType renderFunc):
    renderFunc{renderFunc},
    pRootNode{CreateNode(walls, sectionBounds)}
{
}

std::unique_ptr<BspTree::BspNode> BspTree::CreateNode(const vector<Wall>& walls, const vector<Line>& sectionBounds)
{
    size_t bestSplitterWallIndex {FindBestSplitterWallIndex(walls)};
    vector<Wall> wallsWithoutSplitterWall {walls};
    wallsWithoutSplitterWall.erase(wallsWithoutSplitterWall.begin() + bestSplitterWallIndex);
    
    return std::unique_ptr<BspNode>(new BspNode(this, walls[bestSplitterWallIndex], wallsWithoutSplitterWall, sectionBounds, numNodes++));
}

void BspTree::Print()
{
    if (pRootNode)
        pRootNode->Print();
    else
        cout << "Tree empty" << endl;
}

int32_t BspTree::Find(const Vec2& p)
{
    return (pRootNode ? pRootNode->Find(p) : -1);
}

void BspTree::Render(const Vec2& cameraLoc, bool drawFrontToBack)
{
    if (pRootNode)
        pRootNode->Render(cameraLoc, drawFrontToBack);
}

void BspTree::DebugTraverse(BspTree::DebugFuncType debugFunc)
{
    if (pRootNode)
        pRootNode->DebugTraverse(debugFunc);
}

void BspTree::SplitWalls(const Line& splitterLine, const vector<Wall>& walls, vector<Wall>& backWalls, vector<Wall>& frontWalls)
{
    // loop through all walls in the list, other than the splitter wall
    for (const Wall& wall : walls)
    {
        // see if this wall is bisected by the line formed by this node's wall
        Vec2 intersection;
        double t, u;
        if (GeomUtils::FindLineLineSegIntersection(splitterLine, wall.seg, intersection, t, u) == true)
        {
            // split the wall at the intersection, and store the pieces
            
            // copy over most of the properties of the wall
            // into the two pieces
            Wall backWallPiece {wall};
            Wall frontWallPiece {wall};
            
            bool addBackWallPiece {false};
            bool addFrontWallPiece {false};
            
            // modify the pieces to reflect the intersection point
            // if we think of splitterLine as a "directional ray"
            // starting at splitterLine.p1, then the sign of t represents
            // whether the intersection happened in front of the ray or behind it
            // break up wall into backWallPiece and frontWallPiece accordingly
            //
            // if u == 0, this means the line drawn by the current node's wall
            // touches exactly at the starting end of this wall
            //
            // if u == 1, this means the line drawn by the current node's wall
            // touches exactly at the finishing end of this wall
            
            if (u > 0 && u < 1)
            {
                // the wall needs to be broken up into two
                addBackWallPiece = true;
                addFrontWallPiece = true;
                
                if (GeomUtils::IsPointInFrontOfLine(splitterLine, wall.seg.p1))
                {
                    frontWallPiece.seg.p2 = backWallPiece.seg.p1 = intersection;
                }
                else
                {
                    backWallPiece.seg.p2 = frontWallPiece.seg.p1 = intersection;
                }
            }
            else
            {
                // the ray intersects the wall exactly at the wall start
                if (u == 0)
                {
                    // the wall is entirely to one side of the ray
                    // do a test for where the *end* of the wall is
                    if (GeomUtils::IsPointInFrontOfLine(splitterLine, wall.seg.p2))
                        addFrontWallPiece = true;
                    else
                        addBackWallPiece = true;
                }
                // the ray intersects the wall exactly at the wall end
                else // u == 1
                {
                    // the wall is entirely to one side of the ray
                    // do a test for where the *start* of the wall is
                    if (GeomUtils::IsPointInFrontOfLine(splitterLine, wall.seg.p1))
                        addFrontWallPiece = true;
                    else
                        addBackWallPiece = true;
                }
            }
            
            if (addBackWallPiece)
                backWalls.push_back(backWallPiece);
            
            if (addFrontWallPiece)
                frontWalls.push_back(frontWallPiece);
        }
        else
        {
            // if there's no intersection, this wall must lie entirely
            // on one side of the line or the other
            if (GeomUtils::IsLineSegInFrontOfLine(splitterLine, wall.seg))
                frontWalls.push_back(wall);
            else
                backWalls.push_back(wall);
        }
    }
}

size_t BspTree::FindBestSplitterWallIndex(const vector<Wall>& walls)
{
    vector<Wall> wallsWithoutSplitterWall;
    vector<Wall> backWalls, frontWalls;
    size_t bestSplitterWallIndex {0};
    uint32_t bestSplitterWallScore {std::numeric_limits<uint32_t>::max()};
    
    for (size_t i = 0; i < walls.size(); i++)
    {
        backWalls.clear();
        frontWalls.clear();
        
        wallsWithoutSplitterWall = walls;
        wallsWithoutSplitterWall.erase(wallsWithoutSplitterWall.begin() + i);
        
        SplitWalls(walls[i].seg, wallsWithoutSplitterWall, backWalls, frontWalls);
        
        // see 1) how balanced the back/front sides are (good), and 2) how many
        // wall splits occurred (bad)
        // a lower "score" is better
        // we penalize much more harshly for number of splits than we reward for how balanced the tree is - this
        // ratio is something that is done "by feel"
        uint32_t numberOfSplits {static_cast<uint32_t>((backWalls.size() + frontWalls.size()) - wallsWithoutSplitterWall.size()) / 2};
        uint32_t score {abs(static_cast<int32_t>(backWalls.size()) - static_cast<int32_t>(frontWalls.size())) + numberOfSplits * 8};
        
        if (score < bestSplitterWallScore)
        {
            bestSplitterWallScore = score;
            bestSplitterWallIndex = i;
        }
    }
    
    return bestSplitterWallIndex;
}

