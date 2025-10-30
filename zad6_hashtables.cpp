#include "hash-t.h"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

enum HashMethod {
    CHAINING,
    OPEN_ADDRESSING
};

// Глобальные переменные
HashMethod currentMethod = CHAINING;
void* currentTable = nullptr;
string currentFilename;


// Функция для команды GET
void getCommand(const string& key) {
    if (currentTable == nullptr) {
        cout << "Error: No table created" << endl;
        return;
    }
    
    int value;
    if (currentMethod == CHAINING) {
        value = getValue(static_cast<HTable*>(currentTable), key);
    } else {
        value = getValueOA(static_cast<OATable*>(currentTable), key);
    }
    
    if (value != -1) {
        cout << "Value for key '" << key << "': " << value << endl;
    } else {
        cout << "Key '" << key << "' not found" << endl;
    }
}

// Функции для работы с выбранным методом
void createTableCommand(int capacity) {
    if (currentTable != nullptr) {
        if (currentMethod == CHAINING) {
            deleteTable(static_cast<HTable*>(currentTable));
        } else {
            deleteOATable(static_cast<OATable*>(currentTable));
        }
    }
    
    if (currentMethod == CHAINING) {
        currentTable = createTable(capacity);
    } else {
        currentTable = createOATable(capacity);
    }
    
    cout << "Created hash table with capacity " << capacity << endl;
}

bool insertCommand(const string& key, int value = 0) {
    if (currentTable == nullptr) {
        cout << "Error: No table created" << endl;
        return false;
    }
    
    bool success;
    if (currentMethod == CHAINING) {
        success = insert(static_cast<HTable*>(currentTable), key, value);
    } else {
        success = insertOA(static_cast<OATable*>(currentTable), key, value);
    }
    
    if (success) {
        cout << "Element '" << key << "' inserted successfully" << endl;
    } else {
        cout << "Element '" << key << "' already exists or table full" << endl;
    }
    return success;
}

bool removeCommand(const string& key) {
    if (currentTable == nullptr) {
        cout << "Error: No table created" << endl;
        return false;
    }
    
    bool success;
    if (currentMethod == CHAINING) {
        success = remove(static_cast<HTable*>(currentTable), key);
    } else {
        success = removeOA(static_cast<OATable*>(currentTable), key);
    }
    
    if (success) {
        cout << "Element '" << key << "' removed successfully" << endl;
    } else {
        cout << "Element '" << key << "' not found" << endl;
    }
    return success;
}

bool containsCommand(const string& key) {
    if (currentTable == nullptr) {
        cout << "Error: No table created" << endl;
        return false;
    }
    
    bool exists;
    if (currentMethod == CHAINING) {
        exists = contains(static_cast<HTable*>(currentTable), key);
    } else {
        exists = containsOA(static_cast<OATable*>(currentTable), key);
    }
    
    if (exists) {
        cout << "Element '" << key << "' exists in the table" << endl;
    } else {
        cout << "Element '" << key << "' not found in the table" << endl;
    }
    return exists;
}

void printCommand() {
    if (currentTable == nullptr) {
        cout << "Error: No table created" << endl;
        return;
    }
    
    if (currentMethod == CHAINING) {
        printTable(static_cast<HTable*>(currentTable));
    } else {
        printOATable(static_cast<OATable*>(currentTable));
    }
}

void sizeCommand() {
    if (currentTable == nullptr) {
        cout << "Error: No table created" << endl;
        return;
    }
    
    int size;
    if (currentMethod == CHAINING) {
        size = getSize(static_cast<HTable*>(currentTable));
    } else {
        size = getSizeOA(static_cast<OATable*>(currentTable));
    }
    
    cout << "Table size: " << size << endl;
}

// Функции для работы с файлами
void loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return;
    }
    
    // Создаем таблицу если её нет
    if (currentTable == nullptr) {
        createTableCommand(10);
    }
    
    string line;
    int count = 0;
    while (getline(file, line)) {
        if (!line.empty()) {
            if (insertCommand(line, 0)) {
                count++;
            }
        }
    }
    
    file.close();
    currentFilename = filename;
    cout << "Loaded " << count << " elements from " << filename << endl;
}

void saveToFile(const string& filename) {
    if (currentTable == nullptr) {
        cout << "Error: No table to save" << endl;
        return;
    }
    
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << " for writing" << endl;
        return;
    }
    
    // Для метода цепочек
    if (currentMethod == CHAINING) {
        HTable* ht = static_cast<HTable*>(currentTable);
        for (int i = 0; i < ht->capacity; i++) {
            HNode* curr = ht->buckets[i];
            while (curr != nullptr) {
                file << curr->key << " " << curr->data << endl;
                curr = curr->next;
            }
        }
    }
    // Для метода открытой адресации
    else {
        OATable* ht = static_cast<OATable*>(currentTable);
        for (int i = 0; i < ht->capacity; i++) {
            if (ht->cells[i].state == OCCUPIED) {
                file << ht->cells[i].key << " " << ht->cells[i].value << endl;
            }
        }
    }
    
    file.close();
    cout << "Table saved to " << filename << endl;
}

