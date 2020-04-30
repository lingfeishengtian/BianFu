//
// Created by Hunter Han on 2020-04-28.
//
#include <iostream>
#include <vector>

#include "ExecuteVisitor.h"
#include "ClassVisitor.h"
#include "../scope/primitives/BFIntPrimitive.h"
#include "../error/BianFuLog.h"
#include "../error/BianFuError.h"
#include "../scope/primitives/BFFloatPrimitive.h"

ExecuteVisitor::ExecuteVisitor(Scope s) {
    scope = &s;
}

antlrcpp::Any ExecuteVisitor::visitClassDeclaration(BFParser::ClassDeclarationContext *ctx) {
    std::string className = ctx->id->getText();
    Scope* newClass = new Scope(scope, className);
    scope->classes.insert(std::make_pair(className, newClass));
    //ClassVisitor classVisitor = ClassVisitor(newClass);
    ExecuteVisitor classExecuter = ExecuteVisitor(newClass)
    return newClass;
}

antlrcpp::Any ExecuteVisitor::visitMain(BFParser::MainContext *ctx) {
    logger.log("蝙蝠程序开始", BianFuLog::Situation::WARNING);
    std::vector<BFParser::StatContext*> statList = ctx->stat();
    for (auto & i : statList) {
        visitStat(i);
    }

//    for (auto pair : scope->classes){
//        logger.log(pair.first + " " + pair.second->to_string());
//    }

    logger.log("蝙蝠程序成功！", BianFuLog::WARNING);
    return 0;
}

antlrcpp::Any ExecuteVisitor::visitStat(BFParser::StatContext *ctx) {
    switch(identifyStatement(ctx)){
        case Assign: {
            return visitAssignment(ctx->assignment());
        }
        case Expression:{
            visitExpr(ctx->expr());
            break;
        }
        case ClassDeclaration:{
            visitClassDeclaration(ctx->classDeclaration());
        }
    }
    //std::wcout << identifyStatement(ctx) << std::endl;

    return Scope();
}

ExecuteVisitor::StatementTypes ExecuteVisitor::identifyStatement(BFParser::StatContext *ctx) {
    if(ctx->assignment() != nullptr){
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
            return s1->useOperator(ctx->op->getText(), s2);
        }
        case ParenthesisWrapped:{
            return visitExpr(ctx->expr(0));
        }
        case Identifier: {
            if (scope->variables.find(ctx->getText()) == scope->variables.end())
                throw BianFuError(scope->trace(), "第" + std::to_string(ctx->getStart()->getLine()) + "句有问题\n找不到变量" + ctx->getText());
            return scope->variables[ctx->getText()];
        }
        case Int:
            return static_cast<Scope*>(new BFIntPrimitive(BFIntPrimitive::Primitive::INT, std::stoi(ctx->getText())));
        case Float:
            return static_cast<Scope*>(new BFFloatPrimitive(std::stod(ctx->getText())));
        case FunctionCall:
            return visitDefaultFunctions(ctx->defaultFunctions());
//        case String:
//            return BFIntPrimitive(BFIntPrimitive::Primitive::INT, atoi(ctx->getText().c_str()));
    }
    return Scope();
}

ExecuteVisitor::ExpressionTypes ExecuteVisitor::identifyExpression(BFParser::ExprContext *ctx) {
    if(ctx->op != nullptr) return Operation;
    else if(ctx->OpenPar() != nullptr) return ParenthesisWrapped;
    else if(ctx->QuestionMark() != nullptr) return Ternary;
    else if(ctx->type() != nullptr) {
        if (ctx->type()->INT() != nullptr) return Int;
        else if (ctx->type()->FLOAT() != nullptr) return Float;
        else if (ctx->type()->String() != nullptr) return String;
    }
    else if(ctx->array() != nullptr) return Array;
    else if(ctx->id != nullptr) return Identifier;
    else if(ctx->defaultFunctions() != nullptr) return FunctionCall;
    else throw BianFuError(scope->isGlobal() ? "Global scope." : "Not global: " + scope->trace() + __FUNCTION__, "找不到这段程序的目的：" + ctx->getText());
}

antlrcpp::Any ExecuteVisitor::visitDefaultFunctions(BFParser::DefaultFunctionsContext *ctx) {
    if(ctx->FPrint() != nullptr){
        logger.log(static_cast<Scope*>(visitExpr(ctx->expr()))->to_string());
    }
    return nullptr;
}

antlrcpp::Any ExecuteVisitor::visitAssignment(BFParser::AssignmentContext *ctx) {
    Scope* p = visitExpr(ctx->expr()[1]);
    std::string id = ctx->expr()[0]->getText();
    p->id = id;
    p->parent = scope;
    if(ctx->KVar() != nullptr) {
        if (scope->variables.find(id) == scope->variables.end())
            return scope->variables.insert(std::make_pair(id, p));
        else
            throw BianFuError(scope->trace(),
                              "第" + std::to_string(ctx->getStart()->getLine()) + "句有问题\n" + ctx->getText() + "已存在。");
    }else{
        if (scope->variables.find(id) != scope->variables.end())
            return scope->variables[id] = p;
        else
            throw BianFuError(scope->trace(),
                              "第" + std::to_string(ctx->getStart()->getLine()) + "句有问题\n找不到" + ctx->getText() + "，不能再重新分配。");
    }
}

antlrcpp::Any ExecuteVisitor::visitClassStat(BFParser::ClassStatContext *ctx) {
    if(ctx->assignment() != nullptr){
        if(ctx->assignment()->KVar() != nullptr){

        }else{
            //TODO: THROW ERROR
        }
    }
    return BFParserBaseVisitor::visitClassStat(ctx);
}
