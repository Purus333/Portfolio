#include <windows.h>
#include <iostream>
#include <vector>
#include <time.h>
#include "GameSystem.h"
using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
char g_szClassName[256] = "배틀시티";
#pragma comment(lib, "msimg32.lib") // 링커 2019 오류 뜨면 추가해주면 된다.

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = g_szClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(g_szClassName, g_szClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		1200, 738, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	GameSystem::GetInstane()->Init(hWnd);

	srand((unsigned)time(NULL));

	while (1)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				break;

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			if (GameSystem::GetInstane()->game_status == NULL)
				GameSystem::GetInstane()->Title_Screen();
			else if (GameSystem::GetInstane()->game_status == GAME_OVER)
				GameSystem::GetInstane()->Stage_Screen();
			else if (GameSystem::GetInstane()->game_status == GAME_START && GameSystem::GetInstane()->clear_trigger == false)
				GameSystem::GetInstane()->Game_Screen();
			else if (GameSystem::GetInstane()->game_status == GAME_START && GameSystem::GetInstane()->clear_trigger == true)
				GameSystem::GetInstane()->Clear_Screen();
			else if (GameSystem::GetInstane()->game_status == GAME_EXIT)
				break;
		}

	}

	delete GameSystem::GetInstane();

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = NULL;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:
		return 0;
	case WM_KEYDOWN:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		return 0;
	case WM_TIMER:
		return 0;
	case WM_DESTROY:

		PostQuitMessage(0);

		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}