//
// Created by Hunter Han on 2020-04-28.
//

#include "BFIntPrimitive.h"
#include "../../error/BianFuError.h"
#include "BFFloatPrimitive.h"

BFIntPrimitive::BFIntPrimitive(BFIntPrimitive::Primitive p, int val, const std::string &ID, Scope* par) {
    primitiveType = p;
    value = val;
    id = ID;
    parent = par;
    setType(p);
}

BFIntPrimitive::BFIntPrimitive(BFIntPrimitive::Primitive p, int val) {
    primitiveType = p;
    value = val;
    setType(p);
}

void BFIntPrimitive::setType(BFIntPrimitive::Primitive p) {
    switch (p){
        case INT:
            type = "INT";
            break;
        case CHAR: {
            type = "CHAR";
            break;
        }
        case BOOL:
            type = "BOOL";
            break;
    }
}

std::string BFIntPrimitive::to_string() {
    if(primitiveType == BOOL){
        return value == 0 ? "非" : "是";
    }else if(primitiveType == INT){
        return std::to_string(value);
    }else{
        wchar_t wchar = (wchar_t) value;
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::string narrow = converter.to_bytes(wchar);
        return narrow;
    }
}

Scope* BFIntPrimitive::useOperator(const std::string &op, Scope *scope) {
    auto second = dynamic_cast<BFFloatPrimitive*>(scope);
    if(second != nullptr) {
        BFFloatPrimitive bfFloatPrimitive = BFFloatPrimitive(value);
        return bfFloatPrimitive.useOperator(op, second);
    }else {
        try {
            auto second = dynamic_cast<BFIntPrimitive *>(scope);
            if (second->primitiveType == Primitive::BOOL) {
                if (op == "==") {
                    //BFIntPrimitive ret(Primitive::BOOL, value == second->value);
                    return new BFIntPrimitive(Primitive::BOOL, value == second->value);
                }
                //TODO: Add more operators later
            } else {
                Primitive finalType = this->primitiveType == Primitive::CHAR && second->primitiveType == Primitive::CHAR
                                      ? Primitive::CHAR : Primitive::INT;
                int newVal = this->value;
                if (op == "+") newVal += second->value;
                else if (op == "-") newVal -= second->value;
                else if (op == "*") newVal *= second->value;
                else if (op == "/") newVal /= second->value;
                //TODO: Custom operators
                //BFIntPrimitive ret(finalType, newVal);
                return new BFIntPrimitive(finalType, newVal);
            }
        } catch (std::bad_cast &) {
            throw BianFuError(this->trace(), "操作失败，不能在" + this->id + "和" + scope->id + "做" + op);
        } catch (std::exception &) {
            throw BianFuError(this->trace(),
                              "不知问题在哪儿？ " + this->id + ":" + std::to_string(this->value) + op + scope->id);
        }
        throw BianFuError(this->trace(), "不能在" + this->id + "和" + scope->id + "做" + op);
    }
}
