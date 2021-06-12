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
#define POINTCNT 21

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


POINT stage12[POINTCNT] = {
	{150, 400}, {160, 405}, {170, 410}, {180, 415}, {190, 420},
	{200, 424}, {210, 427}, {222, 431}, {232, 432}, {244, 432},
	{255, 430}, {266, 426}, {275, 420}, {284, 416}, {290, 410},
	{298, 406}, {305, 400}, {313, 394}, {320, 388}, {330, 382}, {338, 378} };

POINT stage23[POINTCNT] = {
	{350, 370}, {360, 360}, {362, 367}, {370, 367}, {376, 367},
	{385, 366}, {390, 366}, {406, 365}, {422, 372}, {435, 378},
	{440, 380}, {450, 383}, {466, 386}, {482, 387}, {498, 388},
	{512, 388}, {526, 386}, {540, 384}, {562, 380}, {576, 372}, {576, 372} };

struct Image {
	HBITMAP hBitmap;
	POINT bitmapSize;	//비트맵 크기
	POINT drawPos;		//그릴 위치
	POINT centerPos;	//중앙 위치
};
Image stageBackground;
Image stageButton[3];

struct Character {
	HBITMAP hBitmap;
	POINT bitmapSize;	//비트맵 크기
	POINT drawPos;		//그릴 위치
	POINT centerPos;	//중앙 위치
	BOOL Move;			//캐릭터 이동
	int Movedir;		//캐릭터 이동 방향
	int moveIndex;		//이동 좌표 인덱스
	int aniIndex;		//캐릭터 애니메이션 인덱스
};
Character kirby;