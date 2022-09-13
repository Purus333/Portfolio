#include "chess_pieces.h"

chess_pieces::chess_pieces()
{

}

void chess_pieces::Init_pieces(HDC hdc, int player_num)
{
	int num_x = 0;

	if (player_num == 0) // 백
	{
		m_King.x = 300;
		m_King.y = 525;
		m_King.draw = FALSE;
		m_King.rt = {300, 525, 375, 600}; // 말의 영역
		m_King.status = ALIVE;

		m_Queen.x = 225;
		m_Queen.y = 525;
		m_Queen.draw = FALSE;
		m_Queen.rt = { 225, 525, 300, 600 };
		m_Queen.status = ALIVE;

		m_Rook[0].x = 0;
		m_Rook[0].y = 525;
		m_Rook[0].draw = FALSE;
		m_Rook[0].rt = { 0, 525, 75, 600 };
		m_Rook[0].status = ALIVE;
		m_Rook[1].x = 525;
		m_Rook[1].y = 525;
		m_Rook[1].draw = FALSE;
		m_Rook[1].rt = { 525, 525, 600, 600 };
		m_Rook[1].status = ALIVE;

		m_Bishop[0].x = 150;
		m_Bishop[0].y = 525;
		m_Bishop[0].draw = FALSE;
		m_Bishop[0].rt = { 150, 525, 225, 600 };
		m_Bishop[0].status = ALIVE;
		m_Bishop[1].x = 375;
		m_Bishop[1].y = 525;
		m_Bishop[1].draw = FALSE;
		m_Bishop[1].rt = { 375, 525, 450, 600 };
		m_Bishop[1].status = ALIVE;

		m_Knight[0].x = 75;
		m_Knight[0].y = 525;
		m_Knight[0].draw = FALSE;
		m_Knight[0].rt = { 75, 525, 150, 600 };
		m_Knight[0].status = ALIVE;
		m_Knight[1].x = 450;
		m_Knight[1].y = 525;
		m_Knight[1].draw = FALSE;
		m_Knight[1].rt = { 450, 525, 525, 600 };
		m_Knight[1].status = ALIVE;

		for (int i = 0; i < 8; i++)
		{
			if (i == 0)
			{
				m_Pawn[i].x = 0;
				m_Pawn[i].y = 450;
				m_Pawn[i].draw = FALSE;
				m_Pawn[i].rt = { 0, 450, 75, 525 };
				m_Pawn[i].first_move = FALSE;
				m_Pawn[i].status = ALIVE;
			}
			else
			{
				num_x += 75;
				m_Pawn[i].x += num_x;
				m_Pawn[i].y = 450;
				m_Pawn[i].draw = FALSE;
				m_Pawn[i].rt = { num_x, 450, num_x + 75, 525 };
				m_Pawn[i].first_move = FALSE;
				m_Pawn[i].status = ALIVE;
			}

		}
	}

	else // 흑
	{
		m_King.x = 300;
		m_King.y = 0;
		m_King.draw = FALSE;
		m_King.rt = { 300, 0, 375, 75 };
		m_King.status = ALIVE;

		m_Queen.x = 225;
		m_Queen.y = 0;
		m_Queen.draw = FALSE;
		m_Queen.rt = { 225, 0, 300, 75 };
		m_Queen.status = ALIVE;

		m_Rook[0].x = 0;
		m_Rook[0].y = 0;
		m_Rook[0].draw = FALSE;
		m_Rook[0].rt = { 0, 0, 75, 75 };
		m_Rook[0].status = ALIVE;
		m_Rook[1].x = 525;
		m_Rook[1].y = 0;
		m_Rook[1].draw = FALSE;
		m_Rook[1].rt = { 525, 0, 600, 75 };
		m_Rook[1].status = ALIVE;

		m_Bishop[0].x = 150;
		m_Bishop[0].y = 0;
		m_Bishop[0].draw = FALSE;
		m_Bishop[0].rt = { 150, 0, 225, 75 };
		m_Bishop[0].status = ALIVE;
		m_Bishop[1].x = 375;
		m_Bishop[1].y = 0;
		m_Bishop[1].draw = FALSE;
		m_Bishop[1].rt = { 375, 0, 450, 75 };
		m_Bishop[1].status = ALIVE;

		m_Knight[0].x = 75;
		m_Knight[0].y = 0;
		m_Knight[0].draw = FALSE;
		m_Knight[0].rt = { 75, 0, 150, 75 };
		m_Knight[0].status = ALIVE;
		m_Knight[1].x = 450;
		m_Knight[1].y = 0;
		m_Knight[1].draw = FALSE;
		m_Knight[1].rt = { 450, 0, 525, 75 };
		m_Knight[1].status = ALIVE;

		for (int i = 0; i < 8; i++)
		{
			if (i == 0)
			{
				m_Pawn[i].x = 0;
				m_Pawn[i].y = 75;
				m_Pawn[i].draw = FALSE;
				m_Pawn[i].rt = { 0, 75, 75, 150 };
				m_Pawn[i].first_move = FALSE;
				m_Pawn[i].status = ALIVE;
			}
			else
			{
				num_x += 75;
				m_Pawn[i].x += num_x;
				m_Pawn[i].y = 75;
				m_Pawn[i].draw = FALSE;
				m_Pawn[i].rt = { num_x, 75, num_x + 75, 150 };
				m_Pawn[i].first_move = FALSE;
				m_Pawn[i].status = ALIVE;
			}

		}
	}

}

