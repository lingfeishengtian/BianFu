//
// Created by Hunter Han on 4/28/2020.
//

#ifndef BIANFU_BIANFULOG_H
#define BIANFU_BIANFULOG_H

#include <iostream>

class BianFuLog {
public:
    enum TextColor{
        BLACK = 30,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        WHITE = 37
    };
    enum Situation{
        WARNING = TextColor::YELLOW,
        ERROR = TextColor::RED,
        MESSAGE =  TextColor::WHITE
    };

    void log(const std::string&);
    void log(const std::string&, TextColor textColor);
    void log(const std::string&, Situation situation);
    void log(int);
    void log(double);
private:
    void internalLog(const std::string&, std::string*);
};


#endif //BIANFU_BIANFULOG_H
