#pragma once
#include <Windows.h>

//��� �����������
#define MenuExit 1
#define OnMenuCLicked 1
#define OnMouseClicked 2
#define OnSetsClicked 3
#define ApplySensitivity 4
#define OnCreatSetClicked 5
#define OnSetOneDelete 6
#define OnSetTwoDelete 7
#define OnSetThreeDelete 8
#define MouseVanishingCheck 15
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
HWND nameSet;
HWND setOne;
HWND setTwo;
HWND setThree;
HWND setOneDelete;
HWND setTwoDelete;
HWND setThreeDelete;


//������� � ���������

char buffer[256];

std::string strBuffer;
std::wstring strToWstrBuffer;

const std::string setsPath = "./sets";


//��������� ������� ��� �������� ����
LRESULT CALLBACK MainClassProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

WNDCLASS CreateWindowClass(HBRUSH color, HCURSOR cursor, HINSTANCE hInst, HICON icon, LPCWSTR name, WNDPROC procedure);

void AddMainWindowMenu(HWND hwnd);
void AddMainWindowWidgets(HWND hwnd);
void ShowMouseWidgets();
void ShowSetsWidgets();
void HideMouseWidgets();
void HideSetsWidgets();
void SetMouseSpeed();

//��������� �������
void CreateSet(HWND hwnd);
void LoadSets();
void DeleteSet(int set);

//������� ��� ������
void testing(HWND hwnd);
