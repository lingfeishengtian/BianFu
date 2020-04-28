//
// Created by Hunter Han on 2020-04-28.
//
#include <iostream>
#include <vector>

#include "ExecuteVisitor.h"
#include "ClassVisitor.h"
#include "../scope/BFPrimitive.h"

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
    return BFPrimitive(BFPrimitive::Primitive::INT, 0);
}

antlrcpp::Any ExecuteVisitor::visitStat(BFParser::StatContext *ctx) {
//    switch(identifyStatement(ctx)){
//        case Assign:
//            scope->variables
//    }
    std::cout << identifyStatement(ctx) << std::endl;
    return BFParserBaseVisitor::visitStat(ctx);
}

ExecuteVisitor::StatementTypes ExecuteVisitor::identifyStatement(BFParser::StatContext *ctx) {
    if(ctx->Equal() != nullptr){
        return Assign;
    }else if(ctx->classDeclaration() != nullptr){
        return ClassDeclaration;
    }else if(ctx->flowControl() != nullptr){
        return FlowControl;
    }else{
        return Expression;
    }
}
