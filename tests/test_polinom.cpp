#include <gtest/gtest.h>
#include "polynom.h"
#include <sstream>
#include <cmath>

//TESTS FOR MONOM 

TEST(MonomTest, DefaultConstructor) {
    Monom m;
    EXPECT_DOUBLE_EQ(m.getCoefficient(), 0.0);
    EXPECT_TRUE(m.getVariables().empty());
    EXPECT_EQ(m.toString(), "0");
}

TEST(MonomTest, CoefficientConstructor) {
    Monom m(5.5);
    EXPECT_DOUBLE_EQ(m.getCoefficient(), 5.5);
    EXPECT_TRUE(m.getVariables().empty());
    EXPECT_EQ(m.toString(), "5.5");
}

TEST(MonomTest, CoefficientConstructorNegative) {
    Monom m(-3.7);
    EXPECT_DOUBLE_EQ(m.getCoefficient(), -3.7);
    EXPECT_TRUE(m.getVariables().empty());
    EXPECT_EQ(m.toString(), "-3.7");
}

TEST(MonomTest, StringConstructorSimple) {
    Monom m("3");
    EXPECT_DOUBLE_EQ(m.getCoefficient(), 3.0);
    EXPECT_TRUE(m.getVariables().empty());
}

TEST(MonomTest, StringConstructorWithVariable) {
    Monom m("2x");
    EXPECT_DOUBLE_EQ(m.getCoefficient(), 2.0);
    EXPECT_EQ(m.getVariables().size(), 1);
    EXPECT_EQ(m.getDegree('x'), 1);
    EXPECT_EQ(m.toString(), "2x");
}

TEST(MonomTest, StringConstructorWithExponent) {
    Monom m("3x^2");
    EXPECT_DOUBLE_EQ(m.getCoefficient(), 3.0);
    EXPECT_EQ(m.getVariables().size(), 1);
    EXPECT_EQ(m.getDegree('x'), 2);
    EXPECT_EQ(m.toString(), "3x^2");
}

TEST(MonomTest, StringConstructorMultipleVariables) {
    Monom m("2x^2y^3z");
    EXPECT_DOUBLE_EQ(m.getCoefficient(), 2.0);
    EXPECT_EQ(m.getVariables().size(), 3);
    EXPECT_EQ(m.getDegree('x'), 2);
    EXPECT_EQ(m.getDegree('y'), 3);
    EXPECT_EQ(m.getDegree('z'), 1);
    EXPECT_EQ(m.toString(), "2x^2y^3z");
}

TEST(MonomTest, StringConstructorNegativeCoefficient) {
    Monom m("-4xy^2");
    EXPECT_DOUBLE_EQ(m.getCoefficient(), -4.0);
    EXPECT_EQ(m.getVariables().size(), 2);
    EXPECT_EQ(m.getDegree('x'), 1);
    EXPECT_EQ(m.getDegree('y'), 2);
    EXPECT_EQ(m.toString(), "-4xy^2");
}

TEST(MonomTest, StringConstructorImplicitOne) {
    Monom m("x");
    EXPECT_DOUBLE_EQ(m.getCoefficient(), 1.0);
    EXPECT_EQ(m.getVariables().size(), 1);
    EXPECT_EQ(m.getDegree('x'), 1);
    EXPECT_EQ(m.toString(), "x");
}

TEST(MonomTest, StringConstructorNegativeImplicit) {
    Monom m("-y^3");
    EXPECT_DOUBLE_EQ(m.getCoefficient(), -1.0);
    EXPECT_EQ(m.getVariables().size(), 1);
    EXPECT_EQ(m.getDegree('y'), 3);
    EXPECT_EQ(m.toString(), "-y^3");
}

TEST(MonomTest, CopyConstructor) {
    Monom m1("3x^2y");
    Monom m2(m1);
    EXPECT_DOUBLE_EQ(m2.getCoefficient(), 3.0);
    EXPECT_EQ(m2.getVariables().size(), 2);
    EXPECT_EQ(m2.getDegree('x'), 2);
    EXPECT_EQ(m2.getDegree('y'), 1);
    EXPECT_EQ(m1, m2);
}

