//
// Created by Hunter Han on 2020-04-28.
//

#include "BianFuErrorListener.h"

using namespace antlr4;

BianFuErrorListener::~BianFuErrorListener() {

}

void BianFuErrorListener::syntaxError(Recognizer *recognizer, Token *offendingSymbol, size_t line,
                                      size_t charPositionInLine, const std::string &msg, std::exception_ptr e) {
    std::cout << "\033[1;31m" << line << ":" << charPositionInLine << " 出问题了！" << "\033[0m" << std::endl;

    if(offendingSymbol->getType() == Token::EOF){
        std::cout << "\033[1;37m" << "你的蝙蝠文件有无内容！" << "\033[0m" << std::endl;
    }else if(msg.find("extraneous input") != std::string::npos) {
        std::cout << "\033[1;37m" << "程序多了个: " << offendingSymbol->getText() << "\033[0m" << std::endl;
    }else{
        std::cout << "\033[1;31m" << "蝙蝠没找到问题：" << msg << "\033[0m" << std::endl;
    }
    std::cout << msg;
}

void BianFuErrorListener::reportAmbiguity(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex,
                                          size_t stopIndex, bool exact, const antlrcpp::BitSet &ambigAlts,
                                          atn::ATNConfigSet *configs) {

}

void BianFuErrorListener::reportAttemptingFullContext(Parser *recognizer, const dfa::DFA &dfa,
                                                      size_t startIndex, size_t stopIndex,
                                                      const antlrcpp::BitSet &conflictingAlts,
                                                      atn::ATNConfigSet *configs) {

}

void BianFuErrorListener::reportContextSensitivity(Parser *recognizer, const dfa::DFA &dfa,
                                                   size_t startIndex, size_t stopIndex, size_t prediction,
                                                   atn::ATNConfigSet *configs) {

}
