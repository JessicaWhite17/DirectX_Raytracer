// DX_test1.cpp : Definiert den Einstiegspunkt f�r die Anwendung.
//

#include "stdafx.h"
#include "DX_test1.h"
#include "D3D.h"
#include "D3D10.h"
#include "D3DX10.h"
#include <iostream>

using namespace std;

#define MAX_LOADSTRING 100

//D3D variables
D3D direct3D;


//Raytracing Variables
ID3D10Texture2D*			pTraceTexture = NULL;	



// Globale Variablen:
HINSTANCE hInst;								// Aktuelle Instanz
HWND ghWnd;
TCHAR szTitle[MAX_LOADSTRING];					// Titelleistentext
TCHAR szWindowClass[MAX_LOADSTRING];			// Klassenname des Hauptfensters

// Vorw�rtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void				trace();
void				initTracing();

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Hier Code einf�gen.
	MSG msg = {0};
	HACCEL hAccelTable;

	// Globale Zeichenfolgen initialisieren
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DX_TEST1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Anwendungsinitialisierung ausf�hren:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DX_TEST1));


	if(FAILED(direct3D.init(ghWnd, hInstance))) 
		MessageBox(ghWnd, L"D3D init failed", L"FAILURE", MB_OK);

	initTracing();

	// Hauptnachrichtenschleife:
	while(WM_QUIT != msg.message)
	{
	
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)== TRUE)
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				direct3D.render();
			}
			

		}
	}

	return (int) msg.wParam;
}




void initTracing()
{
	RECT rc;
	GetClientRect(ghWnd, &rc);

	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	
	D3D10_TEXTURE2D_DESC d;
	d.Width = width;
	d.Height = height;
	d.MipLevels = 1;
	d.ArraySize = 1;
	d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d.SampleDesc.Count = 1;
	d.SampleDesc.Quality = 0;
	d.MiscFlags = 0;
	d.Usage = D3D10_USAGE_DYNAMIC;
	d.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	d.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	if(FAILED(direct3D.pd3dDevice->CreateTexture2D(&d, NULL, &pTraceTexture)))
		MessageBox(ghWnd, L"Failed to create Trace-Texture", L"ASSHOLE", MB_OK);

}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX_TEST1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}



BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Instanzenhandle in der globalen Variablen speichern
   
   ghWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 100, 100, window_width, window_height, NULL, NULL, hInstance, NULL);

  

   if (!ghWnd)
   {
	//  cout << "kein HWND da!";
      return FALSE;
   }

   ShowWindow(ghWnd, nCmdShow);
   UpdateWindow(ghWnd);

   return TRUE;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Men�auswahl bearbeiten:
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
		// TODO: Hier den Zeichnungscode hinzuf�gen.
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

// Meldungshandler f�r Infofeld.
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