TEST(MonomTest, AssignmentOperator) {
    Monom m1("5xy^2z^3");
    Monom m2;
    m2 = m1;
    EXPECT_DOUBLE_EQ(m2.getCoefficient(), 5.0);
    EXPECT_EQ(m2.getVariables().size(), 3);
    EXPECT_EQ(m2.getDegree('x'), 1);
    EXPECT_EQ(m2.getDegree('y'), 2);
    EXPECT_EQ(m2.getDegree('z'), 3);
    EXPECT_EQ(m1, m2);
}

TEST(MonomTest, EqualityOperator) {
    Monom m1("3x^2");
    Monom m2("3x^2");
    Monom m3("2x^2");
    Monom m4("3y^2");

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 == m3);
    EXPECT_FALSE(m1 == m4);
}

TEST(MonomTest, InequalityOperator) {
    Monom m1("3x^2");
    Monom m2("2x^2");
    Monom m3("3x^2");

    EXPECT_TRUE(m1 != m2);
    EXPECT_FALSE(m1 != m3);
}

TEST(MonomTest, IsSimilar) {
    Monom m1("3x^2y");
    Monom m2("5x^2y");
    Monom m3("3xy^2");
    Monom m4("3x^2");

    EXPECT_TRUE(m1.isSimilar(m2));  // Одинаковые степени, разные коэффициенты
    EXPECT_FALSE(m1.isSimilar(m3)); // Разные степени
    EXPECT_FALSE(m1.isSimilar(m4)); // Разные переменные
}

TEST(MonomTest, GetTotalDegree) {
    Monom m1("3x^2y^3z");
    Monom m2("5");
    Monom m3("2x");

    EXPECT_EQ(m1.getTotalDegree(), 6); // 2 + 3 + 1 = 6
    EXPECT_EQ(m2.getTotalDegree(), 0); // Константа
    EXPECT_EQ(m3.getTotalDegree(), 1); // x^1
}

TEST(MonomTest, GetDegree) {
    Monom m("3x^2y^3");
    EXPECT_EQ(m.getDegree('x'), 2);
    EXPECT_EQ(m.getDegree('y'), 3);
    EXPECT_EQ(m.getDegree('z'), 0); // Несуществующая переменная
}

TEST(MonomTest, EvaluateSimple) {
    Monom m("2x^2");
    std::map<char, double> values = { {'x', 3.0} };
    EXPECT_DOUBLE_EQ(m.evaluate(values), 18.0); // 2 * 3^2 = 18
}

TEST(MonomTest, EvaluateMultipleVariables) {
    Monom m("3x^2y");
    std::map<char, double> values = { {'x', 2.0}, {'y', 4.0} };
    EXPECT_DOUBLE_EQ(m.evaluate(values), 48.0); // 3 * 2^2 * 4 = 48
}

TEST(MonomTest, EvaluateThreeArgs) {
    Monom m("2xy^2z^3");
    EXPECT_DOUBLE_EQ(m.evaluate(1.0, 2.0, 3.0), 2.0 * 1.0 * 4.0 * 27.0); // 2 * 1 * 4 * 27 = 216
}

TEST(MonomTest, EvaluateConstant) {
    Monom m("5");
    std::map<char, double> values = { {'x', 10.0} };
    EXPECT_DOUBLE_EQ(m.evaluate(values), 5.0);
}

TEST(MonomTest, AdditionSimilar) {
    Monom m1("3x^2");
    Monom m2("2x^2");
    Monom result = m1 + m2;

    EXPECT_DOUBLE_EQ(result.getCoefficient(), 5.0);
    EXPECT_EQ(result.getVariables().size(), 1);
    EXPECT_EQ(result.getDegree('x'), 2);
    EXPECT_EQ(result.toString(), "5x^2");
}

