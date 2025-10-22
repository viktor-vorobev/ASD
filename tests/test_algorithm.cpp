#include "algorithm.h"
#include "steck.h"
#include <gtest/gtest.h>
#include <vector>
#include <string>

// Тесты для вспомогательных функций
TEST(BracketCheckerTest, IsOpeningBracket) {
    EXPECT_TRUE(isOpeningBracket('('));
    EXPECT_TRUE(isOpeningBracket('['));
    EXPECT_TRUE(isOpeningBracket('{'));
    EXPECT_TRUE(isOpeningBracket('<'));
    EXPECT_FALSE(isOpeningBracket(')'));
    EXPECT_FALSE(isOpeningBracket(']'));
    EXPECT_FALSE(isOpeningBracket('}'));
    EXPECT_FALSE(isOpeningBracket('>'));
    EXPECT_FALSE(isOpeningBracket('a'));
    EXPECT_FALSE(isOpeningBracket('1'));
    EXPECT_FALSE(isOpeningBracket(' '));
}

TEST(BracketCheckerTest, IsClosingBracket) {
    EXPECT_TRUE(isClosingBracket(')'));
    EXPECT_TRUE(isClosingBracket(']'));
    EXPECT_TRUE(isClosingBracket('}'));
    EXPECT_TRUE(isClosingBracket('>'));
    EXPECT_FALSE(isClosingBracket('('));
    EXPECT_FALSE(isClosingBracket('['));
    EXPECT_FALSE(isClosingBracket('{'));
    EXPECT_FALSE(isClosingBracket('<'));
    EXPECT_FALSE(isClosingBracket('a'));
    EXPECT_FALSE(isClosingBracket('1'));
    EXPECT_FALSE(isClosingBracket(' '));
}

TEST(BracketCheckerTest, IsMatchingPair) {
    EXPECT_TRUE(isMatchingPair('(', ')'));
    EXPECT_TRUE(isMatchingPair('[', ']'));
    EXPECT_TRUE(isMatchingPair('{', '}'));
    EXPECT_TRUE(isMatchingPair('<', '>'));
    EXPECT_FALSE(isMatchingPair('(', ']'));
    EXPECT_FALSE(isMatchingPair('[', ')'));
    EXPECT_FALSE(isMatchingPair('{', ')'));
    EXPECT_FALSE(isMatchingPair('(', '}'));
    EXPECT_FALSE(isMatchingPair('a', ')'));
    EXPECT_FALSE(isMatchingPair('(', 'a'));
}

TEST(BracketCheckerTest, GetExpectedClosing) {
    EXPECT_EQ(getExpectedClosing('('), ')');
    EXPECT_EQ(getExpectedClosing('['), ']');
    EXPECT_EQ(getExpectedClosing('{'), '}');
    EXPECT_EQ(getExpectedClosing('<'), '>');
    EXPECT_EQ(getExpectedClosing('a'), '?');
    EXPECT_EQ(getExpectedClosing(')'), '?');
}

// Тесты для основной функции проверки
TEST(BracketCheckerTest, CheckValidSequences) {
    EXPECT_TRUE(checkBracketSequence(""));
    EXPECT_TRUE(checkBracketSequence("()"));
    EXPECT_TRUE(checkBracketSequence("[]"));
    EXPECT_TRUE(checkBracketSequence("{}"));
    EXPECT_TRUE(checkBracketSequence("<>"));
    EXPECT_TRUE(checkBracketSequence("()[]{}"));
    EXPECT_TRUE(checkBracketSequence("([{}])"));
    EXPECT_TRUE(checkBracketSequence("({[]})"));
    EXPECT_TRUE(checkBracketSequence("[({<>})]"));
    EXPECT_TRUE(checkBracketSequence("((()))"));
    EXPECT_TRUE(checkBracketSequence("{{{}}}"));
    EXPECT_TRUE(checkBracketSequence("[[[]]]"));
    EXPECT_TRUE(checkBracketSequence("<<<>>>"));
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
    EXPECT_FALSE(checkBracketSequence("><"));
}

TEST(BracketCheckerTest, CheckSequencesWithOtherCharacters) {
    EXPECT_TRUE(checkBracketSequence("abc"));
    EXPECT_TRUE(checkBracketSequence("a(b)c"));
    EXPECT_TRUE(checkBracketSequence("a[b{c}d]e"));
    EXPECT_TRUE(checkBracketSequence("hello (world [test{string}])"));
    EXPECT_TRUE(checkBracketSequence("int main() { return 0; }"));
    EXPECT_TRUE(checkBracketSequence("<html><body></body></html>"));

    EXPECT_FALSE(checkBracketSequence("a(b)c)"));
    EXPECT_FALSE(checkBracketSequence("a[b{c}d}e]"));
    EXPECT_FALSE(checkBracketSequence("hello (world [test{string])}"));
    EXPECT_FALSE(checkBracketSequence("int main( { return 0; })"));
    EXPECT_FALSE(checkBracketSequence("<html><body></html></body>"));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}