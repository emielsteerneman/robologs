//
// Created by emiel on 19-12-18.
//

#ifndef ROBOLOGS_FIELD_H
#define ROBOLOGS_FIELD_H


#include <QWidget>

#include "GameStateTracker.h"

class Field : public QWidget {
Q_OBJECT
public:
    explicit Field(QWidget *parent = 0);
    void drawGameState(const GameState gameState);


private:
};


#endif //ROBOLOGS_FIELD_H
