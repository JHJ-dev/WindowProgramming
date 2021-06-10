#include <windows.h>
#include <tchar.h>
#include <random>
#include "resource.h"


#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#pragma comment(lib, "msimg32.lib")


#define WIDTH 496
#define HEIGHT 839

#define BLOCKROW 10
#define BLOCKCOL 12
#define BLOCKSIZE 36
#define BLOCKSPEED 5
#define ROWDIS 45 //블럭간 거리
#define COLDIS 45 //블럭간 거리

std::random_device rd;
std::mt19937 engine(rd());

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";
LPCTSTR lpszWindowName = "Window Program";

RECT WindowRect; //window크기

static int Goalscore, Turn;
static bool IsSel, LBSel; //선택여부 확인, 좌클릭누르기
static int selx, sely; //선택한 블럭 인덱스
static int tmpx, tmpy, cnt; //블럭자리 바꿀때 
static int dir;
static int score, destorynum;

enum {
	Left = 0, Right, Up, Down = 3
};
enum {
	Pink = 0, Blue, Green, Gray, Purple, Orange = 5
};

struct Object{
	POINT pos;
	int color; //블럭색 -> 0:핑크, 1:블루, 2:그린, 3:회색, 4:보라, 5:오렌지
	int ani; //애니메이션 순서 인덱스
	bool destroy;
};
Object block[BLOCKCOL][BLOCKROW]; //블럭

struct Bit {
	POINT bitmapSize; //비트맵크기
	HBITMAP bit;
};
Bit Background;
Bit Block[7];

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
bool CheckStart();
void PrintBackGround(HDC hdc); //배경출력
void PrintBoard(HDC hdc); 
void PrintBlock(HDC hdc); //블럭출력
bool BlockSelect(int mx, int my); //마우스 입력과 블럭
void PrintRect(HDC hdc);
bool Check3();
void BlockDown();
bool CheckDown();
void NewBlock();
void PrintScore(HDC hdc);
void Score();