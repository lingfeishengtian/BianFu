//
// Created by Hunter Han on 2020-04-28.
//
#include <iostream>
#include <vector>

#include "ExecuteVisitor.h"
#include "ClassVisitor.h"

ExecuteVisitor::ExecuteVisitor(Scope s) {
    scope = &s;
}

antlrcpp::Any ExecuteVisitor::visitClassDeclaration(BFParser::ClassDeclarationContext *ctx) {
    std::string className = ctx->id()->getText();
    Scope newClass = Scope(scope);
    scope->classes.insert(std::pair<std::string, Scope>(className, &newClass));
    //ClassVisitor classVisitor = ClassVisitor(newClass);

    return newClass;
}

antlrcpp::Any ExecuteVisitor::visitMain(BFParser::MainContext *ctx) {
    std::vector<BFParser::StatContext*> statList = ctx->stat();
    for (int i = 0; i < statList.size(); ++i) {
        visitStat(statList[i]);
    }
    return BFParserBaseVisitor::visitMain(ctx);
}

antlrcpp::Any ExecuteVisitor::visitStat(BFParser::StatContext *ctx) {

    return BFParserBaseVisitor::visitStat(ctx);
}
