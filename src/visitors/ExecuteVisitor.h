//
// Created by Hunter Han on 2020-04-28.
//
#include "BFParserBaseVisitor.h"
#include "../scope/Scope.h"
#include "../error/BianFuLog.h"

#ifndef BIANFU_EXECUTEVISITOR_H
#define BIANFU_EXECUTEVISITOR_H


class ExecuteVisitor : BFParserBaseVisitor {
public:
    Scope *scope;
    explicit ExecuteVisitor(Scope s);

private:
    enum StatementTypes{
        Assign,
        Expression,
        ClassDeclaration,
        FlowControl
    };

    enum ExpressionTypes{
        Operation,
        ParenthesisWrapped,
        Ternary,
        Identifier,
        Int,
        String,
        Array
    };

    BianFuLog logger = BianFuLog();

    antlrcpp::Any visitStat(BFParser::StatContext *ctx) override;
    StatementTypes identifyStatement(BFParser::StatContext *ctx);
    ExpressionTypes identifyExpression(BFParser::ExprContext *ctx);
    void exit(int);
public:
    antlrcpp::Any visitExpr(BFParser::ExprContext *ctx) override;
    antlrcpp::Any visitMain(BFParser::MainContext *ctx) override;
    antlrcpp::Any visitClassDeclaration(BFParser::ClassDeclarationContext *ctx) override;
};


#endif //BIANFU_EXECUTEVISITOR_H
