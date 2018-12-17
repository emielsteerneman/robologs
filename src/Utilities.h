//
// Created by emiel on 17-12-18.
//

#ifndef ROBOLOGS_UTILITIES_H
#define ROBOLOGS_UTILITIES_H

#include <chrono>

namespace u {

std::string timeToString(double timestamp, char const *format = "%Y-%m-%d %H:%M:%S"){
    char buff[32];
    time_t time = (time_t)timestamp;
    strftime(buff, 32, format, localtime(&time));
    return std::string(buff);
}

std::string durationToString(double duration){

    int hours = duration / 3600;
    duration -= hours * 3600;
    int minutes = duration/60;
    duration -= minutes * 60;
    int seconds = duration/1;
    duration -= seconds;
    int milliseconds = duration * 1000;

    return std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);// + "." + std::to_string(milliseconds);
}

}

#endif //ROBOLOGS_UTILITIES_H
