#include "Board.h"
#include "chess_pieces.h"
#pragma once


class Game_System
{
private:
	Board *m_bd;
	Player *m_pr;
public:
	Game_System();

	RECT_AND_STATUS m_All_Pawn[16];
	RECT_AND_STATUS m_All_Rook[4];
	RECT_AND_STATUS m_All_Knight[4];
	RECT_AND_STATUS m_All_Bishop[4];
	RECT_AND_STATUS m_All_Queen[2];
	RECT_AND_STATUS m_All_King[2];
	int dead_num;

	void Init_System(HDC hdc, HINSTANCE hinst);
	void Draw(HDC hdc);
	int Click(HDC hdc, int x , int y);
	void Set_All_Pawn_Pos();
	void All_Piece_Pos(int player_num);
	void Pawn_Check(int num);
	void Rook_Check(int num);
	void Knight_Check(int num);
	void Bishop_Check(int num);
	void Queen_Check(int num);
	void King_Check(int num);
	void Piece_Rules(int num);
	void Die_Check(int player_num);
	int Game_Over_Check();

	~Game_System();
};

