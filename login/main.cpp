#include <iostream>  // Подключение стандартной библиотеки ввода/вывода
#include <fstream>   // Подключение библиотеки для работы с файлами
#include <vector>    // Подключение библиотеки для использования контейнера vector

using namespace std; // Использование стандартного пространства имен

// Глобальные переменные для хранения данных пользователей и количества записей
int colRegistr = 10; // Максимальное количество записей для обработки

// Векторы для хранения данных о пользователях
vector<string> logins(colRegistr, ""); // Вектор для хранения логинов
vector<string> passwords(colRegistr, ""); // Вектор для хранения паролей
vector<string> emails(colRegistr, ""); // Вектор для хранения email-адресов

ifstream ifFile("/Users/admin/Desktop/text.txt"); // Открытие файла для чтения данных

bool duplicate; // Флаг для проверки дубликата логина

// Функция для загрузки данных из файла в векторы
void dataOutput() {
    string login, password, email; // Переменные для хранения считываемых данных
    int index = 0; // Индекс для последовательного заполнения векторов
    
    // Чтение данных из файла и сохранение их в векторах
    while (ifFile >> login >> password >> email && index < colRegistr) {
        logins[index] = login;
        passwords[index] = password;
        emails[index] = email;
        index++;
    }
    ifFile.close(); // Закрытие файла после чтения
}

// Глобальные переменные для хранения ввода пользователя
string loginInput, passwordInput, emailInput;

// Функция для запроса и сохранения пароля и email от пользователя
void passwordAndEmail() {
    cout << "New Password: "; // Приглашение ввести пароль
    cin >> passwordInput;
    cout << "New Email: "; // Приглашение ввести email
    cin >> emailInput;
}

// Функция для проверки наличия дубликата логина
void checkDuplicateLogin() {
    duplicate = false; // Инициализация переменной, указывающей на отсутствие дубликата
    while (!duplicate) { // Пока логин не будет уникальным
        cout << "New Login: "; // Запрос логина
        cin >> loginInput; // Ввод логина
        
        duplicate = true; // Предполагаем, что логин уникален
        for (const auto& check : logins) { // Перебор всех сохраненных логинов
            if (loginInput == check) { // Если найден дубликат
                cout << "Error: Duplicate login\n"; // Сообщение о наличии дубликата
                duplicate = false; // Установка флага дубликата в false
                break;
            }
        }
    }
}

// Функция для добавления новой уникальной записи в файл
int addNewUnique(const string& loginInput, const string& passwordInput, const string& emailInput) {
    ofstream ofFile("/Users/admin/Desktop/text.txt", ios::app); // Открытие файла для добавления данных
    if (!ofFile) { // Проверка успешности открытия файла
        cout << "Error opening file\n"; // Сообщение об ошибке открытия файла
        return 1; // Возврат ошибки
    }
    ofFile << "\n" << loginInput << "\t" << passwordInput << "\t" << emailInput; // Запись новых данных
    ofFile.close(); // Закрытие файла
    return 0; // Возврат 0 при успешной записи
}

// Функция для восстановления аккаунта по email
void restoreAccount() {
    cout << "Enter your email to restore account: "; // Приглашение ввести email для восстановления аккаунта
    cin >> emailInput;
    bool found = false; // Флаг для проверки наличия email в базе данных
    
    // Поиск email в векторе emails
    for (int i = 0; i < emails.size(); i++) {
        if (emails[i] == emailInput) { // Если email найден
            cout << "Account found. Enter new password: "; // Сообщение об успешном нахождении аккаунта и запрос нового пароля
            cin >> passwordInput;
            passwords[i] = passwordInput; // Обновление пароля в векторе
            found = true; // Установка флага found в true
            break;
        }
    }

    if (!found) { // Если email не найден
        cout << "Email not found.\n"; // Сообщение о ненайденном email
    } else { // Если email найден
        ofstream ofFile("/Users/admin/Desktop/text.txt"); // Открытие файла для перезаписи данных
        if (!ofFile) { // Проверка успешности открытия файла
            cout << "Failed to open file for updating.\n"; // Сообщение об ошибке открытия файла
            return; // Завершение функции при ошибке открытия файла
        }
        // Запись обновленных данных в файл
        for (int i = 0; i < colRegistr; i++) {
            ofFile << logins[i] << "\t" << passwords[i] << "\t" << emails[i] << endl;
        }
        ofFile.close(); // Закрытие файла после записи
        cout << "Password updated successfully.\n"; // Сообщение об успешном обновлении пароля
    }
}

int main() {
    // Проверка успешности открытия файла и чтение данных
    if (ifFile) {
        dataOutput(); // Чтение данных из файла
    } else {
        cout << "Error file YOK\n"; // Сообщение об ошибке, если файл не открыт
        return 0; // Прекращение выполнения программы
    }
    
    int click; // Переменная для выбора действия пользователя
    cout << "For new account press - 1, to restore your account press - 2: "; // Приглашение для выбора действия
    cin >> click;
    
    if (click == 1) { // Если выбран вариант создания нового аккаунта
        duplicate = false; // Инициализация флага дубликата как false
        checkDuplicateLogin(); // Проверка и запрос логина
        passwordAndEmail(); // Запрос пароля и email
        
        if (duplicate) { // Если логин уникален
            addNewUnique(loginInput, passwordInput, emailInput); // Добавление новой записи
        }
    } else { // Если выбран вариант восстановления аккаунта
        restoreAccount(); // Восстановление аккаунта
    }
    
    return 0; // Завершение программы
}
