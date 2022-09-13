#include <iostream>
#include <windows.h>
#include <string.h>
#pragma once
using namespace std;

#define PIECE_MAX 16
#define ALIVE 20
#define DEAD 21

#define SELECT_PAWN 58
#define SELECT_ROOK 59
#define SELECT_KNIGHT 60
#define SELECT_BISHOP 61
#define SELECT_QUEEN 62
#define SELECT_KING 63

struct RECT_AND_STATUS
{
	RECT rt;
	int status;
	int player_num;
	int x;
	int y;
};

struct Piece
{
	int x;
	int y;
	int draw;
	RECT rt;
	int first_move; // pawn
	int status;
};

class chess_pieces
{
private:
	HDC MemDC[PIECE_MAX];
	HBITMAP hbtmap, old_hbtmap;
	BITMAP btmap;
public:
	chess_pieces();

	Piece m_King;
	Piece m_Queen;
	Piece m_Rook[2];
	Piece m_Bishop[2];
	Piece m_Knight[2];
	Piece m_Pawn[8];
	int m_x;
	int m_y;

	void Init_pieces(HDC hdc, int player_num);
	void Init_pieces_2(HDC hdc, int player_num);
	void Pieces_Draw(HDC hdc, int x, int y, string name);
	void Set_Piece_Info(int name, int num, int x, int y);

	~chess_pieces();
};

