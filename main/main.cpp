// Copyright 2024 Marina Usova

//#define EASY_EXAMPLE
//#ifdef EASY_EXAMPLE
//
//#include <iostream>
//#include <iomanip>
//#include "../lib_easy_example/easy_example.h"
//
//int main() {
//  int a, b;
//  float result;
//
//  a = 1; b = 4;
//
//  try {
//      result = division(a, b);
//      std::cout << a << " / " << b << " = "
//          << std::setprecision(2) << result << std::endl;
//  } catch (std::exception err) {
//      std::cerr << err.what() << std::endl;
//  }
//
//  a = 1; b = 0;
//
//  try {
//      result = division(a, b);
//      std::cout << a << " / " << b << " = "
//          << std::setprecision(2) << result << std::endl;
//  } catch (std::exception err) {
//      std::cerr << err.what() << std::endl;
//  }
//
//  return 0;
//}
//
//#endif  // EASY_EXAMPLE

#include <iostream>
#include <vector>
#include <string>
#include "../lib_tk2/tk2.h"

int main() {
    // 1-й словарь
    std::vector<std::pair<std::string, int>> dict1 = {
        {"hash", 10072},
        {"list", 13736},
        {"vector", 16262},
        {"massive", 10626},
        {"table", 10083}
    };

    // 2-й словарь
    std::vector<std::pair<std::string, int>> dict2 = {
        {"table", 22727},
        {"vector", 20728},
        {"queue", 28282},
        {"stack", 20607},
        {"list", 20618}
    };

    std::cout << "Merging dictionaries..." << std::endl;

    // Вызываем функцию объединения
    HashTable<int>& result_table = merge_dict(dict1, dict2);

    // Выводим результат на экран
    std::vector<std::string> keys = result_table.getKeys();
    std::vector<int> values = result_table.getValues();

    std::cout << "\nResulting Merged Dictionary:" << std::endl;
    for (size_t i = 0; i < keys.size(); ++i) {
        std::cout << "\"" << keys[i] << "\" - " << values[i] << std::endl;
    }

    // Освобождаем память, так как merge_dict вернул ссылку на кучу
    delete& result_table;

    return 0;
}
