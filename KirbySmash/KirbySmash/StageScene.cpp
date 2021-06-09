#include "StageScene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);								//구조체 크기
	WndClass.style = CS_HREDRAW | CS_VREDRAW;						//윈도우 출력 스타일
	WndClass.lpfnWndProc = (WNDPROC)WndProc;						//프로시저 함수
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;									//인스턴스 핸들
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//실행 파일 아이콘
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//커서
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//윈도우 배경색
	WndClass.lpszMenuName = NULL;									//메뉴 이름
	WndClass.lpszClassName = lpszClass;								//윈도우 클래스 이름
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);				//윈도우 캡션 아이콘
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU, 0, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
	//CreateWindow(윈도우 클래스 이름, 윈도우 타이틀 이름, 윈도우 스타일, x, y, 가로, 세로, 부모 윈도우 핸들, 메뉴 핸들, 응용 프로그램 인스턴스, 생성 윈도우 정보);
	ShowWindow(hWnd, nCmdShow);	//윈도우 화면 출력
	UpdateWindow(hWnd);			//O/S에 WM_PAINT 메시지 전송

	// 윈도우 OS가 프로그램에 전달한 메시지를 받아들여 분석하는 무한 루프
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
		//비트맵 로드
		//스테이지 배경
		stageBackground.hBitmap = (HBITMAP)LoadImage(g_hInst, "stageBackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(stageBackground.hBitmap, sizeof(BITMAP), &bmp);
		stageBackground.bitmapSize = { bmp.bmWidth, bmp.bmHeight };
		//스테이지1 버튼
		stageButton[0].hBitmap = (HBITMAP)LoadImage(g_hInst, "stage1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(stageButton[0].hBitmap, sizeof(BITMAP), &bmp);
		stageButton[0].bitmapSize = { bmp.bmWidth, bmp.bmHeight };
		stageButton[0].drawPos = { 100, 450 };
		stageButton[0].centerPos = { stageButton[0].drawPos.x + bmp.bmWidth / 2, stageButton[0].drawPos.y + bmp.bmHeight / 2 };
		//스테이지2 버튼
		stageButton[1].hBitmap = (HBITMAP)LoadImage(g_hInst, "stage2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(stageButton[1].hBitmap, sizeof(BITMAP), &bmp);
		stageButton[1].bitmapSize = { bmp.bmWidth, bmp.bmHeight };
		stageButton[1].drawPos = { 300, 400 };
		stageButton[1].centerPos = { stageButton[1].drawPos.x + bmp.bmWidth / 2, stageButton[1].drawPos.y + bmp.bmHeight / 2 };
		//스테이지3 버튼
		stageButton[2].hBitmap = (HBITMAP)LoadImage(g_hInst, "stage3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(stageButton[2].hBitmap, sizeof(BITMAP), &bmp);
		stageButton[2].bitmapSize = { bmp.bmWidth, bmp.bmHeight };
		stageButton[2].drawPos = { 520, 395 };
		stageButton[2].centerPos = { stageButton[2].drawPos.x + bmp.bmWidth / 2, stageButton[2].drawPos.y + bmp.bmHeight / 2 };
		//커비
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

		//StretchBlt(출력 DC, 그릴 위치.x, 그릴 위치.y, 그릴 크기.x, 그릴 크기.y, 소스 DC, 메모리DC 위치.x, 메모리DC 위치.y, 가져올 크기.x, 가져올 크기.y, 래스터 연산 방법)
		//TransparentBlt(출력 DC, 그릴 위치.x, 그릴 위치.y, 그릴 크기.x, 그릴 크기.y, 소스 DC, 메모리DC 위치.x, 메모리DC 위치.y, 가져올 크기.x, 가져올 크기.y, 투명하게 할 색상)

		//스테이지 배경 그리기
		oldBitmap2 = (HBITMAP)SelectObject(memdc2, stageBackground.hBitmap);
		StretchBlt(memdc1, 0, 0, SCREENWIDTH, SCREENHEIGHT, memdc2, cx, cy, SCREENWIDTH, SCREENHEIGHT, SRCCOPY);

		//스테이지 버튼 그리기
		for (i = 0; i < STAGECNT; ++i) {
			oldBitmap2 = (HBITMAP)SelectObject(memdc2, stageButton[i].hBitmap);
			TransparentBlt(memdc1, stageButton[i].drawPos.x - cx, stageButton[i].drawPos.y - cy, stageButton[i].bitmapSize.x, stageButton[i].bitmapSize.y, memdc2, 0, 0, stageButton[i].bitmapSize.x, stageButton[i].bitmapSize.y, buttonRGB);
		}

		//커비 그리기
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
				//카메라 좌우 스크롤
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
		//스테이지 선택
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
	float dis;		//두 점 간의 길이
	dis = sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
	if (dis < BUTTONRADIUS) return TRUE;
	else return FALSE;
}