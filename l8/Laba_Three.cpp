#include "stdafx.h"
#include "Laba_Three.h"
#include "windows.h"
#include <vector>
#include <algorithm>

#define MAX_LOADSTRING 200
#define ID_DRAW 100
#define ID_CLEAR 101

using namespace std;

//����������� - ��������� �� �������� �������� ������������ �������(�������� ��� ������ ���-�������)
//_T() - �������� ������ Unicode �� ����� ����������


// ���������� ����������:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����

// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM                MyRegisterClass(HINSTANCE hInstance);	//������� ����������� ����(����� int)
BOOL                InitInstance(HINSTANCE, int);			//������������� ����
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);	//�������� ���� (lresult - �������� ������� ��������� long)
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);		//��� ����� ��������� � int, � ���������

//�������� �������
int APIENTRY wWinMain(HINSTANCE hInstance,				//������� ����������
					  HINSTANCE hPrevInstance,			//���������� ���������� (� ����������� ������ ����)
                      LPWSTR    lpCmdLine,				//��������� ������
                      int       mode)					//����� �����������
{
    UNREFERENCED_PARAMETER(hPrevInstance);				//���� ���������� �� �������, ��� ���������� �� ������������
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���������� ��� �����.

    // ������������� ���������� �����
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LABA_ONE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ��������� ������������� ����������:
    if (!InitInstance (hInstance, mode))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABA_ONE));		//�������� ������� ������� ������
	//��������� ��� �������� ���������
    MSG msg;				

    // ���� ��������� ���������(��������� �� ���������, ����(���), min �������� ����������, max):
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))			//�� ������ �� ������� �������? ���� ��, �� ������� � WndProc
        {
            TranslateMessage(&msg);			//���������� ����� ������� �������
            DispatchMessage(&msg);			//�������� ��������� ������� WndProc
        }
    }
    return (int) msg.wParam;
}

//
//  �������: MyRegisterClass()
//
//  ����������: ������������ ����� ����.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;					//����� ����
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;	// ����� ����(�������������� ���������� ��� ��������� ������ � ������)
    wcex.lpfnWndProc    = WndProc;					//��� ������� ��� ��������� ���������
    wcex.cbClsExtra     = 0;						//��� ������ �����
    wcex.cbWndExtra     = 0;						//��� ������ ����
    wcex.hInstance      = hInstance;				//������� ����������
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABA_ONE));	//������
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);							//������
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);					//���������� ���� ������
    wcex.lpszMenuName   = NULL;								//����
    wcex.lpszClassName  = szWindowClass;										//��� ������ ����
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));	//���������� ��������� ������

    return RegisterClassExW(&wcex);
}

//
//   �������: InitInstance(HINSTANCE, int)
//
//   ����������: ��������� ��������� ���������� � ������� ������� ����.
//
//   �����������:
//        � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//        ��������� � ��������� �� ����� ������� ���� ���������.
//
BOOL InitInstance(HINSTANCE hInstance, int mode)
{
   hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_CAPTION|WS_MINIMIZEBOX|WS_OVERLAPPED|WS_SYSMENU,
      300, 120, 800, 500, nullptr, nullptr, hInstance, nullptr);

   
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, mode);
   UpdateWindow(hWnd);
   return TRUE;
}

