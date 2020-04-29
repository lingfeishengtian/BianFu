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
        Float,
        Int,
        String,
        Array,
        FunctionCall
    };

    BianFuLog logger = BianFuLog();

    antlrcpp::Any visitStat(BFParser::StatContext *ctx) override;
    antlrcpp::Any visitDefaultFunctions(BFParser::DefaultFunctionsContext *ctx) override;
    antlrcpp::Any visitAssignment(BFParser::AssignmentContext *ctx) override;

    StatementTypes identifyStatement(BFParser::StatContext *ctx);
    ExpressionTypes identifyExpression(BFParser::ExprContext *ctx);
    void exit(int);
public:
    antlrcpp::Any visitExpr(BFParser::ExprContext *ctx) override;
    antlrcpp::Any visitMain(BFParser::MainContext *ctx) override;
    antlrcpp::Any visitClassDeclaration(BFParser::ClassDeclarationContext *ctx) override;
};


#endif //BIANFU_EXECUTEVISITOR_H