// Функция для обработки запросов
void processQuery(const string& query) {
    istringstream iss(query);
    string operation;
    iss >> operation;
    
    if (operation == "CREATE") {
        int capacity;
        if (iss >> capacity) {
            createTableCommand(capacity);
        } else {
            cout << "Usage: CREATE <capacity>" << endl;
        }
    }
    else if (operation == "INSERT") {
        string key;
        int value = 0;
        if (iss >> key) {
            iss >> value; // значение опционально
            insertCommand(key, value);
        } else {
            cout << "Usage: INSERT <key> [value]" << endl;
        }
    }
    else if (operation == "REMOVE") {
        string key;
        if (iss >> key) {
            removeCommand(key);
        } else {
            cout << "Usage: REMOVE <key>" << endl;
        }
    }
    else if (operation == "CONTAINS") {
        string key;
        if (iss >> key) {
            containsCommand(key);
        } else {
            cout << "Usage: CONTAINS <key>" << endl;
        }
    }
    else if (operation == "GET") {
        string key;
        if (iss >> key) {
            getCommand(key);
        } else {
            cout << "Usage: GET <key>" << endl;
        }
    }
    else if (operation == "PRINT") {
        printCommand();
    }
    else if (operation == "SIZE") {
        sizeCommand();
    }
    else if (operation == "LOAD") {
        string filename;
        if (iss >> filename) {
            loadFromFile(filename);
        } else {
            cout << "Usage: LOAD <filename>" << endl;
        }
    }
    else if (operation == "SAVE") {
        string filename;
        if (iss >> filename) {
            saveToFile(filename);
        } else if (!currentFilename.empty()) {
            saveToFile(currentFilename);
        } else {
            cout << "Usage: SAVE <filename>" << endl;
        }
    }
    else if (operation == "METHOD") {
        string method;
        if (iss >> method) {
            if (method == "CHAINING") {
                currentMethod = CHAINING;
                cout << "Switched to CHAINING method" << endl;
            }
            else if (method == "OPEN_ADDRESSING" || method == "OA") {
                currentMethod = OPEN_ADDRESSING;
                cout << "Switched to OPEN ADDRESSING method" << endl;
            }
            else {
                cout << "Unknown method. Use CHAINING or OPEN_ADDRESSING" << endl;
            }
        } else {
            cout << "Usage: METHOD <CHAINING|OPEN_ADDRESSING>" << endl;
        }
    }
    else {
        cout << "Unknown operation: " << operation << endl;
        cout << "Available operations: CREATE, INSERT, REMOVE, CONTAINS, GET, PRINT, SIZE, LOAD, SAVE, METHOD" << endl;
    }
}

// Функция для вывода помощи
void printHelp() {
    cout << "Hash Table Manager - Command Line Interface" << endl;
    cout << "Usage: ./zad6_hashtables [options]" << endl;
    cout << endl;
    cout << "Options:" << endl;
    cout << "  --file <filename>      - Load hash table from file" << endl;
    cout << "  --method <method>      - Set method: CHAINING or OPEN_ADDRESSING" << endl;
    cout << "  --create <capacity>    - Create new table with given capacity" << endl;
    cout << "  --query <operation>    - Execute operation" << endl;
    cout << "  --output <filename>    - Output file for SAVE operation" << endl;
    cout << "  --help                 - Show this help" << endl;
    cout << endl;
    cout << "Operations for --query:" << endl;
    cout << "  CREATE <capacity>          - Create new hash table" << endl;
    cout << "  INSERT <key> [value]       - Insert key-value pair" << endl;
    cout << "  REMOVE <key>               - Remove key from table" << endl;
    cout << "  CONTAINS <key>             - Check if key exists" << endl;
    cout << "  GET <key>                  - Get value by key" << endl;
    cout << "  PRINT                      - Print hash table" << endl;
    cout << "  SIZE                       - Get table size" << endl;
    cout << "  LOAD <filename>            - Load data from file" << endl;
    cout << "  SAVE [filename]            - Save data to file" << endl;
    cout << "  METHOD <CHAINING|OPEN_ADDRESSING> - Switch method" << endl;
    cout << endl;
    cout << "Examples:" << endl;
    cout << "  ./zad6_hashtables --file data.txt --method CHAINING --query \"GET apple\"" << endl;
    cout << "  ./zad6_hashtables --create 50 --query \"INSERT apple 5\" --query \"GET apple\"" << endl;
    cout << "  ./zad6_hashtables --file input.txt --query \"SAVE output.txt\"" << endl;
}

int main(int argc, char* argv[]) {
    string query, outputFile;
    
    // Обработка аргументов командной строки
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        
        if (arg == "--file" && i + 1 < argc) {
            string filename = argv[++i];
            loadFromFile(filename);
        }
        else if (arg == "--method" && i + 1 < argc) {
            string method = argv[++i];
            if (method == "CHAINING") {
                currentMethod = CHAINING;
            } else if (method == "OPEN_ADDRESSING" || method == "OA") {
                currentMethod = OPEN_ADDRESSING;
            }
        }
        else if (arg == "--create" && i + 1 < argc) {
            int capacity = stoi(argv[++i]);
            createTableCommand(capacity);
        }
        else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
            processQuery(query);
        }
        else if (arg == "--output" && i + 1 < argc) {
            outputFile = argv[++i];
        }
        else if (arg == "--help") {
            printHelp();
            return 0;
        }
    }
    
    // Если нет аргументов - интерактивный режим
    if (argc == 1) {
        cout << "Hash Table Manager - Interactive Mode" << endl;
        cout << "Type operations (CREATE, INSERT, REMOVE, CONTAINS, GET, PRINT, SIZE, LOAD, SAVE, METHOD)" << endl;
        cout << "Type 'quit' to exit" << endl;
        
        string input;
        while (true) {
            cout << "> ";
            getline(cin, input);
            
            if (input == "quit" || input == "exit") {
                break;
            }
            
            if (!input.empty()) {
                processQuery(input);
            }
        }
    }
    
    // Очистка памяти
    if (currentTable != nullptr) {
        if (currentMethod == CHAINING) {
            deleteTable(static_cast<HTable*>(currentTable));
        } else {
            deleteOATable(static_cast<OATable*>(currentTable));
        }
    }
    
    return 0;
}
