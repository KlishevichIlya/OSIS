#include "stdafx.h"
#include "Laba_Two.h"
#include "windows.h"
#include <vector>
#include <algorithm>

using namespace std;

//����������� - ��������� �� �������� �������� ������������ �������(�������� ��� ������ ���-�������)
//_T() - �������� ������ Unicode �� ����� ����������

#define MAX_LOADSTRING 100
#define ID_ADD 101
#define ID_CLEAR 102
#define ID_TO_RIGHT 103
#define ID_DELETE 104
#define ID_EDIT 105
#define ID_LIST_FIRST 106
#define ID_LIST_SECOND 107
#define MAX_BUFFER_SIZE 100

// ���������� ����������:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����

// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM                MyRegisterClass(HINSTANCE hInstance);	//������� ����������� ����(����� int)
BOOL                InitInstance(HINSTANCE, int);			//������������� ����
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);	//�������� ���� (lresult - �������� ������� ��������� long)
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);		//��� ����� ��������� � int, � ���������
HBRUSH hbrush = CreateSolidBrush(RGB(173, 216, 230));

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
    wcex.hbrBackground  = hbrush;								//���������� ���� ������
    wcex.lpszMenuName   = NULL;									//����
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_CAPTION|WS_OVERLAPPED|WS_SYSMENU,
      330, 150, 690, 340, nullptr, nullptr, hInstance, nullptr);

   
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
	
	//�������� ����������
	HWND hBtn_Add, hBtn_Clear, hBtn_ToRight, hBtn_Delete;
	static HWND hListBox_1, hListBox_2;
	static HWND hEdit;
	static vector<wstring> firstList;
	static vector<wstring> secondList;
	static wstring item;

    switch (message)
    {
	case WM_CREATE:
		//��� ������� ������������ msg � ���������� wParam(LOWORD), ��������������� ����������� ������
		hEdit = CreateWindowA("Edit", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER, 20, 260, 420, 20, hWnd, (HMENU)ID_EDIT, NULL, NULL);
		hListBox_1 = CreateWindowA("ListBox", "", WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 20, 20, 200, 230, hWnd, (HMENU)ID_LIST_FIRST, NULL, NULL);
		hListBox_2 = CreateWindowA("ListBox", "", WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 240, 20, 200, 230, hWnd, (HMENU)ID_LIST_SECOND, NULL, NULL);
		hBtn_Add = CreateWindowW(L"Button", L"��������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 460, 20, 200, 50, hWnd, (HMENU)ID_ADD, NULL, NULL);
		hBtn_Clear = CreateWindowA("Button", "��������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 460, 90, 200, 50, hWnd, (HMENU)ID_CLEAR, NULL, NULL);
		hBtn_ToRight = CreateWindowA("Button", "����������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 460, 160, 200, 50, hWnd, (HMENU)ID_TO_RIGHT, NULL, NULL);
		hBtn_Delete = CreateWindowA("Button", "�������", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 460, 230, 200, 50, hWnd, (HMENU)ID_DELETE, NULL, NULL);
		break;

	case WM_PAINT:
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case ID_ADD:
				int len;
				wchar_t text[MAX_BUFFER_SIZE];
				len = GetWindowTextLengthW(hEdit) + 1;
				GetWindowTextW(hEdit, text, len);
				SetWindowTextW(hEdit, L"");
				if ((find(firstList.begin(), firstList.end(), text) == firstList.end()) && len > 1) {
					SendMessage(hListBox_1, LB_ADDSTRING, 0, (LPARAM)text);
					firstList.push_back(text);
				}
				UpdateWindow(hWnd);
				break;
			case ID_CLEAR:
				SendMessage(hListBox_1, LB_RESETCONTENT, 0, 0);
				SendMessage(hListBox_2, LB_RESETCONTENT, 0, 0);
				firstList.clear();
				secondList.clear();
				UpdateWindow(hWnd);
				break;
			case ID_TO_RIGHT:
				int sel;
				sel = (int)SendMessageW(hListBox_1, LB_GETCURSEL, 0, 0);
				if (~sel) {
					item = (firstList.at(sel));
					if (find(secondList.begin(), secondList.end(), item) == secondList.end()) {
						SendMessage(hListBox_2, LB_ADDSTRING, 0, (LPARAM)item.c_str());
						secondList.push_back(item);
						UpdateWindow(hWnd);
					}
				}
				break;
			case ID_DELETE:
				int ind1 = (int)SendMessage(hListBox_1, LB_GETCURSEL, 0, 0);
				int ind2 = (int)SendMessage(hListBox_2, LB_GETCURSEL, 0, 0);
				if (~ind1) {
					SendMessage(hListBox_1, LB_DELETESTRING, ind1, 0);
					firstList.erase(firstList.begin() + ind1);
				}
				if (~ind2) {
					SendMessage(hListBox_2, LB_DELETESTRING, ind2, 0);
					secondList.erase(secondList.begin() + ind2);
				}
				UpdateWindow(hWnd);
				break;
		}
		break;

    case WM_DESTROY:
        PostQuitMessage(0);
		DeleteObject(hbrush);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
		break;
    }
	return 0;
}

