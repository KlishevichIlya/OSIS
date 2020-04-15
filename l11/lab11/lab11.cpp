// lab11.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lab11.h"

#define MAX_LOADSTRING 100

#define ID_BTN_START 101
#define ID_BTN_STOP 102

#define TIMER_ID 121
#define TIMER_INTERVAL 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HANDLE thread1;
HANDLE thread2;
HANDLE thread3;

bool threadStopped = false;
bool isWorking = false;

int color1 = 0;
int color2 = 0;
int color3 = 0;
int step = 5;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void createControls(HWND hWnd);
void draw(HDC hdc);
void initThreads();
void startThreads();
DWORD WINAPI thread1Proc(CONST LPVOID lpParam);
DWORD WINAPI thread2Proc(CONST LPVOID lpParam);
DWORD WINAPI thread3Proc(CONST LPVOID lpParam);

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
    LoadStringW(hInstance, IDC_LAB11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB11));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB11);
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
		createControls(hWnd);
		initThreads();
		SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL, NULL);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_BTN_START:
				isWorking = true;
				break;
			case ID_BTN_STOP:
				isWorking = false;
				break;
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
			draw(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_TIMER:
		if (isWorking) {
			startThreads();
			InvalidateRect(hWnd, NULL, true);
		}
		break;
    case WM_DESTROY:
		threadStopped = true;
		startThreads();
		KillTimer(hWnd, TIMER_ID);
		PostQuitMessage(0);
		CloseHandle(thread1);
		CloseHandle(thread2);
		CloseHandle(thread3);
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

void createControls(HWND hWnd) {
	CreateWindow(L"button", L"Start",
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		10, 30,
		95, 35,
		hWnd, (HMENU)ID_BTN_START,
		hInst, NULL);
	CreateWindow(L"button", L"Stop",
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		10, 90,
		95, 35,
		hWnd, (HMENU)ID_BTN_STOP,
		hInst, NULL);
}

void initThreads() {
	thread1 = CreateThread(NULL, 0, &thread1Proc, NULL, 0, NULL);
	thread2 = CreateThread(NULL, 0, &thread2Proc, NULL, 0, NULL);
	thread3 = CreateThread(NULL, 0, &thread3Proc, NULL, 0, NULL);
}

void startThreads() {
	ResumeThread(thread1);
	ResumeThread(thread2);
	ResumeThread(thread3);
}

DWORD WINAPI thread1Proc(CONST LPVOID lpParam) {
	while (!threadStopped) {
		if (color1 > 250)
			step = -5;
		if (color1 < 5)
			step = 5;
		color1 += step;
		SuspendThread(GetCurrentThread());
	}
	return 0;
}

DWORD WINAPI thread2Proc(CONST LPVOID lpParam) {
	while (!threadStopped) {
		color2 += step;
		SuspendThread(GetCurrentThread());
	}
	return 0;
}

DWORD WINAPI thread3Proc(CONST LPVOID lpParam) {
	while (!threadStopped) {
		color3 += step;
		SuspendThread(GetCurrentThread());
	}
	return 0;
}

void draw(HDC hdc) {
	HBRUSH brush = CreateSolidBrush(RGB(color1, 0, 0));
	SelectObject(hdc, brush);
	Rectangle(hdc, 200, 0, 400, 500);
	brush = CreateSolidBrush(RGB(0, color2, 0));
	SelectObject(hdc, brush);
	Rectangle(hdc, 400, 0, 600, 500);
	brush = CreateSolidBrush(RGB(0, 0, color3));
	SelectObject(hdc, brush);
	Rectangle(hdc, 600, 0, 800, 500);
}