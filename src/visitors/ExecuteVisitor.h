//
// Created by Hunter Han on 2020-04-28.
//
#include "BFParserBaseVisitor.h"
#include "../scope/Scope.h"

#ifndef BIANFU_EXECUTEVISITOR_H
#define BIANFU_EXECUTEVISITOR_H


class ExecuteVisitor : BFParserBaseVisitor {
public:
    Scope *scope;
    ExecuteVisitor(Scope s);

private:
    antlrcpp::Any visitStat(BFParser::StatContext *ctx) override;
    enum StatementTypes{
        
    };

public:
    antlrcpp::Any visitMain(BFParser::MainContext *ctx) override;
    antlrcpp::Any visitClassDeclaration(BFParser::ClassDeclarationContext *ctx) override;
};


#endif //BIANFU_EXECUTEVISITOR_H
