#include <iostream>

#include "antlr4-runtime.h"

#include "BFLexer.h"
#include "BFParser.h"

#include "error/BianFuErrorListener.h"
#include "scope/Scope.h"
#include "scope/BFPrimitive.h"
#include "visitors/ExecuteVisitor.h"

#ifdef _WIN32
    #include <io.h>
    #include <fcntl.h>
#endif

using namespace antlr4;

int main(int , const char **) {
    #ifdef _WIN32
        _setmode(_fileno(stdout), _O_U16TEXT);
    #endif

    ANTLRInputStream input("a = 3");
    BFLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    BianFuErrorListener bianFuErrorListener = BianFuErrorListener();

    tokens.fill();
//    for (auto token : tokens.getTokens()) {
//        std::cout << token->toString() << std::endl;
//    }

    BFParser parser(&tokens);
    parser.removeErrorListeners();
    parser.addErrorListener(&bianFuErrorListener);

    tree::ParseTree *tree = parser.main();

    std::cout << tree->toStringTree(&parser) << std::endl;

    Scope globalScope = Scope();
    ExecuteVisitor executeVisitor = ExecuteVisitor(globalScope);

    #ifdef _WIN32
        std::wcout << L"蝙蝠系统退出代码：" << static_cast<BFPrimitive>(executeVisitor.visitMain(dynamic_cast<BFParser::MainContext *>(tree))).value << std::endl;
    #else
        std::cout << "蝙蝠系统退出代码：" << static_cast<BFPrimitive>(executeVisitor.visitMain(dynamic_cast<BFParser::MainContext *>(tree))).value << std::endl;
    #endif

    return 0;
}