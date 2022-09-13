#include "chess_pieces.h"
#pragma once

struct TMP_PIECE_SAVE
{
	int tmp_x;
	int tmp_y;
	int tmp_piece_name;
	int tmp_piece_num;
};

class Player
{
private:
	chess_pieces *Cp;
	HDC MemDC;
	HBITMAP hbtmap, old_hbtmap;
	BITMAP btmap;
public:
	Player();

	TMP_PIECE_SAVE tmp_piece_save;
	int select_num;
	int m_x;
	int m_y;
	int clicked_pos_x;
	int clicked_pos_y;
	int clicked_object_num;
	RECT selected_object_rt;
	int m_player_num;
	int my_turn;
	int someting;
	RECT_AND_STATUS tmp_rt;
	int who_is_moved;
	int select_what;

	//// pawn 이동규칙변수
	int pawn_front;
	int pawn_double_front;
	int pawn_diagonal1;
	int pawn_diagonal2;
	//// rook 이동규칙변수
	int rook_front;
	int rook_back;
	int rook_left;
	int rook_right;
	//// knight 이동규칙변수
	int knight_front_left_diagonal1;
	int knight_front_left_diagonal2;
	int knight_front_right_diagonal1;
	int knight_front_right_diagonal2;
	int knight_back_left_diagonal1;
	int knight_back_left_diagonal2;
	int knight_back_right_diagonal1;
	int knight_back_right_diagonal2;
	//// bishop 이동규칙변수
	int bishop_front_left_diagonal;
	int bishop_front_right_diagonal;
	int bishop_back_left_diagonal;
	int bishop_back_right_diagonal;
	//// queen 이동규칙변수
	int queen_front;
	int queen_front_left_diagonal;
	int queen_front_right_diagonal;
	int queen_left;
	int queen_right;
	int queen_back;
	int queen_back_left_diagonal;
	int queen_back_right_diagonal;
	//// king 이동규칙변수
	int king_front;
	int king_front_left_diagonal;
	int king_front_right_diagonal;
	int king_left;
	int king_right;
	int king_back;
	int king_back_left_diagonal;
	int king_back_right_diagonal;

	void Init_Player(HDC hdc, int player_num);
	void Player_Pieces_Draw(HDC hdc, int name, int num, int x, int y);
	void Move_Able_Point(HDC hdc);
	void Click_Check(HDC hdc, int player_num, int x, int y);
	int Move_Check(HDC hdc, int x, int y);
	void Player_Die_Check(int piece_num, int dead_num);

	~Player();
};

