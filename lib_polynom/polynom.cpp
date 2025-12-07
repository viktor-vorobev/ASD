#include "polynom.h"

using namespace std;

// Конструктор по умолчанию
Polynom::Polynom() {}

// Конструктор из монома
Polynom::Polynom(const Monom& monom) {
    if (monom.getCoefficient() != 0) {
        monoms.push_back(monom);
    }
}

// Конструктор из строки
Polynom::Polynom(const string& str) {
    parseString(str);
    simplify();
}

// Конструктор копирования
Polynom::Polynom(const Polynom& other) : monoms(other.monoms) {
    simplify();
}

// Конструктор перемещения
Polynom::Polynom(Polynom&& other) noexcept : monoms(move(other.monoms)) {}

// Оператор присваивания копированием
Polynom& Polynom::operator=(const Polynom& other) {
    if (this != &other) {
        monoms = other.monoms;
        simplify();
    }
    return *this;
}

// Оператор присваивания перемещением
Polynom& Polynom::operator=(Polynom&& other) noexcept {
    if (this != &other) {
        monoms = move(other.monoms);
    }
    return *this;
}

// Проверка на пустой полином
bool Polynom::isEmpty() const {
    return monoms.empty();
}

// Сортировка мономов
void Polynom::sortMonoms() {
    // Преобразуем список в вектор для сортировки
    vector<Monom> vec;
    for (auto it = monoms.begin(); it != monoms.end(); ++it) {
        vec.push_back(*it);
    }

    // Сортируем в порядке убывания
    sort(vec.begin(), vec.end());

    // Очищаем список и добавляем отсортированные мономы
    monoms.clear();
    for (const auto& monom : vec) {
        monoms.push_back(monom);
    }
}

// Приведение подобных членов
void Polynom::combineLikeTerms() {
    if (monoms.empty()) return;

    // Сначала сортируем
    sortMonoms();

    // Объединяем подобные члены
    List<Monom> result;

    auto it = monoms.begin();
    Monom current = *it;
    ++it;

    while (it != monoms.end()) {
        if (current.isSimilar(*it)) {
            try {
                current += *it;
            }
            catch (const exception& e) {
                // Пропускаем, если нельзя сложить
            }
        }
        else {
            if (current.getCoefficient() != 0) {
                result.push_back(current);
            }
            current = *it;
        }
        ++it;
    }

    // Добавляем последний моном
    if (current.getCoefficient() != 0) {
        result.push_back(current);
    }

    monoms = move(result);
}

// Упрощение полинома
void Polynom::simplify() {
    removeZeroMonoms();
    sortMonoms();
    combineLikeTerms();
}

// Преобразование в строку
string Polynom::toString() const {
    if (monoms.empty()) {
        return "0";
    }

    stringstream ss;
    bool first = true;

    for (auto it = monoms.begin(); it != monoms.end(); ++it) {
        const Monom& monom = *it;
        double coef = monom.getCoefficient();

        if (coef == 0) continue;

        if (!first) {
            if (coef > 0) {
                ss << " + ";
            }
            else {
                ss << " - ";
            }
        }
        else {
            if (coef < 0) {
                ss << "-";
            }
            first = false;
        }

        // Выводим моном без знака
        Monom temp = monom;
        if (coef < 0) {
            temp = temp * (-1); // Делаем коэффициент положительным для вывода
        }

        string monomStr = temp.toString();

        // Если моном - просто число и это не первый член
        if (monomStr == "1" && !monom.getVariables().empty()) {
            monomStr = monomStr.substr(1); // Убираем 1
        }

        ss << monomStr;
        first = false;
    }

    string result = ss.str();
    if (result.empty()) {
        return "0";
    }

    return result;
}

// Вычисление значения полинома в точке
double Polynom::evaluate(const map<char, double>& values) const {
    double result = 0;

    for (auto it = monoms.begin(); it != monoms.end(); ++it) {
        result += it->evaluate(values);
    }

    return result;
}

