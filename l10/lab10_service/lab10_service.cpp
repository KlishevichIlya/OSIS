// lab10_service.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lab10_service.h"

#define MAX_LOADSTRING 100

#define DRAW 1
#define GREEN 4
#define BLUE 8
#define RED 16
#define DIAMOND 32 
#define SQUARE 64
#define CIRCLE 128
#define STAR 256
#define CLICK 1001
UINT WM_DRAW_FIGURE = RegisterWindowMessage(_T("DrawFigureMessage"));

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

int colorIndex = -1;
int typeIndex = -1;
HWND checkBox;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void createElements(HWND hWnd);
void sendMessage(HWND hWnd);

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
    LoadStringW(hInstance, IDC_LAB10_SERVICE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB10_SERVICE));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB10_SERVICE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB10_SERVICE);
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
      CW_USEDEFAULT, 0, 220, 200, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void createElements(HWND hWnd) {
	CreateWindow(L"button", L"Red",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
		10, 10, 90, 30, hWnd,
		(HMENU)RED, hInst, NULL);
	CreateWindow(L"button", L"Blue",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		10, 40, 90, 20, hWnd,
		(HMENU)BLUE, hInst, NULL);
	CreateWindow(L"button", L"Green",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		10, 60, 90, 30, hWnd,
		(HMENU)GREEN, hInst, NULL);
	checkBox = CreateWindow(L"button", L"Draw", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		10, 90, 90, 30, hWnd,
		(HMENU)CLICK, hInst, NULL);
	CreateWindow(L"button", L"Diamond",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
		110, 10, 90, 30, hWnd,
		(HMENU)DIAMOND, hInst, NULL);
	CreateWindow(L"button", L"Square",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		110, 40, 90, 20, hWnd,
		(HMENU)SQUARE, hInst, NULL);
	CreateWindow(L"button", L"Circle",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		110, 60, 90, 35, hWnd,
		(HMENU)CIRCLE, hInst, NULL);
	CreateWindow(L"button", L"Star",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		110, 90, 90, 30, hWnd,
		(HMENU)STAR, hInst, NULL);
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
	int wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
    switch (message)
    {
	case WM_CREATE:
		createElements(hWnd);
		break;
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
		case RED:
		case GREEN:
		case BLUE:
			colorIndex = wmId;
			sendMessage(hWnd);
			break;
		case DIAMOND:
		case SQUARE:
		case CIRCLE:
		case STAR:
			typeIndex = wmId;
			sendMessage(hWnd);
			break;
		case CLICK:
			sendMessage(hWnd);
			break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
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

void sendMessage(HWND hWnd) {
	int messageParam = SendMessage(checkBox, BM_GETCHECK, 0, 0);
	if (colorIndex != -1 && typeIndex != -1) {
		messageParam |= colorIndex | typeIndex;
		PostMessage(HWND_BROADCAST, WM_DRAW_FIGURE, messageParam, 0);
	}

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
