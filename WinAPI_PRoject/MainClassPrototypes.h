#pragma once
#include <Windows.h>

//��� �����������
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


//�������

//����
HWND textSensitivity;
HWND sensitivity;
HWND applySensitivity;
HWND textTesting;
HWND mouseVanishing;

//������
HWND createSet;


//��������� ������� ��� �������� ����
LRESULT CALLBACK MainClassProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

WNDCLASS CreateWindowClass(HBRUSH color, HCURSOR cursor, HINSTANCE hInst, HICON icon, LPCWSTR name, WNDPROC procedure);

void AddMainWindowMenu(HWND hwnd);
void AddMainWindowWidgets(HWND hwnd);
void ShowMouseWidgets(HWND hwnd);
void ShowSetsWidgets(HWND hwnd);
void HideMouseWidgets(HWND hwnd);
void HideSetsWidgets(HWND hwnd);

//��������� �������
void CreateSet(HWND hwnd);

//������� ��� ������
void testing(HWND hwnd);
