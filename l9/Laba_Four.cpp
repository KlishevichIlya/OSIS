#include "stdafx.h"
#include "Laba_Four.h"
#include <math.h>

//ƒескприптор - описатель л€ описани€ объектов операционной системы(примерно как индекс хэш-таблицы)
//_T() - создание строки Unicode на этапе компил€ции

#define MAX_LOADSTRING 100
const int SPAN = 10;

// √лобальные переменные:
HINSTANCE hInst;                                // текущий экземпл€р
WCHAR szTitle[MAX_LOADSTRING];                  // “екст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // им€ класса главного окна

												// ќтправить объ€влени€ функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);	//функци€ регистрации окна(почти int)
BOOL                InitInstance(HINSTANCE, int);			//инициализаци€ окна
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);	//прототип окна (lresult - значение оконной процедуры long)

															//головна€ функци€
int APIENTRY wWinMain(HINSTANCE hInstance,				//текущее приложение
	HINSTANCE hPrevInstance,			//предыдущее приложение (в современных всегда ноль)
	LPWSTR    lpCmdLine,				//командна€ строка
	int       mode)					//режим отображени€
{
	UNREFERENCED_PARAMETER(hPrevInstance);				//дабы компил€тор не ругалс€, что переменные не используютс€
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: разместите код здесь.

	// »нициализаци€ глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LABA_ONE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ¬ыполнить инициализацию приложени€:
	if (!InitInstance(hInstance, mode))
	{
		return FALSE;
	}
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABA_ONE));		//загрузка таблицы гор€чих клавиш
																							//структура дл€ хранени€ сообщений
	MSG msg;

	// ÷икл основного сообщени€(указатель на структуру, окна(все), min значение соообщени€, max):
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))			//не нажата ли гор€ча€ клавиша? ≈сли да, то ступаем к WndProc
		{
			TranslateMessage(&msg);			//трансл€ци€ кодов нажатой клавиши
			DispatchMessage(&msg);			//посылает сообщение функции WndProc
		}
	}
	return (int)msg.wParam;
}

//
//  ‘”Ќ ÷»я: MyRegisterClass()
//
//  Ќј«Ќј„≈Ќ»≈: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;					//класс окна
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;	// —тиль окна(атвоматическа€ перерисова при изменении высоты и ширины)
	wcex.lpfnWndProc = WndProc;					//»м€ функции дл€ обработки сообщений
	wcex.cbClsExtra = 0;						//доп данные класс
	wcex.cbWndExtra = 0;						//доп данные окна
	wcex.hInstance = hInstance;				//текущее приложение
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABA_ONE));	//иконка
	wcex.hCursor = LoadCursor(NULL, IDC_CROSS);							//курсор-крестик
	wcex.hbrBackground = HBRUSH(CreateSolidBrush(RGB(255, 255, 255)));		//заполнение окна цветом
	wcex.lpszMenuName = NULL;						//меню
	wcex.lpszClassName = szWindowClass;										//им€ класса окна
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));	//дескриптор маленькой иконки
	return RegisterClassExW(&wcex);
}

//
//   ‘”Ќ ÷»я: InitInstance(HINSTANCE, int)
//
//   Ќј«Ќј„≈Ќ»≈: сохран€ет обработку экземпл€ра и создает главное окно.
//
//    ќћћ≈Ќ“ј–»»:
//        ¬ данной функции дескриптор экземпл€ра сохран€етс€ в глобальной переменной, а также
//        создаетс€ и выводитс€ на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int mode)
{
	hInst = hInstance; // —охранить дескриптор экземпл€ра в глобальной переменной

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
//  ‘”Ќ ÷»я: WndProc(HWND, UINT, WPARAM, LPARAM)	
//
//  Ќј«Ќј„≈Ќ»≈:  обрабатывает сообщени€ в главном окне.
//
//	WM_CREATE - создание окна
//  WM_COMMAND Ч обработать меню приложени€
//  WM_PAINT Ч отрисовать главное окно
//  WM_DESTROY Ч отправить сообщение о выходе и вернутьс€
//	WM_SIZE - генерируетс€ при изменении размера окна
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
		//загружаем изображение --> дескриптор битового образа hbitmap
		hdc = GetDC(hWnd);
		//создаем совместимый с hdc дескриптор
		shadowDC = CreateCompatibleDC(hdc);
		// освобождаем hdc
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
		//вывод картинки(приемник, его координаты, размеры изображени€, источник, его координаты, растрова€ операци€)
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