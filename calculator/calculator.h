// main.cpp - с предустановленными полиномами
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <iomanip>
#include <memory>

#include "monom.h"
#include "polynom.h"

class PolynomialCalculator {
private:
    std::map<std::string, Polynom> variables;
    int polyCounter = 1;  // Счетчик для автоматических имен

    // Инициализация предустановленных полиномов
    void initializeDefaultPolynomials() {
        // Три предустановленных полинома
        variables["quadratic"] = Polynom("x^2 + 2x + 1");
        variables["linear"] = Polynom("2x - 3");
        variables["two_vars"] = Polynom("x^2 - y^2");

        std::cout << "\nПредустановленные полиномы:\n";
        std::cout << "1. quadratic = " << variables["quadratic"] << " (квадратичный полином)\n";
        std::cout << "2. linear = " << variables["linear"] << " (линейный полином)\n";
        std::cout << "3. two_vars = " << variables["two_vars"] << " (полином от двух переменных)\n";
        std::cout << "Можете использовать эти имена в операциях!\n";

        // Устанавливаем счетчик, чтобы не перезаписать
        polyCounter = 4;  // Начинаем с p4
    }

    // Генерация автоматического имени
    std::string generateAutoName() {
        std::string name = "p" + std::to_string(polyCounter);
        polyCounter++;
        return name;
    }

    // Поиск полинома по имени (с поддержкой автозаполнения)
    Polynom* findPolynomial(const std::string& name) {
        auto it = variables.find(name);
        if (it != variables.end()) {
            return &(it->second);
        }
        return nullptr;
    }

    void printMenu() {
        std::cout << "\n=========================================\n";
        std::cout << "       КАЛЬКУЛЯТОР ПОЛИНОМОВ\n";
        std::cout << "=========================================\n";
        std::cout << "1. Ввести полином (автосохранение как pN)\n";
        std::cout << "2. Сложить полиномы\n";
        std::cout << "3. Вычесть полиномы\n";
        std::cout << "4. Умножить полиномы\n";
        std::cout << "5. Умножить полином на число\n";
        std::cout << "6. Разделить полином на число\n";
        std::cout << "7. Вычислить значение полинома\n";
        std::cout << "8. Производная полинома\n";
        std::cout << "9. Интеграл полинома\n";
        std::cout << "10. Показать все переменные\n";
        std::cout << "11. Сохранить в переменную (ручное имя)\n";
        std::cout << "12. Загрузить из переменной\n";
        std::cout << "13. Примеры использования\n";
        std::cout << "14. Удалить переменную\n";
        std::cout << "15. Показать предустановленные полиномы\n";
        std::cout << "16. Демонстрация операций с предустановленными\n";
        std::cout << "0. Выход\n";
        std::cout << "=========================================\n";
        std::cout << "Выберите операцию: ";
    }

