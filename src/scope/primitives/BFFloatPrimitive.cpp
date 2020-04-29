//
// Created by Hunter Han on 4/29/2020.
//

#include "BFFloatPrimitive.h"
#include "BFIntPrimitive.h"
#include "../../error/BianFuError.h"

BFFloatPrimitive::BFFloatPrimitive(double v) {
    value = v;
    type = "FLOAT";
}

BFFloatPrimitive::BFFloatPrimitive(const std::string &iD, double v, Scope *p) {
    parent = p;
    id = iD;
    value = v;
    type = "FLOAT";
}

BFFloatPrimitive *BFFloatPrimitive::useOperator(const std::string &op, Scope *scope) {
    BFIntPrimitive* intPrim = dynamic_cast<BFIntPrimitive*>(scope);
    if(intPrim != nullptr) {
        //Local float primitive, will be discarded after function finishes running.
        BFFloatPrimitive tmpCast = BFFloatPrimitive(intPrim->id, intPrim->value, intPrim->parent);
        return operateOn(op, &tmpCast);
    }else {
        try {
            return operateOn(op, dynamic_cast<BFFloatPrimitive *>(scope));
        } catch (std::bad_cast &) {
            throw BianFuError(trace(), "不支持在" + scope->id + "和" + id + "用" + op);
        }
    }
}

BFFloatPrimitive *BFFloatPrimitive::operateOn(const std::string &op, BFFloatPrimitive *tmp) {
    double finVal = value;
    if(op == "+") finVal += tmp->value;
    else if(op == "-") finVal -= tmp->value;
    else if(op == "/") finVal /= tmp->value;
    else if(op == "*") finVal *= tmp->value;
    else throw BianFuError(trace(), op + "不能用再" + tmp->id + "和" + id + "上！");
    return new BFFloatPrimitive(finVal);
}

std::string BFFloatPrimitive::to_string() {
    return std::to_string(value);
}