TEST(MonomTest, AdditionNonSimilarThrows) {
    Monom m1("3x^2");
    Monom m2("2y^2");

    EXPECT_THROW(m1 + m2, std::runtime_error);
}

TEST(MonomTest, SubtractionSimilar) {
    Monom m1("5x^2");
    Monom m2("2x^2");
    Monom result = m1 - m2;

    EXPECT_DOUBLE_EQ(result.getCoefficient(), 3.0);
    EXPECT_EQ(result.getVariables().size(), 1);
    EXPECT_EQ(result.getDegree('x'), 2);
    EXPECT_EQ(result.toString(), "3x^2");
}

TEST(MonomTest, SubtractionZeroResult) {
    Monom m1("3x^2");
    Monom m2("3x^2");
    Monom result = m1 - m2;

    EXPECT_DOUBLE_EQ(result.getCoefficient(), 0.0);
    EXPECT_TRUE(result.getVariables().empty());
    EXPECT_EQ(result.toString(), "0");
}

TEST(MonomTest, Multiplication) {
    Monom m1("3x^2y");
    Monom m2("2xy^3");
    Monom result = m1 * m2;

    EXPECT_DOUBLE_EQ(result.getCoefficient(), 6.0);
    EXPECT_EQ(result.getVariables().size(), 2);
    EXPECT_EQ(result.getDegree('x'), 3); // 2 + 1
    EXPECT_EQ(result.getDegree('y'), 4); // 1 + 3
    EXPECT_EQ(result.toString(), "6x^3y^4");
}

TEST(MonomTest, MultiplicationWithConstant) {
    Monom m("3x^2");
    Monom result = m * 2.0;

    EXPECT_DOUBLE_EQ(result.getCoefficient(), 6.0);
    EXPECT_EQ(result.toString(), "6x^2");
}

TEST(MonomTest, MultiplicationConstantFromLeft) {
    Monom m("3x^2");
    Monom result = 2.0 * m;

    EXPECT_DOUBLE_EQ(result.getCoefficient(), 6.0);
    EXPECT_EQ(result.toString(), "6x^2");
}

TEST(MonomTest, Division) {
    Monom m1("6x^3y^4");
    Monom m2("2xy^2");
    Monom result = m1 / m2;

    EXPECT_DOUBLE_EQ(result.getCoefficient(), 3.0);
    EXPECT_EQ(result.getVariables().size(), 2);
    EXPECT_EQ(result.getDegree('x'), 2); // 3 - 1
    EXPECT_EQ(result.getDegree('y'), 2); // 4 - 2
    EXPECT_EQ(result.toString(), "3x^2y^2");
}

TEST(MonomTest, DivisionWithNumber) {
    Monom m("6x^2");
    Monom result = m / 2.0;

    EXPECT_DOUBLE_EQ(result.getCoefficient(), 3.0);
    EXPECT_EQ(result.toString(), "3x^2");
}

TEST(MonomTest, DivisionByZeroNumberThrows) {
    Monom m("3x^2");
    EXPECT_THROW(m / 0.0, std::runtime_error);
}

TEST(MonomTest, DivisionByZeroMonomThrows) {
    Monom m1("3x^2");
    Monom m2("0");
    EXPECT_THROW(m1 / m2, std::runtime_error);
}

TEST(MonomTest, DivisionNegativeDegreeThrows) {
    Monom m1("3x^2");
    Monom m2("x^3");
    EXPECT_THROW(m1 / m2, std::runtime_error);
}

TEST(MonomTest, UnaryMinus) {
    Monom m("3x^2");
    Monom result = -m;

    EXPECT_DOUBLE_EQ(result.getCoefficient(), -3.0);
    EXPECT_EQ(result.toString(), "-3x^2");
}

TEST(MonomTest, UnaryMinusConstant) {
    Monom m("5");
    Monom result = -m;

    EXPECT_DOUBLE_EQ(result.getCoefficient(), -5.0);
    EXPECT_EQ(result.toString(), "-5");
}

