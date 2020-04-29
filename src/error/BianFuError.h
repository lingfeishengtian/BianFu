//
// Created by Hunter Han on 4/28/2020.
//

#ifndef BIANFU_BIANFUERROR_H
#define BIANFU_BIANFUERROR_H

#include <iostream>
#include "BianFuLog.h"

class BianFuError : std::exception{
private:
    std::string methodCausingError;
    std::string extraReason;

    BianFuLog log = BianFuLog();
public:
    BianFuError(const std::string &MCE, const std::string &eR);
    void logError();
};


#endif //BIANFU_BIANFUERROR_H
