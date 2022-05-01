
#ifndef GameScreen_hpp
#define GameScreen_hpp

const int SCREEN_WIDTH=960, SCREEN_HEIGHT=640;
const int PADDING_LEFT = 136;
const int PADDING_TOP = 60;
const int MAX_TABLE_WIDTH = SCREEN_WIDTH - PADDING_LEFT * 2;
const int MAX_TABLE_HEIGHT = SCREEN_HEIGHT - PADDING_TOP * 2;

void load_SDL_And_Images();

void unload_SDL_And_Images();

void showMainMenu();

void showLevelMouse(int lvl);

void delLevelMouse();

void showBack();

void showLine(int x, int y, int w, int h);

void showNumOff(int num, int x, int y, int w, int h);

void showNumOn(int num, int x, int y, int w, int h);

void showMarked(int x, int y, int w, int h);

void showReward();

void renderScreen();

#endif /* GameScreen_hpp */
