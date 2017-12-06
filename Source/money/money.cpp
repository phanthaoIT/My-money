// money.cpp : Defines the entry point for the application.
//
#include <Windows.h>
#include "stdafx.h"
#include "money.h"
#include "Resource.h"
#include <commctrl.h>
#include <string>
#include <cwchar>
#include <vector>
#include <io.h>
#include <fcntl.h>
#include <locale>
#include <codecvt>
#include <iostream>
#include <fstream>
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

using namespace std;
#include <windowsx.h>
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;
#define MAX_LOADSTRING 100
#define IDL_LISTVIEW 20
#define ID_ADD		21
#define ID_XOA		22


TCHAR* type[10] = {
	L"Ăn uống", L"Di chuyển", L"Nhà cửa",
	L"Xe cộ", L"Nhu yếu phẩm", L"Dịch vụ",
}; // 6 loại chi tiêu
struct mymoney
{
	WCHAR LoaiChi[50];
	long long soTien;
	WCHAR noidung[100];
}; 
long long total[6] = { 0 };
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND g_hWnd, g_ListView;
HWND loai, button, tien, nd, tong, money, a, b, c, d, e, f, groupD,my;
vector<mymoney> list;
long long sum = 0;
int vtitem;
Color mau[6] = { Color(255, 255, 0, 0), Color(255, 0, 255, 0), Color(255, 0, 0, 255), Color(255, 249, 244, 0), Color(255, 0, 178, 191), Color(255, 81, 31, 144) };
vector<float> toado;
vector<int>color;
HWND p1, p2, p3, p4, p5, p6;
HWND percent[6] = { p1, p2, p3, p4, p5, p6 };
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MONEY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MONEY));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
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
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(201, 228, 214));
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));

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

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle,WS_OVERLAPPED | WS_SYSMENU | WS_ICONIC | WS_MINIMIZEBOX,
      300,0,600,680, NULL, NULL, hInstance, NULL);

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
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
HWND createListView(HWND parentWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight)
{

	HWND m_hListView = CreateWindow(WC_LISTVIEW, L"List View",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | LVS_REPORT | LVS_ICON | LVS_EDITLABELS | LVS_SHOWSELALWAYS | WS_BORDER,
		x, y, nWidth, nHeight, parentWnd, (HMENU)ID, hParentInst, NULL);

	LVCOLUMN lvCol;

	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol.fmt = LVCFMT_LEFT;

	lvCol.cx = 100;
	lvCol.pszText = L"Loại";
	ListView_InsertColumn(m_hListView, 0, &lvCol);

	lvCol.pszText = L"Nội dung";
	lvCol.cx = 125;
	ListView_InsertColumn(m_hListView, 1, &lvCol);

	lvCol.cx = 125;
	lvCol.pszText = L"Số tiền";
	ListView_InsertColumn(m_hListView, 2, &lvCol);

	return m_hListView;
}
bool insertListView(HWND hwnd, int index)
{ // chèn các chi tiêu vào list view
	mymoney item;
	WCHAR* buffer;
	int length;
	length = GetWindowTextLength(nd);
	if (length > 0)
	{
		buffer = new WCHAR[length + 1];
		GetWindowText(nd, buffer, length + 1);
		wcscpy_s(item.noidung, buffer);
	}
	else
	{
		MessageBox(g_hWnd, L"Nội dung không được để trống!", L"Lỗi", MB_ICONWARNING | MB_OK);
		return false;
	}
	length = GetWindowTextLength(tien);
	if (length > 0)
	{
		buffer = new WCHAR[length + 1];
		GetWindowText(tien, buffer, length + 1);
		item.soTien = _wtoi(buffer);
	}
	else
	{
		MessageBox(g_hWnd, L"Số tiền không được để trống!", L"Lỗi", MB_ICONWARNING | MB_OK);
		return false;
	}
	buffer = new WCHAR[50];
	GetWindowText(loai, buffer, 50);
	wcscpy_s(item.LoaiChi, buffer);
	list.push_back(item);
	sum += item.soTien;
	// thêm vào list view
	LV_ITEM lv;
	lv.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
	lv.iItem = index;
	lv.iSubItem = 0;
	lv.pszText = item.LoaiChi;
	ListView_InsertItem(g_ListView, &lv);
	
	ListView_SetItemText(g_ListView, index, 1, item.noidung);
	wsprintf(buffer, L"%I64d", item.soTien);
	ListView_SetItemText(g_ListView, index, 2, buffer);
	return true;

}
void loadToListview(HWND m_hListview)
{// load vào listview
	LV_ITEM lv;
	WCHAR* buffer = new WCHAR[20];

	for (int i = 0; i < list.size(); i++)
	{
		lv.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;

		lv.iItem = i;
		lv.iSubItem = 0;
		lv.pszText = list[i].LoaiChi;
		ListView_InsertItem(m_hListview, &lv); 
		ListView_SetItemText(m_hListview, i, 1, list[i].noidung);
		wsprintf(buffer, L"%I64d", list[i].soTien);
		ListView_SetItemText(m_hListview, i, 2, buffer);
	}
}
void readFile(HWND hwnd, string str)
{
	_setmode(_fileno(stdin), _O_WTEXT);
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wifstream fi(str);
	fi.imbue(loc);
	if (!fi.is_open())
		return;
	fi >> sum;
	fi.ignore();
	mymoney it;
	wstring t;
	while (!fi.eof())
	{
		getline(fi,t);
		wcscpy_s(it.LoaiChi,t.c_str());
		getline(fi, t);
		wcscpy_s(it.noidung, t.c_str());
		fi >> it.soTien;
		fi.ignore();
		list.push_back(it); 
	}
	fi.close();
}
void writeFile(HWND hwnd,string str)
{
	_setmode(_fileno(stdout), _O_WTEXT);
	int length = GetWindowTextLength(hwnd);
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wofstream fo(str);
	fo.imbue(loc);
	if (!fo.is_open())
		return;
	fo << sum;
	for (int i = 0; i < list.size(); i++)
	{
		fo << endl<< list[i].LoaiChi;
		fo << endl<<list[i].noidung;
		fo << endl<<list[i].soTien;
	}
	fo.close();
}
int getType(mymoney item)
{
	if (wcscmp(item.LoaiChi, L"Ăn uống") == 0)
	{
		return 0;
	}
	else if (wcscmp(item.LoaiChi, L"Di chuyển") == 0)
	{
		return 1;
	}
	else if (wcscmp(item.LoaiChi, L"Nhà cửa") == 0)
	{
		return 2;
	}
	else if (wcscmp(item.LoaiChi, L"Xe cộ") == 0)
	{
		return 3;
	}
	else if (wcscmp(item.LoaiChi, L"Nhu yếu phẩm") == 0)
	{
		return 4;
	}
	else
	{
		return 5;
	}
}

