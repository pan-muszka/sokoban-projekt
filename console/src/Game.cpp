#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

void clear()
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

void printMap(Level level)
{
    for (const auto& row : level.getPosition())
            cout << row << endl;
}

Game::Game()
{
    // ...
}

void Game::run(GameMap gameMap)
{
    level.loadFromFile(gameMap.fullPath);

    while (!level.isCompleted())
    {
        clear();

        cout << "Liczba pchniec: " << level.getPushes() << endl << "Liczba ruchow: " << level.getMoves() << endl;

        printMap(level);

        cout << "'W' - do gory, 'S' - w dol, 'A' - w lewo, 'D' - w prawo, 'R' - reset poziomy, 'M' - powrot";
        char input = _getch();
        switch (tolower(input))
        {
        case 'w':
            level.movePlayer(Direction::UP);
            break;
        case 's':
            level.movePlayer(Direction::DOWN);
            break;
        case 'a':
            level.movePlayer(Direction::LEFT);
            break;
        case 'd':
            level.movePlayer(Direction::RIGHT);
            break;
        case 'r':
            level.reset();
            break;
        case 'm':
            return;
        }
    }

    clear();

    printMap(level);
    cout << "Poziom ukonczony!" << endl;
    cout << "Liczba pchniec: " << level.getPushes() << endl << "Liczba ruchow: " << level.getMoves() << endl;

    _getch();
}

std::vector<GameMap> Game::getMaps()
{
    return level.getMaps();
}
