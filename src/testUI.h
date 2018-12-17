//
// Created by emiel on 16-12-18.
//

#ifndef ROBOLOGS_TESTAI_H
#define ROBOLOGS_TESTAI_H

#include <QMainWindow>
#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <memory>

class Field : public QWidget {
Q_OBJECT
public:
    explicit Field(QWidget* parent = nullptr);
    int x = 0;
protected:
    void paintEvent(QPaintEvent *event) override;
};

class testUI : public QMainWindow {
Q_OBJECT
public:
    explicit testUI(QWidget * parent = nullptr);
    int x = 0;
public slots:
    void updateUi();
private:
    std::shared_ptr<QHBoxLayout> horizontalLayout;
    std::shared_ptr<QVBoxLayout> verticalLayout;

    std::shared_ptr<QPushButton> button1;
    std::shared_ptr<QPushButton> button2;
    std::shared_ptr<QPushButton> button3;

    std::shared_ptr<Field> field;
};


#endif //ROBOLOGS_TESTAI_H