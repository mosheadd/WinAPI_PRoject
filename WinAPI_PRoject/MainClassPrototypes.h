#pragma once
#include <Windows.h>

//��� �����������
#define MenuExit 1
#define OnMenuCLicked 1
#define OnMouseClicked 2
#define YES IDYES
#define NO IDNO

//�������
HWND textSensitivity;
HWND sensitivity;

//��������� ������� ��� �������� ����
LRESULT CALLBACK MainClassProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

WNDCLASS CreateWindowClass(HBRUSH color, HCURSOR cursor, HINSTANCE hInst, HICON icon, LPCWSTR name, WNDPROC procedure);

void AddMainWindowMenu(HWND hwnd);
void AddMainWindowWidgets(HWND hwnd);
void MouseWidgets(HWND hwnd);

//������� ��� ������
void testing(HWND hwnd);