void chess_pieces::Init_pieces_2(HDC hdc, int player_num)
{
	if (player_num == 0)
	{
		if (m_King.draw == FALSE)
		{
			MemDC[0] = CreateCompatibleDC(hdc);
			hbtmap = (HBITMAP)LoadImage(NULL, "block_w_05.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
			old_hbtmap = (HBITMAP)SelectObject(MemDC[0], hbtmap);
			GetObject(hbtmap, sizeof(BITMAP), &btmap);
			m_x = btmap.bmWidth;
			m_y = btmap.bmHeight;

			m_King.draw = TRUE;
		}

		if (m_Queen.draw == FALSE)
		{
			MemDC[1] = CreateCompatibleDC(hdc);
			hbtmap = (HBITMAP)LoadImage(NULL, "block_w_04.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
			old_hbtmap = (HBITMAP)SelectObject(MemDC[1], hbtmap);
			GetObject(hbtmap, sizeof(BITMAP), &btmap);
			m_x = btmap.bmWidth;
			m_y = btmap.bmHeight;

			m_Queen.draw = TRUE;
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_Rook[i].draw == FALSE)
			{
				MemDC[i + 2] = CreateCompatibleDC(hdc);
				hbtmap = (HBITMAP)LoadImage(NULL, "block_w_03.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
				old_hbtmap = (HBITMAP)SelectObject(MemDC[i + 2], hbtmap);
				GetObject(hbtmap, sizeof(BITMAP), &btmap);
				m_x = btmap.bmWidth;
				m_y = btmap.bmHeight;

				m_Rook[i].draw = TRUE;
			}
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_Bishop[i].draw == FALSE)
			{
				MemDC[i + 4] = CreateCompatibleDC(hdc);
				hbtmap = (HBITMAP)LoadImage(NULL, "block_w_02.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
				old_hbtmap = (HBITMAP)SelectObject(MemDC[i + 4], hbtmap);
				GetObject(hbtmap, sizeof(BITMAP), &btmap);
				m_x = btmap.bmWidth;
				m_y = btmap.bmHeight;

				m_Bishop[i].draw = TRUE;
			}
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_Knight[i].draw == FALSE)
			{
				MemDC[i + 6] = CreateCompatibleDC(hdc);
				hbtmap = (HBITMAP)LoadImage(NULL, "block_w_01.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
				old_hbtmap = (HBITMAP)SelectObject(MemDC[i + 6], hbtmap);
				GetObject(hbtmap, sizeof(BITMAP), &btmap);
				m_x = btmap.bmWidth;
				m_y = btmap.bmHeight;

				m_Knight[i].draw = TRUE;
			}
		}

		for (int i = 0; i < 8; i++)
		{
			if (m_Pawn[i].draw == FALSE)
			{
				MemDC[i + 8] = CreateCompatibleDC(hdc);
				hbtmap = (HBITMAP)LoadImage(NULL, "block_w_00.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
				old_hbtmap = (HBITMAP)SelectObject(MemDC[i + 8], hbtmap);
				GetObject(hbtmap, sizeof(BITMAP), &btmap);
				m_x = btmap.bmWidth;
				m_y = btmap.bmHeight;

				m_Pawn[i].draw = TRUE;
			}
		}

	}

	////////////////////////////

	else if (player_num == 1)
	{
		if (m_King.draw == FALSE)
		{
			MemDC[0] = CreateCompatibleDC(hdc);
			hbtmap = (HBITMAP)LoadImage(NULL, "block_b_05.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
			old_hbtmap = (HBITMAP)SelectObject(MemDC[0], hbtmap);
			GetObject(hbtmap, sizeof(BITMAP), &btmap);
			m_x = btmap.bmWidth;
			m_y = btmap.bmHeight;

			m_King.draw = TRUE;
		}

		if (m_Queen.draw == FALSE)
		{
			MemDC[1] = CreateCompatibleDC(hdc);
			hbtmap = (HBITMAP)LoadImage(NULL, "block_b_04.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
			old_hbtmap = (HBITMAP)SelectObject(MemDC[1], hbtmap);
			GetObject(hbtmap, sizeof(BITMAP), &btmap);
			m_x = btmap.bmWidth;
			m_y = btmap.bmHeight;

			m_Queen.draw = TRUE;
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_Rook[i].draw == FALSE)
			{
				MemDC[i + 2] = CreateCompatibleDC(hdc);
				hbtmap = (HBITMAP)LoadImage(NULL, "block_b_03.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
				old_hbtmap = (HBITMAP)SelectObject(MemDC[i + 2], hbtmap);
				GetObject(hbtmap, sizeof(BITMAP), &btmap);
				m_x = btmap.bmWidth;
				m_y = btmap.bmHeight;

				m_Rook[i].draw = TRUE;
			}
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_Bishop[i].draw == FALSE)
			{
				MemDC[i + 4] = CreateCompatibleDC(hdc);
				hbtmap = (HBITMAP)LoadImage(NULL, "block_b_02.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
				old_hbtmap = (HBITMAP)SelectObject(MemDC[i + 4], hbtmap);
				GetObject(hbtmap, sizeof(BITMAP), &btmap);
				m_x = btmap.bmWidth;
				m_y = btmap.bmHeight;

				m_Bishop[i].draw = TRUE;
			}
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_Knight[i].draw == FALSE)
			{
				MemDC[i + 6] = CreateCompatibleDC(hdc);
				hbtmap = (HBITMAP)LoadImage(NULL, "block_b_01.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
				old_hbtmap = (HBITMAP)SelectObject(MemDC[i + 6], hbtmap);
				GetObject(hbtmap, sizeof(BITMAP), &btmap);
				m_x = btmap.bmWidth;
				m_y = btmap.bmHeight;

				m_Knight[i].draw = TRUE;
			}
		}

		for (int i = 0; i < 8; i++)
		{
			if (m_Pawn[i].draw == FALSE)
			{
				MemDC[i + 8] = CreateCompatibleDC(hdc);
				hbtmap = (HBITMAP)LoadImage(NULL, "block_b_00.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
				old_hbtmap = (HBITMAP)SelectObject(MemDC[i + 8], hbtmap);
				GetObject(hbtmap, sizeof(BITMAP), &btmap);
				m_x = btmap.bmWidth;
				m_y = btmap.bmHeight;

				m_Pawn[i].draw = TRUE;
			}
		}
	}
}

void chess_pieces::Pieces_Draw(HDC hdc, int x, int y, string name)
{
	if (name == "king")
	TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[0], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "queen")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[1], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "rook1")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[2], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "rook2")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[3], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "bishop1")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[4], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "bishop2")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[5], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "knight1")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[6], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "knight2")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[7], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "pawn1")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[8], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "pawn2")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[9], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "pawn3")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[10], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "pawn4")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[11], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "pawn5")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[12], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "pawn6")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[13], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "pawn7")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[14], 0, 0, m_x, m_y, RGB(255, 0, 255));
	else if (name == "pawn8")
		TransparentBlt(hdc, x, y, m_x - 50, m_y - 50, MemDC[15], 0, 0, m_x, m_y, RGB(255, 0, 255));
}

chess_pieces::~chess_pieces()
{
	for (int i = 0; i < PIECE_MAX; i++)
	{
		DeleteDC(MemDC[i]);
	}
}