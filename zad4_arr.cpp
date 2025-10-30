#include "arr.h"
#include <iostream>
#include <string>
using namespace std;

void createDPTable(Array& dp, int rows, int cols) {
    ACREATE(dp, rows * cols);
    for (int i = 0; i < rows * cols; i++) {
        AAPPEND(dp, "false");
    }
}

void setDPValue(Array& dp, int cols, int i, int j, const string& value) {
    int index = i * cols + j;
    ASET(dp, index, value);
}

string getDPValue(const Array& dp, int cols, int i, int j) {
    int index = i * cols + j;
    return AGET(dp, index);
}

bool matchPattern(const string& str, const string& pattern) {
    int n = str.length();
    int m = pattern.length();
    

    Array dp;
    createDPTable(dp, n + 1, m + 1);
    
    // Базовый случай: пустая строка соответствует пустому шаблону
    setDPValue(dp, m + 1, 0, 0, "true");
    
    // Обработка звездочек в начале шаблона
    for (int j = 1; j <= m; j++) {
        if (pattern[j - 1] == '*') {
            string prevValue = getDPValue(dp, m + 1, 0, j - 1);
            setDPValue(dp, m + 1, 0, j, prevValue);
        } else {
            break;
        }
    }
        for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (pattern[j - 1] == '?' || pattern[j - 1] == str[i - 1]) {
// совпадение символов или ?
                string diagValue = getDPValue(dp, m + 1, i - 1, j - 1);
                setDPValue(dp, m + 1, i, j, diagValue);
            }
            else if (pattern[j - 1] == '*') {

                string option1 = getDPValue(dp, m + 1, i, j - 1);  // Игнорируем звездочку
                string option2 = getDPValue(dp, m + 1, i - 1, j);  // Используем звездочку для символа
                
                if (option1 == "true" || option2 == "true") {
                    setDPValue(dp, m + 1, i, j, "true");
                } else {
                    setDPValue(dp, m + 1, i, j, "false");
                }
            }
            else {
                // Символы не совпадают
                setDPValue(dp, m + 1, i, j, "false");
            }
        }
    }
    
    // Результат находится в правом нижнем углу таблицы
    string resultStr = getDPValue(dp, m + 1, n, m);
    return resultStr == "true";
}

int main() {
    cout << "=== ПРОВЕРКА СООТВЕТСТВИЯ СТРОКИ ШАБЛОНУ ===" << endl;
    
    // Тест 1 из задания
    string str1 = "meow@stud.nstu.ru";
    string pattern1 = "*@stud.nstu.ru";
    bool result1 = matchPattern(str1, pattern1);
    cout << "Тест 1:" << endl;
    cout << "Строка: " << str1 << endl;
    cout << "Шаблон: " << pattern1 << endl;
    cout << "Результат: " << (result1 ? "СОВПАДАЕТ" : "НЕ СОВПАДАЕТ") << endl;
    cout << endl;
    
    // Тест 2 из задания
    string str2 = "hello";
    string pattern2 = "h?lo";
    bool result2 = matchPattern(str2, pattern2);
    cout << "Тест 2:" << endl;
    cout << "Строка: " << str2 << endl;
    cout << "Шаблон: " << pattern2 << endl;
    cout << "Результат: " << (result2 ? "СОВПАДАЕТ" : "НЕ СОВПАДАЕТ") << endl;
    cout << endl;
    
    // Дополнительные тесты
    string str3 = "abc";
    string pattern3 = "a?c";
    bool result3 = matchPattern(str3, pattern3);
    cout << "Тест 3:" << endl;
    cout << "Строка: " << str3 << endl;
    cout << "Шаблон: " << pattern3 << endl;
    cout << "Результат: " << (result3 ? "СОВПАДАЕТ" : "НЕ СОВПАДАЕТ") << endl;
    cout << endl;
    
    // Интерактивный режим
    string inputStr, inputPattern;
    cout << "=== ИНТЕРАКТИВНЫЙ РЕЖИМ ===" << endl;
    cout << "Введите строку: ";
    getline(cin, inputStr);
    cout << "Введите шаблон: ";
    getline(cin, inputPattern);
    
    bool result = matchPattern(inputStr, inputPattern);
    cout << "Результат: " << (result ? "СОВПАДАЕТ" : "НЕ СОВПАДАЕТ") << endl;
    
    return 0;
}
