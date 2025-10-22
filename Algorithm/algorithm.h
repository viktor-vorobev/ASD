#ifndef ALGORITMHS_H
#define ALGORITHMS_H

#include "steck.h"
#include <string>
#include <vector>
#include <iostream>


// Структура для детального результата проверки
struct CheckResult {
    bool isValid;
    std::string message;
    size_t errorPosition;
    char expectedChar;
    char foundChar;
};

// Основные функции проверки скобок
bool checkBracketSequence(const std::string& sequence);
bool checkBracketSequenceDetailed(const std::string& sequence);
std::vector<bool> checkMultipleSequences(const std::vector<std::string>& sequences);
void checkMultipleSequencesDetailed(const std::vector<std::string>& sequences);

// Утилитарные функции
int getMaxBracketDepth(const std::string& sequence);
bool checkParenthesesOnly(const std::string& sequence);
std::vector<size_t> findUnclosedBrackets(const std::string& sequence);

// Вспомогательные функции (могут быть полезны для внешнего использования)
bool isOpeningBracket(char c);
bool isClosingBracket(char c);
bool isMatchingPair(char open, char close);
char getExpectedClosing(char open);

#endif // ALGORITHM_H