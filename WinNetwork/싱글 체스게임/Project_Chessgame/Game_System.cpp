#include "Game_System.h"

Game_System::Game_System()
{

}

void Game_System::Init_System(HDC hdc, HINSTANCE hinst)
{
	int trigger = FALSE;
	int count = 0;
	int x = 0;
	int y = 0;

	m_bd = new Board[BOARD_MAX];

	for (int i = 0; i < BOARD_MAX; i++)
	{
		if (count == 16)
			count = 0;
		if (trigger == FALSE)
		{
			if (count >= 8)
			{
				m_bd[i].board_num = 20;
				trigger = TRUE;
			}
			else
			{
				m_bd[i].board_num = 10;
				trigger = TRUE;
			}
		}
		else if (trigger == TRUE)
		{
			if (count >= 8)
			{
				m_bd[i].board_num = 10;
				trigger = FALSE;
			}
			else
			{
				m_bd[i].board_num = 20;
				trigger = FALSE;
			}
		}
		count++;
	}

	for (int j = 0; j < BOARD_MAX; j++)
	{
		if (x == 8)
		{
			x = 0;
			y++;
		}
		m_bd[j].Board_Init(hdc, hinst, x, y);
		x++;
	}

	m_pr = new Player[2];
	m_pr[0].m_player_num = 0; // 백
	m_pr[1].m_player_num = 1; // 흑
	
	for (int i = 0; i < 2; i++)
		m_pr[i].Init_Player(hdc, m_pr[i].m_player_num);

	Set_All_Pawn_Pos();
}

void Game_System::Draw(HDC hdc)
{
	int x = 0;
	int y = 0;
	int count = 0;

	for (int i = 0; i < BOARD_MAX; i++)
	{
		if (count == 8)
		{
			x = 0;
			y += 75;
			count = 0;
		}
		m_bd[i].Board_Draw(hdc, x, y);
		x += 75;
		count++;
	}


	for (int i = 0; i < 2; i++)
		m_pr[i].Player_Pieces_Draw(hdc);

}

int Game_System::Click(HDC hdc, int x, int y)
{
	int trigger = 0;
	int result = 0;

	for (int i = 0; i < 2; i++)
	{
		result = Game_Over_Check();
		if (result == 10)
			return 10;
		else if (result == 20)
			return 20;

		trigger = m_pr[i].Move_Check(hdc, x, y);
		Piece_Rules(i);
		All_Piece_Pos(i); // 이동했을때 좌표저장

		if (m_pr[i].who_is_moved != -1)
		Die_Check(i);

		m_pr[i].select_what = 0;

		m_pr[i].Click_Check(hdc, i, x, y);
		Pawn_Check(i);
		Rook_Check(i);
		Knight_Check(i);
		Bishop_Check(i);
		Queen_Check(i);
		King_Check(i);

		if (i == 0 && trigger == TRUE)
		{
			if (m_pr[0].my_turn == FALSE)
			{
				m_pr[1].my_turn = TRUE;
			}
		}
		else if (i == 1 && trigger == TRUE)
		{
			if (m_pr[1].my_turn == FALSE)
			{
				m_pr[0].my_turn = TRUE;
			}
		}
		trigger = 0;
	}

	return 0;
}

void Game_System::Set_All_Pawn_Pos()
{
	int num_x = 0;

	for (int i = 0; i < 16; i++) // pawn
	{
		m_All_Pawn[i].status = ALIVE;
		if (i < 8)
			m_All_Pawn[i].player_num = 0;
		else
			m_All_Pawn[i].player_num = 1;

		if (i == 0)
		{
			m_All_Pawn[i].x = 0;
			m_All_Pawn[i].y = 450;
			m_All_Pawn[i].rt = { 0, 450, 75, 525 };
		}
		else if (i < 8)
		{
			num_x += 75;
			m_All_Pawn[i].x += num_x;
			m_All_Pawn[i].y = 450;
			m_All_Pawn[i].rt = { num_x, 450, num_x + 75, 525 };
		}
		else if (i == 8)
		{
			num_x = 0;
			m_All_Pawn[i].x = 0;
			m_All_Pawn[i].y = 75;
			m_All_Pawn[i].rt = { 0, 75, 75, 150 };
		}
		else if (i < 16)
		{
			num_x += 75;
			m_All_Pawn[i].x += num_x;
			m_All_Pawn[i].y = 75;
			m_All_Pawn[i].rt = { num_x, 75, num_x + 75, 150 };
		}

	}

	for (int i = 0; i < 4; i++) // rook
	{
		m_All_Rook[i].status = ALIVE;
		if (i < 2)
			m_All_Rook[i].player_num = 0;
		else
			m_All_Rook[i].player_num = 1;
	}
	m_All_Rook[0].x = 0;
	m_All_Rook[0].y = 525;
	m_All_Rook[0].rt = { 0, 525, 75, 600 };
	m_All_Rook[1].x = 525;
	m_All_Rook[1].y = 525;
	m_All_Rook[1].rt = { 525, 525, 600, 600 };
	m_All_Rook[2].x = 0;
	m_All_Rook[2].y = 0;
	m_All_Rook[2].rt = { 0, 0, 75, 75 };
	m_All_Rook[3].x = 525;
	m_All_Rook[3].y = 0;
	m_All_Rook[3].rt = { 525, 0, 600, 75 };

	for (int i = 0; i < 4; i++) // knight
	{
		m_All_Knight[i].status = ALIVE;
		if (i < 2)
			m_All_Knight[i].player_num = 0;
		else
			m_All_Knight[i].player_num = 1;
	}
	m_All_Knight[0].x = 75;
	m_All_Knight[0].y = 525;
	m_All_Knight[0].rt = { 75, 525, 150, 600 };
	m_All_Knight[1].x = 450;
	m_All_Knight[1].y = 525;
	m_All_Knight[1].rt = { 450, 525, 525, 600 };
	m_All_Knight[2].x = 75;
	m_All_Knight[2].y = 0;
	m_All_Knight[2].rt = { 75, 0, 150, 75 };
	m_All_Knight[3].x = 450;
	m_All_Knight[3].y = 0;
	m_All_Knight[3].rt = { 450, 0, 525, 75 };

	for (int i = 0; i < 4; i++)
	{
		m_All_Bishop[i].status = ALIVE;
		if (i < 2)
			m_All_Bishop[i].player_num = 0;
		else
			m_All_Bishop[i].player_num = 1;
	}
	m_All_Bishop[0].x = 150;
	m_All_Bishop[0].y = 525;
	m_All_Bishop[0].rt = { 150, 525, 225, 600 };
	m_All_Bishop[1].x = 375;
	m_All_Bishop[1].y = 525;
	m_All_Bishop[1].rt = { 375, 525, 450, 600 };
	m_All_Bishop[2].x = 150;
	m_All_Bishop[2].y = 0;
	m_All_Bishop[2].rt = { 150, 0, 225, 75 };
	m_All_Bishop[3].x = 375;
	m_All_Bishop[3].y = 0;
	m_All_Bishop[3].rt = { 375, 0, 450, 75 };

	for (int i = 0; i < 2; i++)
	{
		m_All_Queen[i].status = ALIVE;
		if (i == 0)
		    m_All_Queen[i].player_num = 0;
		else
			m_All_Queen[i].player_num = 1;
	}
	m_All_Queen[0].x = 225;
	m_All_Queen[0].y = 525;
	m_All_Queen[0].rt = { 225, 525, 300, 600 };
	m_All_Queen[1].x = 225;
	m_All_Queen[1].y = 0;
	m_All_Queen[1].rt = { 225, 0, 300, 75 };

	for (int i = 0; i < 2; i++)
	{
		m_All_King[i].status = ALIVE;
		if (i == 0)
			m_All_King[i].player_num = 0;
		else
			m_All_King[i].player_num = 1;
	}
	m_All_King[0].x = 300;
	m_All_King[0].y = 525;
	m_All_King[0].rt = { 300, 525, 375, 600 };
	m_All_King[1].x = 300;
	m_All_King[1].y = 0;
	m_All_King[1].rt = { 300, 0, 375, 75 };
}

