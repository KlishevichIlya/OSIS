// lab13.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lab13.h"

#include <windowsx.h>
#include <string.h>
#include <Psapi.h>
#include <scoped_allocator>
#include <string>
#include <vector>

#define MAX_LOADSTRING 100

#define IDC_LIST_PROCESSES 1000
#define IDC_LIST_MODULES 1001

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND listProcesses;
HWND listModules;

DWORD processIds[1024];
DWORD processesCount;

typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > string_t;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void CreateControls(HWND hWnd);
void AddToList(HWND hList, const string_t& s);
void ClearList(HWND hList);
std::vector<string_t> GetProcessModules(int processId);
string_t GetProcessName(int processId);
void SetProcessList();
int GetSelectedIndex(HWND hList);
int GetSelectedProcess();
void ShowModulesForProcess();
void SetPriority(DWORD priority);
string_t GetPriority(DWORD id);

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
    LoadStringW(hInstance, IDC_LAB13, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB13));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB13));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB13);
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
      CW_USEDEFAULT, 0, 720, 640, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void CreateControls(HWND hWnd) {
	listProcesses = CreateWindow(L"listbox", L"Processes list",
		WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
		40, 40, 300, 500, hWnd, (HMENU)IDC_LIST_PROCESSES, hInst, NULL);

	listModules = CreateWindow(L"listbox", L"Modules list",
		WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
		400, 40, 300, 500, hWnd, (HMENU)IDC_LIST_MODULES, hInst, NULL);
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
		CreateControls(hWnd);
		SetProcessList();
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
			case IDC_LIST_PROCESSES:
				if (HIWORD(wParam) == LBN_SELCHANGE)
					ShowModulesForProcess();
				break;
			case ID_SELECTPRIORITY_REAL:
				SetPriority(REALTIME_PRIORITY_CLASS);
				break;
			case ID_SELECTPRIORITY_HIGH:
				SetPriority(HIGH_PRIORITY_CLASS);
				break;
			case ID_SELECTPRIORITY_ABOVENORMAL:
				SetPriority(ABOVE_NORMAL_PRIORITY_CLASS);
				break;
			case ID_SELECTPRIORITY_NORMAL:
				SetPriority(NORMAL_PRIORITY_CLASS);
				break;
			case ID_SELECTPRIORITY_BELOWNORMAL:
				SetPriority(BELOW_NORMAL_PRIORITY_CLASS);
				break;
			case ID_SELECTPRIORITY_IDLE:
				SetPriority(IDLE_PRIORITY_CLASS);
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_CONTEXTMENU:
	{
		HMENU popup = LoadMenu(hInst, MAKEINTRESOURCE(IDR_PRIORITY));
		popup = GetSubMenu(popup, 0);
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		TrackPopupMenuEx(popup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, xPos, yPos, hWnd, NULL);
		break;
	}
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

void AddToList(HWND hList, const string_t& s) {
	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)s.c_str());
}

void ClearList(HWND hList) {
	SendMessage(hList, LB_RESETCONTENT, 0, 0);
}

std::vector<string_t> GetProcessModules(int processId) {
	DWORD bytesNeeded;
	TCHAR moduleName[1024];
	HMODULE hModules[1024];
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
	std::vector<string_t> result;

	if (hProcess != NULL) {
		if (EnumProcessModules(hProcess, hModules, sizeof(hModules), &bytesNeeded)) {
			int countOfModules = bytesNeeded / sizeof(DWORD);

			// Start from 1 because first module is process name
			for (int i = 1; i < countOfModules; i++) {
				GetModuleBaseName(hProcess, hModules[i], moduleName, sizeof(moduleName) / sizeof(TCHAR));
				string_t moduleNameStr(moduleName);
				if (moduleNameStr == _T(""))
					moduleNameStr = _T("UNNAMED");
				result.push_back(string_t(moduleName));
			}
		}
	}

	CloseHandle(hProcess);

	return result;
}

string_t GetProcessName(int processId) {
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

	if (hProcess == NULL) {
		return _T("unknown");
	}

	TCHAR buffer[MAX_PATH];
	GetModuleFileNameEx(hProcess, NULL, buffer, MAX_PATH);

	string_t fullName = string_t(buffer);
	// Get only filename
	string_t name = fullName.substr(fullName.rfind(_T("\\")) + 1, fullName.length());
	return name;
}

void SetProcessList() {
	DWORD bytesNeeded;
	SendMessage(listProcesses, LB_RESETCONTENT, NULL, NULL);
	DWORD idsData[1024];
	EnumProcesses(idsData, sizeof(idsData), &bytesNeeded);
	processesCount = bytesNeeded / sizeof(DWORD);
	int size = 0;
	string_t name;

	for (int i = 0; i < processesCount; i++) {
		name = GetProcessName(idsData[i]);

		if (name.compare(_T("unknown")) == 0) {
			continue;
		}

		processIds[size] = idsData[i];
		TCHAR itemString[1024];

		string_t priority = GetPriority(processIds[size]);
		_stprintf(itemString, _T("%ls (%d) - %ls"), name.c_str(), processIds[size], priority.c_str());

		AddToList(listProcesses, string_t(itemString));
		size++;
	}

	processesCount = size;
}


int GetSelectedIndex(HWND hList) {
	return SendMessage(hList, LB_GETCURSEL, (WPARAM)(0), (LPARAM)(0));
}

int GetSelectedProcess() {
	int selectedItem = GetSelectedIndex(listProcesses);

	if (selectedItem == LB_ERR) {
		return LB_ERR;
	}

	return processIds[selectedItem];
}

void ShowModulesForProcess() {
	int curProcess = GetSelectedProcess();
	if (curProcess == LB_ERR) {
		return;
	}

	std::vector<string_t> modules = GetProcessModules(curProcess);

	ClearList(listModules);
	for (int i = 0; i < modules.size(); i++) {
		AddToList(listModules, modules[i]);
	}
}

void SetPriority(DWORD priority) {
	int curProcess = GetSelectedProcess();
	if (curProcess == LB_ERR) {
		return;
	}

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_SET_INFORMATION, FALSE, curProcess);

	SetPriorityClass(hProcess, priority);

	CloseHandle(hProcess);

	SetProcessList();
}

string_t GetPriority(DWORD id) {
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_SET_INFORMATION, FALSE, id);
	DWORD priority = GetPriorityClass(hProcess);

	switch (priority) {
	case ABOVE_NORMAL_PRIORITY_CLASS:
		return L"Above normal";
	case BELOW_NORMAL_PRIORITY_CLASS:
		return L"Below normal";
	case HIGH_PRIORITY_CLASS:
		return L"High";
	case IDLE_PRIORITY_CLASS:
		return L"Idle";
	case NORMAL_PRIORITY_CLASS:
		return L"Normal";
	case REALTIME_PRIORITY_CLASS:
		return L"Realtime";
	}
}