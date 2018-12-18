
#include <iostream>
#include <QApplication>
#include <QString>

#include "Interface.h"

Interface::Interface(QWidget *parent) : QWidget(parent) {
    std::cout << "[Interface] New interface created" << std::endl;
    setFixedSize(200, 300);

    btn = new QPushButton("Hello World", this);
    btn->setGeometry(30, 30, 140, 40);
    btn->setCheckable(true);

//    connect(btn, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

    bar = new QProgressBar(this);
    bar->setRange(0, 100);
    bar->setValue(0);
    bar->setGeometry(30, 130, 140, 40);

    slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setValue(0);
    slider->setGeometry(30, 230, 140, 40);

    connect(slider, SIGNAL(valueChanged(int)), bar, SLOT(setValue(int)));

}

void Interface::updateGameState(const GameState& gameState){
    std::cout << "[Interface] Updating game state" << std::endl;
//    QString str = std::to_string(gameState.timestamp);
    btn->setText(QString::number(gameState.timestamp));
    bar->setValue(bar->value() + 1);
}


//#include "Interface.h"
//#include "Interface.h"
//
//#include <QApplication>
//#include <QWidget>
//#include <memory>
//#include <QLabel>
//#include <QPainter>
//#include <QColor>
//#include <QTimer>
//#include <iostream>
//
//std::shared_ptr<Interface> window;
//const QColor FIELD_COLOR {50, 255, 50, 255};
//
//Interface::Interface(QWidget * parent) : QMainWindow(parent) {
//    std::cout << "Interface" << std::endl;
//    setMinimumWidth(800);
//    setMinimumHeight(600);
//
//    horizontalLayout = std::make_shared<QHBoxLayout>();
//    verticalLayout = std::make_shared<QVBoxLayout>();
//
//    field = std::make_shared<Field>(this);
//
//    button1 = std::make_shared<QPushButton>("button1");
//    button2 = std::make_shared<QPushButton>("button2");
//    button3 = std::make_shared<QPushButton>("button3");
//    verticalLayout->addWidget(button1.get());
//    verticalLayout->addWidget(button2.get());
//    verticalLayout->addWidget(button3.get());
//
//    horizontalLayout->addWidget(field.get(), 4);
//    horizontalLayout->addLayout(verticalLayout.get(), 1);
//
//
//    setCentralWidget(new QWidget);
//    centralWidget()->setLayout(horizontalLayout.get());
//
//}
//
//void Interface::updateUi() {
//    button1->setText(QString::number(x));
//    x++;
//}
//
//void Interface::updateGamestate(const GameState &gameState) {
//    std::cout << "Updating GameState" << std::endl;
//}
//
//Field::Field(QWidget *parent) : QWidget(parent){
//    QLabel *label = new QLabel(this);
//    QHBoxLayout *layout = new QHBoxLayout();
//    label->setText("Random String");
//    layout->addWidget(label);
//    setLayout(layout);
//}
//
//void Field::paintEvent(QPaintEvent* event) {
//    std::cout << "PaintEvent triggered " << std::endl;
//    QPainter painter(this);
//    painter.setBrush(FIELD_COLOR);
//    painter.drawRect(0,0, this->size().width(), this->size().height());
//    painter.drawText(24,24, "Waiting for incoming World State");
//
//    painter.drawLine(0, 0, x, 100);
//    x++;
//}
//
