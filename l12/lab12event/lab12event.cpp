// lab12event.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lab12event.h"

#define MAX_LOADSTRING 100

#define TIMER_ID 121
#define TIMER_INTERVAL 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HANDLE thread1;
HANDLE thread2;
HANDLE thread3;
HANDLE event;

int margin = 0;
int state = 0;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void line(HDC hdc, int x1, int y1, int x2, int y2);
void circle(HDC hdc, int x, int y, int r);
void drawTrain(HDC hdc);

DWORD WINAPI threadProc(CONST LPVOID lpParam);
void initThreads();
void startThreads();

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
    LoadStringW(hInstance, IDC_LAB12EVENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB12EVENT));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB12EVENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB12EVENT);
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
    switch (message)
    {
	case WM_CREATE:
		initThreads();
		event = CreateEvent(NULL, TRUE, FALSE, L"Event");
		SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL, NULL);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
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
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			SetEvent(event);
			drawTrain(hdc);
			ResetEvent(event);
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, TRUE);
		break;
    case WM_DESTROY:
		CloseHandle(thread1);
		CloseHandle(thread2);
		CloseHandle(thread3);
		CloseHandle(event);
		KillTimer(hWnd, NULL);
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

void drawTrain(HDC hdc) {
	HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
	HBRUSH skyBlueBrush = CreateSolidBrush(RGB(126, 192, 238));
	 
	if (state == 0) {
		SelectObject(hdc, redBrush);
		Rectangle(hdc, 100, 50, 200, 150);
	}
	if (state == 1) {
		BeginPath(hdc);
		POINT p[4] = { {100, 100}, {150, 50}, {200,100}, {150,150} };
		Polyline(hdc, p, 4);
		CloseFigure(hdc);
		EndPath(hdc);
		SelectObject(hdc, yellowBrush);
		FillPath(hdc);

	
	}
	if (state == 2) {
		SelectObject(hdc, skyBlueBrush);
		circle(hdc, 150, 100, 50);

	}
	 
}

void circle(HDC hdc, int x, int y, int r) {
	Ellipse(hdc, x - r, y - r, x + r, y + r);
}
 

void initThreads() {
	thread1 = CreateThread(NULL, 0, &threadProc, NULL, 0, NULL);
	thread2 = CreateThread(NULL, 0, &threadProc, NULL, 0, NULL);
	thread3 = CreateThread(NULL, 0, &threadProc, NULL, 0, NULL);
}

void startThreads() {
	ResumeThread(thread1);
	ResumeThread(thread2);
	ResumeThread(thread3);
}

DWORD WINAPI threadProc(CONST LPVOID lpParam) {
	while (true) {
		DWORD dwWaitResult = WaitForSingleObject(event, 1);
		switch (dwWaitResult) {
		case WAIT_OBJECT_0:
			state++;
			state %= 3;
			Sleep(150);
			break;

		case WAIT_ABANDONED:
			break;
		}
	}

	return 0;
}