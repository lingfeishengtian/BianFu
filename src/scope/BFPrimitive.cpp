//
// Created by Hunter Han on 2020-04-28.
//

#include "BFPrimitive.h"

BFPrimitive::BFPrimitive(BFPrimitive::Primitive p, int val) {
    primitiveType = p;
    value = val;
}

std::string BFPrimitive::to_string() {
    return std::to_string(value);
}
