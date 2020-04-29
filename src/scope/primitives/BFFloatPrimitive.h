//
// Created by Hunter Han on 4/29/2020.
//

#ifndef BIANFU_BFFLOATPRIMITIVE_H
#define BIANFU_BFFLOATPRIMITIVE_H


#include "../Scope.h"

class BFFloatPrimitive : public Scope {
public:
    double value;

    BFFloatPrimitive(double);
    BFFloatPrimitive(const std::string&, double, Scope*);
    BFFloatPrimitive *useOperator(const std::string&, Scope*) override;
    std::string to_string() override;

private:
    BFFloatPrimitive *operateOn(const std::string&, BFFloatPrimitive*);
};


#endif //BIANFU_BFFLOATPRIMITIVE_H
