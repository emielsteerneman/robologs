//
// Created by emiel on 13-12-18.
//

#ifndef ROBOLOGS_WRITER_H
#define ROBOLOGS_WRITER_H

#include "game/GameStateTracker.h"
#include <fstream>
#include <QWidget>

class Writer : public QWidget {
Q_OBJECT
    std::string filename;
    std::ofstream out;
    bool isFirst = true;
    int nFrames = 0;
    int nFramesTotal = 0;

public:
    Writer(std::string filename);
    ~Writer();
    std::string gameStateToJson(const GameState* gameState);

public slots:
    void write(const GameState* gameState);

};


#endif //ROBOLOGS_WRITER_H
