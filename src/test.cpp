//
// Created by emiel on 4-12-18.
//

#include "Reader.h"
#include "GameStateTracker.h"
#include "Drawer.h"

#include <iostream>
#include <iomanip>
#include <chrono>

int main(){
    std::cout << "test.cpp main" << std::endl;

    int fps = 15;
    int nStates = 2000;

    std::string filename = "../../2018-06-20_21-21_TIGERs_Mannheim-vs-CMμs.log";
    Reader reader;
    reader.openFile(filename);

    GameStateTracker tracker(fps);
    tracker.setReader(&reader);

    Drawer drawer;

    tracker.addTimeline();

    /* Count total time recorded */
    double total_time_recorded = 0;
    {
        double previous = std::get<0>(tracker.gameState.timeline.back());
        for (unsigned long i = tracker.gameState.timeline.size() - 1; 0 < i; i--) {
            auto tup = tracker.gameState.timeline.at(i);
            if(std::get<3>(tup) == RecordingState::RECORDING)
                total_time_recorded += previous - std::get<0>(tup);
            previous = std::get<0>(tup);
        }
    }


    /* Process */
    const std::chrono::steady_clock::time_point begin_real = std::chrono::steady_clock::now();
    const clock_t begin_cpu = clock();
    tracker.tick();
    double timestamp_first = tracker.gameState.timestamp;
    int iStates = 0;
    for(iStates = 0; (nStates <= 0 || iStates < nStates) && !reader.isEof(); iStates++){
        tracker.tick();
        drawer.drawGameState(tracker.gameState);
    }
    const std::chrono::steady_clock::time_point end_real = std::chrono::steady_clock::now();
    const clock_t end_cpu = clock();

    /* Log */
    const double duration_real = (std::chrono::duration_cast<std::chrono::microseconds>(end_real - begin_real).count()) / 1000000.0;
    const double duration_cpu = (float)(end_cpu - begin_cpu) / CLOCKS_PER_SEC;
    const double gameDuration = tracker.gameState.timestamp - timestamp_first;
    const double fpsActual = iStates / (total_time_recorded);
    const double fpsAccuracy = fpsActual < fps ? (100 * fpsActual / fps) : (100 * fps / fpsActual);

    std::cout << std::setprecision(2) << std::fixed << std::endl;
    std::cout << "Duration real    = " << duration_real << std::endl;
    std::cout << "Duration CPU     = " << duration_cpu << std::endl;
    std::cout << "Actual FPS       = " << fpsActual << std::endl;
    std::cout << "Accuracy FPS     = " << fpsAccuracy << "%" << std::endl;
    std::cout << "Packets expected = " << (nStates * 480/fps) << std::endl;
    std::cout << "Packets processed= " << reader.read << std::endl;
    std::cout << "Packets parsed   = " << tracker.parsed << std::endl;
    std::cout << "Packets / second in real = " << (tracker.parsed / duration_real) << std::endl;
    std::cout << "Packets / second in game = " << (tracker.parsed / total_time_recorded) << std::endl;
    std::cout << "Packets / state  = " << (tracker.parsed / (iStates + 1)) << std::endl;
    std::cout << "\nTerminating" << std::endl;

}
















































