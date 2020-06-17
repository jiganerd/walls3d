//
//  SDLHeader.hpp
//  walls3d
//
//  Created by Brian Dolan on 6/16/20.
//  Copyright © 2020 Brian Dolan. All rights reserved.
//

#ifndef SDLHeader_hpp
#define SDLHeader_hpp

#define SDL_MAIN_HANDLED

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SDL.h>
#elif defined(__APPLE__)
#include <SDL2/SDL.h>
#else
#error currently unsupported environment
#endif

#endif /* SDLHeader_hpp */