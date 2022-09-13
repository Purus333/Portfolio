#include "GameSystem.h"

GameSystem* GameSystem::game_system = NULL;

GameSystem::GameSystem()
{
	game_status = FALSE;
	game_keyboard = KEY_UP;
	game_over_trigger = FALSE;
	game_stage = 1;
	flage_exist = TRUE;
	player_life = 4;
	cur_player_life = 4;
	enemy_count = 20;
	cur_time = 0;
	move_time = 0;
	missile_time = 0;
	explosion_motion = 0;
	player_explosion_time = 0;
	item_spawn_time = 0;
	stage_screen_time = 0;
	flage_rt = { 0,0,0,0 };
	game_over_tiktok = 0;
	clear_screen_time = 0;
	next_stage_trigger = FALSE;
	clear_trigger = false;

	for (int i = 0; i < 3; i++)
	{
		player_missile_on[i] = FALSE;
		m_player_missile_rt[i] = { 0,0,0,0 };
	}

	for (int i = 0; i < 4; i++)
	{
		missile_on[i] = FALSE;
		m_missile_rt[i] = { 0,0,0,0 };
		explosion_time[i] = 0;
	}

	for (int i = 0; i < 5; i++)
	{
		m_tank_rt[i] = { 0,0,0,0 };
		shield_time[i] = 0;
	}

	for (int j = 0; j < 5; j++)
		m_explosion_rt[j] = { 0,0,0,0 };

	for (int j = 0; j < 2; j++)
		m_item_rt[j] = { 0,0,0,0 };

}

void GameSystem::Init(HWND hWnd)
{
	B_A_D = new BitMap_And_Draw();
	B_A_D->Init_Bitmap(hWnd);

	MP = new Map();
	MP->Init_Map(game_stage - 1);

	for (int i = 0; i < 5; i++)
	{
		TK[i] = new Tank(); // 탱크생성
		TK[i]->Init_Tank(i);
	}

	for (int i = 0; i < 4; i++)
	{
		ME[i] = new Missile(); // 미사일생성
		ME[i]->Init_Missile();
	}

	for (int i = 0; i < 3; i++)
	{
		PLAYER_ME[i] = new Missile(); // 플레이어 미사일
		PLAYER_ME[i]->Init_Player_Missile();
	}

	IT = new Item();
	IT->Init_Item();

	flage_rt = { 463, 675, 498, 700 }; // 본거지 깃발 rect설정

	for (int i = 0; i < 5; i++)
	{
		if (TK[i]->Get_Player_Check() == FALSE)
			TK[i]->gogo_front = rand() % 70;
	}
}

void GameSystem::Title_Screen()
{
	if (game_keyboard == KEY_UP)
	{
		B_A_D->Draw(410, 50, TITLE_LOGO, TITLE_LOGO);
		B_A_D->Draw(520, 250, TITLE_MENU_01, TITLE_MENU_01);
		B_A_D->Draw(520, 350, TITLE_MENU_02, TITLE_MENU_02);

		B_A_D->Draw(400, 250, TITLE_MENU_03, TITLE_MENU_03);
		B_A_D->Draw(400, 350, TITLE_MENU_04, TITLE_MENU_04);
	}
	else if (game_keyboard == KEY_DOWN)
	{
		B_A_D->Draw(410, 50, TITLE_LOGO, TITLE_LOGO);
		B_A_D->Draw(520, 250, TITLE_MENU_01, TITLE_MENU_01);
		B_A_D->Draw(520, 350, TITLE_MENU_02, TITLE_MENU_02);

		B_A_D->Draw(400, 350, TITLE_MENU_03, TITLE_MENU_03);
		B_A_D->Draw(400, 250, TITLE_MENU_04, TITLE_MENU_04);
	}

	if (GetKeyState(VK_UP) & 0x8000)
	{
		game_keyboard = KEY_UP;
	}
	else if (GetKeyState(VK_DOWN) & 0x8000)
	{
		game_keyboard = KEY_DOWN;
	}
	else if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (game_keyboard == KEY_UP)
		{
			B_A_D->Draw_Black_BackGround(); // 타이틀화면 지우기
			game_status = GAME_OVER;
		}
		else if (game_keyboard == KEY_DOWN)
			game_status = GAME_EXIT;
	}
}

void GameSystem::Game_Screen()
{
	if (game_over_trigger == FALSE && next_stage_trigger == FALSE)
	{
		Control_Tank();

		Show_Ui();
		Show_Map();
		Set_Item();
		Show_Item();
		Create_Tank();
		Set_Upgrade_Tank();
		Show_Tank();
		Show_Shield();
		Show_Bush();
		Show_Missile();
		B_A_D->Draw_Go(); // 일반 draw함수는 렉때문에 TransparentBlt을 먼저 쓴후 BitBlt으로 출력하게 함 (draw_ready로 긁어서 draw_go로 뿌리기)

		for (int i = 0; i < 5; i++)
		{
			if (TK[i]->Get_Tank_Shield() == TRUE && clock() - shield_time[i] >= 2550)
			{
				Off_Shield(i);
				shield_time[i] = clock();
			}
		}

		if (enemy_count == 0 && game_stage < 4)
			Check_Next_Stage();

		Clear_Check();
		Game_Over_Check();
	}
	else if (game_over_trigger == TRUE && next_stage_trigger == FALSE)
	{
		Show_Game_Over();
		ReSet();
		game_status = NULL;
		game_stage = 1;
		player_life = 4;
		cur_player_life = 4;
		Sleep(2000);
	}

	if (game_over_trigger == FALSE && next_stage_trigger == TRUE && clear_trigger == false)
	{
		stage_screen_time = clock();
		B_A_D->Draw_Black_BackGround();
	}
	if (game_over_trigger == FALSE && clear_trigger == true)
	{
		clear_screen_time = clock();
		B_A_D->Draw_Black_BackGround();
	}
}

void GameSystem::Stage_Screen()
{
	if (clock() - stage_screen_time >= 3000)
	{
		ReSet();
		game_status = GAME_START;
		next_stage_trigger = FALSE;
		player_life = cur_player_life;
	}
	else
	{
		B_A_D->Draw_Ready(520, 250, STAGE_01 + game_stage - 1, STAGE_01 + game_stage - 1);
		B_A_D->Draw_Go();
	}
}

void GameSystem::Clear_Screen()
{
	if (clock() - clear_screen_time >= 3000)
	{
		game_stage = 1;
		ReSet();
		game_status = NULL;
		player_life = 4;
		cur_player_life = 4;
	}
	else
	{
		B_A_D->Draw_Ready(520, 250, UI_CLEAR, UI_CLEAR);
		B_A_D->Draw_Go();
	}
}

