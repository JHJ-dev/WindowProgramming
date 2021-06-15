#include "Game.h"

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
	//스테이지씬
	HDC hdc, memdc1, memdc2;
	static HBITMAP hBitmap;
	static HBITMAP oldBitmap1, oldBitmap2;
	static RECT crt;
	PAINTSTRUCT ps;
	BITMAP bmp;

	//게임씬
	static HWND child_hWnd;
	HDC ghdc, MemDC, tmpDC;
	PAINTSTRUCT gps;
	HBITMAP BackBit, oldBackBit, hbit;

	switch (uMsg) {
	case WM_CREATE:
	{
		Sound_Setup();
		FMOD_System_PlaySound(System, bgmSound[0], NULL, 0, &Channel[0]);
		FMOD_Channel_SetVolume(Channel[0], 0.1);
		scene = 0;
		//게임씬
		{
			cnt = 0;
			skill = -1;
			LBSel = true; pause = false; exittost = false;
			GetClientRect(hwnd, &WindowRect);
			GetClientRect(hwnd, &crt);
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
		}

		//스테이지씬
		{
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
			stagekirby.hBitmap = (HBITMAP)LoadImage(g_hInst, "rightwalk.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(stagekirby.hBitmap, sizeof(BITMAP), &bmp);
			stagekirby.bitmapSize = { bmp.bmWidth, bmp.bmHeight };
			stagekirby.centerPos = { stage12[0].x, stage12[0].y - bmp.bmHeight / 2 };
			stagekirby.drawPos = { stagekirby.centerPos.x - bmp.bmHeight / 2, stagekirby.centerPos.y - bmp.bmHeight / 2 };
			stagekirby.aniIndex = 3;
			//페이드
			fade.hBitmap = (HBITMAP)LoadImage(g_hInst, "fade.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(fade.hBitmap, sizeof(BITMAP), &bmp);
			fade.bitmapSize = { bmp.bmWidth, bmp.bmHeight };
			//타이틀
			title.hBitmap = (HBITMAP)LoadImage(g_hInst, "title.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			GetObject(title.hBitmap, sizeof(BITMAP), &bmp);
			title.bitmapSize = { bmp.bmWidth, bmp.bmHeight };

			nowStage = 0;
			SetTimer(hwnd, 12, 100, NULL);
		}
		break;
	}

	case WM_KEYDOWN:
	{
		//게임씬
		if (scene == 1) {
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
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		//게임씬
		if (scene == 1) {
			if (!pause) {
				int mx = LOWORD(lParam);
				int my = HIWORD(lParam);
				if (LBSel) {
					if (skill != -1) { //스킬이 눌려있을때
						switch (skill) {
						case 0:	//폭탄(공격)
							if (bombnum != 0) {
								SkillBomb(mx, my);
								SetTimer(hwnd, 1, 100, NULL);
								SetTimer(hwnd, 6, 200, NULL);
								kirby.Move = true;
								kirby.moveCount = 0;
								selectedSkill = 0;
							}
							break;
						case 1:	//스위치(구르기)
							if (switchnum != 0) {
								SkillSwitch(mx, my);
								if (!IsSel) {
									KillTimer(hwnd, 1);
									SetTimer(hwnd, 7, 50, NULL);
									SetTimer(hwnd, 6, 100, NULL);
									kirby.Move = true;
									kirby.moveCount = 0;
									selectedSkill = 1;
								}
							}
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
								}
							}
							returnscore = score;
							SetTimer(hwnd, 2, 1, NULL); //블럭 자리바꾸기 애니메이션
						}
						else {
							IsSel = true;
						}
					}
					if (skill == -1) {
						skill = SkillSelect(mx, my);
					}
					if (skill == 2 && turnnum != 0) { //턴증가(빨아들이기)
						SetTimer(hwnd, 6, 200, NULL);
						kirby.Move = true;
						kirby.moveCount = 0;
						selectedSkill = 2;
						SkillTurn();
					}
					else if (skill == 3 && returnnum != 0) { //리버스(전화)
						SetTimer(hwnd, 6, 200, NULL);
						kirby.Move = true;
						kirby.moveCount = 0;
						selectedSkill = 3;
						SkillReturn();
					}
				}
			}
			InvalidateRect(hwnd, NULL, FALSE);
		}
		//스테이지씬
		else {
			//스테이지 선택
			if (!stagekirby.Move) {
				mx = LOWORD(lParam);
				my = HIWORD(lParam);
				for (i = 0; i < STAGECNT; ++i) {
					if (InCircle({ stageButton[i].centerPos.x - cx, stageButton[i].centerPos.y - cy }, { mx, my })) {
						Loading = false;
						loadingCnt = 0;
						SetTimer(hwnd, 11, 60, NULL);
						selectedStage = i;
						stage = i + 1;
						if (selectedStage < nowStage) {			//2->1, 3->2
							stagekirby.moveIndex = POINTCNT - 1;
							stagekirby.Movedir = -1;
							stagekirby.Move = TRUE;
						}
						else if (selectedStage > nowStage) {	//1->2, 2->3
							stagekirby.moveIndex = 0;
							stagekirby.Movedir = 1;
							stagekirby.Move = TRUE;
						}
						else {
							stagekirby.aniIndex = 3;
							stagekirby.Move = FALSE;
							Loading = true;
						}
						SetTimer(hwnd, 10, 100, NULL);
					}
				}
			}
		}
		break;
	}

	case WM_TIMER:
	{
		//게임씬
		if (scene == 1) {
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
							//게임종료
							if (score >= Goalscore) {
								pause = true;
								end = true;
								child_hWnd = CreateWindow("ChildClass", NULL, WS_CHILD | WS_VISIBLE,
									40, 240, 400, 233, hwnd, NULL, g_hInst, NULL);

								chexit = CreateWindow("button", "Exit", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
									20, 140, 150, 57, child_hWnd, (HMENU)IDC_EXIT, g_hInst, NULL);
								hbit = (HBITMAP)LoadImage(g_hInst, "exit.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
								SendMessage(chexit, BM_SETIMAGE, 0, (LPARAM)hbit);

								retry = CreateWindow("button", "Retry", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
									200, 140, 150, 57, child_hWnd, (HMENU)IDC_RETRY, g_hInst, NULL);
								hbit = (HBITMAP)LoadImage(g_hInst, "retry.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
								SendMessage(retry, BM_SETIMAGE, 0, (LPARAM)hbit);
							}
							else if (Turn == 0) {
								pause = true;
								end = true;
								child_hWnd = CreateWindow("ChildClass", NULL, WS_CHILD | WS_VISIBLE,
									40, 240, 400, 233, hwnd, NULL, g_hInst, NULL);

								chexit = CreateWindow("button", "Exit", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
									20, 140, 150, 57, child_hWnd, (HMENU)IDC_EXIT, g_hInst, NULL);
								hbit = (HBITMAP)LoadImage(g_hInst, "exit.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
								SendMessage(chexit, BM_SETIMAGE, 0, (LPARAM)hbit);

								retry = CreateWindow("button", "Retry", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
									200, 140, 150, 57, child_hWnd, (HMENU)IDC_RETRY, g_hInst, NULL);
								hbit = (HBITMAP)LoadImage(g_hInst, "retry.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
								SendMessage(retry, BM_SETIMAGE, 0, (LPARAM)hbit);
							}
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
					LBSel = true;
					break;
				case 7:
				{
					if (selectdir == Small) {
						block[sely][selx].blocksize -= 3;
						block[tmpy][tmpx].blocksize -= 3;
						if (block[sely][selx].blocksize == -36) {
							selectdir = Big;
							Object temp = block[tmpy][tmpx];
							POINT temppt = block[tmpy][tmpx].pos;
							block[tmpy][tmpx] = block[sely][selx];
							block[tmpy][tmpx].pos = temppt;
							temppt = block[sely][selx].pos;
							block[sely][selx] = temp;
							block[sely][selx].pos = temppt;
						}
					}
					else {
						block[sely][selx].blocksize += 3;
						block[tmpy][tmpx].blocksize += 3;
						if (block[sely][selx].blocksize == 0) {
							selectdir = Small;
							KillTimer(hwnd, 7);
							if (Check3()) {
								SetTimer(hwnd, 1, 100, NULL);
							}
							else {
								SetTimer(hwnd, 1, 500, NULL);
							}
						}
					}
					break;
				}
				case 8:
				{
					if (Loading) {
						if (loadingCnt == 0) {	//페이드아웃
							if (alpha < 239) alpha += 16;
							else {
								alpha = 255;
								loadingCnt++;
							}
						}
						else {					//페이드인
							if (alpha > 16) alpha -= 16;
							else {
								alpha = 0;
								loadingCnt++;
								Loading = false;
								KillTimer(hwnd, 8);
							}
						}
					}
					break;
				}
				}
			}
			InvalidateRect(hwnd, NULL, FALSE);
		}
		//스테이지씬
		else {
			hdc = GetDC(hwnd);
			if (hBitmap == NULL) hBitmap = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
			memdc1 = CreateCompatibleDC(hdc);
			memdc2 = CreateCompatibleDC(memdc1);
			oldBitmap1 = (HBITMAP)SelectObject(memdc1, hBitmap);

			//StretchBlt(출력 DC, 그릴 위치.x, 그릴 위치.y, 그릴 크기.x, 그릴 크기.y, 소스 DC, 메모리DC 위치.x, 메모리DC 위치.y, 가져올 크기.x, 가져올 크기.y, 래스터 연산 방법)
			//TransparentBlt(출력 DC, 그릴 위치.x, 그릴 위치.y, 그릴 크기.x, 그릴 크기.y, 소스 DC, 메모리DC 위치.x, 메모리DC 위치.y, 가져올 크기.x, 가져올 크기.y, 투명하게 할 색상)

			switch (wParam) {
			case 9:	//그리기
			{
				//스테이지 배경 그리기
				oldBitmap2 = (HBITMAP)SelectObject(memdc2, stageBackground.hBitmap);
				StretchBlt(memdc1, 0, 0, SCREENWIDTH, SCREENHEIGHT, memdc2, cx, cy, SCREENWIDTH, SCREENHEIGHT, SRCCOPY);

				//스테이지 버튼 그리기
				for (i = 0; i < STAGECNT; ++i) {
					oldBitmap2 = (HBITMAP)SelectObject(memdc2, stageButton[i].hBitmap);
					TransparentBlt(memdc1, stageButton[i].drawPos.x - cx, stageButton[i].drawPos.y - cy, stageButton[i].bitmapSize.x, stageButton[i].bitmapSize.y, memdc2, 0, 0, stageButton[i].bitmapSize.x, stageButton[i].bitmapSize.y, buttonRGB);
				}

				//커비 그리기
				if (stagekirby.Movedir == 1) stagekirby.hBitmap = (HBITMAP)LoadImage(g_hInst, "rightwalk.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				else stagekirby.hBitmap = (HBITMAP)LoadImage(g_hInst, "leftwalk.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				oldBitmap2 = (HBITMAP)SelectObject(memdc2, stagekirby.hBitmap);
				stagekirby.drawPos = { stagekirby.centerPos.x - stagekirby.bitmapSize.y / 2, stagekirby.centerPos.y - stagekirby.bitmapSize.y / 2 };
				TransparentBlt(memdc1, stagekirby.drawPos.x - cx, stagekirby.drawPos.y - cy, stagekirby.bitmapSize.y, stagekirby.bitmapSize.y, memdc2, stagekirby.aniIndex * stagekirby.bitmapSize.y, 0, stagekirby.bitmapSize.y, stagekirby.bitmapSize.y, kirbyRGB);

				//페이드인아웃
				bf.BlendOp = 0;
				bf.BlendFlags = 0;
				bf.SourceConstantAlpha = alpha;
				bf.AlphaFormat = 0;

				oldBitmap2 = (HBITMAP)SelectObject(memdc2, fade.hBitmap);
				GdiAlphaBlend(memdc1, 0, 0, fade.bitmapSize.x, fade.bitmapSize.y, memdc2, 0, 0, fade.bitmapSize.x, fade.bitmapSize.y, bf);
			}
			break;

			case 10:	//커비 이동
			{
				if (!Loading) {
					if (stagekirby.Movedir == 1) {	//우로 이동
						if (stagekirby.moveIndex < POINTCNT - 1) {
							if (selectedStage == 1) stagekirby.centerPos = { stage12[stagekirby.moveIndex].x, stage12[stagekirby.moveIndex].y - stagekirby.bitmapSize.y / 2 };
							else stagekirby.centerPos = { stage23[stagekirby.moveIndex].x, stage23[stagekirby.moveIndex].y - stagekirby.bitmapSize.y / 2 };
							if (cx < stageBackground.bitmapSize.x - SCREENWIDTH) cx += stagekirby.Movedir * 8;
							stagekirby.moveIndex++;
						}
						else {
							if (selectedStage == 1) nowStage = 1;
							else nowStage = 2;
							stagekirby.Move = FALSE;
							stagekirby.aniIndex = 3;
							Loading = true;
						}
					}
					else if (stagekirby.Movedir == -1) {	//좌로 이동
						if (stagekirby.moveIndex > 0) {
							if (selectedStage == 0) stagekirby.centerPos = { stage12[stagekirby.moveIndex].x, stage12[stagekirby.moveIndex].y - stagekirby.bitmapSize.y / 2 };
							else stagekirby.centerPos = { stage23[stagekirby.moveIndex].x, stage23[stagekirby.moveIndex].y - stagekirby.bitmapSize.y / 2 };
							if (cx > 0) cx += stagekirby.Movedir * 8;
							stagekirby.moveIndex--;
						}
						else {
							if (selectedStage == 0) nowStage = 0;
							else nowStage = 1;
							stagekirby.Move = FALSE;
							stagekirby.aniIndex = 3;
							Loading = true;
						}
					}
					if (stagekirby.Move) {
						if (stagekirby.aniIndex < 11) stagekirby.aniIndex++;
						if (stagekirby.aniIndex == 11) stagekirby.aniIndex = 0;
					}
				}
			}
			break;

			case 11:	//스테이지 진입 알파값 조정
			{
				if (Loading) {
					if (loadingCnt == 0) {	//페이드아웃
						if (alpha < 239) alpha += 16;
						else {
							alpha = 255;
							loadingCnt++;
							scene = 1;
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
								Turn = 15;
								bombnum = 3;
								switchnum = 2;
								turnnum = 1;
								returnnum = 1;
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
								bombnum = 2;
								switchnum = 3;
								turnnum = 1;
								returnnum = 1;
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
								Goalscore = 8000;
								Turn = 15;
								bombnum = 1;
								switchnum = 3;
								turnnum = 0;
								returnnum = 1;
							}
							break;
							}
							pause = false;
							SetTimer(hwnd, 8, 100, NULL);
							FMOD_Channel_Stop(Channel[0]);
							FMOD_System_PlaySound(System, bgmSound[1], NULL, 0, &Channel[0]);
							FMOD_Channel_SetVolume(Channel[0], 0.1);
							while (CheckStart());
							KillTimer(hwnd, 9);
							KillTimer(hwnd, 10);
							KillTimer(hwnd, 11);
						}
					}
					else { //페이드인
						if (alpha > 16) alpha -= 16;
						else {
							alpha = 0;
							loadingCnt++;
							KillTimer(hwnd, 11);
						}
					}
				}
			}
			break;

			case 12:	//title
			{
				if (loadingCnt == 0) {	//페이드아웃
					if (alpha < 247) alpha += 16;
					else {
						KillTimer(hwnd, 9);
						KillTimer(hwnd, 10);
						KillTimer(hwnd, 11);
						alpha = 255;
						loadingCnt++;
					}
				}
				else {					//페이드인
					if (alpha > 8) alpha -= 64;
					else {
						alpha = 0;
						loadingCnt++;
						SetTimer(hwnd, 9, 100, NULL);
						KillTimer(hwnd, 12);
					}
				}

				bf.BlendOp = 0;
				bf.BlendFlags = 0;
				bf.SourceConstantAlpha = alpha;
				bf.AlphaFormat = 0;

				oldBitmap2 = (HBITMAP)SelectObject(memdc2, title.hBitmap);
				GdiAlphaBlend(memdc1, 0, 0, title.bitmapSize.x, title.bitmapSize.y, memdc2, 0, 0, title.bitmapSize.x, title.bitmapSize.y, bf);
			}
			break;
			}

			SelectObject(memdc2, oldBitmap2);
			DeleteDC(memdc2);
			SelectObject(memdc1, oldBitmap1);
			DeleteDC(memdc1);
			ReleaseDC(hwnd, hdc);
			InvalidateRgn(hwnd, NULL, false);
		}
		break;
	}
	case WM_PAINT:
	{
		//게임씬
		if (scene == 1) {
			if (exittost) {
				KillTimer(hwnd, 1);
				KillTimer(hwnd, 2);
				KillTimer(hwnd, 3);
				KillTimer(hwnd, 4);
				KillTimer(hwnd, 5);
				KillTimer(hwnd, 6);
				KillTimer(hwnd, 7);
				SetTimer(hwnd, 9, 100, NULL);
				FMOD_Channel_Stop(Channel[0]);
				FMOD_System_PlaySound(System, bgmSound[0], NULL, 0, &Channel[0]);
				FMOD_Channel_SetVolume(Channel[0], 0.1);
				exittost = false;
				scene = 0;
			}
			ghdc = BeginPaint(hwnd, &gps);
			//더블버퍼링 
			{
				MemDC = CreateCompatibleDC(ghdc);
				BackBit = CreateCompatibleBitmap(ghdc, WindowRect.right, WindowRect.bottom);
				oldBackBit = (HBITMAP)SelectObject(MemDC, BackBit);
				PatBlt(MemDC, 0, 0, WindowRect.right, WindowRect.bottom, WHITENESS);
				tmpDC = ghdc;
				ghdc = MemDC;
				MemDC = tmpDC;
			}

			PrintBackGround(ghdc);
			PrintBoard(ghdc);
			PrintSkill(ghdc);
			PrintBlock(ghdc);
			PrintScore(ghdc);
			PrintTurn(ghdc);
			PrintKirby(ghdc);
			if (IsSel) {
				PrintRect(ghdc);
			}
			if (skill != -1) {
				PrintSkillRect(ghdc);
			}
			PrintFade(ghdc);

			//더블버퍼링
			{
				tmpDC = ghdc;
				ghdc = MemDC;
				MemDC = tmpDC;
				BitBlt(ghdc, 0, 0, WindowRect.right, WindowRect.bottom, MemDC, 0, 0, SRCCOPY);
				SelectObject(MemDC, oldBackBit);
				DeleteObject(BackBit);
				DeleteDC(MemDC);
			}
			EndPaint(hwnd, &gps);
		}
		//스테이지씬
		else {
			GetClientRect(hwnd, &crt);
			hdc = BeginPaint(hwnd, &ps);
			memdc1 = CreateCompatibleDC(hdc);
			oldBitmap1 = (HBITMAP)SelectObject(memdc1, hBitmap);
			BitBlt(hdc, 0, 0, SCREENWIDTH, SCREENHEIGHT, memdc1, 0, 0, SRCCOPY);
			SelectObject(memdc1, oldBitmap1);
			DeleteDC(memdc1);
			EndPaint(hwnd, &ps);
		}
		break;
	}

	case WM_CHAR:
		//스테이지씬
		if (scene == 0) {
			if (!stagekirby.Move) {
				switch (wParam)
				{
					//카메라 좌우 스크롤
				case 'a':
					if (cx > 0) cx -= 8;
					break;

				case 'd':
					if (cx < stageBackground.bitmapSize.x - SCREENWIDTH) cx += 8;
					break;
				}
			}
		}
		break;

	case WM_DESTROY:
	{
		for (int i = 0; i < SOUND_EFFECT; ++i) {
			FMOD_Sound_Release(bgmSound[i]);
		}
		for (int i = 0; i < EFFECT_COUNT; ++i) {
			FMOD_Sound_Release(effectSound[i]);
		}
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	static Bit chbackground, chend;
	BITMAP bmp;
	HDC memdc, ghdc;
	HBITMAP oldbit;
	TCHAR str[6];
	switch (uMsg)
	{
	case WM_CREATE:
	{
		chbackground.bit = (HBITMAP)LoadImage(g_hInst, "pause.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(chbackground.bit, sizeof(BITMAP), &bmp);
		chbackground.bitmapSize.x = bmp.bmWidth;
		chbackground.bitmapSize.y = bmp.bmHeight;

		chend.bit = (HBITMAP)LoadImage(g_hInst, "gameover.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(chend.bit, sizeof(BITMAP), &bmp);
		chend.bitmapSize.x = bmp.bmWidth;
		chend.bitmapSize.y = bmp.bmHeight;
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_PLAY:
			pause = false;
			DestroyWindow(hWnd);
			break;
		case IDC_EXIT:	//스테이지씬으로 이동
			exittost = true;
			alpha = 0;
			DestroyWindow(hWnd);
			break;
		case IDC_RETRY:
			Reset();
			while (CheckStart());
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		ghdc = BeginPaint(hWnd, &ps);

		if (pause && !end) {
			memdc = CreateCompatibleDC(ghdc);
			oldbit = (HBITMAP)SelectObject(memdc, chbackground.bit);
			TransparentBlt(ghdc, 0, 0, chbackground.bitmapSize.x, chbackground.bitmapSize.y, memdc, 0, 0, chbackground.bitmapSize.x, chbackground.bitmapSize.y, RGB(201, 206, 181));
			SelectObject(memdc, oldbit);
			DeleteObject(memdc);
			DeleteObject(oldbit);
		}
		else if (end) {
			memdc = CreateCompatibleDC(ghdc);
			oldbit = (HBITMAP)SelectObject(memdc, chend.bit);
			TransparentBlt(ghdc, 0, 0, chend.bitmapSize.x, chend.bitmapSize.y, memdc, 0, 0, chend.bitmapSize.x, chend.bitmapSize.y, RGB(201, 206, 181));
			SelectObject(memdc, oldbit);
			DeleteObject(memdc);
			DeleteObject(oldbit);
			SetBkMode(ghdc, TRANSPARENT);
			wsprintf(str, "%d", score);
			TextOut(ghdc, 150, 72, str, strlen(str));
		}
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
	switch (stage) {
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

void PrintBackGround(HDC ghdc) {
	HDC memdc;
	HBITMAP Oldbit;
	memdc = CreateCompatibleDC(ghdc);
	Oldbit = (HBITMAP)SelectObject(memdc, Background.bit);
	BitBlt(ghdc, 0, 0, Background.bitmapSize.x, Background.bitmapSize.y, memdc, 0, 0, SRCCOPY);
	SelectObject(memdc, Oldbit);
	DeleteObject(memdc);
	DeleteObject(Oldbit);
}

void PrintBoard(HDC ghdc) {
	HBRUSH hbrush, oldbrush;
	HPEN hpen, oldpen;

	hpen = CreatePen(PS_SOLID, 3, RGB(150, 150, 150));
	oldpen = (HPEN)SelectObject(ghdc, hpen);
	Rectangle(ghdc, 16, 161, 16 + BLOCKROW * 45, 161 + BLOCKCOL * 45);
	SelectObject(ghdc, oldpen);
	DeleteObject(hpen);

	for (int i = 0; i < BLOCKCOL; ++i) {
		for (int j = 0; j < BLOCKROW; ++j) {
			hbrush = CreateSolidBrush(RGB(255, 255, 255));
			hpen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
			oldpen = (HPEN)SelectObject(ghdc, hpen);
			oldbrush = (HBRUSH)SelectObject(ghdc, hbrush);
			Rectangle(ghdc, 16 + j * 45, 161 + i * 45, 16 + (j + 1) * 45, 161 + (i + 1) * 45);
			SelectObject(ghdc, oldbrush);
			DeleteObject(hbrush);
			SelectObject(ghdc, oldpen);
			DeleteObject(hpen);
		}
	}

}

void PrintSkill(HDC ghdc) {
	HDC memdc;
	HBITMAP Oldbit;
	TCHAR str[3];
	for (int i = 0; i < 4; ++i) {
		memdc = CreateCompatibleDC(ghdc);
		Oldbit = (HBITMAP)SelectObject(memdc, Skill[i].bit);
		TransparentBlt(ghdc, 16 + SKILLDIS * i, 710, Skill[i].bitmapSize.x, Skill[i].bitmapSize.y, memdc, 0, 0, Skill[i].bitmapSize.x, Skill[i].bitmapSize.y, RGB(201, 206, 181));
		SelectObject(memdc, Oldbit);
		DeleteObject(memdc);
		DeleteObject(Oldbit);
		SetBkMode(ghdc, TRANSPARENT);
		SetTextColor(ghdc, RGB(0, 170, 20));
		wsprintf(str, "%d", bombnum);
		TextOut(ghdc, Skill[0].bitmapSize.x + SKILLDIS * 0 - 5, 765, str, strlen(str));

		wsprintf(str, "%d", switchnum);
		TextOut(ghdc, Skill[1].bitmapSize.x + SKILLDIS * 1 - 5, 765, str, strlen(str));

		wsprintf(str, "%d", turnnum);
		TextOut(ghdc, Skill[2].bitmapSize.x + SKILLDIS * 2 - 5, 765, str, strlen(str));

		wsprintf(str, "%d", returnnum);
		TextOut(ghdc, Skill[3].bitmapSize.x + SKILLDIS * 3 - 7, 765, str, strlen(str));
	}
}

void PrintBlock(HDC ghdc) {
	HDC memdc;
	HBITMAP Oldbit;

	for (int i = 0; i < BLOCKCOL; ++i) {
		for (int j = 0; j < BLOCKROW; ++j) {
			switch (block[i][j].color) {
			case Pink: //Pink
				memdc = CreateCompatibleDC(ghdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[0].bit);
				TransparentBlt(ghdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE + block[i][j].blocksize, BLOCKSIZE + block[i][j].blocksize, memdc, (Block[0].bitmapSize.x / 4 * block[i][j].ani), 0, Block[0].bitmapSize.x / 4, Block[0].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case Blue: //Blue
				memdc = CreateCompatibleDC(ghdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[1].bit);
				TransparentBlt(ghdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE + block[i][j].blocksize, BLOCKSIZE + block[i][j].blocksize, memdc, (Block[1].bitmapSize.x / 4 * block[i][j].ani), 0, Block[1].bitmapSize.x / 4, Block[1].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case Green: //Green
				memdc = CreateCompatibleDC(ghdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[2].bit);
				TransparentBlt(ghdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE + block[i][j].blocksize, BLOCKSIZE + block[i][j].blocksize, memdc, (Block[2].bitmapSize.x / 4 * block[i][j].ani), 0, Block[2].bitmapSize.x / 4, Block[2].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case Gray: //Gray
				memdc = CreateCompatibleDC(ghdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[3].bit);
				TransparentBlt(ghdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE + block[i][j].blocksize, BLOCKSIZE + block[i][j].blocksize, memdc, (Block[3].bitmapSize.x / 4 * block[i][j].ani), 0, Block[3].bitmapSize.x / 4, Block[3].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case Purple: //Purple
				memdc = CreateCompatibleDC(ghdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[4].bit);
				TransparentBlt(ghdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE + block[i][j].blocksize, BLOCKSIZE + block[i][j].blocksize, memdc, (Block[4].bitmapSize.x / 4 * block[i][j].ani), 0, Block[4].bitmapSize.x / 4, Block[4].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case Orange: //Orange
				memdc = CreateCompatibleDC(ghdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[5].bit);
				TransparentBlt(ghdc, block[i][j].pos.x, block[i][j].pos.y, BLOCKSIZE + block[i][j].blocksize, BLOCKSIZE + block[i][j].blocksize, memdc, (Block[5].bitmapSize.x / 4 * block[i][j].ani), 0, Block[5].bitmapSize.x / 4, Block[5].bitmapSize.y, RGB(201, 206, 181));
				SelectObject(memdc, Oldbit);
				DeleteObject(memdc);
				DeleteObject(Oldbit);
				break;
			case 6: //Destroy
				memdc = CreateCompatibleDC(ghdc);
				Oldbit = (HBITMAP)SelectObject(memdc, Block[6].bit);
				TransparentBlt(ghdc, block[i][j].pos.x - 5, block[i][j].pos.y - 4, Block[6].bitmapSize.x / 8, Block[6].bitmapSize.y, memdc, (Block[6].bitmapSize.x / 8 * block[i][j].ani), 0, Block[6].bitmapSize.x / 8, Block[6].bitmapSize.y, RGB(201, 206, 181));
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

void PrintScore(HDC ghdc) {
	TCHAR str[6];
	HFONT hfont, oldfont;
	hfont = CreateFontA(20, 0, 0, 0, 1000, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, "배달의민족주아");
	oldfont = (HFONT)SelectObject(ghdc, hfont);
	SetTextColor(ghdc, RGB(0, 0, 0));
	wsprintf(str, "%d", score);
	TextOut(ghdc, 230, 50, str, strlen(str));
	SelectObject(ghdc, oldfont);
	DeleteObject(hfont);

	hfont = CreateFontA(20, 0, 0, 0, 1000, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, "배달의민족주아");
	oldfont = (HFONT)SelectObject(ghdc, hfont);
	wsprintf(str, "%d", Goalscore);
	TextOut(ghdc, 380, 50, str, strlen(str));
	SelectObject(ghdc, oldfont);
	DeleteObject(hfont);
}

void PrintTurn(HDC ghdc) {
	TCHAR str[3];
	HFONT hfont, oldfont;
	SetBkMode(ghdc, TRANSPARENT);
	wsprintf(str, "%d", Turn);
	TextOut(ghdc, 75, 50, str, strlen(str));
}

void PrintKirby(HDC ghdc) {
	HDC memdc;
	HBITMAP Oldbit;
	memdc = CreateCompatibleDC(ghdc);
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
	TransparentBlt(ghdc, kirby.Pos.x, kirby.Pos.y, KIRBYSIZE, KIRBYSIZE, memdc, kirby.aniIndex * kirby.bitmapSize.y, 0, kirby.bitmapSize.y, kirby.bitmapSize.y, kirbyRGB);
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

void PrintRect(HDC ghdc) {
	RECT rect = { block[sely][selx].pos.x - 5, block[sely][selx].pos.y - 5, block[sely][selx].pos.x + BLOCKSIZE + 5, block[sely][selx].pos.y + BLOCKSIZE + 5 };
	HPEN hpen, oldpen;
	hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	oldpen = (HPEN)SelectObject(ghdc, hpen);
	SelectObject(ghdc, GetStockObject(NULL_BRUSH));
	Rectangle(ghdc, block[sely][selx].pos.x - 5, block[sely][selx].pos.y - 5, block[sely][selx].pos.x + BLOCKSIZE + 5, block[sely][selx].pos.y + BLOCKSIZE + 5);
	SelectObject(ghdc, oldpen);
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

void PrintSkillRect(HDC ghdc) {
	HPEN hpen, oldpen;
	hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	oldpen = (HPEN)SelectObject(ghdc, hpen);
	SelectObject(ghdc, GetStockObject(NULL_BRUSH));
	Rectangle(ghdc, 16 + SKILLDIS * skill, 710, 16 + SKILLDIS * skill + Skill[skill].bitmapSize.x, 710 + Skill[skill].bitmapSize.y);
	SelectObject(ghdc, oldpen);
	DeleteObject(hpen);
}

void SkillBomb(int mx, int my) {
	for (int i = 0; i < BLOCKCOL; ++i) {
		for (int j = 0; j < BLOCKROW; ++j) {
			returnblock[i][j] = block[i][j];
		}
	}
	returnscore = score;
	for (int i = 0; i < BLOCKCOL; ++i) {
		for (int j = 0; j < BLOCKROW; ++j) {
			if (block[i][j].pos.x <= mx && block[i][j].pos.x + BLOCKSIZE >= mx && block[i][j].pos.y <= my && block[i][j].pos.y + BLOCKSIZE >= my) {
				if (i == 0 && j == 0) {
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
				skill = -1;
				Turn--;
				bombnum--;
				FMOD_System_PlaySound(System, effectSound[0], NULL, 0, &Channel[1]);
				FMOD_Channel_SetVolume(Channel[1], 0.1);
				break;
			}
		}
	}

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
					for (int k = 0; k < BLOCKCOL; ++k) {
						for (int l = 0; l < BLOCKROW; ++l) {
							returnblock[k][l] = block[k][l];
						}
					}
					returnscore = score;
					tmpx = selx;
					tmpy = sely;
					selx = j;
					sely = i;
					IsSel = false;
					skill = -1;
					switchnum--;
					Turn--;
					FMOD_System_PlaySound(System, effectSound[1], NULL, 0, &Channel[1]);
				}
				break;
			}
		}
	}
}

void SkillTurn() {
	switch (stage) {
	case 1:
		if (Turn <= 15) {
			Turn += 5;
			turnnum--;
			FMOD_System_PlaySound(System, effectSound[2], NULL, 0, &Channel[1]);
		}
		break;
	case 2:
		if (Turn <= 13) {
			Turn += 5;
			turnnum--;
			FMOD_System_PlaySound(System, effectSound[2], NULL, 0, &Channel[1]);
		}
		break;
	case 3:
		if (Turn <= 10) {
			Turn += 5;
			turnnum--;
			FMOD_System_PlaySound(System, effectSound[2], NULL, 0, &Channel[1]);
		}
		break;
	}
	skill = -1;
}

void SkillReturn() {
	switch (stage) {
	case 1:
		if (Turn < 15 || turnnum != 1) {
			for (int i = 0; i < BLOCKCOL; ++i) {
				for (int j = 0; j < BLOCKROW; ++j) {
					block[i][j] = returnblock[i][j];
				}
			}
			score = returnscore;
			Turn--;
			returnnum--;
			FMOD_System_PlaySound(System, effectSound[3], NULL, 0, &Channel[1]);
			FMOD_Channel_SetVolume(Channel[1], 0.3);

		}
		break;
	case 2:
		if (Turn < 18 || turnnum != 1) {
			for (int i = 0; i < BLOCKCOL; ++i) {
				for (int j = 0; j < BLOCKROW; ++j) {
					block[i][j] = returnblock[i][j];
				}
			}
			score = returnscore;
			Turn--;
			returnnum--;
			FMOD_System_PlaySound(System, effectSound[3], NULL, 0, &Channel[1]);
			FMOD_Channel_SetVolume(Channel[1], 0.3);

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
			returnnum--;
			FMOD_System_PlaySound(System, effectSound[3], NULL, 0, &Channel[1]);
			FMOD_Channel_SetVolume(Channel[1], 0.1);
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
					block[j][i].ani = 0;
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

void BlockDown() {
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
		Turn = 15;
		bombnum = 3;
		switchnum = 2;
		turnnum = 1;
		returnnum = 1;
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
		bombnum = 2;
		switchnum = 3;
		turnnum = 1;
		returnnum = 1;
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
		Goalscore = 8000;
		Turn = 15;
		bombnum = 1;
		switchnum = 3;
		turnnum = 0;
		returnnum = 1;
	}
	break;
	}
}

void PrintFade(HDC ghdc) {
	HDC memdc;
	HBITMAP Oldbit;
	memdc = CreateCompatibleDC(ghdc);
	
	//페이드인아웃
	bf.BlendOp = 0;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = alpha;
	bf.AlphaFormat = 0;

	Oldbit = (HBITMAP)SelectObject(memdc, fade.hBitmap);
	GdiAlphaBlend(ghdc, 0, 0, fade.bitmapSize.x, fade.bitmapSize.y, memdc, 0, 0, fade.bitmapSize.x, fade.bitmapSize.y, bf);

	SelectObject(memdc, Oldbit);
	DeleteObject(memdc);
	DeleteObject(Oldbit);
}

BOOL InCircle(POINT a, POINT b) {
	float dis;		//두 점 간의 길이
	dis = sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
	if (dis < BUTTONRADIUS) return TRUE;
	else return FALSE;
}

void Sound_Setup()
{
	FMOD_System_Create(&System);
	FMOD_System_Init(System, 10, FMOD_INIT_NORMAL, NULL);

	FMOD_System_CreateSound(System, "stageBGM.mp3", FMOD_LOOP_NORMAL, 0, &bgmSound[0]);
	FMOD_System_CreateSound(System, "BGM.mp3", FMOD_LOOP_NORMAL, 0, &bgmSound[1]);

	FMOD_System_CreateSound(System, "bombsound.mp3", FMOD_DEFAULT, 0, &effectSound[0]);
	FMOD_System_CreateSound(System, "switchsound.mp3", FMOD_DEFAULT, 0, &effectSound[1]);
	FMOD_System_CreateSound(System, "turnsound.mp3", FMOD_DEFAULT, 0, &effectSound[2]);
	FMOD_System_CreateSound(System, "returnsound.mp3", FMOD_DEFAULT, 0, &effectSound[3]);
}
