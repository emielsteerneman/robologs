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
#include <unistd.h>

#include "output/ASCII.h"
#include "Robologs.h"
#include "input/Reader.h"
#include "game/GameStateTracker.h"
#include "Player.h"
#include "interface/Interface.h"
#include "output/Drawer.h"
#include "output/Writer.h"


std::string getFileName(const std::string& s) {
    char sep = '/';
    // Find the last / in the string
    size_t i = s.rfind(sep, s.length());
    // If / is found, return everything after it
    if (i != std::string::npos) {
        return(s.substr(i+1, s.length() - i));
    }
    return("");
}

//int fps = 60;
//int nStates = 1000;

Robologs::Robologs(){
    std::cout << "[Robologs] New Robologs constructed" << std::endl;
}

int Robologs::startWriter(int argc, char* argv[]) {

    // Input file
    std::string fileIn = "/home/emiel/Desktop/projects/robologs/logs/2019-07-03_14-09_ER-Force-vs-TIGERs_Mannheim.log";
    // Output file, replace extension with .json
    std::string fileOut = fileIn.substr(0, fileIn.find_last_of('.')) + ".json";
    std::cout << "[RoboLogs][startWriter] Writing file to " << fileOut << std::endl;

    Reader reader;
    if(!reader.openFile(fileIn)){
        std::cout << "[RoboLogs][startWriter] Reader could not open file! aborting.." << std::endl;
        return 1;
    }
    GameStateTracker tracker;
    tracker.setHz(30); // <- step size of the world. Max = 60Hz, because SSL-vision runs at 60Hz.
    tracker.setReader(&reader);
    Writer writer(fileOut);

    while(!reader.isEof()){
        tracker.tick();
        writer.write(&tracker.get());
        std::cout << ASCII::GamestateToString(tracker.get(), 48, 36) << std::endl;
        usleep(30 * 1e3);
    }

    writer.endJSON();

    return 0;

}




int Robologs::startInterface(int argc, char* argv[]) {
//    std::cout << "[Robologs] Starting interface" << std::endl;
//
//    QApplication app(argc, argv);
//
//    Interface* interface = new Interface();
//    interface->move((QApplication::desktop()->width() - interface->width()) / 2, (QApplication::desktop()->height() - interface->height()) / 2);
//    interface->show();
//
//    std::string fileIn = "/home/emiel/Desktop/projects/robologs/logs/2019-07-03_14-09_ER-Force-vs-TIGERs_Mannheim.log";
//
//
//    Player* player = new Player(fileIn);
//    QThread *thread = new QThread(this);
//    player->moveToThread(thread);
//    connect(this, SIGNAL(start()), player, SLOT(start()));
//    thread->start();
//
//    connect(player, SIGNAL(signalGameState(const GameState*)), interface, SLOT(updateGameState(const GameState*)), Qt::BlockingQueuedConnection);
//    connect(player, SIGNAL(signalGameInfo(const GameInfo*)), interface, SLOT(setGameInfo(const GameInfo*)), Qt::BlockingQueuedConnection);
//
//    connect(interface, SIGNAL(signalProgress(double)), player, SLOT(findProgress(double)), Qt::BlockingQueuedConnection);
//
//    emit start();
//
//    int result = app.exec();
//    return result;
    return 0;
}


int main(int argc, char* argv[]) {

//    if(argc < 2) {
//        std::cout << "Required filename not given. Terminating." << std::endl;
//        return 0;
//    }
//    std::cout << argv[1] << std::endl;
//    std::cout << getFileName(argv[1]) << std::endl;

    Robologs robologs;

    return robologs.startWriter(argc, argv);

}

//int Robologs::start(int argc, char* argv[]) {
//    std::cout << "[Robologs] Starting" << std::endl;
//
//    QApplication app(argc, argv);
//
//    Interface* interface = new Interface();
//    interface->move((QApplication::desktop()->width() - interface->width()) / 2, (QApplication::desktop()->height() - interface->height()) / 2);
//    interface->show();
//
//    Player* player = new Player();
//    QThread *thread = new QThread(this);
//    player->moveToThread(thread);
//    connect(this, SIGNAL(start()), player, SLOT(start()));
//    thread->start();
//
//    connect(player, SIGNAL(signalGameState(const GameState*)), interface, SLOT(updateGameState(const GameState*)), Qt::BlockingQueuedConnection);
//    connect(player, SIGNAL(signalGameInfo(const GameInfo*)), interface, SLOT(setGameInfo(const GameInfo*)), Qt::BlockingQueuedConnection);
//
//    connect(interface, SIGNAL(signalProgress(double)), player, SLOT(findProgress(double)), Qt::BlockingQueuedConnection);
//
//    emit start();
//
//    int result = app.exec();
//    return result;
//}