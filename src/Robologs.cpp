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
    tracker.setReader(&reader);
//    Writer writer(fileOut);

    for(int i = 0; i < 1000000; i++){
        tracker.tick();
        std::cout << ASCII::GamestateToString(tracker.get(), 48, 36) << std::endl;
        usleep(30 * 1e3);
    }

    return 0;

//    Player* player = new Player(fileIn);

//    if(!player->getInfo())
//        std::cout << "[RoboLogs] Could not get info from player" << std::endl;

//    player->tracker.setHz(30);
//    player->reset();
//    while(!player->reader.isEof()) {
//        player->tick();
//        writer.write(player->tracker.get());
//    }
//    writer.endJSON();
//    std::cout << "[RoboLogs][startWriter] Done writing file" << std::endl;

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

//    return 0;
}




//int Robologs::startInterface(int argc, char* argv[]) {
//    std::cout << "[Robologs] Starting interface" << std::endl;
//
//    QApplication app(argc, argv);
//
//    Interface* interface = new Interface();
//    interface->move((QApplication::desktop()->width() - interface->width()) / 2, (QApplication::desktop()->height() - interface->height()) / 2);
//    interface->show();
//
//    std::string file = "2018-06-20_11-18_TIGERs_Mannheim-vs-ER-Force.log";
//    std::string fileIn = "/media/emiel/HDD500/robocup_logs/" + file;
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
//}


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

