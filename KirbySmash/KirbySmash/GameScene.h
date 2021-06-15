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
#define BLOCKSPEED 9
#define ROWDIS 45 //���� �Ÿ�
#define COLDIS 45 //���� �Ÿ�
#define SKILLDIS 123 //��ų���� �Ÿ�
#define KIRBYSIZE 100

std::random_device rd;
std::mt19937 engine(rd());

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";
LPCTSTR lpszWindowName = "Window Program";

RECT WindowRect; //windowũ��

COLORREF kirbyRGB = RGB(100, 100, 100);

static int Goalscore, Turn;
static bool IsSel, LBSel; //���ÿ��� Ȯ��, ��Ŭ��������
static int selx, sely; //������ �� �ε���
static int tmpx, tmpy, cnt; //���ڸ� �ٲܶ� 
static int dir;
static int score, destorynum;
static int skill, returnscore;
static int selectedSkill;
static int bombnum, switchnum, turnnum, returnnum;
static HWND play, chexit, retry; //���ϵ� ������
static bool pause, end; //�Ͻ�����
enum {
	Left = 0, Right, Up, Down = 3
};
enum {
	Pink = 0, Blue, Green, Gray, Purple, Orange = 5
};

int skillAni[4] = { 6, 11, 5, 6};

struct Object{
	POINT pos;
	int color; //���� -> 0:��ũ, 1:���, 2:�׸�, 3:ȸ��, 4:����, 5:������
	int ani; //�ִϸ��̼� ���� �ε���
	bool destroy;
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