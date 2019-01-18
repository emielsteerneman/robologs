/*

How would this work?

* Somehow grab a vision/referee packet from some kind of source
* Stuff that packet into the GameStateTracker
* Get the GameState as soon as the interval has been reached

*/

#include <iostream>
#include <iomanip>
#include <QApplication>
#include <QObject>
#include <QTimer>
#include <QThread>
#include <QtWidgets/QStyle>
#include <QDesktopWidget>

#include "Robologs.h"
#include "input/Reader.h"
#include "game/GameStateTracker.h"
#include "Player.h"
#include "interface/Interface.h"
#include "output/Drawer.h"
#include "output/Writer.h"


std::string getFileName(const std::string& s) {

    char sep = '/';

    size_t i = s.rfind(sep, s.length());
    if (i != std::string::npos) {
        return(s.substr(i+1, s.length() - i));
    }

    return("");
}

int fps = 60;
int nStates = 1000;

Robologs::Robologs() : QObject(){
    std::cout << "[Robologs] New Robologs constructed" << std::endl;
}

int Robologs::startWriter(int argc, char* argv[]) {
    std::cout << "[Robologs] writer" << std::endl;
    QApplication app(argc, argv);




//    std::string file = "2018-06-19_16-35_RoboDragons-vs-RoboTeam_Twente.log";
//    std::string fileIn = "/media/emiel/HDD500/robocup_logs/" + file;
//    std::string fileIn = "/media/emiel/HDD640/personal/projects/robologs/" + file;
//    std::string fileOut = "/home/emiel/Desktop/AML/" + file;
//    fileOut = fileOut.substr(0, fileOut.find_last_of('.')) + ".json";

    std::string fileIn = argv[1];
    std::string filename = getFileName(fileIn);
    std::string fileOut = "/home/emiel/Desktop/AML/" + filename;
    std::cout << "Writing file to " << fileOut << std::endl;

    Writer* writer = new Writer(fileOut);

    Player* player = new Player(fileIn);
    player->getInfo();
    player->tracker.setHz(30);

    while(!player->reader.isEof()) {
        player->tick();
        writer->write(player->tracker.get());
    }
    writer->endJSON();
    writer->close();
    std::cout << "DONE WITH WRITING FILE!" << std::endl;

//    QThread *thread = new QThread(this);
//    player->moveToThread(thread);
//    connect(this, SIGNAL(start()), player, SLOT(start()));
//    thread->start();

//    connect(player, SIGNAL(signalGameState(const GameState*)), writer, SLOT(write(const GameState*)), Qt::BlockingQueuedConnection);
//
//    emit start();
//
//    int result = app.exec();
//    return result;

    return 0;
}




int Robologs::startInterface(int argc, char* argv[]) {
    std::cout << "[Robologs] Starting interface" << std::endl;

    QApplication app(argc, argv);

    Interface* interface = new Interface();
    interface->move((QApplication::desktop()->width() - interface->width()) / 2, (QApplication::desktop()->height() - interface->height()) / 2);
    interface->show();

    std::string file = "2018-06-20_11-18_TIGERs_Mannheim-vs-ER-Force.log";
    std::string fileIn = "/media/emiel/HDD500/robocup_logs/" + file;

    Player* player = new Player(fileIn);
    QThread *thread = new QThread(this);
    player->moveToThread(thread);
    connect(this, SIGNAL(start()), player, SLOT(start()));
    thread->start();

    connect(player, SIGNAL(signalGameState(const GameState*)), interface, SLOT(updateGameState(const GameState*)), Qt::BlockingQueuedConnection);
    connect(player, SIGNAL(signalGameInfo(const GameInfo*)), interface, SLOT(setGameInfo(const GameInfo*)), Qt::BlockingQueuedConnection);

    connect(interface, SIGNAL(signalProgress(double)), player, SLOT(findProgress(double)), Qt::BlockingQueuedConnection);

    emit start();

    int result = app.exec();
    return result;
}


int main(int argc, char* argv[]) {

    if(argc < 2) {
        std::cout << "Required filename not given. Terminating." << std::endl;
        return 0;
    }
    std::cout << argv[1] << std::endl;
    std::cout << getFileName(argv[1]) << std::endl;

    Robologs robologs;

    return robologs.startWriter(argc, argv);

}

