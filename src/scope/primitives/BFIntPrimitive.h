//
// Created by Hunter Han on 2020-04-28.
//
#ifndef BIANFU_BFINTPRIMITIVE_H
#define BIANFU_BFINTPRIMITIVE_H

#include "../Scope.h"

class BFIntPrimitive : public Scope{
public:
    enum Primitive{
        INT,
        CHAR,
        BOOL
    };

    int value;

    Primitive primitiveType;
    BFIntPrimitive(Primitive, int, const std::string&, Scope*);
    BFIntPrimitive(Primitive, int);

    std::string to_string();
    Scope* useOperator(const std::string&, Scope*) override;
};


#endif //BIANFU_BFINTPRIMITIVE_H
