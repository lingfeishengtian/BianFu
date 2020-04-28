//
// Created by Hunter Han on 2020-04-28.
//

#ifndef BIANFU_BFVARIABLE_H
#define BIANFU_BFVARIABLE_H


class BFVariable {
public:
    enum Primitive{
        INT,
        CHAR,
        STRING,
        ARRAY,
        MAP
    };

    bool isPrimitive();
};


#endif //BIANFU_BFVARIABLE_H
