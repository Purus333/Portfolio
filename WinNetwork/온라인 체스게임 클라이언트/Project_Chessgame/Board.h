#include "Player.h"
#pragma once

#define BOARD_MAX 64

class Board
{
private:
	HDC MemDC;
	HBITMAP hbtmap, old_hbtmap;
	BITMAP btmap;
public:
	Board();

	int board_num;
	int m_x;
	int m_y;
	int pos_x;
	int pos_y;

	void Board_Init(HDC hdc, HINSTANCE hinst, int x, int y);
	void Board_Draw(HDC hdc, int x, int y);

	~Board();
};

