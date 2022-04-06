
#include <iostream>
#include <SDL2/SDL.h>

#include "SDL_Utils.hpp"
//*********************************
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;
//*********************************


int main(int argc, char* argv[])
{
    
    SDL_Window* window = nullptr; //window render to
    SDL_Surface* windowSurface = nullptr; //surface contained by the window
    SDL_Renderer* renderer = nullptr;
    
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, "Nonogram");

    
    //pop-up-fixed-------------------
    bool is_running = true;
    SDL_Event event;
    while (is_running)
    {
        while (SDL_PollEvent(&event)) //Wait indefinitely for the next available event
        {
            if (event.type == SDL_QUIT)
            {
                is_running = false;
            }
        }
        SDL_Delay(16);
    }
    //pop-up-fixed-------------------
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