    // Ввод полинома с автоматическим сохранением
    std::pair<std::string, Polynom> inputPolynomialWithAutoSave(const std::string& prompt = "Введите полином: ", bool autoSave = true) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);

        // Проверяем, не является ли ввод именем существующей переменной
        Polynom* existing = findPolynomial(input);
        if (existing != nullptr) {
            std::cout << "Используется существующая переменная: " << input << std::endl;
            return { input, *existing };
        }

        try {
            Polynom p(input);
            std::string varName;

            if (autoSave && !p.isEmpty()) {
                varName = generateAutoName();
                variables[varName] = p;
                std::cout << "Полином автоматически сохранен как " << varName << std::endl;
            }

            return { varName, p };
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка при создании полинома: " << e.what() << std::endl;
            return { "", Polynom() };
        }
    }

    // Ввод полинома по имени или значению
    std::pair<std::string, Polynom> inputPolynomialByName(const std::string& prompt = "Введите полином или имя переменной: ") {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);

        // Проверяем, не является ли ввод именем существующей переменной
        Polynom* existing = findPolynomial(input);
        if (existing != nullptr) {
            return { input, *existing };
        }

        try {
            Polynom p(input);
            return { "", p };
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
            return { "", Polynom() };
        }
    }

    double inputNumber(const std::string& prompt) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);

        try {
            return std::stod(input);
        }
        catch (...) {
            std::cout << "Ошибка: неверное число\n";
            return 0.0;
        }
    }

    void showExamples() {
        std::cout << "\nПримеры ввода полиномов:\n";
        std::cout << "=============================\n";
        std::cout << "1. Простой полином: 3x^2 + 2x + 1\n";
        std::cout << "2. С отрицательными коэффициентами: x^2 - 2xy + y^2\n";
        std::cout << "3. Несколько переменных: 2x^2y + 3xy^2 - 4z^3\n";
        std::cout << "4. Константа: 5\n";
        std::cout << "5. Моном: -2x^3\n";
        std::cout << "6. Дробные коэффициенты: 0.5x^2 + 1.5y - 2.7\n";
        std::cout << "\nПримеры имен переменных:\n";
        std::cout << "- Предустановленные: quadratic, linear, two_vars\n";
        std::cout << "- Автоматические: p4, p5, p6...\n";
        std::cout << "- Свои имена: myPoly, test, result и т.д.\n";
        std::cout << "Можно использовать как значение, так и имя переменной!\n";
    }

    void showAllVariables() {
        std::cout << "\n--- ВСЕ ПЕРЕМЕННЫЕ ---\n";
        if (variables.empty()) {
            std::cout << "Нет сохраненных переменных.\n";
        }
        else {
            std::cout << "Всего переменных: " << variables.size() << "\n";
            std::cout << "\nПредустановленные:\n";
            for (const auto& name : { "quadratic", "linear", "two_vars" }) {
                if (variables.find(name) != variables.end()) {
                    std::cout << "  " << name << " = " << variables[name] << std::endl;
                }
            }

            std::cout << "\nПользовательские:\n";
            for (const auto& var : variables) {
                if (var.first != "quadratic" && var.first != "linear" && var.first != "two_vars") {
                    std::cout << "  " << var.first << " = " << var.second << std::endl;
                }
            }
        }
    }

    void showDefaultPolynomials() {
        std::cout << "\n--- ПРЕДУСТАНОВЛЕННЫЕ ПОЛИНОМЫ ---\n";
        std::cout << "1. quadratic = " << variables["quadratic"] << "\n";
        std::cout << "   Тип: квадратичный полином от x\n";
        std::cout << "   Использование: вычисление квадратных функций\n\n";

        std::cout << "2. linear = " << variables["linear"] << "\n";
        std::cout << "   Тип: линейный полином от x\n";
        std::cout << "   Использование: вычисление линейных функций\n\n";

        std::cout << "3. two_vars = " << variables["two_vars"] << "\n";
        std::cout << "   Тип: полином от двух переменных\n";
        std::cout << "   Использование: операции с несколькими переменными\n\n";

        std::cout << "Примеры операций с предустановленными полиномами:\n";
        std::cout << "- quadratic + linear\n";
        std::cout << "- quadratic * 2\n";
        std::cout << "- two_vars(x=3, y=2)\n";
        std::cout << "- d/dx(quadratic)\n";
    }

    void demonstrateOperations() {
        std::cout << "\n=== ДЕМОНСТРАЦИЯ ОПЕРАЦИЙ С ПРЕДУСТАНОВЛЕННЫМИ ПОЛИНОМАМИ ===\n\n";

        // 1. Сложение
        std::cout << "1. Сложение:\n";
        Polynom sum = variables["quadratic"] + variables["linear"];
        std::cout << "   quadratic + linear = " << sum << std::endl;

        // 2. Вычитание
        std::cout << "\n2. Вычитание:\n";
        Polynom diff = variables["quadratic"] - variables["linear"];
        std::cout << "   quadratic - linear = " << diff << std::endl;

        // 3. Умножение
        std::cout << "\n3. Умножение на число:\n";
        Polynom mult = variables["linear"] * 3;
        std::cout << "   linear * 3 = " << mult << std::endl;

        // 4. Вычисление значения
        std::cout << "\n4. Вычисление значений:\n";
        std::cout << "   quadratic(2) = " << variables["quadratic"].evaluate(2, 0, 0) << std::endl;
        std::cout << "   two_vars(3, 2) = " << variables["two_vars"].evaluate(3, 2, 0) << std::endl;

        // 5. Производная
        std::cout << "\n5. Производные:\n";
        Polynom deriv1 = variables["quadratic"].derivative('x');
        Polynom deriv2 = variables["linear"].derivative('x');
        std::cout << "   d/dx(quadratic) = " << deriv1 << std::endl;
        std::cout << "   d/dx(linear) = " << deriv2 << std::endl;

        // 6. Интеграл
        std::cout << "\n6. Интегралы:\n";
        Polynom integral1 = variables["linear"].integral('x');
        std::cout << "   ∫(linear)dx = " << integral1 << std::endl;

        // Автоматически сохраняем результаты демонстрации
        variables["demo_sum"] = sum;
        variables["demo_diff"] = diff;
        variables["demo_mult"] = mult;
        variables["demo_deriv1"] = deriv1;
        variables["demo_integral1"] = integral1;

        std::cout << "\nРезультаты демонстрации сохранены:\n";
        std::cout << "- demo_sum = " << sum << std::endl;
        std::cout << "- demo_diff = " << diff << std::endl;
        std::cout << "- demo_mult = " << mult << std::endl;
        std::cout << "- demo_deriv1 = " << deriv1 << std::endl;
        std::cout << "- demo_integral1 = " << integral1 << std::endl;
    }

