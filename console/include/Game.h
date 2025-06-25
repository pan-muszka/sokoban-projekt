#ifndef GAME_H
#define GAME_H

#include "Level.h"

class Game {
private:
    Level level;
public:
    Game();
    void run(GameMap gameMap);
    std::vector<GameMap> getMaps();
};


#endif // GAME_H
