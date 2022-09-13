#include<windows.h>
#include <iostream>
#include "Game_System.h"
using namespace std;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;//글로벌 인스턴스핸들값
LPCTSTR lpszClass = TEXT("체스게임"); //창이름

#pragma comment(lib, "msimg32.lib")


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPervlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	//WndClass는 기본 윈도우환경을 만드는 구조체다. 맴버변수는 밑에와 같다.
	WndClass.cbClsExtra = 0; //예약영역
	WndClass.cbWndExtra = 0; //예약영역 (신경x)
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//배경색
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//커서
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//아이콘 모양
	WndClass.hInstance = hInstance;//(프로그램 핸들값(번호)등록)
	WndClass.lpfnWndProc = WndProc;	//프로세스 함수 호출.
	WndClass.lpszClassName = lpszClass;	//클레스 이름
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;//윈도우의 수직과 수평이 변경 시 다시 그린다.
	RegisterClass(&WndClass);  //만들어진 WidClass를 등록

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))//사용자에게 메시지를 받아오는 함수(WM_QUIT 메시지 받을 시 종료)
	{
		TranslateMessage(&Message); //  키보드 입력 메시지 처리함수
		DispatchMessage(&Message); //받은 메시지를 WndProc에 전달하는 함수
	}
	return (int)Message.wParam;

}

HDC hdc;
Game_System gs;
int game_result = 0;
int game_over = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	int mouse_x = 0;
	int mouse_y = 0;

	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 1000, NULL);

		gs.Init_System(hdc, g_hInst);

		SendMessage(hWnd, WM_TIMER, 1, 0);
		return 0;
	case WM_TIMER:
			if (game_result == 10 && game_over == FALSE)
			{
				game_over = TRUE;

				if (MessageBox(hWnd, TEXT("흑팀승리!!"), TEXT("Black Win"), MB_OK) == IDOK)
				{
					game_result = 0;
				}
			}
			else if (game_result == 20 && game_over == FALSE)
			{
				game_over = TRUE;

				if (MessageBox(hWnd, TEXT("백팀승리!!"), TEXT("White Win"), MB_OK) == IDOK)
				{
					game_result = 0;
				}
			}
		return 0;
	case WM_LBUTTONDOWN:
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);

		game_result = gs.Click(hdc, mouse_x, mouse_y);

		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		gs.Draw(hdc);

		EndPaint(hWnd, &ps);

		return 0;
	case WM_DESTROY:// 윈도우가 파괴되었다는 메세지
		PostQuitMessage(0); //GetMessage함수에 WM_QUIT 메시지를 보낸다.
		KillTimer(hWnd, 1);
		return 0; //WndProc의 Switch는 break 대신 return 0; 를 쓴다.
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam)); // case에 있는 메시지를 제외한 나머지 메시지를 처리한다.
}