public:
    void run() {
        std::cout << "ДОБРО ПОЖАЛОВАТЬ В КАЛЬКУЛЯТОР ПОЛИНОМОВ!\n";
        std::cout << "==========================================\n";

        // Инициализируем предустановленные полиномы
        initializeDefaultPolynomials();

        std::cout << "\nКаждый введенный полином автоматически сохраняется как p4, p5, p6...\n";
        std::cout << "Для выхода введите 'exit' в любое время.\n";

        while (true) {
            printMenu();

            std::string choiceStr;
            std::getline(std::cin, choiceStr);

            if (choiceStr == "exit" || choiceStr == "0") {
                std::cout << "Выход из программы.\n";
                break;
            }

            int choice;
            try {
                choice = std::stoi(choiceStr);
            }
            catch (...) {
                std::cout << "Неверный выбор. Попробуйте снова.\n";
                continue;
            }

            try {
                switch (choice) {
                case 1: {
                    std::cout << "\n--- ВВОД ПОЛИНОМА С АВТОСОХРАНЕНИЕМ ---\n";
                    auto result = inputPolynomialWithAutoSave();
                    if (!result.first.empty() && !result.second.isEmpty()) {
                        std::cout << result.first << " = " << result.second << std::endl;
                    }
                    break;
                }
                case 2: {
                    std::cout << "\n--- СЛОЖЕНИЕ ПОЛИНОМОВ ---\n";
                    std::cout << "Можно использовать предустановленные: quadratic, linear, two_vars\n";
                    auto result1 = inputPolynomialByName("Введите первый полином или имя переменной: ");
                    if (result1.second.isEmpty()) continue;

                    auto result2 = inputPolynomialByName("Введите второй полином или имя переменной: ");
                    if (result2.second.isEmpty()) continue;

                    Polynom sum = result1.second + result2.second;

                    // Форматируем вывод с именами переменных если они есть
                    std::string left = result1.first.empty() ? result1.second.toString() : result1.first;
                    std::string right = result2.first.empty() ? result2.second.toString() : result2.first;

                    std::cout << "\n" << left << " + " << right << " = " << sum << std::endl;

                    // Автоматически сохраняем результат
                    if (!sum.isEmpty()) {
                        std::string resultName = generateAutoName();
                        variables[resultName] = sum;
                        std::cout << "Результат сохранен как " << resultName << std::endl;
                    }
                    break;
                }
                case 3: {
                    std::cout << "\n--- ВЫЧИТАНИЕ ПОЛИНОМОВ ---\n";
                    std::cout << "Можно использовать предустановленные: quadratic, linear, two_vars\n";
                    auto result1 = inputPolynomialByName("Введите первый полином или имя переменной: ");
                    if (result1.second.isEmpty()) continue;

                    auto result2 = inputPolynomialByName("Введите второй полином или имя переменной: ");
                    if (result2.second.isEmpty()) continue;

                    Polynom diff = result1.second - result2.second;

                    std::string left = result1.first.empty() ? result1.second.toString() : result1.first;
                    std::string right = result2.first.empty() ? result2.second.toString() : result2.first;

                    std::cout << "\n" << left << " - " << right << " = " << diff << std::endl;

                    if (!diff.isEmpty()) {
                        std::string resultName = generateAutoName();
                        variables[resultName] = diff;
                        std::cout << "Результат сохранен как " << resultName << std::endl;
                    }
                    break;
                }
                case 4: {
                    std::cout << "\n--- УМНОЖЕНИЕ ПОЛИНОМОВ ---\n";
                    std::cout << "Можно использовать предустановленные: quadratic, linear, two_vars\n";
                    auto result1 = inputPolynomialByName("Введите первый полином или имя переменной: ");
                    if (result1.second.isEmpty()) continue;

                    auto result2 = inputPolynomialByName("Введите второй полином или имя переменной: ");
                    if (result2.second.isEmpty()) continue;

                    Polynom product = result1.second * result2.second;

                    std::string left = result1.first.empty() ? result1.second.toString() : result1.first;
                    std::string right = result2.first.empty() ? result2.second.toString() : result2.first;

                    std::cout << "\n(" << left << ") * (" << right << ") = " << product << std::endl;

                    if (!product.isEmpty()) {
                        std::string resultName = generateAutoName();
                        variables[resultName] = product;
                        std::cout << "Результат сохранен как " << resultName << std::endl;
                    }
                    break;
                }
                case 5: {
                    std::cout << "\n--- УМНОЖЕНИЕ ПОЛИНОМА НА ЧИСЛО ---\n";
                    std::cout << "Можно использовать предустановленные: quadratic, linear, two_vars\n";
                    auto result = inputPolynomialByName("Введите полином или имя переменной: ");
                    if (result.second.isEmpty()) continue;

                    double num = inputNumber("Введите число: ");
                    Polynom product = result.second * num;

                    std::string polyStr = result.first.empty() ? result.second.toString() : result.first;

                    std::cout << "\n" << polyStr << " * " << num << " = " << product << std::endl;

                    if (!product.isEmpty()) {
                        std::string resultName = generateAutoName();
                        variables[resultName] = product;
                        std::cout << "Результат сохранен как " << resultName << std::endl;
                    }
                    break;
                }
                case 6: {
                    std::cout << "\n--- ДЕЛЕНИЕ ПОЛИНОМА НА ЧИСЛО ---\n";
                    std::cout << "Можно использовать предустановленные: quadratic, linear, two_vars\n";
                    auto result = inputPolynomialByName("Введите полином или имя переменной: ");
                    if (result.second.isEmpty()) continue;

                    double num = inputNumber("Введите число: ");

                    try {
                        Polynom quotient = result.second / num;

                        std::string polyStr = result.first.empty() ? result.second.toString() : result.first;

                        std::cout << "\n" << polyStr << " / " << num << " = " << quotient << std::endl;

                        if (!quotient.isEmpty()) {
                            std::string resultName = generateAutoName();
                            variables[resultName] = quotient;
                            std::cout << "Результат сохранен как " << resultName << std::endl;
                        }
                    }
                    catch (const std::exception& e) {
                        std::cout << "Ошибка: " << e.what() << std::endl;
                    }
                    break;
                }
                case 7: {
                    std::cout << "\n--- ВЫЧИСЛЕНИЕ ЗНАЧЕНИЯ ПОЛИНОМА ---\n";
                    std::cout << "Можно использовать предустановленные: quadratic, linear, two_vars\n";
                    auto result = inputPolynomialByName("Введите полином или имя переменной: ");
                    if (result.second.isEmpty()) continue;

                    double x, y, z;
                    std::cout << "Введите значение для x: ";
                    x = inputNumber("");
                    std::cout << "Введите значение для y: ";
                    y = inputNumber("");
                    std::cout << "Введите значение для z: ";
                    z = inputNumber("");

                    try {
                        double evalResult = result.second.evaluate(x, y, z);

                        std::string polyStr = result.first.empty() ? result.second.toString() : result.first;

                        std::cout << "\n" << polyStr << "(" << x << ", " << y << ", " << z << ") = " << evalResult << std::endl;
                    }
                    catch (const std::exception& e) {
                        std::cout << "Ошибка вычисления: " << e.what() << std::endl;
                    }
                    break;
                }
                case 8: {
                    std::cout << "\n--- ПРОИЗВОДНАЯ ПОЛИНОМА ---\n";
                    std::cout << "Можно использовать предустановленные: quadratic, linear, two_vars\n";
                    auto result = inputPolynomialByName("Введите полином или имя переменной: ");
                    if (result.second.isEmpty()) continue;

                    std::string var;
                    std::cout << "Введите переменную для дифференцирования (x/y/z): ";
                    std::getline(std::cin, var);

                    if (var.empty()) var = "x";

                    Polynom derivative = result.second.derivative(var[0]);

                    std::string polyStr = result.first.empty() ? result.second.toString() : result.first;

                    std::cout << "\nd/d" << var << "(" << polyStr << ") = " << derivative << std::endl;

                    if (!derivative.isEmpty()) {
                        std::string resultName = generateAutoName();
                        variables[resultName] = derivative;
                        std::cout << "Результат сохранен как " << resultName << std::endl;
                    }
                    break;
                }
                case 9: {
                    std::cout << "\n--- ИНТЕГРАЛ ПОЛИНОМА ---\n";
                    std::cout << "Можно использовать предустановленные: quadratic, linear, two_vars\n";
                    auto result = inputPolynomialByName("Введите полином или имя переменной: ");
                    if (result.second.isEmpty()) continue;

                    std::string var;
                    std::cout << "Введите переменную для интегрирования (x/y/z): ";
                    std::getline(std::cin, var);

                    if (var.empty()) var = "x";

                    Polynom integral = result.second.integral(var[0]);

                    std::string polyStr = result.first.empty() ? result.second.toString() : result.first;

                    std::cout << "\ninteg(" << polyStr << ")d" << var << " = " << integral << std::endl;

                    if (!integral.isEmpty()) {
                        std::string resultName = generateAutoName();
                        variables[resultName] = integral;
                        std::cout << "Результат сохранен как " << resultName << std::endl;
                    }
                    break;
                }
                case 10: {
                    showAllVariables();
                    break;
                }
                case 11: {
                    std::cout << "\n--- РУЧНОЕ СОХРАНЕНИЕ В ПЕРЕМЕННУЮ ---\n";
                    auto result = inputPolynomialByName("Введите полином для сохранения: ");
                    if (result.second.isEmpty()) continue;

                    std::string varName;
                    std::cout << "Введите имя переменной: ";
                    std::getline(std::cin, varName);

                    if (varName.empty()) {
                        std::cout << "Ошибка: имя переменной не может быть пустым\n";
                        break;
                    }

                    variables[varName] = result.second;
                    std::cout << "\n" << varName << " = " << result.second << std::endl;
                    break;
                }
                case 12: {
                    std::cout << "\n--- ЗАГРУЗКА ПОЛИНОМА ПО ИМЕНИ ---\n";
                    std::cout << "Предустановленные: quadratic, linear, two_vars\n";
                    std::string varName;
                    std::cout << "Введите имя переменной: ";
                    std::getline(std::cin, varName);

                    if (variables.find(varName) != variables.end()) {
                        std::cout << "\n" << varName << " = " << variables[varName] << std::endl;
                    }
                    else {
                        std::cout << "Ошибка: переменная '" << varName << "' не найдена\n";
                        std::cout << "Доступные переменные:\n";
                        int count = 0;
                        for (const auto& var : variables) {
                            if (count < 5) { // Показываем только первые 5
                                std::cout << "  " << var.first << std::endl;
                                count++;
                            }
                        }
                        if (variables.size() > 5) {
                            std::cout << "  ... и еще " << (variables.size() - 5) << " переменных\n";
                        }
                    }
                    break;
                }
                case 13: {
                    showExamples();
                    break;
                }
                case 14: {
                    std::cout << "\n--- УДАЛЕНИЕ ПЕРЕМЕННОЙ ---\n";
                    std::cout << "ВНИМАНИЕ: нельзя удалять предустановленные полиномы!\n";
                    std::string varName;
                    std::cout << "Введите имя переменной для удаления: ";
                    std::getline(std::cin, varName);

                    // Проверяем, не пытаемся ли удалить предустановленный полином
                    if (varName == "quadratic" || varName == "linear" || varName == "two_vars") {
                        std::cout << "Ошибка: нельзя удалять предустановленные полиномы!\n";
                        break;
                    }

                    if (variables.erase(varName) > 0) {
                        std::cout << "Переменная '" << varName << "' удалена\n";
                    }
                    else {
                        std::cout << "Ошибка: переменная '" << varName << "' не найдена\n";
                    }
                    break;
                }
                case 15: {
                    showDefaultPolynomials();
                    break;
                }
                case 16: {
                    demonstrateOperations();
                    break;
                }
                default:
                    std::cout << "Неверный выбор. Попробуйте снова.\n";
                }
            }
            catch (const std::exception& e) {
                std::cout << "Ошибка: " << e.what() << std::endl;
            }

            std::cout << "\nНажмите Enter для продолжения...";
            std::cin.get();
        }

        // Показываем итоговые переменные при выходе
        std::cout << "\n=== ИТОГОВЫЕ ПЕРЕМЕННЫЕ ===\n";
        showAllVariables();
        std::cout << "\nСпасибо за использование калькулятора полиномов!\n";
    }
};

