#include <Windows.h>
#include "MainClassPrototypes.h"


enum MainWindowParams {
	X = 300,
	Y = 300,
	HEIGHT = 500,
	WIDTH = 500
};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{

	WNDCLASS MainClass = CreateWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, IDI_QUESTION), L"MainWindow", MainClassProcedure);

	if (!RegisterClass(&MainClass)) return -1;

	MSG MainMessage = { 0 };

	CreateWindow(MainClass.lpszClassName, L"Okno", WS_OVERLAPPEDWINDOW | WS_VISIBLE, X, Y, WIDTH, HEIGHT, NULL, NULL, NULL, NULL);

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
	switch (msg)
	{
	case WM_COMMAND: 
		switch (wparam)
		{
		case MenuExit:
			int result = MessageBox(hwnd, L"Вы действительно хотите выйти?", L"Выход из приложения", MB_YESNO);
			switch (result)
			{
			case YES:
				PostQuitMessage(0);
			case NO:
				break;
			}
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

	CreateWindowA("button", "Exit", WS_VISIBLE | WS_CHILD, (WIDTH - 80)/ 2, HEIGHT - 100, 80, 25, hwnd, NULL, NULL, NULL);

}

