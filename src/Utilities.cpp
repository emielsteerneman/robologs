//
// Created by emiel on 17-12-18.
//

#include "Utilities.h"

namespace u {

    std::string timeToString(double timestamp, char const *format){
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

        std::string sHours = std::to_string(hours);
        if(sHours.length() == 1) sHours = "0" + sHours;

        std::string sMinutes = std::to_string(minutes);
        if(sMinutes.length() == 1) sMinutes = "0" + sMinutes;

        std::string sSeconds = std::to_string(seconds);
        if(sSeconds.length() == 1) sSeconds = "0" + sSeconds;

        return sHours + ":" + sMinutes + ":" + sSeconds;// + "." + std::to_string(milliseconds);
    }

}