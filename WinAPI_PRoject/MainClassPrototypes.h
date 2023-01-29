#pragma once
#include <Windows.h>

//Все определения
#define MenuExit 1
#define OnMenuCLicked 1
#define OnMouseClicked 2
#define OnSetsClicked 3
#define ApplySensitivity 4
#define OnCreatSetClicked 5
#define MouseVanishingCheck 10
#define OnTestingClicked 999
#define YES IDYES
#define NO IDNO
#define IDC_STATIC_TEST 1000


//Виджеты

//Мышь
HWND textSensitivity;
HWND sensitivity;
HWND applySensitivity;
HWND textTesting;
HWND mouseVanishing;

//Наборы
HWND createSet;


//Прототипы функций для главного окна
LRESULT CALLBACK MainClassProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

WNDCLASS CreateWindowClass(HBRUSH color, HCURSOR cursor, HINSTANCE hInst, HICON icon, LPCWSTR name, WNDPROC procedure);

void AddMainWindowMenu(HWND hwnd);
void AddMainWindowWidgets(HWND hwnd);
void ShowMouseWidgets(HWND hwnd);
void ShowSetsWidgets(HWND hwnd);
void HideMouseWidgets(HWND hwnd);
void HideSetsWidgets(HWND hwnd);

//Остальные функции
void CreateSet(HWND hwnd);

//Функция для тестов
void testing(HWND hwnd);
