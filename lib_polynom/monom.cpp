#include "monom.h"

using namespace std;

// Конструктор по умолчанию
Monom::Monom() : coefficient(0) {}

// Конструктор с коэффициентом
Monom::Monom(double coef) : coefficient(coef) {}

// Конструктор с коэффициентом и переменными
Monom::Monom(double coef, const map<char, int>& vars) :
    coefficient(coef), variables(vars) {
    normalize();
}

// Конструктор из строки
Monom::Monom(const string& str) {
    parseString(str);
    normalize();
}

// Конструктор копирования
Monom::Monom(const Monom& other) :
    coefficient(other.coefficient), variables(other.variables) {
}

// Оператор присваивания
Monom& Monom::operator=(const Monom& other) {
    if (this != &other) {
        coefficient = other.coefficient;
        variables = other.variables;
    }
    return *this;
}

// Вычисление значения монома в точке
double Monom::evaluate(const map<char, double>& values) const {
    double result = coefficient;

    for (const auto& var : variables) {
        char variable = var.first;
        int degree = var.second;

        auto it = values.find(variable);
        if (it != values.end()) {
            result *= pow(it->second, degree);
        }
        else {
            // Если переменная не задана, считаем её равной 1
            if (degree != 0) {
                throw runtime_error("Variable " + string(1, variable) + " not provided");
            }
        }
    }

    return result;
}

// Вычисление значения монома в точке (для трех переменных)
double Monom::evaluate(double x, double y, double z) const {
    map<char, double> values;
    values['x'] = x;
    values['y'] = y;
    values['z'] = z;

    return evaluate(values);
}

// Проверка подобности
bool Monom::isSimilar(const Monom& other) const {
    return variables == other.variables;
}

// Получение степени по переменной
int Monom::getDegree(char var) const {
    auto it = variables.find(var);
    if (it != variables.end()) {
        return it->second;
    }
    return 0;
}

// Получение общей степени
int Monom::getTotalDegree() const {
    int total = 0;
    for (const auto& var : variables) {
        total += var.second;
    }
    return total;
}

// Преобразование в строку
string Monom::toString() const {
    if (coefficient == 0) {
        return "0";
    }

    stringstream ss;

    // Выводим коэффициент
    if (coefficient == -1 && !variables.empty()) {
        ss << "-";
    }
    else if (coefficient == 1 && !variables.empty()) {
        // Не выводим 1 перед переменными
    }
    else {
        ss << coefficient;
    }

    // Выводим переменные
    for (const auto& var : variables) {
        ss << var.first;
        if (var.second > 1) {
            ss << "^" << var.second;
        }
    }

    return ss.str();
}

// Операторы сравнения
bool Monom::operator==(const Monom& other) const {
    return coefficient == other.coefficient && variables == other.variables;
}

bool Monom::operator!=(const Monom& other) const {
    return !(*this == other);
}

// Операторы арифметики с мономами
Monom Monom::operator+(const Monom& other) const {
    if (!isSimilar(other)) {
        throw runtime_error("Cannot add non-similar monoms");
    }

    Monom result(*this);
    result.coefficient += other.coefficient;

    // Если коэффициент стал нулевым, очищаем переменные
    if (result.coefficient == 0) {
        result.variables.clear();
    }

    return result;
}

Monom Monom::operator-(const Monom& other) const {
    if (!isSimilar(other)) {
        throw runtime_error("Cannot subtract non-similar monoms");
    }

    Monom result(*this);
    result.coefficient -= other.coefficient;

    // Если коэффициент стал нулевым, очищаем переменные
    if (result.coefficient == 0) {
        result.variables.clear();
    }

    return result;
}

Monom Monom::operator*(const Monom& other) const {
    Monom result;
    result.coefficient = coefficient * other.coefficient;

    // Копируем переменные из текущего монома
    result.variables = variables;

    // Добавляем/суммируем степени переменных из другого монома
    for (const auto& var : other.variables) {
        result.variables[var.first] += var.second;
    }

    result.normalize();
    return result;
}

Monom Monom::operator/(const Monom& other) const {
    if (other.coefficient == 0) {
        throw runtime_error("Division by zero monom");
    }

    Monom result;
    result.coefficient = coefficient / other.coefficient;

    // Копируем переменные из текущего монома
    result.variables = variables;

    // Вычитаем степени переменных из другого монома
    for (const auto& var : other.variables) {
        int& degree = result.variables[var.first];
        degree -= var.second;

        if (degree < 0) {
            throw runtime_error("Negative degree in division result");
        }
    }

    result.normalize();
    return result;
}

// Операторы арифметики с числами
Monom Monom::operator*(double num) const {
    Monom result(*this);
    result.coefficient *= num;

    if (result.coefficient == 0) {
        result.variables.clear();
    }

    return result;
}

