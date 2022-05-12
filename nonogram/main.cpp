
#include <iostream>
#include <SDL2/SDL.h>
#include <fstream>
#include <string>

#include "SDL_Utils.hpp"
#include "GameScreen.hpp"

int level = 1;
int isEn = -1, isEnTmp = -1;
int mainRows, mainCols, headerRows, sideCols, blank;
int totalRows, totalCols, cellSide, originX, originY;
char **mainTable = nullptr;
int **headerTable = nullptr, **sideTable = nullptr;
char **mainStatus = nullptr;
int **headerStatus = nullptr, **sideStatus = nullptr;
int clickRow, clickCol, score, tmpScore;
int holdX1 = -1, holdY1 = -1, holdX2, holdY2;
bool holding = false;
bool isOnProcess = false; //check if was on process/playing
bool isModified = false; //check if show save process alert or not

bool isQuit = false;
bool isUpdated = true;
bool isWin = false;
bool is_lvl_loaded_failed = true;
bool is_data_loaded_failed = true;

void instruct();
void chooseLevel();
void loadLevel(int lvl);
void loadData();
void askContinue();
void renderTable();
void playing();
void preventAfterWin();
void askSave();
void updateScreen();


int main(int argc, char* argv[])
{
    load_SDL_And_Images();
    do
    {
        while (is_lvl_loaded_failed && isQuit == false)
        {
            showMainMenu();
            instruct();
            chooseLevel();
        }
        
        if (isQuit)
        {
            break; //get out of main loop
        }
        
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
        
        mainStatus = new char *[mainRows];
        for (int i = 0; i < mainRows; i++)
            mainStatus[i] = new char[mainCols];
        
        headerStatus = new int *[headerRows];
        for (int i = 0; i < headerRows; i++)
            headerStatus[i] = new int[mainCols];
        
        sideStatus = new int *[mainRows];
        for (int i = 0; i < mainRows; i++)
            sideStatus[i] = new int[sideCols];
        //end-----should i put 'init table' here?----------------
        
        loadData(); //can import status file here
        if (isOnProcess)
        {
            while (is_data_loaded_failed == true)
            {
                askContinue();
            }
        }
        
        renderTable(); //init table when haven't clicked

        while (is_lvl_loaded_failed == false)
        {
            if (score == mainRows*mainCols)
            {
                isWin = true;
            }
            
            if (!isUpdated)
            {
                updateScreen();
            }
            
            if (isWin == false)
            {
                playing();
            }
            else //prevent modifing table after win
            {
                preventAfterWin();
            }
        }
        

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
        
        for (int i = 0; i < mainRows; i++)
            delete[] mainStatus[i];
        delete [] mainStatus;

        for (int i = 0; i < headerRows; i++)
            delete[] headerStatus[i];
        delete [] headerStatus;

        for (int i = 0; i < mainRows; i++)
            delete[] sideStatus[i];
        delete [] sideStatus;
        //end-----should i put 'del table' here?----------------
        
    }
    while (isQuit == false);
    
    unload_SDL_And_Images();
    return 0;
}

void instruct()
{
    while (isEn >= 0)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            showIns(isEn);
            if ((e.type == SDL_QUIT) ||
                (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
            {
                isQuit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if ((e.button.x > 826)&&(e.button.y > 532)&&
                    (e.button.x < 924)&&(e.button.y < 587))
                    //click change language
                {
                    if (isEn == 1)
                    {
                        isEn = 0;
                    }
                    else
                    {
                        isEn = 1;
                    }
                }
                if ((e.button.x > 41)&&(e.button.y > 532)&&
                    (e.button.x < 95)&&(e.button.y < 586))
                    //click return
                {
                    isEnTmp = isEn;
                    isEn = -1;
                }
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_r)
                {
                    isEnTmp = isEn;
                    isEn = -1;
                }
            }
            renderScreen();
        }
    }
}