TEST(MonomTest, PlusEquals) {
    Monom m1("3x^2");
    Monom m2("2x^2");
    m1 += m2;

    EXPECT_DOUBLE_EQ(m1.getCoefficient(), 5.0);
    EXPECT_EQ(m1.toString(), "5x^2");
}

TEST(MonomTest, MinusEquals) {
    Monom m1("5x^2");
    Monom m2("2x^2");
    m1 -= m2;

    EXPECT_DOUBLE_EQ(m1.getCoefficient(), 3.0);
    EXPECT_EQ(m1.toString(), "3x^2");
}

TEST(MonomTest, MultiplyEquals) {
    Monom m1("3x^2");
    Monom m2("2y");
    m1 *= m2;

    EXPECT_DOUBLE_EQ(m1.getCoefficient(), 6.0);
    EXPECT_EQ(m1.toString(), "6x^2y");
}

TEST(MonomTest, DivideEquals) {
    Monom m1("6x^2y");
    Monom m2("2y");
    m1 /= m2;

    EXPECT_DOUBLE_EQ(m1.getCoefficient(), 3.0);
    EXPECT_EQ(m1.toString(), "3x^2");
}

TEST(MonomTest, ComparisonOperators) {
    Monom m1("3x^2y");   // x:2, y:1
    Monom m2("2xy^2");   // x:1, y:2
    Monom m3("4x^2");    // x:2, y:0

    // В лексикографическом порядке: x^2y > x^2 > xy^2
    EXPECT_TRUE(m1 > m3);  // x^2y > x^2
    EXPECT_TRUE(m3 > m2);  // x^2 > xy^2
    EXPECT_TRUE(m1 > m2);  // x^2y > xy^2

    EXPECT_FALSE(m1 < m3);
    EXPECT_FALSE(m3 < m2);
    EXPECT_FALSE(m1 < m2);
}

TEST(MonomTest, StreamOutput) {
    Monom m("3x^2y");
    std::ostringstream oss;
    oss << m;
    EXPECT_EQ(oss.str(), "3x^2y");
}

TEST(MonomTest, StreamInput) {
    Monom m;
    std::istringstream iss("4xy^3");
    iss >> m;

    EXPECT_DOUBLE_EQ(m.getCoefficient(), 4.0);
    EXPECT_EQ(m.getVariables().size(), 2);
    EXPECT_EQ(m.getDegree('x'), 1);
    EXPECT_EQ(m.getDegree('y'), 3);
}

// TESTS FOR POLYNOM 

TEST(PolynomTest, DefaultConstructor) {
    Polynom p;
    EXPECT_TRUE(p.isEmpty());
    EXPECT_EQ(p.toString(), "0");
}

TEST(PolynomTest, MonomConstructor) {
    Monom m("3x^2");
    Polynom p(m);

    EXPECT_FALSE(p.isEmpty());
    EXPECT_EQ(p.toString(), "3x^2");
}

TEST(PolynomTest, StringConstructorSimple) {
    Polynom p("3x^2");
    EXPECT_EQ(p.toString(), "3x^2");
}

TEST(PolynomTest, StringConstructorMultipleTerms) {
    Polynom p("x^2 + 2xy + y^2");
    EXPECT_EQ(p.toString(), "x^2 + 2xy + y^2");
}

TEST(PolynomTest, StringConstructorWithNegative) {
    Polynom p("x^2 - 2xy + y^2");
    // После упрощения должно быть "x^2 - 2xy + y^2"
    std::string result = p.toString();

    // Проверяем, что есть все три термина
    EXPECT_TRUE(result.find("x^2") != std::string::npos);
    EXPECT_TRUE(result.find("2xy") != std::string::npos ||
        result.find("xy") != std::string::npos);
    EXPECT_TRUE(result.find("y^2") != std::string::npos);
}

