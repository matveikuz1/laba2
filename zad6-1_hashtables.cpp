#include "hash-t.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;

int countAllSubordinates(const string& manager, HTable* pairsTable) {
    int count = 0;
    
    // Ищем прямых подчиненных
    for (int j = 0; j < pairsTable->capacity; j++) {
        HNode* pairNode = pairsTable->buckets[j];
        while (pairNode != nullptr) {
            string key = pairNode->key;
            size_t pos = key.find(':');
            if (pos != string::npos) {
                string mgr = key.substr(0, pos);
                string emp = key.substr(pos + 1);
                
                if (mgr == manager && emp != manager) {
                    count++; 
                    count += countAllSubordinates(emp, pairsTable);
                }
            }
            pairNode = pairNode->next;
        }
    }
    
    return count;
}

HTable* countSubordinates(const vector<pair<string, string>>& pairs) {
    HTable* ht = createTable(20);
    
    // Используем хэш-таблицу для хранения пар в формате "менеджер:сотрудник"
    HTable* pairsTable = createTable(20);
    
    // Сохраняем все пары в формате "менеджер:сотрудник"
    for (const auto& pair : pairs) {
        string key = pair.second + ":" + pair.first; 
        insert(pairsTable, key, 1); 
    }
    
    // Собираем всех уникальных сотрудников
    HTable* allEmployees = createTable(20);
    for (const auto& pair : pairs) {
        insert(allEmployees, pair.first, 0);
        insert(allEmployees, pair.second, 0);
    }
    
    // Для каждого сотрудника считаем количество всех подчиненных (рекурсивно)
    for (int i = 0; i < allEmployees->capacity; i++) {
        HNode* curr = allEmployees->buckets[i];
        while (curr != nullptr) {
            string manager = curr->key;
            int subordinateCount = countAllSubordinates(manager, pairsTable);
            insert(ht, manager, subordinateCount);
            curr = curr->next;
        }
    }
    
    deleteTable(pairsTable);
    deleteTable(allEmployees);
    return ht;
}

// Функция для вывода результатов
void printResults(HTable* ht) {
    vector<string> employees;
    
    // Собираем все ключи
    for (int i = 0; i < ht->capacity; i++) {
        HNode* curr = ht->buckets[i];
        while (curr != nullptr) {
            employees.push_back(curr->key);
            curr = curr->next;
        }
    }
    
    cout << "Результат: ";
    for (size_t i = 0; i < employees.size(); i++) {
        const string& emp = employees[i];
        int index = hashFunction(emp, ht->capacity);
        HNode* curr = ht->buckets[index];
        while (curr != nullptr) {
            if (curr->key == emp) {
                cout << emp << "-" << curr->data;
                if (i != employees.size() - 1) {
                    cout << ", ";
                }
                break;
            }
            curr = curr->next;
        }
    }
    cout << endl;
}

// Интерактивная версия с командами
void interactiveMode() {
    HTable* pairsTable = createTable(20);
    HTable* employeesTable = createTable(20); // для хранения всех сотрудников
    
    cout << "=== Интерактивный режим ===" << endl;
    cout << "Команды:" << endl;
    cout << "ADD <сотрудник> <менеджер>" << endl;
    cout << "PRINT" << endl;
    cout << "EXIT" << endl;
    
    string cmd;
    while (true) {
        cout << "> ";
        getline(cin, cmd);
        
        if (cmd == "EXIT") break;
        
        vector<string> params;
        stringstream ss(cmd);
        string token;
        
        while (ss >> token) {
            params.push_back(token);
        }
        
        if (params.empty()) continue;
        
        if (params[0] == "ADD") {
            if (params.size() < 3) {
                cout << "Ошибка: ADD требует сотрудника и менеджера" << endl;
                continue;
            }
            string emp = params[1];
            string mgr = params[2];
            
            // MGR : EMP
            string key = mgr + ":" + emp;
            if (insert(pairsTable, key, 1)) {
                // Сохраняем сотрудников
                insert(employeesTable, emp, 0);
                insert(employeesTable, mgr, 0);
                cout << "Добавлено: " << emp << " -> " << mgr << " (ключ: " << key << ")" << endl;
            } else {
                cout << "Ошибка: не удалось добавить пару" << endl;
            }
        }
        else if (params[0] == "PRINT") {
            // Создаем таблицу результатов
            HTable* resultTable = createTable(20);
            
            // Для каждого сотрудника считаем всех подчиненных (рекурсивно)
            for (int i = 0; i < employeesTable->capacity; i++) {
                HNode* curr = employeesTable->buckets[i];
                while (curr != nullptr) {
                    string manager = curr->key;
                    int subordinateCount = countAllSubordinates(manager, pairsTable);
                    insert(resultTable, manager, subordinateCount);
                    curr = curr->next;
                }
            }
            
            printResults(resultTable);
            deleteTable(resultTable);
        }
        else {
            cout << "Неизвестная команда" << endl;
        }
    }
    
    deleteTable(pairsTable);
    deleteTable(employeesTable);
}

void customTest() {
    cout << "=== Подсчет количества подчиненных ===" << endl;
    cout << "Выберите режим:" << endl;
    cout << "1 - Ввод данных заранее" << endl;
    cout << "2 - Интерактивный ввод" << endl;
    cout << "Ваш выбор: ";
    
    int choice;
    cin >> choice;
    cin.ignore(); // очищаем буфер
    
    if (choice == 1) {
        cout << "Введите количество пар (сотрудник-менеджер): ";
        int n;
        cin >> n;
        
        vector<pair<string, string>> pairs;
        
        cout << "Введите пары в формате: сотрудник менеджер" << endl;
        for (int i = 0; i < n; i++) {
            string emp, mgr;
            cout << "Пара " << i + 1 << ": ";
            cin >> emp >> mgr;
            pairs.push_back({emp, mgr});
        }
        
        cout << "\nВведенные данные: ";
        for (const auto& pair : pairs) {
            cout << "{" << pair.first << ", " << pair.second << "} ";
        }
        cout << endl;
        
        HTable* result = countSubordinates(pairs);
        printResults(result);
        deleteTable(result);
    }
    else if (choice == 2) {
        interactiveMode();
    }
}

int main() {
    customTest();
    
    cout << "\nНажмите Enter для выхода...";
    cin.ignore();
    cin.get();
    
    return 0;
}
