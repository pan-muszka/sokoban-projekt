#include "Level.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <filesystem>

using namespace rapidjson;

namespace fs = std::filesystem;

Level::Level() {
    FILE* fp = fopen("game/properties.json", "rb"); // non-Windows use "r"

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    Document document;
    document.ParseStream(is);

    fclose(fp);

    wall = document["characters"]["wall"].GetString()[0];
    player = document["characters"]["player"].GetString()[0];
    playerOnGoal = document["characters"]["playerongoal"].GetString()[0];
    box = document["characters"]["box"].GetString()[0];
    boxOnGoal = document["characters"]["boxongoal"].GetString()[0];
    goalSquare = document["characters"]["goalsquare"].GetString()[0];
    floor = document["characters"]["floor"].GetString()[0];

    for (const auto& folder : document["maps"]["folders"].GetArray())
    {
        folders.push_back(folder.GetString());
    }

    for (const auto& ext : document["maps"]["extensions"].GetArray())
    {
        extensions.push_back(ext.GetString());
    }

    moveCounter = 0;
    pushCounter = 0;
}

void Level::loadFromFile(const std::string& path) {
    currentMap.clear();
    originalMap.clear();
    moveCounter = 0;
    pushCounter = 0;

    std::ifstream file(path);
    std::string line;
    while (getline(file, line)) {
        currentMap.push_back(line);
        originalMap.push_back(line);
    }

    for (int y = 0; y < currentMap.size(); ++y)
        for (int x = 0; x < currentMap[y].size(); ++x)
            if (currentMap[y][x] == player || currentMap[y][x] == playerOnGoal)
            {
                playerX = x;
                playerY = y;
                return;
            }
}

const std::vector<std::string>& Level::getPosition() const {
    return currentMap;
}

void Level::reset()
{
    moveCounter = 0;
    pushCounter = 0;
    currentMap = originalMap;
    for (int y = 0; y < currentMap.size(); ++y)
        for (int x = 0; x < currentMap[y].size(); ++x)
            if (currentMap[y][x] == player || currentMap[y][x] == playerOnGoal)
            {
                playerX = x;
                playerY = y;
                return;
            }
}

bool Level::movePlayer(Direction dir)
{
    int dx = 0, dy = 0;

    switch (dir)
    {
        case Direction::UP:    dy = -1; break;
        case Direction::DOWN:  dy = 1; break;
        case Direction::LEFT:  dx = -1; break;
        case Direction::RIGHT: dx = 1; break;
    }

    int x = playerX;
    int y = playerY;
    int nx = x + dx;
    int ny = y + dy;
    int nnx = x + 2 * dx;
    int nny = y + 2 * dy;

    char& target = currentMap[ny][nx];
    char& beyond = currentMap[nny][nnx];

    if (target == wall)
        return false;

    if (target == box || target == boxOnGoal)
    {
        if (beyond == floor || beyond == goalSquare)
        {
            beyond = (beyond == goalSquare) ? boxOnGoal : box;
            target = (target == boxOnGoal) ? goalSquare : floor;
            pushCounter++;
        }
        else
        {
            return false;
        }
    }

    char& from = currentMap[y][x];
    from = (from == playerOnGoal) ? goalSquare : floor;
    target = (target == goalSquare) ? playerOnGoal : player;

    playerX = nx;
    playerY = ny;

    moveCounter++;

    return true;
}

bool Level::isCompleted() const
{
    for (const auto& row : currentMap)
        for (char c : row)
            if (c == goalSquare || c == playerOnGoal)
                return false;
    return true;
}

unsigned int Level::getMoves()
{
    return moveCounter;
}

unsigned int Level::getPushes()
{
    return pushCounter;
}

void Level::loadMaps()
{
    maps.clear();

    for (const auto& folder : folders)
        for (const auto& extension : extensions)
            for (const auto& entry : fs::directory_iterator("game/" + folder))
                if (entry.path().extension() == extension)
                {
                    GameMap loadedMap = {entry.path().stem().string(),
                                       folder,
                                       entry.path().string()};
                    maps.push_back(loadedMap);
                }

}

std::vector<GameMap> Level::getMaps()
{
    loadMaps();
    return maps;
}

