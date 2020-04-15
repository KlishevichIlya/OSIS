// lab10_drawer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lab10_drawer.h"
#define _USE_MATH_DEFINES
#include <math.h>


#define MAX_LOADSTRING 100

#define DRAW 1
#define GREEN 4
#define BLUE 8
#define RED 16
#define DIAMOND 32 
#define SQUARE 64
#define CIRCLE 128
#define STAR 256
#define SCALE 30.0
#define COLOR_TYPE_DIVIDER 5
UINT WM_DRAW_FIGURE = RegisterWindowMessage(_T("DrawFigureMessage"));

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

int typeIndex = -1;
int colorIndex = -1;
int x = 0;
int y = 0;
bool canDraw = false;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void UpdateParams(HWND hWnd, int value);
void DrawFigure(HDC hdc);
void DrawDiamond(HDC hdc, COLORREF color);
void DrawSquare(HDC hdc, COLORREF color);
void DrawCircle(HDC hdc, COLORREF color);
void DrawStar(HDC hdc, COLORREF color);
int GetFigureType(int value);
int GetColor(int value);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB10_DRAWER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB10_DRAWER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB10_DRAWER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB10_DRAWER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;
	PAINTSTRUCT ps;
	HDC hdc;

	if (message == WM_DRAW_FIGURE) {
		UpdateParams(hWnd, wParam);
		return 0;
	}

    switch (message)
    {
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, true);
		break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

		if (canDraw) {
			DrawFigure(hdc);
		}

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

int GetColor(int value) {
	return ((value >> COLOR_TYPE_DIVIDER) << COLOR_TYPE_DIVIDER) ^ value;
}

int GetFigureType(int value) {
	return ((value >> COLOR_TYPE_DIVIDER) << COLOR_TYPE_DIVIDER) & value;
}

void UpdateParams(HWND hWnd, int value) {
	canDraw = (value & DRAW) == DRAW;
	if (canDraw) value = value ^ DRAW;
	colorIndex = GetColor(value);
	typeIndex = GetFigureType(value);
}

void DrawFigure(HDC hdc) {
	HBRUSH hBrush;
	HPEN hPen;
	COLORREF color;
	switch (colorIndex) {
	case RED:
		color = RGB(255, 0, 0);
		break;
	case BLUE:
		color = RGB(0, 0, 255);
		break;
	case GREEN:
		color = RGB(0, 255, 0);
		break;
	}

	hBrush = CreateSolidBrush(color);
	hPen = CreatePen(PS_SOLID, 1, color);
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);

	switch (typeIndex) {
	case DIAMOND:
		DrawDiamond(hdc, color);
		break;
	case SQUARE:
		DrawSquare(hdc, color);
		break;
	case CIRCLE:
		DrawCircle(hdc, color);
		break;
	case STAR:
		DrawStar(hdc, color);
		break;
	}
}

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);
	return LineTo(hdc, x2, y2);
}

void DrawDiamond(HDC hdc, COLORREF color) {
	POINT arr[5];
	arr[0].x = x;
	arr[0].y = y;
	arr[1].x = x + SCALE;
	arr[1].y = y + SCALE;
	arr[2].x = x + 2 * SCALE;
	arr[2].y = y;
	arr[3].x = x + 3 / 2.0 * SCALE;
	arr[3].y = y - SCALE / 2.0;
	arr[4].x = x + SCALE / 2.0;
	arr[4].y = y - SCALE / 2.0;

	Polygon(hdc, arr, 5);
}

void DrawSquare(HDC hdc, COLORREF color) {
	Rectangle(hdc, x, y, x + 30, y + 30);
}

void DrawCircle(HDC hdc, COLORREF color) {
	Ellipse(hdc, x, y, x + SCALE, y + SCALE);
}

void DrawStar(HDC hdc, COLORREF color) {
	POINT arr[5];
	arr[0].x = x;
	arr[0].y = y - SCALE;
	arr[2].x = x + cos(18 * (M_PI / 180)) * SCALE;
	arr[2].y = y - sin(18 * (M_PI / 180)) * SCALE;
	arr[4].x = x + sin(36 * (M_PI / 180)) * SCALE;
	arr[4].y = y + cos(36 * (M_PI / 180)) * SCALE;
	arr[1].x = x - sin(36 * (M_PI / 180)) * SCALE;
	arr[1].y = y + cos(36 * (M_PI / 180)) * SCALE;
	arr[3].x = x - cos(18 * (M_PI / 180)) * SCALE;
	arr[3].y = y - sin(18 * (M_PI / 180)) * SCALE;
	Polygon(hdc, arr, 5);
}