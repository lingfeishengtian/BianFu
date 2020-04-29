#include <iostream>

#include "antlr4-runtime.h"

#include "BFLexer.h"
#include "BFParser.h"

#include "error/BianFuErrorListener.h"
#include "error/BianFuError.h"
#include "scope/Scope.h"
#include "scope/primitives/BFIntPrimitive.h"
#include "visitors/ExecuteVisitor.h"

#ifdef _WIN32
    #include <io.h>
    #include <fcntl.h>
#endif

using namespace antlr4;

int main(int , const char **) {
    #ifdef _WIN32
        _setmode(_fileno(stdout), _O_U16TEXT);
        std::wcout << L"蝙蝠Windows版没有字体颜色。抱歉。" << std::endl;
    #endif

    ANTLRInputStream input("a = 4\nb = 2.1\n(a / b) * 9.6\n类 三{}");
    BFLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    BianFuLog logger = BianFuLog();
    BianFuErrorListener bianFuErrorListener = BianFuErrorListener();

    tokens.fill();
    for (auto token : tokens.getTokens()) {
        std::cout << token->toString() << " " << token->getType() << std::endl;
    }

    BFParser parser(&tokens);
    parser.removeErrorListeners();
    parser.addErrorListener(&bianFuErrorListener);

    tree::ParseTree *tree = parser.main();

    logger.log(tree->toStringTree(&parser), BianFuLog::Situation::WARNING);

    Scope globalScope = Scope();
    ExecuteVisitor executeVisitor = ExecuteVisitor(globalScope);
    try{
        executeVisitor.visitMain(dynamic_cast<BFParser::MainContext *>(tree));
    }catch (BianFuError &error){
        error.logError();
    }
    return 0;
}