// AsciiTable.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "AsciiTable.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HDC hdcCompatible;
HBITMAP hbmpAsciiTable;
SIZE size;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ASCIITABLE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		if (hbmpAsciiTable)
			DeleteObject(hbmpAsciiTable);
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASCIITABLE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	DeleteObject(hbmpAsciiTable);
	DeleteDC(hdcCompatible);

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASCIITABLE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ASCIITABLE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;
   //DWORD dwStyle;
   BOOL bRet;
   HDC hdc;
   RECT rect;

   hInst = hInstance; // Store instance handle in our global variable

   hbmpAsciiTable = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
   if (!hbmpAsciiTable)
	   return FALSE;

   BITMAP bm;
   memset(&bm, 0, sizeof(BITMAP));
   GetObject(hbmpAsciiTable, sizeof(BITMAP), (LPBYTE)&bm);
   size.cx = bm.bmWidth;
   size.cy = bm.bmHeight;

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd)
      return FALSE;

   hdc = GetDC(hWnd);
   if (!hdc)
	   return FALSE;
   hdcCompatible = CreateCompatibleDC(hdc);
   SelectObject(hdcCompatible, hbmpAsciiTable);

   WINDOWINFO info;

   memset(&info, 0, sizeof(info));
   info.cbSize = sizeof(info);
   GetWindowInfo(hWnd, &info);

   rect = info.rcClient;
   rect.right = rect.left + size.cx;
   rect.bottom = rect.top + size.cy;

   LONG dwStyle;
   dwStyle = GetWindowLong(hWnd, GWL_STYLE);
   bRet = AdjustWindowRect(&rect, dwStyle, TRUE);

   if (!bRet)
	   return FALSE;
   
   MoveWindow(hWnd, rect.left, rect.top, rect.right - rect.left, 
	   rect.bottom - rect.top, FALSE);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);   
   
   ReleaseDC(hWnd, hdc);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static POINT pt = {0};

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
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
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...

		hdc = GetDC(hWnd);
		BitBlt(hdc, 0, 0, size.cx, size.cy, hdcCompatible, 0, 0, SRCCOPY);
		ReleaseDC(hWnd, hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);

		SetROP2(hdcCompatible, R2_NOT);
		
		MoveToEx(hdcCompatible, 0, pt.y, NULL);
		LineTo(hdcCompatible, size.cx, pt.y); 

		MoveToEx(hdcCompatible, pt.x, 0, NULL);
		LineTo(hdcCompatible, pt.x, size.cy);

		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		MoveToEx(hdcCompatible, 0, pt.y, NULL);
		LineTo(hdcCompatible, size.cx, pt.y); 

		MoveToEx(hdcCompatible, pt.x, 0, NULL);
		LineTo(hdcCompatible, pt.x, size.cy);

		BitBlt(hdc, 0, 0, size.cx, size.cy, hdcCompatible, 0, 0, SRCCOPY);

		ReleaseDC(hWnd, hdc);
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
