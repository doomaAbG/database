# Student Management System

## Описание
Student Management System — это консольное приложение на C++, разработанное для управления информацией о студентах. Приложение позволяет добавлять, удалять, искать и просматривать записи о студентах (имя, возраст, специальность) с использованием реляционной базы данных PostgreSQL. Проект демонстрирует работу с C++, PostgreSQL через библиотеку `libpq`, а также интеграцию с Visual Studio Code, MSVC и vcpkg для управления зависимостями.

Этот проект был создан для обучения и практики работы с базами данных, SQL и C++ в Windows-окружении.

## Требования
Чтобы запустить проект, нужно установить следующие зависимости:

- **Операционная система**: Windows (поддерживается версия Windows 10/11).
- **Visual Studio 2022 Community** (с установленным компонентом "Desktop development with C++", включая MSVC v143 для x64/x86).
- **Git** (для работы с репозиторием и загрузки проекта).
- **PostgreSQL** (версия 16 или выше, для базы данных студентов).
- **vcpkg** (для управления зависимостями, включая библиотеку `libpq` для работы с PostgreSQL).
- **Visual Studio Code** (опционально, для разработки и отладки).

---

## Установка и настройка

### 1. Установи необходимые инструменты
- **Visual Studio 2022**:
  - Скачай и установи с [visualstudio.microsoft.com](https://visualstudio.microsoft.com/downloads/).
  - Выбери "Desktop development with C++" при установке.
- **PostgreSQL**:
  - Скачай и установи с [postgresql.org](https://www.postgresql.org/download/windows/).
  - Установи пароль для пользователя `postgres` (например, `0000`) и создай базу данных `studentdb`.
- **Git**:
  - Установи с [git-scm.com](https://git-scm.com/downloads/).
- **vcpkg**:
  - Клонируй vcpkg в удобное место (например, `C:\Users\beenk\vcpkg`):
    ```bash
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    .\bootstrap-vcpkg.bat
    .\vcpkg.exe integrate install