//
//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)	
//
//  ����������:  ������������ ��������� � ������� ����.
//
//	WM_CREATE - �������� ����
//  WM_COMMAND � ���������� ���� ����������
//  WM_PAINT � ���������� ������� ����
//  WM_DESTROY � ��������� ��������� � ������ � ���������
//	WM_SIZE - ������������ ��� ��������� ������� ����
//	WM_DRAWITEM - ��������� ��� ������������� ����������� ��������
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hBtnDraw, hBtnClear;
	static bool picture = false;
	POINT fish1[3] = { {270, 370}, {350, 310}, {350, 410} };
	POINT fish1Tail[3] = { {350, 370}, {380, 330}, {380, 400} };
	POINT fish2[7] = { {530, 400}, {670, 300}, {640, 390}, {680, 330}, {680, 470}, {640, 420}, {670, 500} };
	POINT ship[4] = { {200, 300}, {550, 300}, {600, 200}, {150, 200 } };
	POINT flagPirat[4] = { { 405, 45 },{ 405, 15 },{ 455, 15 },{ 455, 45 } };
	POINT flag[3] = { { 400, 190 },{ 400, 15 },{ 280, 190 } };
	POINT flag3[3] = { { 330, 190 },{ 330, 50 },{ 180, 190 } };
	POINT flag2[3] = { { 400, 140 },{ 400, 50 },{ 525, 140 } };
	POINT flag4[3] = { { 400, 190 },{ 400, 90 },{ 550, 190 } };

	POINT seeweed1[7] = { {30, 450}, {45, 350}, {40, 100}, {45, 30}, {55, 130}, {50, 250}, {60, 450} };
	POINT seeweed2[7] = { { 70, 450 },{ 75, 300 },{ 53, 250 },{ 70, 200 },{ 65, 250 },{ 80, 350 },{ 90, 450 } };
	POINT seeweed3[7] = { { 80, 450 },{ 125, 400 },{ 115, 300 },{ 130, 250 },{ 155, 300 },{ 110, 350 },{ 160, 450 } };
	POINT seeweed4[7] = { { 600, 450 },{ 645, 300 },{ 623, 200 },{ 670, 150 },{ 645, 250 },{ 670, 400 },{ 660, 450 } };
	POINT seeweed5[7] = { { 650, 450 },{ 700, 430 },{ 685, 280 },{ 670, 200 },{ 705, 300 },{ 680, 350 },{ 720, 450 } };
	POINT seeweed6[7] = { { 700, 450 },{ 720, 430 },{ 695, 280 },{ 735, 270 },{ 725, 300 },{ 700, 350 },{ 750, 450 } };
	PAINTSTRUCT ps;
	HDC hdc;
	DRAWITEMSTRUCT* lpDrawItem;
	static HBRUSH sandBrush, pinkBrush, redBrush, yellowBrush, blackBrush, greenBrush, stoneBrush, purpleStoneBrush, backBrush, blueBrush, drawBrush, clearBrush; 
	switch (message)
	{
		case WM_CREATE:
			backBrush = CreateSolidBrush(RGB(170, 220, 255));
			blueBrush = CreateSolidBrush(RGB(50, 60, 200));
			purpleStoneBrush = CreateSolidBrush(RGB(93, 71, 139));
			sandBrush = CreateSolidBrush(RGB(184, 134, 11));
			pinkBrush = CreateSolidBrush(RGB(218, 112, 214));
			yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
			blackBrush = CreateSolidBrush(RGB(0, 0, 0));
			redBrush = CreateSolidBrush(RGB(210, 0, 0));
			greenBrush = CreateSolidBrush(RGB(0, 139, 0));
			stoneBrush = CreateSolidBrush(RGB(139, 126, 102));
			hBtnDraw = CreateWindowW(L"Button", L"��������", WS_CHILD | BS_OWNERDRAW | WS_VISIBLE, 710, 20, 60, 60, hWnd, (HMENU)ID_DRAW, NULL, NULL);
			hBtnClear = CreateWindowA("Button", "��������", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 710, 90, 60, 60, hWnd, (HMENU)ID_CLEAR, NULL, NULL);
			break;

		case WM_PAINT:
			if (picture) {
				hdc = BeginPaint(hWnd, &ps);
				SelectObject(hdc, backBrush);
				Rectangle(hdc, 0, 0, 800, 500);
				
				SelectObject(hdc, blueBrush);
				Ellipse(hdc, -100, 220, 1000, 600);
				
				BeginPath(hdc);
				Polyline(hdc, fish1, 3);
				Polyline(hdc, fish1Tail, 3);
				CloseFigure(hdc);
				EndPath(hdc);
				SelectObject(hdc, pinkBrush);
				FillPath(hdc);
				 
				BeginPath(hdc);
				Polyline(hdc, fish2, 7);
				CloseFigure(hdc);
				EndPath(hdc);
				SelectObject(hdc, yellowBrush);
				FillPath(hdc);
				 
				SelectObject(hdc, blackBrush);
				Ellipse(hdc, 300, 355, 315, 370);
				Ellipse(hdc, 555, 390, 570, 410);
					
				BeginPath(hdc);
				Polyline(hdc, ship, 4);
				CloseFigure(hdc);
				EndPath(hdc);
				SelectObject(hdc, stoneBrush);
				FillPath(hdc);
				
				BeginPath(hdc);
				Polyline(hdc, flag, 3);
				CloseFigure(hdc);
				EndPath(hdc);
				SelectObject(hdc, redBrush);
				FillPath(hdc);

				BeginPath(hdc);
				Polyline(hdc, flag2, 3);
				CloseFigure(hdc);
				EndPath(hdc);
				SelectObject(hdc, redBrush);
				FillPath(hdc);

				BeginPath(hdc);
				Polyline(hdc, flag3, 3);
				CloseFigure(hdc);
				EndPath(hdc);
				SelectObject(hdc, redBrush);
				FillPath(hdc);

				BeginPath(hdc);
				Polyline(hdc, flag4, 3);
				CloseFigure(hdc);
				EndPath(hdc);
				SelectObject(hdc, redBrush);
				FillPath(hdc);

				BeginPath(hdc);
				Polyline(hdc, flagPirat, 4);
				CloseFigure(hdc);
				EndPath(hdc);
				SelectObject(hdc, blackBrush);
				FillPath(hdc);

				SelectObject(hdc, blackBrush);
				Ellipse(hdc, 200, 230, 220, 250);
				Ellipse(hdc, 240, 230, 260, 250);
				Ellipse(hdc, 280, 230, 300, 250);
				Ellipse(hdc, 320, 230, 340, 250);
				Ellipse(hdc, 360, 230, 380, 250);
				Ellipse(hdc, 400, 230, 420, 250);
				Ellipse(hdc, 440, 230, 460, 250);
				Ellipse(hdc, 480, 230, 500, 250);
				Ellipse(hdc, 520, 230, 540, 250);

				EndPaint(hWnd, &ps);

			}
			else {
				hdc = BeginPaint(hWnd, &ps);
				SelectObject(hdc, (HBRUSH)(COLOR_WINDOW + 1));
				Rectangle(hdc, 0, 0, 800, 500);
				EndPaint(hWnd, &ps);
			}
			break;

		case WM_DRAWITEM:
			lpDrawItem = (DRAWITEMSTRUCT*)lParam;
			hdc = (*lpDrawItem).hDC;
			drawBrush = CreateSolidBrush(RGB(255, 130, 171));
			clearBrush = CreateSolidBrush(RGB(178, 223, 238));
			if (LOWORD(wParam) == ID_DRAW) {
				SelectObject(hdc, drawBrush);
				Rectangle(hdc, 0, 0, 60, 60);
				SelectObject(hdc, clearBrush);
				Ellipse(hdc, 20, 20, 40, 40);
			}
			else if (LOWORD(wParam) == ID_CLEAR){
				POINT points[3] = { {20, 40}, {30, 30}, {40, 40} };
				SelectObject(hdc, clearBrush);
				Rectangle(hdc, 0, 0, 60, 60);
				SelectObject(hdc, drawBrush);
				Ellipse(hdc, 20, 20, 40, 40);
			}
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case ID_DRAW:
					picture = true;
					InvalidateRect(hWnd, NULL, false);
					break;
				case ID_CLEAR:
					picture = false;
					InvalidateRect(hWnd, NULL, false);
					break;
			}
			break;
		case WM_DESTROY:
			DeleteObject(backBrush);
			DeleteObject(pinkBrush);
			DeleteObject(yellowBrush);
			DeleteObject(sandBrush);
			DeleteObject(greenBrush);
			DeleteObject(blackBrush);
			DeleteObject(stoneBrush);
			DeleteObject(purpleStoneBrush);
			DeleteObject(drawBrush);
			DeleteObject(clearBrush);
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
		return 0;
}