void Game_System::All_Piece_Pos(int player_num)
{
	if (m_pr[player_num].tmp_rt.x != -1)
	{
		if (player_num == 0)
		{
			if (m_pr[player_num].select_what == SELECT_PAWN)
			{
				m_All_Pawn[m_pr[player_num].clicked_object_num].x = m_pr[player_num].tmp_rt.x;
				m_All_Pawn[m_pr[player_num].clicked_object_num].y = m_pr[player_num].tmp_rt.y;
				m_All_Pawn[m_pr[player_num].clicked_object_num].rt = m_pr[player_num].tmp_rt.rt;
			}
			else if (m_pr[player_num].select_what == SELECT_ROOK)
			{
				m_All_Rook[m_pr[player_num].clicked_object_num].x = m_pr[player_num].tmp_rt.x;
				m_All_Rook[m_pr[player_num].clicked_object_num].y = m_pr[player_num].tmp_rt.y;
				m_All_Rook[m_pr[player_num].clicked_object_num].rt = m_pr[player_num].tmp_rt.rt;
			}
			else if (m_pr[player_num].select_what == SELECT_KNIGHT)
			{
				m_All_Knight[m_pr[player_num].clicked_object_num].x = m_pr[player_num].tmp_rt.x;
				m_All_Knight[m_pr[player_num].clicked_object_num].y = m_pr[player_num].tmp_rt.y;
				m_All_Knight[m_pr[player_num].clicked_object_num].rt = m_pr[player_num].tmp_rt.rt;
			}
			else if (m_pr[player_num].select_what == SELECT_BISHOP)
			{
				m_All_Bishop[m_pr[player_num].clicked_object_num].x = m_pr[player_num].tmp_rt.x;
				m_All_Bishop[m_pr[player_num].clicked_object_num].y = m_pr[player_num].tmp_rt.y;
				m_All_Bishop[m_pr[player_num].clicked_object_num].rt = m_pr[player_num].tmp_rt.rt;
			}
			else if (m_pr[player_num].select_what == SELECT_QUEEN)
			{
				m_All_Queen[m_pr[player_num].clicked_object_num].x = m_pr[player_num].tmp_rt.x;
				m_All_Queen[m_pr[player_num].clicked_object_num].y = m_pr[player_num].tmp_rt.y;
				m_All_Queen[m_pr[player_num].clicked_object_num].rt = m_pr[player_num].tmp_rt.rt;
			}
			else if (m_pr[player_num].select_what == SELECT_KING)
			{
				m_All_King[m_pr[player_num].clicked_object_num].x = m_pr[player_num].tmp_rt.x;
				m_All_King[m_pr[player_num].clicked_object_num].y = m_pr[player_num].tmp_rt.y;
				m_All_King[m_pr[player_num].clicked_object_num].rt = m_pr[player_num].tmp_rt.rt;
			}
		}

		else if (player_num == 1)
		{
			if (m_pr[player_num].select_what == SELECT_PAWN)
			{
				m_All_Pawn[m_pr[player_num].clicked_object_num + 8].x = m_pr[player_num].tmp_rt.x;
				m_All_Pawn[m_pr[player_num].clicked_object_num + 8].y = m_pr[player_num].tmp_rt.y;
				m_All_Pawn[m_pr[player_num].clicked_object_num + 8].rt = m_pr[player_num].tmp_rt.rt;
			}
			else if (m_pr[player_num].select_what == SELECT_ROOK)
			{
				m_All_Rook[m_pr[player_num].clicked_object_num + 2].x = m_pr[player_num].tmp_rt.x;
				m_All_Rook[m_pr[player_num].clicked_object_num + 2].y = m_pr[player_num].tmp_rt.y;
				m_All_Rook[m_pr[player_num].clicked_object_num + 2].rt = m_pr[player_num].tmp_rt.rt;
			}
			else if (m_pr[player_num].select_what == SELECT_KNIGHT)
			{
				m_All_Knight[m_pr[player_num].clicked_object_num + 2].x = m_pr[player_num].tmp_rt.x;
				m_All_Knight[m_pr[player_num].clicked_object_num + 2].y = m_pr[player_num].tmp_rt.y;
				m_All_Knight[m_pr[player_num].clicked_object_num + 2].rt = m_pr[player_num].tmp_rt.rt;
			}
			else if (m_pr[player_num].select_what == SELECT_BISHOP)
			{
				m_All_Bishop[m_pr[player_num].clicked_object_num + 2].x = m_pr[player_num].tmp_rt.x;
				m_All_Bishop[m_pr[player_num].clicked_object_num + 2].y = m_pr[player_num].tmp_rt.y;
				m_All_Bishop[m_pr[player_num].clicked_object_num + 2].rt = m_pr[player_num].tmp_rt.rt;
			}
			else if (m_pr[player_num].select_what == SELECT_QUEEN)
			{
				m_All_Queen[m_pr[player_num].clicked_object_num + 1].x = m_pr[player_num].tmp_rt.x;
				m_All_Queen[m_pr[player_num].clicked_object_num + 1].y = m_pr[player_num].tmp_rt.y;
				m_All_Queen[m_pr[player_num].clicked_object_num + 1].rt = m_pr[player_num].tmp_rt.rt;
			}
			else if (m_pr[player_num].select_what == SELECT_KING)
			{
				m_All_King[m_pr[player_num].clicked_object_num + 1].x = m_pr[player_num].tmp_rt.x;
				m_All_King[m_pr[player_num].clicked_object_num + 1].y = m_pr[player_num].tmp_rt.y;
				m_All_King[m_pr[player_num].clicked_object_num + 1].rt = m_pr[player_num].tmp_rt.rt;
			}
		}
	}

	m_pr[player_num].tmp_rt.x = -1;
}

