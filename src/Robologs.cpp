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


Robologs::Robologs(){
    std::cout << "[Robologs] New Robologs constructed" << std::endl;
}

int Robologs::startWriter(int argc, char* argv[]) {

    // Input file
    // Input file
//    std::string fileIn = "/home/emiel/Desktop/projects/robologs/logs/2019-07-03_14-09_ER-Force-vs-TIGERs_Mannheim.log";
//    std::string fileIn  = "/home/selina/Desktop/AML/Project/logs/logfiles/2019-07-03_14-09_ER-Force-vs-TIGERs_Mannheim.log";
    std::string fileIn = "/home/selina/Desktop/AML/Project/logs/logfiles/2019-07-06_04-54_ER-Force-vs-TIGERs_Mannheim.log";
//    std::string fileIn = "/home/selina/Desktop/AML/Project/logs/logfiles/2019-07-06_11-32_ER-Force-vs-ZJUNlict.log";
//    std::string fileIn = "/home/selina/Desktop/AML/Project/logs/logfiles/2019-07-07_05-26_ER-Force-vs-MRL.log";
//    std::string fileIn = "/home/selina/Desktop/AML/Project/logs/logfiles/2019-07-07_06-34_ER-Force-vs-ZJUNlict.log";
    // Output file, replace extension with .json
    std::string fileOut = fileIn.substr(0, fileIn.find_last_of('.')) + ".json";
    std::cout << "[RoboLogs][startWriter] Writing file to " << fileOut << std::endl;

    Reader reader;
    if(!reader.openFile(fileIn)){
        std::cout << "[RoboLogs][startWriter] Reader could not open file! aborting.." << std::endl;
        return 1;
    }
    GameStateTracker tracker(reader);
    tracker.setHz(30); // <- step size of the world. Max = 60Hz, because SSL-vision runs at 60Hz.
    Writer writer(fileOut);

    while(!reader.isEof()){
        tracker.tick();
        writer.write(&tracker.get());
//        std::cout << ASCII::GamestateToString(tracker.get(), 48, 36) << std::endl;
//        usleep(30 * 1e3);
    }

    writer.endJSON();

    return 0;

}


int Robologs::startInterface(int argc, char* argv[]) {
    std::cout << "[Robologs] Starting interface" << std::endl;

    QApplication app(argc, argv);

    std::string fileIn = "/home/selina/Desktop/AML/Project/logs/logfiles/2019-07-03_14-09_ER-Force-vs-TIGERs_Mannheim.log";

    /** Create reader **/
    Reader reader;
    if(!reader.openFile(fileIn)){
        std::cout << "[RoboLogs][startWriter] Reader could not open file! aborting.." << std::endl;
        return 1;
    }

    /** Create timeline for interface **/
    GameInfoTracker infoTracker(reader);
    infoTracker.initTimeline();

    /** Create tracker **/
    GameStateTracker tracker(reader);
    tracker.setHz(30); // <- step size of the world. Max = 60Hz, because SSL-vision runs at 60Hz.

    /** Create player **/
    Player* player = new Player(tracker, infoTracker, 30);
    QThread *thread = new QThread(this);
    player->moveToThread(thread);
    connect(this, SIGNAL(start()), player, SLOT(start()));
    thread->start();

    /** Create interface **/
    Interface* interface = new Interface();
    interface->move((QApplication::desktop()->width() - interface->width()) / 2, (QApplication::desktop()->height() - interface->height()) / 2);
    interface->show();
    interface->setGameInfo(infoTracker.get());

    /** Connect player to interface **/
    connect(player, SIGNAL(signalGameState(const GameState*)), interface, SLOT(updateGameState(const GameState*)), Qt::BlockingQueuedConnection);
    connect(interface, SIGNAL(signalProgress(double)), player, SLOT(findProgress(double)), Qt::BlockingQueuedConnection);

    /** Start player **/
    emit start();

    int result = app.exec();
    return result;
}


int main(int argc, char* argv[]) {

//    if(argc < 2) {
//        std::cout << "Required filename not given. Terminating." << std::endl;
//        return 0;
//    }
//    std::cout << argv[1] << std::endl;
//    std::cout << getFileName(argv[1]) << std::endl;

    Robologs robologs;

    return robologs.startInterface(argc, argv);
//    return robologs.startWriter(argc, argv);

}