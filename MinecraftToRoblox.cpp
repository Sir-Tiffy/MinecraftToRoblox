// MinecraftToRoblox.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MinecraftToRoblox.h"
#include "Convert.h"

#define MAX_LOADSTRING 100

using namespace std;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

static HWND
	hWndListBox,
	//hWndCustomButton, 
	hWndSaveButton, 
	hWndProgressBar;
string mapDir;
char mapName[MAX_PATH];
char programDir[MAX_PATH];

bool running = false;
HANDLE threadHandle;


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	
	INITCOMMONCONTROLSEX settings;
	settings.dwSize = sizeof(INITCOMMONCONTROLSEX);
	settings.dwICC = ICC_STANDARD_CLASSES;
	InitCommonControlsEx(&settings);

	
	GetModuleFileName(GetModuleHandle(NULL),programDir,MAX_PATH);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MINECRAFTTOROBLOX, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINECRAFTTOROBLOX));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0) > 0)
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MINECRAFTTOROBLOX));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
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

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX,
      CW_USEDEFAULT, CW_USEDEFAULT, 800, 560, NULL, NULL, hInstance, NULL);

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
string GetSaveDir(){
	char saveDir[MAX_PATH];
	SHGetFolderPath(0, CSIDL_APPDATA, NULL, 0 ,saveDir);
	return string(saveDir)+"\\.minecraft\\saves\\";
}

void PopulateList(HWND hWnd){
	/*TCHAR minecraftDir[MAX_PATH] = {};
	SHGetFolderPath(hWnd, CSIDL_APPDATA, NULL, 0 ,minecraftDir);
	static const char saveDir[] = "\\.minecraft\\saves\\*.";
	int i = 0;
	for (;minecraftDir[i];++i);
	for (int j = 0; j < 21 && j+i < MAX_PATH; ++j){
		minecraftDir[j+i] = saveDir[j];
	}
	//DlgDirList(hWnd, minecraftDir, Control_MapListBox, 0, DDL_DIRECTORY);*/


	
	//vector<string> result;
	WIN32_FIND_DATA findData;
	mapDir = GetSaveDir();
	HANDLE fileFind = FindFirstFile((mapDir+'*').c_str(), &findData);
	if (fileFind == INVALID_HANDLE_VALUE) return;
	const string upDir("..");
	while (FindNextFile(fileFind, &findData) != 0){
		string filename(findData.cFileName);
		if ((findData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) && filename != upDir){
			ifstream test(mapDir+filename+"\\level.dat",ios::in|ios::binary);
			if (test.good()) SendMessage(hWndListBox, LB_ADDSTRING, 0, (LPARAM)filename.c_str()); //result.push_back(filename);
			test.close();
		}
	}
	FindClose(fileFind);
}

string GetOutputFilename(HWND window, string selectedMap){
	OPENFILENAME info = {};
	info.lStructSize = sizeof(OPENFILENAME);
	info.hwndOwner = window;
	info.hInstance = hInst;
	info.lpstrFilter = "Roblox Place File\0*.rbxl\0";
	char buffer[MAX_PATH];
	selectedMap += ".rbxl";
	const auto s = selectedMap.size()+1;
	for (unsigned int i = 0; i < s; i++) buffer[i] = selectedMap[i];
	info.lpstrFile = buffer;
	info.nMaxFile = MAX_PATH;
	int good = GetSaveFileName(&info);
	if (good)return buffer;
	return "";
}

void EndConvert(){
	EnableWindow(hWndListBox, true);
	SetWindowText(hWndSaveButton, "CONVERT!");
	running = false;
	SendMessage(hWndProgressBar, PBM_SETPOS, 0, 0);
}

bool threadShouldTerminate;

bool ConfirmCancel(HWND window){
	if (MessageBox(window, "Are you sure you want to cancel?","Confirm",MB_ICONWARNING|MB_YESNOCANCEL) == IDYES){
		threadShouldTerminate=true;
		//TerminateThread(threadHandle,0);
		EndConvert();
		return true;
	};
	return false;
}

