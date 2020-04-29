//
// Created by Hunter Han on 4/28/2020.
//
#include <locale>
#include <codecvt>
#include <string>

#include "BianFuLog.h"
#ifdef _WIN32
#include "windows.h"
#endif

void BianFuLog::internalLog(const std::string &s, std::string* colorCode) {
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 12);
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring wide = converter.from_bytes(s);
        std::wcout << wide << std::endl;
    #else
        if(colorCode != nullptr)
            std::cout << "\033[" << *colorCode << "m" << s << "\033[0m" << std::endl;
        else
            std::cout << s << std::endl;
    #endif
}

void BianFuLog::log(const std::string &s) {
    internalLog(s, nullptr);
}

void BianFuLog::log(const std::string &s, BianFuLog::TextColor textColor) {
    std::string colorStr = std::to_string(textColor);
    internalLog(s, &colorStr);
}

void BianFuLog::log(const std::string &s, BianFuLog::Situation situation) {
    std::string colorStr = std::to_string(1) + ";" + std::to_string(situation);
    internalLog(s, &colorStr);
}
