// PROG - MIEIC
// JAS
// Example of a program that prints colored characters on the console (in text mode)
#include "cmd_utils.h"
using namespace std;

//==========================================================================================
void clrscr(void)
{
    COORD coordScreen = { 0, 0 }; // upper left corner
    DWORD cCharsWritten;
    DWORD dwConSize;
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hCon, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    // fill with spaces
    FillConsoleOutputCharacter(hCon, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hCon, &csbi);
    FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    // cursor to upper left corner
    SetConsoleCursorPosition(hCon, coordScreen);
}
//==========================================================================================
// Position the cursor at column 'x', line 'y'
// The coodinates of upper left corner of the screen are (x,y)=(0,0)
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//==========================================================================================
// Set text color
void setcolor(unsigned int color)
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon, color);
}
//==========================================================================================
// Set text color & background
void setcolor(unsigned int color, unsigned int background_color)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    if (background_color == BLACK)
    SetConsoleTextAttribute(hCon, color);
    else
    SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN |
    BACKGROUND_RED);
}
//==========================================================================================
// Fill the screen with colored numbers
// int main()
// {
//     clrscr();
//     srand((unsigned int)time(NULL));
//     for (int x = 0; x < 80; x++)
//     for (int y = 0; y < 24; y++)
//     {
//     gotoxy(x, y);
//     if (rand() % 2 == 0)
//     setcolor(x % 15 + 1);
//     else
//     setcolor(y % 15 + 1, rand() % 2);
//     cout << x % 10;
//     }
// }