//
// Created by Hunter Han on 2020-04-28.
//
#ifndef BIANFU_BFPRIMITIVE_H
#define BIANFU_BFPRIMITIVE_H

#include "Scope.h"

class BFPrimitive : public Scope{
public:
    enum Primitive{
        INT,
        CHAR,
        BOOL
    };

    int value;

    Primitive primitiveType;
    BFPrimitive(Primitive, int, const std::string&, Scope*);
    BFPrimitive(Primitive, int);

    std::string to_string();
    BFPrimitive* useOperator(const std::string&, Scope*) override;
};


#endif //BIANFU_BFPRIMITIVE_H