void chooseLevel()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if ((e.type == SDL_QUIT) ||
            (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
        {
            isQuit = true;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if ((e.button.x > 91)&&(e.button.y > 446)&&
                (e.button.x < 126)&&(e.button.y < 480))
                //click instruction button
            {
                isEn = 1;
            }
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
    if (isEn == -1)
    {
        showLevelMouse(level);
    }
    isModified = false;
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
    tmpScore = blank;
    pFile.close();
    is_lvl_loaded_failed = false;
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
    
    // load value to status tables
    std::string pathStatus = "/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/levels/level-"+std::to_string(level)+"-status.txt";
    std::ifstream fileStatus (pathStatus);
    
    if (fileStatus.is_open())
    {
        fileStatus >> isOnProcess;
        
        fileStatus >> isWin;
        
        fileStatus >> score;
                
        for (int i = 0; i < mainRows; i++)
        {
            for (int k = 0; k < mainCols; k++)
            {
                fileStatus >> mainStatus[i][k];
            }
        }
        
        for (int i = 0; i < headerRows; i++)
        {
            for (int k = 0; k < mainCols; k++)
            {
                fileStatus >> headerStatus[i][k];
            }
        }
        
        for (int i = 0; i < mainRows; i++)
        {
            for (int k = 0; k < sideCols; k++)
            {
                fileStatus >> sideStatus[i][k];
            }
        }
    }
    fileStatus.close();
}

void askContinue()
{
    showBack();
    std::cout << "isOnPro: " << isOnProcess << std::endl;
    if (isOnProcess)
    {
        showContinue(isEnTmp);
    }
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if ((e.type == SDL_QUIT) ||
            (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
        {
//            unload_SDL_And_Images();
//            exit(1);
            is_lvl_loaded_failed = true;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if ((e.button.x > 268)&&(e.button.y > 344)&&
                (e.button.x < 465)&&(e.button.y < 399))
                //click new game
            {
                std::cout << "New game" << std::endl;
                isOnProcess = false;
                isWin = false;
                score = tmpScore;
                isModified = true;
                
                for (int i = 0; i < mainRows; i++)
                {
                    for (int k = 0; k < mainCols; k++)
                    {
                        mainStatus[i][k] = '.';
                    }
                }
                
                for (int i = 0; i < headerRows; i++)
                {
                    for (int k = 0; k < mainCols; k++)
                    {
                        headerStatus[i][k] = 1;
                    }
                }
                
                for (int i = 0; i < mainRows; i++)
                {
                    for (int k = 0; k < sideCols; k++)
                    {
                        sideStatus[i][k] = 1;
                    }
                }
                is_data_loaded_failed = false;
            }
            if ((e.button.x > 494)&&(e.button.y > 344)&&
                (e.button.x < 692)&&(e.button.y < 399))
                //click continue
            {
                std::cout << "Continue" << std::endl;
                is_data_loaded_failed = false;
            }
        }
    }
    renderScreen();
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
    //show num-off
    for (int i = 0; i < headerRows; i++)
    {
        for (int k = 0; k < mainCols; k++)
        {
            showNumOff(headerTable[i][k], originX + (k+sideCols)*cellSide + 1, originY + i*cellSide + 1, cellSide-1, cellSide-1);
        }
    }
    for (int i = 0; i < mainRows; i++)
    {
        for (int k = 0; k < sideCols; k++)
        {
            showNumOff(sideTable[i][k], originX + k*cellSide + 1, originY + (headerRows + i)*cellSide + 1, cellSide-1, cellSide-1);
        }
    }

    //show num-on at the beginning
    for (int i = 0; i < headerRows; i++)
    {
        for (int k = 0; k < mainCols; k++)
        {
            if (headerStatus[i][k] != 0)
            {
                showNumOn(headerTable[i][k], originX + (k+sideCols)*cellSide + 1, originY + i*cellSide + 1, cellSide-1, cellSide-1);
            }
        }
    }
    for (int i = 0; i < mainRows; i++)
    {
        for (int k = 0; k < sideCols; k++)
        {
            if (sideStatus[i][k] != 0)
            {
                showNumOn(sideTable[i][k], originX + k*cellSide + 1, originY + (headerRows + i)*cellSide + 1, cellSide-1, cellSide-1);
            }
        }
    }
    //show marked cell
    for (int i = 0; i < mainRows; i++)
    {
        for (int k = 0; k < mainCols; k++)
        {
            if (mainStatus[i][k] == '0')
            {
                showMarked(originX+cellSide*(sideCols+k)+1, originY+cellSide*(headerRows+i), cellSide, cellSide);
            }
        }
    }
    for (int i = 1; i <= totalRows + 1; i++) //draw horizontal lines
    {
        showLine(originX, originY + (i-1)*cellSide, cellSide*totalCols+1, 1);
        if ((i == 1) || ((i - headerRows - 1) % 5 == 0 && i > headerRows))
        {
            showLine(originX, originY + (i-1)*cellSide, cellSide*totalCols+2, 2);
        }
    }
    for (int i = 1; i <= totalCols + 1; i++) //draw vertical lines
    {
        showLine(originX + (i-1)*cellSide, originY, 1, cellSide*totalRows+1);
        if ((i == 1) || ((i - sideCols - 1) % 5 == 0 && i > sideCols))
        {
            showLine(originX + (i-1)*cellSide, originY, 2, cellSide*totalRows+2);
        }
    }
    showReward(isWin); //is it neccessary?
    renderScreen();
    is_data_loaded_failed = true;
}

void playing()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if ((e.type == SDL_QUIT) ||
            (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
        {
            while (isModified)
            {
                askSave();
            }
            unload_SDL_And_Images();
            exit(1);
        }
        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_r)
            {
                while (isModified)
                {
                    askSave();
                }
                is_lvl_loaded_failed = true;
            }
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if ((e.button.x > 41)&&
                (e.button.y > 532)&&
                (e.button.x < 95)&&
                (e.button.y < 586))
                //click return button
            {
                while (isModified)
                {
                    askSave();
                }
                is_lvl_loaded_failed = true;
            }
            
            if ((e.button.x > originX)&&
                (e.button.y > originY)&&
                (e.button.x < originX + cellSide*totalCols)&&
                (e.button.y < originY + cellSide*totalRows))
                //click inside the table
            {
                clickCol = (e.button.x - originX)/cellSide;
                clickRow = (e.button.y - originY)/cellSide;
                if (clickRow < headerRows)
                {
                    if (clickCol < sideCols) //out corner
                    {
                        clickRow = -1;
                        clickCol = -1;
                        //do nothing
                    }
                    else //header
                    {
                        clickCol -= sideCols;
                        if (headerStatus[clickRow][clickCol] == 0)
                        {
                            headerStatus[clickRow][clickCol] = 1;
                        }
                        else
                        {
                            headerStatus[clickRow][clickCol] = 0;
                        }
                        isModified = true;
                        isUpdated = false;
                    }
                }
                else
                {
                    if (clickCol < sideCols) //side
                    {
                        clickRow -= headerRows;
                        if (sideStatus[clickRow][clickCol] == 0)
                        {
                            sideStatus[clickRow][clickCol] = 1;
                        }
                        else
                        {
                            sideStatus[clickRow][clickCol] = 0;
                        }
                        isModified = true;
                        isUpdated = false;
                    }
                    else //main
                    {
                        clickRow -= headerRows;
                        clickCol -= sideCols;
                        if (e.button.button == SDL_BUTTON_LEFT)
                        {
                            if (mainStatus[clickRow][clickCol] == '.')
                            {
                                mainStatus[clickRow][clickCol] = '0';
                                isModified = true;
                                isUpdated = false;
                                if (mainStatus[clickRow][clickCol] == mainTable[clickRow][clickCol])
                                {
                                    score++;
                                }
                                else
                                {
                                    score--;
                                }
                            }
                            holdX1 = clickRow;
                            holdY1 = clickCol;
                            holdX2 = clickRow;
                            holdY2 = clickCol;
                            holding = true;
                        }
                        if (e.button.button == SDL_BUTTON_RIGHT)
                        {
                            if (mainStatus[clickRow][clickCol] == '0')
                            {
                                mainStatus[clickRow][clickCol] = '.';
                                isModified = true;
                                isUpdated = false;
                                if (mainStatus[clickRow][clickCol] == mainTable[clickRow][clickCol])
                                {
                                    score++;
                                }
                                else
                                {
                                    score--;
                                }
                            }
                        }
                    }
                }
                std::cout << clickRow << " " << clickCol << " " << score << std::endl;
            }
            
        }
        
        if (e.type == SDL_MOUSEBUTTONUP && holdX1 >= 0 && holdY1 >= 0) //release
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                holding = false;
                //change mainStatus
                if (abs(holdX1-holdX2) > abs(holdY1-holdY2)) //fix col
                {
                    bool isX1Smaller = (holdX1 < holdX2) ? true : false;
                    if (isX1Smaller)
                    {
                        for (int i = holdX1; i <= holdX2; i++)
                        {
                            if (mainStatus[i][holdY1] == '.')
                            {
                                mainStatus[i][holdY1] = '0';
                                isUpdated = false;
                                if (mainStatus[i][holdY1] == mainTable[i][holdY1])
                                {
                                    score++;
                                }
                                else
                                {
                                    score--;
                                }
                            }
                        }
                    }
                    else
                    {
                        for (int i = holdX1; i >= holdX2; i--)
                        {
                            if (mainStatus[i][holdY1] == '.')
                            {
                                mainStatus[i][holdY1] = '0';
                                isUpdated = false;
                                if (mainStatus[i][holdY1] == mainTable[i][holdY1])
                                {
                                    score++;
                                }
                                else
                                {
                                    score--;
                                }
                            }
                        }
                    }
                }
                else //fix row
                {
                    bool isY1Smaller = (holdY1 < holdY2) ? true : false;
                    if (isY1Smaller)
                    {
                        for (int i = holdY1; i <= holdY2; i++)
                        {
                            if (mainStatus[holdX1][i] == '.')
                            {
                                mainStatus[holdX1][i] = '0';
                                isUpdated = false;
                                if (mainStatus[holdX1][i] == mainTable[holdX1][i])
                                {
                                    score++;
                                }
                                else
                                {
                                    score--;
                                }
                            }
                        }
                    }
                    else
                    {
                        for (int i = holdY1; i >= holdY2; i--)
                        {
                            if (mainStatus[holdX1][i] == '.')
                            {
                                mainStatus[holdX1][i] = '0';
                                isUpdated = false;
                                if (mainStatus[holdX1][i] == mainTable[holdX1][i])
                                {
                                    score++;
                                }
                                else
                                {
                                    score--;
                                }
                            }
                        }
                    }
                }
                std::cout << holdX1 << " " << holdY1 << " " << holdX2 << " " << holdY2 << std::endl;
                holdX1 = -1;
                holdY1 = -1;
                holdX2 = -1;
                holdY2 = -1;
            }
        }
        
        if (e.type == SDL_MOUSEMOTION)
        {
            if (holding)
            {
                holdY2 = (e.motion.x - originX)/cellSide - sideCols;
                holdX2 = (e.motion.y - originY)/cellSide - headerRows;
            }
        }
    }
}

