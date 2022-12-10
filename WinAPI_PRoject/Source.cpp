#include <Windows.h>
#include "MainClassPrototypes.h"
#include "resource.h"


enum MainWindowParams {
	X = 300,
	Y = 300,
	HEIGHT = 500,
	WIDTH = 500
};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{

	WNDCLASS MainClass = CreateWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)), L"MainWindow", MainClassProcedure);

	if (!RegisterClass(&MainClass)) return -1;

	MSG MainMessage = { 0 };

	CreateWindow(MainClass.lpszClassName, L"Okno", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, X, Y, WIDTH, HEIGHT, NULL, NULL, NULL, NULL);

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
			ShowMouseWidgets(hwnd);
			break;
		case ApplySensitivity:

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

	CreateWindowA("button", "Exit", WS_VISIBLE | WS_CHILD, (WIDTH - 80)/ 2, HEIGHT - 100, 80, 25, hwnd, (HMENU)MenuExit, NULL, NULL);
	CreateWindowA("button", "Мышь", WS_VISIBLE | WS_CHILD, (WIDTH - 80)/ 2, 50, 80, 25, hwnd, (HMENU)OnMouseClicked, NULL, NULL);
	textSensitivity = CreateWindowA("static", "Чувствительность", WS_CHILD, 100, 150, 150, 25, hwnd, NULL, NULL, NULL);
	sensitivity = CreateWindowA("edit", "",  WS_CHILD | ES_MULTILINE | WS_VSCROLL, 220, 150, 50, 25, hwnd, NULL, NULL, NULL);
	applySensitivity = CreateWindowA("button", "Применить", WS_CHILD, 275, 150, 95, 25, hwnd, (HMENU)OnMouseClicked, NULL, NULL);

	//CreateWindowA("button", "hide_mouse", WS_VISIBLE | WS_CHILD, (WIDTH - 80) / 2, 200, 80, 25, hwnd, (HMENU)OnTestingClicked, NULL, NULL);

}

void ShowMouseWidgets(HWND hwnd)
{
	ShowWindow(textSensitivity, SW_SHOW);
	ShowWindow(sensitivity, SW_SHOW);
	ShowWindow(applySensitivity, SW_SHOW);
}

void HideMouseWidgets(HWND hwnd)
{
	ShowWindow(textSensitivity, SW_HIDE);
	ShowWindow(sensitivity, SW_HIDE);
	ShowWindow(applySensitivity, SW_HIDE);
}
