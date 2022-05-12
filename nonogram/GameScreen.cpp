
#include "GameScreen.hpp"
#include "SDL_Utils.hpp"
#include "GameConstants.h"

const char WINDOW_TITLE[] = "Nonogram";
const int MENU_PAGE = 1;

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Texture *levelMouse, *insEn, *insVi, *mainMenu[MENU_PAGE+1], *playGround;
SDL_Texture *conEn, *conVi, *saveEn, *saveVi;
SDL_Texture *dot, *fill, *achive, *line;
SDL_Texture *numberOn[MAX_NUM+1], *numberOff[MAX_NUM+1];

void load_SDL_And_Images()
{
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    levelMouse = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/levelMouse.png", renderer);
    
    insEn = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/instruction-en.png", renderer);
    
    insVi = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/instruction-vi.png", renderer);
    
    conEn = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/continue-en.png", renderer);
    
    conVi = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/continue-vi.png", renderer);
    
    saveEn = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/saveProcess-en.png", renderer);
    
    saveVi = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/saveProcess-vi.png", renderer);
    
    bool is_load_main_failed = false;
    for (int i=1; i<=MENU_PAGE; i++)
    {
        mainMenu[i] = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/main" +std::to_string(i) + ".png", renderer);
        
        if (mainMenu[i] == nullptr)
        {
            is_load_main_failed = true;
            break;
        }
    }

    playGround = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/playGround.png", renderer);
    dot = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/dot.png", renderer);
    fill = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/fill.png", renderer);
    achive = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/achieve.png", renderer);
    
    line = loadTexture("/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/assets/line.png", renderer);
    
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
    
    if (playGround == nullptr || dot == nullptr || fill == nullptr || achive == nullptr || is_load_num_on_failed == true || is_load_num_off_failed == true || is_load_main_failed == true)
    {
        unload_SDL_And_Images();
        exit(1);
    }
}

void unload_SDL_And_Images()
{
    SDL_DestroyTexture(playGround);
    SDL_DestroyTexture(fill);
    SDL_DestroyTexture(dot);
    SDL_DestroyTexture(achive);
    SDL_DestroyTexture(line);
    for (int i = 0; i <= MENU_PAGE; i++)
    {
        SDL_DestroyTexture(mainMenu[i]);
    }
    for (int i = 0; i <= MAX_NUM; i++)
    {
        SDL_DestroyTexture(numberOn[i]);
        SDL_DestroyTexture(numberOff[i]);
    }
    quitSDL(window, renderer);
}

void showMainMenu()
{
    renderTexture(mainMenu[1], renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void showLevelMouse(int lvl)
{
    if (lvl < 5)
    {
        renderTexture(levelMouse, renderer, 381+(lvl-1)*134, 245, 138, 50);
    }
    else
    {
        renderTexture(levelMouse, renderer, 381+(lvl-5)*134, 448, 138, 50);
    }
}

void showIns(int isEn)
{
    if (isEn == 1)
    {
        renderTexture(insEn, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    if (isEn == 0)
    {
        renderTexture(insVi, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    if (isEn == -1)
    {
        renderTexture(insEn, renderer, 0, 0, 0, 0);
    }
}

void showBack()
{
    renderTexture(playGround, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void showContinue(int isEn)
{
    if (isEn == 1 || isEn == -1)
    {
        renderTexture(conEn, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    if (isEn == 0)
    {
        renderTexture(conVi, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}

void showSaveProcess(int isEn)
{
    if (isEn == 1 || isEn == -1)
    {
        renderTexture(saveEn, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    if (isEn == 0)
    {
        renderTexture(saveVi, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}


void showLine(int x, int y, int w, int h)
{
    renderTexture(line, renderer, x, y, w, h);
}

void showNumOff(int num, int x, int y, int w, int h)
{
    renderTexture(numberOff[num], renderer, x, y, w, h);
}

void showNumOn(int num, int x, int y, int w, int h)
{
    renderTexture(numberOn[num], renderer, x, y, w, h);
}

void showMarked(int x, int y, int w, int h)
{
    renderTexture(fill, renderer, x, y, w, h);
}

void showReward(bool isWin)
{
    if (isWin)
    {
        renderTexture(achive, renderer, SCREEN_WIDTH - PADDING_LEFT - 40, PADDING_TOP, 110, 151);
    }
    else
    {
        renderTexture(achive, renderer, SCREEN_WIDTH - 3/2*PADDING_LEFT, PADDING_TOP, 0, 0);
    }
}

void renderScreen()
{
    SDL_RenderPresent(renderer);
}


