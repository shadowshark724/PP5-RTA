// RTA Cody Stanko.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "RTA Cody Stanko.h"
#include "DirectXStuff.h"
#include "..\RTADLL\RTADLL.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hWnd;
int width = 500;
int height = 500;
DirectXStuff Direct;
RTA::functions stuff;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

struct  vertex
{
	float xyzw[4];
};
bool bonesOn = true;
float mouseX, mouseY;

int FbxToDirect()
{
	if (Direct.vertsIn != nullptr)
	{
		delete[] Direct.vertsIn;
	}
	int iSize = stuff.getIndexSize();
	int vSize = stuff.getVertexSize();
	if (iSize <= 0)
	{
		return -1;
	}
	if (vSize <= 0)
	{
		return -2;
	}
	Direct.index = new int[iSize];
	for (int i = 0; i < iSize; i++)
	{
		Direct.index[i] = stuff.getIndex()[i];
	}
	Direct.vertsIn = new DirectXStuff::vertex[(vSize*2)+44+ stuff.getBones().size()];
	RTA::Vertex * fbxVert = stuff.getVertex();
	DirectXStuff::vertex * temps = new DirectXStuff::vertex[vSize];
	//Direct.vertsIn = new DirectXStuff::vertex[vSize];
	for (int i = 0; i < vSize; i++)
	{
		temps[i].Position.x = fbxVert[i].xyzw[0];
		temps[i].Position.y = fbxVert[i].xyzw[1];
		temps[i].Position.z = fbxVert[i].xyzw[2];
		temps[i].Position.w = 1.0f;
		temps[i].Color.x = 1.0f;
		temps[i].Color.y = 0.0f;
		temps[i].Color.z = 0.0f;
		temps[i].Color.w = 1.0f;
	//	printf("Debug here - %0.2f, %0.2f, %0.2f\n", Direct.vertsIn[i].Position.x, Direct.vertsIn[i].Position.y, Direct.vertsIn[i].Position.z);
	}
	for (int i = 0; i < vSize / 3; i++)
	{
		Direct.vertsIn[i * 6 + 0] = temps[i * 3 + 0]; 
		Direct.vertsIn[i * 6 + 1] = temps[i * 3 + 1];
		Direct.vertsIn[i * 6 + 2] = temps[i * 3 + 1];
		Direct.vertsIn[i * 6 + 3] = temps[i * 3 + 2];
		Direct.vertsIn[i * 6 + 4] = temps[i * 3 + 2];
		Direct.vertsIn[i * 6 + 5] = temps[i * 3 + 0];
	}

	for (int i = 0; i < 11;i++)
	{
		DirectXStuff::vertex temp;
		temp.Color.x = 1.0f;
		temp.Color.y = 1.0f;
		temp.Color.z = 1.0f;
		temp.Color.w = 1.0f;
		temp.Position.w = 1.0f;
		temp.Position.y = 0;

		temp.Position.x = -250;
		temp.Position.z = (i*100 - 500)/2;
		Direct.vertsIn[(i*4)+(vSize*2)] = temp;
		temp.Position.x = 250;
		temp.Position.z = (i*100 - 500)/2;
		Direct.vertsIn[(i*4)+1+(vSize*2)] = temp;
		temp.Position.z = -250;
		temp.Position.x = (i*100 - 500)/2;
		Direct.vertsIn[(i*4)+2+(vSize*2)] = temp;
		temp.Position.z = 250;
		temp.Position.x = (i*100 - 500)/2;
		Direct.vertsIn[(i*4)+3+(vSize*2)] = temp;
	}

	if (bonesOn)
	{
		std::vector<RTA::Vertex> tempBone = stuff.getBones();
		Direct.boneSize = stuff.getBones().size();
		for (int i = (vSize*2)+44; i < (vSize*2)+44+tempBone.size(); i++)
		{
			//DirectXStuff::vertex temp;
			int hold = i - ((vSize * 2) + 44);
			Direct.vertsIn[i].Position.x = tempBone[hold].xyzw[0];
			Direct.vertsIn[i].Position.y = tempBone[hold].xyzw[1];
			Direct.vertsIn[i].Position.z = tempBone[hold].xyzw[2];
			Direct.vertsIn[i].Position.w = 1.0f;//tempBone[i-(vSize*2)].xyzw[3];
			Direct.vertsIn[i].Color.x = 0.0f;
			Direct.vertsIn[i].Color.y = 0.0f;
			Direct.vertsIn[i].Color.z = 1.0f;
			Direct.vertsIn[i].Color.w = 1.0f;

			//Direct.bonesHere.push_back(temp);
			//printf("Debug here - %0.4f, %0.4f, %0.4f\n", tempBone[i - (vSize * 2)].xyzw[0], tempBone[i - (vSize * 2)].xyzw[1], tempBone[i - (vSize * 2)].xyzw[2]);

		}
		printf("Number of bones in list - %d", stuff.getBones().size());
		//vSize += tempBone.size();
	}

	Direct.vSize = (vSize * 2)+44 + stuff.getBones().size();
	if (Direct.index == nullptr)
	{
		return -3;
	}
	return 1;
}

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
    LoadStringW(hInstance, IDC_RTACODYSTANKO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	
	//RTA::functions stuff;
	

	#ifndef NDEBUG
		AllocConsole();
		FILE* new_std_in_out;
		freopen_s(&new_std_in_out, "CONOUT$", "w", stdout);
		freopen_s(&new_std_in_out, "CONIN$", "r", stdin);
		std::cout << "Hello world!\n";
		std::cout << stuff.returnint();
		stuff.initFBX();
		int trythis = FbxToDirect();
		if (trythis <= 0)
			printf("ERROR: %d", trythis);
		else
			printf("DirectX has data!");
	#endif
		Direct.Start(hWnd, width, height);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RTACODYSTANKO));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		if (GetAsyncKeyState('R') & 0x001)
		{
			bonesOn = !bonesOn;
			Direct.bonesOn = !Direct.bonesOn;
			//FbxToDirect();
			printf("bones = %d", bonesOn);
		}
		if (GetKeyState(VK_RBUTTON) < 0)
		{
			POINT mouse;

			GetCursorPos(&mouse);
			ScreenToClient(hWnd, &mouse);

			mouseX = (float)(mouse.x) / width - 0.5f;
			mouseY = (float)(mouse.y) / height - 0.5f;

			Direct.UpdateCamera(mouseX, mouseY);

			mouse.x = width / 2;
			mouse.y = height / 2;
			ClientToScreen(hWnd, &mouse);

			SetCursorPos(mouse.x, mouse.y);
		}
		
		Direct.Render();
    }

#ifndef NDEBUGd
	FreeConsole();
#endif

	//RTA::functions::deletestuff();
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RTACODYSTANKO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_RTACODYSTANKO);
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

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
	   CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr);

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
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}