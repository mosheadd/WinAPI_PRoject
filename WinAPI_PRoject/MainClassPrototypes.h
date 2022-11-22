#pragma once
#include <Windows.h>

//Все определения
#define MenuExit 1
#define OnMenuCLicked 1
#define OnMouseClicked 2
#define YES IDYES
#define NO IDNO

//Виджеты
HWND textSensitivity;
HWND sensitivity;

//Прототипы функций для главного окна
LRESULT CALLBACK MainClassProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

WNDCLASS CreateWindowClass(HBRUSH color, HCURSOR cursor, HINSTANCE hInst, HICON icon, LPCWSTR name, WNDPROC procedure);

void AddMainWindowMenu(HWND hwnd);
void AddMainWindowWidgets(HWND hwnd);
void MouseWidgets(HWND hwnd);

//Функция для тестов
void testing(HWND hwnd);
