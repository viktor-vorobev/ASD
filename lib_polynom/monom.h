#ifndef MONOM_H
#define MONOM_H

#include <iostream>
#include <string>
#include <cmath>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>

class Monom {
private:
    double coefficient; // коэффициент
    std::map<char, int> variables; // переменные и их степени

public:
    // Конструкторы
    Monom();
    Monom(double coef);
    Monom(double coef, const std::map<char, int>& vars);
    Monom(const std::string& str);

    // Конструктор копирования
    Monom(const Monom& other);

    // Деструктор
    ~Monom() = default;

    // Оператор присваивания
    Monom& operator=(const Monom& other);

    // Геттеры
    double getCoefficient() const { return coefficient; }
    const std::map<char, int>& getVariables() const { return variables; }

    // Сеттеры
    void setCoefficient(double coef) { coefficient = coef; }

    // Вычисление значения монома в точке
    double evaluate(const std::map<char, double>& values) const;
    double evaluate(double x, double y, double z) const;

    // Проверка подобности (одинаковые степени переменных)
    bool isSimilar(const Monom& other) const;

    // Получение степени монома по переменной
    int getDegree(char var) const;

    // Получение общей степени
    int getTotalDegree() const;

    // Преобразование в строку
    std::string toString() const;

    // Операторы сравнения
    bool operator==(const Monom& other) const;
    bool operator!=(const Monom& other) const;

    // Операторы арифметики с мономами
    Monom operator+(const Monom& other) const;
    Monom operator-(const Monom& other) const;
    Monom operator*(const Monom& other) const;
    Monom operator/(const Monom& other) const;

    // Операторы арифметики с числами
    Monom operator*(double num) const;
    Monom operator/(double num) const;

    // Унарный минус
    Monom operator-() const;

    // Операторы присваивания
    Monom& operator+=(const Monom& other);
    Monom& operator-=(const Monom& other);
    Monom& operator*=(const Monom& other);
    Monom& operator/=(const Monom& other);
    Monom& operator*=(double num);
    Monom& operator/=(double num);

    // Операторы ввода/вывода
    friend std::ostream& operator<<(std::ostream& os, const Monom& monom);
    friend std::istream& operator>>(std::istream& is, Monom& monom);

    // Сравнение для упорядочивания (лексикографическое)
    bool operator<(const Monom& other) const;
    bool operator>(const Monom& other) const;

private:
    // Вспомогательные методы
    void parseString(const std::string& str);
    void normalize();
    void removeZeroCoefficientVars();
};

// Внешние операторы для умножения/деления на число
Monom operator*(double num, const Monom& monom);
Monom operator/(double num, const Monom& monom);

#endif // MONOM_H