void preventAfterWin()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if ((e.type == SDL_QUIT) ||
            (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
        {
            is_lvl_loaded_failed = true;
        }
        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_r)
            {
                while (isModified)
                {
                    askSave();
                }
                is_lvl_loaded_failed = true;
            }
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if ((e.button.x > 41)&&
                (e.button.y > 532)&&
                (e.button.x < 95)&&
                (e.button.y < 586))
                //click return button
            {
                while (isModified)
                {
                    askSave();
                }
                is_lvl_loaded_failed = true;
            }
        }
    }
}

void askSave()
{
    showBack();
    showSaveProcess(isEnTmp);
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if ((e.type == SDL_QUIT) ||
            (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
        {
//            unload_SDL_And_Images();
//            exit(1);
            is_lvl_loaded_failed = true;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if ((e.button.x > 268)&&(e.button.y > 344)&&
                (e.button.x < 465)&&(e.button.y < 399))
                //click don't save
            {
                std::cout << "Don't save" << std::endl;
                //export data from status arrays to status file
                std::string pathStatus = "/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/levels/level-"+std::to_string(level)+"-status.txt";
                std::ofstream fStatus (pathStatus);
                //isOnProcess isWin
                fStatus << "0 0 " << tmpScore << std::endl;
                //main
                for (int i = 0; i < mainRows; i++)
                {
                    for (int k = 0; k < mainCols; k++)
                    {
                        fStatus << ". ";
                    }
                    fStatus << std::endl;
                }
                fStatus << std::endl;

                //header
                for (int i = 0; i < headerRows; i++)
                {
                    for (int k = 0; k < mainCols; k++)
                    {
                        fStatus << "1 ";
                    }
                    fStatus << std::endl;
                }
                fStatus << std::endl;

                //side
                for (int i = 0; i < mainRows; i++)
                {
                    for (int k = 0; k < sideCols; k++)
                    {
                        fStatus << "1 ";
                    }
                    fStatus << std::endl;
                }
                fStatus.close();
                isModified = false;
            }
            if ((e.button.x > 494)&&(e.button.y > 344)&&
                (e.button.x < 692)&&(e.button.y < 399))
                //click save
            {
                std::cout << "Save" << std::endl;
                //export data from status arrays to status file
                std::string pathStatus = "/Users/haht/CodeSpace/Cpp/__LTNC__/XCode/nonogram/nonogram/levels/level-"+std::to_string(level)+"-status.txt";
                std::ofstream fStatus (pathStatus);
                //isOnProcess isWin
                if (isWin)
                {
                    fStatus << "1 1 " << score << std::endl;
                }
                else
                {
                    fStatus << "1 0 " << score << std::endl;
                }
                //main
                for (int i = 0; i < mainRows; i++)
                {
                    for (int k = 0; k < mainCols; k++)
                    {
                        fStatus << mainStatus[i][k] << " ";
                    }
                    fStatus << std::endl;
                }
                fStatus << std::endl;

                //header
                for (int i = 0; i < headerRows; i++)
                {
                    for (int k = 0; k < mainCols; k++)
                    {
                        fStatus << headerStatus[i][k] << " ";
                    }
                    fStatus << std::endl;
                }
                fStatus << std::endl;

                //side
                for (int i = 0; i < mainRows; i++)
                {
                    for (int k = 0; k < sideCols; k++)
                    {
                        fStatus << sideStatus[i][k] << " ";
                    }
                    fStatus << std::endl;
                }
                fStatus.close();
                
                isModified = false;
            }
        }
    }
    renderScreen();
}

void updateScreen()
{
    showBack();
    //show num-off every screen
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
    //show num-on if match condition
    for (int i = 0; i < headerRows; i++)
    {
        for (int k = 0; k < mainCols; k++)
        {
            if (headerStatus[i][k] == 1)
            {
                showNumOn(headerTable[i][k], originX + (k+sideCols)*cellSide + 1, originY + i*cellSide + 1, cellSide-1, cellSide-1);
            }
        }
    }
    for (int i = 0; i < mainRows; i++)
    {
        for (int k = 0; k < sideCols; k++)
        {
            if (sideStatus[i][k] == 1)
            {
                showNumOn(sideTable[i][k], originX + k*cellSide + 1, originY + (headerRows + i)*cellSide + 1, cellSide-1, cellSide-1);
            }
        }
    }
    
    //show holding cells
    if (holdX1 >= 0 && holdY1 >= 0)
    {
        holdX2 = (holdX2 < 0) ? 0 : holdX2;
        holdY2 = (holdY2 < 0) ? 0 : holdY2;
        holdX2 = (holdX2 >= mainRows) ? (mainRows - 1) : holdX2;
        holdY2 = (holdY2 >= mainCols) ? (mainCols - 1) : holdY2;
    
        if (abs(holdX1-holdX2) > abs(holdY1-holdY2)) //fix col
        {
            bool isX1Smaller = (holdX1 < holdX2) ? true : false;
            if (isX1Smaller)
            {
                for (int i = holdX1; i <= holdX2; i++)
                {
                    showMarked(originX+cellSide*(sideCols+holdY1)+cellSide/4+1, originY+cellSide*(headerRows+i)+cellSide/4+1, cellSide/2, cellSide/2);
                }
            }
            else
            {
                for (int i = holdX1; i >= holdX2; i--)
                {
                    showMarked(originX+cellSide*(sideCols+holdY1)+cellSide/4+1, originY+cellSide*(headerRows+i)+cellSide/4+1, cellSide/2, cellSide/2);
                }
            }
        }
        else //fix row
        {
            bool isY1Smaller = (holdY1 < holdY2) ? true : false;
            if (isY1Smaller)
            {
                for (int i = holdY1; i <= holdY2; i++)
                {
                    showMarked(originX+cellSide*(sideCols+i)+cellSide/4+1, originY+cellSide*(headerRows+holdX1)+cellSide/4+1, cellSide/2, cellSide/2);
                }
            }
            else
            {
                for (int i = holdY1; i >= holdY2; i--)
                {
                    showMarked(originX+cellSide*(sideCols+i)+cellSide/4+1, originY+cellSide*(headerRows+holdX1)+cellSide/4+1, cellSide/2, cellSide/2);
                }
            }
        }
    }
    
    //show marked cell
    for (int i = 0; i < mainRows; i++)
    {
        for (int k = 0; k < mainCols; k++)
        {
            if (mainStatus[i][k] == '0')
            {
                showMarked(originX+cellSide*(sideCols+k)+1, originY+cellSide*(headerRows+i), cellSide, cellSide);
            }
        }
    }
    
    //draw again because init table was disappeare
    
    for (int i = 1; i <= totalRows + 1; i++) //draw horizontal lines
    {
        showLine(originX, originY + (i-1)*cellSide, cellSide*totalCols+1, 1);
        if ((i == 1) || ((i - headerRows - 1) % 5 == 0 && i > headerRows))
        {
            showLine(originX, originY + (i-1)*cellSide, cellSide*totalCols+2, 2);
        }
    }
    for (int i = 1; i <= totalCols + 1; i++) //draw vertical lines
    {
        showLine(originX + (i-1)*cellSide, originY, 1, cellSide*totalRows+1);
        if ((i == 1) || ((i - sideCols - 1) % 5 == 0 && i > sideCols))
        {
            showLine(originX + (i-1)*cellSide, originY, 2, cellSide*totalRows+2);
        }
    }
    showReward(isWin);
    
    renderScreen();
}
