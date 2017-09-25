#ifndef LUTIL_H
#define LUTIL_H

#include "lopengl.h"

//Screen constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 512;
const int SCREEN_FPS = 60;

bool initGL();

bool loadMedia();

void update();

void render();

void pressedMouse(int button, int state, int x, int y);

int sgn(int a);

bool valid(int x, int y);

int dist(int x1, int y1, int x2, int y2);

void resetPlot();

void bresenAlg(int x1, int y1, int x2, int y2);

void drawCirc(int xc, int yc, int x, int y);

void bresenCirc(int xc, int yc, int r);

void polymode(int x, int y, bool cls = false);

void imgSave(int width,int height);

void pressedKey(unsigned char key, int x, int y);

#endif
