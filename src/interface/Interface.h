#ifndef ROBOLOGS_INTERFACE_H
#define ROBOLOGS_INTERFACE_H


#include <QWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>
#include <QLineEdit>

#include "Field.h"
#include "Timeline.h"
#include "game/GameInfoTracker.h"
#include "game/GameStateTracker.h"

class Interface : public QWidget {
Q_OBJECT
public:
    explicit Interface(QWidget *parent = 0);
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void updateGameState(const GameState* gameState);
    void setGameInfo(const GameInfo* gameInfo);

private:
    const GameInfo* gameInfo = nullptr;

    Field *field;
    Timeline *timeline;
    QLineEdit* gameStageCommand;
};

#endif // ROBOLOGS_INTERFACE_H
