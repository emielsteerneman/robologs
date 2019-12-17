//
// Created by emiel on 17-12-19.
//

#ifndef ROBOLOGS_ASCII_H
#define ROBOLOGS_ASCII_H


#include <string>
#include <game/GameStateTracker.h>

class ASCII {

public:
    static std::string emptyField(int WIDTH, int HEIGHT);
    static std::string GamestateToString(const GameState &gameState, int FIELD_WIDTH, int FIELD_HEIGHT);
};


#endif //ROBOLOGS_ASCII_H
