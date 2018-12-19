//
// Created by emiel on 18-12-18.
//

#ifndef ROBOLOGS_ROBOLOGS_H
#define ROBOLOGS_ROBOLOGS_H

#include "input/Reader.h"
#include "game/GameStateTracker.h"
#include "interface/Interface.h"

class Robologs : public QObject {
    Q_OBJECT
public:
    explicit Robologs();
    int start(int argc, char* argv[]);

};

#endif //ROBOLOGS_ROBOLOGS_H
