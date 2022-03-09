
#include <iostream>
#include <SDL2/SDL.h>

//***********************************************

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;

//***********************************************


int main(int argc, char* argv[])
{
    SDL_Window* window = nullptr; //The window we'll be rendering to
    SDL_Surface* windowSurface = nullptr; //The surface contained by the window
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) //Initialize SDL
    {
        std::cout << "SDL could not initialize! SDL_Error: " <<  SDL_GetError() << std::endl;
    }
    else
    {
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == NULL)
        {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            windowSurface = SDL_GetWindowSurface(window);
            
            //Fill the surface white
            SDL_FillRect( windowSurface, NULL, SDL_MapRGB( windowSurface->format, 0xFF, 0xFF, 0xFF ) );
                        
            //Update the surface
            SDL_UpdateWindowSurface( window );
                
            //-----------pop-up-fixed---------------------
            SDL_UpdateWindowSurface(window);
            bool is_running = true;
            SDL_Event event;
            while (is_running)
            {
                while (SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT)
                    {
                        is_running = false;
                    }
                }
                SDL_Delay(16);
            }
            //--------------------------------------------
        }
    }
    
    SDL_DestroyWindow(window); //free memory
    SDL_Quit();
    return 0;
}

//***********************************************


//***********************************************
