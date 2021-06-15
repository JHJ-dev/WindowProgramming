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

//ȭ��
#define WIDTH 496
#define HEIGHT 839
#define SCREENWIDTH 480
#define SCREENHEIGHT 800

//���Ӿ�
#define BLOCKROW 10
#define BLOCKCOL 12
#define BLOCKSIZE 36
#define BLOCKSPEED 9
#define ROWDIS 45 //���� �Ÿ�
#define COLDIS 45 //���� �Ÿ�
#define SKILLDIS 123 //��ų���� �Ÿ�
#define KIRBYSIZE 100
#define SOUND_EFFECT 2
#define EFFECT_COUNT 4
#define CHANNEL_COUNT 5

//����������
#define STAGECNT 3				//�������� ����
#define BUTTONRADIUS 35			//�������� ��ư ������
#define POINTCNT 24

std::random_device rd;
std::mt19937 engine(rd());

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";
LPCTSTR lpszWindowName = "Ŀ�� ���Ž�";

//------------------------------------����ü---------------------------------------------
//���Ӿ�
struct Object {
	POINT pos;
	int color; //���� -> 0:��ũ, 1:���, 2:�׸�, 3:ȸ��, 4:����, 5:������
	int ani; //�ִϸ��̼� ���� �ε���
	bool destroy;
	int blocksize{ 0 };
};
Object block[BLOCKCOL][BLOCKROW]; //��
Object returnblock[BLOCKCOL][BLOCKROW];

struct Bit {
	POINT bitmapSize; //��Ʈ��ũ��
	HBITMAP bit;
};
Bit Background;
Bit Block[7];
Bit Skill[4];
Bit Child[3];

struct Kirby {
	HBITMAP hBitmap;
	POINT bitmapSize;	//��Ʈ�� ũ��
	POINT Pos;			//�׸� ��ġ
	BOOL Move;			//ĳ���� �̵�
	int moveCount;		//ĳ���� �̵� Ƚ��
	int movedir;		//ĳ���� �̵� ����
	int aniIndex;		//ĳ���� �ִϸ��̼� �ε���
};
Kirby kirby;

//����������
struct Image {
	HBITMAP hBitmap;
	POINT bitmapSize;	//��Ʈ�� ũ��
	POINT drawPos;		//�׸� ��ġ
	POINT centerPos;	//�߾� ��ġ
};
Image stageBackground;
Image stageButton[3];
Image fade;
Image title;

struct Character {
	HBITMAP hBitmap;
	POINT bitmapSize;	//��Ʈ�� ũ��
	POINT drawPos;		//�׸� ��ġ
	POINT centerPos;	//�߾� ��ġ
	BOOL Move;			//ĳ���� �̵�
	int Movedir = 1;		//ĳ���� �̵� ����
	int moveIndex;		//�̵� ��ǥ �ε���
	int aniIndex;		//ĳ���� �ִϸ��̼� �ε���
};
Character stagekirby;

//------------------------------------����---------------------------------------------
//���Ӿ�
RECT WindowRect; //windowũ��
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
int skillAni[4] = { 7, 11, 5, 7 };	//����, ������, ���Ƶ��̱�, ��ȭ

static FMOD_SYSTEM* System;
static FMOD_SOUND* bgmSound[SOUND_EFFECT];
static FMOD_SOUND* effectSound[EFFECT_COUNT];
static FMOD_CHANNEL* Channel[CHANNEL_COUNT];

static int Goalscore, Turn;
static bool IsSel, LBSel; //���ÿ��� Ȯ��, ��Ŭ��������
static int selx, sely; //������ �� �ε���
static int tmpx, tmpy, cnt; //���ڸ� �ٲܶ� 
static int dir;
static int score, destorynum;
static int skill, returnscore;
static int selectedSkill;
static int bombnum, switchnum, turnnum, returnnum;
static int selectdir{ Small };
static HWND play, chexit, retry; //���ϵ� ������
static bool pause, end, exittost; //�Ͻ�����
static int stage = 1;

//����������
COLORREF buttonRGB = RGB(201, 206, 181);

int i;	//for�� ��
static int cx, cy;			//ī�޶� ��ǥ
static int mx, my;			//���콺 ��ǥ
static int nowStage;		//���� ��������
static int selectedStage;	//���õ� ��������
static int alpha;			//���̵��ξƿ� ���İ�
static BLENDFUNCTION bf;
static BOOL Loading;
static int loadingCnt;
static int scene;			//���� ��(0: ��������, 1: ����)

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

//------------------------------------�Լ�---------------------------------------------
//���Ӿ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool CheckStart();
void PrintBackGround(HDC hdc); //������
void PrintBoard(HDC hdc);
void PrintBlock(HDC hdc); //�����
void PrintSkill(HDC hdc);
bool BlockSelect(int mx, int my); //���콺 �Է°� ��
void PrintRect(HDC hdc);
bool Check3(); //�� �����°� Ȯ��
void BlockDown();
bool CheckDown();
void BlockChange(int i, int j); //���� ���������� �迭����
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

//����������
BOOL InCircle(POINT a, POINT b);		//���콺 ��ǥ�� �� �ȿ� �ִ��� üũ