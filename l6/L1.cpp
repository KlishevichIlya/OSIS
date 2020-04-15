#include "stdafx.h"
#include "L1.h"

//����������� - ��������� �� �������� �������� ������������ �������(�������� ��� ������ ���-�������)
//_T() - �������� ������ Unicode �� ����� ����������

#define MAX_LOADSTRING 100

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
    wcex.hCursor        = LoadCursor(NULL, IDC_CROSS);							//������-�������
    wcex.hbrBackground  = hbrush;								//���������� ���� ������
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LABA_ONE);						//����
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
	PAINTSTRUCT ps;								//���������� ��� �����������
	HDC hdc;									//���������� ��������� ����������(�������� ������� ������)
	static int x = 0;							//���������� ������ ������ �� �����������
	static int sx, sy;
	TCHAR *text = _T("Hello world");
	HFONT myfont;
	SIZE size;									//��������� SIZE � ������� cx � ������� ������ cy
	TCHAR str[256];
	static bool first = true;

    switch (message)
    {

	// lparam - � ������� ����� ������, � ������� - �����
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);			//���������� ���������� �� �������� �������
            // ��������� ����� � ����:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case ID_START:
				SetTimer(hWnd, 1, 50, NULL);
				first = false;
				break;
			case ID_STOP:
				KillTimer(hWnd, 1);
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_TIMER:
		// 92 - ������ �������, 1 - ���
		if (x + 92 + 1 < sx) {
			x += 1;
		}
		else {
			x = 0;
		}
		//����������� ���� (hWnd, ������� �����������, ����� �� ������� ���)
		InvalidateRect(hWnd, NULL, TRUE);
		break;

    case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//myfont(������, ������, ���� ������� ������, ������� �������, 
		//		 ������������, �������� �����������, ��������� ��������,
		//		 �������� ������, ��� � ���������)
		myfont = CreateFont(80, 10, 0, 0, 800, 1, 0, 0, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, _T("Arial"));
		if (first == false) {
			SelectObject(hdc, myfont);
			SetTextColor(hdc, RGB(25, 25, 112)); //���� ������
			SetBkMode(hdc, TRANSPARENT);
			//���������� ������ � ������ ������
			GetTextExtentPoint32(hdc, text, _tcslen(text), &size);
			//TextOut(hdc, (x,y) - ������ ������, ��������� �� ������, ����� ��������� ������)
			TextOut(hdc, x, sy / 2 - 40, text, _tcsclen(text));
		}
		DeleteObject(myfont);
		EndPaint(hWnd, &ps);							//������������ ���������
		break;

    case WM_DESTROY:
        PostQuitMessage(0);
		DeleteObject(hbrush);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���������� ��������� ��� ���� "� ���������".
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
