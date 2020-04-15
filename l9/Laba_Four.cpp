#include "stdafx.h"
#include "Laba_Four.h"
#include <math.h>

//����������� - ��������� �� �������� �������� ������������ �������(�������� ��� ������ ���-�������)
//_T() - �������� ������ Unicode �� ����� ����������

#define MAX_LOADSTRING 100
const int SPAN = 10;

// ���������� ����������:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����

												// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM                MyRegisterClass(HINSTANCE hInstance);	//������� ����������� ����(����� int)
BOOL                InitInstance(HINSTANCE, int);			//������������� ����
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);	//�������� ���� (lresult - �������� ������� ��������� long)

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
	if (!InitInstance(hInstance, mode))
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
	return (int)msg.wParam;
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
	wcex.style = CS_HREDRAW | CS_VREDRAW;	// ����� ����(�������������� ���������� ��� ��������� ������ � ������)
	wcex.lpfnWndProc = WndProc;					//��� ������� ��� ��������� ���������
	wcex.cbClsExtra = 0;						//��� ������ �����
	wcex.cbWndExtra = 0;						//��� ������ ����
	wcex.hInstance = hInstance;				//������� ����������
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABA_ONE));	//������
	wcex.hCursor = LoadCursor(NULL, IDC_CROSS);							//������-�������
	wcex.hbrBackground = HBRUSH(CreateSolidBrush(RGB(255, 255, 255)));		//���������� ���� ������
	wcex.lpszMenuName = NULL;						//����
	wcex.lpszClassName = szWindowClass;										//��� ������ ����
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));	//���������� ��������� ������
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

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static HDC shadowDC;
	static double x_step, y_step, x = 0, y = 0, x_click, y_click, x_src = 0, y_src = 0;
	static HBITMAP phase_1, phase_2, phase_3, phase_4;
	static long counter = 0;
	static bool first = true;
	switch (message)
	{

	case WM_CREATE:
		phase_1 = LoadBitmap(hInst, MAKEINTRESOURCE(Phase_1));
		phase_2 = LoadBitmap(hInst, MAKEINTRESOURCE(Phase_2));
		phase_3 = LoadBitmap(hInst, MAKEINTRESOURCE(Phase_3));
		phase_4 = LoadBitmap(hInst, MAKEINTRESOURCE(Phase_4));
		//��������� ����������� --> ���������� �������� ������ hbitmap
		hdc = GetDC(hWnd);
		//������� ����������� � hdc ����������
		shadowDC = CreateCompatibleDC(hdc);
		// ����������� hdc
		ReleaseDC(hWnd, hdc);
		break;

	case WM_LBUTTONDOWN:
		{
		if (first) {
			SetTimer(hWnd, 1, 150, NULL);
			first = false;
		}
			x_click = LOWORD(lParam);
			y_click = HIWORD(lParam);
			x_step = (x_click - x_src) / 20;
			y_step = (y_click - y_src) / 20;
		}
		break;

	case WM_TIMER:
		if (x < x_click - 0.5 || x > x_click + 0.5){
			x += x_step;
			x_src = x;
			y += y_step;
			y_src = y;
		}
		switch (counter % 4) {
			case 0:
				SelectObject(shadowDC, phase_1);
				counter++;
				break;
			case 1:
				SelectObject(shadowDC, phase_2);
				counter++;
				break;
			case 2:
				SelectObject(shadowDC, phase_3);
				counter++;
				break;
			case 3:
				SelectObject(shadowDC, phase_4);
				counter++;
				break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
		

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//����� ��������(��������, ��� ����������, ������� �����������, ��������, ��� ����������, ��������� ��������)
		BitBlt(hdc, x - 16, y - 16, 32, 32, shadowDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		DeleteDC(shadowDC);
		break;

	default: 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}