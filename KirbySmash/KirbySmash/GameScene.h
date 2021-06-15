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
#define ROWDIS 45 //블럭간 거리
#define COLDIS 45 //블럭간 거리
#define SKILLDIS 123 //스킬블럭간 거리
#define KIRBYSIZE 100

std::random_device rd;
std::mt19937 engine(rd());

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";
LPCTSTR lpszWindowName = "Window Program";

RECT WindowRect; //window크기

COLORREF kirbyRGB = RGB(100, 100, 100);

static int Goalscore, Turn;
static bool IsSel, LBSel; //선택여부 확인, 좌클릭누르기
static int selx, sely; //선택한 블럭 인덱스
static int tmpx, tmpy, cnt; //블럭자리 바꿀때 
static int dir;
static int score, destorynum;
static int skill, returnscore;
static int selectedSkill;
static int bombnum, switchnum, turnnum, returnnum;
static HWND play, chexit, retry; //차일드 윈도우
static bool pause, end; //일시정지
enum {
	Left = 0, Right, Up, Down = 3
};
enum {
	Pink = 0, Blue, Green, Gray, Purple, Orange = 5
};

int skillAni[4] = { 6, 11, 5, 6};

struct Object{
	POINT pos;
	int color; //블럭색 -> 0:핑크, 1:블루, 2:그린, 3:회색, 4:보라, 5:오렌지
	int ani; //애니메이션 순서 인덱스
	bool destroy;
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