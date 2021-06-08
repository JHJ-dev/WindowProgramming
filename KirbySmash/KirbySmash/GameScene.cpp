#include "Gamescene.h"
#define stage 1

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
	HDC hdc, MemDC, tmpDC;
	PAINTSTRUCT ps;
	BITMAP bmp;
	HBITMAP BackBit, oldBackBit;
	
	switch (uMsg) {
	case WM_CREATE:
		cnt = 0;
		LBSel = true;
		GetClientRect(hwnd, &WindowRect);
		//비트맵 로드, 크기저장
		{
			Background.bit = (HBITMAP)LoadImage(g_hInst, "BackGround(bmp).bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(Background.bit, sizeof(BITMAP), &bmp);
			Background.bitmapSize.x = bmp.bmWidth;
			Background.bitmapSize.y = bmp.bmHeight;
			Block[0].bit = (HBITMAP)LoadImage(g_hInst, "BlockAnimation(Pink).bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(Block[0].bit, sizeof(BITMAP), &bmp);
			Block[0].bitmapSize.x = bmp.bmWidth;
			Block[0].bitmapSize.y = bmp.bmHeight;

			Block[1].bit = (HBITMAP)LoadImage(g_hInst, "BlockAnimation(Blue).bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(Block[1].bit, sizeof(BITMAP), &bmp);
			Block[1].bitmapSize.x = bmp.bmWidth;
			Block[1].bitmapSize.y = bmp.bmHeight;

			Block[2].bit = (HBITMAP)LoadImage(g_hInst, "BlockAnimation(Green).bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(Block[2].bit, sizeof(BITMAP), &bmp);
			Block[2].bitmapSize.x = bmp.bmWidth;
			Block[2].bitmapSize.y = bmp.bmHeight;

			Block[3].bit = (HBITMAP)LoadImage(g_hInst, "BlockAnimation(Gray).bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(Block[3].bit, sizeof(BITMAP), &bmp);
			Block[3].bitmapSize.x = bmp.bmWidth;
			Block[3].bitmapSize.y = bmp.bmHeight;

			Block[4].bit = (HBITMAP)LoadImage(g_hInst, "BlockAnimation(Purple).bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(Block[4].bit, sizeof(BITMAP), &bmp);
			Block[4].bitmapSize.x = bmp.bmWidth;
			Block[4].bitmapSize.y = bmp.bmHeight;

			Block[5].bit = (HBITMAP)LoadImage(g_hInst, "BlockAnimation(Orange).bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(Block[5].bit, sizeof(BITMAP), &bmp);
			Block[5].bitmapSize.x = bmp.bmWidth;
			Block[5].bitmapSize.y = bmp.bmHeight;

			Block[6].bit = (HBITMAP)LoadImage(g_hInst, "DestroyEffect.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(Block[6].bit, sizeof(BITMAP), &bmp);
			Block[6].bitmapSize.x = bmp.bmWidth;
			Block[6].bitmapSize.y = bmp.bmHeight;
		}
	
		//블럭정보
		switch (stage) {
		case 1:
		{
			std::uniform_int_distribution<> uid(0, 3);
			for (int i = 0; i < BLOCKCOL; ++i) {
				for (int j = 0; j < BLOCKROW; ++j) {
					block[i][j].color = uid(engine);
					block[i][j].pos = { 20 + j * ROWDIS, 163 + i * COLDIS };
				}
			}
		}
			break;
		case 2:
		{
			std::uniform_int_distribution<> uid(0, 4);
			for (int i = 0; i < BLOCKCOL; ++i) {
				for (int j = 0; j < BLOCKROW; ++j) {
					block[i][j].color = uid(engine);
					block[i][j].pos = { 20 + j * ROWDIS, 163 + i * COLDIS };
				}
			}
		}
			break;
		case 3:
		{
			std::uniform_int_distribution<> uid(0, 5);
			for (int i = 0; i < BLOCKCOL; ++i) {
				for (int j = 0; j < BLOCKROW; ++j) {
					block[i][j].color = uid(engine);
					block[i][j].pos = { 20 + j * ROWDIS, 163 + i * COLDIS };
				}
			}
		}
			break;
		}
		SetTimer(hwnd, 1, 500, NULL);
		break;
	case WM_LBUTTONDOWN:
	{
		int mx = LOWORD(lParam);
		int my = HIWORD(lParam);
		if (LBSel) {
			if (CheckBlock(mx, my)) {
				if (IsSel) {
					SetTimer(hwnd, 2, 1, NULL); //블럭 자리바꾸기 애니메이션
					IsSel = false;
					LBSel = false;
				}
				else {
					IsSel = true;
				}
			}
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	}
	case WM_TIMER:
		switch (wParam) {
		case 1: //가만히 있을 때 애니메이션
			for (int i = 0; i < BLOCKCOL; ++i) {
				for (int j = 0; j < BLOCKROW; ++j) {
					if (!block[i][j].destroy) {
						block[i][j].ani++;
						if (block[i][j].ani == 4) {
							block[i][j].ani = 0;
						}
					}
					else {
						block[i][j].ani++;
						if (block[i][j].ani == 8) {
							block[i][j].ani = 0;
							block[i][j].destroy = false;
							block[i][j].color = -1;
							block[i][j].color = -1;
							SetTimer(hwnd, 1, 500, NULL);
							SetTimer(hwnd, 3, 1, NULL);
						}
					}
				}
			}
			break;
		case 2: //블럭 자리바꾸기 애니메이션
			switch (dir) {
			case Left:
				block[tmpy][tmpx].pos.x -= 1;
				block[sely][selx].pos.x += 1;
				cnt++;
				if (cnt == ROWDIS) {
					POINT ptmp = block[tmpy][tmpx].pos;
					block[tmpy][tmpx].pos = block[sely][selx].pos;
					block[sely][selx].pos = ptmp;
					int ctmp = block[tmpy][tmpx].color;
					block[tmpy][tmpx].color = block[sely][selx].color;
					block[sely][selx].color = ctmp;
					cnt = 0;
					dir = 4;
					LBSel = true;
					KillTimer(hwnd, 2);
					if (Check3()) {
						SetTimer(hwnd, 1, 100, NULL);
					}
				}
				break;
			case Right:
				block[tmpy][tmpx].pos.x += 1;
				block[sely][selx].pos.x -= 1;
				cnt++;
				if (cnt == ROWDIS) {
					POINT ptmp = block[tmpy][tmpx].pos;
					block[tmpy][tmpx].pos = block[sely][selx].pos;
					block[sely][selx].pos = ptmp;
					int ctmp = block[tmpy][tmpx].color;
					block[tmpy][tmpx].color = block[sely][selx].color;
					block[sely][selx].color = ctmp;
					cnt = 0;
					dir = 4;
					LBSel = true;
					KillTimer(hwnd, 2);
					if (Check3()) {
						SetTimer(hwnd, 1, 100, NULL);
					}
				}
				break;
			case Up:
				block[tmpy][tmpx].pos.y -= 1;
				block[sely][selx].pos.y += 1;
				cnt++;
				if (cnt == COLDIS) {
					POINT ptmp = block[tmpy][tmpx].pos;
					block[tmpy][tmpx].pos = block[sely][selx].pos;
					block[sely][selx].pos = ptmp;
					int ctmp = block[tmpy][tmpx].color;
					block[tmpy][tmpx].color = block[sely][selx].color;
					block[sely][selx].color = ctmp;
					cnt = 0;
					dir = 4;
					LBSel = true;
					KillTimer(hwnd, 2);
					if (Check3()) {
						SetTimer(hwnd, 1, 100, NULL);
					}
				}
				break;
			case Down:
				block[tmpy][tmpx].pos.y += 1;
				block[sely][selx].pos.y -= 1;
				cnt++;
				if (cnt == COLDIS) {
					POINT ptmp = block[tmpy][tmpx].pos;
					block[tmpy][tmpx].pos = block[sely][selx].pos;
					block[sely][selx].pos = ptmp;
					int ctmp = block[tmpy][tmpx].color;
					block[tmpy][tmpx].color = block[sely][selx].color;
					block[sely][selx].color = ctmp;
					cnt = 0;
					dir = 4;
					LBSel = true;
					KillTimer(hwnd, 2);
					if (Check3()) {
						SetTimer(hwnd, 1, 100, NULL);
					}
				}
				break;
			case 4:
				LBSel = true;
				KillTimer(hwnd, 2);
				break;
			}
			break;
		case 3: //블럭 내려오는 애니메이션
			BlockDown();
			cnt++;
			if (cnt == ROWDIS) {
				for (int i = 0; i < BLOCKROW; ++i) {
					for (int j = 0; j < BLOCKCOL - 1; ++j) {
						if (block[j + 1][i].color == -1) {
							Object tmp = block[j + 1][i];
							block[j + 1][i] = block[j][i];
							block[j][i] = tmp;
							cnt = 0;
						}
					}
				}
				if (!CheckDown()) {
					cnt = 0;
					SetTimer(hwnd, 4, 1, NULL);
					KillTimer(hwnd, 3);
				}
			}
			break;
		case 4: //새로운 블럭만들기
			NewBlock();
			KillTimer(hwnd, 4);
			break;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//더블버퍼링 
		{
			MemDC = CreateCompatibleDC(hdc);
			BackBit = CreateCompatibleBitmap(hdc, WindowRect.right, WindowRect.bottom);
			oldBackBit = (HBITMAP)SelectObject(MemDC, BackBit);
			PatBlt(MemDC, 0, 0, WindowRect.right, WindowRect.bottom, WHITENESS);
			tmpDC = hdc;
			hdc = MemDC;
			MemDC = tmpDC;
		}

		PrintBackGround(hdc);
		PrintBlock(hdc);
		if (IsSel) {
			PrintRect(hdc);
		}
		//더블버퍼링
		{
			tmpDC = hdc;
			hdc = MemDC;
			MemDC = tmpDC;
			BitBlt(hdc, 0, 0, WindowRect.right, WindowRect.bottom, MemDC, 0, 0, SRCCOPY);
			SelectObject(MemDC, oldBackBit);
			DeleteObject(BackBit);
			DeleteDC(MemDC);
		}
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void PrintBackGround(HDC hdc) {
	HDC memdc;
	HBITMAP Oldbit;
	memdc = CreateCompatibleDC(hdc);
	Oldbit = (HBITMAP)SelectObject(memdc, Background.bit);
	StretchBlt(hdc, 0, 0, WindowRect.right, WindowRect.bottom, memdc, 0, 0, Background.bitmapSize.x, Background.bitmapSize.y, SRCCOPY);
	SelectObject(memdc, Oldbit);
	DeleteObject(memdc);
	DeleteObject(Oldbit);
}

void PrintBlock(HDC hdc) {
	HDC memdc;
	HBITMAP Oldbit;

	for (int i = 0; i < BLOCKCOL; ++i) {
		for (int j = 0; j < BLOCKROW; ++j) {
			switch (block[i][j].color) {
			case 0: //Pink
				memdc = CreateCompatibleDC(hdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[0].bit);
				TransparentBlt(hdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE, BLOCKSIZE, memdc, (Block[0].bitmapSize.x / 4 * block[i][j].ani), 0, Block[0].bitmapSize.x / 4, Block[0].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case 1: //Blue
				memdc = CreateCompatibleDC(hdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[1].bit);
				TransparentBlt(hdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE, BLOCKSIZE, memdc, (Block[1].bitmapSize.x / 4 * block[i][j].ani), 0, Block[1].bitmapSize.x / 4, Block[1].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case 2: //Green
				memdc = CreateCompatibleDC(hdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[2].bit);
				TransparentBlt(hdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE, BLOCKSIZE, memdc, (Block[2].bitmapSize.x / 4 * block[i][j].ani), 0, Block[2].bitmapSize.x / 4, Block[2].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case 3: //Gray
				memdc = CreateCompatibleDC(hdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[3].bit);
				TransparentBlt(hdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE, BLOCKSIZE, memdc, (Block[3].bitmapSize.x / 4 * block[i][j].ani), 0, Block[3].bitmapSize.x / 4, Block[3].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case 4: //Purple
				memdc = CreateCompatibleDC(hdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[4].bit);
				TransparentBlt(hdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE, BLOCKSIZE, memdc, (Block[4].bitmapSize.x / 4 * block[i][j].ani), 0, Block[4].bitmapSize.x / 4, Block[4].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case 5: //Orange
				memdc = CreateCompatibleDC(hdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[5].bit);
				TransparentBlt(hdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE, BLOCKSIZE, memdc, (Block[5].bitmapSize.x / 4 * block[i][j].ani), 0, Block[5].bitmapSize.x / 4, Block[5].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case 6: //Destroy
				memdc = CreateCompatibleDC(hdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[6].bit);
				TransparentBlt(hdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE, BLOCKSIZE, memdc, (Block[6].bitmapSize.x / 8 * block[i][j].ani), 0, Block[6].bitmapSize.x / 8, Block[6].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			default:
				break;
			}
		}
	}
	
}

bool CheckBlock(int mx, int my) {
	for (int i = 0; i < BLOCKCOL; ++i) {
		for (int j = 0; j < BLOCKROW; ++j) {
			if (block[i][j].pos.x <= mx && block[i][j].pos.x + BLOCKSIZE >= mx && block[i][j].pos.y <= my && block[i][j].pos.y + BLOCKSIZE >= my) {
				if (IsSel) {
					tmpx = selx;
					tmpy = sely;
					selx = j;
					sely = i;
					if (block[tmpy][tmpx].pos.x + ROWDIS == block[sely][selx].pos.x && block[tmpy][tmpx].pos.y == block[sely][selx].pos.y && block[tmpy][tmpx].color != block[sely][selx].color) {
						dir = Right;
					}
					else if (block[tmpy][tmpx].pos.x - ROWDIS == block[sely][selx].pos.x && block[tmpy][tmpx].pos.y == block[sely][selx].pos.y && block[tmpy][tmpx].color != block[sely][selx].color) {
						dir = Left;
					}
					else if (block[tmpy][tmpx].pos.y + COLDIS == block[sely][selx].pos.y && block[tmpy][tmpx].pos.x == block[sely][selx].pos.x && block[tmpy][tmpx].color != block[sely][selx].color) {
						dir = Down;
					}
					else if (block[tmpy][tmpx].pos.y - COLDIS == block[sely][selx].pos.y && block[tmpy][tmpx].pos.x == block[sely][selx].pos.x && block[tmpy][tmpx].color != block[sely][selx].color) {
						dir = Up;
					}
					else {
						dir = 4;
					}
				}
				else {
					sely = i;
					selx = j;                             
				}
				return true;
			}
		}
	}
	return false;
}
void PrintRect(HDC hdc) {
	RECT rect = {block[sely][selx].pos.x, block[sely][selx].pos.y, block[sely][selx].pos.x + BLOCKSIZE, block[sely][selx].pos.y + BLOCKSIZE};
	HBRUSH hbrush, oldbrush;
	hbrush = CreateSolidBrush(RGB(255, 0, 0));
	oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
	FrameRect(hdc, &rect, hbrush);
	SelectObject(hdc, oldbrush);
	DeleteObject(hbrush);
}

bool Check3() {
	//가로확인
	for (int i = 0; i < BLOCKCOL; ++i) {
		for (int j = 0; j < BLOCKROW - 2; ++j) {
			if (block[i][j].color == block[i][j + 1].color && block[i][j].color == block[i][j + 2].color && block[i][j].color != -1) {
				while (block[i][j].color == block[i][j + 1].color) {
					block[i][j].destroy = true;
					block[i][j].color = 6;
					block[i][j].ani = 0;
					if (j == BLOCKROW - 1) {
						block[i][j + 1].destroy = true;
						block[i][j + 1].color = 6;
						block[i][j + 1].ani = 0;
						break;
					}
					j++;
				}
				block[i][j].destroy = true;
				block[i][j].color = 6;
				block[i][j].ani = 0;
				return true;
			}
		}
	}

	//세로확인
	for (int i = 0; i < BLOCKROW; ++i) {
		for (int j = 0; j < BLOCKCOL - 2; ++j) {
			if (block[j][i].color == block[j + 1][i].color && block[j][i].color == block[j + 2][i].color && block[i][j].color != -1) {
				while (block[i][j].color == block[i][j + 1].color) {
					block[j][i].destroy = true;
					block[j][i].color = 6;
					block[j][i].ani= 0;
					if (j == BLOCKROW - 1) {
						block[j + 1][i].destroy = true;
						block[j + 1][i].color = 6;
						block[j + 1][i].ani = 0;
						break;
					}
					j++;
				}
				block[j][i].destroy = true;
				block[j][i].color = 6;
				block[j][i].ani = 0;
				return true;
			}
		}
	}
	return false;
}
void BlockDown() {
	for (int i = 0; i < BLOCKROW; ++i) {
		for (int j = 0; j < BLOCKCOL - 1; ++j) {
			if (block[j + 1][i].color == -1) {
				block[j][i].pos.y += 1;
			}
		}
	}
}
bool CheckDown() {
	for (int i = 0; i < BLOCKROW; ++i) {
		for (int j = 0; j < BLOCKCOL - 1; ++j) {
			if (block[j + 1][i].color == -1) {
				return true;
			}
		}
	}
	return false;
}
void NewBlock() {
	for (int i = 0; i < BLOCKROW; ++i) {
		if (block[0][i].color == -1) {
			switch (stage) {
			case 1:
			{
				std::uniform_int_distribution<> uid(0, 3);
				block[0][i].color = uid(engine);
				block[0][i].pos = { 20 + i * ROWDIS, 163 };
			}
			break;
			case 2:
			{
				std::uniform_int_distribution<> uid(0, 4);
				block[0][i].color = uid(engine);
				block[0][i].pos = { 20 + i * ROWDIS, 163 };
			}
			break;
			case 3:
			{
				std::uniform_int_distribution<> uid(0, 5);
				block[0][i].color = uid(engine);
				block[0][i].pos = { 20 + i * ROWDIS, 163 };
			}
			break;
			}
		}
	}
}