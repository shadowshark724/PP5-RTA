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
int width = 1000;
int height = 1000;
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
RTA::anim_clip temp_clip;
bool bonesOn = true;
float mouseX, mouseY;
int bonemove = 0;
bool timeon = true;

int wrapFrame(int curr)
{
	if (curr == -1)
	{
		return temp_clip.frames.size() - 1;
	}
	if (curr == temp_clip.frames.size())
	{
		return 0;
	}
	else
		return curr;
}

int FbxToDirect()
{
	if (Direct.vertsIn.size() > 0)
	{
		Direct.vertsIn.clear();
	}
	int iSize = stuff.getIndex().size();
	int vSize = stuff.getVertex().size();
	if (iSize <= 0)
	{
		return -1;
	}
	if (vSize <= 0)
	{
		return -2;
	}
	//Direct.index = new int[iSize];
	/*for (int i = 0; i < iSize; i++)
	{
		Direct.index.push_back(stuff.getIndex()[i]);
	}*/
	//Direct.vertsIn.resize((vSize * 2) + 44 + stuff.getBones().size()); //= new DirectXStuff::vertex[(vSize*2)+44+ stuff.getBones().size()];
	//RTA::Vertex * fbxVert = stuff.getVertex();
	std::vector<DirectXStuff::vertex> temps;// = new DirectXStuff::vertex[vSize];
	Direct.index = stuff.getIndex();
	//Direct.vertsIn = new DirectXStuff::vertex[vSize];
	
	for (int i = 0; i < 11; i++)
	{
		DirectXStuff::vertex temp;
		temp.Color.x = 1.0f;
		temp.Color.y = 1.0f;
		temp.Color.z = 1.0f;
		temp.Color.w = 1.0f;
		temp.Position.w = 1.0f;
		temp.Position.y = 0;

		temp.Position.x = -250;
		temp.Position.z = (i * 100 - 500) / 2;
		Direct.grid.push_back(temp);
		temp.Position.x = 250;
		temp.Position.z = (i * 100 - 500) / 2;
		Direct.grid.push_back(temp);
		temp.Position.z = -250;
		temp.Position.x = (i * 100 - 500) / 2;
		Direct.grid.push_back(temp);
		temp.Position.z = 250;
		temp.Position.x = (i * 100 - 500) / 2;
		Direct.grid.push_back(temp);
	}
	
	if (Direct.vertsIn.size() == 0)
	{
		std::vector<RTA::Vertex> rvert = stuff.getVertex();
		for (int i = 0; i < vSize; i++)
		{
			DirectXStuff::vertex temp;
			temp.Position.x = rvert[i].xyzw[0];
			temp.Position.y = rvert[i].xyzw[1];
			temp.Position.z = rvert[i].xyzw[2];
			temp.Position.w = 1.0f;
			temp.Color.x = rvert[i].color[0];
			temp.Color.y = rvert[i].color[1];
			temp.Color.z = rvert[i].color[2];
			temp.Color.w = rvert[i].color[3];
			temp.Normal.x = rvert[i].norm[0];
			temp.Normal.y = rvert[i].norm[1];
			temp.Normal.z = rvert[i].norm[2];
			temp.Normal.w = rvert[i].norm[3];
			Direct.vertsIn.push_back(temp);
			//temps.push_back(temp);
		//	printf("Debug here - %0.2f, %0.2f, %0.2f\n", Direct.vertsIn[i].Position.x, Direct.vertsIn[i].Position.y, Direct.vertsIn[i].Position.z);
		}
	}
	//Direct.vertsIn.resize(vSize);
	//for (int i = 0; i < vSize / 3; i++)
	//{
	//	//DirectXStuff::vertex temp1,temp2,temp3;
	//	Direct.vertsIn[i * 6 + 0] = temps[i * 3 + 0]; 
	//	Direct.vertsIn[i * 6 + 1] = temps[i * 3 + 1];
	//	Direct.vertsIn[i * 6 + 2] = temps[i * 3 + 1];
	//	Direct.vertsIn[i * 6 + 3] = temps[i * 3 + 2];
	//	Direct.vertsIn[i * 6 + 4] = temps[i * 3 + 2];
	//	Direct.vertsIn[i * 6 + 5] = temps[i * 3 + 0];
	//}

	if (bonesOn)
	{
		Direct.bonesHere.clear();
		for (int k = 0; k < temp_clip.frames.size(); k++)
		{
			std::vector<RTA::Vertex> tempBone = temp_clip.frames[k].joints; //stuff.getAnim().frames[bonemove].joints;//stuff.getBones();
			//Direct.boneSize = stuff.getBones().size();
			std::vector<DirectXStuff::vertex> vtemp;
			for (int i = 0; i < tempBone.size(); i++)
			{
				//DirectXStuff::vertex temp;
				//int hold = i - ((vSize * 2) + 44);
				DirectXStuff::vertex temp;
				temp.Position.x = tempBone[i].xyzw[0];
				temp.Position.y = tempBone[i].xyzw[1];
				temp.Position.z = tempBone[i].xyzw[2];
				temp.Position.w = 1.0f;//tempBone[i-(vSize*2)].xyzw[3];
				temp.Color.x = 0.0f;
				temp.Color.y = 0.0f;
				temp.Color.z = 1.0f;
				temp.Color.w = 1.0f;
				vtemp.push_back(temp);
				//Direct.bonesHere.push_back(temp);
				//Direct.bonesHere.push_back(temp);
				//printf("Debug here - %0.4f, %0.4f, %0.4f\n", tempBone[i - (vSize * 2)].xyzw[0], tempBone[i - (vSize * 2)].xyzw[1], tempBone[i - (vSize * 2)].xyzw[2]);
			}
			Direct.boneanims.push_back(vtemp);
			printf("Number of bones in list - %d", stuff.getBones().size());
			//vSize += tempBone.size();
			//Direct.boneSize = tempBone.size();
		}
	}

	//Direct.vSize = (vSize * 2) + 44 + stuff.getBones().size();//(vSize * 2)+44 + stuff.getBones().size();
	if (Direct.vertsIn.size() <= 0)
	{
		return -4;
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
	#endif
		temp_clip = stuff.getAnim();
		int trythis = FbxToDirect();
		/*if (trythis <= 0)
			printf("ERROR: %d", trythis);
		else
			printf("DirectX has data!");*/
		Direct.Start(hWnd, width, height);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RTACODYSTANKO));

    MSG msg;

	std::chrono::time_point<std::chrono::system_clock> lastnow;
	lastnow = std::chrono::system_clock::now();
	float time = 0;
	//float dTime = 0;
	float fpsupdate = 0;
	int key = 0;
	int framecount = 0;
	float lasttime = 0;
    // Main message loop:
	bool run = true;

	while (run)
	{
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (msg.message == WM_QUIT)
				run = false;
		}

		time += (std::chrono::system_clock::now() - lastnow).count() * 0.00000001f;
		//time += dTime;
		lastnow = std::chrono::system_clock::now();
		//lasttime = temp_clip.frames[wrapFrame(bonemove-1)].time;
		
		if (timeon == true)
		{
			float ltime = temp_clip.frames[temp_clip.frames.size() - 1].time;

			float mtime = fmod(time, ltime);

			int frame = 0;


			for (int i = 0; i < Direct.boneanims.size(); i++)
			{
				if (temp_clip.frames[i].time > mtime)
				{
					frame = i;
					break;
				}
			}
			int prevFrame = wrapFrame(frame - 1);

			float timeDiff = temp_clip.frames[frame].time - temp_clip.frames[prevFrame].time;

			float timeIn = mtime - temp_clip.frames[prevFrame].time;

			if (frame == 0)
			{
				timeDiff = temp_clip.frames[frame].time;
				timeIn = mtime;
			}

			float ratio = timeIn / timeDiff;

			Direct.bonetemp.clear();
			for (int i = 0; i < temp_clip.frames[prevFrame].joints.size(); i++)
			{
				RTA::Vertex temp1, temp2;
				DirectXStuff::vertex temp3;
				temp1 = temp_clip.frames[prevFrame].joints[i];
				temp2 = temp_clip.frames[frame].joints[i];

				temp3.Position.x = (temp2.xyzw[0] - temp1.xyzw[0])*ratio + temp1.xyzw[0];
				temp3.Position.y = (temp2.xyzw[1] - temp1.xyzw[1])*ratio + temp1.xyzw[1];
				temp3.Position.z = (temp2.xyzw[2] - temp1.xyzw[2])*ratio + temp1.xyzw[2];
				temp3.Position.w = 1.0f;
				temp3.Color = { 1.0f,1.0f,0.0f,1.0f };
				Direct.bonetemp.push_back(temp3);
			}
		}
		if (fpsupdate < time)
		{
			fpsupdate = time + 1;
			printf("FPS - %d", framecount);
			framecount = 0;
		}
		framecount++;
		if (GetAsyncKeyState('T') & 0x001)
		{	
			timeon = !timeon;
			if (timeon == false)
			{
				Direct.bonetemp.clear();
				for (int i = 0; i < temp_clip.frames[bonemove].joints.size(); i++)
				{
					DirectXStuff::vertex temp;
					temp.Position.x = temp_clip.frames[bonemove].joints[i].xyzw[0];
					temp.Position.y = temp_clip.frames[bonemove].joints[i].xyzw[1];
					temp.Position.z = temp_clip.frames[bonemove].joints[i].xyzw[2];
					temp.Position.w = 1.0f;
					temp.Color = { 1.0f,0.0f,0.0f,0.0f };
					Direct.bonetemp.push_back(temp);
				}
			}
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
		//printf("current frame : %d", bonemove);

			//Direct.animFrame = bonemove;
			Direct.update = true;
//FbxToDirect();
		

		if (GetAsyncKeyState(VK_LEFT) & 0x001 && timeon == false)
		{
		if (bonemove - 1 < 0)
		{
			bonemove = stuff.getAnim().frames.size() - 1;
		}
		else
			bonemove--;
		Direct.bonetemp.clear();
		for (int i = 0; i < temp_clip.frames[bonemove].joints.size(); i++)
		{
			DirectXStuff::vertex temp;
			temp.Position.x = temp_clip.frames[bonemove].joints[i].xyzw[0];
			temp.Position.y = temp_clip.frames[bonemove].joints[i].xyzw[1];
			temp.Position.z = temp_clip.frames[bonemove].joints[i].xyzw[2];
			temp.Position.w = 1.0f;
			temp.Color = { 1.0f,1.0f,0.0f,0.0f };
			Direct.bonetemp.push_back(temp);
		}
		Direct.update = true;
		//FbxToDirect();
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x001 && timeon == false)
		{
			if (bonemove + 1 == stuff.getAnim().frames.size())
			{
				bonemove = 0;
			}
			else
				bonemove++;
			Direct.bonetemp.clear();
			for (int i = 0; i < temp_clip.frames[bonemove].joints.size(); i++)
			{
				DirectXStuff::vertex temp;
				temp.Position.x = temp_clip.frames[bonemove].joints[i].xyzw[0];
				temp.Position.y = temp_clip.frames[bonemove].joints[i].xyzw[1];
				temp.Position.z = temp_clip.frames[bonemove].joints[i].xyzw[2];
				temp.Position.w = 1.0f;
				temp.Color = { 1.0f,1.0f,0.0f,0.0f };
				Direct.bonetemp.push_back(temp);
			}
			Direct.update = true;
		//	FbxToDirect();
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
	wcex.lpszMenuName   = NULL;// MAKEINTRESOURCEW(IDC_RTACODYSTANKO);
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