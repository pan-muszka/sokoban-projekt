#pragma once
#include <vector>
#include <string>

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Level {
private:
    std::vector<std::string> originalMap;
    std::vector<std::string> currentMap;
    int playerX, playerY;
    char wall, player, playerOnGoal, box, boxOnGoal, goalSquare, floor;
    unsigned int moveCounter, pushCounter;
public:
    unsigned int getMoves();
    unsigned int getPushes();
    void loadFromFile(const std::string& filename);
    const std::vector<std::string>& getPosition() const;
    bool movePlayer(Direction dir);
    void reset();
    bool isCompleted() const;
    Level();
};
