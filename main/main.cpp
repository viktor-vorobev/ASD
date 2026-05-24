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
#include <iostream>
#include <vector>
#include "../lib_heap/heap.cpp"

// —труктура узла дл€ хранени€ пациентов в куче
struct PriorityNode {
    std::string name;
    int priority;   // 1 - платно, 2 - по записи, 3 - без записи
    size_t order;   // јвтоматический пор€дковый номер прихода дл€ FIFO

    //  онструкторы
    PriorityNode() : priority(3), order(0) {}
    PriorityNode(std::string n, int p, size_t o) : name(n), priority(p), order(o) {}

    
    bool operator<(const PriorityNode& other) const {
       
        if (priority != other.priority) {
            return priority < other.priority;
        }
      
        return order < other.order;
    }
};

struct Patient {
    std::string name;
    int arrival_time; 
    int priority;    
};

int timeToMinutes(const std::string& timeStr) {
    size_t sep = timeStr.find('.');
    if (sep == std::string::npos) sep = timeStr.find(':');
    int hours = std::stoi(timeStr.substr(0, sep));
    int minutes = std::stoi(timeStr.substr(sep + 1));
    return hours * 60 + minutes;
}

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

    std::vector<int> arr = { 13, 5, 8, 3, 9, 7, 10, 11, 6, 4 };

    std::cout << "original array: ";
    for (int num : arr) std::cout << num << " ";
    std::cout << "\n";

    std::vector<int> sortedArr = heapSort(arr);

    std::cout << "sorted array: ";
    for (int num : sortedArr) std::cout << num << " ";
    std::cout << "\n";

    std::vector<Patient> patients = {
        {"Ivanov",      timeToMinutes("12.30"), 2},
        {"Petrov",      timeToMinutes("12.35"), 3},
        {"Voronin",     timeToMinutes("12.50"), 2},
        {"Sidorov",     timeToMinutes("12.55"), 1},
        {"Vasiliev",    timeToMinutes("13.10"), 2},
        {"Mihailov",    timeToMinutes("13.15"), 3},
        {"Borisov",     timeToMinutes("14.05"), 1},
        {"Vladimirov",  timeToMinutes("14.07"), 1},
        {"Fedorov",     timeToMinutes("14.30"), 2},
        {"Nesterov",    timeToMinutes("14.50"), 2}
    };

    MinHeap<PriorityNode> heap;

    std::vector<std::string> admission_order; 
    size_t global_counter = 0;               
    size_t next_patient_idx = 0;             

    int current_time = patients[0].arrival_time;
    const int appointment_duration = 20;     

    while (next_patient_idx < patients.size() || !heap.empty()) { 
        if (heap.empty() && current_time < patients[next_patient_idx].arrival_time) { 
            current_time = patients[next_patient_idx].arrival_time;
        }

        while (next_patient_idx < patients.size() && patients[next_patient_idx].arrival_time <= current_time) {
            heap.insert(PriorityNode(
                patients[next_patient_idx].name,
                patients[next_patient_idx].priority,
                global_counter++
            ));
            next_patient_idx++;
        }

        if (!heap.empty()) { //
            PriorityNode treated_patient = heap.pop(); 
            admission_order.push_back(treated_patient.name);

            current_time += appointment_duration;
        }
    }

    std::cout << "Patient admission procedure:\n";
    std::cout << "-------------------------\n";
    for (size_t i = 0; i < admission_order.size(); ++i) {
        std::cout << i + 1 << ". " << admission_order[i] << "\n";
    }

    return 0;
}