bool ShouldThreadTerminate(){
	return threadShouldTerminate;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	switch (message){
		case WM_USER:
			if (wParam == THREAD_DONE) EndConvert();
			break;
		case WM_CREATE:{
			CreateWindow("STATIC", "Step 1:  Choose a Minecraft world to convert."/*"If it is not in the list, click the \"Choose Manually\" button:"*/, WS_CHILD|WS_VISIBLE|SS_SIMPLE|SS_LEFT, 8, 12, 800, 20, hWnd,0, 0, 0);
			//hWndCustomButton = CreateWindowEx(0,  WC_BUTTON, "Choose Manually", WS_CHILD|WS_VISIBLE, 512+96+16, 8, 128, 24, hWnd, (HMENU)Control_OpenCustomButton, NULL, NULL);
			hWndListBox = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTBOX, NULL, WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|LBS_NOTIFY, 8, 40, 778, 200, hWnd, (HMENU)Control_MapListBox, NULL, NULL);
			CreateWindow("STATIC", "Step 2:  Click the CONVERT! button and choose where you want your new Roblox place", WS_CHILD|WS_VISIBLE|SS_LEFT, 12, 256, 768, 20, hWnd,0, 0, 0);
			hWndSaveButton = CreateWindowEx(0, WC_BUTTON, "CONVERT!", WS_CHILD|WS_VISIBLE, 12, 284, 768, 64, hWnd, (HMENU)Control_ConvertButton, NULL, NULL);
			CreateWindow("STATIC", "Step 3:  Wait for conversion to complete", WS_CHILD|WS_VISIBLE|SS_LEFT, 12, 364, 768, 20, hWnd,0, 0, 0);
			hWndProgressBar = CreateWindowEx(0, PROGRESS_CLASS, 0, WS_CHILD|WS_VISIBLE, 12, 392, 768, 32, hWnd, NULL, NULL, NULL);
			CreateWindow("STATIC", "Step 4:\n  a) Open the new Roblox place in Roblox Studio\n  b) Press the Run button to build the terrain\n  c) Wait until terrain has finished building\n  d) Save the game", WS_CHILD|WS_VISIBLE|SS_LEFT, 12, 440, 320, 80, hWnd,0, 0, 0);
			CreateWindow("STATIC", "Minecraft to Roblox Converter\nVersion 1.0  11/12/13\nBy brandonhare", WS_CHILD|WS_VISIBLE|SS_RIGHT, 595, 480, 195, 50, hWnd,0, 0, 0);
			
			EnableWindow(hWndSaveButton, false);
			SendMessage(hWndProgressBar, PBM_SETPOS, 0, 0);

			//SendMessage(hWndProgressBar, PBM_SETPOS, 50, 0);
			//SendMessage(hWndListBox, WM_SETFONT, (WPARAM)GetStockObject(SYSTEM_FIXED_FONT),1);

			//get minecraft dir
			PopulateList(hWnd);

			return 0;
		}
		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case Control_MapListBox:
				if (wmEvent == LBN_SELCHANGE){
					int currentIndex = SendMessage(hWndListBox, LB_GETCURSEL,0, 0);
					if (currentIndex == LB_ERR) EnableWindow(hWndSaveButton, false);
					else {
						SendMessage(hWndListBox, LB_GETTEXT, currentIndex, (LPARAM)mapName);
						EnableWindow(hWndSaveButton, true);
					}
				}
				break;
			/*case Control_OpenCustomButton:{
				OPENFILENAME ofn = {0};
				char filename[MAX_PATH] = "";

				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.lpstrFilter = "MIDI Files\0*.mid\0";
				ofn.lpstrFile = filename;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR;
				ofn.lpstrInitialDir = ".\\midi\\";
				//ofn.lpstrDefExt = "";
				if (GetOpenFileName(&ofn)){
					ChangeState(new PlayableGameState(filename),true);
				}
				break;
			}*/
			case Control_ConvertButton:
				if (running){
					ConfirmCancel(hWnd);
				} else {
					string outputFilename = GetOutputFilename(hWnd, mapName);
					if (outputFilename == "") break;
					
					EnableWindow(hWndListBox, false);
					ConvertParam* stuff = new ConvertParam;
					stuff->programDir = programDir;
					stuff->mapFolder = mapDir+mapName+"\\region\\";
					stuff->mapName = mapName;
					stuff->outputFilename = outputFilename;
					stuff->window = hWnd;
					stuff->bar = hWndProgressBar;
					running = true;
					SetWindowText(hWndSaveButton, "Cancel");
					threadShouldTerminate = false;
					threadHandle = CreateThread(0, 0,(LPTHREAD_START_ROUTINE)Convert,stuff,0,NULL);

					//(Convert,string(programDir), mapDir+mapName+"\\region\\",string(mapName),outputFilename,hWnd,hWndProgressBar);
					
				//Convert(programDir, mapDir+mapName+"\\region\\", mapName, outputFilename, hWnd, hWndProgressBar);
				}
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_CLOSE:
			if (running){
				if (ConfirmCancel(hWnd)) DestroyWindow(hWnd);
			} else DestroyWindow(hWnd);
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
/*INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
*/