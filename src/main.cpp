#include <string>
#include <vector>
#include <iostream>
#include <signal.h>

#include "antlr4-runtime.h"

#include "BFLexer.h"
#include "BFParser.h"

#include "error/BianFuErrorListener.h"
#include "error/BianFuError.h"
#include "scope/Scope.h"
#include "visitors/ExecuteVisitor.h"
#include "visitors/CompileVisitor.h"

using namespace antlr4;
BianFuLog logger = BianFuLog();

void handleSIGILL(int sig){
    logger.log("程序出问题，清发给开发人: A return type was probably not found. A SIGILL fault occurred. Use debug to trace this error. Exit code: " + std::to_string(sig),
            BianFuLog::Situation::ERROR);
    exit(sig);
}

int main(int , const char **) {
    #ifdef _WIN32
        _setmode(_fileno(stdout), _O_U16TEXT);
        std::wcout << L"蝙蝠Windows版没有字体颜色。抱歉。" << std::endl;
    #endif

    ANTLRInputStream input("变量 t = 'j'\n变量 从 = {'H', 'P', t, 2}\n变量 阿 = 34.2\n变量 不 = 4\n出(2 + 5.1)\n出()\n出(从)\n出()");
    BFLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    BianFuErrorListener bianFuErrorListener = BianFuErrorListener();
    signal(SIGILL, handleSIGILL);

    tokens.fill();
    for (auto token : tokens.getTokens()) {
        std::cout << token->toString() << " " << std::endl;
    }

    BFParser parser(&tokens);
    parser.removeErrorListeners();
    parser.addErrorListener(&bianFuErrorListener);

    tree::ParseTree *tree = parser.main();

    logger.log(tree->toStringTree(&parser), BianFuLog::Situation::WARNING);

    Scope globalScope = Scope();
    CompileVisitor executeVisitor = CompileVisitor();
    try{
        executeVisitor.visitMain(dynamic_cast<BFParser::MainContext *>(tree));
    }catch (BianFuError &error){
        error.logError();
        logger.log("蝙蝠程序失败", BianFuLog::Situation::ERROR);
    }

    return 0;
}