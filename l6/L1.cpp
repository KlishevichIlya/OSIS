#include "stdafx.h"
#include "L1.h"

//Дескприптор - описатель ля описания объектов операционной системы(примерно как индекс хэш-таблицы)
//_T() - создание строки Unicode на этапе компиляции

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);	//функция регистрации окна(почти int)
BOOL                InitInstance(HINSTANCE, int);			//инициализация окна
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);	//прототип окна (lresult - значение оконной процедуры long)
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);		//тип может содержать и int, и указатель
HBRUSH hbrush = CreateSolidBrush(RGB(173, 216, 230));

//головная функция
int APIENTRY wWinMain(HINSTANCE hInstance,				//текущее приложение
					  HINSTANCE hPrevInstance,			//предыдущее приложение (в современных всегда ноль)
                      LPWSTR    lpCmdLine,				//командная строка
                      int       mode)					//режим отображения
{
    UNREFERENCED_PARAMETER(hPrevInstance);				//дабы компилятор не ругался, что переменные не используются
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LABA_ONE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, mode))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABA_ONE));		//загрузка таблицы горячих клавиш
	//структура для хранения сообщений
    MSG msg;				

    // Цикл основного сообщения(указатель на структуру, окна(все), min значение соообщения, max):
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))			//не нажата ли горячая клавиша? Если да, то ступаем к WndProc
        {
            TranslateMessage(&msg);			//трансляция кодов нажатой клавиши
            DispatchMessage(&msg);			//посылает сообщение функции WndProc
        }
    }
    return (int) msg.wParam;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;					//класс окна
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;	// Стиль окна(атвоматическая перерисова при изменении высоты и ширины)
    wcex.lpfnWndProc    = WndProc;					//Имя функции для обработки сообщений
    wcex.cbClsExtra     = 0;						//доп данные класс
    wcex.cbWndExtra     = 0;						//доп данные окна
    wcex.hInstance      = hInstance;				//текущее приложение
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABA_ONE));	//иконка
    wcex.hCursor        = LoadCursor(NULL, IDC_CROSS);							//курсор-крестик
    wcex.hbrBackground  = hbrush;								//заполнение окна цветом
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LABA_ONE);						//меню
    wcex.lpszClassName  = szWindowClass;										//имя класса окна
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));	//дескриптор маленькой иконки
    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int mode)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

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
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)	
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//	WM_CREATE - создание окна
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//	WM_SIZE - генерируется при изменении размера окна
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;								//информация для перерисовки
	HDC hdc;									//дескриптор контекста устройства(свойства данного вывода)
	static int x = 0;							//координаты начала вывода по горизонтали
	static int sx, sy;
	TCHAR *text = _T("Hello world");
	HFONT myfont;
	SIZE size;									//структура SIZE с шириной cx и высотой строки cy
	TCHAR str[256];
	static bool first = true;

    switch (message)
    {

	// lparam - в младшем слове ширину, в старшем - длину
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);			//извлечение параметров из младшего разряда
            // Разобрать выбор в меню:
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
		// 92 - ширина надписи, 1 - шаг
		if (x + 92 + 1 < sx) {
			x += 1;
		}
		else {
			x = 0;
		}
		//перерисовка окна (hWnd, область перерисовки, нужно ли стирать фон)
		InvalidateRect(hWnd, NULL, TRUE);
		break;

    case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//myfont(высота, ширина, угол наклона строки, каждого символа, 
		//		 насыщенность, точность отображения, отсечение символов,
		//		 качество шрифта, тип и семейство)
		myfont = CreateFont(80, 10, 0, 0, 800, 1, 0, 0, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, _T("Arial"));
		if (first == false) {
			SelectObject(hdc, myfont);
			SetTextColor(hdc, RGB(25, 25, 112)); //цвет текста
			SetBkMode(hdc, TRANSPARENT);
			//логическая ширина и высота строки
			GetTextExtentPoint32(hdc, text, _tcslen(text), &size);
			//TextOut(hdc, (x,y) - начало вывода, указатель на строку, длина выводимой строки)
			TextOut(hdc, x, sy / 2 - 40, text, _tcsclen(text));
		}
		DeleteObject(myfont);
		EndPaint(hWnd, &ps);							//освобождение контекста
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

// Обработчик сообщений для окна "О программе".
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
