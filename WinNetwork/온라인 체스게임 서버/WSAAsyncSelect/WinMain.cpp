#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#define SERVERPORT 9001
#define BUFSIZE 512
#define WM_SOCKET (WM_USER + 1) //네트워크 이벤트를 전달할 사용자 정의 윈도우 메시지

// 소켓 정보 저장을 위한 구조체
// 널문자열를 추가하기 위해 BUFSIZE + 1이다
// 각각 받은 바이트 수와 보낸 바이트 수를 유지하기 위한 변수
// recvdelayed 변수는 FD_READ 메시지를 받았지만  대응 함수인 recv()함수를 호출하지 않은 경우에 TRUE로 설정하게 된다.
// next는 연결리스트(자기 참조 구조체)로 관리하기 위해 필요하다. 소켓을 생성하고 메모리에 저장하는 위치는 각각 다르기 때문에
// 등록, 삭제에 불편함이 없고 크기에 제약이 없어야한다
struct SOCKETINFO
{
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	BOOL recvdelayed;
	SOCKETINFO* next;
};

SOCKETINFO* SocketInfoList;

struct CHESS_PIECE
{
	int x;
	int y;
	int piece_name;
	int piece_num;
};

enum PLAYER_COLOR
{
	WHITE,
	BLACK
};

int Player_Max = 0;
SOCKET Black_Player = NULL;
SOCKET White_Player = NULL;
bool black_set_check = false;
bool white_set_check = false;
CHESS_PIECE *save_piece;
int log_count = 0;

//소켓 메시지 처리
void ProcessSocketMessage(HWND, UINT, WPARAM, LPARAM);
//WINDOW 메시지 처리
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//소켓 관리 함수
BOOL AddSocketInfo(SOCKET sock);
SOCKETINFO* GetSocketInfo(SOCKET sock);
void RemoveSocketInfo(SOCKET sock);

//오류 출력 함수
void err_quit(const char* msg);
void err_display(const char* msg);
void err_display(int errcode);

//로그창 출력을 위한 함수이다
void Log_Add(char *buf);
void RemoveSocketInfo_Max(SOCKET sock);

HINSTANCE g_hInst;//글로벌 인스턴스핸들값
LPCTSTR lpszClass = TEXT("체스서버"); //창이름
HWND g_log;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPervlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	//WndClass는 기본 윈도우환경을 만드는 구조체다. 맴버변수는 밑에와 같다.
	WndClass.cbClsExtra = 0; //예약영역
	WndClass.cbWndExtra = 0; //예약영역 (신경x)
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//배경색
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//커서
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//아이콘 모양
	WndClass.hInstance = hInstance;//(프로그램 핸들값(번호)등록)
	WndClass.lpfnWndProc = WndProc;	//프로세스 함수 호출
	WndClass.lpszClassName = lpszClass;	//클레스 이름
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;//윈도우의 수직과 수평이 변경 시 다시 그린다.
	RegisterClass(&WndClass);  //만들어진 WidClass를 등록

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 540, 560, NULL, (HMENU)NULL, hInstance, NULL);
	
	if (hWnd == NULL)
		return 1;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	int retval;

	//윈도우 소켓 초기화
	WSADATA wsa;
	
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	//socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR)
		err_quit("bind 에러입니다");

	//listen()
	retval = listen(listen_sock, SOMAXCONN);

	if (retval == SOCKET_ERROR)
		err_quit("listen 에러입니다");

	//WSAAsySelect()
	//FD_ACCEPT, FD_CLOSE 이벤트만 등록함
	retval = WSAAsyncSelect(listen_sock, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);

	if (retval == SOCKET_ERROR)
		err_quit("WSAAsyncSelect 에러입니다");

	while (GetMessage(&Message, NULL, 0, 0))//사용자에게 메시지를 받아오는 함수(WM_QUIT 메시지 받을 시 종료)
	{
		TranslateMessage(&Message); //  키보드 입력 메시지 처리함수
		DispatchMessage(&Message); //받은 메시지를 WndProc에 전달하는 함수
	}

	//윈도우 소켓 종료
	WSACleanup();
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE:
		g_log = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 500, 500, hWnd, (HMENU)100, g_hInst, NULL);
		break;
		//등록한 유저 메시지를 받게 되면 처리하는 부분
		//메시지 처리를 위한 공간이 길어질 경우를 대비해서 따로 메시지를 처리하는 함수를 만들어준다.
		//윈도우 메시지를 그대로 처리해야하기때문에 인자를 그대로 넘져주는 네트워크 메시지 처리 함수를 만들어 준다.
	case WM_SOCKET://소켓 관련 윈도우 메시지
		ProcessSocketMessage(hWnd, iMessage, wParam, lParam);
		break;
	case WM_DESTROY:// 윈도우가 파괴되었다는 메세지
		PostQuitMessage(0); //GetMessage함수에 WM_QUIT 메시지를 보낸다.
		return 0; //WndProc의 Switch는 break 대신 return 0; 를 쓴다.
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam)); // case에 있는 메시지를 제외한 나머지 메시지를 처리한다.
}

