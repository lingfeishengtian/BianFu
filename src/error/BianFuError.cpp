//
// Created by Hunter Han on 4/28/2020.
//

#include "BianFuError.h"

BianFuError::BianFuError(const std::string &MCE, const std::string &eR) {
    methodCausingError = MCE;
    extraReason = eR;
}

void BianFuError::logError() {
    log.log(extraReason, BianFuLog::Situation::ERROR);
    log.log("");
    log.log(methodCausingError, BianFuLog::Situation::MESSAGE);
}
