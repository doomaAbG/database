#include <iostream>
#include <string>
#include <libpq-fe.h> // Библиотека для PostgreSQL
#include <algorithm>  // Для all_of
#include <stdexcept>  // Для try/catch с stoi

using namespace std;

// Проверка соединения с базой
void checkConn(PGconn* conn) {
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        exit(1);
    }
}

// Выполнение SQL-запроса
PGresult* executeQuery(PGconn* conn, const string& query) {
    PGresult* res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "Query failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return nullptr;
    }
    return res;
}

// Добавление студента
void addStudent(PGconn* conn) {
    string name, major;
    string ageStr; // Используем строку для ввода возраста, чтобы проверить её вручную
    int age;

    cout << "Enter student name: ";
    getline(cin, name);
    cout << "Enter student age (numeric value only, e.g., 17): ";
    getline(cin, ageStr); // Читаем возраст как строку

    // Проверка, что строка не пустая и содержит только числа
    while (ageStr.empty() || !all_of(ageStr.begin(), ageStr.end(), isdigit)) {
        cout << "Invalid age. Please enter a valid numeric age (e.g., 17): ";
        getline(cin, ageStr);
    }

    // Преобразуем строку в int
    try {
        age = stoi(ageStr);
        if (age <= 0) {
            cout << "Age must be positive. Please enter a valid age: ";
            getline(cin, ageStr);
            while (!all_of(ageStr.begin(), ageStr.end(), isdigit) || stoi(ageStr) <= 0) {
                cout << "Invalid age. Please enter a positive numeric age (e.g., 17): ";
                getline(cin, ageStr);
            }
            age = stoi(ageStr);
        }
    } catch (const exception& e) {
        cerr << "Error converting age: " << e.what() << endl;
        cout << "Please enter a valid numeric age (e.g., 17): ";
        getline(cin, ageStr);
        while (ageStr.empty() || !all_of(ageStr.begin(), ageStr.end(), isdigit)) {
            cout << "Invalid age. Please enter a valid numeric age (e.g., 17): ";
            getline(cin, ageStr);
        }
        age = stoi(ageStr);
    }

    cout << "Enter student major: ";
    getline(cin, major);

    // Сохраняем to_string(age) в отдельную переменную, чтобы буфер остался действительным
    string ageStrParam = to_string(age);

    string query = "INSERT INTO students (name, age, major) VALUES ($1, $2, $3)";
    const char* paramValues[3] = {name.c_str(), ageStrParam.c_str(), major.c_str()};
    PGresult* res = PQexecParams(conn, query.c_str(), 3, nullptr, paramValues, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Insert failed: " << PQerrorMessage(conn) << endl;
    } else {
        cout << "Student added successfully! Name: " << name << ", Age: " << age << ", Major: " << major << endl;
    }
    PQclear(res);
}

// Удаление студента
void deleteStudent(PGconn* conn) {
    string name;
    cout << "Enter student name to delete: ";
    getline(cin, name);

    string query = "DELETE FROM students WHERE name = $1";
    const char* paramValues[1] = {name.c_str()};
    PGresult* res = PQexecParams(conn, query.c_str(), 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Delete failed: " << PQerrorMessage(conn) << endl;
    } else if (PQcmdTuples(res)[0] == '0') {
        cout << "No student found with name: " << name << endl;
    } else {
        cout << "Student deleted successfully! Name: " << name << endl;
    }
    PQclear(res);
}

// Поиск студента
void searchStudent(PGconn* conn) {
    string name;
    cout << "Enter student name to search: ";
    getline(cin, name);

    string query = "SELECT * FROM students WHERE name = $1";
    const char* paramValues[1] = {name.c_str()};
    PGresult* res = PQexecParams(conn, query.c_str(), 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        if (rows == 0) {
            cout << "No student found with name: " << name << endl;
        } else {
            cout << "Found " << rows << " student(s) with name: " << name << endl;
            for (int i = 0; i < rows; i++) {
                cout << "ID: " << PQgetvalue(res, i, 0) << ", Name: " << PQgetvalue(res, i, 1)
                     << ", Age: " << PQgetvalue(res, i, 2) << ", Major: " << PQgetvalue(res, i, 3) << "\n";
            }
        }
    } else {
        cerr << "Search failed: " << PQerrorMessage(conn) << endl;
    }
    PQclear(res);
}

// Просмотр всех студентов
void listAllStudents(PGconn* conn) {
    string query = "SELECT * FROM students";
    PGresult* res = executeQuery(conn, query);
    if (res) {
        int rows = PQntuples(res);
        cout << "Total students: " << rows << "\n";
        if (rows == 0) {
            cout << "No students found.\n";
        } else {
            for (int i = 0; i < rows; i++) {
                cout << "ID: " << PQgetvalue(res, i, 0) << ", Name: " << PQgetvalue(res, i, 1)
                     << ", Age: " << PQgetvalue(res, i, 2) << ", Major: " << PQgetvalue(res, i, 3) << "\n";
            }
        }
        PQclear(res);
    }
}

int main() {
    // Подключение к базе
    PGconn* conn = PQconnectdb("host=localhost port=5432 dbname=studentdb user=postgres password=0000");
    checkConn(conn);

    int choice;
    do {
        cout << "\nStudent Management System\n";
        cout << "1. Add student\n";
        cout << "2. Delete student\n";
        cout << "3. Search student\n";
        cout << "4. List all students\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Очистка буфера

        switch (choice) {
            case 1:
                addStudent(conn);
                break;
            case 2:
                deleteStudent(conn);
                break;
            case 3:
                searchStudent(conn);
                break;
            case 4:
                listAllStudents(conn);
                break;
            case 5:
                cout << "Exiting Student Management System...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    // Закрытие соединения
    PQfinish(conn);
    return 0;
}