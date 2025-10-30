#include "hash-t.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;



// SET_AT - проверка наличия элемента в множестве

bool SET_AT(HTable* ht, const string& key) {
    return contains(ht, key);
}

// SETADD - добавление элемента в множество
bool SETADD(HTable* ht, const string& key, int value = 0) {
    return insert(ht, key, value);
}

// SETDEL - удаление элемента из множества
bool SETDEL(HTable* ht, const string& key) {
    return remove(ht, key);
}

// Объединение множеств
HTable* SETUNI(HTable* ht1, HTable* ht2) {
    int newCapacity = ht1->capacity + ht2->capacity;
    HTable* result = createTable(newCapacity);
    for (int i = 0; i < ht1->capacity; i++) {
        HNode* curr = ht1->buckets[i];
        while (curr != nullptr) {
            insert(result, curr->key, curr->data);
            curr = curr->next;
        }
    }
    for (int i = 0; i < ht2->capacity; i++) {
        HNode* curr = ht2->buckets[i];
        while (curr != nullptr) {
            insert(result, curr->key, curr->data);
            curr = curr->next;
        }
    }
    return result;
}

// Пересечение множеств
HTable* SETINTERS(HTable* ht1, HTable* ht2) {
    int newCapacity = min(ht1->capacity, ht2->capacity);
    HTable* result = createTable(newCapacity);
    
    for (int i = 0; i < ht1->capacity; i++) {
        HNode* curr = ht1->buckets[i];
        while (curr != nullptr) {
            if (contains(ht2, curr->key)) {
                insert(result, curr->key, curr->data);
            }
            curr = curr->next;
        }
    }
    return result;
}

// Разность множеств
HTable* SETDIFER(HTable* ht1, HTable* ht2) {
    int newCapacity = ht1->capacity;
    HTable* result = createTable(newCapacity);
    
    for (int i = 0; i < ht1->capacity; i++) {
        HNode* curr = ht1->buckets[i];
        while (curr != nullptr) {
            if (!contains(ht2, curr->key)) {
                insert(result, curr->key, curr->data);
            }
            curr = curr->next;
        }
    }
    return result;
}

// Симметрическая разность
HTable* symmetricDifferenceSets(HTable* ht1, HTable* ht2) {
    HTable* diff1 = SETDIFER(ht1, ht2);
    HTable* diff2 = SETDIFER(ht2, ht1);
    HTable* result = SETUNI(diff1, diff2);
    
    deleteTable(diff1);
    deleteTable(diff2);
    
    return result;
}

// Вывод множества
void printSet(HTable* ht) {
    cout << "{ ";
    bool first = true;
    for (int i = 0; i < ht->capacity; i++) {
        HNode* curr = ht->buckets[i];
        while (curr != nullptr) {
            if (!first) cout << ", ";
            cout << curr->key;
            first = false;
            curr = curr->next;
        }
    }
    cout << " }" << endl;
}

// Функция для сохранения множества в файл
void saveToFile(HTable* ht, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << " for writing" << endl;
        return;
    }
    
    for (int i = 0; i < ht->capacity; i++) {
        HNode* curr = ht->buckets[i];
        while (curr != nullptr) {
            file << curr->key << endl;
            curr = curr->next;
        }
    }
    
    file.close();
    cout << "Set saved to " << filename << endl;
}

// Функция для загрузки данных из файла
HTable* loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return nullptr;
    }
    
    HTable* ht = createTable(100);
    
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            insert(ht, line, 0);
        }
    }
    
    file.close();
    return ht;
}

// Функция для создания нового файла с множеством
bool createNewSetFile(const string& filename, const string& initialData = "") {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot create file " << filename << endl;
        return false;
    }
    
    if (!initialData.empty()) {
        istringstream iss(initialData);
        string item;
        while (iss >> item) {
            file << item << endl;
        }
    }
    
    file.close();
    cout << "New set file created: " << filename << endl;
    return true;
}

// Функция для добавления элемента в файл
void addToFile(const string& filename, const string& key) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << key << endl;
        file.close();
    }
}

// Функция для обработки запросов

