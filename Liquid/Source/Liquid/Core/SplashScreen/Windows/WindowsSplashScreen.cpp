#include "LiquidPCH.h"

#ifdef LQ_PLATFORM_WINDOWS

#include "WindowsSplashScreen.h"

namespace Liquid {

	// From https://stackoverflow.com/a/17387176
	String GetLastErrorAsString()
	{
		DWORD errorMessageID = ::GetLastError();
		if (errorMessageID == 0)
			return String();

		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		String message(messageBuffer, size);
		LocalFree(messageBuffer);
		return message;
	}

	WindowsSplashScreen::WindowsSplashScreen(const SplashScreenCreateInfo& createInfo)
	{
		HINSTANCE hInstance = GetModuleHandleW(NULL);

		WNDCLASSEXW wc = { sizeof(wc) };
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInstance;
		wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
		wc.hIcon = (HICON)LoadImageW(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
		wc.lpszClassName = L"LiquidSplash";

		m_WindowClass = RegisterClassExW(&wc);
		LQ_VERIFY(m_WindowClass, "Failed to register window class\n{0}", GetLastErrorAsString());

		DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		style |= WS_SYSMENU | WS_MINIMIZEBOX;
		// style |= WS_POPUP;

		DWORD exStyle = WS_EX_APPWINDOW;

		RECT rect = { 0, 0, createInfo.Width, createInfo.Height };
		AdjustWindowRectEx(&rect, style, FALSE, exStyle);

		uint32 x = (GetSystemMetrics(SM_CXSCREEN) / 2) - (createInfo.Width / 2);
		uint32 y = (GetSystemMetrics(SM_CYSCREEN) / 2) - (createInfo.Height / 2);

		m_HWND = CreateWindowExW(
			exStyle,
			MAKEINTATOM(m_WindowClass),
			L"",
			style,
			x,
			y,
			createInfo.Width,
			createInfo.Height,
			NULL,
			NULL,
			hInstance,
			NULL
		);

		LQ_VERIFY(m_HWND, "Failed to create window\n{0}", GetLastErrorAsString());

		SetPropW(m_HWND, L"Instance", this);

		ShowWindow(m_HWND, SW_SHOWNA);
		SetFocus(m_HWND);

		// UpdateWindow(m_HWND);

		while (true)
		{
			MSG msg;
			while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
	}

	WindowsSplashScreen::~WindowsSplashScreen()
	{

	}

	LRESULT CALLBACK WindowsSplashScreen::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WindowsSplashScreen* instance = static_cast<WindowsSplashScreen*>(GetPropW(hWnd, L"Instance"));
		if (!instance)
			return DefWindowProc(hWnd, uMsg, wParam, lParam);

		switch (uMsg)
		{
		case WM_PAINT:
		{
			static PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);

			RECT r;
			SetRect(&r, 10, 10, 100, 100);
			FillRect(hDC, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));

			EndPaint(hWnd, &ps);
			return 0;
		}
		}
		
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}

}

#endif