Monom Monom::operator/(double num) const {
    if (num == 0) {
        throw runtime_error("Division by zero");
    }

    Monom result(*this);
    result.coefficient /= num;

    return result;
}

// Унарный минус
Monom Monom::operator-() const {
    Monom result(*this);
    result.coefficient = -result.coefficient;
    return result;
}

// Операторы присваивания
Monom& Monom::operator+=(const Monom& other) {
    *this = *this + other;
    return *this;
}

Monom& Monom::operator-=(const Monom& other) {
    *this = *this - other;
    return *this;
}

Monom& Monom::operator*=(const Monom& other) {
    *this = *this * other;
    return *this;
}

Monom& Monom::operator/=(const Monom& other) {
    *this = *this / other;
    return *this;
}

Monom& Monom::operator*=(double num) {
    *this = *this * num;
    return *this;
}

Monom& Monom::operator/=(double num) {
    *this = *this / num;
    return *this;
}

// Оператор вывода
ostream& operator<<(ostream& os, const Monom& monom) {
    os << monom.toString();
    return os;
}

// Оператор ввода
istream& operator>>(istream& is, Monom& monom) {
    string str;
    is >> str;
    monom = Monom(str);
    return is;
}

// Сравнение для упорядочивания
bool Monom::operator<(const Monom& other) const {
    // Сначала сравниваем степени x
    int degX1 = getDegree('x');
    int degX2 = other.getDegree('x');
    if (degX1 != degX2) {
        return degX1 > degX2; // Большая степень x идет раньше
    }

    // Затем степени y
    int degY1 = getDegree('y');
    int degY2 = other.getDegree('y');
    if (degY1 != degY2) {
        return degY1 > degY2; // Большая степень y идет раньше
    }

    // Затем степени z
    int degZ1 = getDegree('z');
    int degZ2 = other.getDegree('z');
    if (degZ1 != degZ2) {
        return degZ1 > degZ2; // Большая степень z идет раньше
    }

    // Если все степени равны, сравниваем коэффициенты
    return coefficient > other.coefficient;
}

bool Monom::operator>(const Monom& other) const {
    return other < *this;
}

// Внешние операторы для умножения/деления на число
Monom operator*(double num, const Monom& monom) {
    return monom * num;
}

Monom operator/(double num, const Monom& monom) {
    Monom result(num);
    return result / monom;
}

// Вспомогательные методы
void Monom::parseString(const string& str) {
    coefficient = 1.0;
    variables.clear();

    if (str.empty()) {
        coefficient = 0;
        return;
    }

    string s = str;
    // Удаляем пробелы
    s.erase(remove(s.begin(), s.end(), ' '), s.end());

    if (s.empty()) {
        coefficient = 0;
        return;
    }

    // Обрабатываем знак
    size_t start = 0;
    if (s[0] == '+' || s[0] == '-') {
        if (s[0] == '-') {
            coefficient = -1;
        }
        start = 1;
    }

    // Парсим коэффициент
    size_t i = start;
    bool hasCoefficient = false;
    string coefStr = "";

    while (i < s.length() && (isdigit(s[i]) || s[i] == '.' || s[i] == ',')) {
        if (s[i] == ',') s[i] = '.'; // Заменяем запятую на точку
        coefStr += s[i];
        i++;
        hasCoefficient = true;
    }

    if (hasCoefficient && !coefStr.empty()) {
        coefficient *= stod(coefStr);
    }

    // Парсим переменные
    while (i < s.length()) {
        if (isalpha(s[i])) {
            char var = s[i];
            i++;

            // Проверяем степень
            if (i < s.length() && s[i] == '^') {
                i++;
                string degreeStr = "";
                while (i < s.length() && isdigit(s[i])) {
                    degreeStr += s[i];
                    i++;
                }

                if (!degreeStr.empty()) {
                    variables[var] = stoi(degreeStr);
                }
                else {
                    variables[var] = 1;
                }
            }
            else {
                variables[var] = 1;
            }
        }
        else {
            i++; // Пропускаем неизвестные символы
        }
    }
}

void Monom::normalize() {
    // Удаляем переменные с нулевой степенью
    removeZeroCoefficientVars();

    // Сортируем переменные
    vector<pair<char, int>> sortedVars(variables.begin(), variables.end());
    sort(sortedVars.begin(), sortedVars.end());

    variables.clear();
    for (const auto& var : sortedVars) {
        if (var.second > 0) {
            variables[var.first] = var.second;
        }
    }
}

void Monom::removeZeroCoefficientVars() {
    auto it = variables.begin();
    while (it != variables.end()) {
        if (it->second == 0) {
            it = variables.erase(it);
        }
        else {
            ++it;
        }
    }
}