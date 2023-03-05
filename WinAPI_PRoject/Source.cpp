#include <Windows.h>
#include <Winuser.h>
#include <stdio.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "MainClassPrototypes.h"
#include "resource.h"
#include "sets.h"
#include "string"
#pragma comment(lib, "user32.lib") 


enum MainWindowParams {
	X = 630,
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
	int newMouseSpeed;
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
			HideSetsWidgets();
			ShowMouseWidgets();
			break;
		case OnSetsClicked:
			HideMouseWidgets();
			ShowSetsWidgets();
			LoadSets();
			break;
		case ApplySensitivity:
			GetWindowTextA(sensitivity, buffer, 256);
			if (!buffer[1])
			{
				newMouseSpeed = buffer[0] - '0';
				try
				{
					if (!std::isdigit(buffer[0])) throw 1;
					SetMouseSpeed(newMouseSpeed);
				}
				catch (int error)
				{
					switch (error)
					{
					case 1:
						MessageBox(NULL, L"Значение должно быть натуральным числом.", L"Ошибка", MB_ICONERROR | MB_OK);
						break;
					}
				}
			}
			else
			{
				strBuffer = "  ";
				strBuffer[0] = buffer[0];
				strBuffer[1] = buffer[1];
				try
				{
					for (auto c : strBuffer) if (!std::isdigit(c)) throw 1;
					if (std::stoi(strBuffer) > 20 || std::stoi(strBuffer) <= 0) throw 2;
					newMouseSpeed = std::stoi(strBuffer);
					SetMouseSpeed(newMouseSpeed);
				}
				catch (int error)
				{
					switch (error)
					{
					case 1:
						MessageBox(NULL, L"Значение должно быть натуральным числом.", L"Ошибка", MB_ICONERROR | MB_OK);
						break;
					case 2:
						MessageBox(NULL, L"Значение должно быть в пределах от 1 до 20.", L"Ошибка", MB_ICONERROR | MB_OK);
						break;
					}
				}
			}
			strBuffer = "";
			break;
		case OnCreatSetClicked:
			CreateSet(hwnd);
			break;
		case OnSetOneDelete:
			DeleteSet(1);
			break;
		case OnSetTwoDelete:
			DeleteSet(2);
			break;
		case OnSetThreeDelete:
			DeleteSet(3);
			break;
		case OnSetOneApply:
			ApplySet(1);
			break;
		case OnSetTwoApply:
			ApplySet(2);
			break;
		case OnSetThreeApply:
			ApplySet(3);
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

	textTesting = CreateWindowA("static", "a", WS_CHILD, 100, 180, 150, 25, hwnd, NULL, NULL, NULL);
	textSensitivity = CreateWindowA("static", "Чувствительность", WS_CHILD, 100, 150, 150, 25, hwnd, NULL, NULL, NULL);
	sensitivity = CreateWindowA("edit", "",  WS_CHILD | ES_MULTILINE, 220, 150, 50, 25, hwnd, NULL, NULL, NULL);
	applySensitivity = CreateWindowA("button", "Применить", WS_CHILD, 275, 150, 95, 25, hwnd, (HMENU)ApplySensitivity, NULL, NULL);
	mouseVanishing = CreateWindowA("button", "Исчезновение мыши при вводе", WS_CHILD | BS_AUTOCHECKBOX, 30, 180, 235, 25, hwnd, (HMENU)MouseVanishingCheck, NULL, NULL);

