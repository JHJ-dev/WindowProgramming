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

	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	WndClass.lpszClassName = "ChildClass";
	WndClass.lpfnWndProc = ChildProc;
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
	static HWND child_hWnd;
	HDC hdc, MemDC, tmpDC;
	PAINTSTRUCT ps;
	BITMAP bmp;
	HBITMAP BackBit, oldBackBit, hbit;
	switch (uMsg) {
	case WM_CREATE:
		cnt = 0;
		skill = -1;
		LBSel = true; pause = false;
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

			Skill[0].bit = (HBITMAP)LoadImage(g_hInst, "bomb.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(Skill[0].bit, sizeof(BITMAP), &bmp);
			Skill[0].bitmapSize.x = bmp.bmWidth;
			Skill[0].bitmapSize.y = bmp.bmHeight;

			Skill[1].bit = (HBITMAP)LoadImage(g_hInst, "switch.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(Skill[0].bit, sizeof(BITMAP), &bmp);
			Skill[1].bitmapSize.x = bmp.bmWidth;
			Skill[1].bitmapSize.y = bmp.bmHeight;

			Skill[2].bit = (HBITMAP)LoadImage(g_hInst, "plus.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(Skill[0].bit, sizeof(BITMAP), &bmp);
			Skill[2].bitmapSize.x = bmp.bmWidth;
			Skill[2].bitmapSize.y = bmp.bmHeight;

			Skill[3].bit = (HBITMAP)LoadImage(g_hInst, "return.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(Skill[0].bit, sizeof(BITMAP), &bmp);
			Skill[3].bitmapSize.x = bmp.bmWidth;
			Skill[3].bitmapSize.y = bmp.bmHeight;

			kirby.hBitmap = (HBITMAP)LoadImage(g_hInst, "rightroll.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(kirby.hBitmap, sizeof(BITMAP), &bmp);
			kirby.bitmapSize = { bmp.bmWidth,  bmp.bmHeight };
			kirby.Pos = { 200, 60 };
			kirby.movedir = 1;
			selectedSkill = 2;
		}

		//스테이지정보
		switch (stage) {
		case 1:
		{
			std::uniform_int_distribution<> uid(Pink, Gray);
			for (int i = 0; i < BLOCKCOL; ++i) {
				for (int j = 0; j < BLOCKROW; ++j) {
					block[i][j].color = uid(engine);
					block[i][j].pos = { 16 + j * ROWDIS + 4, 161 + i * COLDIS + 4 };
				}
			}
			Goalscore = 5000;
			Turn = 20;
		}
		break;
		case 2:
		{
			std::uniform_int_distribution<> uid(Pink, Purple);
			for (int i = 0; i < BLOCKCOL; ++i) {
				for (int j = 0; j < BLOCKROW; ++j) {
					block[i][j].color = uid(engine);
					block[i][j].pos = { 16 + j * ROWDIS + 4, 161 + i * COLDIS + 4 };
				}
			}
			Goalscore = 7000;
			Turn = 18;
		}
		break;
		case 3:
		{
			std::uniform_int_distribution<> uid(Pink, Orange);
			for (int i = 0; i < BLOCKCOL; ++i) {
				for (int j = 0; j < BLOCKROW; ++j) {
					block[i][j].color = uid(engine);
					block[i][j].pos = { 16 + j * ROWDIS + 4, 161 + i * COLDIS + 4 };
				}
			}
			Goalscore = 10000;
			Turn = 15;
		}
		break;
		}
		SetTimer(hwnd, 1, 500, NULL);
		while (CheckStart());
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			if (!pause) {
				pause = true;
				child_hWnd = CreateWindow("ChildClass", NULL, WS_CHILD | WS_VISIBLE,
					90, 240, 300, 318, hwnd, NULL, g_hInst, NULL);
				play = CreateWindow("button", "Play", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
					75, 60, 150, 57, child_hWnd, (HMENU)IDC_PLAY, g_hInst, NULL);
				hbit = (HBITMAP)LoadImage(g_hInst, "continue.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(play, BM_SETIMAGE, 0, (LPARAM)hbit);

				chexit = CreateWindow("button", "Exit", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
					75, 137, 150, 57, child_hWnd, (HMENU)IDC_EXIT, g_hInst, NULL);
				hbit = (HBITMAP)LoadImage(g_hInst, "exit.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(chexit, BM_SETIMAGE, 0, (LPARAM)hbit);

				retry = CreateWindow("button", "Retry", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
					75, 214, 150, 57, child_hWnd, (HMENU)IDC_RETRY, g_hInst, NULL);
				hbit = (HBITMAP)LoadImage(g_hInst, "retry.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(retry, BM_SETIMAGE, 0, (LPARAM)hbit);
			}
			else {
				DestroyWindow(child_hWnd);
				pause = false;
			}
			break;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_PLAY:
			DestroyWindow(child_hWnd);
			//SendMessage(child_hWnd, uMsg, IDC_PLAY, lParam); 
			break;
		case IDC_EXIT:	
			SendMessage(child_hWnd, uMsg, IDC_EXIT, lParam);
			break;
		case IDC_RETRY:
			SetTimer(hwnd, 1, 500, NULL);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
	{
		if (!pause) {
			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);
			if (LBSel) {
				if (skill != -1) { //스킬이 눌려있을때
					switch (skill) {
					case 0:	//폭탄(공격)
						SkillBomb(mx, my);
						SetTimer(hwnd, 1, 100, NULL);
						SetTimer(hwnd, 6, 200, NULL);
						kirby.Move = true;
						kirby.moveCount = 0;
						selectedSkill = 0;
						break;
					case 1:	//스위치(구르기)
						SkillSwitch(mx, my);
						SetTimer(hwnd, 6, 100, NULL);
						kirby.Move = true;
						kirby.moveCount = 0;
						selectedSkill = 1;
						break;
					default:
						skill = -1;
						break;
					}
				}
				else if (BlockSelect(mx, my)) {
					if (IsSel) {
						IsSel = false;
						LBSel = false;
						for (int i = 0; i < BLOCKCOL; ++i) {
							for (int j = 0; j < BLOCKROW; ++j) {
								returnblock[i][j] = block[i][j];
								returnscore = score;
							}
						}
						SetTimer(hwnd, 2, 1, NULL); //블럭 자리바꾸기 애니메이션
					}
					else {
						IsSel = true;
					}
				}
				if (skill == -1) {
					skill = SkillSelect(mx, my);
				}
				if (skill == 2) {		//턴증가(빨아들이기)
					SetTimer(hwnd, 6, 200, NULL);
					kirby.Move = true;
					kirby.moveCount = 0;
					selectedSkill = 2;
					SkillTurn();
				}
				else if (skill == 3) {	//리버스(전화)
					SetTimer(hwnd, 6, 200, NULL);
					kirby.Move = true;
					kirby.moveCount = 0;
					selectedSkill = 3;
					SkillReturn();
				}
			}
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	}
	case WM_TIMER:
		if (!pause) {
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
							//터지는 애니메이션
							block[i][j].ani++;
							if (block[i][j].ani == 8) {
								block[i][j].ani = 0;
								block[i][j].destroy = false;
								block[i][j].color = -1;
								LBSel = false;
								KillTimer(hwnd, 1);
								SetTimer(hwnd, 3, 1, NULL);

							}
						}
					}
				}
				break;
			case 2: //블럭 자리바꾸기 애니메이션
				switch (dir) {
				case Left:
					block[tmpy][tmpx].pos.x -= (BLOCKSPEED - 6);
					block[sely][selx].pos.x += (BLOCKSPEED - 6);
					cnt++;
					if (cnt == ROWDIS / 3) {
						Object temp = block[tmpy][tmpx];
						block[tmpy][tmpx] = block[sely][selx];
						block[sely][selx] = temp;
						cnt = 0;
						KillTimer(hwnd, 2);
						if (Check3()) {
							dir = 4;
							Turn--;
							SetTimer(hwnd, 1, 100, NULL);
						}
						else {
							SetTimer(hwnd, 5, 1, NULL);
						}
					}
					break;
				case Right:
					block[sely][selx].pos.x -= (BLOCKSPEED - 6);
					block[tmpy][tmpx].pos.x += (BLOCKSPEED - 6);
					cnt++;
					if (cnt == ROWDIS / 3) {
						Object temp = block[tmpy][tmpx];
						block[tmpy][tmpx] = block[sely][selx];
						block[sely][selx] = temp;
						cnt = 0;
						KillTimer(hwnd, 2);
						if (Check3()) {
							dir = 4;
							Turn--;
							SetTimer(hwnd, 1, 100, NULL);
						}
						else {
							SetTimer(hwnd, 5, 1, NULL);
						}
					}
					break;
				case Up:
					block[tmpy][tmpx].pos.y -= (BLOCKSPEED - 6);
					block[sely][selx].pos.y += (BLOCKSPEED - 6);
					cnt++;
					if (cnt == COLDIS / 3) {
						Object temp = block[tmpy][tmpx];
						block[tmpy][tmpx] = block[sely][selx];
						block[sely][selx] = temp;
						cnt = 0;
						KillTimer(hwnd, 2);
						if (Check3()) {
							dir = 4;
							Turn--;
							SetTimer(hwnd, 1, 100, NULL);
						}
						else {
							SetTimer(hwnd, 5, 1, NULL);
						}
					}
					break;
				case Down:
					block[tmpy][tmpx].pos.y += (BLOCKSPEED - 6);
					block[sely][selx].pos.y -= (BLOCKSPEED - 6);
					cnt++;
					if (cnt == COLDIS / 3) {
						Object temp = block[tmpy][tmpx];
						block[tmpy][tmpx] = block[sely][selx];
						block[sely][selx] = temp;
						cnt = 0;
						KillTimer(hwnd, 2);
						if (Check3()) {
							dir = 4;
							Turn--;
							SetTimer(hwnd, 1, 100, NULL);
						}
						else {
							SetTimer(hwnd, 5, 1, NULL);
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
				LBSel = false;
				BlockDown();
				cnt++;
				if (cnt == ROWDIS / 9) {
					for (int i = 0; i < BLOCKROW; ++i) {
						for (int j = 0; j < BLOCKCOL; ++j) {
							if (CheckChange(i, j) && block[j][i].color != -1) {
								BlockChange(i, j);
							}
						}
					}
					if (!CheckDown()) {
						SetTimer(hwnd, 4, 1, NULL);
						KillTimer(hwnd, 3);
					}
					cnt = 0;
				}
				break;
			case 4: //새로운 블럭만들기
			{
				bool timer3{ false };
				NewBlock();
				KillTimer(hwnd, 4);
				for (int i = 0; i < BLOCKROW; ++i) {
					if (CheckDown()) {
						SetTimer(hwnd, 3, 1, NULL);
						timer3 = true;
						break;
					}
				}
				if (!timer3) {
					if (Check3()) {
						SetTimer(hwnd, 1, 100, NULL);
					}
					else {
						LBSel = true;
						SetTimer(hwnd, 1, 500, NULL);
					}
				}
			}
			break;
			case 5: // 블럭제자리(자리를 바꿨는데 터지지 않을때)
				switch (dir) {
				case Left:
					block[tmpy][tmpx].pos.x -= (BLOCKSPEED - 6);
					block[sely][selx].pos.x += (BLOCKSPEED - 6);
					cnt++;
					if (cnt == ROWDIS / 3) {
						Object temp = block[tmpy][tmpx];
						block[tmpy][tmpx] = block[sely][selx];
						block[sely][selx] = temp;
						cnt = 0; dir = 4;
						KillTimer(hwnd, 5);
					}
					break;
				case Right:
					block[tmpy][tmpx].pos.x += (BLOCKSPEED - 6);
					block[sely][selx].pos.x -= (BLOCKSPEED - 6);
					cnt++;
					if (cnt == ROWDIS / 3) {
						Object temp = block[tmpy][tmpx];
						block[tmpy][tmpx] = block[sely][selx];
						block[sely][selx] = temp;
						cnt = 0; dir = 4;
						KillTimer(hwnd, 5);
					}
					break;
				case Up:
					block[tmpy][tmpx].pos.y -= (BLOCKSPEED - 6);
					block[sely][selx].pos.y += (BLOCKSPEED - 6);
					cnt++;
					if (cnt == COLDIS / 3) {
						Object temp = block[tmpy][tmpx];
						block[tmpy][tmpx] = block[sely][selx];
						block[sely][selx] = temp;
						cnt = 0; dir = 4;
						KillTimer(hwnd, 5);
					}
					break;
				case Down:
					block[tmpy][tmpx].pos.y += (BLOCKSPEED - 6);
					block[sely][selx].pos.y -= (BLOCKSPEED - 6);
					cnt++;
					if (cnt == COLDIS / 3) {
						Object temp = block[tmpy][tmpx];
						block[tmpy][tmpx] = block[sely][selx];
						block[sely][selx] = temp;
						cnt = 0; dir = 4;
						KillTimer(hwnd, 5);
					}
					break;
				}
				LBSel = true;
				break;
			case 6:	//캐릭터 애니메이션
				if (kirby.Move) {
					if (kirby.aniIndex < skillAni[selectedSkill]) {
						kirby.aniIndex++;
						if (kirby.movedir == 1) kirby.Pos.x += 10;
						else kirby.Pos.x -= 10;
						if (kirby.aniIndex == skillAni[selectedSkill] - 1) {
							kirby.moveCount++;
							kirby.aniIndex = 0;
							if (kirby.moveCount != 2) kirby.movedir *= -1;
						}
						if (kirby.moveCount == 2) {
							kirby.Move = false;
							KillTimer(hwnd, 6);
						}
					}
				}
				break;
				LBSel = true;
			}
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
		PrintBoard(hdc);
		PrintSkill(hdc);
		PrintBlock(hdc);
		PrintScore(hdc);
		PrintTurn(hdc);
		PrintKirby(hdc);
		if (IsSel) {
			PrintRect(hdc);
		}
		if (skill != -1) {
			PrintSkillRect(hdc);
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
		
		//게임종료
		if (score >= Goalscore) {
			KillTimer(hwnd, 1);
			KillTimer(hwnd, 2);
			KillTimer(hwnd, 3);
			KillTimer(hwnd, 4);
			MessageBox(hwnd, "목표달성!", "알림", MB_OK);
			PostQuitMessage(0);
		}
		else if (Turn == 0) {
			KillTimer(hwnd, 1);
			KillTimer(hwnd, 2);
			KillTimer(hwnd, 3);
			KillTimer(hwnd, 4);
			MessageBox(hwnd, "목표실패!", "알림", MB_OK);
			PostQuitMessage(0);
		}
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	static Bit chbackground;
	BITMAP bmp;
	HDC memdc, hdc, MemDC, tmpDC;
	HBITMAP oldbit, BackBit, oldBackBit;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		chbackground.bit = (HBITMAP)LoadImage(g_hInst, "pause.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(chbackground.bit, sizeof(BITMAP), &bmp);
		chbackground.bitmapSize.x = bmp.bmWidth;
		chbackground.bitmapSize.y = bmp.bmHeight;
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_PLAY:
			pause = false;
			DestroyWindow(hWnd);
			break;
		case IDC_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDC_RETRY:
			Reset();
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		memdc = CreateCompatibleDC(hdc);
		oldbit = (HBITMAP)SelectObject(memdc, chbackground.bit);
		TransparentBlt(hdc, 0, 0, chbackground.bitmapSize.x, chbackground.bitmapSize.y, memdc, 0, 0, chbackground.bitmapSize.x, chbackground.bitmapSize.y, RGB(201, 206 , 181));
		SelectObject(memdc, oldbit);
		DeleteObject(memdc);
		DeleteObject(oldbit);

		EndPaint(hWnd, &ps);
		break;
	}

	case WM_DESTROY:
		DestroyWindow(hWnd);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
bool CheckStart() {
	std::uniform_int_distribution<> uid1(Pink, Gray);
	std::uniform_int_distribution<> uid2(Pink, Purple);
	std::uniform_int_distribution<> uid3(Pink, Orange);
	switch(stage) {
	case 1:
		for (int i = 0; i < BLOCKCOL; ++i) {
			for (int j = 0; j < BLOCKROW - 2; ++j) {
				if (block[i][j].color == block[i][j + 1].color && block[i][j].color == block[i][j + 2].color) {
					while (block[i][j].color == block[i][j + 1].color) {
						block[i][j].color = uid1(engine);
					}
					return true;
				}
			}
		}
		for (int i = 0; i < BLOCKROW; ++i) {
			for (int j = 0; j < BLOCKCOL - 2; ++j) {
				if (block[j][i].color == block[j + 1][i].color && block[j][i].color == block[j + 2][i].color) {
					while (block[j][i].color == block[j + 1][i].color) {
						block[j][i].color = uid1(engine);
					}
					return true;
				}
			}
		}
		break;
	case 2:
		for (int i = 0; i < BLOCKCOL; ++i) {
			for (int j = 0; j < BLOCKROW - 2; ++j) {
				if (block[i][j].color == block[i][j + 1].color && block[i][j].color == block[i][j + 2].color) {
					while (block[i][j].color == block[i][j + 1].color) {
						block[i][j].color = uid2(engine);
					}
					return true;
				}
			}
		}
		for (int i = 0; i < BLOCKROW; ++i) {
			for (int j = 0; j < BLOCKCOL - 2; ++j) {
				if (block[j][i].color == block[j + 1][i].color && block[j][i].color == block[j + 2][i].color) {
					while (block[j][i].color == block[j + 1][i].color) {
						block[j][i].color = uid2(engine);
					}
					return true;
				}
			}
		}
		break;
	case 3:
		for (int i = 0; i < BLOCKCOL; ++i) {
			for (int j = 0; j < BLOCKROW - 2; ++j) {
				if (block[i][j].color == block[i][j + 1].color && block[i][j].color == block[i][j + 2].color) {
					while (block[i][j].color == block[i][j + 1].color) {
						block[i][j].color = uid3(engine);
					}
					return true;
				}
			}
		}
		for (int i = 0; i < BLOCKROW; ++i) {
			for (int j = 0; j < BLOCKCOL - 2; ++j) {
				if (block[j][i].color == block[j + 1][i].color && block[j][i].color == block[j + 2][i].color) {
					while (block[j][i].color == block[j + 1][i].color) {
						block[j][i].color = uid3(engine);
					}
					return true;
				}
			}
		}
		break;

	}
	return false;
}
void PrintBackGround(HDC hdc) {
	HDC memdc;
	HBITMAP Oldbit;
	memdc = CreateCompatibleDC(hdc);
	Oldbit = (HBITMAP)SelectObject(memdc, Background.bit);
	BitBlt(hdc, 0, 0, Background.bitmapSize.x, Background.bitmapSize.y, memdc, 0, 0, SRCCOPY);
	SelectObject(memdc, Oldbit);
	DeleteObject(memdc);
	DeleteObject(Oldbit);
}

void PrintBoard(HDC hdc) {
	HBRUSH hbrush, oldbrush;
	HPEN hpen, oldpen;
	
	hpen = CreatePen(PS_SOLID, 3, RGB(150, 150, 150));
	oldpen = (HPEN)SelectObject(hdc, hpen);
	Rectangle(hdc, 16, 161, 16 + BLOCKROW * 45, 161 + BLOCKCOL * 45);
	SelectObject(hdc, oldpen);
	DeleteObject(hpen);

	for (int i = 0; i < BLOCKCOL; ++i) {
		for (int j = 0; j < BLOCKROW; ++j) {
			hbrush = CreateSolidBrush(RGB(255, 255, 255));
			hpen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
			oldpen = (HPEN)SelectObject(hdc, hpen);
			oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
			Rectangle(hdc, 16 + j * 45, 161 + i * 45, 16 + (j + 1) * 45, 161 + (i + 1) * 45);
			SelectObject(hdc, oldbrush);
			DeleteObject(hbrush);
			SelectObject(hdc, oldpen);
			DeleteObject(hpen);
		}
	}
	
}
void PrintSkill(HDC hdc) {
	HDC memdc;
	HBITMAP Oldbit;
	for (int i = 0; i < 4; ++i) {
		memdc = CreateCompatibleDC(hdc);
		Oldbit = (HBITMAP)SelectObject(memdc, Skill[i].bit);
		TransparentBlt(hdc, 16 + SKILLDIS * i, 710, Skill[i].bitmapSize.x, Skill[i].bitmapSize.y, memdc, 0, 0, Skill[i].bitmapSize.x, Skill[i].bitmapSize.y, RGB(201, 206, 181));
		SelectObject(memdc, Oldbit);
		DeleteObject(memdc);
		DeleteObject(Oldbit);
	}
}
void PrintBlock(HDC hdc) {
	HDC memdc;
	HBITMAP Oldbit;

	for (int i = 0; i < BLOCKCOL; ++i) {
		for (int j = 0; j < BLOCKROW; ++j) {
			switch (block[i][j].color) {
			case Pink: //Pink
				memdc = CreateCompatibleDC(hdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[0].bit);
				TransparentBlt(hdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE, BLOCKSIZE, memdc, (Block[0].bitmapSize.x / 4 * block[i][j].ani), 0, Block[0].bitmapSize.x / 4, Block[0].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case Blue: //Blue
				memdc = CreateCompatibleDC(hdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[1].bit);
				TransparentBlt(hdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE, BLOCKSIZE, memdc, (Block[1].bitmapSize.x / 4 * block[i][j].ani), 0, Block[1].bitmapSize.x / 4, Block[1].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case Green: //Green
				memdc = CreateCompatibleDC(hdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[2].bit);
				TransparentBlt(hdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE, BLOCKSIZE, memdc, (Block[2].bitmapSize.x / 4 * block[i][j].ani), 0, Block[2].bitmapSize.x / 4, Block[2].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case Gray: //Gray
				memdc = CreateCompatibleDC(hdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[3].bit);
				TransparentBlt(hdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE, BLOCKSIZE, memdc, (Block[3].bitmapSize.x / 4 * block[i][j].ani), 0, Block[3].bitmapSize.x / 4, Block[3].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case Purple: //Purple
				memdc = CreateCompatibleDC(hdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[4].bit);
				TransparentBlt(hdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE, BLOCKSIZE, memdc, (Block[4].bitmapSize.x / 4 * block[i][j].ani), 0, Block[4].bitmapSize.x / 4, Block[4].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case Orange: //Orange
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
				TransparentBlt(hdc, block[i][j].pos.x - 5, block[i][j].pos.y - 4, Block[6].bitmapSize.x / 8, Block[6].bitmapSize.y, memdc, (Block[6].bitmapSize.x / 8 * block[i][j].ani), 0, Block[6].bitmapSize.x / 8, Block[6].bitmapSize.y, RGB(201, 206, 181));
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
void PrintScore(HDC hdc) {
	TCHAR str[6];
	HFONT hfont, oldfont;
	wsprintf(str, "%d", score);
	TextOut(hdc, 230, 50, str, strlen(str));;
	wsprintf(str, "%d", Goalscore);
	TextOut(hdc, 380, 50, str, strlen(str));
}
void PrintTurn(HDC hdc) {
	TCHAR str[3];
	HFONT hfont, oldfont;
	wsprintf(str, "%d", Turn);
	TextOut(hdc, 75, 50, str, strlen(str));
}

void PrintKirby(HDC hdc) {
	HDC memdc;
	HBITMAP Oldbit;
	memdc = CreateCompatibleDC(hdc);
	switch (selectedSkill) {
	case 0:
		if (kirby.movedir == 1) kirby.hBitmap = (HBITMAP)LoadImage(g_hInst, "rightattack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		else kirby.hBitmap = (HBITMAP)LoadImage(g_hInst, "leftattack.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case 1:
		if (kirby.movedir == 1) kirby.hBitmap = (HBITMAP)LoadImage(g_hInst, "rightroll.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		else kirby.hBitmap = (HBITMAP)LoadImage(g_hInst, "leftroll.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case 2:
		if (kirby.movedir == 1) kirby.hBitmap = (HBITMAP)LoadImage(g_hInst, "rightinhale.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		else kirby.hBitmap = (HBITMAP)LoadImage(g_hInst, "leftinhale.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	case 3:
		if (kirby.movedir == 1) kirby.hBitmap = (HBITMAP)LoadImage(g_hInst, "rightcall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		else kirby.hBitmap = (HBITMAP)LoadImage(g_hInst, "leftcall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		break;
	}
	Oldbit = (HBITMAP)SelectObject(memdc, kirby.hBitmap);
	TransparentBlt(hdc, kirby.Pos.x, kirby.Pos.y, KIRBYSIZE, KIRBYSIZE, memdc, kirby.aniIndex * kirby.bitmapSize.y, 0, kirby.bitmapSize.y, kirby.bitmapSize.y, kirbyRGB);
	SelectObject(memdc, Oldbit);
	DeleteObject(memdc);
	DeleteObject(Oldbit);
}

bool BlockSelect(int mx, int my) {
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
	RECT rect = {block[sely][selx].pos.x - 5, block[sely][selx].pos.y - 5, block[sely][selx].pos.x + BLOCKSIZE + 5, block[sely][selx].pos.y + BLOCKSIZE + 5};
	HPEN hpen, oldpen;
	hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	oldpen = (HPEN)SelectObject(hdc, hpen);
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, block[sely][selx].pos.x - 5, block[sely][selx].pos.y - 5, block[sely][selx].pos.x + BLOCKSIZE + 5, block[sely][selx].pos.y + BLOCKSIZE + 5);
	SelectObject(hdc, oldpen);
	DeleteObject(hpen);
}

int SkillSelect(int mx, int my) {
	for (int i = 0; i < 4; ++i) {
		if (mx >= 16 + SKILLDIS * i && mx <= 16 + SKILLDIS * i + Skill[i].bitmapSize.x && my >= 710 && my <= 710 + Skill[i].bitmapSize.y) {
			IsSel = false;
			return i;
		}
	}
	return -1;
}
void PrintSkillRect(HDC hdc) {
	HPEN hpen, oldpen;
	hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	oldpen = (HPEN)SelectObject(hdc, hpen);
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, 16 + SKILLDIS * skill, 710, 16 + SKILLDIS * skill + Skill[skill].bitmapSize.x, 710 + Skill[skill].bitmapSize.y);
	SelectObject(hdc, oldpen);
	DeleteObject(hpen);
}
void SkillBomb(int mx, int my) {
	for (int i = 0; i < BLOCKCOL; ++i) {
		for (int j = 0; j < BLOCKROW; ++j) {
			if (block[i][j].pos.x <= mx && block[i][j].pos.x + BLOCKSIZE >= mx && block[i][j].pos.y <= my && block[i][j].pos.y + BLOCKSIZE >= my) {
				if (i == 0 && j== 0) {
					block[i][j].destroy = true; block[i + 1][j].destroy = true; block[i + 1][j + 1].destroy = true; block[i][j + 1].destroy = true;
					block[i][j].color = 6;      block[i + 1][j].color = 6;      block[i + 1][j + 1].color = 6;      block[i][j + 1].color = 6;
					block[i][j].ani = 0;        block[i + 1][j].ani = 0;        block[i + 1][j + 1].ani = 0;        block[i][j + 1].ani = 0;
					score += 40;
				}
				else if (i == BLOCKCOL - 1 && j == BLOCKROW - 1) {
					block[i][j].destroy = true; block[i - 1][j].destroy = true; block[i - 1][j - 1].destroy = true; block[i][j - 1].destroy = true;
					block[i][j].color = 6;      block[i - 1][j].color = 6;      block[i - 1][j - 1].color = 6;      block[i][j - 1].color = 6;
					block[i][j].ani = 0;        block[i - 1][j].ani = 0;        block[i - 1][j - 1].ani = 0;        block[i][j - 1].ani = 0;
					score += 40;
				}
				else if (i == 0 && j == BLOCKROW - 1) {
					block[i][j].destroy = true; block[i + 1][j].destroy = true; block[i + 1][j - 1].destroy = true; block[i][j - 1].destroy = true;
					block[i][j].color = 6;      block[i + 1][j].color = 6;      block[i + 1][j - 1].color = 6;      block[i][j - 1].color = 6;
					block[i][j].ani = 0;        block[i + 1][j].ani = 0;        block[i + 1][j - 1].ani = 0;        block[i][j - 1].ani = 0;
					score += 40;
				}
				else if (i == BLOCKCOL - 1 && j == 0) {
					block[i][j].destroy = true; block[i - 1][j].destroy = true; block[i - 1][j + 1].destroy = true; block[i][j + 1].destroy = true;
					block[i][j].color = 6;      block[i - 1][j].color = 6;      block[i - 1][j + 1].color = 6;      block[i][j + 1].color = 6;
					block[i][j].ani = 0;        block[i - 1][j].ani = 0;        block[i - 1][j + 1].ani = 0;        block[i][j + 1].ani = 0;
					score += 40;
				}
				else if (i == 0) {
					block[i][j].destroy = true; block[i][j + 1].destroy = true; block[i][j - 1].destroy = true; block[i + 1][j].destroy = true; block[i + 1][j + 1].destroy = true; block[i + 1][j - 1].destroy = true;
					block[i][j].color = 6;      block[i][j + 1].color = 6;      block[i][j - 1].color = 6;      block[i + 1][j].color = 6;      block[i + 1][j + 1].color = 6;      block[i + 1][j - 1].color = 6;
					block[i][j].ani = 0;        block[i][j + 1].ani = 0;        block[i][j - 1].ani = 0;        block[i + 1][j].ani = 0;        block[i + 1][j + 1].ani = 0;        block[i + 1][j - 1].ani = 0;
					score += 60;
				}
				else if (i == BLOCKCOL - 1) {
					block[i][j].destroy = true; block[i][j + 1].destroy = true; block[i][j - 1].destroy = true; block[i - 1][j].destroy = true; block[i - 1][j + 1].destroy = true; block[i - 1][j - 1].destroy = true;
					block[i][j].color = 6;      block[i][j + 1].color = 6;      block[i][j - 1].color = 6;      block[i - 1][j].color = 6;      block[i - 1][j + 1].color = 6;      block[i - 1][j - 1].color = 6;
					block[i][j].ani = 0;        block[i][j + 1].ani = 0;        block[i][j - 1].ani = 0;        block[i - 1][j].ani = 0;        block[i - 1][j + 1].ani = 0;        block[i - 1][j - 1].ani = 0;
					score += 60;
				}
				else if (j == 0) {
					block[i][j].destroy = true; block[i + 1][j].destroy = true; block[i - 1][j].destroy = true; block[i][j + 1].destroy = true; block[i + 1][j + 1].destroy = true; block[i - 1][j + 1].destroy = true;
					block[i][j].color = 6;      block[i + 1][j].color = 6;      block[i - 1][j].color = 6;      block[i][j + 1].color = 6;      block[i + 1][j + 1].color = 6;      block[i - 1][j + 1].color = 6;
					block[i][j].ani = 0;        block[i + 1][j].ani = 0;        block[i - 1][j].ani = 0;        block[i][j + 1].ani = 0;        block[i + 1][j + 1].ani = 0;        block[i - 1][j + 1].ani = 0;
					score += 60;
				}
				else if (j == BLOCKROW - 1) {
					block[i][j].destroy = true; block[i + 1][j].destroy = true; block[i - 1][j].destroy = true; block[i][j - 1].destroy = true; block[i + 1][j - 1].destroy = true; block[i - 1][j - 1].destroy = true;
					block[i][j].color = 6;      block[i + 1][j].color = 6;      block[i - 1][j].color = 6;      block[i][j - 1].color = 6;      block[i + 1][j - 1].color = 6;      block[i - 1][j - 1].color = 6;
					block[i][j].ani = 0;        block[i + 1][j].ani = 0;        block[i - 1][j].ani = 0;        block[i][j - 1].ani = 0;        block[i + 1][j - 1].ani = 0;        block[i - 1][j - 1].ani = 0;
					score += 60;
				}
				else {
					block[i][j].destroy = true; block[i + 1][j].destroy = true; block[i - 1][j].destroy = true; block[i][j + 1].destroy = true; block[i][j - 1].destroy = true; block[i - 1][j - 1].destroy = true; block[i - 1][j + 1].destroy = true; block[i + 1][j + 1].destroy = true; block[i + 1][j - 1].destroy = true;
					block[i][j].color = 6;      block[i + 1][j].color = 6;      block[i - 1][j].color = 6;      block[i][j + 1].color = 6;      block[i][j - 1].color = 6;      block[i - 1][j - 1].color = 6;	    block[i - 1][j + 1].color = 6;	    block[i + 1][j + 1].color = 6;	    block[i + 1][j - 1].color = 6;
					block[i][j].ani = 0;        block[i + 1][j].ani = 0;        block[i - 1][j].ani = 0;        block[i][j + 1].ani = 0;        block[i][j - 1].ani = 0;        block[i - 1][j - 1].ani = 0;	    block[i - 1][j + 1].ani = 0;	    block[i + 1][j + 1].ani = 0;	    block[i + 1][j - 1].ani = 0;
					score += 90;
				}
			}
		}
	}
	skill = -1;
	Turn--;
}
void SkillSwitch(int mx, int my) {
	for (int i = 0; i < BLOCKCOL; ++i) {
		for (int j = 0; j < BLOCKROW; ++j) {
			if (block[i][j].pos.x <= mx && block[i][j].pos.x + BLOCKSIZE >= mx && block[i][j].pos.y <= my && block[i][j].pos.y + BLOCKSIZE >= my) {
				if (!IsSel) {
					selx = j;
					sely = i;
					IsSel = true;
				}
				else {
					tmpx = selx;
					tmpy = sely;
					selx = j;
					sely = i;
					IsSel = false;
					skill = -1;
				}
			}
		}
	}
}
void SkillTurn() {
	switch (stage) {
	case 1:
		if (Turn <= 15) {
			Turn += 5;
		}
		break;
	case 2:
		if (Turn <= 13) {
			Turn += 5;
		}
		break;
	case 3:
		if (Turn <= 10) {
			Turn += 5;
		}
		break;
	}
	skill = -1;
}
void SkillReturn() {
	switch (stage) {
	case 1:
		if (Turn < 20) {
			for (int i = 0; i < BLOCKCOL; ++i) {
				for (int j = 0; j < BLOCKROW; ++j) {
					block[i][j] = returnblock[i][j];
				}
			}
			score = returnscore;
			Turn--;
		}
		break;
	case 2:
		if (Turn < 18) {
			for (int i = 0; i < BLOCKCOL; ++i) {
				for (int j = 0; j < BLOCKROW; ++j) {
					block[i][j] = returnblock[i][j];
				}
			}
			score = returnscore;
			Turn--;
		}
		break;
	case 3:
		if (Turn < 15) {
			for (int i = 0; i < BLOCKCOL; ++i) {
				for (int j = 0; j < BLOCKROW; ++j) {
					block[i][j] = returnblock[i][j];
				}
			}
			score = returnscore;
			Turn--;
		}
		break;
	}
	skill = -1;
}

bool Check3() {
	bool check{ false };
	//가로확인
	for (int i = 0; i < BLOCKCOL; ++i) {
		for (int j = 0; j < BLOCKROW - 2; ++j) {
			Score();
			if (block[i][j].color == block[i][j + 1].color && block[i][j].color == block[i][j + 2].color && block[i][j].color != -1) {
				//가로만 3개이상
				while (block[i][j].color == block[i][j + 1].color) {
					int tempcolor = block[i][j].color;
					block[i][j].destroy = true;
					block[i][j].color = 6;
					block[i][j].ani = 0;
					destorynum++;
					if (j == BLOCKROW - 2) {
						block[i][j + 1].destroy = true;
						block[i][j + 1].color = 6;
						block[i][j + 1].ani = 0;
						destorynum++;
						//break;
					}
					if (tempcolor == block[i + 1][j].color && tempcolor == block[i + 2][j].color) {
						//가로세로(위) 모두 3개이상
						int temp = i + 1;
						while (block[temp][j].color == block[temp + 1][j].color) {
							block[temp][j].destroy = true;
							block[temp][j].color = 6;
							block[temp][j].ani = 0;
							destorynum++;
							if (temp == BLOCKCOL - 2) {
								block[temp + 1][j].destroy = true;
								block[temp + 1][j].color = 6;
								block[temp + 1][j].ani = 0;
								destorynum++;
								break;
							}
							temp++;
						}
						block[temp][j].destroy = true;
						block[temp][j].color = 6;
						block[temp][j].ani = 0;
						destorynum++;
					}
					if (i >= 2 && tempcolor == block[i - 1][j].color && tempcolor == block[i - 2][j].color) {
						//가로세로(아래) 모두 3개이상
						int temp = i - 1;
						while (block[temp][j].color == block[temp - 1][j].color) {
							block[temp][j].destroy = true;
							block[temp][j].color = 6;
							block[temp][j].ani = 0;
							destorynum++;
							if (temp == 1) {
								block[temp - 1][j].destroy = true;
								block[temp - 1][j].color = 6;
								block[temp - 1][j].ani = 0;
								destorynum++;
								break;
							}
							temp--;
						}
						block[temp][j].destroy = true;
						block[temp][j].color = 6;
						block[temp][j].ani = 0;
						destorynum++;
					}
					j++;

				}
				int tempcolor = block[i][j].color;
				block[i][j].destroy = true;
				block[i][j].color = 6;
				block[i][j].ani = 0;
				destorynum++;
				if (tempcolor == block[i + 1][j].color && tempcolor == block[i + 2][j].color) {
					//가로세로(위) 모두 3개이상
					int temp = i + 1;
					while (block[temp][j].color == block[temp + 1][j].color) {
						block[temp][j].destroy = true;
						block[temp][j].color = 6;
						block[temp][j].ani = 0;
						destorynum++;
						if (temp == BLOCKCOL - 2) {
							block[temp + 1][j].destroy = true;
							block[temp + 1][j].color = 6;
							block[temp + 1][j].ani = 0;
							destorynum++;
							break;
						}
						temp++;
					}
					block[temp][j].destroy = true;
					block[temp][j].color = 6;
					block[temp][j].ani = 0;
					destorynum++;
				}
				if (i >= 2 && tempcolor == block[i - 1][j].color && tempcolor == block[i - 2][j].color) {
					//가로세로(아래) 모두 3개이상
					int temp = i - 1;
					while (block[temp][j].color == block[temp - 1][j].color) {
						block[temp][j].destroy = true;
						block[temp][j].color = 6;
						block[temp][j].ani = 0;
						destorynum++;
						if (temp == 1) {
							block[temp - 1][j].destroy = true;
							block[temp - 1][j].color = 6;
							block[temp - 1][j].ani = 0;
							destorynum++;
							break;
						}
						temp--;
					}
					block[temp][j].destroy = true;
					block[temp][j].color = 6;
					block[temp][j].ani = 0;
					destorynum++;
				}
				check = true;
			}
		}
	}

	//세로확인
	for (int i = 0; i < BLOCKROW; ++i) {
		for (int j = 0; j < BLOCKCOL - 2; ++j) {
			Score();
			if (block[j][i].color == block[j + 1][i].color && block[j][i].color == block[j + 2][i].color && block[j][i].color != -1) {
				while (block[j][i].color == block[j + 1][i].color) {
					block[j][i].destroy = true;
					block[j][i].color = 6;
					block[j][i].ani= 0;
					destorynum++;
					if (j == BLOCKCOL - 2) {
						block[j + 1][i].destroy = true;
						block[j + 1][i].color = 6;
						block[j + 1][i].ani = 0;
						destorynum++;
						break;
					}
					j++;
				}
				block[j][i].destroy = true;
				block[j][i].color = 6;
				block[j][i].ani = 0;
				destorynum++;
				check = true;
			}
		}
	}
	return check;
}
void Score() {
	switch (destorynum) {
	case 1:
		score += destorynum * 10;
		break;
	case 2:
		score += destorynum * 15;
		break;
	case 3:
		score += destorynum * 20;
		break;
	case 4:
		score += destorynum * 25;
		break;
	case 5:
		score += destorynum * 30;
		break;
	case 6:
		score += destorynum * 35;
		break;
	case 7:
		score += destorynum * 40;
		break;
	default:
		break;
	}
	destorynum = 0;
}
void BlockDown()	 {
	for (int i = 0; i < BLOCKROW; ++i) {
		for (int j = 0; j < BLOCKCOL - 1; ++j) {
			if (block[j + 1][i].color == -1 && block[j][i].color != -1) {
				block[j][i].pos.y += BLOCKSPEED;
			}
		}
	}
}
bool CheckDown() {
	for (int i = 0; i < BLOCKROW; ++i) {
		for (int j = 1; j < BLOCKCOL; ++j) {
			if (block[j][i].color == -1 && block[j - 1][i].color != -1) {
				return true;
			}
		}
	}
	return false;
} 
void BlockChange(int i, int j) {
	for (int l = 0; l < BLOCKCOL; ++l) {
		if (block[j][i].pos.x == block[l][i].pos.x && block[j][i].pos.y == block[l][i].pos.y && j != l && l == BLOCKCOL - 1) {
			Object temp = block[l][i];
			block[l][i] = block[j][i];
			block[j][i] = temp;
			block[j][i].pos.y -= (l - j) * COLDIS;
		}
		else if (block[j][i].pos.x == block[l][i].pos.x && block[j][i].pos.y == block[l][i].pos.y && j != l && block[l + 1][i].color != -1) {
			Object temp = block[l][i];
			block[l][i] = block[j][i];
			block[j][i] = temp;
			block[j][i].pos.y -= (l - j) * COLDIS;
		}
	}
}
bool CheckChange(int i, int j) {
	for (int k = 0; k < BLOCKROW; ++k) {
		for (int l = 0; l < BLOCKCOL; ++l) {
			if (block[j][i].pos.x == block[l][k].pos.x && block[j][i].pos.y == block[l][k].pos.y && j != l && l == BLOCKCOL - 1) {
				return true;
			}
			else if (block[j][i].pos.x == block[l][k].pos.x && block[j][i].pos.y == block[l][k].pos.y && j != l && block[l + 1][k].color != -1) {
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
				block[0][i].pos = { 16 + i * ROWDIS + 4, 161 + 4 };
			}
			break;
			case 2:
			{
				std::uniform_int_distribution<> uid(0, 4);
				block[0][i].color = uid(engine);
				block[0][i].pos = { 16 + i * ROWDIS + 4, 161 + 4 };
			}
			break;
			case 3:
			{
				std::uniform_int_distribution<> uid(0, 5);
				block[0][i].color = uid(engine);
				block[0][i].pos = { 16 + i * ROWDIS + 4, 161 + 4 };
			}
			break;
			}
		}
	}
}
void Reset() {
	cnt = 0;
	score = 0;
	skill = -1;
	LBSel = true; pause = false;
	switch (stage) {
	case 1:
	{
		std::uniform_int_distribution<> uid(Pink, Gray);
		for (int i = 0; i < BLOCKCOL; ++i) {
			for (int j = 0; j < BLOCKROW; ++j) {
				block[i][j].color = uid(engine);
				block[i][j].pos = { 16 + j * ROWDIS + 4, 161 + i * COLDIS + 4 };
			}
		}
		Goalscore = 5000;
		Turn = 20;
	}
	break;
	case 2:
	{
		std::uniform_int_distribution<> uid(Pink, Purple);
		for (int i = 0; i < BLOCKCOL; ++i) {
			for (int j = 0; j < BLOCKROW; ++j) {
				block[i][j].color = uid(engine);
				block[i][j].pos = { 16 + j * ROWDIS + 4, 161 + i * COLDIS + 4 };
			}
		}
		Goalscore = 7000;
		Turn = 18;
	}
	break;
	case 3:
	{
		std::uniform_int_distribution<> uid(Pink, Orange);
		for (int i = 0; i < BLOCKCOL; ++i) {
			for (int j = 0; j < BLOCKROW; ++j) {
				block[i][j].color = uid(engine);
				block[i][j].pos = { 16 + j * ROWDIS + 4, 161 + i * COLDIS + 4 };
			}
		}
		Goalscore = 10000;
		Turn = 15;
	}
	break;
	}
	while (CheckStart());
}