//
// Created by Hunter Han on 2020-04-29.
//

#include "BFFunc.h"

BFFunc::BFFunc(const std::string iD, BFParser::BlockContext* bC) {
    ID = iD;
    blockContext = bC;
    returnType = "VOID";
}

BFFunc::BFFunc() {
    blockContext = nullptr;
}
