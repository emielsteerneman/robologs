//
// Created by emiel on 16-12-18.
//

#include "testUI.h"
#include "testUI.h"

#include <QApplication>
#include <QWidget>
#include <memory>
#include <QLabel>
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <iostream>

std::shared_ptr<testUI> window;
const QColor FIELD_COLOR {50, 255, 50, 255};

testUI::testUI(QWidget * parent) : QMainWindow(parent){
    setMinimumWidth(800);
    setMinimumHeight(600);

    horizontalLayout = std::make_shared<QHBoxLayout>();
    verticalLayout = std::make_shared<QVBoxLayout>();

    field = std::make_shared<Field>(this);

    button1 = std::make_shared<QPushButton>("button1");
    button2 = std::make_shared<QPushButton>("button2");
    button3 = std::make_shared<QPushButton>("button3");
    verticalLayout->addWidget(button1.get());
    verticalLayout->addWidget(button2.get());
    verticalLayout->addWidget(button3.get());

    horizontalLayout->addWidget(field.get(), 4);
    horizontalLayout->addLayout(verticalLayout.get(), 1);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(horizontalLayout.get());


    auto *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update())); // Triggers PaintEvent
    connect(timer, SIGNAL(timeout()), this, SLOT(updateUi()));
//    connect(timer, SIGNAL(timeout()), this, SLOT(updateWidgets()));
    timer->start(20); // delay in ms

}

void testUI::updateUi() {
    button1->setText(QString::number(x));
    x++;
}

Field::Field(QWidget *parent) : QWidget(parent){
    QLabel *label = new QLabel(this);
    QHBoxLayout *layout = new QHBoxLayout();
    label->setText("Random String");
    layout->addWidget(label);
    setLayout(layout);
}

void Field::paintEvent(QPaintEvent* event) {
    std::cout << "PaintEvent triggered " << std::endl;
    QPainter painter(this);
    painter.setBrush(FIELD_COLOR);
    painter.drawRect(0,0, this->size().width(), this->size().height());
    painter.drawText(24,24, "Waiting for incoming World State");

    painter.drawLine(0, 0, x, 100);
    x++;
}

