// lab14.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lab14.h"
#include "RegistryService.h"

#define MAX_LOADSTRING 100

#define ID_LIST_RESULTS 1001
#define ID_EDIT_QUERY 1002
#define ID_BTN_SEARCH 1003
#define ID_BTN_CANCEL 1004
#define ID_CHECK_HKLM 1005
#define ID_CHECK_HKCR 1006
#define ID_CHECK_HKCU 1007
#define ID_CHECK_HKU 1008
#define ID_CHECK_HKCC 1009

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HANDLE thread;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void createControls(HWND hWnd);
void onBtnSearchClick(HWND hWnd);
void onBtnStopClick(HWND hWnd);
void clearList(HWND hList);
DWORD WINAPI searchThreadProc(CONST LPVOID lpParam);
std::vector<HKEY> getSearchKeys(HWND hWnd);

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
    LoadStringW(hInstance, IDC_LAB14, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB14));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB14));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB14);
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
      CW_USEDEFAULT, 0, 600, 550, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


void createControls(HWND hWnd) {
	CreateWindow(L"edit", L"", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE,
		20, 20, 200, 30, hWnd, (HMENU)ID_EDIT_QUERY, hInst, NULL);
	CreateWindow(L"button", L"Search", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		230, 20, 60, 30, hWnd, (HMENU)ID_BTN_SEARCH, hInst, NULL);
	CreateWindow(L"button", L"Cancel", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 20, 60, 30, hWnd, (HMENU)ID_BTN_CANCEL, hInst, NULL);
	EnableWindow(GetDlgItem(hWnd, ID_BTN_CANCEL), false);

	CreateWindow(L"listbox", L"Results", WS_CHILD | WS_VISIBLE | LBS_EXTENDEDSEL | WS_BORDER | WS_VSCROLL | WS_HSCROLL,
		20, 70, 360, 400, hWnd, (HMENU)ID_LIST_RESULTS, hInst, NULL);
	SendDlgItemMessage(hWnd, ID_LIST_RESULTS, LB_SETHORIZONTALEXTENT, 1000, NULL);

	CreateWindow(L"button", L"HKLM", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
		400, 70, 100, 50, hWnd, (HMENU)ID_CHECK_HKLM, hInst, NULL);
	CreateWindow(L"button", L"HKCU", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
		400, 120, 100, 50, hWnd, (HMENU)ID_CHECK_HKCU, hInst, NULL);
	CreateWindow(L"button", L"HKCR", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
		400, 170, 100, 50, hWnd, (HMENU)ID_CHECK_HKCR, hInst, NULL);
	CreateWindow(L"button", L"HKU", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
		400, 220, 100, 50, hWnd, (HMENU)ID_CHECK_HKU, hInst, NULL);
	CreateWindow(L"button", L"HKCC", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
		400, 270, 100, 50, hWnd, (HMENU)ID_CHECK_HKCC, hInst, NULL);
}

void onBtnSearchClick(HWND hWnd) {
	onBtnStopClick(hWnd);
	thread = CreateThread(NULL, 0, &searchThreadProc, hWnd, 0, NULL);
	ResumeThread(thread);
}

void onBtnStopClick(HWND hWnd) {
	EnableWindow(GetDlgItem(hWnd, ID_BTN_CANCEL), false);
	SuspendThread(thread);
	TerminateThread(thread, 0);
}

void clearList(HWND hList) {
	SendMessage(hList, LB_RESETCONTENT, NULL, NULL);
}

DWORD WINAPI searchThreadProc(CONST LPVOID lpParam) {
	TCHAR buffer[1024];
	HWND hWnd = static_cast<HWND>(lpParam);
	HWND hList = GetDlgItem(hWnd, ID_LIST_RESULTS);
	clearList(hList);
	EnableWindow(GetDlgItem(hWnd, ID_BTN_CANCEL), true);

	GetWindowText(GetDlgItem(hWnd, ID_EDIT_QUERY), buffer, 1024);
	auto query = std::wstring(buffer);

	auto searchKeys = getSearchKeys(hWnd);

	RegistryService* regService = new RegistryService(hList);
	regService->search(query, searchKeys);

	EnableWindow(GetDlgItem(hWnd, ID_BTN_CANCEL), false);
	return 0;
}

std::vector<HKEY> getSearchKeys(HWND hWnd) {
	std::vector<HKEY> result;

	if (IsDlgButtonChecked(hWnd, ID_CHECK_HKLM)) {
		result.push_back(HKEY_LOCAL_MACHINE);
	}

	if (IsDlgButtonChecked(hWnd, ID_CHECK_HKCC)) {
		result.push_back(HKEY_CURRENT_CONFIG);
	}

	if (IsDlgButtonChecked(hWnd, ID_CHECK_HKCU)) {
		result.push_back(HKEY_CURRENT_USER);
	}

	if (IsDlgButtonChecked(hWnd, ID_CHECK_HKCR)) {
		result.push_back(HKEY_CLASSES_ROOT);
	}

	if (IsDlgButtonChecked(hWnd, ID_CHECK_HKU)) {
		result.push_back(HKEY_USERS);
	}

	return result;
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
			case ID_BTN_SEARCH:
				onBtnSearchClick(hWnd);
				break;
			case ID_BTN_CANCEL:
				onBtnStopClick(hWnd);
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
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
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
