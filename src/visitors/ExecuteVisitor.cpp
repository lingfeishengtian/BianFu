//
// Created by Hunter Han on 2020-04-28.
//
#include <iostream>
#include <vector>

#include "ExecuteVisitor.h"
#include "ClassVisitor.h"
#include "../scope/BFPrimitive.h"
#include "../error/BianFuLog.h"
#include "../error/BianFuError.h"

ExecuteVisitor::ExecuteVisitor(Scope s) {
    scope = &s;
}

antlrcpp::Any ExecuteVisitor::visitClassDeclaration(BFParser::ClassDeclarationContext *ctx) {
    std::string className = ctx->ID()->getText();
    Scope newClass = Scope(scope, className);
    scope->classes.insert(std::pair<std::string, Scope*>(className, &newClass));
    //ClassVisitor classVisitor = ClassVisitor(newClass);

    return newClass;
}

antlrcpp::Any ExecuteVisitor::visitMain(BFParser::MainContext *ctx) {
    std::vector<BFParser::StatContext*> statList = ctx->stat();
    for (auto & i : statList) {
        visitStat(i);
    }

    logger.log("蝙蝠程序成功！", BianFuLog::WARNING);
    return 0;
}

antlrcpp::Any ExecuteVisitor::visitStat(BFParser::StatContext *ctx) {
    switch(identifyStatement(ctx)){
        case Assign: {
//            try {
            Scope* p = visitExpr(ctx->expr()[1]);
            std::string id = ctx->expr()[0]->getText();
            scope->variables.insert(std::make_pair(id, p));
//            }catch(std::exception e){
//                logger.log(e.);
//                //TODO: Handle error
//            }
            break;
        }
        case Expression:{
            visitExpr(ctx->expr()[0]);
            break;
        }
    }
    //std::wcout << identifyStatement(ctx) << std::endl;

    return Scope();
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

antlrcpp::Any ExecuteVisitor::visitExpr(BFParser::ExprContext *ctx) {
    switch (identifyExpression(ctx)){
        case Operation: {
            Scope* s1 = visitExpr(ctx->expr()[0]);
            Scope* s2 = visitExpr(ctx->expr()[1]);
            Scope* s3 = s1->useOperator(ctx->op->getText(), s2);
            return s3;
        }
        case ParenthesisWrapped:{
            return visitExpr(ctx->expr(0));
        }
        case Identifier:
            return scope->variables[ctx->getText()];
        case Int:
            int a = std::stoi(ctx->getText());
            return static_cast<Scope*>(new BFPrimitive(BFPrimitive::Primitive::INT, a));
//        case String:
//            return BFPrimitive(BFPrimitive::Primitive::INT, atoi(ctx->getText().c_str()));
    }
    return Scope();
}

ExecuteVisitor::ExpressionTypes ExecuteVisitor::identifyExpression(BFParser::ExprContext *ctx) {
    if(ctx->op != nullptr) return Operation;
    else if(ctx->OpenPar() != nullptr) return ParenthesisWrapped;
    else if(ctx->QuestionMark() != nullptr) return Ternary;
    else if(ctx->INT() != nullptr) return Int;
    else if(ctx->String() != nullptr) return String;
    else if(ctx->array() != nullptr) return Array;
    else if(ctx->id != nullptr) return Identifier;
    else throw BianFuError(scope->isGlobal() ? "Global scope." : "Not global: " + scope->trace() + __FUNCTION__, "找不到这段程序的目的：" + ctx->getText());
}