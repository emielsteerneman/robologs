//
// Created by emiel on 4-12-18.
//

#include <iostream>

#include "Reader.h"
#include "GameStateTracker.h"

int main(){
    std::cout << "test.cpp main" << std::endl;

    std::string filename = "../../2018-06-20_21-21_TIGERs_Mannheim-vs-CMμs.log";
    Reader reader;
    reader.openFile(filename);

    GameStateTracker tracker;
    tracker.setReader(&reader);

    for(int i = 0; i < 30000; i++){
        tracker.tick();
    }

//    while(tracker.tick() != -2);

    std::cout << "\nTerminating" << std::endl;

}

