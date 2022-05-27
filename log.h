#ifndef INC_LOG_H
#define INC_LOG_H

#include <iostream>

// #define DEBUG_ON

#define LOG(msg) do {std::cout << msg;} while(0)
#define LOGLN(msg) do {LOG(msg << '\n');} while(0)

#ifdef DEBUG_ON
    #define DEBUG(msg) LOG(msg)
    #define DEBUGLN(msg) LOGLN(msg)
#else
    #define DEBUG(msg) do {} while(0)
    #define DEBUGLN(msg) do {} while(0)
#endif

#endif //INC_LOG_H