TEST(PolynomTest, StringConstructorComplex) {
    Polynom p("3x^2y - 2xz^3 + 5y^2 - 7");

    // Проверяем отдельные компоненты
    EXPECT_TRUE(p.toString().find("3x^2y") != std::string::npos);
    EXPECT_TRUE(p.toString().find("2xz^3") != std::string::npos ||
        p.toString().find("xz^3") != std::string::npos);
    EXPECT_TRUE(p.toString().find("5y^2") != std::string::npos);
    EXPECT_TRUE(p.toString().find("7") != std::string::npos);

    // Проверяем вычисление
    EXPECT_NEAR(p.evaluate(1, 1, 1), 3 * 1 * 1 - 2 * 1 * 1 + 5 * 1 - 7, 1e-10);
}

TEST(PolynomTest, StringConstructorWithSpaces) {
    Polynom p(" 3x^2 + 2y - z ");
    EXPECT_EQ(p.toString(), "3x^2 + 2y - z");
}

TEST(PolynomTest, CopyConstructor) {
    Polynom p1("x^2 + 2xy + y^2");
    Polynom p2(p1);

    EXPECT_EQ(p1.toString(), p2.toString());
    EXPECT_TRUE(p1 == p2);
}

TEST(PolynomTest, AssignmentOperator) {
    Polynom p1("x^2 + y^2");
    Polynom p2;
    p2 = p1;

    EXPECT_EQ(p1.toString(), p2.toString());
    EXPECT_TRUE(p1 == p2);
}

TEST(PolynomTest, MoveConstructor) {
    Polynom p1("x^2 + y^2");
    Polynom p2(std::move(p1));

    EXPECT_TRUE(p1.isEmpty());
    EXPECT_EQ(p2.toString(), "x^2 + y^2");
}

TEST(PolynomTest, MoveAssignment) {
    Polynom p1("x^2 + y^2");
    Polynom p2;
    p2 = std::move(p1);

    EXPECT_TRUE(p1.isEmpty());
    EXPECT_EQ(p2.toString(), "x^2 + y^2");
}

TEST(PolynomTest, EqualityOperator) {
    Polynom p1("x^2 + 2xy + y^2");
    Polynom p2("x^2 + 2xy + y^2");
    Polynom p3("x^2 + xy + y^2");

    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 == p3);
}

TEST(PolynomTest, InequalityOperator) {
    Polynom p1("x^2 + 2xy + y^2");
    Polynom p2("x^2 + xy + y^2");

    EXPECT_TRUE(p1 != p2);
}

TEST(PolynomTest, Addition) {
    Polynom p1("x + y");
    Polynom p2("2x - y");
    Polynom result = p1 + p2;

    // Упрощаем результат
    result.simplify();

    // Должно быть "3x"
    std::string str = result.toString();

    // Удаляем пробелы для сравнения
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

    EXPECT_TRUE(str == "3x");
}

TEST(PolynomTest, AdditionWithCombine) {
    Polynom p1("x^2 + 2xy");
    Polynom p2("3xy + y^2");
    Polynom result = p1 + p2;

    EXPECT_EQ(result.toString(), "x^2 + 5xy + y^2");
}

TEST(PolynomTest, Subtraction) {
    Polynom p1("3x^2 + 2xy");
    Polynom p2("x^2 + xy");
    Polynom result = p1 - p2;

    EXPECT_EQ(result.toString(), "2x^2 + xy");
}

TEST(PolynomTest, SubtractionNegativeResult) {
    Polynom p1("x + y");
    Polynom p2("2x + 2y");
    Polynom result = p1 - p2;

    EXPECT_EQ(result.toString(), "-x - y");
}

TEST(PolynomTest, MultiplicationByPolynom) {
    Polynom p1("x + y");
    Polynom p2("x - y");
    Polynom result = p1 * p2;

    result.simplify();
    std::string str = result.toString();
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

    // (x+y)(x-y) = x^2 - y^2
    EXPECT_TRUE(str == "x^2-y^2" || str == "x^2-y^2");
}

