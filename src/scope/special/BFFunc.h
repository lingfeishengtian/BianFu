//
// Created by Hunter Han on 2020-04-29.
//

#ifndef BIANFU_BFFUNC_H
#define BIANFU_BFFUNC_H

#include "BFParser.h"

class BFFunc {
public:
    std::string ID;
    std::string returnType;
    std::map<std::string, std::string> paramList;
    BFParser::BlockContext* blockContext;

    enum AccessPerm{
        Public,
        Private
    };

    BFFunc();
    BFFunc(const std::string iD, BFParser::BlockContext* bC);
};


#endif //BIANFU_BFFUNC_H
