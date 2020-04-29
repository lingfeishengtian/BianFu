//
// Created by Hunter Han on 2020-04-28.
//

#include "BFPrimitive.h"
#include "../error/BianFuError.h"

BFPrimitive::BFPrimitive(BFPrimitive::Primitive p, int val, const std::string &ID, Scope* par) {
    primitiveType = p;
    value = val;
    id = ID;
    parent = par;
}

BFPrimitive::BFPrimitive(BFPrimitive::Primitive p, int val) {
    primitiveType = p;
    value = val;
}

std::string BFPrimitive::to_string() {
    return std::to_string(value);
}

BFPrimitive* BFPrimitive::useOperator(const std::string &op, Scope *scope) {
    try {
        auto second = reinterpret_cast<BFPrimitive*>(scope);
        if(second->primitiveType == Primitive::BOOL) {
            if (op == "=="){
                //BFPrimitive ret(Primitive::BOOL, value == second->value);
                return new BFPrimitive(Primitive::BOOL, value == second->value);
            }
            //TODO: Add more operators later
        } else {
            Primitive finalType = this->primitiveType == Primitive::CHAR && second->primitiveType == Primitive::CHAR ? Primitive::CHAR : Primitive::INT;
            int newVal = this->value;
            if(op == "+") newVal += second->value;
            else if(op == "-") newVal -= second->value;
            else if(op == "*") newVal *= second->value;
            else if(op == "/") newVal /= second->value;
            //TODO: Custom operators
            //BFPrimitive ret(finalType, newVal);
            return new BFPrimitive(finalType, newVal);
        }
    }catch (std::bad_cast&){
        throw BianFuError(this->trace(), "操作失败，不能在" + this->id + "和" + scope->id + "做" + op);
    }catch (std::exception&){
        throw BianFuError(this->trace(), "不知问题在哪儿？ " + this->id + ":" + std::to_string(this->value) + op + scope->id);
    }
    throw BianFuError(this->trace(), "不能在" + this->id + "和" + scope->id + "做" + op);
}