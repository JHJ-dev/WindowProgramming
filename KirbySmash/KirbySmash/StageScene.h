#include <windows.h>
#include <stdio.h>
#include <math.h>

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#pragma comment (lib, "msimg32.lib")

#define WIDTH 496
#define HEIGHT 839
#define SCREENWIDTH 480
#define SCREENHEIGHT 800
#define STAGECNT 3				//스테이지 개수
#define BUTTONRADIUS 35			//스테이지 버튼 반지름
#define POINTCNT 24

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";
LPCTSTR lpszWindowName = "Window Program";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

//함수
BOOL InCircle(POINT a, POINT b);		//마우스 좌표가 원 안에 있는지 체크

COLORREF buttonRGB = RGB(201, 206, 181);
COLORREF kirbyRGB = RGB(100, 100, 100);

int i;	//for문 용
static int cx, cy;			//카메라 좌표
static int mx, my;			//마우스 좌표
static int nowStage;		//현재 스테이지
static int selectedStage;	//선택된 스테이지
static int alpha;			//페이드인아웃 알파값
static BOOL Loading;
static int loadingCnt;

POINT stage12[POINTCNT] = {
	{140, 388}, {148, 393}, {158, 399}, {166, 404}, {174, 408}, {183, 413},
	{192, 418}, {200, 422}, {208, 426}, {218, 429}, {228, 432}, {239, 432},
	{250, 430}, {262, 426}, {271, 421}, {280, 416}, {297, 405}, {306, 400},
	{314, 394}, {323, 388}, {332, 381}, {342, 376}, {351, 371}, {361, 367} };

POINT stage23[POINTCNT] = {
	{372, 362}, {384, 366}, {396, 366}, {408, 365}, {419, 369}, {431, 375},
	{442, 379}, {452, 383}, {462, 385}, {473, 388}, {484, 389}, {494, 389},
	{504, 388}, {514, 388}, {524, 387}, {532, 386}, {542, 385}, {550, 384},
	{559, 380}, {567, 376}, {575, 371}, {583, 367}, {590, 363}, {596, 359} };

struct Image {
	HBITMAP hBitmap;
	POINT bitmapSize;	//비트맵 크기
	POINT drawPos;		//그릴 위치
	POINT centerPos;	//중앙 위치
};
Image stageBackground;
Image stageButton[3];
Image fade;

struct Character {
	HBITMAP hBitmap;
	POINT bitmapSize;	//비트맵 크기
	POINT drawPos;		//그릴 위치
	POINT centerPos;	//중앙 위치
	BOOL Move;			//캐릭터 이동
	int Movedir = 1;		//캐릭터 이동 방향
	int moveIndex;		//이동 좌표 인덱스
	int aniIndex;		//캐릭터 애니메이션 인덱스
};
Character kirby;