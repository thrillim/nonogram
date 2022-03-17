
#include <iostream>
#include <SDL2/SDL.h>

//*********************************
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;
//*********************************


int main(int argc, char* argv[])
{
    SDL_Window* window = nullptr; //window render to
    SDL_Surface* windowSurface = nullptr; //surface contained by the window
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else //initial SDL successfully
    {
        window = SDL_CreateWindow("Nonogram",                  //title
                                  SDL_WINDOWPOS_CENTERED,      //position x
                                  SDL_WINDOWPOS_CENTERED,      //position y
                                  SCREEN_WIDTH, SCREEN_HEIGHT, //size
                                  SDL_WINDOW_SHOWN);           //window is visible
        if (window == NULL)
        {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        }
        else //create window successfully
        {
            windowSurface = SDL_GetWindowSurface(window);
            SDL_FillRect(windowSurface,//select surface
                         NULL,         //select rectangle
                         SDL_MapRGB(   //color
                                    windowSurface->format,
                                    0xEF,0xE1,0xCE)//pantone buttercream
                         );
            SDL_UpdateWindowSurface(window); //reflect changes to portions of the surface
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
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
