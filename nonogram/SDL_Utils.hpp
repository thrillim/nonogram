
#ifndef SDL_Utils_hpp
#define SDL_Utils_hpp

#include <iostream>
#include <SDL2/SDL.h>

void logSDLError(std::ostream& os,
                 const std::string &msg,
                 bool fatal);

void initSDL(SDL_Window* &window,
             SDL_Renderer* &renderer,
             int screenWidth, int screenHeight,
             const char* windowTitle);

void quitSDL(SDL_Window* window, SDL_Renderer* renderer);

void waitUntilKeyPressed();

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

#endif /* SDL_Utils_hpp */
