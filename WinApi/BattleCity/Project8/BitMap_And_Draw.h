#include <Windows.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <stack>
using namespace std;
#pragma once

#define BITMAP_MAX 63
#define BITMAP_SIZE_MAX 63

enum bit_map
{
	BLANK,
	N_BLOCK_00, N_BLOCK_01, N_BLOCK_02, N_BLOCK_03, N_BLOCK_04,
	GRAY_BLOCK, BUSH_BLOCK, WATER_BLOCK, WHITE_BLOCK_00, WHITE_BLOCK_01, WHITE_BLOCK_02, WHITE_BLOCK_03, WHITE_BLOCK_04,
	BASEMENT_ALIVE, BASEMENT_DEAD, ENEMY_DOWN_00, ENEMY_DOWN_01, ENEMY_DOWN_02, ENEMY_LEFT_00, ENEMY_LEFT_01, ENEMY_LEFT_02,
	ENEMY_RIGHT_00, ENEMY_RIGHT_01, ENEMY_RIGHT_02, ENEMY_UP_00, ENEMY_UP_01, ENEMY_UP_02, ENEMY_LIFE, EXPLOSION_00, EXPLOSION_01,
	EXPLOSION_02, EXPLOSION_03, EXPLOSION_04, GAME_OVERS, ITEM_SHIELD, ITEM_SPEED, MISSILE_RIGHT, MISSILE_LEFT, MISSILE_UP, MISSILE_DOWN, PLAYER_LIFE, SHIELD_00, SHIELD_01, STAGE_ICON,
	STAGE_01, STAGE_02, STAGE_03, PLAYER_DONW_00, PLAYER_DONW_01,
	PLAYER_LEFT_00, PLAYER_LEFT_01, PLAYER_RIGHT_00, PLAYER_RIGHT_01, PLAYER_UP_00, PLAYER_UP_01, TITLE_LOGO, TITLE_MENU_01, TITLE_MENU_02,
	TITLE_MENU_03, TITLE_MENU_04, UI, UI_CLEAR
};

class BitMap_And_Draw
{
private:
	HDC hdc;
	BITMAP B_Info;
	HDC Entire_DC; // 그리기를위한 전체배경이며, 검은화면으로도 쓴다
	HDC MemDC[BITMAP_MAX]; //// 리소스 저장할곳
	HBITMAP	m_BitMap[BITMAP_MAX];
	HBITMAP	m_Old_BitMap[BITMAP_MAX];
	SIZE m_size[BITMAP_SIZE_MAX];
	LPCSTR m_bitmap[BITMAP_MAX] = { "blank.bmp", "block00.bmp", "block01.bmp", "block02.bmp", "block03.bmp", "block04.bmp", "block05.bmp", 
	"block06.bmp", "block07.bmp", "block08.bmp", "block09.bmp","block10.bmp","block11.bmp", "block12.bmp", "block13.bmp", "block14.bmp", 
	"e_down_00.bmp", "e_down_01.bmp", "e_down_02.bmp", "e_left_00.bmp", "e_left_01.bmp", "e_left_02.bmp", "e_right_00.bmp", "e_right_01.bmp", "e_right_02.bmp", "e_up_00.bmp", 
	"e_up_01.bmp", "e_up_02.bmp", "enemy_icon.bmp", "explosion_00.bmp", "explosion_01.bmp", "explosion_02.bmp", "explosion_03.bmp", "explosion_04.bmp", "game_over.bmp", "item_shield.bmp", "item_speed.bmp", "missile_00.bmp", "missile_01.bmp", "missile_02.bmp", 
	"missile_03.bmp", "player_icon.bmp", "shield_00.bmp", "shield_01.bmp", "stage_icon.bmp", "stage01.bmp", "stage02.bmp", "stage03.bmp", "tank_down_00.bmp", "tank_down_01.bmp", "tank_left_00.bmp", "tank_left_01.bmp", "tank_right_00.bmp", "tank_right_01.bmp",
	"tank_up_00.bmp", "tank_up_01.bmp", "title_logo.bmp", "title_menu01.bmp", "title_menu02.bmp", "title_menu03.bmp", "title_menu04.bmp", "ui.bmp", "ui_clear.bmp" };
	//// 로컬파일명이 중복일 경우 읽어오지못하는것 발생, 그냥 바로 파일이름만 적기
public:
	BitMap_And_Draw();

	void Init_Bitmap(HWND hWnd);
	void Draw(int _x, int _y, int _m_size, int dc_num);
	void Draw_Ready(int _x, int _y, int _m_size, int dc_num);
	void Draw_Detail_Ready(int _x, int _y, int _m_size, int dc_num, int x_size, int y_size);
	void Draw_Go();
	void Draw_Black_BackGround();

	~BitMap_And_Draw();
};