//소켓 관련 윈도우 메시지 처리
//여러 클라이언트가 붙을 수는 있다. Send한 소켓으로 그대로 Recv하고 있다 전체 클라이언트가 알아야할 데이터라면
//전체에게 보내줘야한다, 전체에 주는 코드도 추가해보기**
void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//데이터 통신에 사용할 변수
	SOCKETINFO *ptr_sock;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;

	int addrlen, retval;
	char buf[BUFSIZE];
	bool tmp = false;
	int set_color = 0;
	int log_out = true;

	//오류 발생 여부 확인
	if (WSAGETSELECTERROR(lParam))
	{
		err_display(WSAGETSELECTERROR(lParam));

		if (Black_Player == wParam)
		{
			send(White_Player, (char*)&log_out, sizeof(log_out), 0);
			Black_Player = NULL;
			White_Player = NULL;
			black_set_check = false;
			white_set_check = false;
		}
		else if (White_Player == wParam)
		{
			send(Black_Player, (char*)&log_out, sizeof(log_out), 0);
			Black_Player = NULL;
			White_Player = NULL;
			black_set_check = false;
			white_set_check = false;
		}

		RemoveSocketInfo(wParam);
		return;
	}

	//메시지 처리
	switch (WSAGETSELECTEVENT(lParam))
	{
		//accept() 함수를 호출하고 리턴 값을 확인하여 오류를 처리한다.
	case FD_ACCEPT:

		addrlen = sizeof(clientaddr);
		client_sock = accept(wParam, (SOCKADDR*)&clientaddr, &addrlen);

		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept() 에러입니다.");
			return;
		}

		sprintf(buf, "[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		Log_Add(buf); // 접속자의 로그 추가해서 출력을 해준다

		//접속한 클라이언트 소켓을 등록한다.
		if (Player_Max < 2)
		{
			Player_Max++;
			AddSocketInfo(client_sock);
		}
		else
		{
			RemoveSocketInfo_Max(client_sock); // 인원초과시
			break;
		}

		if (Black_Player == NULL) // 접속 순으로 블랙,화이트
			Black_Player = client_sock;
		else if (White_Player == NULL)
			White_Player = client_sock;

		//FD_READ, FD_WRITE, FD_CLOSE를 등록한다.
		//ACCPET한후에 다시 새로운 설정으로 등록하면 덮어씌워진다 (최근 등록한것을 따른다)
		retval = WSAAsyncSelect(client_sock, hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);

		if (retval == SOCKET_ERROR)
		{
			err_display("SWAAsyncSelect() 에러입니다.");
			RemoveSocketInfo(client_sock);
		}

		break;
	case FD_READ: // 클라에서 데이터를 보내면 반응한다
		//소켓 정보 구조체를 받는다.
		ptr_sock = GetSocketInfo(wParam);

		if (ptr_sock == NULL)
			return;

		//이번에 받았지만 아직 보내지 않은 데이터가 있다면 받았다는 사실만 기록하고 리턴한다.
		if(ptr_sock->recvbytes > 0)
		{
			ptr_sock->recvdelayed = TRUE;
			return;
		}

		//데이터받기
		retval = recv(ptr_sock->sock, ptr_sock->buf, BUFSIZE, 0);

		if (retval == SOCKET_ERROR)
		{
			err_display("recv() 에러입니다.");
			RemoveSocketInfo(wParam);
			return;
		}

		ptr_sock->recvbytes = retval; // 리턴받은값을 recvbytes에 저장해준다

		//받은 데이터 출력
		ptr_sock->buf[retval] = '\0';

		if (retval == 1)
			tmp = (bool)ptr_sock->buf;
		if (ptr_sock->recvbytes == sizeof(bool) && tmp == true) // 로그인시에는 bool값이 들어오기때문이다
		{
			if (Black_Player == ptr_sock->sock && black_set_check == false)
			{
				black_set_check = true;
				set_color = WHITE;
				send(ptr_sock->sock, (char*)&set_color, sizeof(set_color), NULL);
				sprintf_s(buf, "흰색 플레이어 준비완료");
				Log_Add(buf);
			}
			else if (White_Player == ptr_sock->sock && white_set_check == false)
			{
				white_set_check = true;
				set_color = BLACK;
				send(ptr_sock->sock, (char*)&set_color, sizeof(set_color), NULL);
				sprintf_s(buf, "검은색 플레이어 준비완료");
				Log_Add(buf);
			}

			ptr_sock->recvbytes = ptr_sock->sendbytes = 0; // 초기화해준다
		}
		else if (ptr_sock->recvbytes == sizeof(CHESS_PIECE))
		{
			save_piece = (CHESS_PIECE*)ptr_sock->buf;

			if (Black_Player == ptr_sock->sock)
			{
				sprintf(buf, "흰색 플레이어 이동[X: %d, Y: %d]", save_piece->x, save_piece->y);
				Log_Add(buf);
			}
			else if (White_Player == ptr_sock->sock)
			{
				sprintf(buf, "검은색 플레이어 이동[X: %d, Y: %d]", save_piece->x, save_piece->y);
				Log_Add(buf);
			}
		}

		// break;가 없는 이유는 write 까지 처리해야 하기 때문이다

	case FD_WRITE:
		ptr_sock = GetSocketInfo(wParam);

		if (ptr_sock == NULL)
			return;
		if (ptr_sock->recvbytes <= ptr_sock->sendbytes)
			return;

		//데이터 보내기
		if (Black_Player == ptr_sock->sock)
			retval = send(White_Player, ptr_sock->buf + ptr_sock->sendbytes, ptr_sock->recvbytes - ptr_sock->sendbytes, 0);
		else if (White_Player == ptr_sock->sock)
			retval = send(Black_Player, ptr_sock->buf + ptr_sock->sendbytes, ptr_sock->recvbytes - ptr_sock->sendbytes, 0);

		if (retval == SOCKET_ERROR)
		{
			err_display("send() 에러입니다.");
			RemoveSocketInfo(wParam);
			return;
		}

		ptr_sock->sendbytes += retval;

		// 받은 데이터를 모두 보냈는지 체크
		// 받았지만 보내지 않은 파일 즉 TRUE로만 만들어둔 데이터들을 보낸다
		if (ptr_sock->recvbytes == ptr_sock->sendbytes)
		{
			ptr_sock->recvbytes = ptr_sock->sendbytes = 0;

			if (ptr_sock->recvdelayed)
			{
				ptr_sock->recvdelayed = FALSE;
				PostMessage(hWnd, WM_SOCKET, wParam, FD_READ);
			}
		}

		break;
	case FD_CLOSE: // 접속이 끊어지면 반응한다
		RemoveSocketInfo(wParam);
		break;
	}
}

