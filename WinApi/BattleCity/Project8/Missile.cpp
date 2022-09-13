#include "Missile.h"

Missile::Missile()
{
	m_ix = 0;
	m_iy = 0;
	m_direct = 0;
	m_check_player = 0;
	m_status = FALSE;
	missile_start_x = 0;
	missile_start_y = 0;
}

void Missile::Init_Missile()
{
	m_ix = 0;
	m_iy = 0;
	m_direct = 0;
	m_check_player = FALSE;
	m_status = FALSE;
}

void Missile::Init_Player_Missile()
{
	m_ix = 0;
	m_iy = 0;
	m_direct = 0;
	m_check_player = TRUE;
	m_status = FALSE;
}

int Missile::Move_Missile(int dir)
{
	if (m_status == FALSE)
	{
		m_direct = dir;
		m_status = TRUE;
	}

	if (m_status == TRUE)
	{
		if (m_direct == UP)
		{
			m_iy -= 5;
		}
		else if (m_direct == DOWN)
		{
			m_iy += 5;
		}
		else if (m_direct == LEFT)
		{
			m_ix -= 5;
		}
		else if (m_direct == RIGHT)
		{
			m_ix += 5;
		}
	}

	return 0;
}

int Missile::Get_Missile_X()
{
	return m_ix;
}

int Missile::Get_Missile_Y()
{
	return m_iy;
}

void Missile::Set_Missile_X(int value)
{
	m_ix = value;
}

void Missile::Set_Missile_Y(int value)
{
	m_iy = value;
}

int Missile::Get_Missile_Status()
{
	return m_status;
}

void Missile::Set_Missile_Status(int value)
{
	m_status = value;
}

int Missile::Get_Missile_Player()
{
	return m_check_player;
}

int Missile::Get_Missile_Direct()
{
	return m_direct;
}

Missile::~Missile()
{

}