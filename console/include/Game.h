#ifndef GAME_H
#define GAME_H

#include "Level.h"

class Game {
private:
    Level level;
public:
    Game();
    void run();
};


#endif // GAME_H
