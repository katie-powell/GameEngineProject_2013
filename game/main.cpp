// Game.cpp is the main entry point of the gmae
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../engine/graphics/GraphicsDevice.h"
#include "../engine/game/GameWorld.h"

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		return 0;

	case WM_ACTIVATE:
		if(wParam == WA_INACTIVE)
		{
			ITP485::GameWorld::get().SetPaused(true);
		}
		else
		{
			ITP485::GameWorld::get().SetPaused(false);
		}
		return 0;

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"ITP485 Game", NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow(L"ITP485 Game", L"ITP485 Game Engine",
		WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768,
		NULL, NULL, wc.hInstance, NULL);

	// Setup our GameWorld and GraphicsDevice singletons
	ITP485::GraphicsDevice::get().Setup(hWnd);
	ITP485::GameWorld::get().Setup();
	ITP485::GameWorld::get().LoadLevel("level.ini");

	// Show the window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	LARGE_INTEGER freq, beforeCount, afterCount;
	QueryPerformanceFrequency(&freq); // Store frequency in freq
	QueryPerformanceCounter(&beforeCount); // Store counter in count

	

	// enter the main game loop
	bool bQuit = false;
	while (!bQuit)
	{
		QueryPerformanceCounter(&afterCount);
		QueryPerformanceFrequency(&freq);
		// Update the game world based on delta time
		ITP485::GameWorld::get().Update(((float)(afterCount.QuadPart-beforeCount.QuadPart))/freq.QuadPart);
		//ITP485::GameWorld::get().Update(1.0f);
		beforeCount.QuadPart = afterCount.QuadPart;

		// Render this frame
		ITP485::GraphicsDevice::get().Render();

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				bQuit = true;
				break;
			}
		}
	}

	// Cleanup the GameWorld and GraphicsDevice singletons
	ITP485::GameWorld::get().Cleanup();
	ITP485::GraphicsDevice::get().Cleanup();

	UnregisterClass(L"ITP485 Game", wc.hInstance);
	return 0;
}