void GameSystem::Control_Tank()
{
	int result = 0;
	int trigger = 0;
	int tmp_direct = 0;

	Control_Update();

	///////////////// 이밑은 미사일관련함수

	Set_Missile_Start(); // (GetKeyState(VK_SPACE) & 0x8000) 입력됬을 경우 미사일설정
	Set_Missile_Rt(); // 미사일 rt를 매번 업데이트
	Missile_End_Check(); // 미사일 맵밖으로 나간것 체크

	for (int i = 0; i < 4; i++)
	{
		trigger = Block_Collision(i);
		if (trigger == TRUE)
			break;
	}

	Tank_Collision();
	Missile_Collision();
	Item_Collision();
	flage_Collision();
}

void GameSystem::Show_Map()
{
	char value = NULL;
	int num = 0;

	for (int i = 0; i < MAP_MAX; i++)
	{
		for (int j = 0; j < MAP_MAX; j++)
		{
			value = MP->Get_Map_Info(i, j);

			if (value == 'N')
				B_A_D->Draw_Ready(WIDTH * j, HEIGHT * i, BLANK, BLANK);
			else if (value == 'B')
				B_A_D->Draw_Ready(WIDTH * j, HEIGHT * i, N_BLOCK_00, N_BLOCK_00);
			else if (value == 'G')
				B_A_D->Draw_Ready(WIDTH * j, HEIGHT * i, GRAY_BLOCK, GRAY_BLOCK);
			else if (value == 'w')
				B_A_D->Draw_Ready(WIDTH * j, HEIGHT * i, WATER_BLOCK, WATER_BLOCK);
			else if (value == 'W')
				B_A_D->Draw_Ready(WIDTH * j, HEIGHT * i, WHITE_BLOCK_00, WHITE_BLOCK_00);

			m_block_rt[num] = { WIDTH * j, HEIGHT * i, (WIDTH * j) + 33, (HEIGHT * i) + 25 };
			num++;
		}
	}

	if (flage_exist == TRUE)
		B_A_D->Draw_Ready(flage_rt.left, flage_rt.top, BASEMENT_ALIVE, BASEMENT_ALIVE); // 본거지 깃발
	else if (flage_exist == FALSE)
		B_A_D->Draw_Ready(flage_rt.left, flage_rt.top, BASEMENT_DEAD, BASEMENT_DEAD);
}

void GameSystem::Show_Bush()
{
	char value;

	for (int i = 0; i < MAP_MAX; i++)
	{
		for (int j = 0; j < MAP_MAX; j++)
		{
			value = MP->Get_Map_Info(i, j);
			if (value == 'b')
				B_A_D->Draw_Ready(WIDTH * j, HEIGHT * i, BUSH_BLOCK, BUSH_BLOCK);
		}
	}
}

int GameSystem::Show_Tank()
{
	if (TK[0]->Get_Player_Check() == TRUE && TK[0]->Get_Status() == ALIVE)
	{
		if (UP == TK[0]->Get_Tank_Direct())
			B_A_D->Draw_Ready(TK[0]->player_start_x + TK[0]->Get_Tank_X(), TK[0]->player_start_y + TK[0]->Get_Tank_Y(), PLAYER_UP_00 + TK[0]->Get_Tank_Motion(), PLAYER_UP_00 + TK[0]->Get_Tank_Motion());
		else if (DOWN == TK[0]->Get_Tank_Direct())
			B_A_D->Draw_Ready(TK[0]->player_start_x + TK[0]->Get_Tank_X(), TK[0]->player_start_y + TK[0]->Get_Tank_Y(), PLAYER_DONW_00 + TK[0]->Get_Tank_Motion(), PLAYER_DONW_00 + TK[0]->Get_Tank_Motion());
		else if (LEFT == TK[0]->Get_Tank_Direct())
			B_A_D->Draw_Ready(TK[0]->player_start_x + TK[0]->Get_Tank_X(), TK[0]->player_start_y + TK[0]->Get_Tank_Y(), PLAYER_LEFT_00 + TK[0]->Get_Tank_Motion(), PLAYER_LEFT_00 + TK[0]->Get_Tank_Motion());
		else if (RIGHT == TK[0]->Get_Tank_Direct())
			B_A_D->Draw_Ready(TK[0]->player_start_x + TK[0]->Get_Tank_X(), TK[0]->player_start_y + TK[0]->Get_Tank_Y(), PLAYER_RIGHT_00 + TK[0]->Get_Tank_Motion(), PLAYER_RIGHT_00 + TK[0]->Get_Tank_Motion());
	}

	for (int i = 1; i < 5; i++)
	{
		if (TK[i]->Get_Status() == ALIVE)
		{
			if (UP == TK[i]->Get_Tank_Direct() && TK[i]->Get_Tank_Speed() == 1)
				B_A_D->Draw_Ready(TK[i]->enemy_start_x + TK[i]->Get_Tank_X(), TK[i]->enemy_start_y + TK[i]->Get_Tank_Y(), ENEMY_UP_00 + TK[i]->Get_Tank_Motion(), ENEMY_UP_00 + TK[i]->Get_Tank_Motion());
			else if (DOWN == TK[i]->Get_Tank_Direct() && TK[i]->Get_Tank_Speed() == 1)
				B_A_D->Draw_Ready(TK[i]->enemy_start_x + TK[i]->Get_Tank_X(), TK[i]->enemy_start_y + TK[i]->Get_Tank_Y(), ENEMY_DOWN_00 + TK[i]->Get_Tank_Motion(), ENEMY_DOWN_00 + TK[i]->Get_Tank_Motion());
			else if (LEFT == TK[i]->Get_Tank_Direct() && TK[i]->Get_Tank_Speed() == 1)
				B_A_D->Draw_Ready(TK[i]->enemy_start_x + TK[i]->Get_Tank_X(), TK[i]->enemy_start_y + TK[i]->Get_Tank_Y(), ENEMY_LEFT_00 + TK[i]->Get_Tank_Motion(), ENEMY_LEFT_00 + TK[i]->Get_Tank_Motion());
			else if (RIGHT == TK[i]->Get_Tank_Direct() && TK[i]->Get_Tank_Speed() == 1)
				B_A_D->Draw_Ready(TK[i]->enemy_start_x + TK[i]->Get_Tank_X(), TK[i]->enemy_start_y + TK[i]->Get_Tank_Y(), ENEMY_RIGHT_00 + TK[i]->Get_Tank_Motion(), ENEMY_RIGHT_00 + TK[i]->Get_Tank_Motion());

			///////////////////////////////////////// 밑은 강화형 탱크 움직임

			else if (UP == TK[i]->Get_Tank_Direct() && TK[i]->Get_Tank_Speed() == 2)
				B_A_D->Draw_Ready(TK[i]->enemy_start_x + TK[i]->Get_Tank_X(), TK[i]->enemy_start_y + TK[i]->Get_Tank_Y(), ENEMY_UP_00 + TK[i]->Get_Tank_Motion(), ENEMY_UP_00 + TK[i]->Get_Tank_Motion());
			else if (DOWN == TK[i]->Get_Tank_Direct() && TK[i]->Get_Tank_Speed() == 2)
				B_A_D->Draw_Ready(TK[i]->enemy_start_x + TK[i]->Get_Tank_X(), TK[i]->enemy_start_y + TK[i]->Get_Tank_Y(), ENEMY_DOWN_00 + TK[i]->Get_Tank_Motion(), ENEMY_DOWN_00 + TK[i]->Get_Tank_Motion());
			else if (LEFT == TK[i]->Get_Tank_Direct() && TK[i]->Get_Tank_Speed() == 2)
				B_A_D->Draw_Ready(TK[i]->enemy_start_x + TK[i]->Get_Tank_X(), TK[i]->enemy_start_y + TK[i]->Get_Tank_Y(), ENEMY_LEFT_00 + TK[i]->Get_Tank_Motion(), ENEMY_LEFT_00 + TK[i]->Get_Tank_Motion());
			else if (RIGHT == TK[i]->Get_Tank_Direct() && TK[i]->Get_Tank_Speed() == 2)
				B_A_D->Draw_Ready(TK[i]->enemy_start_x + TK[i]->Get_Tank_X(), TK[i]->enemy_start_y + TK[i]->Get_Tank_Y(), ENEMY_RIGHT_00 + TK[i]->Get_Tank_Motion(), ENEMY_RIGHT_00 + TK[i]->Get_Tank_Motion());
		}
	}

	Show_Tank_Collision();

	return 0;
}

