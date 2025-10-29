#include "algorithm.h"
#include "steck.h"
#include <gtest/gtest.h>

TEST(BracketCheckerTest, CheckValidSequences) {
    EXPECT_TRUE(checkBracketSequence(""));
    EXPECT_TRUE(checkBracketSequence("()"));
    EXPECT_TRUE(checkBracketSequence("[]"));
    EXPECT_TRUE(checkBracketSequence("{}"));
    EXPECT_TRUE(checkBracketSequence("()[]{}"));
    EXPECT_TRUE(checkBracketSequence("([{}])"));
    EXPECT_TRUE(checkBracketSequence("({[]})"));
    EXPECT_TRUE(checkBracketSequence("((()))"));
    EXPECT_TRUE(checkBracketSequence("{{{}}}"));
    EXPECT_TRUE(checkBracketSequence("[[[]]]"));
}

TEST(BracketCheckerTest, CheckInvalidSequences) {
    EXPECT_FALSE(checkBracketSequence("("));
    EXPECT_FALSE(checkBracketSequence(")"));
    EXPECT_FALSE(checkBracketSequence("["));
    EXPECT_FALSE(checkBracketSequence("]"));
    EXPECT_FALSE(checkBracketSequence("([)]"));
    EXPECT_FALSE(checkBracketSequence("({[}])"));
    EXPECT_FALSE(checkBracketSequence("((())"));
    EXPECT_FALSE(checkBracketSequence("())"));
    EXPECT_FALSE(checkBracketSequence("{[}]"));
    EXPECT_FALSE(checkBracketSequence(")("));
    EXPECT_FALSE(checkBracketSequence("]["));
    EXPECT_FALSE(checkBracketSequence("}{"));
}

TEST(BracketCheckerTest, MixedBracketTypes) {
    EXPECT_TRUE(checkBracketSequence("()[]{}"));
    EXPECT_TRUE(checkBracketSequence("({[]})"));
    EXPECT_TRUE(checkBracketSequence("[({})]"));
    EXPECT_FALSE(checkBracketSequence("({[}])"));
    EXPECT_FALSE(checkBracketSequence("([{)]}"));
}

TEST(BracketCheckerTest, ComplexValidSequences) {
    EXPECT_TRUE(checkBracketSequence("({()[{}]})"));
    EXPECT_TRUE(checkBracketSequence("{[()({})]}"));
    EXPECT_TRUE(checkBracketSequence("((([[[{{{}}}]]])))"));
}

TEST(BracketCheckerTest, ComplexInvalidSequences) {
    EXPECT_FALSE(checkBracketSequence("({()[{}}])"));
    EXPECT_FALSE(checkBracketSequence("{[()({})}]"));
    EXPECT_FALSE(checkBracketSequence("((([[[{{{}}]]])))"));
}

