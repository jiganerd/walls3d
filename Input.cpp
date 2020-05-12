//
//  Input.cpp
//  engine3d
//
//  Created by Brian Dolan on 4/26/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#include "Input.hpp"
#include <SDL2/SDL.h>

bool Input::ProcessKeys()
{
    SDL_Event e;
    bool altPressed = false;
    tabFirstPressed = false;

    // TODO: this occasionally throws an exception on OSX...
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            return true;
        }
        else if ((e.type == SDL_KEYDOWN) || (e.type == SDL_KEYUP))
        {
            bool keyDown = (e.type == SDL_KEYDOWN);
            switch (e.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    return true;
                    break;

                case SDLK_LALT:
                    altPressed = keyDown;
                    
                    // switch between rotating and strafing when alt is pressed or released
                    if (altPressed)
                    {
                        if (rotateLeft)
                        {
                            rotateLeft = false;
                            strafeLeft = true;
                        }
                        if (rotateRight)
                        {
                            rotateRight = false;
                            strafeRight = true;
                        }
                    }
                    else
                    {
                        if (strafeLeft)
                        {
                            strafeLeft = false;
                            rotateLeft = true;
                        }
                        if (strafeRight)
                        {
                            strafeRight = false;
                            rotateRight = true;
                        }
                    }
                    break;

                case SDLK_UP:
                    moveForward = keyDown;
                    break;

                case SDLK_DOWN:
                    moveBackward = keyDown;
                    break;

                case SDLK_LEFT:
                    if (keyDown)
                    {
                        if (altPressed)
                            strafeLeft = true;
                        else
                            rotateLeft = true;
                    }
                    else
                    {
                        rotateLeft = false;
                        strafeLeft = false;
                    }
                    break;

                case SDLK_RIGHT:
                    if (keyDown)
                    {
                        if (altPressed)
                            strafeRight = true;
                        else
                            rotateRight = true;
                    }
                    else
                    {
                        rotateRight = false;
                        strafeRight = false;
                    }
                    break;

                case SDLK_LSHIFT:
                    shiftPressed = keyDown;
                    break;
                    
                case SDLK_TAB:
                    tabFirstPressed = keyDown;
                    break;
                    
                default:
                    break;
            }
        }
    }

    return false;
}