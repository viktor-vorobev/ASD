#include "algorithm.h"
#include "steck.h"
#include <iostream>
#include <unordered_map>

namespace {
    const std::unordered_map<char, char> BRACKET_PAIRS = {
        {')', '('},
        {']', '['},
        {'}', '{'}
    };

    const std::unordered_map<char, bool> OPENING_BRACKETS = {
        {'(', true},
        {'[', true},
        {'{', true}
    };

    const std::unordered_map<char, bool> CLOSING_BRACKETS = {
        {')', true},
        {']', true},
        {'}', true}
    };

    bool isOpeningBracket(char c) {
        return OPENING_BRACKETS.find(c) != OPENING_BRACKETS.end();
    }

    bool isClosingBracket(char c) {
        return CLOSING_BRACKETS.find(c) != CLOSING_BRACKETS.end();
    }

    bool isMatchingPair(char open, char close) {
        auto it = BRACKET_PAIRS.find(close);
        return it != BRACKET_PAIRS.end() && it->second == open;
    }

    char getExpectedClosing(char open) {
        for (const auto& pair : BRACKET_PAIRS) {
            if (pair.second == open) {
                return pair.first;
            }
        }
        return '?';
    }
}

bool checkBracketSequence(const std::string& sequence) {
    Stack<char> bracketStack;

    for (size_t i = 0; i < sequence.length(); ++i) {
        char current = sequence[i];

        if (isOpeningBracket(current)) {
            bracketStack.push(current);
        }
        else if (isClosingBracket(current)) {
            if (bracketStack.empty()) {
                return false;
            }

            char topBracket = bracketStack.top();
            if (isMatchingPair(topBracket, current)) {
                bracketStack.pop();
            }
            else {
                return false;
            }
        }
    }

    return bracketStack.empty();
}