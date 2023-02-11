#include <Windows.h>
#include <Winuser.h>
#include <stdio.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include "MainClassPrototypes.h"
#include "resource.h"
#include "sets.h"
#include "string"
#pragma comment(lib, "user32.lib") 


enum MainWindowParams {
	X = 300,
	Y = 300,
	HEIGHT = 500,
	WIDTH = 500
};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{

	WNDCLASS MainClass = CreateWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)),
		L"MainWindow", MainClassProcedure);

	if (!RegisterClass(&MainClass)) return -1;

	MSG MainMessage = { 0 };

	CreateWindow(MainClass.lpszClassName, L"Window", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, X, Y, WIDTH, HEIGHT, NULL,
		NULL, NULL, NULL);

	while (GetMessage(&MainMessage, NULL, NULL, NULL))
	{
		TranslateMessage(&MainMessage);
		DispatchMessageW(&MainMessage);
	}

	return 0;
}

WNDCLASS CreateWindowClass(HBRUSH color, HCURSOR cursor, HINSTANCE hInst, HICON icon, LPCWSTR name, WNDPROC procedure)
{

	WNDCLASS wndclass = { 0 };

	wndclass.hCursor = cursor;
	wndclass.hIcon = icon;
	wndclass.lpszClassName = name;
	wndclass.hInstance = hInst;
	wndclass.hbrBackground = color;
	wndclass.lpfnWndProc = procedure;

	return wndclass;
}