	createSet = CreateWindowA("button", "Создать набор", WS_CHILD, WIDTH / 2 - 105, 150, 105, 25, hwnd, (HMENU)OnCreatSetClicked, NULL, NULL);
	nameSet = CreateWindowA("edit", "", WS_CHILD | ES_MULTILINE, WIDTH / 2 + 5, 150, 150, 25, hwnd, NULL, NULL, NULL);
	setOne = CreateWindowA("static", "", WS_CHILD, 100, HEIGHT / 2 - (int)(30 * max_sets_count / 2), 100, 30, hwnd, NULL, NULL, NULL);
	setTwo = CreateWindowA("static", "", WS_CHILD, 100, HEIGHT / 2 - (int)(30 * max_sets_count / 2) + 30, 100, 30, hwnd, NULL, NULL, NULL);
	setThree = CreateWindowA("static", "", WS_CHILD, 100, HEIGHT / 2 - (int)(30 * max_sets_count / 2) + 60, 100, 30, hwnd, NULL, NULL, NULL);
	setOneApply = CreateWindowA("button", "Применить", WS_CHILD, 200, HEIGHT / 2 - (int)(30 * max_sets_count / 2), 95, 30, hwnd, (HMENU)OnSetOneApply, NULL, NULL);
	setTwoApply = CreateWindowA("button", "Применить", WS_CHILD, 200, HEIGHT / 2 - (int)(30 * max_sets_count / 2) + 30, 95, 30, hwnd, (HMENU)OnSetTwoApply, NULL, NULL);
	setThreeApply = CreateWindowA("button", "Применить", WS_CHILD, 200, HEIGHT / 2 - (int)(30 * max_sets_count / 2) + 60, 95, 30, hwnd, (HMENU)OnSetThreeApply, NULL, NULL);
	setOneDelete = CreateWindowA("button", "Удалить", WS_CHILD, 300, HEIGHT / 2 - (int)(30 * max_sets_count / 2), 95, 30, hwnd, (HMENU)OnSetOneDelete, NULL, NULL);
	setTwoDelete = CreateWindowA("button", "Удалить", WS_CHILD, 300, HEIGHT / 2 - (int)(30 * max_sets_count / 2) + 30, 95, 30, hwnd, (HMENU)OnSetTwoDelete, NULL, NULL);
	setThreeDelete = CreateWindowA("button", "Удалить", WS_CHILD, 300, HEIGHT / 2 - (int)(30 * max_sets_count / 2) + 60, 95, 30, hwnd, (HMENU)OnSetThreeDelete, NULL, NULL);
	//CreateWindowA("button", "hide_mouse", WS_VISIBLE | WS_CHILD, (WIDTH - 80) / 2, 200, 80, 25, hwnd, (HMENU)OnTestingClicked, NULL, NULL);

}