void Game_System::Pawn_Check(int num)
{
	if (m_pr[num].m_player_num == 0 && m_pr[num].clicked_object_num != -1 && m_pr[num].select_what == SELECT_PAWN)
	{
		for (int i = 0; i < 16; i++)
		{
			if (m_All_Pawn[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Pawn[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left && m_All_Pawn[i].status != DEAD ||
				i < 4 && m_All_Rook[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Rook[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left && m_All_Rook[i].status != DEAD ||
				i < 4 && m_All_Knight[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Knight[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left && m_All_Knight[i].status != DEAD ||
				i < 4 && m_All_Bishop[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Bishop[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left && m_All_Bishop[i].status != DEAD ||
				i < 1 && m_All_Queen[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Queen[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left && m_All_Queen[i].status != DEAD ||
				i < 1 && m_All_King[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_King[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left && m_All_King[i].status != DEAD)
			{
				m_pr[num].pawn_front = TRUE;
				m_pr[num].someting = TRUE;
			}
			if (m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 150 && m_All_Pawn[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left && m_All_Pawn[i].status != DEAD ||
				m_All_Rook[i].player_num == 0 && i < 4 && m_All_Rook[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 150 && m_All_Rook[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left && m_All_Rook[i].status != DEAD ||
				m_All_Knight[i].player_num == 0 && i < 4 && m_All_Knight[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 150 && m_All_Knight[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left && m_All_Knight[i].status != DEAD ||
				m_All_Bishop[i].player_num == 0 && i < 4 && m_All_Bishop[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 150 && m_All_Bishop[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left && m_All_Bishop[i].status != DEAD ||
				m_All_Queen[i].player_num == 0 && i < 1 && m_All_Queen[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 150 && m_All_Queen[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left && m_All_Queen[i].status != DEAD ||
				m_All_King[i].player_num == 0 && i < 1 && m_All_King[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 150 && m_All_King[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left && m_All_King[i].status != DEAD)
			{
				m_pr[num].pawn_double_front = TRUE;
				m_pr[num].someting = TRUE;
			}
			if (m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Pawn[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left - 75 && m_All_Pawn[i].status != DEAD ||
				i < 4 && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Rook[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left - 75 && m_All_Rook[i].status != DEAD ||
				i < 4 && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Knight[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left - 75 && m_All_Knight[i].status != DEAD ||
				i < 4 && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Bishop[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left - 75 && m_All_Bishop[i].status != DEAD ||
				i < 2 && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Queen[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left - 75 && m_All_Queen[i].status != DEAD ||
				i < 2 && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_King[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left - 75 && m_All_King[i].status != DEAD)
			{
				m_pr[num].pawn_diagonal1 = TRUE;
				m_pr[num].someting = TRUE;
			}
			if (m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Pawn[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left + 75 && m_All_Pawn[i].status != DEAD ||
				i < 4 && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Rook[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left + 75 && m_All_Rook[i].status != DEAD ||
				i < 4 && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Knight[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left + 75 && m_All_Knight[i].status != DEAD ||
				i < 4 && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Bishop[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left + 75 && m_All_Bishop[i].status != DEAD ||
				i < 2 && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_Queen[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left + 75 && m_All_Queen[i].status != DEAD ||
				i < 2 && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num].rt.top - 75 && m_All_King[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num].rt.left + 75 && m_All_King[i].status != DEAD)
			{
				m_pr[num].pawn_diagonal2 = TRUE;
				m_pr[num].someting = TRUE;
			}
		}

	}
	else if (m_pr[num].m_player_num == 1 && m_pr[num].clicked_object_num != -1 && m_pr[num].select_what == SELECT_PAWN)
	{
		for (int i = 0; i < 16; i++)
		{
			if (m_All_Pawn[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Pawn[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left && m_All_Pawn[i].status != DEAD ||
				i < 4 && m_All_Rook[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Rook[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left && m_All_Rook[i].status != DEAD ||
				i < 4 && m_All_Knight[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Knight[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left && m_All_Knight[i].status != DEAD ||
				i < 4 && m_All_Bishop[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Bishop[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left && m_All_Bishop[i].status != DEAD ||
				i < 2 && m_All_Queen[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Queen[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left && m_All_Queen[i].status != DEAD ||
				i < 2 && m_All_King[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_King[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left && m_All_King[i].status != DEAD)
			{
				m_pr[num].pawn_front = TRUE;
				m_pr[num].someting = TRUE;
			}
			if (m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 150 && m_All_Pawn[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left && m_All_Pawn[i].status != DEAD ||
				m_All_Rook[i].player_num == 1 && i < 4 && m_All_Rook[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 150 && m_All_Rook[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left && m_All_Rook[i].status != DEAD ||
				m_All_Knight[i].player_num == 1 && i < 4 && m_All_Knight[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 150 && m_All_Knight[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left && m_All_Knight[i].status != DEAD ||
				m_All_Bishop[i].player_num == 1 && i < 4 && m_All_Bishop[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 150 && m_All_Bishop[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left && m_All_Bishop[i].status != DEAD ||
				m_All_Queen[i].player_num == 1 && i < 2 && m_All_Queen[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 150 && m_All_Queen[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left && m_All_Queen[i].status != DEAD ||
				m_All_King[i].player_num == 1 && i < 2 && m_All_King[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 150 && m_All_King[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left && m_All_King[i].status != DEAD)
			{
				m_pr[num].pawn_double_front = TRUE;
				m_pr[num].someting = TRUE;
			}
			if (m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Pawn[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left - 75 && m_All_Pawn[i].status != DEAD ||
				m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Rook[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left - 75 && m_All_Rook[i].status != DEAD ||
				m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Knight[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left - 75 && m_All_Knight[i].status != DEAD ||
				m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Bishop[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left - 75 && m_All_Bishop[i].status != DEAD ||
				m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Queen[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left - 75 && m_All_Queen[i].status != DEAD ||
				m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_King[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left - 75 && m_All_King[i].status != DEAD)
			{
				m_pr[num].pawn_diagonal1 = TRUE;
				m_pr[num].someting = TRUE;
			}
			if (m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Pawn[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left + 75 && m_All_Pawn[i].status != DEAD ||
				m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Rook[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left + 75 && m_All_Rook[i].status != DEAD ||
				m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Knight[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left + 75 && m_All_Knight[i].status != DEAD ||
				m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Bishop[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left + 75 && m_All_Bishop[i].status != DEAD ||
				m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_Queen[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left + 75 && m_All_Queen[i].status != DEAD ||
				m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.top + 75 && m_All_King[i].rt.left == m_All_Pawn[m_pr[num].clicked_object_num + 8].rt.left + 75 && m_All_King[i].status != DEAD)
			{
				m_pr[num].pawn_diagonal2 = TRUE;
				m_pr[num].someting = TRUE;
			}
		}
	}

}

void Game_System::Rook_Check(int num)
{
	int i = 0;
	int rt_num = 75;
	// 재귀함수 고려하기
	if (m_pr[num].m_player_num == 0 && m_pr[num].clicked_object_num != -1 && m_pr[num].select_what == SELECT_ROOK)
	{
		while (1) // 앞
		{
			if (i == 16)
			{
				m_pr[0].rook_front++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left || m_All_Rook[m_pr[num].clicked_object_num].rt.top == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				m_All_Rook[m_pr[num].clicked_object_num].rt.top - rt_num == -75 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left || m_All_Rook[m_pr[num].clicked_object_num].rt.top == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				m_All_Rook[m_pr[num].clicked_object_num].rt.top - rt_num == -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left)
			{
				if (m_All_Rook[m_pr[num].clicked_object_num].rt.top != 0) // 출력확인
				m_pr[0].rook_front++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;
		while (1) // 뒤
		{
			if (i == 16)
			{
				m_pr[0].rook_back++;
				i = 0;
				rt_num += 75;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left || m_All_Rook[m_pr[num].clicked_object_num].rt.top == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				m_All_Rook[m_pr[num].clicked_object_num].rt.top + rt_num == 600 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left || m_All_Rook[m_pr[num].clicked_object_num].rt.top == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				m_All_Rook[m_pr[num].clicked_object_num].rt.top + rt_num == 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left)
			{
				if (m_All_Rook[m_pr[num].clicked_object_num].rt.top != 525)
				m_pr[0].rook_back++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;
		while (1) // 왼쪽
		{
			if (i == 16)
			{
				m_pr[0].rook_left++;
				i = 0;
				rt_num += 75;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left - rt_num || m_All_Rook[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left - rt_num ||
				m_All_Rook[m_pr[num].clicked_object_num].rt.left - rt_num == -75 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left - rt_num)
			{
				break;
			}

			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left - rt_num || m_All_Rook[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left - rt_num ||
				m_All_Rook[m_pr[num].clicked_object_num].rt.left - rt_num == -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left - rt_num)
			{
				if (m_All_Rook[m_pr[num].clicked_object_num].rt.left != 0)
					m_pr[0].rook_left++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;
		while (1) // 오른쪽
		{
			if (i == 16)
			{
				m_pr[0].rook_right++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left + rt_num || m_All_Rook[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left + rt_num ||
				m_All_Rook[m_pr[num].clicked_object_num].rt.left + rt_num == 600 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left + rt_num)
			{
				break;
			}

			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left + rt_num || m_All_Rook[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left + rt_num ||
				m_All_Rook[m_pr[num].clicked_object_num].rt.left + rt_num == 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num].rt.top && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num].rt.left + rt_num)
			{
				if (m_All_Rook[m_pr[num].clicked_object_num].rt.left != 525)
					m_pr[0].rook_right++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;
	}

	////////////////////////////////////////

	else if (m_pr[num].m_player_num == 1 && m_pr[num].clicked_object_num != -1 && m_pr[num].select_what == SELECT_ROOK)
	{

		while (1) // 앞
		{
			if (i == 16)
			{
				m_pr[1].rook_front++;
				i = 0;
				rt_num += 75;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left || m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top + rt_num == 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left || m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top + rt_num == 600 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left)
			{
				if (m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top != 525)
					m_pr[1].rook_front++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 뒤
		{
			if (i == 16)
			{
				m_pr[1].rook_back++;
				i = 0;
				rt_num += 75;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left || m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top - rt_num == -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left || m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top - rt_num == -75 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left)
			{
				if (m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top != 0)
					m_pr[1].rook_back++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 왼쪽
		{
			if (i == 16)
			{
				m_pr[1].rook_left++;
				i = 0;
				rt_num += 75;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left - rt_num || m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left - rt_num == -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left - rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left - rt_num || m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left - rt_num == -75 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left - rt_num)
			{
				if (m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left != 0)
					m_pr[1].rook_left++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;
		
		while (1) // 오른쪽
		{
			if (i == 16)
			{
				m_pr[1].rook_right++;
				i = 0;
				rt_num += 75;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left + rt_num || m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left + rt_num == 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left + rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Pawn[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left + rt_num || m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Knight[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Rook[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left + rt_num == 600 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Bishop[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_Queen[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.top && m_All_King[i].rt.left == m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left + rt_num)
			{
				if (m_All_Rook[m_pr[num].clicked_object_num + 2].rt.left != 525)
					m_pr[1].rook_right++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;
	}
}

void Game_System::Knight_Check(int num)
{
	if (m_pr[num].m_player_num == 0 && m_pr[num].clicked_object_num != -1 && m_pr[num].select_what == SELECT_KNIGHT)
	{
		for (int i = 0; i < 16; i++)
		{
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 150 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 150 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 150 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num].rt.top - 150 <= -75 || m_All_Knight[m_pr[num].clicked_object_num].rt.left - 75 <= -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 150 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 150 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Queen[i].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 150 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 75 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_front_left_diagonal1 = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 150 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 150 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 150 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num].rt.top - 75 <= -75 || m_All_Knight[m_pr[num].clicked_object_num].rt.left - 150 <= -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 150 == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 150 == m_All_Queen[i].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 75 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 150 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_front_left_diagonal2 = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 150 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 150 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 150 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num].rt.top - 150 <= -75 || m_All_Knight[m_pr[num].clicked_object_num].rt.left + 75 >= 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 150 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 150 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Queen[i].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 150 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 75 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_front_right_diagonal1 = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 150 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 150 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 150 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num].rt.top - 75 <= -75 || m_All_Knight[m_pr[num].clicked_object_num].rt.left + 150 >= 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 150 == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 150 == m_All_Queen[i].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top - 75 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 150 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_front_right_diagonal2 = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 150 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 150 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 150 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num].rt.top + 150 >= 600 || m_All_Knight[m_pr[num].clicked_object_num].rt.left - 75 <= -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 150 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 150 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Queen[i].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 150 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 75 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_back_left_diagonal1 = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 150 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 150 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 150 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num].rt.top + 75 >= 600 || m_All_Knight[m_pr[num].clicked_object_num].rt.left - 150 <= -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 150 == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 150 == m_All_Queen[i].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 75 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left - 150 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_back_left_diagonal2 = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 150 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 150 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 150 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num].rt.top + 150 >= 600 || m_All_Knight[m_pr[num].clicked_object_num].rt.left + 75 >= 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 150 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 150 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Queen[i].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 150 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 75 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_back_right_diagonal1 = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 150 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 150 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 150 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num].rt.top + 75 >= 600 || m_All_Knight[m_pr[num].clicked_object_num].rt.left + 150 >= 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 150 == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 150 == m_All_Queen[i].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_Knight[m_pr[num].clicked_object_num].rt.top + 75 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num].rt.left + 150 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_back_right_diagonal2 = TRUE;
			}
		}
	}

	///////////////////////////////

	if (m_pr[num].m_player_num == 1 && m_pr[num].clicked_object_num != -1 && m_pr[num].select_what == SELECT_KNIGHT)
	{
		for (int i = 0; i < 16; i++)
		{
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 150 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 150 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 150 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 75 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 150 >= 600 || m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 75 <= -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 150 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 75 == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 150 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 75 == m_All_Queen[i].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 150 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 75 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_front_left_diagonal1 = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 75 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 150 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 75 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 150 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 75 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 150 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 75 >= 600 || m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 150 <= -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 75 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 150 == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 75 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 150 == m_All_Queen[i].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 75 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 150 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_front_left_diagonal2 = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 150 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 150 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 150 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 75 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 150 >= 600 || m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 75 >= 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 150 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 75 == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 150 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 75 == m_All_Queen[i].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 150 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 75 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_front_right_diagonal1 = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 75 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 150 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 75 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 150 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 75 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 150 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 75 >= 600 || m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 150 >= 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 75 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 150 == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 75 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 150 == m_All_Queen[i].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top + 75 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 150 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_front_right_diagonal2 = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 150 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 150 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 150 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 75 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 150 <= -75 || m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 75 <= -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 150 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 75 == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 150 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 75 == m_All_Queen[i].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 150 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 75 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_back_left_diagonal1 = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 75 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 150 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 75 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 150 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 75 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 150 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 75 <= -75 || m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 150 <= -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 75 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 150 == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 75 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 150 == m_All_Queen[i].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 75 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left - 150 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_back_left_diagonal2 = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 150 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 150 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 150 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 75 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 150 <= -75 || m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 75 >= 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 150 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 75 == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 150 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 75 == m_All_Queen[i].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 150 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 75 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_back_right_diagonal1 = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 75 == m_All_Pawn[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 150 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 75 == m_All_Rook[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 150 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 75 == m_All_Knight[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 150 == m_All_Knight[i].rt.left ||
				m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 75 <= -75 || m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 150 >= 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 75 == m_All_Bishop[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 150 == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 75 == m_All_Queen[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 150 == m_All_Queen[i].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.top - 75 == m_All_King[i].rt.top && m_All_Knight[m_pr[num].clicked_object_num + 2].rt.left + 150 == m_All_King[i].rt.left)
			{
				m_pr[num].knight_back_right_diagonal2 = TRUE;
			}
		}
	}
}

void Game_System::Bishop_Check(int num)
{
	int i = 0;
	int rt_num = 75;

	if (m_pr[num].m_player_num == 0 && m_pr[num].clicked_object_num != -1 && m_pr[num].select_what == SELECT_BISHOP)
	{
		while (1) // 앞의 왼쪽대각선
		{
			if (i == 16)
			{
				m_pr[0].bishop_front_left_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num || m_All_Bishop[m_pr[num].clicked_object_num].rt.top == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num || m_All_Bishop[m_pr[num].clicked_object_num].rt.top == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num)
			{
				if (m_All_Bishop[m_pr[num].clicked_object_num].rt.top != 0)
					m_pr[0].bishop_front_left_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 앞의 오른쪽대각선
		{
			if (i == 16)
			{
				m_pr[0].bishop_front_right_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num || m_All_Bishop[m_pr[num].clicked_object_num].rt.top == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num || m_All_Bishop[m_pr[num].clicked_object_num].rt.top == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num)
			{
				if (m_All_Bishop[m_pr[num].clicked_object_num].rt.top != 0)
					m_pr[0].bishop_front_right_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 뒤의 왼쪽대각선
		{
			if (i == 16)
			{
				m_pr[0].bishop_back_left_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num || m_All_Bishop[m_pr[num].clicked_object_num].rt.top == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num || m_All_Bishop[m_pr[num].clicked_object_num].rt.top == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left - rt_num)
			{
				if (m_All_Bishop[m_pr[num].clicked_object_num].rt.top != 525)
					m_pr[0].bishop_back_left_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 뒤의 오른쪽대각선
		{
			if (i == 16)
			{
				m_pr[0].bishop_back_right_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num || m_All_Bishop[m_pr[num].clicked_object_num].rt.top == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num || m_All_Bishop[m_pr[num].clicked_object_num].rt.top == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num].rt.left + rt_num)
			{
				if (m_All_Bishop[m_pr[num].clicked_object_num].rt.top != 525)
					m_pr[0].bishop_back_right_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;
	}

	/////////////////////////

	else if (m_pr[num].m_player_num == 1 && m_pr[num].clicked_object_num != -1 && m_pr[num].select_what == SELECT_BISHOP)
	{
		while (1) // 앞의 왼쪽대각선
		{
			if (i == 16)
			{
				m_pr[1].bishop_front_left_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left == 0 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num)
			{
				if (m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top != 525)
					m_pr[1].bishop_front_left_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 앞의 오른쪽대각선
		{
			if (i == 16)
			{
				m_pr[1].bishop_front_right_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left == 525 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top + rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num)
			{
				if (m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top != 525)
					m_pr[1].bishop_front_right_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 뒤의 왼쪽대각선
		{
			if (i == 16)
			{
				m_pr[1].bishop_back_left_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left == 0 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left - rt_num)
			{
				if (m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top != 0)
					m_pr[1].bishop_back_left_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 뒤의 오른쪽대각선
		{
			if (i == 16)
			{
				m_pr[1].bishop_back_right_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num == -75 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num == 600 || m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left == 525 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top - rt_num && m_All_King[i].rt.left == m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.left + rt_num)
			{
				if (m_All_Bishop[m_pr[num].clicked_object_num + 2].rt.top != 0)
					m_pr[1].bishop_back_right_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;
    }
}

void Game_System::Queen_Check(int num)
{
	int i = 0;
	int rt_num = 75;

	if (m_pr[num].m_player_num == 0 && m_pr[num].clicked_object_num != -1 && m_pr[num].select_what == SELECT_QUEEN)
	{
		while (1) // 앞
		{
			if (i == 16)
			{
				m_pr[0].queen_front++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left || m_All_Queen[m_pr[num].clicked_object_num].rt.top == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num == -75 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left || m_All_Queen[m_pr[num].clicked_object_num].rt.top == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num == -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num].rt.top != 0) // 출력확인
					m_pr[0].queen_front++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 앞의 왼쪽대각선
		{
			if (i == 16)
			{
				m_pr[0].queen_front_left_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num || m_All_Queen[m_pr[num].clicked_object_num].rt.top == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num || m_All_Queen[m_pr[num].clicked_object_num].rt.top == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num].rt.top != 0)
					m_pr[0].queen_front_left_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 앞의 오른쪽대각선
		{
			if (i == 16)
			{
				m_pr[0].queen_front_right_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num || m_All_Queen[m_pr[num].clicked_object_num].rt.top == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num || m_All_Queen[m_pr[num].clicked_object_num].rt.top == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top - rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num].rt.top != 0)
					m_pr[0].queen_front_right_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 왼쪽
		{
			if (i == 16)
			{
				m_pr[0].queen_left++;
				i = 0;
				rt_num += 75;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num || m_All_Queen[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num == -75 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num)
			{
				break;
			}

			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num || m_All_Queen[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num == -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num].rt.left != 0)
					m_pr[0].queen_left++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 오른쪽
		{
			if (i == 16)
			{
				m_pr[0].queen_right++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num || m_All_Queen[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num == 600 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num)
			{
				break;
			}

			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num || m_All_Queen[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num == 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num].rt.left != 525)
					m_pr[0].queen_right++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 뒤
		{
			if (i == 16)
			{
				m_pr[0].queen_back++;
				i = 0;
				rt_num += 75;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left || m_All_Queen[m_pr[num].clicked_object_num].rt.top == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num == 600 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left || m_All_Queen[m_pr[num].clicked_object_num].rt.top == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num == 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num].rt.top != 525)
					m_pr[0].queen_back++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 뒤의 왼쪽대각선
		{
			if (i == 16)
			{
				m_pr[0].queen_back_left_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num || m_All_Queen[m_pr[num].clicked_object_num].rt.top == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num || m_All_Queen[m_pr[num].clicked_object_num].rt.top == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left - rt_num)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num].rt.top != 525)
					m_pr[0].queen_back_left_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 뒤의 오른쪽대각선
		{
			if (i == 16)
			{
				m_pr[0].queen_back_right_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num || m_All_Queen[m_pr[num].clicked_object_num].rt.top == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num || m_All_Queen[m_pr[num].clicked_object_num].rt.top == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num].rt.top + rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num].rt.left + rt_num)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num].rt.top != 525)
					m_pr[0].queen_back_right_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;
	}

	////////////////////////

	else if (m_pr[num].m_player_num == 1 && m_pr[num].clicked_object_num != -1 && m_pr[num].select_what == SELECT_QUEEN)
	{
		while (1) // 앞
		{
			if (i == 16)
			{
				m_pr[1].queen_front++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num == 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num == 600 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top != 525) // 출력확인
					m_pr[1].queen_front++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 앞의 왼쪽대각선
		{
			if (i == 16)
			{
				m_pr[1].queen_front_left_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left == 0 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top != 525)
					m_pr[1].queen_front_left_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 앞의 오른쪽대각선
		{
			if (i == 16)
			{
				m_pr[1].queen_front_right_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left == 525 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top + rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top != 525)
					m_pr[1].queen_front_right_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 왼쪽
		{
			if (i == 16)
			{
				m_pr[1].queen_left++;
				i = 0;
				rt_num += 75;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num == -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Queen[i].rt.left == m_All_King[m_pr[num].clicked_object_num + 1].rt.left - rt_num)
			{
				break;
			}

			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num == -75 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Queen[i].rt.left == m_All_King[m_pr[num].clicked_object_num + 1].rt.left - rt_num)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left != 0)
					m_pr[1].queen_left++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 오른쪽
		{
			if (i == 16)
			{
				m_pr[1].queen_right++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left == 525 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num == 600 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num)
			{
				break;
			}

			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left == 525 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num == 600 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left != 525)
					m_pr[1].queen_right++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 뒤
		{
			if (i == 16)
			{
				m_pr[1].queen_back++;
				i = 0;
				rt_num += 75;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num == -75 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num == -75 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top != 0)
					m_pr[1].queen_back++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 뒤의 왼쪽대각선
		{
			if (i == 16)
			{
				m_pr[1].queen_back_left_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left == 0 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left - rt_num)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top != 0)
					m_pr[1].queen_back_left_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;

		while (1) // 뒤의 오른쪽대각선
		{
			if (i == 16)
			{
				m_pr[1].queen_back_right_diagonal++;
				i = 0;
				rt_num += 75;
			}

			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top == 0 ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 2 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 1 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num)
			{
				break;
			}
			else if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_Pawn[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Pawn[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top == 0 ||
				i < 2 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_Knight[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Knight[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 2 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_Rook[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Rook[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num == -75 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num == 600 || m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left == 525 ||
				i < 2 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_Bishop[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Bishop[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_Queen[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_Queen[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num ||
				i < 1 && m_All_King[i].status == ALIVE && m_All_King[i].player_num == 0 && m_All_King[i].rt.top == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top - rt_num && m_All_King[i].rt.left == m_All_Queen[m_pr[num].clicked_object_num + 1].rt.left + rt_num)
			{
				if (m_All_Queen[m_pr[num].clicked_object_num + 1].rt.top != 0)
					m_pr[1].queen_back_right_diagonal++;
				break;
			}
			else
			{
				i++;
			}
		}
		i = 0;
		rt_num = 75;
    }


}

void Game_System::King_Check(int num)
{
	if (m_pr[num].m_player_num == 0 && m_pr[num].clicked_object_num != -1 && m_pr[num].select_what == SELECT_KING)
	{
		for (int i = 0; i < 16; i++)
		{
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == -75 || m_All_King[m_pr[num].clicked_object_num].rt.top == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_front = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == -75 || m_All_King[m_pr[num].clicked_object_num].rt.top == 0 || m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == -75 || m_All_King[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_front_left_diagonal = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == -75 || m_All_King[m_pr[num].clicked_object_num].rt.top == 0 || m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == 600 || m_All_King[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top - 75 == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_front_right_diagonal = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == -75 || m_All_King[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_left = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == 600 || m_All_King[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_right = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == 600 || m_All_King[m_pr[num].clicked_object_num].rt.top == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_back = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == 525 || m_All_King[m_pr[num].clicked_object_num].rt.top == 525 || m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == -75 || m_All_King[m_pr[num].clicked_object_num].rt.left == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left - 75 == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_back_left_diagonal = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == 525 || m_All_King[m_pr[num].clicked_object_num].rt.top == 525 || m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == 600 || m_All_King[m_pr[num].clicked_object_num].rt.left == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Bishop[i].rt.left ||
				i < 1 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 0 && m_All_King[m_pr[num].clicked_object_num].rt.top + 75 == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num].rt.left + 75 == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_back_right_diagonal = TRUE;
			}
		}
	}

	//////////////////

	else if (m_pr[num].m_player_num == 1 && m_pr[num].clicked_object_num != -1 && m_pr[num].select_what == SELECT_KING)
	{
		for (int i = 0; i < 16; i++)
		{
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == 600 || m_All_King[m_pr[num].clicked_object_num + 1].rt.top == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_front = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == 600 || m_All_King[m_pr[num].clicked_object_num + 1].rt.top == 525 || m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == -75 || m_All_King[m_pr[num].clicked_object_num + 1].rt.left == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_front_left_diagonal = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == 600 || m_All_King[m_pr[num].clicked_object_num + 1].rt.top == 525 || m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == 600 || m_All_King[m_pr[num].clicked_object_num + 1].rt.left == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top + 75 == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_front_right_diagonal = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == -75 || m_All_King[m_pr[num].clicked_object_num + 1].rt.left == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_left = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == 600 || m_All_King[m_pr[num].clicked_object_num + 1].rt.left == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_right = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == -75 || m_All_King[m_pr[num].clicked_object_num + 1].rt.top == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_back = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == -75 || m_All_King[m_pr[num].clicked_object_num + 1].rt.top == 0 || m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == -75 || m_All_King[m_pr[num].clicked_object_num + 1].rt.left == 0 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left - 75 == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_back_left_diagonal = TRUE;
			}
			if (m_All_Pawn[i].status == ALIVE && m_All_Pawn[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Pawn[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Pawn[i].rt.left ||
				i < 4 && m_All_Rook[i].status == ALIVE && m_All_Rook[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Rook[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Rook[i].rt.left ||
				i < 4 && m_All_Knight[i].status == ALIVE && m_All_Knight[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Knight[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Knight[i].rt.left ||
				m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == -75 || m_All_King[m_pr[num].clicked_object_num + 1].rt.top == 0 || m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == 600 || m_All_King[m_pr[num].clicked_object_num + 1].rt.left == 525 ||
				i < 4 && m_All_Bishop[i].status == ALIVE && m_All_Bishop[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Bishop[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Bishop[i].rt.left ||
				i < 2 && m_All_Queen[i].status == ALIVE && m_All_Queen[i].player_num == 1 && m_All_King[m_pr[num].clicked_object_num + 1].rt.top - 75 == m_All_Queen[i].rt.top && m_All_King[m_pr[num].clicked_object_num + 1].rt.left + 75 == m_All_Queen[i].rt.left)
			{
				m_pr[num].king_back_right_diagonal = TRUE;
			}
		}
	}
}

void Game_System::Die_Check(int player_num)
{
	if (player_num == 0 && m_All_Pawn[m_pr[player_num].who_is_moved].status != DEAD && m_pr[player_num].select_what == SELECT_PAWN)
	{
		for (int i = 8; i < 16; i++) // pawn
		{
			if (m_All_Pawn[m_pr[player_num].who_is_moved].x == m_All_Pawn[i].x && m_All_Pawn[m_pr[player_num].who_is_moved].y == m_All_Pawn[i].y && m_All_Pawn[i].status != DEAD)
			{
				m_All_Pawn[i].status = DEAD;
				dead_num = i - 8;
				m_pr[1].Player_Die_Check(10, dead_num);
				return;
			}
		}
		for (int i = 2; i < 4; i++) // rook, knight, bishop
		{
			if (m_All_Pawn[m_pr[player_num].who_is_moved].x == m_All_Rook[i].x && m_All_Pawn[m_pr[player_num].who_is_moved].y == m_All_Rook[i].y && m_All_Rook[i].status != DEAD)
			{
				m_All_Rook[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(20, dead_num);
				return;
			}
			else if (m_All_Pawn[m_pr[player_num].who_is_moved].x == m_All_Knight[i].x && m_All_Pawn[m_pr[player_num].who_is_moved].y == m_All_Knight[i].y && m_All_Knight[i].status != DEAD)
			{
				m_All_Knight[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(30, dead_num);
				return;
			}
			else if (m_All_Pawn[m_pr[player_num].who_is_moved].x == m_All_Bishop[i].x && m_All_Pawn[m_pr[player_num].who_is_moved].y == m_All_Bishop[i].y && m_All_Bishop[i].status != DEAD)
			{
				m_All_Bishop[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(40, dead_num);
				return;
			}
		}

		if (m_All_Pawn[m_pr[player_num].who_is_moved].x == m_All_Queen[1].x && m_All_Pawn[m_pr[player_num].who_is_moved].y == m_All_Queen[1].y && m_All_Queen[1].status != DEAD)
		{
			m_All_Queen[1].status = DEAD;
			dead_num = 0;
			m_pr[1].Player_Die_Check(50, dead_num);
			return;
		}
		if (m_All_Pawn[m_pr[player_num].who_is_moved].x == m_All_King[1].x && m_All_Pawn[m_pr[player_num].who_is_moved].y == m_All_King[1].y && m_All_King[1].status != DEAD)
		{
			m_All_King[1].status = DEAD;
			dead_num = 0;
			m_pr[1].Player_Die_Check(60, dead_num);
			return;
		}
	}
	else if (player_num == 0 && m_All_Rook[m_pr[player_num].who_is_moved].status != DEAD && m_pr[player_num].select_what == SELECT_ROOK)
	{
		for (int i = 8; i < 16; i++) // pawn
		{
			if (m_All_Rook[m_pr[player_num].who_is_moved].x == m_All_Pawn[i].x && m_All_Rook[m_pr[player_num].who_is_moved].y == m_All_Pawn[i].y && m_All_Pawn[i].status != DEAD)
			{
				m_All_Pawn[i].status = DEAD;
				dead_num = i - 8;
				m_pr[1].Player_Die_Check(10, dead_num);
				return;
			}
		}
		for (int i = 2; i < 4; i++) // rook, knight, bishop
		{
			if (m_All_Rook[m_pr[player_num].who_is_moved].x == m_All_Rook[i].x && m_All_Rook[m_pr[player_num].who_is_moved].y == m_All_Rook[i].y && m_All_Rook[i].status != DEAD)
			{
				m_All_Rook[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(20, dead_num);
				return;
			}
			else if (m_All_Rook[m_pr[player_num].who_is_moved].x == m_All_Knight[i].x && m_All_Rook[m_pr[player_num].who_is_moved].y == m_All_Knight[i].y && m_All_Knight[i].status != DEAD)
			{
				m_All_Knight[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(30, dead_num);
				return;
			}
			else if (m_All_Rook[m_pr[player_num].who_is_moved].x == m_All_Bishop[i].x && m_All_Rook[m_pr[player_num].who_is_moved].y == m_All_Bishop[i].y && m_All_Bishop[i].status != DEAD)
			{
				m_All_Bishop[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(40, dead_num);
				return;
			}
		}

		if (m_All_Rook[m_pr[player_num].who_is_moved].x == m_All_Queen[1].x && m_All_Rook[m_pr[player_num].who_is_moved].y == m_All_Queen[1].y && m_All_Queen[1].status != DEAD)
		{
			m_All_Queen[1].status = DEAD;
			dead_num = 0;
			m_pr[1].Player_Die_Check(50, dead_num);
			return;
		}
		if (m_All_Rook[m_pr[player_num].who_is_moved].x == m_All_King[1].x && m_All_Rook[m_pr[player_num].who_is_moved].y == m_All_King[1].y && m_All_King[1].status != DEAD)
		{
			m_All_King[1].status = DEAD;
			dead_num = 0;
			m_pr[1].Player_Die_Check(60, dead_num);
			return;
		}
	}
	else if (player_num == 0 && m_All_Knight[m_pr[player_num].who_is_moved].status != DEAD && m_pr[player_num].select_what == SELECT_KNIGHT)
	{
		for (int i = 8; i < 16; i++) // pawn
		{
			if (m_All_Knight[m_pr[player_num].who_is_moved].x == m_All_Pawn[i].x && m_All_Knight[m_pr[player_num].who_is_moved].y == m_All_Pawn[i].y && m_All_Pawn[i].status != DEAD)
			{
				m_All_Pawn[i].status = DEAD;
				dead_num = i - 8;
				m_pr[1].Player_Die_Check(10, dead_num);
				return;
			}
		}
		for (int i = 2; i < 4; i++) // rook, knight, bishop
		{
			if (m_All_Knight[m_pr[player_num].who_is_moved].x == m_All_Rook[i].x && m_All_Knight[m_pr[player_num].who_is_moved].y == m_All_Rook[i].y && m_All_Rook[i].status != DEAD)
			{
				m_All_Rook[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(20, dead_num);
				return;
			}
			else if (m_All_Knight[m_pr[player_num].who_is_moved].x == m_All_Knight[i].x && m_All_Knight[m_pr[player_num].who_is_moved].y == m_All_Knight[i].y && m_All_Knight[i].status != DEAD)
			{
				m_All_Knight[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(30, dead_num);
				return;
			}
			else if (m_All_Knight[m_pr[player_num].who_is_moved].x == m_All_Bishop[i].x && m_All_Knight[m_pr[player_num].who_is_moved].y == m_All_Bishop[i].y && m_All_Bishop[i].status != DEAD)
			{
				m_All_Bishop[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(40, dead_num);
				return;
			}
		}

		if (m_All_Knight[m_pr[player_num].who_is_moved].x == m_All_Queen[1].x && m_All_Knight[m_pr[player_num].who_is_moved].y == m_All_Queen[1].y && m_All_Queen[1].status != DEAD)
		{
			m_All_Queen[1].status = DEAD;
			dead_num = 0;
			m_pr[1].Player_Die_Check(50, dead_num);
			return;
		}
		if (m_All_Knight[m_pr[player_num].who_is_moved].x == m_All_King[1].x && m_All_Knight[m_pr[player_num].who_is_moved].y == m_All_King[1].y && m_All_King[1].status != DEAD)
		{
			m_All_King[1].status = DEAD;
			dead_num = 0;
			m_pr[1].Player_Die_Check(60, dead_num);
			return;
		}
	}
	else if (player_num == 0 && m_All_Bishop[m_pr[player_num].who_is_moved].status != DEAD && m_pr[player_num].select_what == SELECT_BISHOP)
	{
		for (int i = 8; i < 16; i++) // pawn
		{
			if (m_All_Bishop[m_pr[player_num].who_is_moved].x == m_All_Pawn[i].x && m_All_Bishop[m_pr[player_num].who_is_moved].y == m_All_Pawn[i].y && m_All_Pawn[i].status != DEAD)
			{
				m_All_Pawn[i].status = DEAD;
				dead_num = i - 8;
				m_pr[1].Player_Die_Check(10, dead_num);
				return;
			}
		}
		for (int i = 2; i < 4; i++) // rook, knight, bishop
		{
			if (m_All_Bishop[m_pr[player_num].who_is_moved].x == m_All_Rook[i].x && m_All_Bishop[m_pr[player_num].who_is_moved].y == m_All_Rook[i].y && m_All_Rook[i].status != DEAD)
			{
				m_All_Rook[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(20, dead_num);
				return;
			}
			else if (m_All_Bishop[m_pr[player_num].who_is_moved].x == m_All_Knight[i].x && m_All_Bishop[m_pr[player_num].who_is_moved].y == m_All_Knight[i].y && m_All_Knight[i].status != DEAD)
			{
				m_All_Knight[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(30, dead_num);
				return;
			}
			else if (m_All_Bishop[m_pr[player_num].who_is_moved].x == m_All_Bishop[i].x && m_All_Bishop[m_pr[player_num].who_is_moved].y == m_All_Bishop[i].y && m_All_Bishop[i].status != DEAD)
			{
				m_All_Bishop[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(40, dead_num);
				return;
			}
		}

		if (m_All_Bishop[m_pr[player_num].who_is_moved].x == m_All_Queen[1].x && m_All_Bishop[m_pr[player_num].who_is_moved].y == m_All_Queen[1].y && m_All_Queen[1].status != DEAD)
		{
			m_All_Queen[1].status = DEAD;
			dead_num = 0;
			m_pr[1].Player_Die_Check(50, dead_num);
			return;
		}
		if (m_All_Bishop[m_pr[player_num].who_is_moved].x == m_All_King[1].x && m_All_Bishop[m_pr[player_num].who_is_moved].y == m_All_King[1].y && m_All_King[1].status != DEAD)
		{
			m_All_King[1].status = DEAD;
			dead_num = 0;
			m_pr[1].Player_Die_Check(60, dead_num);
			return;
		}
    }
	else if (player_num == 0 && m_All_Queen[m_pr[player_num].who_is_moved].status != DEAD && m_pr[player_num].select_what == SELECT_QUEEN)
	{
		for (int i = 8; i < 16; i++) // pawn
		{
			if (m_All_Queen[m_pr[player_num].who_is_moved].x == m_All_Pawn[i].x && m_All_Queen[m_pr[player_num].who_is_moved].y == m_All_Pawn[i].y && m_All_Pawn[i].status != DEAD)
			{
				m_All_Pawn[i].status = DEAD;
				dead_num = i - 8;
				m_pr[1].Player_Die_Check(10, dead_num);
				return;
			}
		}
		for (int i = 2; i < 4; i++) // rook, knight, bishop
		{
			if (m_All_Queen[m_pr[player_num].who_is_moved].x == m_All_Rook[i].x && m_All_Queen[m_pr[player_num].who_is_moved].y == m_All_Rook[i].y && m_All_Rook[i].status != DEAD)
			{
				m_All_Rook[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(20, dead_num);
				return;
			}
			else if (m_All_Queen[m_pr[player_num].who_is_moved].x == m_All_Knight[i].x && m_All_Queen[m_pr[player_num].who_is_moved].y == m_All_Knight[i].y && m_All_Knight[i].status != DEAD)
			{
				m_All_Knight[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(30, dead_num);
				return;
			}
			else if (m_All_Queen[m_pr[player_num].who_is_moved].x == m_All_Bishop[i].x && m_All_Queen[m_pr[player_num].who_is_moved].y == m_All_Bishop[i].y && m_All_Bishop[i].status != DEAD)
			{
				m_All_Bishop[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(40, dead_num);
				return;
			}
		}

		if (m_All_Queen[m_pr[player_num].who_is_moved].x == m_All_Queen[1].x && m_All_Queen[m_pr[player_num].who_is_moved].y == m_All_Queen[1].y && m_All_Queen[1].status != DEAD)
		{
			m_All_Queen[1].status = DEAD;
			dead_num = 0;
			m_pr[1].Player_Die_Check(50, dead_num);
			return;
		}
		if (m_All_Queen[m_pr[player_num].who_is_moved].x == m_All_King[1].x && m_All_Queen[m_pr[player_num].who_is_moved].y == m_All_King[1].y && m_All_King[1].status != DEAD)
		{
			m_All_King[1].status = DEAD;
			dead_num = 0;
			m_pr[1].Player_Die_Check(60, dead_num);
			return;
		}
    }

	else if (player_num == 0 && m_All_King[m_pr[player_num].who_is_moved].status != DEAD && m_pr[player_num].select_what == SELECT_KING)
	{
		for (int i = 8; i < 16; i++) // pawn
		{
			if (m_All_King[m_pr[player_num].who_is_moved].x == m_All_Pawn[i].x && m_All_King[m_pr[player_num].who_is_moved].y == m_All_Pawn[i].y && m_All_Pawn[i].status != DEAD)
			{
				m_All_Pawn[i].status = DEAD;
				dead_num = i - 8;
				m_pr[1].Player_Die_Check(10, dead_num);
				return;
			}
		}
		for (int i = 2; i < 4; i++) // rook, knight, bishop
		{
			if (m_All_King[m_pr[player_num].who_is_moved].x == m_All_Rook[i].x && m_All_King[m_pr[player_num].who_is_moved].y == m_All_Rook[i].y && m_All_Rook[i].status != DEAD)
			{
				m_All_Rook[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(20, dead_num);
				return;
			}
			else if (m_All_King[m_pr[player_num].who_is_moved].x == m_All_Knight[i].x && m_All_King[m_pr[player_num].who_is_moved].y == m_All_Knight[i].y && m_All_Knight[i].status != DEAD)
			{
				m_All_Knight[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(30, dead_num);
				return;
			}
			else if (m_All_King[m_pr[player_num].who_is_moved].x == m_All_Bishop[i].x && m_All_King[m_pr[player_num].who_is_moved].y == m_All_Bishop[i].y && m_All_Bishop[i].status != DEAD)
			{
				m_All_Bishop[i].status = DEAD;
				dead_num = i - 2;
				m_pr[1].Player_Die_Check(40, dead_num);
				return;
			}
		}

		if (m_All_King[m_pr[player_num].who_is_moved].x == m_All_Queen[1].x && m_All_King[m_pr[player_num].who_is_moved].y == m_All_Queen[1].y && m_All_Queen[1].status != DEAD)
		{
			m_All_Queen[1].status = DEAD;
			dead_num = 0;
			m_pr[1].Player_Die_Check(50, dead_num);
			return;
		}
		if (m_All_King[m_pr[player_num].who_is_moved].x == m_All_King[1].x && m_All_King[m_pr[player_num].who_is_moved].y == m_All_King[1].y && m_All_King[1].status != DEAD)
		{
			m_All_King[1].status = DEAD;
			dead_num = 0;
			m_pr[1].Player_Die_Check(60, dead_num);
			return;
		}
    }

	//////////////////////////////////

	else if (player_num == 1 && m_All_Pawn[m_pr[player_num].who_is_moved + 8].status != DEAD && m_pr[player_num].select_what == SELECT_PAWN)
	{
		for (int i = 0; i < 8; i++) // pawn
		{
			if (m_All_Pawn[m_pr[player_num].who_is_moved + 8].x == m_All_Pawn[i].x && m_All_Pawn[m_pr[player_num].who_is_moved + 8].y == m_All_Pawn[i].y && m_All_Pawn[i].status != DEAD)
			{
				m_All_Pawn[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(10, dead_num);
				return;
			}
		}
		for (int i = 0; i < 2; i++) // rook, knight, bishop
		{
			if (m_All_Pawn[m_pr[player_num].who_is_moved + 8].x == m_All_Rook[i].x && m_All_Pawn[m_pr[player_num].who_is_moved + 8].y == m_All_Rook[i].y && m_All_Rook[i].status != DEAD)
			{
				m_All_Rook[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(20, dead_num);
				return;
			}
			else if (m_All_Pawn[m_pr[player_num].who_is_moved + 8].x == m_All_Knight[i].x && m_All_Pawn[m_pr[player_num].who_is_moved + 8].y == m_All_Knight[i].y && m_All_Knight[i].status != DEAD)
			{
				m_All_Knight[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(30, dead_num);
				return;
			}
			else if (m_All_Pawn[m_pr[player_num].who_is_moved + 8].x == m_All_Bishop[i].x && m_All_Pawn[m_pr[player_num].who_is_moved + 8].y == m_All_Bishop[i].y && m_All_Bishop[i].status != DEAD)
			{
				m_All_Bishop[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(40, dead_num);
				return;
			}
		}

		if (m_All_Pawn[m_pr[player_num].who_is_moved + 8].x == m_All_Queen[0].x && m_All_Pawn[m_pr[player_num].who_is_moved + 8].y == m_All_Queen[0].y && m_All_Queen[0].status != DEAD)
		{
			m_All_Queen[0].status = DEAD;
			dead_num = 0;
			m_pr[0].Player_Die_Check(50, dead_num);
			return;
		}
		if (m_All_Pawn[m_pr[player_num].who_is_moved + 8].x == m_All_King[0].x && m_All_Pawn[m_pr[player_num].who_is_moved + 8].y == m_All_King[0].y && m_All_King[0].status != DEAD)
		{
			m_All_King[0].status = DEAD;
			dead_num = 0;
			m_pr[0].Player_Die_Check(60, dead_num);
			return;
		}
	}
	else if (player_num == 1 && m_All_Rook[m_pr[player_num].who_is_moved + 2].status != DEAD && m_pr[player_num].select_what == SELECT_ROOK)
	{
		for (int i = 0; i < 8; i++) // pawn
		{
			if (m_All_Rook[m_pr[player_num].who_is_moved + 2].x == m_All_Pawn[i].x && m_All_Rook[m_pr[player_num].who_is_moved + 2].y == m_All_Pawn[i].y && m_All_Pawn[i].status != DEAD)
			{
				m_All_Pawn[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(10, dead_num);
				return;
			}
		}
		for (int i = 0; i < 2; i++) // rook, knight, bishop
		{
			if (m_All_Rook[m_pr[player_num].who_is_moved + 2].x == m_All_Rook[i].x && m_All_Rook[m_pr[player_num].who_is_moved + 2].y == m_All_Rook[i].y && m_All_Rook[i].status != DEAD)
			{
				m_All_Rook[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(20, dead_num);
				return;
			}
			else if (m_All_Rook[m_pr[player_num].who_is_moved + 2].x == m_All_Knight[i].x && m_All_Rook[m_pr[player_num].who_is_moved + 2].y == m_All_Knight[i].y && m_All_Knight[i].status != DEAD)
			{
				m_All_Knight[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(30, dead_num);
				return;
			}
			else if (m_All_Rook[m_pr[player_num].who_is_moved + 2].x == m_All_Bishop[i].x && m_All_Rook[m_pr[player_num].who_is_moved + 2].y == m_All_Bishop[i].y && m_All_Bishop[i].status != DEAD)
			{
				m_All_Bishop[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(40, dead_num);
				return;
			}
		}

		if (m_All_Rook[m_pr[player_num].who_is_moved + 2].x == m_All_Queen[0].x && m_All_Rook[m_pr[player_num].who_is_moved + 2].y == m_All_Queen[0].y && m_All_Queen[0].status != DEAD)
		{
			m_All_Queen[0].status = DEAD;
			dead_num = 0;
			m_pr[0].Player_Die_Check(50, dead_num);
			return;
		}
		if (m_All_Rook[m_pr[player_num].who_is_moved + 2].x == m_All_King[0].x && m_All_Rook[m_pr[player_num].who_is_moved + 2].y == m_All_King[0].y && m_All_King[0].status != DEAD)
		{
			m_All_King[0].status = DEAD;
			dead_num = 0;
			m_pr[0].Player_Die_Check(60, dead_num);
			return;
		}
	}
	else if (player_num == 1 && m_All_Knight[m_pr[player_num].who_is_moved + 2].status != DEAD && m_pr[player_num].select_what == SELECT_KNIGHT)
	{
	    for (int i = 0; i < 8; i++) // pawn
		{
			if (m_All_Knight[m_pr[player_num].who_is_moved + 2].x == m_All_Pawn[i].x && m_All_Knight[m_pr[player_num].who_is_moved + 2].y == m_All_Pawn[i].y && m_All_Pawn[i].status != DEAD)
			{
				m_All_Pawn[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(10, dead_num);
				return;
			}
		}
		for (int i = 0; i < 2; i++) // rook, knight, bishop
		{
			if (m_All_Knight[m_pr[player_num].who_is_moved + 2].x == m_All_Rook[i].x && m_All_Knight[m_pr[player_num].who_is_moved + 2].y == m_All_Rook[i].y && m_All_Rook[i].status != DEAD)
			{
				m_All_Rook[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(20, dead_num);
				return;
			}
			else if (m_All_Knight[m_pr[player_num].who_is_moved + 2].x == m_All_Knight[i].x && m_All_Knight[m_pr[player_num].who_is_moved + 2].y == m_All_Knight[i].y && m_All_Knight[i].status != DEAD)
			{
				m_All_Knight[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(30, dead_num);
				return;
			}
			else if (m_All_Knight[m_pr[player_num].who_is_moved + 2].x == m_All_Bishop[i].x && m_All_Knight[m_pr[player_num].who_is_moved + 2].y == m_All_Bishop[i].y && m_All_Bishop[i].status != DEAD)
			{
				m_All_Bishop[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(40, dead_num);
				return;
			}
		}

		if (m_All_Knight[m_pr[player_num].who_is_moved + 2].x == m_All_Queen[0].x && m_All_Knight[m_pr[player_num].who_is_moved + 2].y == m_All_Queen[0].y && m_All_Queen[0].status != DEAD)
		{
			m_All_Queen[0].status = DEAD;
			dead_num = 0;
			m_pr[0].Player_Die_Check(50, dead_num);
			return;
		}
		if (m_All_Knight[m_pr[player_num].who_is_moved + 2].x == m_All_King[0].x && m_All_Knight[m_pr[player_num].who_is_moved + 2].y == m_All_King[0].y && m_All_King[0].status != DEAD)
		{
			m_All_King[0].status = DEAD;
			dead_num = 0;
			m_pr[0].Player_Die_Check(60, dead_num);
			return;
		}
    }
	else if (player_num == 1 && m_All_Bishop[m_pr[player_num].who_is_moved + 2].status != DEAD && m_pr[player_num].select_what == SELECT_BISHOP)
	{
	    for (int i = 0; i < 8; i++) // pawn
		{
			if (m_All_Bishop[m_pr[player_num].who_is_moved + 2].x == m_All_Pawn[i].x && m_All_Bishop[m_pr[player_num].who_is_moved + 2].y == m_All_Pawn[i].y && m_All_Pawn[i].status != DEAD)
			{
				m_All_Pawn[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(10, dead_num);
				return;
			}
		}
		for (int i = 0; i < 2; i++) // rook, knight, bishop
		{
			if (m_All_Bishop[m_pr[player_num].who_is_moved + 2].x == m_All_Rook[i].x && m_All_Bishop[m_pr[player_num].who_is_moved + 2].y == m_All_Rook[i].y && m_All_Rook[i].status != DEAD)
			{
				m_All_Rook[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(20, dead_num);
				return;
			}
			else if (m_All_Bishop[m_pr[player_num].who_is_moved + 2].x == m_All_Knight[i].x && m_All_Bishop[m_pr[player_num].who_is_moved + 2].y == m_All_Knight[i].y && m_All_Knight[i].status != DEAD)
			{
				m_All_Knight[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(30, dead_num);
				return;
			}
			else if (m_All_Bishop[m_pr[player_num].who_is_moved + 2].x == m_All_Bishop[i].x && m_All_Bishop[m_pr[player_num].who_is_moved + 2].y == m_All_Bishop[i].y && m_All_Bishop[i].status != DEAD)
			{
				m_All_Bishop[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(40, dead_num);
				return;
			}
		}

		if (m_All_Bishop[m_pr[player_num].who_is_moved + 2].x == m_All_Queen[0].x && m_All_Bishop[m_pr[player_num].who_is_moved + 2].y == m_All_Queen[0].y && m_All_Queen[0].status != DEAD)
		{
			m_All_Queen[0].status = DEAD;
			dead_num = 0;
			m_pr[0].Player_Die_Check(50, dead_num);
			return;
		}
		if (m_All_Bishop[m_pr[player_num].who_is_moved + 2].x == m_All_King[0].x && m_All_Bishop[m_pr[player_num].who_is_moved + 2].y == m_All_King[0].y && m_All_King[0].status != DEAD)
		{
			m_All_King[0].status = DEAD;
			dead_num = 0;
			m_pr[0].Player_Die_Check(60, dead_num);
			return;
		}
    }
	else if (player_num == 1 && m_All_Queen[m_pr[player_num].who_is_moved + 1].status != DEAD && m_pr[player_num].select_what == SELECT_QUEEN)
	{
		for (int i = 0; i < 8; i++) // pawn
		{
			if (m_All_Queen[m_pr[player_num].who_is_moved + 1].x == m_All_Pawn[i].x && m_All_Queen[m_pr[player_num].who_is_moved + 1].y == m_All_Pawn[i].y && m_All_Pawn[i].status != DEAD)
			{
				m_All_Pawn[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(10, dead_num);
				return;
			}
		}
		for (int i = 0; i < 2; i++) // rook, knight, bishop
		{
			if (m_All_Queen[m_pr[player_num].who_is_moved + 1].x == m_All_Rook[i].x && m_All_Queen[m_pr[player_num].who_is_moved + 1].y == m_All_Rook[i].y && m_All_Rook[i].status != DEAD)
			{
				m_All_Rook[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(20, dead_num);
				return;
			}
			else if (m_All_Queen[m_pr[player_num].who_is_moved + 1].x == m_All_Knight[i].x && m_All_Queen[m_pr[player_num].who_is_moved + 1].y == m_All_Knight[i].y && m_All_Knight[i].status != DEAD)
			{
				m_All_Knight[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(30, dead_num);
				return;
			}
			else if (m_All_Queen[m_pr[player_num].who_is_moved + 1].x == m_All_Bishop[i].x && m_All_Queen[m_pr[player_num].who_is_moved + 1].y == m_All_Bishop[i].y && m_All_Bishop[i].status != DEAD)
			{
				m_All_Bishop[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(40, dead_num);
				return;
			}
		}

		if (m_All_Queen[m_pr[player_num].who_is_moved + 1].x == m_All_Queen[0].x && m_All_Queen[m_pr[player_num].who_is_moved + 1].y == m_All_Queen[0].y && m_All_Queen[0].status != DEAD)
		{
			m_All_Queen[0].status = DEAD;
			dead_num = 0;
			m_pr[0].Player_Die_Check(50, dead_num);
			return;
		}
		if (m_All_Queen[m_pr[player_num].who_is_moved + 1].x == m_All_King[0].x && m_All_Queen[m_pr[player_num].who_is_moved + 1].y == m_All_King[0].y && m_All_King[0].status != DEAD)
		{
			m_All_King[0].status = DEAD;
			dead_num = 0;
			m_pr[0].Player_Die_Check(60, dead_num);
			return;
		}
    }

	else if (player_num == 1 && m_All_King[m_pr[player_num].who_is_moved + 1].status != DEAD && m_pr[player_num].select_what == SELECT_KING)
	{
		for (int i = 0; i < 8; i++) // pawn
		{
			if (m_All_King[m_pr[player_num].who_is_moved + 1].x == m_All_Pawn[i].x && m_All_King[m_pr[player_num].who_is_moved + 1].y == m_All_Pawn[i].y && m_All_Pawn[i].status != DEAD)
			{
				m_All_Pawn[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(10, dead_num);
				return;
			}
		}
		for (int i = 0; i < 2; i++) // rook, knight, bishop
		{
			if (m_All_King[m_pr[player_num].who_is_moved + 1].x == m_All_Rook[i].x && m_All_King[m_pr[player_num].who_is_moved + 1].y == m_All_Rook[i].y && m_All_Rook[i].status != DEAD)
			{
				m_All_Rook[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(20, dead_num);
				return;
			}
			else if (m_All_King[m_pr[player_num].who_is_moved + 1].x == m_All_Knight[i].x && m_All_King[m_pr[player_num].who_is_moved + 1].y == m_All_Knight[i].y && m_All_Knight[i].status != DEAD)
			{
				m_All_Knight[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(30, dead_num);
				return;
			}
			else if (m_All_King[m_pr[player_num].who_is_moved + 1].x == m_All_Bishop[i].x && m_All_King[m_pr[player_num].who_is_moved + 1].y == m_All_Bishop[i].y && m_All_Bishop[i].status != DEAD)
			{
				m_All_Bishop[i].status = DEAD;
				dead_num = i;
				m_pr[0].Player_Die_Check(40, dead_num);
				return;
			}
		}

		if (m_All_King[m_pr[player_num].who_is_moved + 1].x == m_All_Queen[0].x && m_All_King[m_pr[player_num].who_is_moved + 1].y == m_All_Queen[0].y && m_All_Queen[0].status != DEAD)
		{
			m_All_Queen[0].status = DEAD;
			dead_num = 0;
			m_pr[0].Player_Die_Check(50, dead_num);
			return;
		}
		if (m_All_King[m_pr[player_num].who_is_moved + 1].x == m_All_King[0].x && m_All_King[m_pr[player_num].who_is_moved + 1].y == m_All_King[0].y && m_All_King[0].status != DEAD)
		{
			m_All_King[0].status = DEAD;
			dead_num = 0;
			m_pr[0].Player_Die_Check(60, dead_num);
			return;
		}
    }
}

void Game_System::Piece_Rules(int num)
{
	m_pr[num].someting = FALSE;
	m_pr[num].pawn_front = FALSE;
	m_pr[num].pawn_double_front = FALSE;
	m_pr[num].pawn_diagonal1 = FALSE;
	m_pr[num].pawn_diagonal2 = FALSE;

	m_pr[num].rook_front = FALSE;
	m_pr[num].rook_back = FALSE;
	m_pr[num].rook_left = FALSE;
	m_pr[num].rook_right = FALSE;

	m_pr[num].knight_front_left_diagonal1 = FALSE;
	m_pr[num].knight_front_left_diagonal2 = FALSE;
	m_pr[num].knight_front_right_diagonal1 = FALSE;
	m_pr[num].knight_front_right_diagonal2 = FALSE;
	m_pr[num].knight_back_left_diagonal1 = FALSE;
	m_pr[num].knight_back_left_diagonal2 = FALSE;
	m_pr[num].knight_back_right_diagonal1 = FALSE;
	m_pr[num].knight_back_right_diagonal2 = FALSE;

	m_pr[num].bishop_front_left_diagonal = FALSE;
	m_pr[num].bishop_front_right_diagonal = FALSE;
	m_pr[num].bishop_back_left_diagonal = FALSE;
	m_pr[num].bishop_back_right_diagonal = FALSE;

	m_pr[num].queen_front = FALSE;
	m_pr[num].queen_front_left_diagonal = FALSE;
	m_pr[num].queen_front_right_diagonal = FALSE;
	m_pr[num].queen_left = FALSE;
	m_pr[num].queen_right = FALSE;
	m_pr[num].queen_back = FALSE;
	m_pr[num].queen_back_left_diagonal = FALSE;
	m_pr[num].queen_back_right_diagonal = FALSE;

	m_pr[num].king_front = FALSE;
	m_pr[num].king_front_left_diagonal = FALSE;
	m_pr[num].king_front_right_diagonal = FALSE;
	m_pr[num].king_left = FALSE;
	m_pr[num].king_right = FALSE;
	m_pr[num].king_back = FALSE;
	m_pr[num].king_back_left_diagonal = FALSE;
	m_pr[num].king_back_right_diagonal = FALSE;
}

int Game_System::Game_Over_Check()
{
	if (m_All_King[0].status == DEAD)
		return 10;
	else if (m_All_King[1].status == DEAD)
		return 20;

	return 0;
}

Game_System::~Game_System()
{

}