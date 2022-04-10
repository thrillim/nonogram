
#include "GameScreen.hpp"
#include "SDL_Utils.hpp"
#include "GameConstants.h"

const char WINDOW_TITLE[] = "Nonogram";
const int SCREEN_WIDTH=960, SCREEN_HEIGHT=640;
const int TABLE_X0=214, TABLE_Y0=53, TABLE_X1=748, TABLE_Y1=587;
const int TABLE_ROW_NUM=13, TABLE_COL_NUM=13;

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Texture *mainMenu, *playGround;
SDL_Texture *dot, *fill, *achive;
SDL_Texture *numberOn[MAX_NUM+1], *numberOff[MAX_NUM+1];

void load_SDL_And_Images()
{
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    mainMenu = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/main.png", renderer);
    playGround = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/playGround.png", renderer);
    dot = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/dot.png", renderer);
    fill = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/fill.png", renderer);
    achive = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/achieve.png", renderer);
    
    bool is_load_num_on_failed = false;
    bool is_load_num_off_failed = false;
    for (int i=1; i<=MAX_NUM; i++)
    {
        numberOn[i] = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/" +std::to_string(i) + "-on.png", renderer);
        
        if (numberOn[i] == nullptr)
        {
            is_load_num_on_failed = true;
            break;
        }
        
        numberOff[i] = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/" +std::to_string(i) + "-off.png", renderer);
        
        if (numberOff[i] == nullptr)
        {
            is_load_num_off_failed = true;
            break;
        }
    }
    
    if (mainMenu == nullptr || playGround == nullptr || dot == nullptr || fill == nullptr || achive == nullptr || is_load_num_on_failed == true || is_load_num_off_failed == true)
    {
        unload_SDL_And_Images();
        exit(1);
    }
}

void unload_SDL_And_Images()
{
    SDL_DestroyTexture(mainMenu);
    SDL_DestroyTexture(playGround);
    SDL_DestroyTexture(fill);
    SDL_DestroyTexture(dot);
    SDL_DestroyTexture(achive);
    for (int i = 0; i <= MAX_NUM; i++)
    {
        SDL_DestroyTexture(numberOn[i]);
        SDL_DestroyTexture(numberOff[i]);
    }
    quitSDL(window, renderer);
}

void showMainMenu()
{
    renderTexture(mainMenu, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void renderScreen()
{
    SDL_RenderPresent(renderer);
}


