//
// Created by hunterhan on 5/2/20.
//

#ifndef BIANFU_COMPILEVISITOR_H
#define BIANFU_COMPILEVISITOR_H

#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/Support/TargetSelect.h>

#include <BFParser.h>
#include <BFParserBaseVisitor.h>
#include "../error/BianFuLog.h"
#include "../scope/CompilerScope.h"

class CompileVisitor : BFParserBaseVisitor{
public:
    BianFuLog logger = BianFuLog();
    static std::map<std::string, llvm::FunctionCallee> systemFunctions;
    static std::map<std::string, llvm::Value*> systemStrings;

    CompileVisitor();
    CompileVisitor(std::string modName, CompilerScope* s);

    /// main isn't clearly defined in BianFu, it's really just code that's not a class.
    antlrcpp::Any visitMain(BFParser::MainContext *ctx) override;

private:
    CompilerScope* scope;

    antlrcpp::Any visitStat(BFParser::StatContext *ctx) override;
    antlrcpp::Any visitExpr(BFParser::ExprContext *ctx) override;
    antlrcpp::Any visitAssignment(BFParser::AssignmentContext *ctx) override;
    antlrcpp::Any visitTypeDef(BFParser::TypeDefContext *ctx) override;
    antlrcpp::Any visitDefaultFunctions(BFParser::DefaultFunctionsContext *ctx) override;
    antlrcpp::Any visitClassStat(BFParser::ClassStatContext *ctx) override;
    antlrcpp::Any visitClassDeclaration(BFParser::ClassDeclarationContext *ctx) override;
    antlrcpp::Any visitFunctionDeclaration(BFParser::FunctionDeclarationContext *ctx) override;
    antlrcpp::Any visitBlock(BFParser::BlockContext *ctx) override;
    antlrcpp::Any visitArray(BFParser::ArrayContext *ctx) override;

    llvm::Value* opDoubles(llvm::Value* f, llvm::Value* s, std::string op);
};


#endif //BIANFU_COMPILEVISITOR_H
