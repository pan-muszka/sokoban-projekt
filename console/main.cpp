#include "Game.h"
#include <cstdio>
#include <conio.h>
#include <iostream>
#include <windows.h>

void clearMenu();

unsigned char currentPos = 0;
bool quitGame = false;
const unsigned char MENUITEMS = 2;

void printMainMenu()
{
        clearMenu();
        std::cout << "==== SOKOBAN ====" << std::endl;
        std::cout << (currentPos % MENUITEMS == 0 ? ">" : " ") << "Start gry" << std::endl;
        std::cout << (currentPos % MENUITEMS == 1 ? ">" : " ") << "Wyjscie" << std::endl;
}

void confirmSelection()
{
    switch (currentPos % MENUITEMS)
    {
    case 0:
    {
        Game game;
        game.run();
        break;
    }
    case 1:
        quitGame = true;
        break;
    }
}

int main() {
    while (!quitGame)
    {
        printMainMenu();
        char input = _getch();
        switch (tolower(input))
        {
        case 'w':
            --currentPos;
            break;
        case 's':
            ++currentPos;
            break;
        case 13:
            confirmSelection();
            break;
        }
    }

    return 0;
}

void clearMenu()
{
    COORD topLeft  = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}
