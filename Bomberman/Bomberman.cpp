// Bomberman.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Bomberman.h"
#include "cLogic.h"

#pragma comment(lib, "msimg32.lib")

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

cLogic g_Logic;

void DrawMap(HWND hWnd);            //���Ƶ�ͼ
void DrawStoneAndBrike(HDC hDc);    //��ʯͷ��שͷ
void DrawMan(HDC hDc);              //����

void OnKeyDown(HWND hWnd, WPARAM wParam);
void OnKeyUp(HWND hWnd, WPARAM wParam);



// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BOMBERMAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BOMBERMAN));

    MSG msg;

    // ����Ϣѭ��: 
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BOMBERMAN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_BOMBERMAN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX
	   & ~WS_THICKFRAME,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   HDC screenDc;
   screenDc = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
   int xRes = GetDeviceCaps(screenDc, HORZRES);
   int yRes = GetDeviceCaps(screenDc, VERTRES);

   int xPos = (xRes - 856) / 2;
   int yPos = (yRes - 678) / 2;

   MoveWindow(hWnd, xPos, yPos, 856, 678, NULL);

   g_Logic.GameBegin();

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
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
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
			DrawMap(hWnd);
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_KEYDOWN:
		OnKeyDown(hWnd, wParam);
		break;
	case WM_KEYUP:
		OnKeyUp(hWnd, wParam);
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ�������
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

void DrawMap(HWND hWnd)
{
	HDC hDc = GetWindowDC(hWnd);

	HDC memDc = CreateCompatibleDC(NULL);  //����DC
	HBITMAP hBitMap = CreateCompatibleBitmap(hDc, 850, 650);
	SelectObject(memDc, hBitMap);
	//����ˢ����ɫ
	HBRUSH hBrushBack = CreateSolidBrush(RGB(0, 255, 0));
	RECT rectBack;
	rectBack.top = 0;
	rectBack.bottom = 650;
	rectBack.left = 0;
	rectBack.right = 850;
	FillRect(memDc, &rectBack, hBrushBack);

	DrawStoneAndBrike(memDc);
	
	DrawMan(memDc);

	BitBlt(hDc, 3, 25, 850, 650, memDc, 0, 0, SRCCOPY);//����

	DeleteObject(memDc);
	DeleteObject(hBitMap);
	DeleteObject(hBrushBack);
	
	ReleaseDC(hWnd, hDc);
}

void DrawStoneAndBrike(HDC hDc)
{
	HBRUSH hbrushStone = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH hbrushBrike = CreateSolidBrush(RGB(255, 98, 21));

	for (int i = 0; i < LatticeNum_L; i++)
	{
		for (int j = 0; j < LatticeNum_H; j++)
		{
			LatticeType curType = g_Logic.m_Lattice[i][j];
			if (curType == Type_Stone)
			{
				//��ʯͷ
				RECT rect;
				rect.left = j*Lattice_Len;
				rect.right = rect.left + Lattice_Len;
				rect.top = i*Lattice_Len;
				rect.bottom = rect.top + Lattice_Len;
				FillRect(hDc, &rect, hbrushStone);  //�Ѿ���ʯͷ��������DC��
			}
			else if (curType == Type_Brike || curType == Type_BrikeDoor)
			{
				//שͷ
				RECT rect;
				rect.left = j*Lattice_Len;
				rect.right = rect.left + Lattice_Len;
				rect.top = i*Lattice_Len;
				rect.bottom = rect.top + Lattice_Len;
				FillRect(hDc, &rect, hbrushBrike);  //�Ѿ���שͷ��������DC��
			}
		}
	}
	DeleteObject(hbrushStone);
	DeleteObject(hbrushBrike);
}

void DrawMan(HDC hDc)              //����
{
	HDC dcTemp;
	dcTemp = CreateCompatibleDC(NULL);
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, _T("FireMan.bmp"), IMAGE_BITMAP, Lattice_Len,
		Lattice_Len, LR_LOADFROMFILE);
	CreateCompatibleBitmap(dcTemp, Lattice_Len, Lattice_Len);
	SelectObject(dcTemp, hBitmap);
	//����
	TransparentBlt(hDc, g_Logic.m_fireManPos.x, g_Logic.m_fireManPos.y, Lattice_Len, Lattice_Len,
		dcTemp, 0, 0, Lattice_Len, Lattice_Len, RGB(255, 0, 255));
	
	DeleteObject(dcTemp);
	DeleteObject(hBitmap);
}

void OnKeyDown(HWND hWnd, WPARAM wParam)
{
	if (g_Logic.m_bIsDown)
	{
		return;
	}
	switch (wParam)
	{
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		g_Logic.m_bIsDown = true;
		g_Logic.ManMove(wParam);
		RECT rect;
		GetClientRect(hWnd, &rect);
		InvalidateRect(hWnd, &rect, true);
		UpdateWindow(hWnd);
		break;
	default:
		break;
	}
}

void OnKeyUp(HWND hWnd, WPARAM wParam)
{
	switch (wParam)
	{
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		g_Logic.ManStop();
		break;
	default:
		break;
	}
}