//
// Created by emiel on 4-12-18.
//

#include "Reader.h"
#include "GameStateTracker.h"
#include "Drawer.h"
#include "Writer.h"
#include "Utilities.h"

#include <iostream>
#include <iomanip>

int main(){

    int fps = 60;
    int nStates = 1000;

    std::string filename = "../../2018-06-20_21-21_TIGERs_Mannheim-vs-CMμs.log";
//    std::string filename = "../../2018-06-19_16-35_RoboDragons-vs-RoboTeam_Twente.log";

    Reader reader;
    reader.openFile(filename);

    GameStateTracker tracker(fps);
    tracker.setInput(&reader);
    tracker.addInfo();

    Drawer drawer;
    Writer writer("../../log_tiger_cmus.json");


    /* Count total time recorded */
    double total_time_recorded = 0;

    std::string started_at = u::timeToString(tracker.getInfo().t_start);
    std::string stopped_at = u::timeToString(tracker.getInfo().t_stop);
    std::string game_duration = u::durationToString(tracker.getInfo().t_duration);

    /* Process */
    const std::chrono::steady_clock::time_point begin_real = std::chrono::steady_clock::now();
    tracker.tick();
    int iStates = 0;
    for(iStates = 0; (nStates <= 0 || iStates < nStates) && !reader.isEof(); iStates++){
        tracker.tick();
        writer.write(tracker.get());

        try{
            drawer.drawGameState(tracker.get());
        }catch(const std::exception& e){
            std::cout << "Drawer exception caught!" << std::endl;
            std::cout << e.what() << std::endl;
        }

    }
    const std::chrono::steady_clock::time_point end_real = std::chrono::steady_clock::now();
    const clock_t end_cpu = clock();

    /* Log */
    const double duration_real = (std::chrono::duration_cast<std::chrono::microseconds>(end_real - begin_real).count()) / 1000000.0;

    const GameInfo& info = tracker.getInfo();

    std::cout << std::setprecision(2) << std::fixed << std::endl;
    std::cout << "Duration real    = " << duration_real << std::endl;
    std::cout << "Start of log     = " << started_at << std::endl;
    std::cout << "End of log       = " << stopped_at << std::endl;
    std::cout << "Duration of log  = " << game_duration << std::endl;
    std::cout << "Packets parsed   = " << tracker.getInfo().nPackets << std::endl;

    std::cout << "Packets:" << std::endl;
    std::cout << "Total    = " << info.nPackets          << " (" << info.pps << "/s)" << std::endl;
    std::cout << "Referee  = " << info.referee_nPackets  << " (" << info.referee_pps << "/s)" << std::endl;
    std::cout << "Vision   = " << info.vision_nPackets   << " (" << info.vision_pps << "/s)" << std::endl;
    std::cout << "Geometry = " << info.geometry_nPackets << " (" << info.geometry_pps << "/s)" << std::endl;
    std::cout << "Invalid  = " << info.invalid_nPackets  << " (" << info.invalid_pps << "/s)" << std::endl;

    std::cout << "\nTerminating" << std::endl;

}
















