// Вычисление значения полинома в точке (для трех переменных)
double Polynom::evaluate(double x, double y, double z) const {
    map<char, double> values;
    values['x'] = x;
    values['y'] = y;
    values['z'] = z;

    return evaluate(values);
}

// Операторы сравнения
bool Polynom::operator==(const Polynom& other) const {
    Polynom thisSimplified = *this;
    Polynom otherSimplified = other;
    thisSimplified.simplify();
    otherSimplified.simplify();

    if (thisSimplified.monoms.size() != otherSimplified.monoms.size()) {
        return false;
    }

    auto it1 = thisSimplified.monoms.cbegin();
    auto it2 = otherSimplified.monoms.cbegin();

    while (it1 != thisSimplified.monoms.cend() && it2 != otherSimplified.monoms.cend()) {
        if (*it1 != *it2) {
            return false;
        }
        ++it1;
        ++it2;
    }

    return true;
}

bool Polynom::operator!=(const Polynom& other) const {
    return !(*this == other);
}

// Арифметические операторы
Polynom Polynom::operator+(const Polynom& other) const {
    Polynom result(*this);

    for (auto it = other.monoms.begin(); it != other.monoms.end(); ++it) {
        result.addMonom(*it);
    }

    result.simplify();
    return result;
}

Polynom Polynom::operator-(const Polynom& other) const {
    Polynom result(*this);

    for (auto it = other.monoms.begin(); it != other.monoms.end(); ++it) {
        result.addMonom(-(*it));
    }

    result.simplify();
    return result;
}

Polynom Polynom::operator*(const Polynom& other) const {
    Polynom result;

    for (auto it1 = monoms.begin(); it1 != monoms.end(); ++it1) {
        for (auto it2 = other.monoms.begin(); it2 != other.monoms.end(); ++it2) {
            try {
                Monom product = (*it1) * (*it2);
                result.addMonom(product);
            }
            catch (const exception& e) {
                // Пропускаем, если умножение невозможно
            }
        }
    }

    result.simplify();
    return result;
}

Polynom Polynom::operator/(const Polynom& other) const {
    // Простое деление полиномов (не полное деление с остатком)
    // Работает только если other - моном
    if (other.monoms.size() != 1) {
        throw runtime_error("Division is only implemented for monomial divisors");
    }

    Polynom result;
    Monom divisor = *other.monoms.begin();

    for (auto it = monoms.begin(); it != monoms.end(); ++it) {
        try {
            Monom quotient = (*it) / divisor;
            result.addMonom(quotient);
        }
        catch (const exception& e) {
            throw runtime_error("Cannot divide polynom: " + string(e.what()));
        }
    }

    result.simplify();
    return result;
}

Polynom Polynom::operator/(double num) const {
    if (num == 0) {
        throw runtime_error("Division by zero");
    }

    Polynom result;

    for (auto it = monoms.begin(); it != monoms.end(); ++it) {
        Monom divided = (*it) / num;
        result.addMonom(divided);
    }

    result.simplify();
    return result;
}

// Унарный минус
Polynom Polynom::operator-() const {
    Polynom result;

    for (auto it = monoms.begin(); it != monoms.end(); ++it) {
        result.addMonom(-(*it));
    }

    return result;
}

// Операторы присваивания
Polynom& Polynom::operator+=(const Polynom& other) {
    *this = *this + other;
    return *this;
}

Polynom& Polynom::operator-=(const Polynom& other) {
    *this = *this - other;
    return *this;
}

Polynom& Polynom::operator*=(const Polynom& other) {
    *this = *this * other;
    return *this;
}

Polynom& Polynom::operator/=(const Polynom& other) {
    *this = *this / other;
    return *this;
}

Polynom& Polynom::operator/=(double num) {
    *this = *this / num;
    return *this;
}

// Умножение на число
Polynom Polynom::operator*(double num) const {
    Polynom result;

    for (auto it = monoms.begin(); it != monoms.end(); ++it) {
        result.addMonom((*it) * num);
    }

    result.simplify();
    return result;
}