void processQuery(HTable* ht, const string& query, const string& filename = "", const string& outputFile = "") {
    istringstream iss(query);
    string operation, param1, param2;
    iss >> operation >> param1 >> param2;
    
    if (operation == "SETADD") {
        if (SETADD(ht, param1, 0)) {
            cout << "Element '" << param1 << "' added successfully" << endl;
            if (!filename.empty()) {
                addToFile(filename, param1);
            }
        } else {
            cout << "Element '" << param1 << "' already exists" << endl;
        }
    }
    else if (operation == "SETDEL") {
        if (SETDEL(ht, param1)) {
            cout << "Element '" << param1 << "' deleted successfully" << endl;
            if (!filename.empty()) {
                saveToFile(ht, filename);
            }
        } else {
            cout << "Element '" << param1 << "' not found" << endl;
        }
    }
    else if (operation == "SET_AT") {
        if (SET_AT(ht, param1)) {
            cout << "Element '" << param1 << "' exists in the set" << endl;
        } else {
            cout << "Element '" << param1 << "' not found in the set" << endl;
        }
    }
    else if (operation == "PRINT") {
        cout << "Current set: ";
        printSet(ht);
    }
    else if (operation == "SIZE") {
        cout << "Set size: " << getSize(ht) << endl;
    }
    else if (operation == "UNION" || operation == "INTERSECTION" || 
             operation == "DIFFERENCE" || operation == "SYMMETRIC_DIFFERENCE") {
        
        if (param1.empty()) {
            cout << "Error: Specify second set file for " << operation << " operation" << endl;
            return;
        }
        
        HTable* secondSet = loadFromFile(param1);
        if (!secondSet) {
            cout << "Error: Cannot load set from " << param1 << endl;
            return;
        }
        
        HTable* result = nullptr;
        
        if (operation == "UNION") {
            result = SETUNI(ht, secondSet);
            cout << "Union result: ";
        }
        else if (operation == "INTERSECTION") {
            result = SETINTERS(ht, secondSet);
            cout << "Intersection result: ";
        }
        else if (operation == "DIFFERENCE") {
            result = SETDIFER(ht, secondSet);
            cout << "Difference result (A - B): ";
        }
        else if (operation == "SYMMETRIC_DIFFERENCE") {
            result = symmetricDifferenceSets(ht, secondSet);
            cout << "Symmetric difference result: ";
        }
        
        printSet(result);
        
        if (!outputFile.empty()) {
            saveToFile(result, outputFile);
            cout << "Result saved to " << outputFile << endl;
        }
        
        deleteTable(secondSet);
        deleteTable(result);
    }
    else {
        cout << "Unknown operation: " << operation << endl;
        cout << "Available operations: SETADD, SETDEL, SET_AT, PRINT, SIZE, UNION, INTERSECTION, DIFFERENCE, SYMMETRIC_DIFFERENCE" << endl;
    }
}

// Функция для вывода помощи
void printHelp() {
    cout << "Использование: ./program --file <имя_файла> [опции]" << endl;
    cout << "Опции:" << endl;
    cout << "  --query <операция>      - Выполнить операцию над множеством" << endl;
    cout << "  --create / -c           - Создать новый файл множества" << endl;
    cout << "  --data <элементы>       - Начальные данные для нового множества (через пробел)" << endl;
    cout << "  --output <имя_файла>    - Выходной файл для операций с множеством" << endl;
    cout << "  --help / -h             - Показать эту справку" << endl;
    cout << endl;
    cout << "Операции для --query:" << endl;
    cout << "  SETADD <ключ>              - Добавить элемент в множество" << endl;
    cout << "  SETDEL <ключ>              - Удалить элемент из множества" << endl;
    cout << "  SET_AT <ключ>              - Проверить наличие элемента" << endl;
    cout << "  PRINT                      - Вывести все элементы" << endl;
    cout << "  SIZE                       - Получить размер множества" << endl;
    cout << "  UNION <файл_множества2>    - Объединение с другим множеством" << endl;
    cout << "  INTERSECTION <файл_множества2> - Пересечение с другим множеством" << endl;
    cout << "  DIFFERENCE <файл_множества2>   - Разность с другим множеством (A - B)" << endl;
    cout << "  SYMMETRIC_DIFFERENCE <файл_множества2> - Симметрическая разность" << endl;
    cout << endl;
    cout << "Примеры:" << endl;
    cout << "  ./zad2_set --file setA.txt --query \"UNION setB.txt\" --output result.txt" << endl;
    cout << "  ./zad2_set --file setA.txt --query \"INTERSECTION setB.txt\"" << endl;
    cout << "  ./zad2_set --file setA.txt --query \"DIFFERENCE setB.txt\"" << endl;
}

// main функция
int main(int argc, char* argv[]) {
    string filename, query, outputFile;
    bool createNew = false;
    string initialData;
    
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        }
        else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
        else if (arg == "--create" || arg == "-c") {
            createNew = true;
        }
        else if (arg == "--data" && i + 1 < argc) {
            initialData = argv[++i];
        }
        else if (arg == "--output" && i + 1 < argc) {
            outputFile = argv[++i];
        }
        else if (arg == "--help" || arg == "-h") {
            printHelp();
            return 0;
        }
    }
    
    if (filename.empty()) {
        cout << "Error: Missing filename" << endl;
        printHelp();
        return 1;
    }
    
    if (createNew) {
        if (!createNewSetFile(filename, initialData)) {
            return 1;
        }
    }
    
    HTable* set = loadFromFile(filename);
    if (!set) {
        cout << "File not found. Use --create to create a new set file." << endl;
        return 1;
    }
    
    cout << "Loaded " << getSize(set) << " elements from " << filename << endl;
    
    if (!query.empty()) {
        processQuery(set, query, filename, outputFile);
    } else {
        cout << "No query specified. Use --query <operation>" << endl;
    }
    
    deleteTable(set);
    return 0;
}
