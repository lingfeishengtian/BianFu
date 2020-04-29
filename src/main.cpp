#include <iostream>

#include "antlr4-runtime.h"

#include "BFLexer.h"
#include "BFParser.h"

#include "error/BianFuErrorListener.h"
#include "error/BianFuError.h"
#include "scope/Scope.h"
#include "scope/BFPrimitive.h"
#include "visitors/ExecuteVisitor.h"

#ifdef _WIN32
    #include <io.h>
    #include <fcntl.h>
#endif

using namespace antlr4;

int main(int , const char **) {
//    std::map<std::string, Scope*> map;
//    BFPrimitive t = BFPrimitive(BFPrimitive::Primitive::INT, 9);
//    map["te"] = &t;
//    std::cout << map["te"]->getValueIfPrimitive();
    #ifdef _WIN32
        _setmode(_fileno(stdout), _O_U16TEXT);
        std::wcout << L"蝙蝠Windows版没有字体颜色。抱歉。" << std::endl;
    #endif

    ANTLRInputStream input("a = 1\nb = 99\na + b * b");
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
    try{
        executeVisitor.visitMain(dynamic_cast<BFParser::MainContext *>(tree));
    }catch (BianFuError &error){
        error.logError();
    }
    return 0;
}