Polynom& Polynom::operator*=(double num) {
    *this = *this * num;
    return *this;
}

// Оператор вывода
ostream& operator<<(ostream& os, const Polynom& poly) {
    os << poly.toString();
    return os;
}

// Оператор ввода
istream& operator>>(istream& is, Polynom& poly) {
    string str;
    getline(is, str);
    poly = Polynom(str);
    return is;
}

// Производная полинома
Polynom Polynom::derivative(char var) const {
    Polynom result;

    for (auto it = monoms.begin(); it != monoms.end(); ++it) {
        const Monom& monom = *it;
        int degree = monom.getDegree(var);

        if (degree > 0) {
            // Создаем новый моном с уменьшенной степенью и умноженный на старую степень
            Monom derivativeMonom = monom;
            derivativeMonom.setCoefficient(monom.getCoefficient() * degree);

            // Уменьшаем степень переменной var
            map<char, int> vars = derivativeMonom.getVariables();
            vars[var] = degree - 1;
            if (vars[var] == 0) {
                vars.erase(var);
            }

            // Создаем новый моном
            Monom newMonom(derivativeMonom.getCoefficient(), vars);
            result.addMonom(newMonom);
        }
    }

    result.simplify();
    return result;
}

// Интеграл полинома
Polynom Polynom::integral(char var) const {
    Polynom result;

    for (auto it = monoms.begin(); it != monoms.end(); ++it) {
        const Monom& monom = *it;
        int degree = monom.getDegree(var);

        // Создаем новый моном с увеличенной степенью и деленный на новую степень
        Monom integralMonom = monom;

        // Увеличиваем степень переменной var
        map<char, int> vars = integralMonom.getVariables();
        vars[var] = degree + 1;

        // Делим коэффициент на новую степень
        double newCoef = monom.getCoefficient() / (degree + 1);

        // Создаем новый моном
        Monom newMonom(newCoef, vars);
        result.addMonom(newMonom);
    }

    result.simplify();
    return result;
}

// Вспомогательные методы
void Polynom::parseString(const string& str) {
    monoms.clear();

    if (str.empty()) {
        return;
    }

    string s = str;

    // Заменяем вычитание на добавление отрицательного
    for (size_t i = 1; i < s.length(); i++) {
        if (s[i] == '-') {
            s[i] = '+';
            s.insert(i, "-");
            i++;
        }
    }

    // Разбиваем на мономы по знаку '+'
    vector<string> monomStrings;
    string current;

    for (char c : s) {
        if (c == '+') {
            if (!current.empty()) {
                monomStrings.push_back(current);
                current.clear();
            }
        }
        else {
            current += c;
        }
    }

    if (!current.empty()) {
        monomStrings.push_back(current);
    }

    // Парсим каждый моном
    for (const string& monomStr : monomStrings) {
        if (!monomStr.empty()) {
            try {
                Monom monom(monomStr);
                if (monom.getCoefficient() != 0) {
                    addMonom(monom);
                }
            }
            catch (const exception& e) {
                // Пропускаем некорректные мономы
            }
        }
    }
}

void Polynom::addMonom(const Monom& monom) {
    if (monom.getCoefficient() == 0) {
        return;
    }

    // Ищем подобный моном
    bool found = false;

    for (auto it = monoms.begin(); it != monoms.end(); ++it) {
        if (it->isSimilar(monom)) {
            try {
                *it += monom;
                found = true;
                break;
            }
            catch (const exception& e) {
                // Не удалось сложить, добавляем как новый
            }
        }
    }

    if (!found) {
        monoms.push_back(monom);
    }
}

void Polynom::removeZeroMonoms() {
    List<Monom> result;

    for (auto it = monoms.begin(); it != monoms.end(); ++it) {
        if (it->getCoefficient() != 0) {
            result.push_back(*it);
        }
    }

    monoms = move(result);
}

// Внешние операторы
Polynom operator*(double num, const Polynom& poly) {
    return poly * num;
}

Polynom operator/(double num, const Polynom& poly) {
    return Polynom(Monom(num)) / poly;
}