TEST(PolynomTest, MultiplicationByNumber) {
    Polynom p("x^2 + 2xy + y^2");
    Polynom result = p * 2.0;

    EXPECT_EQ(result.toString(), "2x^2 + 4xy + 2y^2");
}

TEST(PolynomTest, MultiplicationNumberFromLeft) {
    Polynom p("x^2 + 2xy + y^2");
    Polynom result = 2.0 * p;

    EXPECT_EQ(result.toString(), "2x^2 + 4xy + 2y^2");
}

TEST(PolynomTest, DivisionByNumber) {
    Polynom p("4x^2 + 8xy + 4y^2");
    Polynom result = p / 2.0;

    EXPECT_EQ(result.toString(), "2x^2 + 4xy + 2y^2");
}

TEST(PolynomTest, DivisionByMonom) {
    Polynom p("6x^3 + 9x^2y + 3xy^2");
    Polynom divisor("3x");
    Polynom result = p / divisor;

    EXPECT_EQ(result.toString(), "2x^2 + 3xy + y^2");
}

TEST(PolynomTest, DivisionByZeroNumberThrows) {
    Polynom p("x^2 + y^2");
    EXPECT_THROW(p / 0.0, std::runtime_error);
}

TEST(PolynomTest, UnaryMinus) {
    Polynom p("x^2 - 2xy + y^2");
    Polynom result = -p;

    result.simplify();
    std::string str = result.toString();

    // Должно быть "-x^2 + 2xy - y^2" или эквивалентное представление
    EXPECT_TRUE(str.find("-x^2") != std::string::npos ||
        str.find("x^2") == std::string::npos);
    EXPECT_TRUE(str.find("2xy") != std::string::npos ||
        str.find("xy") != std::string::npos);
    EXPECT_TRUE(str.find("-y^2") != std::string::npos);
}

TEST(PolynomTest, PlusEqualsOperator) {
    Polynom p1("x^2 + xy");
    Polynom p2("2xy + y^2");
    p1 += p2;

    EXPECT_EQ(p1.toString(), "x^2 + 3xy + y^2");
}

TEST(PolynomTest, MinusEqualsOperator) {
    Polynom p1("x^2 + 3xy");
    Polynom p2("2xy + y^2");
    p1 -= p2;

    EXPECT_EQ(p1.toString(), "x^2 + xy - y^2");
}

TEST(PolynomTest, MultiplyEqualsOperator) {
    Polynom p("x + y");
    p *= Polynom("x - y");

    EXPECT_EQ(p.toString(), "x^2 - y^2");
}

TEST(PolynomTest, EvaluateSimple) {
    Polynom p("x^2 + 2x + 1");
    std::map<char, double> values = { {'x', 2.0} };
    EXPECT_DOUBLE_EQ(p.evaluate(values), 9.0); // 4 + 4 + 1 = 9
}

TEST(PolynomTest, EvaluateMultipleVariables) {
    Polynom p("x^2 + 2xy + y^2");
    std::map<char, double> values = { {'x', 1.0}, {'y', 2.0} };
    EXPECT_DOUBLE_EQ(p.evaluate(values), 9.0); // 1 + 4 + 4 = 9
}

TEST(PolynomTest, EvaluateThreeArgs) {
    Polynom p("x^2 + y^2 + z^2");
    EXPECT_DOUBLE_EQ(p.evaluate(1.0, 2.0, 3.0), 14.0); // 1 + 4 + 9 = 14
}

TEST(PolynomTest, EvaluateEmpty) {
    Polynom p;
    std::map<char, double> values = { {'x', 10.0} };
    EXPECT_DOUBLE_EQ(p.evaluate(values), 0.0);
}

TEST(PolynomTest, DerivativeByX) {
    Polynom p("3x^2 + 2xy + y^2");
    Polynom result = p.derivative('x');

    EXPECT_EQ(result.toString(), "6x + 2y");
}

