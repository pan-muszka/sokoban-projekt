#include "Game.h"
#include <cstdio>
#include <conio.h>
#include <iostream>
#include <windows.h>

void clearMenu();

int currentPos = 0;
bool quitGame = false;
const unsigned char MENUITEMS = 2;
unsigned char currentView = 0;

std::vector<GameMap> loadedMaps;

std::vector<GameMap> limitScreenMaps(unsigned char num, unsigned long long int pos, unsigned char* pointerPos)
{
    if (num >= loadedMaps.size())
        return loadedMaps;

    int startP, endP;

    startP = pos - (num / 2);
    endP = startP + num;

    while (startP < 0)
    {
        startP++;
        endP++;
    }

    while (endP > loadedMaps.size())
    {
        startP--;
        endP--;
    }

    *pointerPos = (currentPos % loadedMaps.size()) - startP;

    std::vector<GameMap> smallList(loadedMaps.begin() + startP, loadedMaps.begin() + endP);

    return smallList;
}

void printView()
{
    switch (currentView)
    {
    case 0: // Menu g³ówne
    {
        clearMenu();
        std::cout << "==== SOKOBAN ====" << std::endl;
        std::cout << (currentPos % MENUITEMS == 0 ? ">" : " ") << "Wybor poziomu" << std::endl;
        std::cout << (currentPos % MENUITEMS == 1 ? ">" : " ") << "Wyjscie" << std::endl;
        std::cout << "'W' - do gory, 'S' - w dol, 'ENTER' - zatwierdz" << std::endl;
        break;
    }
    case 1: // Wybór poziomu
    {
        const unsigned long long int mapsNumber = loadedMaps.size();
        const unsigned long long int currentPosOnMapsSelection = currentPos % mapsNumber;
        unsigned char mapsToShow = 11;

        unsigned char pointerPosOnSmallerVector = currentPosOnMapsSelection;
        std::vector<GameMap> mapsShowedOnScreen = limitScreenMaps(mapsToShow, currentPosOnMapsSelection, &pointerPosOnSmallerVector);

        clearMenu();
        std::cout << "==== WYBOR POZIOMU ====" << std::endl;
        for (std::size_t i = 0; i < mapsShowedOnScreen.size(); ++i) {
            const GameMap gameMap = mapsShowedOnScreen[i];
            std::cout << (pointerPosOnSmallerVector == i ? ">" : " ") << gameMap.name << " z " << gameMap.parentFolder << std::endl;
        }
        std::cout << "'W' - do gory, 'S' - w dol, 'ENTER' - zatwierdz, 'M' - wroc do glownego menu" << std::endl;
        break;
    }
    }
}

void confirmSelection(Game* game)
{
    switch (currentView)
    {
    case 0: // Menu g³ówne
    {
        switch (((currentPos % MENUITEMS) + MENUITEMS) % MENUITEMS)
        {
        case 0:
        {
            loadedMaps = game->getMaps();
            currentView = 1;
            currentPos = 0;
            break;
        }
        case 1:
            quitGame = true;
            break;
        }
        break;
    }
    case 1: // Wybór poziomu
    {
        long long int selectedMap = currentPos % loadedMaps.size();
        game->run(loadedMaps[selectedMap]);
        break;
    }
    }

}

int main() {
    Game game;
    while (!quitGame)
    {
        printView();
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
            confirmSelection(&game);
            break;
        case 'm':
            currentView = 0;
            currentPos = 0;
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
