// Copyright 2024 Marina Usova

//#define EASY_EXAMPLE
//
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

#include <ctime>
#include <cstdlib>
#include <string>
#include "../lib_skiplist/skiplist.h"

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    SkipList<std::string, double> skip_list(6);

    skip_list.insert("key6", 7.523);
    skip_list.insert("key2", 4.0);
    skip_list.insert("key1", 12.15);
    skip_list.insert("key13", 87.2);
    skip_list.insert("key655", 58.4238);
    skip_list.insert("key234", 78.5);
    skip_list.insert("key634", 5.8);
    skip_list.insert("key7654", 8.769455);

    skip_list.print();

    return 0;
}