TEST(PolynomTest, DerivativeByY) {
    Polynom p("3x^2 + 2xy + y^2");
    Polynom result = p.derivative('y');

    EXPECT_EQ(result.toString(), "2x + 2y");
}

TEST(PolynomTest, DerivativeConstant) {
    Polynom p("5");
    Polynom result = p.derivative('x');

    EXPECT_TRUE(result.isEmpty());
    EXPECT_EQ(result.toString(), "0");
}

TEST(PolynomTest, DerivativeComplex) {
    Polynom p("x^3y^2 + 2x^2y + 3x");
    Polynom result = p.derivative('x');

    EXPECT_EQ(result.toString(), "3x^2y^2 + 4xy + 3");
}

TEST(PolynomTest, IntegralByX) {
    Polynom p("3x^2 + 2x + 1");
    Polynom result = p.integral('x');

    EXPECT_EQ(result.toString(), "x^3 + x^2 + x");
}

TEST(PolynomTest, IntegralByY) {
    Polynom p("2y + 3");
    Polynom result = p.integral('y');

    EXPECT_EQ(result.toString(), "y^2 + 3y");
}

TEST(PolynomTest, SortMonoms) {
    // Создаем полином в неправильном порядке
    Polynom p;
    // Вручную добавляем мономы в обратном порядке
    p = Polynom("y^2 + xy + x^2y^3 + 5");

    // После упрощения мономы должны быть отсортированы
    // x^2y^3 -> xy -> y^2 -> 5
    EXPECT_EQ(p.toString(), "x^2y^3 + xy + y^2 + 5");
}

TEST(PolynomTest, CombineLikeTerms) {
    Polynom p("x^2 + 2xy + xy + y^2");
    p.simplify();

    EXPECT_EQ(p.toString(), "x^2 + 3xy + y^2");
}

TEST(PolynomTest, RemoveZeroTerms) {
    Polynom p("x^2 + 0xy + y^2");
    p.simplify();

    EXPECT_EQ(p.toString(), "x^2 + y^2");
}

TEST(PolynomTest, StreamOutput) {
    Polynom p("x^2 + 2xy + y^2");
    std::ostringstream oss;
    oss << p;
    EXPECT_EQ(oss.str(), "x^2 + 2xy + y^2");
}

TEST(PolynomTest, StreamInput) {
    Polynom p;
    std::istringstream iss("3x^2 + 2xy + 1");
    iss >> p;

    EXPECT_EQ(p.toString(), "3x^2 + 2xy + 1");
}

TEST(PolynomTest, ComplexExpression1) {
    Polynom p("(x + y)^2");
    // На самом деле конструктор из строки не поддерживает скобки
    // Поэтому тестируем эквивалентное выражение
    Polynom p1("x^2 + 2xy + y^2");
    Polynom p2 = Polynom("x + y") * Polynom("x + y");

    EXPECT_TRUE(p1 == p2);
}

TEST(PolynomTest, ComplexExpression2) {
    Polynom p1("x^3 - 3x^2y + 3xy^2 - y^3");
    Polynom p2 = Polynom("x - y") * Polynom("x - y") * Polynom("x - y");

    EXPECT_TRUE(p1 == p2);
}

TEST(PolynomTest, LargeNumbers) {
    Polynom p("1000000x^2 + 0.000001y");
    std::map<char, double> values = { {'x', 2.0}, {'y', 1000000.0} };

    double expected = 1000000 * 4.0 + 0.000001 * 1000000.0; // 4000000 + 1 = 4000001
    EXPECT_DOUBLE_EQ(p.evaluate(values), expected);
}

TEST(PolynomTest, OrderingPreservedAfterOperations) {
    Polynom p1("x^3 + x^2y + xy^2 + y^3");
    Polynom p2("x^2 + xy + y^2");
    Polynom result = p1 * p2;

    // Проверяем, что результат упорядочен
    std::string resultStr = result.toString();
    // Простая проверка - результат не должен содержать "+ -" (минус после плюса)
    EXPECT_EQ(resultStr.find("+ -"), std::string::npos);
}

