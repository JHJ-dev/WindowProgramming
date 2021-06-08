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
#define ROWDIS 45 //������ �Ÿ�
#define COLDIS 43 //������ �Ÿ�
std::random_device rd;
std::mt19937 engine(rd());

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";
LPCTSTR lpszWindowName = "Window Program";

RECT WindowRect; //windowũ��

bool IsSel, LBSel; //���ÿ��� Ȯ��, ��Ŭ��������
int selx, sely; //������ ���� �ε���
POINT temp;
int tmpx, tmpy, cnt;
int dir;

enum {
	Left = 0, Right, Up, Down = 3
};

struct Object{
	POINT pos;
	int color; //������ -> 0:��ũ, 1:����, 2:�׸�, 3:ȸ��, 4:����, 5:������
	int ani; //�ִϸ��̼� ���� �ε���
	bool destroy;
};
Object block[BLOCKCOL][BLOCKROW]; //����

struct Bit {
	POINT bitmapSize; //��Ʈ��ũ��
	HBITMAP bit;
};
Bit Background;
Bit Block[7];

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void PrintBackGround(HDC hdc); //������
void PrintBlock(HDC hdc); //�������
bool CheckBlock(int mx, int my); //���콺 �Է°� ����
void PrintRect(HDC hdc);
bool Check3();
void BlockDown();
bool CheckDown();