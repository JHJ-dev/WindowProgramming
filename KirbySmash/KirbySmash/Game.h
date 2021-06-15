#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tchar.h>
#include <random>
#include <math.h>
#include <fmod.h>
#include "resource.h"

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#pragma comment(lib, "msimg32.lib")

//화면
#define WIDTH 496
#define HEIGHT 839
#define SCREENWIDTH 480
#define SCREENHEIGHT 800

//게임씬
#define BLOCKROW 10
#define BLOCKCOL 12
#define BLOCKSIZE 36
#define BLOCKSPEED 9
#define ROWDIS 45 //블럭간 거리
#define COLDIS 45 //블럭간 거리
#define SKILLDIS 123 //스킬블럭간 거리
#define KIRBYSIZE 100
#define SOUND_EFFECT 2
#define EFFECT_COUNT 4
#define CHANNEL_COUNT 5

//스테이지씬
#define STAGECNT 3				//스테이지 개수
#define BUTTONRADIUS 35			//스테이지 버튼 반지름
#define POINTCNT 24

std::random_device rd;
std::mt19937 engine(rd());

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";
LPCTSTR lpszWindowName = "커비 스매시";

//------------------------------------구조체---------------------------------------------
//게임씬
struct Object {
	POINT pos;
	int color; //블럭색 -> 0:핑크, 1:블루, 2:그린, 3:회색, 4:보라, 5:오렌지
	int ani; //애니메이션 순서 인덱스
	bool destroy;
	int blocksize{ 0 };
};
Object block[BLOCKCOL][BLOCKROW]; //블럭
Object returnblock[BLOCKCOL][BLOCKROW];

struct Bit {
	POINT bitmapSize; //비트맵크기
	HBITMAP bit;
};
Bit Background;
Bit Block[7];
Bit Skill[4];
Bit Child[3];

struct Kirby {
	HBITMAP hBitmap;
	POINT bitmapSize;	//비트맵 크기
	POINT Pos;			//그릴 위치
	BOOL Move;			//캐릭터 이동
	int moveCount;		//캐릭터 이동 횟수
	int movedir;		//캐릭터 이동 방향
	int aniIndex;		//캐릭터 애니메이션 인덱스
};
Kirby kirby;

//스테이지씬
struct Image {
	HBITMAP hBitmap;
	POINT bitmapSize;	//비트맵 크기
	POINT drawPos;		//그릴 위치
	POINT centerPos;	//중앙 위치
};
Image stageBackground;
Image stageButton[3];
Image fade;
Image title;

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
Character stagekirby;

//------------------------------------변수---------------------------------------------
//게임씬
RECT WindowRect; //window크기
COLORREF kirbyRGB = RGB(100, 100, 100);

enum {
	Left = 0, Right, Up, Down = 3
};
enum {
	Pink = 0, Blue, Green, Gray, Purple, Orange = 5
};
enum {
	Small = 0, Big = 1
};
int skillAni[4] = { 7, 11, 5, 7 };	//공격, 구르기, 빨아들이기, 전화

static FMOD_SYSTEM* System;
static FMOD_SOUND* bgmSound[SOUND_EFFECT];
static FMOD_SOUND* effectSound[EFFECT_COUNT];
static FMOD_CHANNEL* Channel[CHANNEL_COUNT];

static int Goalscore, Turn;
static bool IsSel, LBSel; //선택여부 확인, 좌클릭누르기
static int selx, sely; //선택한 블럭 인덱스
static int tmpx, tmpy, cnt; //블럭자리 바꿀때 
static int dir;
static int score, destorynum;
static int skill, returnscore;
static int selectedSkill;
static int bombnum, switchnum, turnnum, returnnum;
static int selectdir{ Small };
static HWND play, chexit, retry; //차일드 윈도우
static bool pause, end, exittost; //일시정지
static int stage = 1;

//스테이지씬
COLORREF buttonRGB = RGB(201, 206, 181);

int i;	//for문 용
static int cx, cy;			//카메라 좌표
static int mx, my;			//마우스 좌표
static int nowStage;		//현재 스테이지
static int selectedStage;	//선택된 스테이지
static int alpha;			//페이드인아웃 알파값
static BLENDFUNCTION bf;
static BOOL Loading;
static int loadingCnt;
static int scene;			//현재 씬(0: 스테이지, 1: 게임)

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

//------------------------------------함수---------------------------------------------
//게임씬
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool CheckStart();
void PrintBackGround(HDC hdc); //배경출력
void PrintBoard(HDC hdc);
void PrintBlock(HDC hdc); //블럭출력
void PrintSkill(HDC hdc);
bool BlockSelect(int mx, int my); //마우스 입력과 블럭
void PrintRect(HDC hdc);
bool Check3(); //블럭 터지는거 확인
void BlockDown();
bool CheckDown();
void BlockChange(int i, int j); //블럭이 내려왔을때 배열조정
bool CheckChange(int i, int j);
void NewBlock();
void PrintScore(HDC hdc);
void Score();
void PrintTurn(HDC hdc);
int SkillSelect(int mx, int my);
void PrintSkillRect(HDC hdc);
void SkillBomb(int mx, int my);
void SkillSwitch(int mx, int my);
void PrintKirby(HDC hdc);
void SkillTurn();
void SkillReturn();
void Reset();
void PrintFade(HDC hdc);
void Sound_Setup();
int digit();

//스테이지씬
BOOL InCircle(POINT a, POINT b);		//마우스 좌표가 원 안에 있는지 체크