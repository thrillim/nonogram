
#include <iostream>
#include <SDL2/SDL.h>
#include <fstream>
#include <string>

#include "SDL_Utils.hpp"
#include "GameScreen.hpp"

int level = 1;
int mainRows, mainCols, headerRows, sideCols, blank;
int totalRows, totalCols, cellSide, originX, originY;
char **mainTable = nullptr;
int **headerTable = nullptr, **sideTable = nullptr;
int clickRow, clickCol, score;

bool isUpdated = true;
bool isWin = false;
bool is_lvl_loaded_failed = true;

void chooseLevel();
void loadLevel(int lvl);
void loadData();
void renderTable();
void playing();

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
    
    loadData();
    
    do
    {
        renderTable();
        playing();
    }
    while (isWin == false);

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
    
    
    renderScreen();
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
    std::cout << lvl << std::endl;
    std::string pathProp = "/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/levels/level-"+std::to_string(level)+"-prop.txt";
    std::ifstream pFile(pathProp);
    if (pFile.is_open())
    {
        pFile >> mainRows >> mainCols >> headerRows >> sideCols >> blank;
    }
    score = blank;
    pFile.close();
    is_lvl_loaded_failed = false;
    delLevelMouse();
}

void loadData()
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

void renderTable()
{
    showBack();
    totalCols = sideCols + mainCols;
    totalRows = headerRows + mainRows;
    int cellHeight = MAX_TABLE_HEIGHT/totalRows;
    int cellWidth = MAX_TABLE_WIDTH/totalCols;
    cellSide = (cellHeight > cellWidth) ? cellWidth : cellHeight;
    originX = SCREEN_WIDTH/2 - cellSide * (float)totalCols/2;
    originY = SCREEN_HEIGHT/2 - cellSide * (float)totalRows/2;
    
    for (int i = 1; i <= totalRows + 1; i++) //draw horizontal lines
    {
        showLine(originX, originY + (i-1)*cellSide, cellSide*totalCols+1, 2);
        if ((i == 1) || (i - headerRows - 1) % 5 == 0)
        {
            showLine(originX, originY + (i-1)*cellSide, cellSide*totalCols+2, 3);
        }
    }
    for (int i = 1; i <= totalCols + 1; i++) //draw vertical lines
    {
        showLine(originX + (i-1)*cellSide, originY, 2, cellSide*totalRows+1);
        if ((i == 1) || (i - sideCols - 1) % 5 == 0)
        {
            showLine(originX + (i-1)*cellSide, originY, 3, cellSide*totalRows+2);
        }
    }
    for (int i = 0; i < headerRows; i++)
    {
        for (int k = 0; k < mainCols; k++)
        {
            if (headerTable[i][k] != 0)
            {
                showNumOff(headerTable[i][k], originX + (k+sideCols)*cellSide + 1, originY + i*cellSide + 1, cellSide-1, cellSide-1);
            }
        }
    }
    for (int i = 0; i < mainRows; i++)
    {
        for (int k = 0; k < sideCols; k++)
        {
            if (sideTable[i][k] != 0)
            {
                showNumOff(sideTable[i][k], originX + k*cellSide + 1, originY + (headerRows + i)*cellSide + 1, cellSide-1, cellSide-1);
            }
        }
    }
    for (int i = 0; i < headerRows; i++)
    {
        for (int k = 0; k < mainCols; k++)
        {
            if (headerTable[i][k] != 0)
            {
                showNumOn(headerTable[i][k], originX + (k+sideCols)*cellSide + 1, originY + i*cellSide + 1, cellSide-1, cellSide-1);
            }
        }
    }
    for (int i = 0; i < mainRows; i++)
    {
        for (int k = 0; k < sideCols; k++)
        {
            if (sideTable[i][k] != 0)
            {
                showNumOn(sideTable[i][k], originX + k*cellSide + 1, originY + (headerRows + i)*cellSide + 1, cellSide-1, cellSide-1);
            }
        }
    }
    renderScreen();
}

void playing()
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
            if (e.key.keysym.sym == SDLK_r)
            {
                isWin = true;
            }
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if ((e.button.x > originX)&&
                (e.button.y > originY)&&
                (e.button.x < originX + cellSide*totalCols)&&
                (e.button.y < originY + cellSide*totalRows))
                //click inside the table
            {
                clickCol = (e.button.x - originX)/cellSide;
                clickRow = (e.button.y - originY)/cellSide;
                std::cout << clickRow << " " << clickCol << std::endl;
                isUpdated = false;
                if (clickRow < headerRows)
                {
                    if (clickCol < sideCols) //out corner
                    {
                        clickRow = -1;
                        clickCol = -1;
                        std::cout << "out " << clickRow << " " << clickCol << std::endl;
                    }
                    else //header
                    {
                        clickCol -= sideCols;
                        std::cout << "header " << clickRow << " " << clickCol << std::endl;
                    }
                }
                else
                {
                    if (clickCol < sideCols) //side
                    {
                        clickRow -= headerRows;
                        std::cout << "side " << clickRow << " " << clickCol << std::endl;
                    }
                    else //main
                    {
                        clickRow -= headerRows;
                        clickCol -= sideCols;
                        std::cout << "main " << clickRow << " " << clickCol << std::endl;
                    }
                }
            }
        }
    }
//    renderScreen();
}
