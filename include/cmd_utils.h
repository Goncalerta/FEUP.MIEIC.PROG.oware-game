#ifndef CMD_UTILS_H
#define CMD_UTILS_H

#define NOMINMAX
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>

//==========================================================================================
//COLOR CODES: (alternative: use symbolic const’s)
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

//==========================================================================================
void clrscr(void);
//==========================================================================================
// Position the cursor at column 'x', line 'y'
// The coodinates of upper left corner of the screen are (x,y)=(0,0)
void gotoxy(int x, int y);
//==========================================================================================
// Set text color
void setcolor(unsigned int color);
//==========================================================================================
// Set text color & background
void setcolor(unsigned int color, unsigned int background_color);

#endif