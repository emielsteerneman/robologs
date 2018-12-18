#ifndef ROBOLOGS_INTERFACE_H
#define ROBOLOGS_INTERFACE_H


#include <QWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>

class Interface : public QWidget {
Q_OBJECT
public:
    explicit Interface(QWidget *parent = 0);

signals:
public slots:
    void updateGameState(int i);
private:
    QPushButton *btn;
    QProgressBar *bar;
    QSlider *slider;
};

#endif // ROBOLOGS_INTERFACE_H







//#ifndef ROBOLOGS_TESTAI_H
//#define ROBOLOGS_TESTAI_H
//
//#include <QMainWindow>
//#include <QWidget>
//#include <QBoxLayout>
//#include <QPushButton>
//#include <memory>
//
//#include "GameStateTracker.h"
//
//class Field : public QWidget {
//Q_OBJECT
//public:
//    explicit Field(QWidget* parent = nullptr);
//    int x = 0;
//protected:
//    void paintEvent(QPaintEvent *event) override;
//};
//
//class Interface : public QMainWindow {
//Q_OBJECT
//public:
//    explicit Interface(QWidget * parent = nullptr);
//    int x = 0;
//public slots:
//    void updateUi();
//    void updateGamestate(const GameState& gameState);
//private:
//    std::shared_ptr<QHBoxLayout> horizontalLayout;
//    std::shared_ptr<QVBoxLayout> verticalLayout;
//
//    std::shared_ptr<QPushButton> button1;
//    std::shared_ptr<QPushButton> button2;
//    std::shared_ptr<QPushButton> button3;
//
//    std::shared_ptr<Field> field;
//};
//
//
//#endif //ROBOLOGS_TESTAI_H