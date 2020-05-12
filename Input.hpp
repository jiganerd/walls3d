//
//  Input.hpp
//  engine3d
//
//  Created by Brian Dolan on 4/26/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef Input_hpp
#define Input_hpp

// abstracts the library for keyboard processing
// decodes keyboard input to simple first-person-shooter-like actions
// also processes other general-purpose key presses
class Input
{
public:
    Input() = default;
    ~Input() = default;
    bool ProcessKeys();
    bool GetMoveForward() { return moveForward; }
    bool GetMoveBackward() { return moveBackward; }
    bool GetRotateLeft() { return rotateLeft; }
    bool GetRotateRight() { return rotateRight; }
    bool GetStrafeLeft() { return strafeLeft; }
    bool GetStrafeRight() { return strafeRight; }
    bool GetShiftPressed() { return shiftPressed; }
    bool GetTabFirstPressed() { return tabFirstPressed; }
    
private:
    bool moveForward = false;
    bool moveBackward = false;
    bool rotateLeft = false;
    bool rotateRight = false;
    bool strafeLeft = false;
    bool strafeRight = false;
    bool shiftPressed = false;
    bool tabFirstPressed = false;
};

#endif /* Input_hpp */
