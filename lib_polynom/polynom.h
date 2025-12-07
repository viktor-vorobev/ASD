#ifndef POLYNOM_H
#define POLYNOM_H

#include "monom.h"
#include "list.h"
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <algorithm>

class Polynom {
private:
    List<Monom> monoms; // Список мономов

public:
    // Конструкторы
    Polynom();
    Polynom(const Monom& monom);
    Polynom(const std::string& str);
    Polynom(const Polynom& other);
    Polynom(Polynom&& other) noexcept;

    // Деструктор
    ~Polynom() = default;

    // Операторы присваивания
    Polynom& operator=(const Polynom& other);
    Polynom& operator=(Polynom&& other) noexcept;

    // Геттеры
    const List<Monom>& getMonoms() const { return monoms; }

    // Проверка на пустой полином
    bool isEmpty() const;

    // Сортировка мономов
    void sortMonoms();

    // Приведение подобных членов
    void combineLikeTerms();

    // Упрощение полинома
    void simplify();

    // Преобразование в строку
    std::string toString() const;

    // Вычисление значения полинома в точке
    double evaluate(const std::map<char, double>& values) const;
    double evaluate(double x, double y, double z) const;

    // Операторы сравнения
    bool operator==(const Polynom& other) const;
    bool operator!=(const Polynom& other) const;

    // Арифметические операторы
    Polynom operator+(const Polynom& other) const;
    Polynom operator-(const Polynom& other) const;
    Polynom operator*(const Polynom& other) const;
    Polynom operator/(const Polynom& other) const; // Деление полиномов (может быть не точным)
    Polynom operator/(double num) const;

    // Унарный минус
    Polynom operator-() const;

    // Операторы присваивания
    Polynom& operator+=(const Polynom& other);
    Polynom& operator-=(const Polynom& other);
    Polynom& operator*=(const Polynom& other);
    Polynom& operator/=(const Polynom& other);
    Polynom& operator/=(double num);

    // Умножение на число
    Polynom operator*(double num) const;
    Polynom& operator*=(double num);

    // Операторы ввода/вывода
    friend std::ostream& operator<<(std::ostream& os, const Polynom& poly);
    friend std::istream& operator>>(std::istream& is, Polynom& poly);

    // Производная полинома
    Polynom derivative(char var) const;

    // Интеграл полинома
    Polynom integral(char var) const;

private:
    // Вспомогательные методы
    void parseString(const std::string& str);
    void addMonom(const Monom& monom);
    void removeZeroMonoms();
};

// Внешние операторы
Polynom operator*(double num, const Polynom& poly);
Polynom operator/(double num, const Polynom& poly);

#endif // POLYNOM_H