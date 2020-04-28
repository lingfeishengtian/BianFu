//
// Created by Hunter Han on 2020-04-28.
//
#ifndef BIANFU_BFPRIMITIVE_H
#define BIANFU_BFPRIMITIVE_H

#include "Scope.h"

class BFPrimitive : Scope{
public:
    enum Primitive{
        INT,
        CHAR
    };

    int value;

    Primitive primitiveType;
    BFPrimitive(Primitive p, int val);

    std::string to_string();
};


#endif //BIANFU_BFPRIMITIVE_H
