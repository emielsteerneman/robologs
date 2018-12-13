//
// Created by emiel on 13-12-18.
//

#ifndef ROBOLOGS_WRITER_H
#define ROBOLOGS_WRITER_H

#include "GameStateTracker.h"
#include <fstream>

class Writer {
    std::string filename;
    std::ofstream out;
    bool isFirst = true;

public:
    Writer(std::string filename);
    ~Writer();
    void write(const GameState& gameState);
    std::string gameStateToJson(const GameState& gameState);

};


#endif //ROBOLOGS_WRITER_H
