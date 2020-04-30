#include <iostream>

#include "antlr4-runtime.h"

#include "BFLexer.h"
#include "BFParser.h"

#include "error/BianFuErrorListener.h"
#include "error/BianFuError.h"
#include "scope/Scope.h"
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

    ANTLRInputStream input("变量 啊 = 4\n变量 b = 2.1\n变量 c = (b/啊) * 9.6\n出(c)\nc =啊- b\n出(c)\n类 三{}");
    BFLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    BianFuLog logger = BianFuLog();
    BianFuErrorListener bianFuErrorListener = BianFuErrorListener();

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
    ExecuteVisitor executeVisitor = ExecuteVisitor(globalScope);
    try{
        executeVisitor.visitMain(dynamic_cast<BFParser::MainContext *>(tree));
    }catch (BianFuError &error){
        error.logError();
        logger.log("蝙蝠程序失败", BianFuLog::Situation::ERROR);
    }
    return 0;
}