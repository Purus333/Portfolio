#include "Player.h"

Player::Player()
{

}

void Player::Init_Player(HDC hdc, int player_num)
{
	Cp = new chess_pieces;

	Cp->Init_pieces(hdc, player_num);
	Cp->Init_pieces_2(hdc, player_num);

	MemDC = CreateCompatibleDC(hdc);

	hbtmap = (HBITMAP)LoadImage(NULL, "block03.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	old_hbtmap = (HBITMAP)SelectObject(MemDC, hbtmap);
	GetObject(hbtmap, sizeof(BITMAP), &btmap);
	m_x = btmap.bmWidth;
	m_y = btmap.bmHeight;

	if (player_num == 0)
	{
		my_turn = TRUE;
	}
	else if (player_num == 1)
	{
		my_turn = FALSE;
	}

	clicked_object_num = NULL;
	tmp_rt.x = NULL;
	tmp_rt.y = NULL;
	who_is_moved = NULL;

}

void Player::Player_Pieces_Draw(HDC hdc)
{
	char buf[30] = {};

	if (Cp->m_King.status == ALIVE)
	Cp->Pieces_Draw(hdc, Cp->m_King.x, Cp->m_King.y, "king");
	if (Cp->m_Queen.status == ALIVE)
	Cp->Pieces_Draw(hdc, Cp->m_Queen.x, Cp->m_Queen.y, "queen");
	if (Cp->m_Rook[0].status == ALIVE)
	Cp->Pieces_Draw(hdc, Cp->m_Rook[0].x, Cp->m_Rook[0].y, "rook1");
	if (Cp->m_Rook[1].status == ALIVE)
	Cp->Pieces_Draw(hdc, Cp->m_Rook[1].x, Cp->m_Rook[1].y, "rook2");
	if (Cp->m_Bishop[0].status == ALIVE)
	Cp->Pieces_Draw(hdc, Cp->m_Bishop[0].x, Cp->m_Bishop[0].y, "bishop1");
	if (Cp->m_Bishop[1].status == ALIVE)
	Cp->Pieces_Draw(hdc, Cp->m_Bishop[1].x, Cp->m_Bishop[1].y, "bishop2");
	if (Cp->m_Knight[0].status == ALIVE)
	Cp->Pieces_Draw(hdc, Cp->m_Knight[0].x, Cp->m_Knight[0].y, "knight1");
	if (Cp->m_Knight[1].status == ALIVE)
	Cp->Pieces_Draw(hdc, Cp->m_Knight[1].x, Cp->m_Knight[1].y, "knight2");

	for (int i = 0; i < 8; i++)
	{
		sprintf_s(buf, "pawn%d", i + 1);
		if (Cp->m_Pawn[i].status == ALIVE)
		Cp->Pieces_Draw(hdc, Cp->m_Pawn[i].x, Cp->m_Pawn[i].y, buf);
	}

	Move_Able_Point(hdc); // 장기 클릭시 이동 범위출력

}

void Player::Move_Able_Point(HDC hdc)
{
	int path_num = 75;

	int draw_pawn_path = 0;

	if (select_num == SELECT_PAWN)
	{
		for (int i = 0; i < 8; i++)
		{
			if (clicked_object_num == i)
			{
				if (Cp->m_Pawn[clicked_object_num].first_move == TRUE)
					draw_pawn_path = TRUE;
			}
		}
	}

	// clicked_pos_x와 y에 +20은 꼭넣어야 사각형이 중앙에 그려진다
	//// pawn
	if (select_num == SELECT_PAWN && m_player_num == 0 && my_turn == TRUE)
	{
		if (someting == TRUE)
		{
			if (pawn_diagonal1 == TRUE)
				TransparentBlt(hdc, clicked_pos_x - 75 + 20, clicked_pos_y - 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
			if (pawn_diagonal2 == TRUE)
				TransparentBlt(hdc, clicked_pos_x + 75 + 20, clicked_pos_y - 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
		}
		if (pawn_front != TRUE)
			TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y - 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
		if (draw_pawn_path == FALSE && pawn_front != TRUE && pawn_double_front != TRUE)
			TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y - 150 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	else if (select_num == SELECT_PAWN && m_player_num == 1 && my_turn == TRUE)
	{
		if (someting == TRUE)
		{
			if (pawn_diagonal1 == TRUE)
				TransparentBlt(hdc, clicked_pos_x - 75 + 20, clicked_pos_y + 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
			if (pawn_diagonal2 == TRUE)
				TransparentBlt(hdc, clicked_pos_x + 75 + 20, clicked_pos_y + 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
		}
		if (pawn_front != TRUE)
			TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y + 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
		if (draw_pawn_path == FALSE && pawn_front != TRUE && pawn_double_front != TRUE)
			TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y + 150 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}

	//// rook
	else if (select_num == SELECT_ROOK && m_player_num == 0 && my_turn == TRUE)
	{
		if (rook_front != NULL)
		{
			for (int i = 0; i < rook_front; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y - path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (rook_back != NULL)
		{
			for (int i = 0; i < rook_back; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y + path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (rook_left != NULL)
		{
			for (int i = 0; i < rook_left; i++)
			{
				TransparentBlt(hdc, clicked_pos_x - path_num + 20, clicked_pos_y + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (rook_right != NULL)
		{
			for (int i = 0; i < rook_right; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + path_num + 20, clicked_pos_y + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
	}

	else if (select_num == SELECT_ROOK && m_player_num == 1 && my_turn == TRUE)
	{
		if (rook_front != NULL)
		{
			for (int i = 0; i < rook_front; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y + path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (rook_back != NULL)
		{
			for (int i = 0; i < rook_back; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y - path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (rook_left != NULL)
		{
			for (int i = 0; i < rook_left; i++)
			{
				TransparentBlt(hdc, clicked_pos_x - path_num + 20, clicked_pos_y + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (rook_right != NULL)
		{
			for (int i = 0; i < rook_right; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + path_num + 20, clicked_pos_y + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
	}

	//// knight
	else if (select_num == SELECT_KNIGHT && m_player_num == 0 && my_turn == TRUE)
	{
	if (knight_front_left_diagonal1 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x - 75 + 20, clicked_pos_y - 150 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (knight_front_left_diagonal2 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x - 150 + 20, clicked_pos_y - 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (knight_front_right_diagonal1 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 75 + 20, clicked_pos_y - 150 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (knight_front_right_diagonal2 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 150 + 20, clicked_pos_y - 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (knight_back_left_diagonal1 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x - 75 + 20, clicked_pos_y + 150 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (knight_back_left_diagonal2 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x - 150 + 20, clicked_pos_y + 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (knight_back_right_diagonal1 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 75 + 20, clicked_pos_y + 150 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (knight_back_right_diagonal2 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 150 + 20, clicked_pos_y + 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
    }

	else if (select_num == SELECT_KNIGHT && m_player_num == 1 && my_turn == TRUE)
	{
	if (knight_front_left_diagonal1 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x - 75 + 20, clicked_pos_y + 150 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (knight_front_left_diagonal2 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x - 150 + 20, clicked_pos_y + 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (knight_front_right_diagonal1 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 75 + 20, clicked_pos_y + 150 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (knight_front_right_diagonal2 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 150 + 20, clicked_pos_y + 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (knight_back_left_diagonal1 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x - 75 + 20, clicked_pos_y - 150 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (knight_back_left_diagonal2 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x - 150 + 20, clicked_pos_y - 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (knight_back_right_diagonal1 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 75 + 20, clicked_pos_y - 150 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (knight_back_right_diagonal2 != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 150 + 20, clicked_pos_y - 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}

    }

	//// bishop
	else if (select_num == SELECT_BISHOP && m_player_num == 0 && my_turn == TRUE)
	{
		if (bishop_front_left_diagonal != NULL)
		{
			for (int i = 0; i < bishop_front_left_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x - path_num + 20, clicked_pos_y - path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (bishop_front_right_diagonal != NULL)
		{
			for (int i = 0; i < bishop_front_right_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + path_num + 20, clicked_pos_y - path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (bishop_back_left_diagonal != NULL)
		{
			for (int i = 0; i < bishop_back_left_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x - path_num + 20, clicked_pos_y + path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (bishop_back_right_diagonal != NULL)
		{
			for (int i = 0; i < bishop_back_right_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + path_num + 20, clicked_pos_y + path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
    }

	else if (select_num == SELECT_BISHOP && m_player_num == 1 && my_turn == TRUE)
	{
		if (bishop_front_left_diagonal != NULL)
		{
			for (int i = 0; i < bishop_front_left_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x - path_num + 20, clicked_pos_y + path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (bishop_front_right_diagonal != NULL)
		{
			for (int i = 0; i < bishop_front_right_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + path_num + 20, clicked_pos_y + path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (bishop_back_left_diagonal != NULL)
		{
			for (int i = 0; i < bishop_back_left_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x - path_num + 20, clicked_pos_y - path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (bishop_back_right_diagonal != NULL)
		{
			for (int i = 0; i < bishop_back_right_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + path_num + 20, clicked_pos_y - path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
    }

	//// queen
	else if (select_num == SELECT_QUEEN && m_player_num == 0 && my_turn == TRUE)
	{
		if (queen_front != NULL)
		{
			for (int i = 0; i < queen_front; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y - path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (queen_front_left_diagonal != NULL)
		{
			for (int i = 0; i < queen_front_left_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x - path_num + 20, clicked_pos_y - path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (queen_front_right_diagonal != NULL)
		{
			for (int i = 0; i < queen_front_right_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + path_num + 20, clicked_pos_y - path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (queen_left != NULL)
		{
			for (int i = 0; i < queen_left; i++)
			{
				TransparentBlt(hdc, clicked_pos_x - path_num + 20, clicked_pos_y + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (queen_right != NULL)
		{
			for (int i = 0; i < queen_right; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + path_num + 20, clicked_pos_y + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (queen_back != NULL)
		{
			for (int i = 0; i < queen_back; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y + path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (queen_back_left_diagonal != NULL)
		{
			for (int i = 0; i < queen_back_left_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x - path_num + 20, clicked_pos_y + path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (queen_back_right_diagonal != NULL)
		{
			for (int i = 0; i < queen_back_right_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + path_num + 20, clicked_pos_y + path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
    }

	else if (select_num == SELECT_QUEEN && m_player_num == 1 && my_turn == TRUE)
	{
		if (queen_front != NULL)
		{
			for (int i = 0; i < queen_front; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y + path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (queen_front_left_diagonal != NULL)
		{
			for (int i = 0; i < queen_front_left_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x - path_num + 20, clicked_pos_y + path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (queen_front_right_diagonal != NULL)
		{
			for (int i = 0; i < queen_front_right_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + path_num + 20, clicked_pos_y + path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (queen_left != NULL)
		{
			for (int i = 0; i < queen_left; i++)
			{
				TransparentBlt(hdc, clicked_pos_x - path_num + 20, clicked_pos_y + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (queen_right != NULL)
		{
			for (int i = 0; i < queen_right; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + path_num + 20, clicked_pos_y + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (queen_back != NULL)
		{
			for (int i = 0; i < queen_back; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y - path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (queen_back_left_diagonal != NULL)
		{
			for (int i = 0; i < queen_back_left_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x - path_num + 20, clicked_pos_y - path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
		if (queen_back_right_diagonal != NULL)
		{
			for (int i = 0; i < queen_back_right_diagonal; i++)
			{
				TransparentBlt(hdc, clicked_pos_x + path_num + 20, clicked_pos_y - path_num + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
				path_num += 75;
			}
			path_num = 75;
		}
    }

	//// king
	else if (select_num == SELECT_KING && m_player_num == 0 && my_turn == TRUE)
	{
	if (king_front != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y - 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (king_front_left_diagonal != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x - 75 + 20, clicked_pos_y - 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (king_front_right_diagonal != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 75 + 20, clicked_pos_y - 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (king_left != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x - 75 + 20, clicked_pos_y + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (king_right != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 75 + 20, clicked_pos_y + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (king_back != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y + 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (king_back_left_diagonal != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x - 75 + 20, clicked_pos_y + 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (king_back_right_diagonal != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 75 + 20, clicked_pos_y + 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
    }

	else if (select_num == SELECT_KING && m_player_num == 1 && my_turn == TRUE)
	{
	if (king_front != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y + 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (king_front_left_diagonal != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x - 75 + 20, clicked_pos_y + 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (king_front_right_diagonal != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 75 + 20, clicked_pos_y + 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (king_left != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x - 75 + 20, clicked_pos_y + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (king_right != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 75 + 20, clicked_pos_y + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (king_back != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 20, clicked_pos_y - 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (king_back_left_diagonal != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x - 75 + 20, clicked_pos_y - 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
	if (king_back_right_diagonal != TRUE)
	{
		TransparentBlt(hdc, clicked_pos_x + 75 + 20, clicked_pos_y - 75 + 20, m_x - 90, m_y - 90, MemDC, 0, 0, m_x, m_y, RGB(255, 0, 255));
	}
    }
}

int Player::Move_Check(HDC hdc, int x, int y)
{
	int path_num = 75;
	int count = 0;

	int front_count = 0;
	int back_count = 0;
	int left_count = 0;
	int right_count = 0;

	int front_left_count = 0;
	int front_right_count = 0;
	int back_left_count = 0;
	int back_right_count = 0;

	// 클릭한곳으로 말을 옮기는 함수
	if (select_num == SELECT_PAWN && m_player_num == 0 && my_turn == TRUE) // 폰
	{
		if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top - 75 <= y && selected_object_rt.bottom - 75 >= y && pawn_front != TRUE)
		{
			Cp->m_Pawn[clicked_object_num].x = selected_object_rt.left;
			Cp->m_Pawn[clicked_object_num].y = selected_object_rt.top - 75;
			Cp->m_Pawn[clicked_object_num].rt = { selected_object_rt.left, selected_object_rt.top - 75, selected_object_rt.left + 75, selected_object_rt.top - 75 + 75 };

			tmp_rt.x = selected_object_rt.left;
			tmp_rt.y = selected_object_rt.top - 75;
			tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top - 75, selected_object_rt.left + 75, selected_object_rt.top - 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Pawn[clicked_object_num].first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top - 150 <= y && selected_object_rt.bottom - 150 >= y && Cp->m_Pawn[clicked_object_num].first_move != TRUE && someting == FALSE)
		{
			Cp->m_Pawn[clicked_object_num].x = selected_object_rt.left;
			Cp->m_Pawn[clicked_object_num].y = selected_object_rt.top - 150;
			Cp->m_Pawn[clicked_object_num].rt = { selected_object_rt.left, selected_object_rt.top - 150, selected_object_rt.left + 75, selected_object_rt.top - 150 + 75 };

			tmp_rt.x = selected_object_rt.left;
			tmp_rt.y = selected_object_rt.top - 150;
			tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top - 150, selected_object_rt.left + 75, selected_object_rt.top - 150 + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Pawn[clicked_object_num].first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left - 75 <= x && selected_object_rt.right - 75 >= x && selected_object_rt.top - 75 <= y && selected_object_rt.bottom - 75 >= y && pawn_diagonal1 == TRUE)
		{
			Cp->m_Pawn[clicked_object_num].x = selected_object_rt.left - 75;
			Cp->m_Pawn[clicked_object_num].y = selected_object_rt.top - 75;
			Cp->m_Pawn[clicked_object_num].rt = { selected_object_rt.left - 75, selected_object_rt.top - 75, selected_object_rt.left - 75 + 75, selected_object_rt.top - 75 + 75 };

			tmp_rt.x = selected_object_rt.left - 75;
			tmp_rt.y = selected_object_rt.top - 75;
			tmp_rt.rt = { selected_object_rt.left - 75, selected_object_rt.top - 75, selected_object_rt.left - 75 + 75, selected_object_rt.top - 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left + 75 <= x && selected_object_rt.right + 75 >= x && selected_object_rt.top - 75 <= y && selected_object_rt.bottom - 75 >= y && pawn_diagonal2 == TRUE)
		{
			Cp->m_Pawn[clicked_object_num].x = selected_object_rt.left + 75;
			Cp->m_Pawn[clicked_object_num].y = selected_object_rt.top - 75;
			Cp->m_Pawn[clicked_object_num].rt = { selected_object_rt.left + 75, selected_object_rt.top - 75, selected_object_rt.left + 75 + 75, selected_object_rt.top - 75 + 75 };

			tmp_rt.x = selected_object_rt.left + 75;
			tmp_rt.y = selected_object_rt.top - 75;
			tmp_rt.rt = { selected_object_rt.left + 75, selected_object_rt.top - 75, selected_object_rt.left + 75 + 75, selected_object_rt.top - 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
	}

	else if (select_num == SELECT_PAWN && m_player_num == 1 && my_turn == TRUE) // 폰
	{
		if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top + 75 <= y && selected_object_rt.bottom + 75 >= y && pawn_front != TRUE)
		{
			Cp->m_Pawn[clicked_object_num].x = selected_object_rt.left;
			Cp->m_Pawn[clicked_object_num].y = selected_object_rt.top + 75;
			Cp->m_Pawn[clicked_object_num].rt = { selected_object_rt.left, selected_object_rt.top + 75, selected_object_rt.left + 75, selected_object_rt.top + 75 + 75 };

			tmp_rt.x = selected_object_rt.left;
			tmp_rt.y = selected_object_rt.top + 75;
			tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top + 75, selected_object_rt.left + 75, selected_object_rt.top + 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Pawn[clicked_object_num].first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top + 150 <= y && selected_object_rt.bottom + 150 >= y && Cp->m_Pawn[clicked_object_num].first_move != TRUE && someting == FALSE)
		{
			Cp->m_Pawn[clicked_object_num].x = selected_object_rt.left;
			Cp->m_Pawn[clicked_object_num].y = selected_object_rt.top + 150;
			Cp->m_Pawn[clicked_object_num].rt = { selected_object_rt.left, selected_object_rt.top + 150, selected_object_rt.left + 75, selected_object_rt.top + 150 + 75 };

			tmp_rt.x = selected_object_rt.left;
			tmp_rt.y = selected_object_rt.top + 150;
			tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top + 150, selected_object_rt.left + 75, selected_object_rt.top + 150 + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Pawn[clicked_object_num].first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left - 75 <= x && selected_object_rt.right - 75 >= x && selected_object_rt.top + 75 <= y && selected_object_rt.bottom + 75 >= y && pawn_diagonal1 == TRUE)
		{
			Cp->m_Pawn[clicked_object_num].x = selected_object_rt.left - 75;
			Cp->m_Pawn[clicked_object_num].y = selected_object_rt.top + 75;
			Cp->m_Pawn[clicked_object_num].rt = { selected_object_rt.left - 75, selected_object_rt.top + 75, selected_object_rt.left - 75 + 75, selected_object_rt.top + 75 + 75 };

			tmp_rt.x = selected_object_rt.left - 75;
			tmp_rt.y = selected_object_rt.top + 75;
			tmp_rt.rt = { selected_object_rt.left - 75, selected_object_rt.top + 75, selected_object_rt.left - 75 + 75, selected_object_rt.top + 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left + 75 <= x && selected_object_rt.right + 75 >= x && selected_object_rt.top + 75 <= y && selected_object_rt.bottom + 75 >= y && pawn_diagonal2 == TRUE)
		{
			Cp->m_Pawn[clicked_object_num].x = selected_object_rt.left + 75;
			Cp->m_Pawn[clicked_object_num].y = selected_object_rt.top + 75;
			Cp->m_Pawn[clicked_object_num].rt = { selected_object_rt.left + 75, selected_object_rt.top + 75, selected_object_rt.left + 75 + 75, selected_object_rt.top + 75 + 75 };

			tmp_rt.x = selected_object_rt.left + 75;
			tmp_rt.y = selected_object_rt.top + 75;
			tmp_rt.rt = { selected_object_rt.left + 75, selected_object_rt.top + 75, selected_object_rt.left + 75 + 75, selected_object_rt.top + 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
	}

	else if (select_num == SELECT_ROOK && m_player_num == 0 && my_turn == TRUE) // 룩
	{
		while (1)
		{
			//앞
			if (count == rook_front + rook_back + rook_left + rook_right)
			{
				break;
			}
			if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top - path_num <= y && selected_object_rt.bottom - path_num >= y && front_count < rook_front)
			{

				Cp->m_Rook[clicked_object_num].x = selected_object_rt.left;
				Cp->m_Rook[clicked_object_num].y = selected_object_rt.top - path_num;
				Cp->m_Rook[clicked_object_num].rt = { selected_object_rt.left, selected_object_rt.top - path_num, selected_object_rt.left + 75, selected_object_rt.top - path_num + 75 };

				tmp_rt.x = selected_object_rt.left;
				tmp_rt.y = selected_object_rt.top - path_num;
				tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top - path_num, selected_object_rt.left + 75, selected_object_rt.top - path_num + 75 };

				select_num = 0;
				my_turn = FALSE;
				Cp->m_Rook[clicked_object_num].first_move = TRUE;
				who_is_moved = clicked_object_num;
				return TRUE;

			}
			//뒤
			else if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top + path_num <= y && selected_object_rt.bottom + path_num >= y && back_count < rook_back)
			{
				Cp->m_Rook[clicked_object_num].x = selected_object_rt.left;
				Cp->m_Rook[clicked_object_num].y = selected_object_rt.top + path_num;
				Cp->m_Rook[clicked_object_num].rt = { selected_object_rt.left, selected_object_rt.top + path_num, selected_object_rt.left + 75, selected_object_rt.top + path_num + 75 };

				tmp_rt.x = selected_object_rt.left;
				tmp_rt.y = selected_object_rt.top + path_num;
				tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top + path_num, selected_object_rt.left + 75, selected_object_rt.top + path_num + 75 };

				select_num = 0;
				my_turn = FALSE;
				Cp->m_Rook[clicked_object_num].first_move = TRUE;
				who_is_moved = clicked_object_num;
				return TRUE;
			}
			//왼쪽
			else if (selected_object_rt.left - path_num <= x && selected_object_rt.right - path_num >= x && selected_object_rt.top <= y && selected_object_rt.bottom >= y && left_count < rook_left)
			{
				Cp->m_Rook[clicked_object_num].x = selected_object_rt.left - path_num;
				Cp->m_Rook[clicked_object_num].y = selected_object_rt.top;
				Cp->m_Rook[clicked_object_num].rt = { selected_object_rt.left - path_num, selected_object_rt.top, selected_object_rt.left - path_num + 75, selected_object_rt.top + 75 };

				tmp_rt.x = selected_object_rt.left - path_num;
				tmp_rt.y = selected_object_rt.top;
				tmp_rt.rt = { selected_object_rt.left - path_num, selected_object_rt.top, selected_object_rt.left - path_num + 75, selected_object_rt.top + 75 };

				select_num = 0;
				my_turn = FALSE;
				Cp->m_Rook[clicked_object_num].first_move = TRUE;
				who_is_moved = clicked_object_num;
				return TRUE;
			}
			//오른쪽
			else if (selected_object_rt.left + path_num <= x && selected_object_rt.right + path_num >= x && selected_object_rt.top <= y && selected_object_rt.bottom >= y && right_count < rook_right)
			{
				Cp->m_Rook[clicked_object_num].x = selected_object_rt.left + path_num;
				Cp->m_Rook[clicked_object_num].y = selected_object_rt.top;
				Cp->m_Rook[clicked_object_num].rt = { selected_object_rt.left + path_num, selected_object_rt.top, selected_object_rt.left + path_num + 75, selected_object_rt.top + 75 };

				tmp_rt.x = selected_object_rt.left + path_num;
				tmp_rt.y = selected_object_rt.top;
				tmp_rt.rt = { selected_object_rt.left + path_num, selected_object_rt.top, selected_object_rt.left + path_num + 75, selected_object_rt.top + 75 };

				select_num = 0;
				my_turn = FALSE;
				Cp->m_Rook[clicked_object_num].first_move = TRUE;
				who_is_moved = clicked_object_num;
				return TRUE;
			}
			else
			{
				path_num += 75;
				count++;
				front_count++;
				back_count++;
				left_count++;
				right_count++;
			}
		}
	}

	else if (select_num == SELECT_ROOK && m_player_num == 1 && my_turn == TRUE)
	{
	while (1)
	{
		//앞
		if (count == rook_front + rook_back + rook_left + rook_right)
		{
			break;
		}
		if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top + path_num <= y && selected_object_rt.bottom + path_num >= y && front_count < rook_front)
		{

			Cp->m_Rook[clicked_object_num].x = selected_object_rt.left;
			Cp->m_Rook[clicked_object_num].y = selected_object_rt.top + path_num;
			Cp->m_Rook[clicked_object_num].rt = { selected_object_rt.left, selected_object_rt.top + path_num, selected_object_rt.left + 75, selected_object_rt.top + path_num + 75 };

			tmp_rt.x = selected_object_rt.left;
			tmp_rt.y = selected_object_rt.top + path_num;
			tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top + path_num, selected_object_rt.left + 75, selected_object_rt.top + path_num + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Rook[clicked_object_num].first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;

		}
		//뒤
		else if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top - path_num <= y && selected_object_rt.bottom - path_num >= y && back_count < rook_back)
		{
			Cp->m_Rook[clicked_object_num].x = selected_object_rt.left;
			Cp->m_Rook[clicked_object_num].y = selected_object_rt.top - path_num;
			Cp->m_Rook[clicked_object_num].rt = { selected_object_rt.left, selected_object_rt.top - path_num, selected_object_rt.left + 75, selected_object_rt.top - path_num + 75 };

			tmp_rt.x = selected_object_rt.left;
			tmp_rt.y = selected_object_rt.top - path_num;
			tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top - path_num, selected_object_rt.left + 75, selected_object_rt.top - path_num + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Rook[clicked_object_num].first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		//왼쪽
		else if (selected_object_rt.left - path_num <= x && selected_object_rt.right - path_num >= x && selected_object_rt.top <= y && selected_object_rt.bottom >= y && left_count < rook_left)
		{
			Cp->m_Rook[clicked_object_num].x = selected_object_rt.left - path_num;
			Cp->m_Rook[clicked_object_num].y = selected_object_rt.top;
			Cp->m_Rook[clicked_object_num].rt = { selected_object_rt.left - path_num, selected_object_rt.top, selected_object_rt.left - path_num + 75, selected_object_rt.top + 75 };

			tmp_rt.x = selected_object_rt.left - path_num;
			tmp_rt.y = selected_object_rt.top;
			tmp_rt.rt = { selected_object_rt.left - path_num, selected_object_rt.top, selected_object_rt.left - path_num + 75, selected_object_rt.top + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Rook[clicked_object_num].first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		//오른쪽
		else if (selected_object_rt.left + path_num <= x && selected_object_rt.right + path_num >= x && selected_object_rt.top <= y && selected_object_rt.bottom >= y && right_count < rook_right)
		{
			Cp->m_Rook[clicked_object_num].x = selected_object_rt.left + path_num;
			Cp->m_Rook[clicked_object_num].y = selected_object_rt.top;
			Cp->m_Rook[clicked_object_num].rt = { selected_object_rt.left + path_num, selected_object_rt.top, selected_object_rt.left + path_num + 75, selected_object_rt.top + 75 };

			tmp_rt.x = selected_object_rt.left + path_num;
			tmp_rt.y = selected_object_rt.top;
			tmp_rt.rt = { selected_object_rt.left + path_num, selected_object_rt.top, selected_object_rt.left + path_num + 75, selected_object_rt.top + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Rook[clicked_object_num].first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else
		{
			path_num += 75;
			count++;
			front_count++;
			back_count++;
			left_count++;
			right_count++;
		}
	}
	}

	else if (select_num == SELECT_KNIGHT && m_player_num == 0 && my_turn == TRUE) // 나이트
	{
	if (selected_object_rt.left - 75 <= x && selected_object_rt.right - 75 >= x && selected_object_rt.top - 150 <= y && selected_object_rt.bottom - 150 >= y && knight_front_left_diagonal1 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left - 75;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top - 150;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left - 75, selected_object_rt.top - 150, selected_object_rt.left - 75 + 75, selected_object_rt.top - 150 + 75 };

		tmp_rt.x = selected_object_rt.left - 75;
		tmp_rt.y = selected_object_rt.top - 150;
		tmp_rt.rt = { selected_object_rt.left - 75, selected_object_rt.top - 150, selected_object_rt.left - 75 + 75, selected_object_rt.top - 150 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
	else if (selected_object_rt.left - 150 <= x && selected_object_rt.right - 150 >= x && selected_object_rt.top - 75 <= y && selected_object_rt.bottom - 75 >= y && knight_front_left_diagonal2 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left - 150;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top - 75;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left - 150, selected_object_rt.top - 75, selected_object_rt.left - 150 + 75, selected_object_rt.top - 75 + 75 };

		tmp_rt.x = selected_object_rt.left - 150;
		tmp_rt.y = selected_object_rt.top - 75;
		tmp_rt.rt = { selected_object_rt.left - 150, selected_object_rt.top - 75, selected_object_rt.left - 150 + 75, selected_object_rt.top - 75 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
	else if (selected_object_rt.left + 75 <= x && selected_object_rt.right + 75 >= x && selected_object_rt.top - 150 <= y && selected_object_rt.bottom - 150 >= y && knight_front_right_diagonal1 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left + 75;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top - 150;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left + 75, selected_object_rt.top - 150, selected_object_rt.left + 75 + 75, selected_object_rt.top - 150 + 75 };

		tmp_rt.x = selected_object_rt.left + 75;
		tmp_rt.y = selected_object_rt.top - 150;
		tmp_rt.rt = { selected_object_rt.left + 75, selected_object_rt.top - 150, selected_object_rt.left + 75 + 75, selected_object_rt.top - 150 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
	else if (selected_object_rt.left + 150 <= x && selected_object_rt.right + 150 >= x && selected_object_rt.top - 75 <= y && selected_object_rt.bottom - 75 >= y && knight_front_right_diagonal2 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left + 150;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top - 75;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left + 150, selected_object_rt.top - 75, selected_object_rt.left + 150 + 75, selected_object_rt.top - 75 + 75 };

		tmp_rt.x = selected_object_rt.left + 150;
		tmp_rt.y = selected_object_rt.top - 75;
		tmp_rt.rt = { selected_object_rt.left + 150, selected_object_rt.top - 75, selected_object_rt.left + 150 + 75, selected_object_rt.top - 75 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
	else if (selected_object_rt.left - 75 <= x && selected_object_rt.right - 75 >= x && selected_object_rt.top + 150 <= y && selected_object_rt.bottom + 150 >= y && knight_back_left_diagonal1 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left - 75;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top + 150;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left - 75, selected_object_rt.top + 150, selected_object_rt.left - 75 + 75, selected_object_rt.top + 150 + 75 };

		tmp_rt.x = selected_object_rt.left - 75;
		tmp_rt.y = selected_object_rt.top + 150;
		tmp_rt.rt = { selected_object_rt.left - 75, selected_object_rt.top + 150, selected_object_rt.left - 75 + 75, selected_object_rt.top + 150 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
	else if (selected_object_rt.left - 150 <= x && selected_object_rt.right - 150 >= x && selected_object_rt.top + 75 <= y && selected_object_rt.bottom + 75 >= y && knight_back_left_diagonal2 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left - 150;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top + 75;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left - 150, selected_object_rt.top + 75, selected_object_rt.left - 150 + 75, selected_object_rt.top + 75 + 75 };

		tmp_rt.x = selected_object_rt.left - 150;
		tmp_rt.y = selected_object_rt.top + 75;
		tmp_rt.rt = { selected_object_rt.left - 150, selected_object_rt.top + 75, selected_object_rt.left - 150 + 75, selected_object_rt.top + 75 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
	else if (selected_object_rt.left + 75 <= x && selected_object_rt.right + 75 >= x && selected_object_rt.top + 150 <= y && selected_object_rt.bottom + 150 >= y && knight_back_right_diagonal1 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left + 75;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top + 150;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left + 75, selected_object_rt.top + 150, selected_object_rt.left + 75 + 75, selected_object_rt.top + 150 + 75 };

		tmp_rt.x = selected_object_rt.left + 75;
		tmp_rt.y = selected_object_rt.top + 150;
		tmp_rt.rt = { selected_object_rt.left + 75, selected_object_rt.top + 150, selected_object_rt.left + 75 + 75, selected_object_rt.top + 150 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
	else if (selected_object_rt.left + 150 <= x && selected_object_rt.right + 150 >= x && selected_object_rt.top + 75 <= y && selected_object_rt.bottom + 75 >= y && knight_back_right_diagonal2 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left + 150;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top + 75;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left + 150, selected_object_rt.top + 75, selected_object_rt.left + 150 + 75, selected_object_rt.top + 75 + 75 };

		tmp_rt.x = selected_object_rt.left + 150;
		tmp_rt.y = selected_object_rt.top + 75;
		tmp_rt.rt = { selected_object_rt.left + 150, selected_object_rt.top + 75, selected_object_rt.left + 150 + 75, selected_object_rt.top + 75 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
    }

	else if (select_num == SELECT_KNIGHT && m_player_num == 1 && my_turn == TRUE)
	{
	if (selected_object_rt.left - 75 <= x && selected_object_rt.right - 75 >= x && selected_object_rt.top + 150 <= y && selected_object_rt.bottom + 150 >= y && knight_front_left_diagonal1 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left - 75;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top + 150;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left - 75, selected_object_rt.top + 150, selected_object_rt.left - 75 + 75, selected_object_rt.top + 150 + 75 };

		tmp_rt.x = selected_object_rt.left - 75;
		tmp_rt.y = selected_object_rt.top + 150;
		tmp_rt.rt = { selected_object_rt.left - 75, selected_object_rt.top + 150, selected_object_rt.left - 75 + 75, selected_object_rt.top + 150 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
	else if (selected_object_rt.left - 150 <= x && selected_object_rt.right - 150 >= x && selected_object_rt.top + 75 <= y && selected_object_rt.bottom + 75 >= y && knight_front_left_diagonal2 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left - 150;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top + 75;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left - 150, selected_object_rt.top + 75, selected_object_rt.left - 150 + 75, selected_object_rt.top + 75 + 75 };

		tmp_rt.x = selected_object_rt.left - 150;
		tmp_rt.y = selected_object_rt.top + 75;
		tmp_rt.rt = { selected_object_rt.left - 150, selected_object_rt.top + 75, selected_object_rt.left - 150 + 75, selected_object_rt.top + 75 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
	else if (selected_object_rt.left + 75 <= x && selected_object_rt.right + 75 >= x && selected_object_rt.top + 150 <= y && selected_object_rt.bottom + 150 >= y && knight_front_right_diagonal1 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left + 75;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top + 150;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left + 75, selected_object_rt.top + 150, selected_object_rt.left + 75 + 75, selected_object_rt.top + 150 + 75 };

		tmp_rt.x = selected_object_rt.left + 75;
		tmp_rt.y = selected_object_rt.top + 150;
		tmp_rt.rt = { selected_object_rt.left + 75, selected_object_rt.top + 150, selected_object_rt.left + 75 + 75, selected_object_rt.top + 150 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
	else if (selected_object_rt.left + 150 <= x && selected_object_rt.right + 150 >= x && selected_object_rt.top + 75 <= y && selected_object_rt.bottom + 75 >= y && knight_front_right_diagonal2 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left + 150;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top + 75;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left + 150, selected_object_rt.top + 75, selected_object_rt.left + 150 + 75, selected_object_rt.top + 75 + 75 };

		tmp_rt.x = selected_object_rt.left + 150;
		tmp_rt.y = selected_object_rt.top + 75;
		tmp_rt.rt = { selected_object_rt.left + 150, selected_object_rt.top + 75, selected_object_rt.left + 150 + 75, selected_object_rt.top + 75 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
	else if (selected_object_rt.left - 75 <= x && selected_object_rt.right - 75 >= x && selected_object_rt.top - 150 <= y && selected_object_rt.bottom - 150 >= y && knight_back_left_diagonal1 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left - 75;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top - 150;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left - 75, selected_object_rt.top - 150, selected_object_rt.left - 75 + 75, selected_object_rt.top - 150 + 75 };

		tmp_rt.x = selected_object_rt.left - 75;
		tmp_rt.y = selected_object_rt.top - 150;
		tmp_rt.rt = { selected_object_rt.left - 75, selected_object_rt.top - 150, selected_object_rt.left - 75 + 75, selected_object_rt.top - 150 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
	else if (selected_object_rt.left - 150 <= x && selected_object_rt.right - 150 >= x && selected_object_rt.top - 75 <= y && selected_object_rt.bottom - 75 >= y && knight_back_left_diagonal2 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left - 150;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top - 75;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left - 150, selected_object_rt.top - 75, selected_object_rt.left - 150 + 75, selected_object_rt.top - 75 + 75 };

		tmp_rt.x = selected_object_rt.left - 150;
		tmp_rt.y = selected_object_rt.top - 75;
		tmp_rt.rt = { selected_object_rt.left - 150, selected_object_rt.top - 75, selected_object_rt.left - 150 + 75, selected_object_rt.top - 75 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
	else if (selected_object_rt.left + 75 <= x && selected_object_rt.right + 75 >= x && selected_object_rt.top - 150 <= y && selected_object_rt.bottom - 150 >= y && knight_back_right_diagonal1 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left + 75;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top - 150;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left + 75, selected_object_rt.top - 150, selected_object_rt.left + 75 + 75, selected_object_rt.top - 150 + 75 };

		tmp_rt.x = selected_object_rt.left + 75;
		tmp_rt.y = selected_object_rt.top - 150;
		tmp_rt.rt = { selected_object_rt.left + 75, selected_object_rt.top - 150, selected_object_rt.left + 75 + 75, selected_object_rt.top - 150 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
	else if (selected_object_rt.left + 150 <= x && selected_object_rt.right + 150 >= x && selected_object_rt.top - 75 <= y && selected_object_rt.bottom - 75 >= y && knight_back_right_diagonal2 != TRUE)
	{
		Cp->m_Knight[clicked_object_num].x = selected_object_rt.left + 150;
		Cp->m_Knight[clicked_object_num].y = selected_object_rt.top - 75;
		Cp->m_Knight[clicked_object_num].rt = { selected_object_rt.left + 150, selected_object_rt.top - 75, selected_object_rt.left + 150 + 75, selected_object_rt.top - 75 + 75 };

		tmp_rt.x = selected_object_rt.left + 150;
		tmp_rt.y = selected_object_rt.top - 75;
		tmp_rt.rt = { selected_object_rt.left + 150, selected_object_rt.top - 75, selected_object_rt.left + 150 + 75, selected_object_rt.top - 75 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
	}
    }

	else if (select_num == SELECT_BISHOP && m_player_num == 0 && my_turn == TRUE)
	{
		while (1)
		{
			//앞의 왼쪽대각
			if (count == bishop_front_left_diagonal + bishop_front_right_diagonal + bishop_back_left_diagonal + bishop_back_right_diagonal)
			{
				break;
			}
			if (selected_object_rt.left - path_num <= x && selected_object_rt.right - path_num >= x && selected_object_rt.top - path_num <= y && selected_object_rt.bottom - path_num >= y && front_left_count < bishop_front_left_diagonal)
			{

				Cp->m_Bishop[clicked_object_num].x = selected_object_rt.left - path_num;
				Cp->m_Bishop[clicked_object_num].y = selected_object_rt.top - path_num;
				Cp->m_Bishop[clicked_object_num].rt = { selected_object_rt.left - path_num, selected_object_rt.top - path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top - path_num + 75 };

				tmp_rt.x = selected_object_rt.left - path_num;
				tmp_rt.y = selected_object_rt.top - path_num;
				tmp_rt.rt = { selected_object_rt.left - path_num, selected_object_rt.top - path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top - path_num + 75 };

				select_num = 0;
				my_turn = FALSE;
				Cp->m_Bishop[clicked_object_num].first_move = TRUE;
				who_is_moved = clicked_object_num;
				return TRUE;

			}
			//앞의 오른쪽대각
			else if (selected_object_rt.left + path_num <= x && selected_object_rt.right + path_num >= x && selected_object_rt.top - path_num <= y && selected_object_rt.bottom - path_num >= y && front_right_count < bishop_front_right_diagonal)
			{
				Cp->m_Bishop[clicked_object_num].x = selected_object_rt.left + path_num;
				Cp->m_Bishop[clicked_object_num].y = selected_object_rt.top - path_num;
				Cp->m_Bishop[clicked_object_num].rt = { selected_object_rt.left + path_num, selected_object_rt.top - path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top - path_num + 75 };

				tmp_rt.x = selected_object_rt.left + path_num;
				tmp_rt.y = selected_object_rt.top - path_num;
				tmp_rt.rt = { selected_object_rt.left + path_num, selected_object_rt.top - path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top - path_num + 75 };

				select_num = 0;
				my_turn = FALSE;
				Cp->m_Bishop[clicked_object_num].first_move = TRUE;
				who_is_moved = clicked_object_num;
				return TRUE;
			}
			//뒤의 왼쪽대각
			else if (selected_object_rt.left - path_num <= x && selected_object_rt.right - path_num >= x && selected_object_rt.top + path_num <= y && selected_object_rt.bottom + path_num >= y && back_left_count < bishop_back_left_diagonal)
			{

				Cp->m_Bishop[clicked_object_num].x = selected_object_rt.left - path_num;
				Cp->m_Bishop[clicked_object_num].y = selected_object_rt.top + path_num;
				Cp->m_Bishop[clicked_object_num].rt = { selected_object_rt.left - path_num, selected_object_rt.top + path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top + path_num + 75 };

				tmp_rt.x = selected_object_rt.left - path_num;
				tmp_rt.y = selected_object_rt.top + path_num;
				tmp_rt.rt = { selected_object_rt.left - path_num, selected_object_rt.top + path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top + path_num + 75 };

				select_num = 0;
				my_turn = FALSE;
				Cp->m_Bishop[clicked_object_num].first_move = TRUE;
				who_is_moved = clicked_object_num;
				return TRUE;

			}
			//뒤의 오른쪽대각
			else if (selected_object_rt.left + path_num <= x && selected_object_rt.right + path_num >= x && selected_object_rt.top + path_num <= y && selected_object_rt.bottom + path_num >= y && back_right_count < bishop_back_right_diagonal)
			{
				Cp->m_Bishop[clicked_object_num].x = selected_object_rt.left + path_num;
				Cp->m_Bishop[clicked_object_num].y = selected_object_rt.top + path_num;
				Cp->m_Bishop[clicked_object_num].rt = { selected_object_rt.left + path_num, selected_object_rt.top + path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top + path_num + 75 };

				tmp_rt.x = selected_object_rt.left + path_num;
				tmp_rt.y = selected_object_rt.top + path_num;
				tmp_rt.rt = { selected_object_rt.left + path_num, selected_object_rt.top + path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top + path_num + 75 };

				select_num = 0;
				my_turn = FALSE;
				Cp->m_Bishop[clicked_object_num].first_move = TRUE;
				who_is_moved = clicked_object_num;
				return TRUE;
			}
			else
			{
				path_num += 75;
				count++;
				front_left_count++;
				front_right_count++;
				back_left_count++;
				back_right_count++;
			}
		}
    }

	else if (select_num == SELECT_BISHOP && m_player_num == 1 && my_turn == TRUE)
	{
		while (1)
		{
			//앞의 왼쪽대각
			if (count == bishop_front_left_diagonal + bishop_front_right_diagonal + bishop_back_left_diagonal + bishop_back_right_diagonal)
			{
				break;
			}
			if (selected_object_rt.left - path_num <= x && selected_object_rt.right - path_num >= x && selected_object_rt.top + path_num <= y && selected_object_rt.bottom + path_num >= y && front_left_count < bishop_front_left_diagonal)
			{
				Cp->m_Bishop[clicked_object_num].x = selected_object_rt.left - path_num;
				Cp->m_Bishop[clicked_object_num].y = selected_object_rt.top + path_num;
				Cp->m_Bishop[clicked_object_num].rt = { selected_object_rt.left - path_num, selected_object_rt.top + path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top + path_num + 75 };

				tmp_rt.x = selected_object_rt.left - path_num;
				tmp_rt.y = selected_object_rt.top + path_num;
				tmp_rt.rt = { selected_object_rt.left - path_num, selected_object_rt.top + path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top + path_num + 75 };

				select_num = 0;
				my_turn = FALSE;
				Cp->m_Bishop[clicked_object_num].first_move = TRUE;
				who_is_moved = clicked_object_num;
				return TRUE;
			}
			//앞의 오른쪽대각
			else if (selected_object_rt.left + path_num <= x && selected_object_rt.right + path_num >= x && selected_object_rt.top + path_num <= y && selected_object_rt.bottom + path_num >= y && front_right_count < bishop_front_right_diagonal)
			{
				Cp->m_Bishop[clicked_object_num].x = selected_object_rt.left + path_num;
				Cp->m_Bishop[clicked_object_num].y = selected_object_rt.top + path_num;
				Cp->m_Bishop[clicked_object_num].rt = { selected_object_rt.left + path_num, selected_object_rt.top + path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top + path_num + 75 };

				tmp_rt.x = selected_object_rt.left + path_num;
				tmp_rt.y = selected_object_rt.top + path_num;
				tmp_rt.rt = { selected_object_rt.left + path_num, selected_object_rt.top + path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top + path_num + 75 };

				select_num = 0;
				my_turn = FALSE;
				Cp->m_Bishop[clicked_object_num].first_move = TRUE;
				who_is_moved = clicked_object_num;
				return TRUE;
			}
			//뒤의 왼쪽대각
			else if (selected_object_rt.left - path_num <= x && selected_object_rt.right - path_num >= x && selected_object_rt.top - path_num <= y && selected_object_rt.bottom - path_num >= y && back_left_count < bishop_back_left_diagonal)
			{
				Cp->m_Bishop[clicked_object_num].x = selected_object_rt.left - path_num;
				Cp->m_Bishop[clicked_object_num].y = selected_object_rt.top - path_num;
				Cp->m_Bishop[clicked_object_num].rt = { selected_object_rt.left - path_num, selected_object_rt.top - path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top - path_num + 75 };

				tmp_rt.x = selected_object_rt.left - path_num;
				tmp_rt.y = selected_object_rt.top - path_num;
				tmp_rt.rt = { selected_object_rt.left - path_num, selected_object_rt.top - path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top - path_num + 75 };

				select_num = 0;
				my_turn = FALSE;
				Cp->m_Bishop[clicked_object_num].first_move = TRUE;
				who_is_moved = clicked_object_num;
				return TRUE;
			}
			//뒤의 오른쪽대각
			else if (selected_object_rt.left + path_num <= x && selected_object_rt.right + path_num >= x && selected_object_rt.top - path_num <= y && selected_object_rt.bottom - path_num >= y && back_right_count < bishop_back_right_diagonal)
			{
				Cp->m_Bishop[clicked_object_num].x = selected_object_rt.left + path_num;
				Cp->m_Bishop[clicked_object_num].y = selected_object_rt.top - path_num;
				Cp->m_Bishop[clicked_object_num].rt = { selected_object_rt.left + path_num, selected_object_rt.top - path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top - path_num + 75 };

				tmp_rt.x = selected_object_rt.left + path_num;
				tmp_rt.y = selected_object_rt.top - path_num;
				tmp_rt.rt = { selected_object_rt.left + path_num, selected_object_rt.top - path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top - path_num + 75 };

				select_num = 0;
				my_turn = FALSE;
				Cp->m_Bishop[clicked_object_num].first_move = TRUE;
				who_is_moved = clicked_object_num;
				return TRUE;
			}
			else
			{
				path_num += 75;
				count++;
				front_left_count++;
				front_right_count++;
				back_left_count++;
				back_right_count++;
			}
		}
    }

	else if (select_num == SELECT_QUEEN && m_player_num == 0 && my_turn == TRUE) // 퀸
	{
	while (1)
	{
		if (count == queen_front + queen_front_left_diagonal + queen_front_right_diagonal + queen_left + queen_right + queen_back + queen_back_left_diagonal + queen_back_right_diagonal)
		{
			break;
		}
		if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top - path_num <= y && selected_object_rt.bottom - path_num >= y && front_count < queen_front)
		{
			// 앞
			Cp->m_Queen.x = selected_object_rt.left;
			Cp->m_Queen.y = selected_object_rt.top - path_num;
			Cp->m_Queen.rt = { selected_object_rt.left, selected_object_rt.top - path_num, selected_object_rt.left + 75, selected_object_rt.top - path_num + 75 };

			tmp_rt.x = selected_object_rt.left;
			tmp_rt.y = selected_object_rt.top - path_num;
			tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top - path_num, selected_object_rt.left + 75, selected_object_rt.top - path_num + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Queen.first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;

		}
		else if (selected_object_rt.left - path_num <= x && selected_object_rt.right - path_num >= x && selected_object_rt.top - path_num <= y && selected_object_rt.bottom - path_num >= y && front_left_count < queen_front_left_diagonal)
		{
			// 앞의 왼쪽대각
			Cp->m_Queen.x = selected_object_rt.left - path_num;
			Cp->m_Queen.y = selected_object_rt.top - path_num;
			Cp->m_Queen.rt = { selected_object_rt.left - path_num, selected_object_rt.top - path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top - path_num + 75 };

			tmp_rt.x = selected_object_rt.left - path_num;
			tmp_rt.y = selected_object_rt.top - path_num;
			tmp_rt.rt = { selected_object_rt.left - path_num, selected_object_rt.top - path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top - path_num + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Queen.first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;

		}
		else if (selected_object_rt.left + path_num <= x && selected_object_rt.right + path_num >= x && selected_object_rt.top - path_num <= y && selected_object_rt.bottom - path_num >= y && front_right_count < queen_front_right_diagonal)
		{
			// 앞의 오른쪽대각
			Cp->m_Queen.x = selected_object_rt.left + path_num;
			Cp->m_Queen.y = selected_object_rt.top - path_num;
			Cp->m_Queen.rt = { selected_object_rt.left + path_num, selected_object_rt.top - path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top - path_num + 75 };

			tmp_rt.x = selected_object_rt.left + path_num;
			tmp_rt.y = selected_object_rt.top - path_num;
			tmp_rt.rt = { selected_object_rt.left + path_num, selected_object_rt.top - path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top - path_num + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Queen.first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;

		}
		else if (selected_object_rt.left - path_num <= x && selected_object_rt.right - path_num >= x && selected_object_rt.top <= y && selected_object_rt.bottom >= y && left_count < queen_left)
		{
			// 왼쪽
			Cp->m_Queen.x = selected_object_rt.left - path_num;
			Cp->m_Queen.y = selected_object_rt.top;
			Cp->m_Queen.rt = { selected_object_rt.left - path_num, selected_object_rt.top, selected_object_rt.left - path_num + 75, selected_object_rt.top + 75 };

			tmp_rt.x = selected_object_rt.left - path_num;
			tmp_rt.y = selected_object_rt.top;
			tmp_rt.rt = { selected_object_rt.left - path_num, selected_object_rt.top, selected_object_rt.left - path_num + 75, selected_object_rt.top + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Queen.first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;

		}
		else if (selected_object_rt.left + path_num <= x && selected_object_rt.right + path_num >= x && selected_object_rt.top <= y && selected_object_rt.bottom >= y && right_count < queen_right)
		{
			// 오른쪽
			Cp->m_Queen.x = selected_object_rt.left + path_num;
			Cp->m_Queen.y = selected_object_rt.top;
			Cp->m_Queen.rt = { selected_object_rt.left + path_num, selected_object_rt.top, selected_object_rt.left + path_num + 75, selected_object_rt.top + 75 };

			tmp_rt.x = selected_object_rt.left + path_num;
			tmp_rt.y = selected_object_rt.top;
			tmp_rt.rt = { selected_object_rt.left + path_num, selected_object_rt.top, selected_object_rt.left + path_num + 75, selected_object_rt.top + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Queen.first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;

		}
		else if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top + path_num <= y && selected_object_rt.bottom + path_num >= y && back_count < queen_back)
		{
			// 뒤
			Cp->m_Queen.x = selected_object_rt.left;
			Cp->m_Queen.y = selected_object_rt.top + path_num;
			Cp->m_Queen.rt = { selected_object_rt.left, selected_object_rt.top + path_num, selected_object_rt.left + 75, selected_object_rt.top + path_num + 75 };

			tmp_rt.x = selected_object_rt.left;
			tmp_rt.y = selected_object_rt.top + path_num;
			tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top + path_num, selected_object_rt.left + 75, selected_object_rt.top + path_num + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Queen.first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;

		}
		else if (selected_object_rt.left - path_num <= x && selected_object_rt.right - path_num >= x && selected_object_rt.top + path_num <= y && selected_object_rt.bottom + path_num >= y && back_left_count < queen_back_left_diagonal)
		{
		// 뒤의 왼쪽대각
		Cp->m_Queen.x = selected_object_rt.left - path_num;
		Cp->m_Queen.y = selected_object_rt.top + path_num;
		Cp->m_Queen.rt = { selected_object_rt.left - path_num, selected_object_rt.top + path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top + path_num + 75 };

		tmp_rt.x = selected_object_rt.left - path_num;
		tmp_rt.y = selected_object_rt.top + path_num;
		tmp_rt.rt = { selected_object_rt.left - path_num, selected_object_rt.top + path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top + path_num + 75 };

		select_num = 0;
		my_turn = FALSE;
		Cp->m_Queen.first_move = TRUE;
		who_is_moved = clicked_object_num;
		return TRUE;

		}
		else if (selected_object_rt.left + path_num <= x && selected_object_rt.right + path_num >= x && selected_object_rt.top + path_num <= y && selected_object_rt.bottom + path_num >= y && back_right_count < queen_back_right_diagonal)
		{
		// 뒤의 오른쪽대각
		Cp->m_Queen.x = selected_object_rt.left + path_num;
		Cp->m_Queen.y = selected_object_rt.top + path_num;
		Cp->m_Queen.rt = { selected_object_rt.left + path_num, selected_object_rt.top + path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top + path_num + 75 };

		tmp_rt.x = selected_object_rt.left + path_num;
		tmp_rt.y = selected_object_rt.top + path_num;
		tmp_rt.rt = { selected_object_rt.left + path_num, selected_object_rt.top + path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top + path_num + 75 };

		select_num = 0;
		my_turn = FALSE;
		Cp->m_Queen.first_move = TRUE;
		who_is_moved = clicked_object_num;
		return TRUE;

		}
		else
		{
			path_num += 75;
			count++;
			front_count++;
			back_count++;
			left_count++;
			right_count++;
			front_left_count++;
			front_right_count++;
			back_left_count++;
			back_right_count++;
		}
	}
    }

	else if (select_num == SELECT_QUEEN && m_player_num == 1 && my_turn == TRUE)
	{
	while (1)
	{
		if (count == queen_front + queen_front_left_diagonal + queen_front_right_diagonal + queen_left + queen_right + queen_back + queen_back_left_diagonal + queen_back_right_diagonal)
		{
			break;
		}
		if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top + path_num <= y && selected_object_rt.bottom + path_num >= y && front_count < queen_front)
		{
			// 앞
			Cp->m_Queen.x = selected_object_rt.left;
			Cp->m_Queen.y = selected_object_rt.top + path_num;
			Cp->m_Queen.rt = { selected_object_rt.left, selected_object_rt.top + path_num, selected_object_rt.left + 75, selected_object_rt.top + path_num + 75 };

			tmp_rt.x = selected_object_rt.left;
			tmp_rt.y = selected_object_rt.top + path_num;
			tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top + path_num, selected_object_rt.left + 75, selected_object_rt.top + path_num + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Queen.first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;

		}
		else if (selected_object_rt.left - path_num <= x && selected_object_rt.right - path_num >= x && selected_object_rt.top + path_num <= y && selected_object_rt.bottom + path_num >= y && front_left_count < queen_front_left_diagonal)
		{
			// 앞의 왼쪽대각
			Cp->m_Queen.x = selected_object_rt.left - path_num;
			Cp->m_Queen.y = selected_object_rt.top + path_num;
			Cp->m_Queen.rt = { selected_object_rt.left - path_num, selected_object_rt.top + path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top + path_num + 75 };

			tmp_rt.x = selected_object_rt.left - path_num;
			tmp_rt.y = selected_object_rt.top + path_num;
			tmp_rt.rt = { selected_object_rt.left - path_num, selected_object_rt.top + path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top + path_num + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Queen.first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;

		}
		else if (selected_object_rt.left + path_num <= x && selected_object_rt.right + path_num >= x && selected_object_rt.top + path_num <= y && selected_object_rt.bottom + path_num >= y && front_right_count < queen_front_right_diagonal)
		{
			// 앞의 오른쪽대각
			Cp->m_Queen.x = selected_object_rt.left + path_num;
			Cp->m_Queen.y = selected_object_rt.top + path_num;
			Cp->m_Queen.rt = { selected_object_rt.left + path_num, selected_object_rt.top + path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top + path_num + 75 };

			tmp_rt.x = selected_object_rt.left + path_num;
			tmp_rt.y = selected_object_rt.top + path_num;
			tmp_rt.rt = { selected_object_rt.left + path_num, selected_object_rt.top + path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top + path_num + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Queen.first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;

		}
		else if (selected_object_rt.left - path_num <= x && selected_object_rt.right - path_num >= x && selected_object_rt.top <= y && selected_object_rt.bottom >= y && left_count < queen_left)
		{
			// 왼쪽
			Cp->m_Queen.x = selected_object_rt.left - path_num;
			Cp->m_Queen.y = selected_object_rt.top;
			Cp->m_Queen.rt = { selected_object_rt.left - path_num, selected_object_rt.top, selected_object_rt.left - path_num + 75, selected_object_rt.top + 75 };

			tmp_rt.x = selected_object_rt.left - path_num;
			tmp_rt.y = selected_object_rt.top;
			tmp_rt.rt = { selected_object_rt.left - path_num, selected_object_rt.top, selected_object_rt.left - path_num + 75, selected_object_rt.top + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Queen.first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;

		}
		else if (selected_object_rt.left + path_num <= x && selected_object_rt.right + path_num >= x && selected_object_rt.top <= y && selected_object_rt.bottom >= y && right_count < queen_right)
		{
			// 오른쪽
			Cp->m_Queen.x = selected_object_rt.left + path_num;
			Cp->m_Queen.y = selected_object_rt.top;
			Cp->m_Queen.rt = { selected_object_rt.left + path_num, selected_object_rt.top, selected_object_rt.left + path_num + 75, selected_object_rt.top + 75 };

			tmp_rt.x = selected_object_rt.left + path_num;
			tmp_rt.y = selected_object_rt.top;
			tmp_rt.rt = { selected_object_rt.left + path_num, selected_object_rt.top, selected_object_rt.left + path_num + 75, selected_object_rt.top + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Queen.first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;

		}
		else if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top - path_num <= y && selected_object_rt.bottom - path_num >= y && back_count < queen_back)
		{
			// 뒤
			Cp->m_Queen.x = selected_object_rt.left;
			Cp->m_Queen.y = selected_object_rt.top - path_num;
			Cp->m_Queen.rt = { selected_object_rt.left, selected_object_rt.top - path_num, selected_object_rt.left + 75, selected_object_rt.top - path_num + 75 };

			tmp_rt.x = selected_object_rt.left;
			tmp_rt.y = selected_object_rt.top - path_num;
			tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top - path_num, selected_object_rt.left + 75, selected_object_rt.top - path_num + 75 };

			select_num = 0;
			my_turn = FALSE;
			Cp->m_Queen.first_move = TRUE;
			who_is_moved = clicked_object_num;
			return TRUE;

		}
		else if (selected_object_rt.left - path_num <= x && selected_object_rt.right - path_num >= x && selected_object_rt.top - path_num <= y && selected_object_rt.bottom - path_num >= y && back_left_count < queen_back_left_diagonal)
		{
		// 뒤의 왼쪽대각
		Cp->m_Queen.x = selected_object_rt.left - path_num;
		Cp->m_Queen.y = selected_object_rt.top - path_num;
		Cp->m_Queen.rt = { selected_object_rt.left - path_num, selected_object_rt.top - path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top - path_num + 75 };

		tmp_rt.x = selected_object_rt.left - path_num;
		tmp_rt.y = selected_object_rt.top - path_num;
		tmp_rt.rt = { selected_object_rt.left - path_num, selected_object_rt.top - path_num, selected_object_rt.left - path_num + 75, selected_object_rt.top - path_num + 75 };

		select_num = 0;
		my_turn = FALSE;
		Cp->m_Queen.first_move = TRUE;
		who_is_moved = clicked_object_num;
		return TRUE;

		}
		else if (selected_object_rt.left + path_num <= x && selected_object_rt.right + path_num >= x && selected_object_rt.top - path_num <= y && selected_object_rt.bottom - path_num >= y && back_right_count < queen_back_right_diagonal)
		{
		// 뒤의 오른쪽대각
		Cp->m_Queen.x = selected_object_rt.left + path_num;
		Cp->m_Queen.y = selected_object_rt.top - path_num;
		Cp->m_Queen.rt = { selected_object_rt.left + path_num, selected_object_rt.top - path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top - path_num + 75 };

		tmp_rt.x = selected_object_rt.left + path_num;
		tmp_rt.y = selected_object_rt.top - path_num;
		tmp_rt.rt = { selected_object_rt.left + path_num, selected_object_rt.top - path_num, selected_object_rt.left + path_num + 75, selected_object_rt.top - path_num + 75 };

		select_num = 0;
		my_turn = FALSE;
		Cp->m_Queen.first_move = TRUE;
		who_is_moved = clicked_object_num;
		return TRUE;

		}
		else
		{
			path_num += 75;
			count++;
			front_count++;
			back_count++;
			left_count++;
			right_count++;
			front_left_count++;
			front_right_count++;
			back_left_count++;
			back_right_count++;
		}
	}
    }

	else if (select_num == SELECT_KING && m_player_num == 0 && my_turn == TRUE) // 킹
	{
		if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top - 75 <= y && selected_object_rt.bottom - 75 >= y && king_front != TRUE)
		{
			Cp->m_King.x = selected_object_rt.left;
			Cp->m_King.y = selected_object_rt.top - 75;
			Cp->m_King.rt = { selected_object_rt.left, selected_object_rt.top - 75, selected_object_rt.left + 75, selected_object_rt.top - 75 + 75 };

			tmp_rt.x = selected_object_rt.left;
			tmp_rt.y = selected_object_rt.top - 75;
			tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top - 75, selected_object_rt.left + 75, selected_object_rt.top - 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left - 75 <= x && selected_object_rt.right - 75 >= x && selected_object_rt.top - 75 <= y && selected_object_rt.bottom - 75 >= y && king_front_left_diagonal != TRUE)
		{
			Cp->m_King.x = selected_object_rt.left - 75;
			Cp->m_King.y = selected_object_rt.top - 75;
			Cp->m_King.rt = { selected_object_rt.left - 75, selected_object_rt.top - 75, selected_object_rt.left - 75 + 75, selected_object_rt.top - 75 + 75 };

			tmp_rt.x = selected_object_rt.left - 75;
			tmp_rt.y = selected_object_rt.top - 75;
			tmp_rt.rt = { selected_object_rt.left - 75, selected_object_rt.top - 75, selected_object_rt.left - 75 + 75, selected_object_rt.top - 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left + 75 <= x && selected_object_rt.right + 75 >= x && selected_object_rt.top - 75 <= y && selected_object_rt.bottom - 75 >= y && king_front_right_diagonal != TRUE)
		{
			Cp->m_King.x = selected_object_rt.left + 75;
			Cp->m_King.y = selected_object_rt.top - 75;
			Cp->m_King.rt = { selected_object_rt.left + 75, selected_object_rt.top - 75, selected_object_rt.left + 75 + 75, selected_object_rt.top - 75 + 75 };

			tmp_rt.x = selected_object_rt.left + 75;
			tmp_rt.y = selected_object_rt.top - 75;
			tmp_rt.rt = { selected_object_rt.left + 75, selected_object_rt.top - 75, selected_object_rt.left + 75 + 75, selected_object_rt.top - 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left - 75 <= x && selected_object_rt.right - 75 >= x && selected_object_rt.top <= y && selected_object_rt.bottom >= y && king_left != TRUE)
		{
			Cp->m_King.x = selected_object_rt.left - 75;
			Cp->m_King.y = selected_object_rt.top;
			Cp->m_King.rt = { selected_object_rt.left - 75, selected_object_rt.top, selected_object_rt.left - 75 + 75, selected_object_rt.top + 75 };

			tmp_rt.x = selected_object_rt.left - 75;
			tmp_rt.y = selected_object_rt.top;
			tmp_rt.rt = { selected_object_rt.left - 75, selected_object_rt.top, selected_object_rt.left - 75 + 75, selected_object_rt.top + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left + 75 <= x && selected_object_rt.right + 75 >= x && selected_object_rt.top <= y && selected_object_rt.bottom >= y && king_right != TRUE)
		{
			Cp->m_King.x = selected_object_rt.left + 75;
			Cp->m_King.y = selected_object_rt.top;
			Cp->m_King.rt = { selected_object_rt.left + 75, selected_object_rt.top, selected_object_rt.left + 75 + 75, selected_object_rt.top + 75 };

			tmp_rt.x = selected_object_rt.left + 75;
			tmp_rt.y = selected_object_rt.top;
			tmp_rt.rt = { selected_object_rt.left + 75, selected_object_rt.top, selected_object_rt.left + 75 + 75, selected_object_rt.top + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top + 75 <= y && selected_object_rt.bottom + 75 >= y && king_back != TRUE)
		{
			Cp->m_King.x = selected_object_rt.left;
			Cp->m_King.y = selected_object_rt.top + 75;
			Cp->m_King.rt = { selected_object_rt.left, selected_object_rt.top + 75, selected_object_rt.left + 75, selected_object_rt.top + 75 + 75 };

			tmp_rt.x = selected_object_rt.left;
			tmp_rt.y = selected_object_rt.top + 75;
			tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top + 75, selected_object_rt.left + 75, selected_object_rt.top + 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left - 75 <= x && selected_object_rt.right - 75 >= x && selected_object_rt.top + 75 <= y && selected_object_rt.bottom + 75 >= y && king_back_left_diagonal != TRUE)
		{
			Cp->m_King.x = selected_object_rt.left - 75;
			Cp->m_King.y = selected_object_rt.top + 75;
			Cp->m_King.rt = { selected_object_rt.left - 75, selected_object_rt.top + 75, selected_object_rt.left - 75 + 75, selected_object_rt.top + 75 + 75 };

			tmp_rt.x = selected_object_rt.left - 75;
			tmp_rt.y = selected_object_rt.top + 75;
			tmp_rt.rt = { selected_object_rt.left - 75, selected_object_rt.top + 75, selected_object_rt.left - 75 + 75, selected_object_rt.top + 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left + 75 <= x && selected_object_rt.right + 75 >= x && selected_object_rt.top + 75 <= y && selected_object_rt.bottom + 75 >= y && king_back_right_diagonal != TRUE)
		{
		Cp->m_King.x = selected_object_rt.left + 75;
		Cp->m_King.y = selected_object_rt.top + 75;
		Cp->m_King.rt = { selected_object_rt.left + 75, selected_object_rt.top + 75, selected_object_rt.left + 75 + 75, selected_object_rt.top + 75 + 75 };

		tmp_rt.x = selected_object_rt.left + 75;
		tmp_rt.y = selected_object_rt.top + 75;
		tmp_rt.rt = { selected_object_rt.left + 75, selected_object_rt.top + 75, selected_object_rt.left + 75 + 75, selected_object_rt.top + 75 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
		}
	}

	else if (select_num == SELECT_KING && m_player_num == 1 && my_turn == TRUE) // 킹
	{
		if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top + 75 <= y && selected_object_rt.bottom + 75 >= y && king_front != TRUE)
		{
			Cp->m_King.x = selected_object_rt.left;
			Cp->m_King.y = selected_object_rt.top + 75;
			Cp->m_King.rt = { selected_object_rt.left, selected_object_rt.top + 75, selected_object_rt.left + 75, selected_object_rt.top + 75 + 75 };

			tmp_rt.x = selected_object_rt.left;
			tmp_rt.y = selected_object_rt.top + 75;
			tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top + 75, selected_object_rt.left + 75, selected_object_rt.top + 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left - 75 <= x && selected_object_rt.right - 75 >= x && selected_object_rt.top + 75 <= y && selected_object_rt.bottom + 75 >= y && king_front_left_diagonal != TRUE)
		{
			Cp->m_King.x = selected_object_rt.left - 75;
			Cp->m_King.y = selected_object_rt.top + 75;
			Cp->m_King.rt = { selected_object_rt.left - 75, selected_object_rt.top + 75, selected_object_rt.left - 75 + 75, selected_object_rt.top + 75 + 75 };

			tmp_rt.x = selected_object_rt.left - 75;
			tmp_rt.y = selected_object_rt.top + 75;
			tmp_rt.rt = { selected_object_rt.left - 75, selected_object_rt.top + 75, selected_object_rt.left - 75 + 75, selected_object_rt.top + 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left + 75 <= x && selected_object_rt.right + 75 >= x && selected_object_rt.top + 75 <= y && selected_object_rt.bottom + 75 >= y && king_front_right_diagonal != TRUE)
		{
			Cp->m_King.x = selected_object_rt.left + 75;
			Cp->m_King.y = selected_object_rt.top + 75;
			Cp->m_King.rt = { selected_object_rt.left + 75, selected_object_rt.top + 75, selected_object_rt.left + 75 + 75, selected_object_rt.top + 75 + 75 };

			tmp_rt.x = selected_object_rt.left + 75;
			tmp_rt.y = selected_object_rt.top + 75;
			tmp_rt.rt = { selected_object_rt.left + 75, selected_object_rt.top + 75, selected_object_rt.left + 75 + 75, selected_object_rt.top + 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left - 75 <= x && selected_object_rt.right - 75 >= x && selected_object_rt.top <= y && selected_object_rt.bottom >= y && king_left != TRUE)
		{
			Cp->m_King.x = selected_object_rt.left - 75;
			Cp->m_King.y = selected_object_rt.top;
			Cp->m_King.rt = { selected_object_rt.left - 75, selected_object_rt.top, selected_object_rt.left - 75 + 75, selected_object_rt.top + 75 };

			tmp_rt.x = selected_object_rt.left - 75;
			tmp_rt.y = selected_object_rt.top;
			tmp_rt.rt = { selected_object_rt.left - 75, selected_object_rt.top, selected_object_rt.left - 75 + 75, selected_object_rt.top + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left + 75 <= x && selected_object_rt.right + 75 >= x && selected_object_rt.top <= y && selected_object_rt.bottom >= y && king_right != TRUE)
		{
			Cp->m_King.x = selected_object_rt.left + 75;
			Cp->m_King.y = selected_object_rt.top;
			Cp->m_King.rt = { selected_object_rt.left + 75, selected_object_rt.top, selected_object_rt.left + 75 + 75, selected_object_rt.top + 75 };

			tmp_rt.x = selected_object_rt.left + 75;
			tmp_rt.y = selected_object_rt.top;
			tmp_rt.rt = { selected_object_rt.left + 75, selected_object_rt.top, selected_object_rt.left + 75 + 75, selected_object_rt.top + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left <= x && selected_object_rt.right >= x && selected_object_rt.top - 75 <= y && selected_object_rt.bottom - 75 >= y && king_back != TRUE)
		{
			Cp->m_King.x = selected_object_rt.left;
			Cp->m_King.y = selected_object_rt.top - 75;
			Cp->m_King.rt = { selected_object_rt.left, selected_object_rt.top - 75, selected_object_rt.left + 75, selected_object_rt.top - 75 + 75 };

			tmp_rt.x = selected_object_rt.left;
			tmp_rt.y = selected_object_rt.top - 75;
			tmp_rt.rt = { selected_object_rt.left, selected_object_rt.top - 75, selected_object_rt.left + 75, selected_object_rt.top - 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left - 75 <= x && selected_object_rt.right - 75 >= x && selected_object_rt.top - 75 <= y && selected_object_rt.bottom - 75 >= y && king_back_left_diagonal != TRUE)
		{
			Cp->m_King.x = selected_object_rt.left - 75;
			Cp->m_King.y = selected_object_rt.top - 75;
			Cp->m_King.rt = { selected_object_rt.left - 75, selected_object_rt.top - 75, selected_object_rt.left - 75 + 75, selected_object_rt.top - 75 + 75 };

			tmp_rt.x = selected_object_rt.left - 75;
			tmp_rt.y = selected_object_rt.top - 75;
			tmp_rt.rt = { selected_object_rt.left - 75, selected_object_rt.top - 75, selected_object_rt.left - 75 + 75, selected_object_rt.top - 75 + 75 };

			select_num = 0;
			my_turn = FALSE;
			who_is_moved = clicked_object_num;
			return TRUE;
		}
		else if (selected_object_rt.left + 75 <= x && selected_object_rt.right + 75 >= x && selected_object_rt.top - 75 <= y && selected_object_rt.bottom - 75 >= y && king_back_right_diagonal != TRUE)
		{
		Cp->m_King.x = selected_object_rt.left + 75;
		Cp->m_King.y = selected_object_rt.top - 75;
		Cp->m_King.rt = { selected_object_rt.left + 75, selected_object_rt.top - 75, selected_object_rt.left + 75 + 75, selected_object_rt.top - 75 + 75 };

		tmp_rt.x = selected_object_rt.left + 75;
		tmp_rt.y = selected_object_rt.top - 75;
		tmp_rt.rt = { selected_object_rt.left + 75, selected_object_rt.top - 75, selected_object_rt.left + 75 + 75, selected_object_rt.top - 75 + 75 };

		select_num = 0;
		my_turn = FALSE;
		who_is_moved = clicked_object_num;
		return TRUE;
		}
    }


	who_is_moved = -1;
	tmp_rt.x = -1;
	tmp_rt.y = -1;
	return FALSE;
}

void Player::Click_Check(HDC hdc, int player_num, int x, int y)
{
	// 말선택후 이동가능한 경로를 설정하는 함수

	if (my_turn == TRUE)
	{
		for (int i = 0; i < 8; i++)
		{
			if (Cp->m_Pawn[i].rt.left <= x && Cp->m_Pawn[i].rt.right >= x && Cp->m_Pawn[i].rt.top <= y && Cp->m_Pawn[i].rt.bottom >= y && Cp->m_Pawn[i].status == ALIVE) // 클릭한 말이 범위안에 있을때
			{
				// pawn
				select_num = SELECT_PAWN;
				select_what = SELECT_PAWN; // 게임시스템에서 사용하기 위해만듬
				clicked_pos_x = Cp->m_Pawn[i].rt.left;
				clicked_pos_y = Cp->m_Pawn[i].rt.top;
				clicked_object_num = i;
				selected_object_rt = { Cp->m_Pawn[i].rt.left, Cp->m_Pawn[i].rt.top, Cp->m_Pawn[i].rt.right, Cp->m_Pawn[i].rt.bottom };
				break;
			}
			else if (i < 2 && Cp->m_Rook[i].rt.left <= x && Cp->m_Rook[i].rt.right >= x && Cp->m_Rook[i].rt.top <= y && Cp->m_Rook[i].rt.bottom >= y && Cp->m_Rook[i].status == ALIVE)
			{
				// rook
				select_num = SELECT_ROOK;
				select_what = SELECT_ROOK;
				clicked_pos_x = Cp->m_Rook[i].rt.left;
				clicked_pos_y = Cp->m_Rook[i].rt.top;
				clicked_object_num = i;
				selected_object_rt = { Cp->m_Rook[i].rt.left, Cp->m_Rook[i].rt.top, Cp->m_Rook[i].rt.right, Cp->m_Rook[i].rt.bottom };
				break;
			}
			else if (i < 2 && Cp->m_Knight[i].rt.left <= x && Cp->m_Knight[i].rt.right >= x && Cp->m_Knight[i].rt.top <= y && Cp->m_Knight[i].rt.bottom >= y && Cp->m_Knight[i].status == ALIVE)
			{
				// knight
				select_num = SELECT_KNIGHT;
				select_what = SELECT_KNIGHT;
				clicked_pos_x = Cp->m_Knight[i].rt.left;
				clicked_pos_y = Cp->m_Knight[i].rt.top;
				clicked_object_num = i;
				selected_object_rt = { Cp->m_Knight[i].rt.left, Cp->m_Knight[i].rt.top, Cp->m_Knight[i].rt.right, Cp->m_Knight[i].rt.bottom };
				break;
			}
			else if (i < 2 && Cp->m_Bishop[i].rt.left <= x && Cp->m_Bishop[i].rt.right >= x && Cp->m_Bishop[i].rt.top <= y && Cp->m_Bishop[i].rt.bottom >= y && Cp->m_Bishop[i].status == ALIVE)
			{
				// bishop
				select_num = SELECT_BISHOP;
				select_what = SELECT_BISHOP;
				clicked_pos_x = Cp->m_Bishop[i].rt.left;
				clicked_pos_y = Cp->m_Bishop[i].rt.top;
				clicked_object_num = i;
				selected_object_rt = { Cp->m_Bishop[i].rt.left, Cp->m_Bishop[i].rt.top, Cp->m_Bishop[i].rt.right, Cp->m_Bishop[i].rt.bottom };
				break;
			}
			else if (i < 1 && Cp->m_Queen.rt.left <= x && Cp->m_Queen.rt.right >= x && Cp->m_Queen.rt.top <= y && Cp->m_Queen.rt.bottom >= y && Cp->m_Queen.status == ALIVE)
			{
				select_num = SELECT_QUEEN;
				select_what = SELECT_QUEEN;
				clicked_pos_x = Cp->m_Queen.rt.left;
				clicked_pos_y = Cp->m_Queen.rt.top;
				clicked_object_num = i;
				selected_object_rt = { Cp->m_Queen.rt.left, Cp->m_Queen.rt.top, Cp->m_Queen.rt.right, Cp->m_Queen.rt.bottom };
				break;
			}
			else if (i < 1 && Cp->m_King.rt.left <= x && Cp->m_King.rt.right >= x && Cp->m_King.rt.top <= y && Cp->m_King.rt.bottom >= y && Cp->m_King.status == ALIVE)
			{
				select_num = SELECT_KING;
				select_what = SELECT_KING;
				clicked_pos_x = Cp->m_King.rt.left;
				clicked_pos_y = Cp->m_King.rt.top;
				clicked_object_num = i;
				selected_object_rt = { Cp->m_King.rt.left, Cp->m_King.rt.top, Cp->m_King.rt.right, Cp->m_King.rt.bottom };
				break;
			}
			else
			{
				clicked_object_num = -1;
				select_num = 0;
			}
		}
	}
}

void Player::Player_Die_Check(int piece_num, int dead_num)
{
	if (piece_num == 10) // pawn
	{
		Cp->m_Pawn[dead_num].status = DEAD;
	}
	else if (piece_num == 20) // rook
	{
		Cp->m_Rook[dead_num].status = DEAD;
	}
	else if (piece_num == 30)
	{
		Cp->m_Knight[dead_num].status = DEAD;
	}
	else if (piece_num == 40)
	{
		Cp->m_Bishop[dead_num].status = DEAD;
	}
	else if (piece_num == 50)
	{
		Cp->m_Queen.status = DEAD;
	}
	else if (piece_num == 60)
	{
		Cp->m_King.status = DEAD;
	}
}

Player::~Player()
{

}