LRESULT CALLBACK MainClassProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	int result = 0;
	switch (msg)
	{
	case WM_COMMAND: 
		switch (wparam)
		{
		case MenuExit:
			result = MessageBox(hwnd, L"Вы действительно хотите выйти?", L"Выход из приложения", MB_YESNO);
			if(result == YES) PostQuitMessage(0);
			break;
		case OnMouseClicked:
			HideSetsWidgets(hwnd);
			ShowMouseWidgets(hwnd);
			break;
		case OnSetsClicked:
			HideMouseWidgets(hwnd);
			ShowSetsWidgets(hwnd);
			break;
		case ApplySensitivity:
			SetMouseSpeed(hwnd);
			break;
		case OnCreatSetClicked:
			CreateSet(hwnd);
			break;
		case OnTestingClicked:
			
			break;
		}
		break;
	case WM_CREATE:
		AddMainWindowMenu(hwnd);
		AddMainWindowWidgets(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

void AddMainWindowMenu(HWND hwnd)
{
	HMENU root_menu = CreateMenu();
	HMENU exit_app = CreateMenu();

	AppendMenuW(exit_app, MF_STRING, MenuExit, L"Выйти");
	AppendMenuW(root_menu, MF_POPUP, (UINT_PTR)exit_app, L"Приложение");

	SetMenu(hwnd, root_menu);
} 

void AddMainWindowWidgets(HWND hwnd)
{

	int n = 2; //кол-во кнопок
	int max_sets_count = 3; //кол-во наборов

	CreateWindowA("button", "Exit", WS_VISIBLE | WS_CHILD, (WIDTH - 80)/ 2, HEIGHT - 100, 80, 25, hwnd, (HMENU)MenuExit, NULL, NULL);
	CreateWindowA("button", "Мышь", WS_VISIBLE | WS_CHILD, WIDTH / 2 - 80 * n / 2, 50, 80, 25, hwnd, (HMENU)OnMouseClicked, NULL, NULL);
	CreateWindowA("button", "Наборы", WS_VISIBLE | WS_CHILD, WIDTH / 2 - 80 * n / 2 + 80, 50, 80, 25, hwnd, (HMENU)OnSetsClicked, NULL, NULL);

	textTesting = CreateWindowA("static", "a", WS_CHILD | WS_VISIBLE, 100, 180, 150, 25, hwnd, NULL, NULL, NULL);
	textSensitivity = CreateWindowA("static", "Чувствительность", WS_CHILD, 100, 150, 150, 25, hwnd, NULL, NULL, NULL);
	sensitivity = CreateWindowA("edit", "",  WS_CHILD | ES_MULTILINE, 220, 150, 50, 25, hwnd, NULL, NULL, NULL);
	applySensitivity = CreateWindowA("button", "Применить", WS_CHILD, 275, 150, 95, 25, hwnd, (HMENU)ApplySensitivity, NULL, NULL);
	mouseVanishing = CreateWindowA("button", "Исчезновение мыши при вводе", WS_CHILD | BS_AUTOCHECKBOX, 10, 180, 235, 25, hwnd, (HMENU)MouseVanishingCheck, NULL, NULL);

	createSet = CreateWindowA("button", "Создать набор", WS_CHILD, WIDTH / 2 - 105, 150, 105, 25, hwnd, (HMENU)OnCreatSetClicked, NULL, NULL);
	setOne = CreateWindowA("static", "Набор 1", WS_CHILD, 100, HEIGHT / 2 - (int)(30 * max_sets_count / 2), 60, 30, hwnd, NULL, NULL, NULL);
	setTwo = CreateWindowA("static", "Набор 2", WS_CHILD, 100, HEIGHT / 2 - (int)(30 * max_sets_count / 2) + 30, 60, 30, hwnd, NULL, NULL, NULL);
	setThree = CreateWindowA("static", "Набор 3", WS_CHILD, 100, HEIGHT / 2 - (int)(30 * max_sets_count / 2) + 60, 60, 30, hwnd, NULL, NULL, NULL);
	setOneDelete = CreateWindowA("button", "Удалить", WS_CHILD, 165, HEIGHT / 2 - (int)(30 * max_sets_count / 2), 95, 30, hwnd, (HMENU)OnSetOneDelete, NULL, NULL);
	setTwoDelete = CreateWindowA("button", "Удалить", WS_CHILD, 165, HEIGHT / 2 - (int)(30 * max_sets_count / 2) + 30, 95, 30, hwnd, (HMENU)OnSetTwoDelete, NULL, NULL);
	setThreeDelete = CreateWindowA("button", "Удалить", WS_CHILD, 165, HEIGHT / 2 - (int)(30 * max_sets_count / 2) + 60, 95, 30, hwnd, (HMENU)OnSetThreeDelete, NULL, NULL);
	//CreateWindowA("button", "hide_mouse", WS_VISIBLE | WS_CHILD, (WIDTH - 80) / 2, 200, 80, 25, hwnd, (HMENU)OnTestingClicked, NULL, NULL);

}

void ShowMouseWidgets(HWND hwnd)
{


	//SystemParametersInfoW(SPI_SETMOUSEVANISH, 0, (PVOID)FALSE, 0);
	//unsigned int speed = 5;
	//SystemParametersInfo(SPI_SETMOUSESPEED,
	//	0,
	//	(void*)speed,
	//	SPIF_SENDCHANGE);

	unsigned int spd;
	BOOL res = SystemParametersInfo(SPI_GETMOUSESPEED,
		0,
		&spd,
		0);
	wchar_t buffer2[256];
	wsprintfW(buffer2, L"%d", spd);
	SetWindowTextW(sensitivity, buffer2);
	ShowWindow(textSensitivity, SW_SHOW);
	ShowWindow(sensitivity, SW_SHOW);
	ShowWindow(applySensitivity, SW_SHOW);
	ShowWindow(mouseVanishing, SW_SHOW);
}

void ShowSetsWidgets(HWND hwnd)
{
	ShowWindow(createSet, SW_SHOW);
	ShowWindow(setOne, SW_SHOW);
	ShowWindow(setTwo, SW_SHOW);
	ShowWindow(setThree, SW_SHOW);
	ShowWindow(setOneDelete, SW_SHOW);
	ShowWindow(setTwoDelete, SW_SHOW);
	ShowWindow(setThreeDelete, SW_SHOW);
}

void HideMouseWidgets(HWND hwnd)
{
	ShowWindow(textSensitivity, SW_HIDE);
	ShowWindow(sensitivity, SW_HIDE);
	ShowWindow(applySensitivity, SW_HIDE);
	ShowWindow(mouseVanishing, SW_HIDE);

}

void SetMouseSpeed(HWND hwnd)
{
	GetWindowTextA(sensitivity, buffer, 256);
	SetWindowTextW(textTesting, (LPCWSTR)buffer);
	int newMouseSpeed = buffer[1] - '0';
	if (!buffer[1]) newMouseSpeed = buffer[0] - '0';
	else
	{
		std::string strBuffer = "  ";
		strBuffer[0] = buffer[0];
		strBuffer[1] = buffer[1];
		newMouseSpeed = std::stoi(strBuffer);
	}
	SystemParametersInfo(SPI_SETMOUSESPEED,
		0,
		(void*)newMouseSpeed,
		SPIF_SENDCHANGE);
}

void HideSetsWidgets(HWND hwnd)
{
	ShowWindow(createSet, SW_HIDE);
	ShowWindow(setOne, SW_HIDE);
	ShowWindow(setTwo, SW_HIDE);
	ShowWindow(setThree, SW_HIDE);
	ShowWindow(setOneDelete, SW_HIDE);
	ShowWindow(setTwoDelete, SW_HIDE);
	ShowWindow(setThreeDelete, SW_HIDE);
}

void CreateSet(HWND hwnd)
{
	Set set;
	set.mouseVanishing = SendMessage(GetDlgItem(hwnd, MouseVanishingCheck), BM_GETCHECK, 0, 0);
	nlohmann::json j{};
	j["mouseVanishing"] = set.mouseVanishing;

	std::ofstream file("set.json");
	file << j;
}
