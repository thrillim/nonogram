
#include <iostream>
#include <SDL2/SDL.h>

#include "SDL_Utils.hpp"
#include "GameScreen.hpp"

int level = 1;
bool is_lvl_loaded_failed = true;

void chooseLevel();
void loadLevel(int lvl);


struct Visibility
{
    bool isVisible=false;   //Có đang được hiển thị trên màn hình hay không
    int showTime=-1;        //Thời gian hiển thị nếu đang hiển thị. Nếu <=0: hiển thị không thời hạn
    int waitTime=-1;        //Thời gian đợi để hiển thị nếu đang ẩn. Nếu <=0: ẩn không thời hạn
};

struct ScreenVisibility
{
    Visibility vStartMsg, vHumanMsg, vHumanNumber;
    Visibility vComputerMsg, vComputerComparision, vJustClicked;
    bool isUpToDate=false;
};

int main(int argc, char* argv[])
{
    load_SDL_And_Images();
    
    do {
    showMainMenu();
    chooseLevel();
    }
    while (is_lvl_loaded_failed);
    
//    showBack();
    waitUntilKeyPressed();
    unload_SDL_And_Images();
    return 0;
}

void chooseLevel()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if ((e.type == SDL_QUIT) ||
            (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
        {
            unload_SDL_And_Images();
            exit(1);
        }
        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_RETURN)
            {
                loadLevel(level);
                break;
            }
            if (e.key.keysym.sym == SDLK_LEFT && level != 1 && level != 5)
            {
                level--;
            }
            if (e.key.keysym.sym == SDLK_RIGHT && level != 4 && level != 8)
            {
                level++;
            }
            if (e.key.keysym.sym == SDLK_UP && level > 4)
            {
                level -= 4;
            }
            if (e.key.keysym.sym == SDLK_DOWN && level < 5)
            {
                level += 4;
            }
        }
    }
    showLevelMouse(level);
    renderScreen();
}

void loadLevel(int lvl)
{
    std::cout << lvl;
    is_lvl_loaded_failed = false;
    delLevelMouse();
    showBack();
}
