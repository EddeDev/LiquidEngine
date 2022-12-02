#include "LiquidPCH.h"

#ifdef LQ_PLATFORM_WINDOWS

#include "Liquid/Core/SplashScreen/SplashScreen.h"

namespace Liquid {

	namespace Utils {

		// From https://stackoverflow.com/a/17387176
		String GetLastErrorAsString()
		{
			DWORD errorMessageID = ::GetLastError();
			if (errorMessageID == 0)
				return String();

			LPSTR messageBuffer = NULL;
			size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

			String message(messageBuffer, size);
			LocalFree(messageBuffer);
			return message;
		}

	}

	struct WindowsSplashScreenData
	{
		uint32 Width = 1440 / 2;
		uint32 Height = 720 / 2;

		HANDLE ThreadHandle = NULL;
		DWORD ThreadID = NULL;
		ATOM WindowClass = NULL;
		HWND WindowHandle = NULL;
	};

	static WindowsSplashScreenData s_Data;

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		HDC hdc;
		PAINTSTRUCT ps;

		switch (uMsg)
		{
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);

			RECT r;
			SetRect(&r, 0, 0, s_Data.Width, s_Data.Height);
			FillRect(hdc, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));

			DrawTextW(hdc, L"Hello World!", -1, &r, DT_SINGLELINE | DT_NOCLIP);

			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProcW(hWnd, uMsg, wParam, lParam);
		}

		return 0;
	}
	
	static DWORD WINAPI StartThread(LPVOID param)
	{
		HINSTANCE hInstance = GetModuleHandleW(NULL);

		WNDCLASSEXW wc = {};
		wc.cbSize = sizeof(WNDCLASSEXW);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInstance;
		wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
		wc.hIcon = (HICON)LoadImageW(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
		wc.lpszClassName = L"LiquidSplash";

		s_Data.WindowClass = RegisterClassExW(&wc);
		LQ_VERIFY(s_Data.WindowClass, "Failed to register window class\n{0}", Utils::GetLastErrorAsString());

		DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		style |= WS_SYSMENU | WS_MINIMIZEBOX;
		style |= WS_POPUP;

		const bool showInTaskbar = false;
		DWORD exStyle = showInTaskbar ? WS_EX_APPWINDOW : WS_EX_TOOLWINDOW;

		RECT rect = { 0, 0, static_cast<LONG>(s_Data.Width), static_cast<LONG>(s_Data.Height) };
		AdjustWindowRectEx(&rect, style, FALSE, exStyle);

		uint32 x = (GetSystemMetrics(SM_CXSCREEN) / 2) - (s_Data.Width / 2);
		uint32 y = (GetSystemMetrics(SM_CYSCREEN) / 2) - (s_Data.Height / 2);

		s_Data.WindowHandle = CreateWindowExW(
			exStyle,
			MAKEINTATOM(s_Data.WindowClass),
			L"SplashScreen",
			style,
			x,
			y,
			s_Data.Width,
			s_Data.Height,
			NULL,
			NULL,
			hInstance,
			NULL
		);

		LQ_VERIFY(s_Data.WindowHandle, "Failed to create window\n{0}", Utils::GetLastErrorAsString());

		ShowWindow(s_Data.WindowHandle, SW_SHOWNA);
		// SetFocus(s_Data.WindowHandle);

		MSG message;
		bool isCloseRequested = false;
		while (!isCloseRequested)
		{
			if (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessageW(&message);

				if (message.message == WM_QUIT)
					isCloseRequested = true;
			}
		}

		UnregisterClassW(MAKEINTATOM(s_Data.WindowClass), hInstance);
		DestroyWindow(s_Data.WindowHandle);
		s_Data.WindowClass = NULL;
		s_Data.WindowHandle = NULL;
		return 0;
	}

	void SplashScreen::Show()
	{
		const SIZE_T stackSize = 1024 * 128; // 128 KB
		s_Data.ThreadHandle = CreateThread(NULL, stackSize, StartThread, NULL, STACK_SIZE_PARAM_IS_A_RESERVATION, &s_Data.ThreadID);
	}

	void SplashScreen::Hide()
	{
		if (s_Data.ThreadHandle)
		{
			if (s_Data.WindowHandle)
				PostMessageW(s_Data.WindowHandle, WM_DESTROY, NULL, NULL);

			WaitForSingleObject(s_Data.ThreadHandle, INFINITE);

			CloseHandle(s_Data.ThreadHandle);
			s_Data.ThreadHandle = NULL;
		}
	}

}

#endif