void ShowMouseWidgets()
{
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

void ShowSetsWidgets()
{
	ShowWindow(createSet, SW_SHOW);
	ShowWindow(nameSet, SW_SHOW);
}

void HideMouseWidgets()
{
	ShowWindow(textSensitivity, SW_HIDE);
	ShowWindow(sensitivity, SW_HIDE);
	ShowWindow(applySensitivity, SW_HIDE);
	ShowWindow(mouseVanishing, SW_HIDE);
}

void SetMouseSpeed(int speed)
{
	SystemParametersInfo(SPI_SETMOUSESPEED,
		0,
		(void*)speed,
		SPIF_SENDCHANGE);
}

void HideSetsWidgets()
{
	ShowWindow(createSet, SW_HIDE);
	ShowWindow(nameSet, SW_HIDE);
	ShowWindow(setOne, SW_HIDE);
	ShowWindow(setTwo, SW_HIDE);
	ShowWindow(setThree, SW_HIDE);
	ShowWindow(setOneApply, SW_HIDE);
	ShowWindow(setTwoApply, SW_HIDE);
	ShowWindow(setThreeApply, SW_HIDE);
	ShowWindow(setOneDelete, SW_HIDE);
	ShowWindow(setTwoDelete, SW_HIDE);
	ShowWindow(setThreeDelete, SW_HIDE);
}

void CreateSet(HWND hwnd)
{
	Set set;
	set.mouseVanishing = SendMessage(GetDlgItem(hwnd, MouseVanishingCheck), BM_GETCHECK, 0, 0);
	GetWindowTextA(sensitivity, buffer, 256);
	if (!buffer[1]) set.mouseSpeed = buffer[0] - '0';
	else
	{
		strBuffer = "  ";
		strBuffer[0] = buffer[0];
		strBuffer[1] = buffer[1];
		set.mouseSpeed = std::stoi(strBuffer);
	}
	nlohmann::json j{};
	j["mouseVanishing"] = set.mouseVanishing;
	j["mouseSpeed"] = set.mouseSpeed;

	GetWindowTextA(nameSet, buffer, 256);
	int bufferI = 0;
	strBuffer = "";
	while (buffer[bufferI])
	{
		strBuffer += buffer[bufferI];
		bufferI++;
	}

	std::ofstream file("./sets/" + strBuffer + ".json");
	file << j;
	file.close();
	LoadSets();
}

void ApplySet(int set)
{
	std::string path = "./sets/";
	int bufferI = 0;
	switch (set)
	{
	case 1:
		GetWindowTextA(setOne, buffer, 256);
		break;
	case 2:
		GetWindowTextA(setTwo, buffer, 256);
		break;
	case 3:
		GetWindowTextA(setThree, buffer, 256);
		break;
	}
	while (buffer[bufferI])
	{
		path += buffer[bufferI];
		bufferI++;
	}
	path += ".json";
	strToWstrBuffer = std::wstring(path.begin(), path.end());
	SetWindowTextW(textTesting, strToWstrBuffer.c_str());
	strToWstrBuffer = L"";
	std::ifstream ifs(path);
	nlohmann::json jsondata = nlohmann::json::parse(ifs);
	SetMouseSpeed(jsondata["mouseSpeed"]);
}

void LoadSets()
{
	std::vector<std::string> fileNames;
	for (const auto& entry : std::filesystem::directory_iterator(setsPath)) fileNames.push_back(entry.path().generic_string());
	if (fileNames.size() >= 1)
	{
		strToWstrBuffer = std::wstring(fileNames[0].begin() + 7, fileNames[0].end() - 5);
		SetWindowTextW(setOne, strToWstrBuffer.c_str());
		ShowWindow(setOne, SW_SHOW);
		ShowWindow(setOneApply, SW_SHOW);
		ShowWindow(setOneDelete, SW_SHOW);
	}
	if (fileNames.size() >= 2)
	{
		strToWstrBuffer = std::wstring(fileNames[1].begin() + 7, fileNames[1].end() - 5);
		SetWindowTextW(setTwo, strToWstrBuffer.c_str());
		ShowWindow(setTwo, SW_SHOW);
		ShowWindow(setTwoApply, SW_SHOW);
		ShowWindow(setTwoDelete, SW_SHOW);
	}
	if (fileNames.size() == 3)
	{
		strToWstrBuffer = std::wstring(fileNames[2].begin() + 7, fileNames[2].end() - 5);
		SetWindowTextW(setThree, strToWstrBuffer.c_str());
		ShowWindow(setThree, SW_SHOW);
		ShowWindow(setThreeApply, SW_SHOW);
		ShowWindow(setThreeDelete, SW_SHOW);
	}
	strToWstrBuffer = L"";

}

void DeleteSet(int set)
{
	int bufferI = 0;
	switch (set)
	{
	case 1:
		SetWindowTextW(textTesting, L"1");
		GetWindowTextA(setOne, buffer, 256);
		break;
	case 2:
		SetWindowTextW(textTesting, L"2");
		GetWindowTextA(setTwo, buffer, 256);
		break;
	case 3:
		SetWindowTextW(textTesting, L"3");
		GetWindowTextA(setThree, buffer, 256);
		break;
	}
	strBuffer = "";
	while (buffer[bufferI])
	{
		strBuffer += buffer[bufferI];
		bufferI++;
	}
	std::filesystem::remove("./sets/" + strBuffer + ".json");
	strBuffer = "";
	HideSetsWidgets();
	ShowSetsWidgets();
	LoadSets();
}