void GameSystem::Show_Shield()
{
	if (TK[0]->Get_Tank_Shield() == TRUE && TK[0]->Get_Status() == ALIVE)
	{
		if (TK[0]->Get_Tank_Shield_Motion() == 0)
		{
			B_A_D->Draw_Ready(TK[0]->player_start_x + TK[0]->Get_Tank_X(), TK[0]->player_start_y + TK[0]->Get_Tank_Y(), SHIELD_00 + TK[0]->Get_Tank_Shield_Motion(), SHIELD_00 + TK[0]->Get_Tank_Shield_Motion());
			TK[0]->Set_Tank_Shield_Motion(1);
		}
		else if (TK[0]->Get_Tank_Shield_Motion() == 1)
		{
			B_A_D->Draw_Ready(TK[0]->player_start_x + TK[0]->Get_Tank_X(), TK[0]->player_start_y + TK[0]->Get_Tank_Y(), SHIELD_00 + TK[0]->Get_Tank_Shield_Motion(), SHIELD_00 + TK[0]->Get_Tank_Shield_Motion());
			TK[0]->Set_Tank_Shield_Motion(0);
		}
	}

	for (int i = 1; i < 5; i++)
	{
		if (TK[i]->Get_Tank_Shield() == TRUE)
		{
			if (TK[i]->Get_Tank_Shield_Motion() == 0)
			{
				B_A_D->Draw_Ready(TK[i]->enemy_start_x + TK[i]->Get_Tank_X(), TK[i]->enemy_start_y + TK[i]->Get_Tank_Y(), SHIELD_00 + TK[i]->Get_Tank_Shield_Motion(), SHIELD_00 + TK[i]->Get_Tank_Shield_Motion());
				TK[i]->Set_Tank_Shield_Motion(1);
			}
			else if (TK[i]->Get_Tank_Shield_Motion() == 1)
			{
				B_A_D->Draw_Ready(TK[i]->enemy_start_x + TK[i]->Get_Tank_X(), TK[i]->enemy_start_y + TK[i]->Get_Tank_Y(), SHIELD_00 + TK[i]->Get_Tank_Shield_Motion(), SHIELD_00 + TK[i]->Get_Tank_Shield_Motion());
				TK[i]->Set_Tank_Shield_Motion(0);
			}
		}
	}
}

