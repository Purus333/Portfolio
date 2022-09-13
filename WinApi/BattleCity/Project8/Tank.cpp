#include "Tank.h"

Tank::Tank()
{
	m_ix = 0;
	m_iy = 0;
	motion = 0;
	direct = NULL;
	player = NULL;
	status = DEAD;
	shield = FALSE;
	shield_motion = 0;
	speed = 1;
	move_start_check = FALSE;
	save_pre_direct = FALSE;
	turn_switch = FALSE;
	level_up_mode = FALSE;
	left_count = 0;
	right_count = 0;
	up_count = 0;
	down_count = 0;
	gogo_front = 0;
}

void Tank::Init_Tank(int player_or_enemy)
{
	if (player_or_enemy == 0)
	{
		m_ix = 0;
		m_iy = 0;
		m_pre_x = 0;
		m_pre_y = 0;
		motion = 0;
		direct = UP;
		player = TRUE;
		status = DEAD;
		player_start_x = 400;
		player_start_y = 600;
		// player
	}
	else if (player_or_enemy > 0)
	{
		m_ix = 0;
		m_iy = 0;
		m_pre_x = 0;
		m_pre_y = 0;
		motion = 0;
		enemy_start_x = 0;
		enemy_start_y = 0;
		move_count = 0;
		direct = DOWN;
		player = FALSE;
		status = DEAD;
		// enemy
	}
}

void Tank::Moveing(int dir)
{
	if (move_count == 0 && player == FALSE)
		direct = dir;
	else if (player == TRUE)
		direct = dir;

	m_pre_x = m_ix;
	m_pre_y = m_iy;

	if (direct == UP)
	{
		m_iy -= 2 * speed;
	}
	else if (direct == DOWN)
	{
		m_iy += 2 * speed;
	}
	else if (direct == LEFT)
	{
		m_ix -= 2 * speed;
	}
	else if (direct == RIGHT)
	{
		m_ix += 2 * speed;
	}

	if (player == TRUE)
	{
		if (motion == 0)
			motion = 1;
		else if (motion == 1)
			motion = 0;
	}
	else if (player == FALSE)
	{
		if (speed == 1)
		{
			if (motion == 0)
				motion = 1;
			else if (motion == 1)
				motion = 0;
		}
		else if (speed == 2)
		{
			if (motion == 0)
				motion = 1;
			else if (motion == 1)
				motion = 2;
			else if (motion == 2)
				motion = 0;
		}
	}


	if (player == FALSE)
		move_count++;
	if (move_count == 7)
		move_count = 0;
}

int Tank::Get_Tank_X()
{
	return (int)m_ix;
}

int Tank::Get_Tank_Y()
{
	return (int)m_iy;
}

int Tank::Get_Tank_Direct()
{
	return direct;
}

int Tank::Get_Tank_Motion()
{
	return motion;
}

int Tank::Get_Player_Check()
{
	return player;
}

int Tank::Get_Status()
{
	return status;
}

void Tank::Set_Status(int value)
{
	status = value;
}

void Tank::RollBack_pos()
{
	m_ix = m_pre_x;
	m_iy = m_pre_y;
}

void Tank::Set_Tank_X(int num)
{
	m_ix = num;
}

void Tank::Set_Tank_Y(int num)
{
	m_iy = num;
}

bool Tank::Get_Tank_Shield()
{
	return shield;
}

void Tank::Set_Tank_Shield(int value)
{
	shield = value;
}

int Tank::Get_Tank_Shield_Motion()
{
	return shield_motion;
}

void Tank::Set_Tank_Shield_Motion(int num)
{
	shield_motion = num;
}

void Tank::Set_Tank_Speed(double num)
{
	speed += num;
}

void Tank::ReSet_Speed(int num)
{
	speed = num;
}

double Tank::Get_Tank_Speed()
{
	return speed;
}

int Tank::Get_Move_Start_Check()
{
	return move_start_check;
}

void Tank::Set_Move_Start_Check(int num)
{
	move_start_check = num;
}

void Tank::Set_Tank_Direct(int value)
{
	direct = value;
}

int Tank::Get_Pre_Direct()
{
	return save_pre_direct;
}

void Tank::Set_Pre_Direct(int num)
{
	save_pre_direct = num;
}

int Tank::Get_Turn_Switch()
{
	return turn_switch;
}

void Tank::Set_Turn_Switch(int num)
{
	turn_switch = num;
}

Tank::~Tank()
{

}