#include "Network.h"

int Network::login_fail = 0;
CHESS_PIECE Network::m_piece;
CHESS_PIECE Network::m_recv_piece;
bool Network::m_player_connect = false;
bool Network::m_player_turn = false;
bool Network::m_player_done_check = false;
int Network::m_set_player_color;
int Network::m_color_set_check = false;
bool Network::m_recv_check = false;
//// LINK 오류방지

Network::Network()
{

}

void Network::Init_Network(HWND hWnd)
{
	m_hWnd = hWnd;
	
	if (WSAStartup(MAKEWORD(2, 2), &m_WsaData) != 0)
	{
		cout << "WSAStartup() Error!" << endl;
		system("pause");
	}

	m_ServerSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&m_ServerAddr, 0, sizeof(m_ServerAddr));
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_ServerAddr.sin_port = htons(9001);

	if (connect(m_ServerSock, (SOCKADDR*)&m_ServerAddr, sizeof(m_ServerAddr)) == SOCKET_ERROR)
	{
		cout << "Connet() Error!" << endl;
		system("pause");
		return;
	}

	m_player_connect = true;

	m_SendThread = (HANDLE)_beginthreadex(NULL, 0, Send, (void*)&m_ServerSock, 0, NULL);
	m_RecvThread = (HANDLE)_beginthreadex(NULL, 0, Recv, (void*)&m_ServerSock, 0, NULL);
}

unsigned WINAPI Network::Send(void *arg)
{
	SOCKET sock = *((SOCKET*)arg);
	int value = 0;
	int connect_check = 0;

	while (1)
	{
		if (connect_check == false)
		{
			send(sock, (char*)&m_player_connect, sizeof(m_player_connect), 0);
			connect_check = true;
		}
		else if (m_player_done_check == true)
		{
			send(sock, (char*)&m_piece, sizeof(m_piece), 0);
			m_player_done_check = false;
			m_player_turn = false;
		}
	}

	return 0;
}

unsigned WINAPI Network::Recv(void *arg)
{
	SOCKET sock = *((SOCKET*)arg);
	int value = 0;
	char buf[BUFSIZ];
	int tmp = 0;

	while (1)
	{
		value = recv(sock, buf, sizeof(buf), 0);

		if (value == SOCKET_ERROR)
			break;
		if (value == 0)
			break;

		buf[value] = '\n';  // 끝을 구별하기 위해

		if (value == 4 && m_color_set_check == false) // 처음때만 설정해준다
		{
			m_set_player_color = (int)*buf; // *포인터 붙여줘야한다*
			m_color_set_check = true;

			if (m_set_player_color == WHITE)
				m_player_turn = true;
			else if (m_set_player_color == BLACK)
				m_player_turn = false;
		}

		else if (value == sizeof(CHESS_PIECE) && m_recv_check == false) // 상대방 체스말의 값
		{
			CHESS_PIECE *tmp_piece;
			tmp_piece = (CHESS_PIECE*)buf;

			m_recv_piece.piece_name = tmp_piece->piece_name;
			m_recv_piece.piece_num = tmp_piece->piece_num;
			m_recv_piece.x = tmp_piece->x;
			m_recv_piece.y = tmp_piece->y;

			m_recv_check = true; // 다음턴까지 true
		}

		else if (value == sizeof(bool))
		{
			login_fail = 10;
		}
		else if (value == sizeof(int))
		{
			tmp = (bool)buf;
			if (tmp == true)
			{
				login_fail = 100;
			}
			else
				break;
			tmp = 0;
		}
	}

	return 0;
}

void Network::Release_Network()
{
	//// 따로 분류해줘야 오브젝트를 기다리지않고 계속돌아간다
	//// 대신 종료시에 꼭 이 함수를 사용해서 해제 해줘야 한다
	//WaitForSingleObject(m_SendThread, INFINITE);
	//WaitForSingleObject(m_RecvThread, INFINITE);

	closesocket(m_ServerSock);

	WSACleanup();
}

Network::~Network()
{

}