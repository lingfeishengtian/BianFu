/* Copyright (c) 2012-2017 The ANTLR Project. All rights reserved.
 * Use of this file is governed by the BSD 3-clause license that
 * can be found in the LICENSE.txt file in the project root.
 */

//
//  main.cpp
//  antlr4-cpp-demo
//
//  Created by Mike Lischke on 13.03.16.
//

#include <iostream>

#include "antlr4-runtime.h"

#include "BFLexer.h"
#include "BFParser.h"

#include "error/BianFuErrorListener.h"
#include "scope/Scope.h"
#include "visitors/ExecuteVisitor.h"

using namespace antlr4;

int main(int , const char **) {
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


    return 0;
}