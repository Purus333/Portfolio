#include <Windows.h>
#include <iostream>
#include <string.h>
#pragma once

#define UP 11
#define DOWN 12
#define LEFT 13
#define RIGHT 14

class Missile
{
private:
	int m_ix;
	int m_iy;
	int m_direct;
	int m_check_player;
	int m_status;
public:
	Missile();

	int missile_start_x;
	int missile_start_y;

	void Init_Missile();
	void Init_Player_Missile();
	int Move_Missile(int dir);
	int Get_Missile_X();
	int Get_Missile_Y();
	void Set_Missile_X(int value);
	void Set_Missile_Y(int value);
	int Get_Missile_Status();
	void Set_Missile_Status(int value);
	int Get_Missile_Player();
	int Get_Missile_Direct();

	~Missile();
};

