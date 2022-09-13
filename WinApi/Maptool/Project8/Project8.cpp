#include <windows.h>
#include <iostream>
#include <vector>
using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
char g_szClassName[256] = "맵툴";
#pragma comment(lib, "msimg32.lib") // 링커 2019 오류 뜨면 추가해주면 된다.

#define WIDTH 24
#define HEIGHT 24
#define MAP_MAX 28

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
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

struct game_map
{
	char block;
	RECT rt;
};

char load_message[30] = "불러왔습니다";
game_map map[MAP_MAX][MAP_MAX];
char select_block = 'N';

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush;
	HBRUSH old_hBrush;

	switch (iMessage)
	{
	case WM_CREATE:

		for (int i = 0; i < MAP_MAX; i++) // 맵을 초기화
		{
			for (int j = 0; j < MAP_MAX; j++)
			{
				map[i][j].block = 'N';
				map[i][j].rt = { j * WIDTH, i * HEIGHT, j * WIDTH + 24, i * HEIGHT + 24 };
			}
		}

		CreateWindow("button", "NOTING", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 900, 0, 80, 30, hWnd, (HMENU)0, g_hInst, NULL);
		CreateWindow("button", "NOMAL_BLOCK", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 900, 40, 130, 30, hWnd, (HMENU)1, g_hInst, NULL);
		CreateWindow("button", "WHITE_BLOCK", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 900, 80, 130, 30, hWnd, (HMENU)2, g_hInst, NULL);
		CreateWindow("button", "GRAY_BLOCK", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 900, 120, 130, 30, hWnd, (HMENU)3, g_hInst, NULL);
		CreateWindow("button", "BUSH_BLOCK", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 900, 160, 130, 30, hWnd, (HMENU)4, g_hInst, NULL);
		CreateWindow("button", "WATER_BLOCK", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 900, 200, 130, 30, hWnd, (HMENU)5, g_hInst, NULL);

		CreateWindow("button", "Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 900, 300, 100, 30, hWnd, (HMENU)100, g_hInst, NULL);
		CreateWindow("button", "Load", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 900, 350, 100, 30, hWnd, (HMENU)101, g_hInst, NULL);
		return 0;
	case WM_TIMER:
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 0:
			select_block = 'N';
			break;
		case 1:
			select_block = 'B'; // 노말 블럭
			break;
		case 2:
			select_block = 'W';
			break;
		case 3:
			select_block = 'G';
			break;
		case 4:
			select_block = 'b'; // 부쉬
			break;
		case 5:
			select_block = 'w'; // 워터
			break;
		case 100: // 세이브
		{
			OPENFILENAME OFN;
			char str[300];
			char lpstrFile[MAX_PATH] = "";
			char lpstrPath[MAX_PATH] = "";

			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hWnd;
			OFN.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 256;

			//현재 폴더의 경로를 얻어온다(어플리케이션이 실행되는)
			GetCurrentDirectory(MAX_PATH, lpstrPath);

			OFN.lpstrInitialDir = lpstrPath;

			//OPENFILENAME 구조체로 파일열기 대화창을 열고 해당파일을 열게되면
			if (GetSaveFileName(&OFN) == 0)
			{
				DWORD err = CommDlgExtendedError();
				break;
			}

			HANDLE hFile = CreateFile(OFN.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

			for (int i = 0; i < MAP_MAX; i++)
			{
				for (int j = 0; j < MAP_MAX; j++)
				{
					DWORD writeB;

					WriteFile(hFile, &map[i][j].block, sizeof(char), &writeB, NULL);
				}
			}

			MessageBox(hWnd, "성공", "Save", MB_OK);

			CloseHandle(hFile); // 꼭닫아야함 아니면 메모리 누수
			InvalidateRect(hWnd, NULL, false);
		}
		break;
		case 101: // 로드
		{
			HANDLE hFile = CreateFile("stage.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

			if (hFile == INVALID_HANDLE_VALUE)
			{
				MessageBox(hWnd, "stage.txt 파일이 없습니다.", "오류", MB_OK);
			}

			else
			{
				for (int i = 0; i < MAP_MAX; i++)
				{
					for (int j = 0; j < MAP_MAX; j++)
					{
						DWORD readB;
						ReadFile(hFile, &map[i][j].block, sizeof(char), &readB, NULL);
					}
				}

				MessageBox(hWnd, load_message, "Load", MB_OK);
			}

			CloseHandle(hFile);
			InvalidateRect(hWnd, NULL, false);
		}
		break;
		}
		return 0;
	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		if (pt.x < 672 && pt.x > 0 && pt.y < 672 && pt.y > 0)
		{
			for (int i = 0; i < MAP_MAX; i++)
			{
				for (int j = 0; j < MAP_MAX; j++)
				{
					if (pt.x >= map[i][j].rt.left && pt.x < map[i][j].rt.right && pt.y >= map[i][j].rt.top && pt.y < map[i][j].rt.bottom)
					{
						map[i][j].block = select_block;
						InvalidateRect(hWnd, NULL, false);
						return 0;
					}
				}
			}
		}
	}
		return 0;
	case WM_RBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		if (pt.x < 672 && pt.x > 0 && pt.y < 672 && pt.y > 0)
		{
			for (int i = 0; i < MAP_MAX; i++)
			{
				for (int j = 0; j < MAP_MAX; j++)
				{
					if (pt.x >= map[i][j].rt.left && pt.x < map[i][j].rt.right && pt.y >= map[i][j].rt.top && pt.y < map[i][j].rt.bottom)
					{
						map[i][j].block = 'N';
						InvalidateRect(hWnd, NULL, false);
						return 0;
					}
				}
			}

		}
	}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < MAP_MAX; i++)
		{
			for (int j = 0; j < MAP_MAX; j++)
			{
				if (map[i][j].block == 'N')
					Rectangle(hdc, j * WIDTH, i * HEIGHT, (j + 1) * WIDTH, (i + 1) * HEIGHT);
				else if (map[i][j].block == 'B')
				{
					hBrush = CreateSolidBrush(RGB(150, 75, 0));
					old_hBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Rectangle(hdc, j * WIDTH, i * HEIGHT, (j + 1) * WIDTH, (i + 1) * HEIGHT);
					SelectObject(hdc, old_hBrush);
					DeleteObject(hBrush);
				}
				else if (map[i][j].block == 'W')
				{
					hBrush = CreateSolidBrush(RGB(190, 190, 190));
					old_hBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Rectangle(hdc, j * WIDTH, i * HEIGHT, (j + 1) * WIDTH, (i + 1) * HEIGHT);
					SelectObject(hdc, old_hBrush);
					DeleteObject(hBrush);
				}
				else if (map[i][j].block == 'G')
				{
					hBrush = CreateSolidBrush(RGB(110, 110, 110));
					old_hBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Rectangle(hdc, j * WIDTH, i * HEIGHT, (j + 1) * WIDTH, (i + 1) * HEIGHT);
					SelectObject(hdc, old_hBrush);
					DeleteObject(hBrush);
				}
				else if (map[i][j].block == 'b')
				{
					hBrush = CreateSolidBrush(RGB(0, 255, 0));
					old_hBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Rectangle(hdc, j * WIDTH, i * HEIGHT, (j + 1) * WIDTH, (i + 1) * HEIGHT);
					SelectObject(hdc, old_hBrush);
					DeleteObject(hBrush);
				}
				else if (map[i][j].block == 'w')
				{
					hBrush = CreateSolidBrush(RGB(0, 0, 255));
					old_hBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Rectangle(hdc, j * WIDTH, i * HEIGHT, (j + 1) * WIDTH, (i + 1) * HEIGHT);
					SelectObject(hdc, old_hBrush);
					DeleteObject(hBrush);
				}
			}
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}