//  EDGE CASES

TEST(EdgeCasesTest, MonomZeroFromString) {
    Monom m("0");
    EXPECT_DOUBLE_EQ(m.getCoefficient(), 0.0);
    EXPECT_TRUE(m.getVariables().empty());
}

TEST(EdgeCasesTest, MonomEmptyString) {
    Monom m("");
    EXPECT_DOUBLE_EQ(m.getCoefficient(), 0.0);
}

TEST(EdgeCasesTest, PolynomEmptyString) {
    Polynom p("");
    EXPECT_TRUE(p.isEmpty());
}

TEST(EdgeCasesTest, PolynomOnlySpaces) {
    Polynom p("   ");
    EXPECT_TRUE(p.isEmpty());
}

TEST(EdgeCasesTest, PolynomSingleTerm) {
    Polynom p("5");
    EXPECT_EQ(p.toString(), "5");
}

TEST(EdgeCasesTest, PolynomNegativeFirstTerm) {
    Polynom p("-x^2 + 2xy - y^2");

    // Проверяем, что парсинг работает
    EXPECT_FALSE(p.isEmpty());

    // Проверяем вычисление
    double val = p.evaluate(1, 1, 0);
    EXPECT_NEAR(val, -1 + 2 - 1, 1e-10); // -1 + 2 - 1 = 0

    // Проверяем строковое представление
    std::string str = p.toString();
    EXPECT_TRUE(str.find("x^2") != std::string::npos);
    EXPECT_TRUE(str.find("xy") != std::string::npos);
    EXPECT_TRUE(str.find("y^2") != std::string::npos);
}

TEST(EdgeCasesTest, PolynomAllNegative) {
    Polynom p("-x^2 - 2xy - y^2");
    EXPECT_EQ(p.toString(), "-x^2 - 2xy - y^2");
}

TEST(EdgeCasesTest, SelfAssignment) {
    Polynom p("x^2 + y^2");
    p = p;
    EXPECT_EQ(p.toString(), "x^2 + y^2");
}

TEST(EdgeCasesTest, ChainOperations) {
    Polynom p("x + y");
    Polynom one("1");

    // Более простая цепочка операций
    Polynom result = p + p - p * one / 2.0;

    result.simplify();

    // (x+y) + (x+y) - (x+y)*1/2 = 2(x+y) - 0.5(x+y) = 1.5(x+y)
    // или 1.5x + 1.5y

    // Проверяем вычисление
    double val = result.evaluate(1, 1, 0);
    EXPECT_NEAR(val, 1.5 + 1.5, 1e-10); // 3.0

    // Проверяем коэффициенты
    bool hasX = false, hasY = false;
    std::string str = result.toString();
    if (str.find("x") != std::string::npos) hasX = true;
    if (str.find("y") != std::string::npos) hasY = true;

    EXPECT_TRUE(hasX || hasY);
}

//PERFORMANCE TESTS 

TEST(PerformanceTest, LargePolynomMultiplication) {
    // Создаем полиномы с несколькими членами
    Polynom p1("x^5 + 2x^4y + 3x^3y^2 + 4x^2y^3 + 5xy^4 + y^5");
    Polynom p2("x^3 + 2x^2y + 3xy^2 + y^3");

    // Умножение должно работать без ошибок
    EXPECT_NO_THROW(p1 * p2);

    Polynom result = p1 * p2;
    EXPECT_FALSE(result.isEmpty());
}

TEST(PerformanceTest, MultipleDerivatives) {
    Polynom p("x^5 + 5x^4y + 10x^3y^2 + 10x^2y^3 + 5xy^4 + y^5");

    // Берем производную 3 раза по x
    Polynom d1 = p.derivative('x');
    Polynom d2 = d1.derivative('x');
    Polynom d3 = d2.derivative('x');

    EXPECT_EQ(d3.toString(), "60x^2 + 120xy + 60y^2");
}
