//
// Created by emiel on 17-12-18.
//

#ifndef ROBOLOGS_UTILITIES_H
#define ROBOLOGS_UTILITIES_H

#include <chrono>
#include <iostream>

namespace u {

std::string timeToString(double timestamp, char const *format = "%Y-%m-%d %H:%M:%S");

std::string durationToString(double duration);

}

#endif //ROBOLOGS_UTILITIES_H
