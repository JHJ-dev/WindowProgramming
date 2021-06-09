#include "StageScene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);								//����ü ũ��
	WndClass.style = CS_HREDRAW | CS_VREDRAW;						//������ ��� ��Ÿ��
	WndClass.lpfnWndProc = (WNDPROC)WndProc;						//���ν��� �Լ�
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;									//�ν��Ͻ� �ڵ�
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//���� ���� ������
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//Ŀ��
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//������ ����
	WndClass.lpszMenuName = NULL;									//�޴� �̸�
	WndClass.lpszClassName = lpszClass;								//������ Ŭ���� �̸�
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);				//������ ĸ�� ������
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU, 0, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
	//CreateWindow(������ Ŭ���� �̸�, ������ Ÿ��Ʋ �̸�, ������ ��Ÿ��, x, y, ����, ����, �θ� ������ �ڵ�, �޴� �ڵ�, ���� ���α׷� �ν��Ͻ�, ���� ������ ����);
	ShowWindow(hWnd, nCmdShow);	//������ ȭ�� ���
	UpdateWindow(hWnd);			//O/S�� WM_PAINT �޽��� ����

	// ������ OS�� ���α׷��� ������ �޽����� �޾Ƶ鿩 �м��ϴ� ���� ����
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc1, memdc2;
	static HBITMAP hBitmap;
	static HBITMAP oldBitmap1, oldBitmap2;
	static RECT crt;

	PAINTSTRUCT ps;
	BITMAP bmp;

	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hwnd, &crt);
		//��Ʈ�� �ε�
		//�������� ���
		stageBackground.hBitmap = (HBITMAP)LoadImage(g_hInst, "stageBackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(stageBackground.hBitmap, sizeof(BITMAP), &bmp);
		stageBackground.bitmapSize = { bmp.bmWidth, bmp.bmHeight };
		//��������1 ��ư
		stageButton[0].hBitmap = (HBITMAP)LoadImage(g_hInst, "stage1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(stageButton[0].hBitmap, sizeof(BITMAP), &bmp);
		stageButton[0].bitmapSize = { bmp.bmWidth, bmp.bmHeight };
		stageButton[0].drawPos = { 100, 450 };
		stageButton[0].centerPos = { stageButton[0].drawPos.x + bmp.bmWidth / 2, stageButton[0].drawPos.y + bmp.bmHeight / 2 };
		//��������2 ��ư
		stageButton[1].hBitmap = (HBITMAP)LoadImage(g_hInst, "stage2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(stageButton[1].hBitmap, sizeof(BITMAP), &bmp);
		stageButton[1].bitmapSize = { bmp.bmWidth, bmp.bmHeight };
		stageButton[1].drawPos = { 300, 400 };
		stageButton[1].centerPos = { stageButton[1].drawPos.x + bmp.bmWidth / 2, stageButton[1].drawPos.y + bmp.bmHeight / 2 };
		//��������3 ��ư
		stageButton[2].hBitmap = (HBITMAP)LoadImage(g_hInst, "stage3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(stageButton[2].hBitmap, sizeof(BITMAP), &bmp);
		stageButton[2].bitmapSize = { bmp.bmWidth, bmp.bmHeight };
		stageButton[2].drawPos = { 520, 395 };
		stageButton[2].centerPos = { stageButton[2].drawPos.x + bmp.bmWidth / 2, stageButton[2].drawPos.y + bmp.bmHeight / 2 };
		//Ŀ��
		kirby.hBitmap = (HBITMAP)LoadImage(g_hInst, "kirby.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(kirby.hBitmap, sizeof(BITMAP), &bmp);
		kirby.bitmapSize = { bmp.bmWidth, bmp.bmHeight };
		kirby.centerPos = { stage12[0].x, stage12[0].y - bmp.bmWidth / 2 };
		kirby.drawPos = { kirby.centerPos.x - bmp.bmWidth / 2, kirby.centerPos.y - bmp.bmHeight / 2 };

		nowStage = 0;
		SetTimer(hwnd, 1, 100, NULL);
		break;

	case WM_TIMER:
		hdc = GetDC(hwnd);
		if (hBitmap == NULL) hBitmap = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
		memdc1 = CreateCompatibleDC(hdc);
		memdc2 = CreateCompatibleDC(memdc1);
		oldBitmap1 = (HBITMAP)SelectObject(memdc1, hBitmap);

		//StretchBlt(��� DC, �׸� ��ġ.x, �׸� ��ġ.y, �׸� ũ��.x, �׸� ũ��.y, �ҽ� DC, �޸�DC ��ġ.x, �޸�DC ��ġ.y, ������ ũ��.x, ������ ũ��.y, ������ ���� ���)
		//TransparentBlt(��� DC, �׸� ��ġ.x, �׸� ��ġ.y, �׸� ũ��.x, �׸� ũ��.y, �ҽ� DC, �޸�DC ��ġ.x, �޸�DC ��ġ.y, ������ ũ��.x, ������ ũ��.y, �����ϰ� �� ����)

		//�������� ��� �׸���
		oldBitmap2 = (HBITMAP)SelectObject(memdc2, stageBackground.hBitmap);
		StretchBlt(memdc1, 0, 0, SCREENWIDTH, SCREENHEIGHT, memdc2, cx, cy, SCREENWIDTH, SCREENHEIGHT, SRCCOPY);

		//�������� ��ư �׸���
		for (i = 0; i < STAGECNT; ++i) {
			oldBitmap2 = (HBITMAP)SelectObject(memdc2, stageButton[i].hBitmap);
			TransparentBlt(memdc1, stageButton[i].drawPos.x - cx, stageButton[i].drawPos.y - cy, stageButton[i].bitmapSize.x, stageButton[i].bitmapSize.y, memdc2, 0, 0, stageButton[i].bitmapSize.x, stageButton[i].bitmapSize.y, buttonRGB);
		}

		//Ŀ�� �׸���
		if (wParam == 2) {
			if (Movedir == 1) {
				if (moveIndex < POINTCNT - 1) {
					if (selectedStage == 1) kirby.centerPos = { stage12[moveIndex].x, stage12[moveIndex].y - kirby.bitmapSize.y / 2 };
					else kirby.centerPos = { stage23[moveIndex].x, stage23[moveIndex].y - kirby.bitmapSize.y / 2 };
					if (cx < stageBackground.bitmapSize.x - SCREENWIDTH) cx += Movedir * 10;
					moveIndex++;
					//printf("%d : %d, %d\n", moveIndex, stage23[moveIndex].x, stage23[moveIndex].y);
				}
				else {
					if (selectedStage == 1) nowStage = 1;
					else nowStage = 2;
					Move = FALSE;
				}
			}
			else if (Movedir == -1) {
				if (moveIndex > 0) {
					if (selectedStage == 0) kirby.centerPos = { stage12[moveIndex].x, stage12[moveIndex].y - kirby.bitmapSize.y / 2 };
					else kirby.centerPos = { stage23[moveIndex].x, stage23[moveIndex].y - kirby.bitmapSize.y / 2 };
					if (cx > 0) cx += Movedir * 10;
					moveIndex--;
				}
				else {
					if (selectedStage == 0) nowStage = 0;
					else nowStage = 1;
					Move = FALSE;
				}
			}
		}


		oldBitmap2 = (HBITMAP)SelectObject(memdc2, kirby.hBitmap);
		kirby.drawPos = { kirby.centerPos.x - kirby.bitmapSize.x / 2, kirby.centerPos.y - kirby.bitmapSize.y / 2 };
		TransparentBlt(memdc1, kirby.drawPos.x - cx, kirby.drawPos.y - cy, kirby.bitmapSize.x, kirby.bitmapSize.y, memdc2, 0, 0, kirby.bitmapSize.x, kirby.bitmapSize.y, kirbyRGB);

		SelectObject(memdc2, oldBitmap2);
		DeleteDC(memdc2);
		SelectObject(memdc1, oldBitmap1);
		DeleteDC(memdc1);
		ReleaseDC(hwnd, hdc);
		InvalidateRgn(hwnd, NULL, false);
		break;

	case WM_PAINT:
		GetClientRect(hwnd, &crt);
		hdc = BeginPaint(hwnd, &ps);
		memdc1 = CreateCompatibleDC(hdc);
		oldBitmap1 = (HBITMAP)SelectObject(memdc1, hBitmap);
		BitBlt(hdc, 0, 0, SCREENWIDTH, SCREENHEIGHT, memdc1, 0, 0, SRCCOPY);
		SelectObject(memdc1, oldBitmap1);
		DeleteDC(memdc1);
		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
		if(!Move){
			switch (wParam)
			{
				//ī�޶� �¿� ��ũ��
			case 'a':
				if (cx > 0) cx -= 5;
				break;

			case 'd':
				if (cx < stageBackground.bitmapSize.x - SCREENWIDTH) cx += 5;
				break;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		//�������� ����
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		for (i = 0; i < STAGECNT; ++i) {
			if (InCircle({ stageButton[i].centerPos.x - cx, stageButton[i].centerPos.y - cy }, { mx, my })) {
				selectedStage = i;
				if (selectedStage < nowStage) {
					moveIndex = POINTCNT - 1;
					Movedir = -1;
				}
				else if (selectedStage > nowStage) {
					moveIndex = 0;
					Movedir = 1;
				}
				else  Movedir = 0;
				Move = TRUE;
				SetTimer(hwnd, 2, 100, NULL);
			}
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL InCircle(POINT a, POINT b) {
	float dis;		//�� �� ���� ����
	dis = sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
	if (dis < BUTTONRADIUS) return TRUE;
	else return FALSE;
}