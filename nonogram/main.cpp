
#include <iostream>
#include <SDL2/SDL.h>
#include <fstream>
#include <string>

#include "SDL_Utils.hpp"
#include "GameScreen.hpp"

int level = 1;
int mainRows, mainCols, headerRows, sideCols, blank;
char **mainTable = nullptr;
int **headerTable = nullptr, **sideTable = nullptr;
bool isWin = false;
bool is_lvl_loaded_failed = true;
bool is_data_loaded_failed = true;

void chooseLevel();
void loadLevel(int lvl);
void loadData(char **mainTable, int **headerTable, int **sideTable);

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
    
    //begin---should i put 'init table' here?----------------
    mainTable = new char *[mainRows];
    for (int i = 0; i < mainRows; i++)
        mainTable[i] = new char[mainCols];
    
    headerTable = new int *[headerRows];
    for (int i = 0; i < headerRows; i++)
        headerTable[i] = new int[mainCols];
    
    sideTable = new int *[mainRows];
    for (int i = 0; i < mainRows; i++)
        sideTable[i] = new int[sideCols];
    //end-----should i put 'init table' here?----------------
    
    loadData(mainTable, headerTable, sideTable);
    
    for (int i = 0; i < mainRows; i++)
    {
        for (int k = 0; k < mainCols; k++)
        {
            std::cout << mainTable[i][k] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    for (int i = 0; i < headerRows; i++)
        {
            for (int k = 0; k < mainCols; k++)
            {
                std::cout << headerTable[i][k] << " ";
            }
            std::cout << std::endl;
        }
    std::cout << std::endl;
    
    for (int i = 0; i < mainRows; i++)
    {
        for (int k = 0; k < sideCols; k++)
        {
            std::cout << sideTable[i][k] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    //begin---should i put 'del table' here?----------------
    for (int i = 0; i < mainRows; i++)
        delete[] mainTable[i];
    delete [] mainTable;

    for (int i = 0; i < headerRows; i++)
        delete[] headerTable[i];
    delete [] headerTable;

    for (int i = 0; i < mainRows; i++)
        delete[] sideTable[i];
    delete [] sideTable;
    //end-----should i put 'del table' here?----------------
    
    
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
    std::string pathProp = "/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/levels/level-"+std::to_string(level)+"-prop.txt";
    std::ifstream pFile(pathProp);
    if (pFile.is_open())
    {
        pFile >> mainRows >> mainCols >> headerRows >> sideCols >> blank;
        std::cout << std::endl << mainRows << std::endl << mainCols
                  << std::endl << headerRows << std::endl << sideCols
                  << std::endl << blank << std::endl;
    }
    pFile.close();
    is_lvl_loaded_failed = false;
    delLevelMouse();
    showBack();
}

void loadData(char **mainTable, int **headerTable, int **sideTable)
{
    std::string path = "/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/levels/level-"+std::to_string(level)+".txt";
    std::ifstream file(path);
    if (file.is_open())
    {
        int temp;
        file >> temp >> temp;
        for (int i = 0; i < mainRows; i++)
        {
            for (int k = 0; k < mainCols; k++)
            {
                file >> mainTable[i][k];
            }
        }
        for (int i = 0; i < headerRows; i++)
        {
            for (int k = 0; k < mainCols; k++)
            {
                file >> headerTable[i][k];
            }
        }
        for (int i = 0; i < mainRows; i++)
        {
            for (int k = 0; k < sideCols; k++)
            {
                file >> sideTable[i][k];
            }
        }
    }
    file.close();
}
