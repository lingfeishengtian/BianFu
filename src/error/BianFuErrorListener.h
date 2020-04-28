//
// Created by Hunter Han on 2020-04-28.
//
#include "antlr4-runtime.h"

#ifndef BIANFU_BIANFUERRORLISTENER_H
#define BIANFU_BIANFUERRORLISTENER_H


class BianFuErrorListener : public antlr4::BaseErrorListener{
public:
    ~BianFuErrorListener() override;

    void
    syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line, size_t charPositionInLine,
                const std::string &msg, std::exception_ptr e) override;

    void reportAmbiguity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex, size_t stopIndex,
                         bool exact, const antlrcpp::BitSet &ambigAlts, antlr4::atn::ATNConfigSet *configs) override;

    void reportAttemptingFullContext(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex,
                                     size_t stopIndex, const antlrcpp::BitSet &conflictingAlts,
                                     antlr4::atn::ATNConfigSet *configs) override;

    void reportContextSensitivity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex,
                                  size_t stopIndex, size_t prediction, antlr4::atn::ATNConfigSet *configs) override;
};


#endif //BIANFU_BIANFUERRORLISTENER_H
