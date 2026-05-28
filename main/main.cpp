#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

#include "../lib_unsorted_table_tree/unsorted_table_tree.h"
#include "../lib_sorted_table_bst/sorted_table_bst.h"
#include "../lib_sorted_table_avl/sorted_table_avl.h"
#include "../lib_sorted_table_rb/sorted_table_rb.h"
#include "../lib_polynom/polynom.h"

#include "../lib_tk2/tk2.h"

void displayTreeTableContent(const std::string& tableName, const ITable<std::string, Polynom>& table) {
    std::cout << " table contents: " << tableName << " (size: " << table.size() << ")\n";

    std::vector<std::string> keys = table.getKeys();
    std::vector<Polynom> values = table.getValues();

    for (size_t i = 0; i < keys.size(); ++i) {
        std::cout << "  Key: [ " << std::setw(10) << std::left << keys[i]
            << " ]  --->  polynom: " << values[i].toString() << "\n";
    }
}

// Визуализация работы таблиц на деревьях 
void runTreeTablesVisualization() {
    std::cout << "     VISUALIZATION AND TESTING OF TREE-BASED TABLES\n";

    // 1. Проверка BST
    std::cout << "\n Checking an ordered table in a Binary Search Tree\n";
    SortedTableOnBST<std::string, Polynom> bstTable;
    std::cout << "--> Inserting the keys in a chaotic order: ('Beta', 'Omega', 'Alpha', 'Gamma')\n";
    bstTable.insert("Beta", Polynom("2x^2 + 5x"));
    bstTable.insert("Omega", Polynom("x^3 - 1"));
    bstTable.insert("Alpha", Polynom("7x^5"));
    bstTable.insert("Gamma", Polynom("3y^2 - z"));

    displayTreeTableContent("SortedTableOnBST (Infix tree traversal)", bstTable);
    std::cout << "The keys were arranged alphabetically automatically (In-Order traversal).\n";

    // 2. Проверка структуры дерева общего вида
    std::cout << "\n Visualization of node locations by tree level (BFS)\n";
    UnsortedTableOnTree<std::string, Polynom> unsortedTreeTable;
    unsortedTreeTable.insert("Root", Polynom("5"));
    unsortedTreeTable.insert("Left_1", Polynom("10"));
    unsortedTreeTable.insert("Right_1", Polynom("15"));

    std::cout << "\nStarting built-in serialization (uses Level-Order traversal of the tree):\n";
    std::cout << unsortedTreeTable.serialize();

    std::cout << "\nA graphical representation of the location of this table in RAM:\n";
    std::cout << "               [ Root (5) ]                <-- Level 0\n";
    std::cout << "              /              \\\n";
    std::cout << "      [ Left_1 (10) ]     [ Right_1 (15) ] <-- Level 1\n";

    // 3. Проверка Move-семантики
    std::cout << "\n Demonstration of Move Semantics on an AVL Tree\n";
    SortedTableOnAVL<std::string, Polynom> avlTable;
    Polynom heavyPoly("x^10 + 5x^9 - 3x^4 + 2x - 11");
    std::cout << "    Size of the list of monomes before moving: " << heavyPoly.getMonoms().size() << " elements.\n";

    avlTable.insert("HeavyKey", std::move(heavyPoly));
    std::cout << "    The polynomial has been successfully captured by the AVL table!\n";
    std::cout << "   Checking the data in the table: " << avlTable.find("HeavyKey").toString() << "\n";
}

// Объединение словарей с использованием Хэш-таблиц 
void runDictionaryMerge() {
    std::cout << "    MERGING DICTIONARIES ON HASH TABLES\n";

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

    HashTable<int>& result_table = merge_dict(dict1, dict2);

    std::vector<std::string> keys = result_table.getKeys();
    std::vector<int> values = result_table.getValues();

    std::cout << "\nResulting Merged Dictionary:" << std::endl;
    for (size_t i = 0; i < keys.size(); ++i) {
        std::cout << "\"" << keys[i] << "\" - " << values[i] << std::endl;
    }

    delete& result_table;
}

int main() {
    int choice = -1;

    while (true) {
        std::cout << "              Choose           \n";
        std::cout << " 1. Visualize the operation of tables on trees\n";
        std::cout << " 2. Start merging dictionaries \n";
        std::cout << " 0. Exit\n";
        std::cout << "Your choose: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string invalidInput;
            std::cin >> invalidInput;
            std::cout << "\nError!\n\n";
            continue;
        }

        if (choice == 0) {
            std::cout << "Exit\n";
            break;
        }

        switch (choice) {
        case 1:
            runTreeTablesVisualization();
            break;
        case 2:
            runDictionaryMerge();
            break;
        default:
            std::cout << "\nNo such found \n\n";
            break;
        }
    }

    return 0;
}