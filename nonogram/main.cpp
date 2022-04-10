
#include <iostream>
#include <SDL2/SDL.h>

#include "SDL_Utils.hpp"
#include "GameScreen.hpp"

int main(int argc, char* argv[])
{
    load_SDL_And_Images();
    showMainMenu();
    renderScreen();
    waitUntilKeyPressed();
    unload_SDL_And_Images();
    return 0;
}