//소켓 정보 추가
BOOL AddSocketInfo(SOCKET sock)
{
	SOCKETINFO* ptr = new SOCKETINFO;

	if (ptr == NULL)
	{
		printf("[오류] 메모리가 부족합니다\n");
		return FALSE;
	}

	ptr->sock = sock;
	ptr->recvbytes = 0;
	ptr->sendbytes = 0;
	ptr->recvdelayed = FALSE;
	ptr->next = SocketInfoList;
	SocketInfoList = ptr;

	return TRUE;
}

//소켓 정보 얻기
SOCKETINFO* GetSocketInfo(SOCKET sock)
{
	SOCKETINFO* ptr = SocketInfoList;

	while (ptr)
	{
		if (ptr->sock == sock)
			return ptr;

		ptr = ptr->next;
	}
	
	return NULL;
}

//소켓 정보 제거
void RemoveSocketInfo(SOCKET sock)
{
	char buf[BUFSIZ];
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(sock, (SOCKADDR*)&clientaddr, &addrlen);

	sprintf_s(buf, "[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	Log_Add(buf);

	SOCKETINFO* curr = SocketInfoList;
	SOCKETINFO* prev = NULL;

	while (curr)
	{
		if (curr->sock == sock)
		{
			if (prev)
				prev->next = curr->next;
			else
				SocketInfoList = curr->next;

			closesocket(curr->sock);

			Player_Max--;
		}

		prev = curr;
		curr = curr->next;
	}
}

//소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

//소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	char buf[BUFSIZ];

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	sprintf_s(buf, "[%s] %s", msg, (const char*)lpMsgBuf);
	Log_Add(buf);
	LocalFree(lpMsgBuf);
}

//소켓 함수 오류 출력
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	char buf[BUFSIZ];

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		errcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	sprintf_s(buf, "[오류] %s", (const char*)lpMsgBuf);
	Log_Add(buf);
	LocalFree(lpMsgBuf);
}

void Log_Add(char *buf)
{
	SendMessage(g_log, LB_ADDSTRING, 0, (LPARAM)buf);
	log_count++;

	if (log_count >= 31)
	{
		SendMessage(g_log, LB_SETTOPINDEX, log_count - 1, 0);
	}
}

// 접속자가 가득차있을때 종료
void RemoveSocketInfo_Max(SOCKET sock)
{
	bool no_login = false;
	char buf[BUFSIZ];
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(sock, (SOCKADDR*)&clientaddr, &addrlen);

	send(sock, (char*)&no_login, sizeof(no_login), NULL);

	sprintf_s(buf, "인원초과로 강제종료 : IP 주소=%s, 포트 번호=%d", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	Log_Add(buf);

	closesocket(sock);
}