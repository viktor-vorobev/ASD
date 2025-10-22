#include "algorithm.h"
#include "steck.h"
#include <unordered_map>

// Константы для типов скобок
namespace {
    const std::unordered_map<char, char> BRACKET_PAIRS = {
        {')', '('},
        {']', '['},
        {'}', '{'},
        {'>', '<'}
    };

    const std::unordered_map<char, bool> OPENING_BRACKETS = {
        {'(', true},
        {'[', true},
        {'{', true},
        {'<', true}
    };

    const std::unordered_map<char, bool> CLOSING_BRACKETS = {
        {')', true},
        {']', true},
        {'}', true},
        {'>', true}
    };
}

// Реализации вспомогательных функций
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

// Реализации основных функций
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

bool checkBracketSequenceDetailed(const std::string& sequence) {
    Stack<char> bracketStack;

    std::cout << "Checking sequence: \"" << sequence << "\"" << std::endl;

    for (size_t i = 0; i < sequence.length(); ++i) {
        char current = sequence[i];

        if (isOpeningBracket(current)) {
            bracketStack.push(current);
            std::cout << "  Position " << i << ": Pushed opening bracket '" << current << "'" << std::endl;
        }
        else if (isClosingBracket(current)) {
            if (bracketStack.empty()) {
                std::cout << "  ✗ Position " << i << ": Closing bracket '" << current
                    << "' without matching opening bracket" << std::endl;
                std::cout << "Result: INVALID" << std::endl << std::endl;
                return false;
            }

            char topBracket = bracketStack.top();
            if (isMatchingPair(topBracket, current)) {
                bracketStack.pop();
                std::cout << "  Position " << i << ": Matched '" << topBracket
                    << "' with '" << current << "' - popped from stack" << std::endl;
            }
            else {
                std::cout << "  ✗ Position " << i << ": Expected '"
                    << getExpectedClosing(topBracket) << "' but found '"
                    << current << "'" << std::endl;
                std::cout << "Result: INVALID" << std::endl << std::endl;
                return false;
            }
        }
    }

    if (!bracketStack.empty()) {
        std::cout << "  ✗ Unclosed bracket(s): ";
        while (!bracketStack.empty()) {
            std::cout << bracketStack.top() << " ";
            bracketStack.pop();
        }
        std::cout << std::endl;
        std::cout << "Result: INVALID" << std::endl << std::endl;
        return false;
    }

    std::cout << "  ✓ All brackets are properly balanced" << std::endl;
    std::cout << "Result: VALID" << std::endl << std::endl;
    return true;
}

std::vector<bool> checkMultipleSequences(const std::vector<std::string>& sequences) {
    std::vector<bool> results;
    results.reserve(sequences.size());

    for (const auto& seq : sequences) {
        results.push_back(checkBracketSequence(seq));
    }

    return results;
}

void checkMultipleSequencesDetailed(const std::vector<std::string>& sequences) {
    std::cout << "=== CHECKING MULTIPLE SEQUENCES ===" << std::endl;

    for (size_t i = 0; i < sequences.size(); ++i) {
        std::cout << "Sequence " << (i + 1) << ":" << std::endl;
        checkBracketSequenceDetailed(sequences[i]);
    }
}

int getMaxBracketDepth(const std::string& sequence) {
    Stack<char> tempStack;
    int maxDepth = 0;

    for (char c : sequence) {
        if (isOpeningBracket(c)) {
            tempStack.push(c);
            if (tempStack.size() > maxDepth) {
                maxDepth = tempStack.size();
            }
        }
        else if (isClosingBracket(c)) {
            if (!tempStack.empty()) {
                tempStack.pop();
            }
        }
    }

    return maxDepth;
}

bool checkParenthesesOnly(const std::string& sequence) {
    Stack<char> parenStack;

    for (char c : sequence) {
        if (c == '(') {
            parenStack.push(c);
        }
        else if (c == ')') {
            if (parenStack.empty()) {
                return false;
            }
            parenStack.pop();
        }
    }

    return parenStack.empty();
}

std::vector<size_t> findUnclosedBrackets(const std::string& sequence) {
    Stack<std::pair<char, size_t>> bracketStack;
    std::vector<size_t> unclosedPositions;

    for (size_t i = 0; i < sequence.length(); ++i) {
        char current = sequence[i];

        if (isOpeningBracket(current)) {
            bracketStack.push({ current, i });
        }
        else if (isClosingBracket(current)) {
            if (!bracketStack.empty()) {
                char topBracket = bracketStack.top().first;
                if (isMatchingPair(topBracket, current)) {
                    bracketStack.pop();
                }
            }
        }
    }

    while (!bracketStack.empty()) {
        unclosedPositions.push_back(bracketStack.top().second);
        bracketStack.pop();
    }

    std::reverse(unclosedPositions.begin(), unclosedPositions.end());
    return unclosedPositions;
}