void drawchart()
{
	toado.clear();
	color.clear();
	double rad = 0;
	for (int i = 0; i < 6; i++)
		total[i] = 0;
	toado.push_back(rad);
	for (int i = 0; i < list.size(); i++)
	{
		total[getType(list[i])]+=list[i].soTien;
	}
	for (int i = 0; i < 6; i++)
	{
		if (total[i] >0)
		{
			rad += (double)total[i] / sum * 360;
			toado.push_back(rad);
			color.push_back(i);
		}
	}
}
void setText(HWND hwnd, long long value, wstring text)
{
	WCHAR s[255];
	wsprintf(s, L"%ld", value);
	SetWindowText(hwnd, ((wstring)s+text).c_str());
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(18, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	switch (message)
	{
	case WM_CREATE:
	{
					  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
					  g_hWnd = hWnd;
					  HWND groupA = CreateWindow(L"BUTTON", L" Thêm Chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP, 20, 20, 180, 250, hWnd,(HMENU)ID_ADD, hInst, NULL);
					  SendMessage(groupA, WM_SETFONT, WPARAM(hFont), TRUE);
					  loai = CreateWindow(WC_COMBOBOX, L"", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE|WS_BORDER,50, 60, 100, 25, hWnd, NULL, hInst,NULL);
					  SendMessage(loai, WM_SETFONT, WPARAM(hFont), TRUE);
					  
					  nd = CreateWindow(L"EDIT", L"Nội dung", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 50, 105, 100, 25, hWnd, NULL, hInst, NULL);
					  SendMessage(nd, WM_SETFONT, WPARAM(hFont), TRUE);
					
					  tien = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_MULTILINE | ES_AUTOVSCROLL, 50, 160, 100, 25, hWnd, NULL, hInst, NULL);
					  SendMessage(tien, EM_SETCUEBANNER, TRUE, (LPARAM)L"Số tiền");
					  SendMessage(tien, WM_SETFONT, WPARAM(hFont), TRUE);
					  button = CreateWindow(L"BUTTON", L"THÊM", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 70, 210, 70, 40, hWnd,(HMENU)ID_ADD, hInst, NULL);
					  SendMessage(button, WM_SETFONT, WPARAM(hFont), TRUE);

					  HWND groupB = CreateWindow(L"BUTTON", L"Danh sách chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP, 220, 20, 340, 250, hWnd, NULL, hInst, NULL);
					  SendMessage(groupB, WM_SETFONT, WPARAM(hFont), TRUE);
					  g_ListView = createListView(hWnd, IDL_LISTVIEW, hInst,230,50,320,210);

					  HWND groupC = CreateWindow(L"BUTTON", L"Thống kê", WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP, 20, 290, 540, 340, hWnd, NULL, hInst, NULL);
					  SendMessage(groupC, WM_SETFONT, WPARAM(hFont), TRUE);
					  HFONT font = CreateFont(28, lf.lfWidth,
						  lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
						  lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
						  lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
						  lf.lfPitchAndFamily, lf.lfFaceName);
					  tong = CreateWindow(L"STATIC", L"Tổng cộng:", WS_CHILD | WS_VISIBLE, 240, 580, 130, 50, hWnd, NULL, hInst, NULL);
					  SendMessage(tong, WM_SETFONT, WPARAM(font), TRUE);
					  money = CreateWindow(L"STATIC", L"", WS_CHILD | WS_VISIBLE, 370, 580, 150, 30, hWnd, NULL, hInst, NULL);
					  SendMessage(money, WM_SETFONT, WPARAM(font), TRUE);
					  SetTextColor((HDC)tong, RGB(255, 0, 0));

					  groupD = CreateWindow(L"BUTTON", L"Chú Giải", WS_CHILD | BS_GROUPBOX | WS_GROUP, 240 ,340, 290,210, hWnd, NULL, hInst, NULL);
					  SendMessage(groupD, WM_SETFONT, WPARAM(hFont), TRUE);
					  a = CreateWindow(L"STATIC", L"Ăn uống", WS_CHILD , 290, 370, 200, 20, hWnd, NULL, hInst, NULL);
					  SendMessage(a, WM_SETFONT, WPARAM(hFont), TRUE);
					  b = CreateWindow(L"STATIC", L"Di chuyển", WS_CHILD , 290, 400, 200, 50, hWnd, NULL, hInst, NULL);
					  SendMessage(b, WM_SETFONT, WPARAM(hFont), TRUE);
					  c = CreateWindow(L"STATIC", L"Nhà cửa", WS_CHILD , 290, 430, 200, 50, hWnd, NULL, hInst, NULL);
					  SendMessage(c, WM_SETFONT, WPARAM(hFont), TRUE);
					  d = CreateWindow(L"STATIC", L"Xe cộ", WS_CHILD ,290,460,200,50, hWnd, NULL, hInst, NULL);
					  SendMessage(d, WM_SETFONT, WPARAM(hFont), TRUE);
					  e = CreateWindow(L"STATIC", L"Nhu yếu phẩm", WS_CHILD,290,490,200,50, hWnd, NULL, hInst, NULL);
					  SendMessage(e, WM_SETFONT, WPARAM(hFont), TRUE);
					  f = CreateWindow(L"STATIC", L"Dịch vụ", WS_CHILD , 290,520,200,50,hWnd, NULL, hInst, NULL);
					  SendMessage(f, WM_SETFONT, WPARAM(hFont), TRUE);
					  for (int i = 0; i < 6;i++)
						  percent[i] = CreateWindow(L"STATIC", L"", WS_CHILD|WS_VISIBLE, 430, 370+i*30, 200, 50,hWnd, NULL, hInst, NULL);
					  for (int i = 0; i < 6; i++)
					  {
						  SendMessage(loai, CB_ADDSTRING, i, (LPARAM)type[i]);
					  }
					  ComboBox_SetCurSel(loai, 0);
					 readFile(hWnd, "text.txt");
					 WCHAR s[255];
					 wsprintf(s, L"%I64d", sum);
					 SetWindowText(money, (LPCWSTR)s);
					 vtitem = list.size();
					 loadToListview(g_ListView);

	}
		break;
	case WM_CTLCOLORSTATIC:
	{
							  SetBkMode((HDC)wParam, TRANSPARENT);
							  wmId = LOWORD(wParam);
							  HDC hdcStatic = (HDC)wParam;
							  if ((HWND)lParam == tong || (HWND)lParam == money || (HWND)lParam == my)
							  {
								  SetTextColor(hdcStatic, RGB(255,0,0));
							  }
							  return (LRESULT)GetStockObject(NULL_BRUSH);
	}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_CLEAR:
			sum = 0;
			vtitem = 0;
			setText(money, 0, L"");
			ListView_DeleteAllItems(g_ListView);
			list.clear();
			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_ADD:
		{
					   if (insertListView(g_ListView, vtitem))
					   {
						   WCHAR s[255];
						   wsprintf(s, L"%I64d", sum);
						   SetWindowText(money, (LPCWSTR)s);
						   vtitem++;
						   InvalidateRect(hWnd, NULL, 1);
					   }
		}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{
					 hdc = BeginPaint(hWnd, &ps);
					 drawchart();
					 Graphics* graphics=new Graphics(hdc);
					 Pen* pen = new Pen(Color(255,255 ,255), 5);
					 int i = 0; int j = 0;
					 Brush* brush = NULL;
					 if (sum > 0)
					 {
						 graphics->DrawEllipse(pen, 40, 360, 180, 180);
						 while (i < toado.size() - 1)
						 {
							 brush = new SolidBrush(mau[color[i]]);
							 graphics->FillPie(brush, Rect(40, 360, 180, 180), -toado[i], -toado[i + 1] + toado[i]);
							 i++;
						 }
						 ShowWindow(groupD, SW_SHOW);
						 ShowWindow(a, SW_SHOW);
						 ShowWindow(b, SW_SHOW);
						 ShowWindow(c, SW_SHOW);
						 ShowWindow(d, SW_SHOW);
						 ShowWindow(e, SW_SHOW);
						 ShowWindow(f, SW_SHOW);
						 for (int i = 0; i < 6; i++)
							ShowWindow(percent[i], SW_SHOW);
						 for (int i = 0; i < 6; i++)
						 {
							 brush = new SolidBrush(mau[i]);
							 graphics->FillPie(brush, Rect(230, 360 + i * 30, 50, 50), 0, -45);
							 double phantram = (double)total[i] / sum;
							 setText(percent[i], phantram * 100, L" %");
						 }
					 }
					 else
					 {
						 ShowWindow(groupD, SW_HIDE);
						 ShowWindow(a, SW_HIDE);
						 ShowWindow(b, SW_HIDE);
						 ShowWindow(c, SW_HIDE);
						 ShowWindow(d, SW_HIDE);
						 ShowWindow(e, SW_HIDE);
						 ShowWindow(f, SW_HIDE);
						 for (int i = 0; i < 6;i++)
							 ShowWindow(percent[i], SW_HIDE);
					 }
					 delete graphics;
					 EndPaint(hWnd, &ps);
	}
		break;
	case WM_DESTROY:
		GdiplusShutdown(gdiplusToken);
		writeFile(hWnd,"text.txt");
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