void GameSystem::Show_Missile()
{
	for (int i = 0; i < 3; i++)
	{
		if (PLAYER_ME[i]->Get_Missile_Player() == TRUE && PLAYER_ME[i]->Get_Missile_Status() == TRUE && TK[0]->Get_Status() == ALIVE)
		{
			if (UP == PLAYER_ME[i]->Get_Missile_Direct())
				B_A_D->Draw_Ready(PLAYER_ME[i]->missile_start_x + PLAYER_ME[i]->Get_Missile_X(), PLAYER_ME[i]->missile_start_y + PLAYER_ME[i]->Get_Missile_Y(), MISSILE_UP, MISSILE_UP);
			else if (DOWN == PLAYER_ME[i]->Get_Missile_Direct())
				B_A_D->Draw_Ready(PLAYER_ME[i]->missile_start_x + PLAYER_ME[i]->Get_Missile_X(), PLAYER_ME[i]->missile_start_y + PLAYER_ME[i]->Get_Missile_Y(), MISSILE_DOWN, MISSILE_DOWN);
			else if (LEFT == PLAYER_ME[i]->Get_Missile_Direct())
				B_A_D->Draw_Ready(PLAYER_ME[i]->missile_start_x + PLAYER_ME[i]->Get_Missile_X(), PLAYER_ME[i]->missile_start_y + PLAYER_ME[i]->Get_Missile_Y(), MISSILE_LEFT, MISSILE_LEFT);
			else if (RIGHT == PLAYER_ME[i]->Get_Missile_Direct())
				B_A_D->Draw_Ready(PLAYER_ME[i]->missile_start_x + PLAYER_ME[i]->Get_Missile_X(), PLAYER_ME[i]->missile_start_y + PLAYER_ME[i]->Get_Missile_Y(), MISSILE_RIGHT, MISSILE_RIGHT);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (ME[i]->Get_Missile_Player() == FALSE && ME[i]->Get_Missile_Status() == TRUE)
		{
			if (UP == ME[i]->Get_Missile_Direct())
				B_A_D->Draw_Ready(ME[i]->missile_start_x + ME[i]->Get_Missile_X(), ME[i]->missile_start_y + ME[i]->Get_Missile_Y(), MISSILE_UP, MISSILE_UP);
			else if (DOWN == ME[i]->Get_Missile_Direct())
				B_A_D->Draw_Ready(ME[i]->missile_start_x + ME[i]->Get_Missile_X(), ME[i]->missile_start_y + ME[i]->Get_Missile_Y(), MISSILE_DOWN, MISSILE_DOWN);
			else if (LEFT == ME[i]->Get_Missile_Direct())
				B_A_D->Draw_Ready(ME[i]->missile_start_x + ME[i]->Get_Missile_X(), ME[i]->missile_start_y + ME[i]->Get_Missile_Y(), MISSILE_LEFT, MISSILE_LEFT);
			else if (RIGHT == ME[i]->Get_Missile_Direct())
				B_A_D->Draw_Ready(ME[i]->missile_start_x + ME[i]->Get_Missile_X(), ME[i]->missile_start_y + ME[i]->Get_Missile_Y(), MISSILE_RIGHT, MISSILE_RIGHT);
		}
	}
}

int GameSystem::Check_Block_Tank(int num)
{
	RECT temp;
	int _num = 0;
	char tmp;

	if (num == 0)
	{
		for (int i = 0; i < MAP_MAX; i++)
		{
			for (int j = 0; j < MAP_MAX; j++)
			{
				tmp = MP->Get_Map_Info(i, j);
				if (IntersectRect(&temp, &m_tank_rt[num], &m_block_rt[_num]) && tmp != 'N' && tmp != 'b')
					return 1;
				else if (0 >= m_tank_rt[num].left || 0 >= m_tank_rt[num].top || 918 <= m_tank_rt[num].right || 698 <= m_tank_rt[num].bottom)
					return 1;
				_num++;
			}
		}
	}
	else if (num > 0)
	{
		for (int i = 0; i < MAP_MAX; i++)
		{
			for (int j = 0; j < MAP_MAX; j++)
			{
				tmp = MP->Get_Map_Info(i, j);
				if (IntersectRect(&temp, &m_tank_rt[num], &m_block_rt[_num]) && tmp != 'N' && tmp != 'b')
					return 1;
				else if (0 >= m_tank_rt[num].left || 0 >= m_tank_rt[num].top || 918 <= m_tank_rt[num].right || 698 <= m_tank_rt[num].bottom)
					return 1;
				_num++;
			}
		}
	}

	return 0;
}

int GameSystem::Create_Tank()
{

	if (TK[0]->Get_Status() == DEAD && player_life != -1)
	{
		cur_player_life = player_life;

		if (player_life == 0)
			player_life--;
		else
		{
			TK[0]->Set_Status(ALIVE);
			m_tank_rt[0] = { TK[0]->player_start_x + TK[0]->Get_Tank_X(), TK[0]->player_start_y + TK[0]->Get_Tank_Y(), TK[0]->player_start_x + TK[0]->Get_Tank_X() + 28, TK[0]->player_start_y + TK[0]->Get_Tank_Y() + 20 };
			//// tank rect는 블럭보다 right와 bottom이 5씩 작다
			player_life--;
			On_Shield(0);
			TK[0]->ReSet_Speed(1);
			shield_time[0] = clock();
		}

	}

	if (clock() - cur_time >= 2500)
	{
		for (int i = 1; i < 5; i++)
		{
			if (TK[i]->Get_Status() == DEAD && enemy_count != 0)
			{
				TK[i]->Set_Status(ALIVE);
				Set_Enemy_Pos(i);
				On_Shield(i);
				shield_time[i] = clock();
				enemy_count--;
				break;
			}
		}
		cur_time = clock();
	}

	return 0;
}

void GameSystem::Set_Enemy_Pos(int num)
{
	char tmp;
	int tmp_x = 0;
	int tmp_y = 0;
	int value = 0;
	int trigger = FALSE;

	while (1)
	{
		trigger = FALSE;
		value = rand() % 27;
		tmp = MP->Get_Map_Info(0, value);

		if (tmp == 'N')
		{
			for (int i = 1; i < 5; i++)
			{
				if (m_tank_rt[i].left == m_block_rt[value].left && m_tank_rt[i].top == m_block_rt[value].top)
					trigger = TRUE;
			}

			if (trigger == FALSE)
			{
				m_tank_rt[num] = { m_block_rt[value].left, m_block_rt[value].top + 2, m_block_rt[value].left + 28, m_block_rt[value].top + 2 + 20};
				TK[num]->enemy_start_x = m_tank_rt[num].left;
				TK[num]->enemy_start_y = m_tank_rt[num].top;
				break;
			}
		}
	}
}

void GameSystem::Set_Missile_Start()
{
	for (int i = 0; i < 3; i++)
	{
		if (PLAYER_ME[i]->Get_Missile_Status() == FALSE)
		{
			if (TK[0]->Get_Tank_Direct() == UP)
			{
				PLAYER_ME[i]->missile_start_x = m_tank_rt[0].left + 13;
				PLAYER_ME[i]->missile_start_y = m_tank_rt[0].top;
			}
			else if (TK[0]->Get_Tank_Direct() == DOWN)
			{
				PLAYER_ME[i]->missile_start_x = m_tank_rt[0].left + 15;
				PLAYER_ME[i]->missile_start_y = m_tank_rt[0].top + 16;
			}
			else if (TK[0]->Get_Tank_Direct() == LEFT)
			{
				PLAYER_ME[i]->missile_start_x = m_tank_rt[0].left + 5;
				PLAYER_ME[i]->missile_start_y = m_tank_rt[0].top + 12;
			}
			else if (TK[0]->Get_Tank_Direct() == RIGHT)
			{
				PLAYER_ME[i]->missile_start_x = m_tank_rt[0].left + 19;
				PLAYER_ME[i]->missile_start_y = m_tank_rt[0].top + 9;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (ME[i]->Get_Missile_Status() == FALSE)
		{
			if (TK[i + 1]->Get_Tank_Direct() == UP)
			{
				ME[i]->missile_start_x = m_tank_rt[i + 1].left + 13;
				ME[i]->missile_start_y = m_tank_rt[i + 1].top;
			}
			else if (TK[i + 1]->Get_Tank_Direct() == DOWN)
			{
				ME[i]->missile_start_x = m_tank_rt[i + 1].left + 15;
				ME[i]->missile_start_y = m_tank_rt[i + 1].top + 16;
			}
			else if (TK[i + 1]->Get_Tank_Direct() == LEFT)
			{
				ME[i]->missile_start_x = m_tank_rt[i + 1].left + 5;
				ME[i]->missile_start_y = m_tank_rt[i + 1].top + 12;
			}
			else if (TK[i + 1]->Get_Tank_Direct() == RIGHT)
			{
				ME[i]->missile_start_x = m_tank_rt[i + 1].left + 19;
				ME[i]->missile_start_y = m_tank_rt[i + 1].top + 9;
			}
		}

		if (TK[i + 1]->Get_Status() == ALIVE)
			missile_on[i] = TRUE;
	}
}

void GameSystem::Set_Missile_Rt()
{
	for (int i = 0; i < 3; i++)
	{
		if (PLAYER_ME[i]->Get_Missile_Status() == TRUE)
		{
			if (TK[0]->Get_Tank_Direct() == UP)
				m_player_missile_rt[i] = { PLAYER_ME[i]->missile_start_x + PLAYER_ME[i]->Get_Missile_X(), PLAYER_ME[i]->missile_start_y + PLAYER_ME[i]->Get_Missile_Y(), PLAYER_ME[i]->missile_start_x + PLAYER_ME[i]->Get_Missile_X() + 10, PLAYER_ME[i]->missile_start_y + PLAYER_ME[i]->Get_Missile_Y() + 7 };
			else if (TK[0]->Get_Tank_Direct() == DOWN)
				m_player_missile_rt[i] = { PLAYER_ME[i]->missile_start_x + PLAYER_ME[i]->Get_Missile_X(), PLAYER_ME[i]->missile_start_y + PLAYER_ME[i]->Get_Missile_Y(), PLAYER_ME[i]->missile_start_x + PLAYER_ME[i]->Get_Missile_X() + 10, PLAYER_ME[i]->missile_start_y + PLAYER_ME[i]->Get_Missile_Y() + 7 };
			else if (TK[0]->Get_Tank_Direct() == LEFT)
				m_player_missile_rt[i] = { PLAYER_ME[i]->missile_start_x + PLAYER_ME[i]->Get_Missile_X(), PLAYER_ME[i]->missile_start_y + PLAYER_ME[i]->Get_Missile_Y(), PLAYER_ME[i]->missile_start_x + PLAYER_ME[i]->Get_Missile_X() + 10, PLAYER_ME[i]->missile_start_y + PLAYER_ME[i]->Get_Missile_Y() + 7 };
			else if (TK[0]->Get_Tank_Direct() == RIGHT)
				m_player_missile_rt[i] = { PLAYER_ME[i]->missile_start_x + PLAYER_ME[i]->Get_Missile_X(), PLAYER_ME[i]->missile_start_y + PLAYER_ME[i]->Get_Missile_Y(), PLAYER_ME[i]->missile_start_x + PLAYER_ME[i]->Get_Missile_X() + 10, PLAYER_ME[i]->missile_start_y + PLAYER_ME[i]->Get_Missile_Y() + 7 };
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (ME[i]->Get_Missile_Status() == TRUE)
		{
			if (TK[i + 1]->Get_Tank_Direct() == UP)
				m_missile_rt[i] = { ME[i]->missile_start_x + ME[i]->Get_Missile_X(), ME[i]->missile_start_y + ME[i]->Get_Missile_Y(), ME[i]->missile_start_x + ME[i]->Get_Missile_X() + 10, ME[i]->missile_start_y + ME[i]->Get_Missile_Y() + 7 };
			else if (TK[i + 1]->Get_Tank_Direct() == DOWN)
				m_missile_rt[i] = { ME[i]->missile_start_x + ME[i]->Get_Missile_X(), ME[i]->missile_start_y + ME[i]->Get_Missile_Y(), ME[i]->missile_start_x + ME[i]->Get_Missile_X() + 10, ME[i]->missile_start_y + ME[i]->Get_Missile_Y() + 7 };
			else if (TK[i + 1]->Get_Tank_Direct() == LEFT)
				m_missile_rt[i] = { ME[i]->missile_start_x + ME[i]->Get_Missile_X(), ME[i]->missile_start_y + ME[i]->Get_Missile_Y(), ME[i]->missile_start_x + ME[i]->Get_Missile_X() + 10, ME[i]->missile_start_y + ME[i]->Get_Missile_Y() + 7 };
			else if (TK[i + 1]->Get_Tank_Direct() == RIGHT)
				m_missile_rt[i] = { ME[i]->missile_start_x + ME[i]->Get_Missile_X(), ME[i]->missile_start_y + ME[i]->Get_Missile_Y(), ME[i]->missile_start_x + ME[i]->Get_Missile_X() + 10, ME[i]->missile_start_y + ME[i]->Get_Missile_Y() + 7 };
		}
	}
}

void GameSystem::Missile_End_Check()
{
	for (int i = 0; i < 3; i++)
	{
		if (PLAYER_ME[i]->Get_Missile_Status() == TRUE)
		{
			if (0 >= m_player_missile_rt[i].left || 0 >= m_player_missile_rt[i].top || 918 <= m_player_missile_rt[i].right || 698 <= m_player_missile_rt[i].bottom)
				Player_Missile_Dead(i);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (ME[i]->Get_Missile_Status() == TRUE)
		{
			if (0 >= m_missile_rt[i].left || 0 >= m_missile_rt[i].top || 918 <= m_missile_rt[i].right || 698 <= m_missile_rt[i].bottom)
				Missile_Dead(i);
		}
	}
}

void GameSystem::Player_Missile_Dead(int num)
{
	player_missile_on[num] = FALSE;
	PLAYER_ME[num]->Set_Missile_Status(FALSE);
	m_player_missile_rt[num] = { 0,0,0,0 };
	PLAYER_ME[num]->missile_start_x = 0;
	PLAYER_ME[num]->missile_start_y = 0;
	PLAYER_ME[num]->Set_Missile_X(0);
	PLAYER_ME[num]->Set_Missile_Y(0); // all 초기화
}

void GameSystem::Missile_Dead(int num)
{
	missile_on[num] = FALSE;
	ME[num]->Set_Missile_Status(FALSE);
	m_missile_rt[num] = { 0,0,0,0 };
	ME[num]->missile_start_x = 0;
	ME[num]->missile_start_y = 0;
	ME[num]->Set_Missile_X(0);
	ME[num]->Set_Missile_Y(0); // all 초기화
}

int GameSystem::Block_Collision(int num)
{
	RECT temp;
	int _nums = 0;
	char tmp;

	for (int i = 0; i < MAP_MAX; i++)
	{
		for (int j = 0; j < MAP_MAX; j++)
		{
			tmp = MP->Get_Map_Info(i, j);
			if (IntersectRect(&temp, &m_missile_rt[num], &m_block_rt[_nums]) && tmp != 'N' && tmp != 'b' && tmp != 'w' && tmp != 'W')
			{
				MP->Set_Map_Info(i, j, 'N');
				Missile_Dead(num);
				return 1;
			}
			else if (IntersectRect(&temp, &m_missile_rt[num], &m_block_rt[_nums]) && tmp == 'W') // 화이트블럭은 무적
			{
				Missile_Dead(num);
				return 1;
			}
			else if (IntersectRect(&temp, &m_player_missile_rt[num], &m_block_rt[_nums]) && tmp != 'N' && tmp != 'b' && tmp != 'w' && tmp != 'W')
			{
				MP->Set_Map_Info(i, j, 'N');
				Player_Missile_Dead(num);
				return 1;
			}
			else if (IntersectRect(&temp, &m_player_missile_rt[num], &m_block_rt[_nums]) && tmp == 'W') // 화이트블럭은 무적
			{
				Player_Missile_Dead(num);
				return 1;
			}
			_nums++;
		}
	}
	return 0;
}

void GameSystem::Tank_Collision()
{
	RECT temp;

	for (int i = 0; i < 3; i++) // 플레이어 탄환 갯수
	{
		for (int j = 1; j < 5; j++) // 적 탱크 갯수
		{
			if (IntersectRect(&temp, &m_player_missile_rt[i], &m_tank_rt[j]) && TK[j]->Get_Tank_Shield() == FALSE)
			{
				m_explosion_rt[j] = m_tank_rt[j]; // 폭발할 rt 저장
				Player_Missile_Dead(i);
				Tank_Dead(j);
				return;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (IntersectRect(&temp, &m_missile_rt[i], &m_tank_rt[0]) && TK[0]->Get_Tank_Shield() == FALSE)
		{
			m_explosion_rt[0] = m_tank_rt[0];
			Missile_Dead(i);
			Tank_Dead(0);
			game_over_tiktok = clock();
			return;
		}
	}
}

void GameSystem::Tank_Dead(int num)
{
	TK[num]->Set_Status(DEAD);
	m_tank_rt[num] = { 0,0,0,0 };
	TK[num]->enemy_start_x = 0;
	TK[num]->enemy_start_y = 0;
	TK[num]->Set_Tank_X(0);
	TK[num]->Set_Tank_Y(0);
	shield_time[num] = clock();
	TK[num]->Set_Move_Start_Check(FALSE);
	TK[num]->Set_Turn_Switch(FALSE);

	if (TK[num]->Get_Player_Check() == FALSE)
	{
		while (st[num - 1].empty() == FALSE)
		{
			st[num - 1].pop();
		}
	}

	if (TK[num]->Get_Player_Check() == FALSE)
		TK[num]->gogo_front = rand() % 70;
}

void GameSystem::Missile_Collision()
{
	RECT temp;

	for (int i = 0; i < 3; i++) // 플레이어 탄환
	{
		for (int j = 0; j < 4; j++) // 적 탄환
		{
			if (IntersectRect(&temp, &m_player_missile_rt[i], &m_missile_rt[j]))
			{
				Player_Missile_Dead(i);
				Missile_Dead(j);
				return;
			}
		}
	}
}

void GameSystem::Show_Tank_Collision()
{
	if (m_explosion_rt[0].left != 0)
	{
		if (explosion_motion < 3)
			B_A_D->Draw_Ready(m_explosion_rt[0].left, m_explosion_rt[0].top, EXPLOSION_00 + explosion_motion, EXPLOSION_00 + explosion_motion);
		else
			B_A_D->Draw_Ready(m_explosion_rt[0].left - 20, m_explosion_rt[0].top - 9, EXPLOSION_00 + explosion_motion, EXPLOSION_00 + explosion_motion);

		if (clock() - player_explosion_time >= 90)
		{
			explosion_motion++;
			player_explosion_time = clock();
		}

		if (explosion_motion == 5)
		{
			m_explosion_rt[0] = { 0,0,0,0 };
			explosion_motion = 0;
		}
	}

	for (int i = 1; i < 5; i++)
	{
		if (m_explosion_rt[i].left != 0)
		{
			if (explosion_motion < 3)
				B_A_D->Draw_Ready(m_explosion_rt[i].left, m_explosion_rt[i].top, EXPLOSION_00 + explosion_motion, EXPLOSION_00 + explosion_motion);
			else
				B_A_D->Draw_Ready(m_explosion_rt[i].left - 20, m_explosion_rt[i].top - 9, EXPLOSION_00 + explosion_motion, EXPLOSION_00 + explosion_motion);

			if (clock() - explosion_time[i - 1] >= 90)
			{
				explosion_motion++;
				explosion_time[i - 1] = clock();
			}

			if (explosion_motion == 5)
			{
				m_explosion_rt[i] = { 0,0,0,0 };
				explosion_motion = 0;
			}
		}
	}
}

void GameSystem::On_Shield(int num)
{
	if (TK[num]->Get_Tank_Shield() == FALSE)
		TK[num]->Set_Tank_Shield(TRUE);
}

void GameSystem::Off_Shield(int num)
{
	if (TK[num]->Get_Tank_Shield() == TRUE)
		TK[num]->Set_Tank_Shield(FALSE);
}

void GameSystem::Set_Item()
{
	int spawn_chance = 0;
	int item_number = 0;
	int tmp_y = 0;
	int tmp_x = 0;
	char tmp;

	if (clock() - item_spawn_time >= 1000)
	{
		spawn_chance = rand() % 5;

		if (spawn_chance == 3)
		{
			item_number = rand() % 2;
			if (IT->item_info[item_number].on_off == FALSE)
			{
				tmp_y = rand() % MAP_MAX;
				tmp_x = rand() % MAP_MAX;
				tmp = MP->Get_Map_Info(tmp_y, tmp_x);

				if (tmp == 'N' && (MAP_MAX * (tmp_y + 1)) - (MAP_MAX - tmp_x) <= BLOCK_MAX - 1)
				{
					IT->item_info[item_number].on_off = TRUE;
					m_item_rt[item_number] = m_block_rt[(MAP_MAX * (tmp_y + 1)) - (MAP_MAX - tmp_x)];
				}
			}
		}
		item_spawn_time = clock();
	}

}

void GameSystem::Show_Item()
{
	for (int i = 0; i < 2; i++)
	{
		if (IT->item_info[i].on_off == TRUE)
		{
			if (i == 0)
				B_A_D->Draw_Ready(m_item_rt[i].left, m_item_rt[i].top, ITEM_SHIELD, ITEM_SHIELD);
			else
				B_A_D->Draw_Ready(m_item_rt[i].left, m_item_rt[i].top, ITEM_SPEED, ITEM_SPEED);
		}
	}
}

void GameSystem::Item_Collision()
{
	RECT temp;

	for (int i = 0; i < 2; i++)
	{
		if (IntersectRect(&temp, &m_item_rt[i], &m_tank_rt[0]) && IT->item_info[i].on_off == TRUE)
		{
			if (i == 0) // 쉴드
			{
				IT->item_info[0].on_off = FALSE;
				m_item_rt[0] = { -1,-1,-1,-1 };
				On_Shield(0);
				shield_time[0] = clock();
			}
			else if (i == 1) // 스피드업
			{
				IT->item_info[1].on_off = FALSE;
				m_item_rt[1] = { -1,-1,-1,-1 };
				TK[0]->Set_Tank_Speed(0.2);
			}
		}
	}
}

void GameSystem::Show_Ui()
{
	int num1 = 0;
	int num2 = 0;

	B_A_D->Draw_Detail_Ready(910, 0, UI, UI, 3, 17); // 회색화면

	for (int i = 0; i < enemy_count; i++)
	{
		if (i == 9)
		{
			B_A_D->Draw_Ready(970 + num1, 40 + num2, ENEMY_LIFE, ENEMY_LIFE);
			num1 += 20;
			num2 = 0;
		}
		else
		{
			B_A_D->Draw_Ready(970 + num1, 40 + num2, ENEMY_LIFE, ENEMY_LIFE);
			num2 += 20;
		}
	}
	num1 = 0;
	num2 = 0;
	for (int i = 0; i < player_life; i++)
	{
		B_A_D->Draw_Ready(975 + num1, 290 + num2, PLAYER_LIFE, PLAYER_LIFE); // 플레이어 체력
		num1 += 20;
	}
	num1 = 0;
	num2 = 0;
	if (flage_exist == TRUE)
	B_A_D->Draw_Ready(975 + num1, 350 + num2, STAGE_ICON, STAGE_ICON); // 깃발ui
}

void GameSystem::flage_Collision()
{
	RECT temp;

	for (int i = 0; i < 4; i++)
	{
		if (IntersectRect(&temp, &m_missile_rt[i], &flage_rt) && flage_exist == TRUE)
		{
			flage_exist = FALSE;
			Missile_Dead(i);
			game_over_tiktok = clock(); // 깃발 부숴진것을 몇초간만 보여주기 위해 세팅
		}
	}
}

void GameSystem::Game_Over_Check()
{
	if (flage_exist == FALSE || player_life == -1 && TK[0]->Get_Status() == DEAD)
	{
		if (clock() - game_over_tiktok >= 3000)
		{
			game_over_trigger = TRUE;
		}
	}
}

void GameSystem::Show_Game_Over()
{
	B_A_D->Draw_Black_BackGround();
	B_A_D->Draw_Ready(520, 250, GAME_OVERS, GAME_OVERS);
	B_A_D->Draw_Go();
}

void GameSystem::ReSet()
{
	game_status = FALSE;
	game_keyboard = KEY_UP;
	game_over_trigger = FALSE;
	flage_exist = TRUE;
	enemy_count = 20;
	cur_time = 0;
	move_time = 0;
	missile_time = 0;
	explosion_motion = 0;
	player_explosion_time = 0;
	item_spawn_time = 0;
	stage_screen_time = 0;
	flage_rt = { 0,0,0,0 };
	game_over_tiktok = 0;
	clear_screen_time = 0;
	next_stage_trigger = FALSE;
	clear_trigger = false;

	for (int i = 0; i < 3; i++)
	{
		player_missile_on[i] = FALSE;
		m_player_missile_rt[i] = { 0,0,0,0 };
	}

	for (int i = 0; i < 4; i++)
	{
		missile_on[i] = FALSE;
		m_missile_rt[i] = { 0,0,0,0 };
		explosion_time[i] = 0;
	}

	for (int i = 0; i < 5; i++)
	{
		m_tank_rt[i] = { 0,0,0,0 };
		shield_time[i] = 0;
	}

	for (int j = 0; j < 5; j++)
		m_explosion_rt[j] = { 0,0,0,0 };

	for (int j = 0; j < 2; j++)
		m_item_rt[j] = { 0,0,0,0 };

	///////////////////////////////

	MP = new Map();
	MP->Init_Map(game_stage - 1);

	for (int i = 0; i < 5; i++)
	{
		TK[i] = new Tank(); // 탱크생성
		TK[i]->Init_Tank(i);
	}

	for (int i = 0; i < 4; i++)
	{
		ME[i] = new Missile(); // 미사일생성
		ME[i]->Init_Missile();
	}

	for (int i = 0; i < 3; i++)
	{
		PLAYER_ME[i] = new Missile(); // 플레이어 미사일
		PLAYER_ME[i]->Init_Player_Missile();
	}

	IT = new Item();
	IT->Init_Item();

	flage_rt = { 463, 675, 498, 700 }; // 본거지 깃발 rect설정

	for (int i = 1; i < 5; i++)
	{
		while (st[i - 1].empty() == FALSE)
		{
			st[i - 1].pop();
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (TK[i]->Get_Player_Check() == FALSE)
			TK[i]->gogo_front = rand() % 70;
	}
}

void GameSystem::Check_Next_Stage()
{
	for (int i = 1; i < 5; i++)
	{
		if (TK[i]->Get_Status() == ALIVE)
		{
			next_stage_trigger = FALSE;
			break;
		}
		else
		{
			next_stage_trigger = TRUE;
		}
	}

	if (next_stage_trigger == TRUE)
	{
		game_stage++;
		if (game_stage != 4)
			game_status = GAME_OVER;
	}
}

void GameSystem::Clear_Check()
{
	if (game_stage == 4)
		clear_trigger = true;
}

void GameSystem::Set_Upgrade_Tank()
{
	if (game_stage == 2)
	{
		if (TK[3]->Get_Status() == ALIVE)
			TK[3]->ReSet_Speed(2);
	}
	else if (game_stage == 3)
	{
		for (int i = 3; i < 5; i++)
			if (TK[i]->Get_Status() == ALIVE)
				TK[i]->ReSet_Speed(2);
	}
}

void GameSystem::Control_Update()
{
	for (int i = 0; i < 5; i++)
		if (TK[i]->Get_Player_Check() == TRUE)
			Player_Update(i);
	Enemy_Update();
}

void GameSystem::Player_Update(int num)
{
	int result = 0;
	int trigger = 0;
	int tmp_direct = 0;

	if (GetKeyState(VK_UP) & 0x8000)
	{
		result = Check_Block_Tank(num);
		if (result == 0)
			TK[num]->Moveing(UP);
		else
			TK[num]->RollBack_pos();
		// 블럭과 부딫치는지 비교, 맵끝인지 비교 후 moving으로 가게하기
	}
	else if (GetKeyState(VK_DOWN) & 0x8000)
	{
		result = Check_Block_Tank(num);
		if (result == 0)
			TK[num]->Moveing(DOWN);
		else
			TK[num]->RollBack_pos();
	}
	else if (GetKeyState(VK_LEFT) & 0x8000)
	{
		result = Check_Block_Tank(num);
		if (result == 0)
			TK[num]->Moveing(LEFT);
		else
			TK[num]->RollBack_pos();
	}
	else if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		result = Check_Block_Tank(num);
		if (result == 0)
			TK[num]->Moveing(RIGHT);
		else
			TK[num]->RollBack_pos();
	}
	else if (GetKeyState(VK_SPACE) & 0x8000)
	{
		for (int i = 0; i < 3; i++)
		{
			if (player_missile_on[i] == FALSE && clock() - missile_time >= 600)
			{
				player_missile_on[i] = TRUE;
				missile_time = clock();
				break;
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (player_missile_on[i] == TRUE)
			PLAYER_ME[i]->Move_Missile(TK[num]->Get_Tank_Direct());
	}

	m_tank_rt[num] = { TK[num]->player_start_x + TK[num]->Get_Tank_X(), TK[num]->player_start_y + TK[num]->Get_Tank_Y(), TK[num]->player_start_x + TK[num]->Get_Tank_X() + 28, TK[num]->player_start_y + TK[num]->Get_Tank_Y() + 20 };
}

void GameSystem::Enemy_Update()
{
	int result = 0;
	int trigger = 0;
	int tmp_direct = 0;

	if (clock() - move_time >= 10)
	{
		for (int i = 0; i < 5; i++)
		{
			result = Check_Block_Tank(i);

			if (TK[i]->Get_Status() == ALIVE && TK[i]->Get_Player_Check() == FALSE && TK[i]->gogo_front != 0)
			{
				TK[i]->gogo_front--;
				TK[i]->Set_Tank_Direct(DOWN);
				TK[i]->Moveing(TK[i]->Get_Tank_Direct());
				m_tank_rt[i] = { TK[i]->enemy_start_x + TK[i]->Get_Tank_X(), TK[i]->enemy_start_y + TK[i]->Get_Tank_Y(), TK[i]->enemy_start_x + TK[i]->Get_Tank_X() + 28, TK[i]->enemy_start_y + TK[i]->Get_Tank_Y() + 20 };
			}
			else if (TK[i]->Get_Status() == ALIVE && TK[i]->Get_Player_Check() == FALSE && TK[i]->gogo_front == 0)
			{
				if (TK[i]->Get_Move_Start_Check() == FALSE && TK[i]->level_up_mode == FALSE)
				{
					while (TK[i]->Get_Move_Start_Check() == FALSE)
					{
						tmp_direct = rand() % 3 + 12;

						if (tmp_direct != TK[i]->Get_Pre_Direct())
						{
							TK[i]->Set_Tank_Direct(tmp_direct); // 하우좌 로 움직인다
							TK[i]->Set_Move_Start_Check(TRUE);
						}
					}
				}

				if (TK[i]->level_up_mode == FALSE)
					result = Check_Block_Tank(i); // 맵끝에 닿거나 블럭에 닿으면 1을 리턴한다
				else
					result = 22;

				if (result == 0 && TK[i]->Get_Turn_Switch() == FALSE)
				{
					TK[i]->Set_Pre_Direct(TK[i]->Get_Tank_Direct());
					st[i - 1].push(m_tank_rt[i]); // 현재좌표를 스택에 넣는다
					TK[i]->Moveing(TK[i]->Get_Tank_Direct());
					m_tank_rt[i] = { TK[i]->enemy_start_x + TK[i]->Get_Tank_X(), TK[i]->enemy_start_y + TK[i]->Get_Tank_Y(), TK[i]->enemy_start_x + TK[i]->Get_Tank_X() + 28, TK[i]->enemy_start_y + TK[i]->Get_Tank_Y() + 20 };
				}
				else if (result == 1)
					TK[i]->Set_Turn_Switch(TRUE);

				if (TK[i]->Get_Turn_Switch() == TRUE)
				{
					if (st[i - 1].empty() == TRUE)
					{
						TK[i]->Set_Turn_Switch(FALSE);
						TK[i]->Set_Move_Start_Check(FALSE);
						if (game_stage > 1)
							TK[i]->level_up_mode = TRUE;
						break;
					}

					st[i - 1].pop();

					if (TK[i]->Get_Pre_Direct() == LEFT)
					{
						TK[i]->Set_Tank_Direct(RIGHT);
						TK[i]->Moveing(TK[i]->Get_Tank_Direct());
					}
					else if (TK[i]->Get_Pre_Direct() == RIGHT)
					{
						TK[i]->Set_Tank_Direct(LEFT);
						TK[i]->Moveing(TK[i]->Get_Tank_Direct());
					}
					else if (TK[i]->Get_Pre_Direct() == UP)
					{
						TK[i]->Set_Tank_Direct(DOWN);
						TK[i]->Moveing(TK[i]->Get_Tank_Direct());
					}
					else if (TK[i]->Get_Pre_Direct() == DOWN)
					{
						TK[i]->Set_Tank_Direct(UP);
						TK[i]->Moveing(TK[i]->Get_Tank_Direct());
					}
					m_tank_rt[i] = { TK[i]->enemy_start_x + TK[i]->Get_Tank_X(), TK[i]->enemy_start_y + TK[i]->Get_Tank_Y(), TK[i]->enemy_start_x + TK[i]->Get_Tank_X() + 28, TK[i]->enemy_start_y + TK[i]->Get_Tank_Y() + 20 };
				}

				if (result == 22 && TK[i]->up_count == 0 && TK[i]->down_count == 0 && TK[i]->left_count == 0 && TK[i]->right_count == 0)
				{
					if (m_tank_rt[0].top > m_tank_rt[i].top)
					{
						if (m_tank_rt[0].left > m_tank_rt[i].left)
						{
							TK[i]->down_count = 23;
							TK[i]->right_count = 23;
						}
						else if (m_tank_rt[0].left < m_tank_rt[i].left)
						{
							TK[i]->down_count = 23;
							TK[i]->left_count = 23;
						}
					}
					else if (m_tank_rt[0].top < m_tank_rt[i].top)
					{
						if (m_tank_rt[0].left > m_tank_rt[i].left)
						{
							TK[i]->up_count = 23;
							TK[i]->right_count = 23;
						}
						else if (m_tank_rt[0].left < m_tank_rt[i].left)
						{
							TK[i]->up_count = 23;
							TK[i]->left_count = 23;
						}
					}
				}

				if (TK[i]->up_count > 0 || TK[i]->down_count > 0 || TK[i]->left_count > 0 || TK[i]->right_count > 0)
				{
					result = Check_Block_Tank(i);

					if (result == 1)
						TK[i]->RollBack_pos();

					else
					{
						if (TK[i]->up_count > 0)
						{
							TK[i]->Set_Tank_Direct(UP);
							TK[i]->Moveing(TK[i]->Get_Tank_Direct());
							TK[i]->up_count--;
						}
						else if (TK[i]->down_count > 0)
						{
							TK[i]->Set_Tank_Direct(DOWN);
							TK[i]->Moveing(TK[i]->Get_Tank_Direct());
							TK[i]->down_count--;
						}
						else if (TK[i]->left_count > 0)
						{
							TK[i]->Set_Tank_Direct(LEFT);
							TK[i]->Moveing(TK[i]->Get_Tank_Direct());
							TK[i]->left_count--;
						}
						else if (TK[i]->right_count > 0)
						{
							TK[i]->Set_Tank_Direct(RIGHT);
							TK[i]->Moveing(TK[i]->Get_Tank_Direct());
							TK[i]->right_count--;
						}
					}

					m_tank_rt[i] = { TK[i]->enemy_start_x + TK[i]->Get_Tank_X(), TK[i]->enemy_start_y + TK[i]->Get_Tank_Y(), TK[i]->enemy_start_x + TK[i]->Get_Tank_X() + 28, TK[i]->enemy_start_y + TK[i]->Get_Tank_Y() + 20 };
					if (TK[i]->up_count == 0 && TK[i]->down_count == 0 && TK[i]->left_count == 0 && TK[i]->right_count == 0)
						TK[i]->level_up_mode = FALSE;
				}
			}

		move_time = clock();
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (missile_on[i] == TRUE)
			ME[i]->Move_Missile(TK[i + 1]->Get_Tank_Direct());
	}
}

GameSystem::~GameSystem()
{
	//// 동적할당 해제를 해줘야 각 클래스의 소멸자에서 삭제된다
	delete B_A_D;
	delete MP;

	for (int i = 0; i < 5; i++)
		delete TK[i];

	for (int i = 0; i < 4; i++)
		delete ME[i];

	for (int i = 0; i < 3; i++)
		delete PLAYER_ME[i];

	delete IT;

}