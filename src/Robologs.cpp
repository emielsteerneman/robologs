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

#include "Robologs.h"
#include "input/Reader.h"
#include "game/GameStateTracker.h"
#include "Player.h"
#include "interface/Interface.h"
#include "output/Drawer.h"
#include "output/Writer.h"


int fps = 60;
int nStates = 1000;

Robologs::Robologs() : QObject(){
    std::cout << "[Robologs] New Robologs constructed" << std::endl;

//    Reader reader;
//    reader.openFile("../../2018-06-20_21-21_TIGERs_Mannheim-vs-CMμs.log");
//
//    GameStateTracker tracker(60);
//    tracker.setInput(&reader);
//    tracker.addInfo();
//
//    auto *timer = new QTimer();
//    connect(timer, SIGNAL(timeout()), &iface, SLOT(update())); // Triggers PaintEvent
//    connect(timer, SIGNAL(timeout()), &iface, SLOT(updateUi()));
//    timer->start(1000); // delay in ms

}

int Robologs::start(int argc, char* argv[]) {
    std::cout << "[Robologs] Starting" << std::endl;

    QApplication app(argc, argv);

    Player player;

    QThread *thread = new QThread(this);


    Interface interface;
    interface.setWindowState(Qt::WindowMaximized);
    interface.show();

    connect(&player, SIGNAL(nextGameState(const GameState&)), &interface, SLOT(updateGameState(const GameState&)));

    player.start();

    return app.exec();
}


int main(int argc, char* argv[]) {

    Robologs robologs;

    return robologs.start(argc, argv);


//    std::string started_at = u::timeToString(tracker.getInfo().t_start);
//    std::string stopped_at = u::timeToString(tracker.getInfo().t_stop);
//    std::string game_duration = u::durationToString(tracker.getInfo().t_duration);
//
//    /* Process */
//    const std::chrono::steady_clock::time_point begin_real = std::chrono::steady_clock::now();
//
//    tracker.tick();
//    int iStates = 0;
//    for(iStates = 0; (nStates <= 0 || iStates < nStates) && !reader.isEof(); iStates++){
//        tracker.tick();
////        writer.write(tracker.get());
//
//        try{
////            drawer.drawGameState(tracker.get());
//        }catch(const std::exception& e){
//            std::cout << "Drawer exception caught!" << std::endl;
//            std::cout << e.what() << std::endl;
//        }
//
//    }
//    const std::chrono::steady_clock::time_point end_real = std::chrono::steady_clock::now();
//    const clock_t end_cpu = clock();
//
//    /* Log */
//    const double duration_real = (std::chrono::duration_cast<std::chrono::microseconds>(end_real - begin_real).count()) / 1000000.0;
//
//    const GameInfo& info = tracker.getInfo();
//
//    std::cout << std::setprecision(2) << std::fixed << std::endl;
//    std::cout << "Duration real    = " << duration_real << std::endl;
//    std::cout << "Start of log     = " << started_at << std::endl;
//    std::cout << "End of log       = " << stopped_at << std::endl;
//    std::cout << "Duration of log  = " << game_duration << std::endl;
//    std::cout << "Packets parsed   = " << tracker.getInfo().nPackets << std::endl;
//
//    std::cout << "Packets:" << std::endl;
//    std::cout << "Total    = " << info.nPackets          << " (" << info.pps << "/s)" << std::endl;
//    std::cout << "Referee  = " << info.referee_nPackets  << " (" << info.referee_pps << "/s)" << std::endl;
//    std::cout << "Vision   = " << info.vision_nPackets   << " (" << info.vision_pps << "/s)" << std::endl;
//    std::cout << "Geometry = " << info.geometry_nPackets << " (" << info.geometry_pps << "/s)" << std::endl;
//    std::cout << "Invalid  = " << info.invalid_nPackets  << " (" << info.invalid_pps << "/s)" << std::endl;
//
//    std::cout << "\nTerminating" << std::endl;

}
















































