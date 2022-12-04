#include "LiquidPCH.h"

#ifdef LQ_PLATFORM_WINDOWS

#include "Liquid/Core/SplashScreen/SplashScreen.h"

#include <stb_image.h>

namespace Liquid {

	enum class SplashScreenFontType : uint8
	{
		Normal = 0,
		Small,
		Title
	};

	struct WindowsSplashScreenData
	{
		bool ShowInTaskbar = false;
		bool AllowFading = true;

		HANDLE ThreadHandle = NULL;
		DWORD ThreadID = NULL;
		ATOM WindowClass = NULL;
		HWND WindowHandle = NULL;
		
		HBITMAP Bitmap = NULL;
		std::unordered_map<SplashScreenFontType, HFONT> Fonts;

		std::mutex Mutex;
		uint8 Progress = 0;
		String ProgressText = "Loading...";
	};

	static WindowsSplashScreenData s_Data;

	void SplashScreen::SetProgress(uint8 progress)
	{
		if (s_Data.ThreadHandle)
		{
			std::lock_guard<std::mutex> lock(s_Data.Mutex);

			if (s_Data.Progress != progress)
			{
				s_Data.Progress = progress;
				InvalidateRect(s_Data.WindowHandle, NULL, FALSE);
			}
		}
	}

	void SplashScreen::SetText(const String& text)
	{
		if (s_Data.ThreadHandle)
		{
			std::lock_guard<std::mutex> lock(s_Data.Mutex);

			if (s_Data.ProgressText != text)
			{
				s_Data.ProgressText = text;
				InvalidateRect(s_Data.WindowHandle, NULL, FALSE);
			}
		}
	}

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		HDC hdc;
		PAINTSTRUCT ps;

		switch (uMsg)
		{
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			DrawState(hdc, DSS_NORMAL, NULL, (LPARAM)s_Data.Bitmap, NULL, 0, 0, 0, 0, DST_BITMAP);

			{
				std::lock_guard<std::mutex> lock(s_Data.Mutex);

				SetBkColor(hdc, 0x00000000);
				SetBkMode(hdc, TRANSPARENT);
				SetTextAlign(hdc, TA_LEFT | TA_TOP | TA_NOUPDATECP);

				RECT clientRect;
				GetClientRect(hWnd, &clientRect);

				uint32 padding = 8;

				// Label
				WString label = L"Liquid Editor";
				SelectObject(hdc, s_Data.Fonts[SplashScreenFontType::Title]);
				SetTextColor(hdc, RGB(255, 255, 255));
				TextOut(hdc, padding, clientRect.bottom - 60, label.c_str(), static_cast<int32>(label.size()));

				// Version
				WString version = L"Liquid Editor v1.0";
				SetTextColor(hdc, RGB(160, 160, 160));
				SelectObject(hdc, s_Data.Fonts[SplashScreenFontType::Normal]);
				TextOut(hdc, padding, clientRect.bottom - 40, version.c_str(), static_cast<int32>(version.size()));

				// Progress
				WString progressText;
				progressText += std::to_wstring(s_Data.Progress);
				progressText += L"% - ";
				progressText += StringUtils::ToWideString(s_Data.ProgressText);
				SelectObject(hdc, s_Data.Fonts[SplashScreenFontType::Normal]);
				SetTextColor(hdc, RGB(160, 160, 160));
				TextOut(hdc, padding, clientRect.bottom - 20, progressText.c_str(), static_cast<int32>(progressText.size()));

				// Copyright
				WString copyright = L"Copyright ©  EddeDev.  All rights reserved.";
				SelectObject(hdc, s_Data.Fonts[SplashScreenFontType::Small]);
				SetTextColor(hdc, RGB(160, 160, 160));
				TextOut(hdc, clientRect.right - 150, clientRect.bottom - 18, copyright.c_str(), static_cast<int32>(copyright.size()));
			}

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

	static void RGBtoBGR(const uint8* src, uint8* dst, uint32 width, uint32 height, uint32 channels)
	{
		for (uint32 x = 0; x < width; x++)
		{
			for (uint32 y = 0; y < height; y++)
			{
				*dst++ = src[2];
				*dst++ = src[1];
				*dst++ = src[0];
				if (channels == 4)
					*dst++ = src[3];
				src += channels;
			}
		}
	}

	static HBITMAP LoadBitmapImage(const String& path)
	{
		int32 width, height, channels;
		uint8* rgbData = stbi_load(path.c_str(), &width, &height, &channels, STBI_default);
		if (!rgbData)
			return NULL;

		size_t dataSize = width * height * channels;
		uint8* bgrData = new uint8[dataSize];
		RGBtoBGR(rgbData, bgrData, width, height, channels);

		BITMAPINFO info = {};
		info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		info.bmiHeader.biWidth = (LONG)width;
		info.bmiHeader.biHeight = -(LONG)height;
		info.bmiHeader.biPlanes = 1;
		info.bmiHeader.biBitCount = 8 * channels;
		info.bmiHeader.biCompression = BI_RGB;

		HDC hdc = GetDC(NULL);
		HBITMAP bitmap = CreateCompatibleBitmap(hdc, width, height);
		SetDIBits(hdc, bitmap, 0, height, bgrData, &info, DIB_RGB_COLORS);

		delete[] bgrData;
		stbi_image_free(rgbData);

		return bitmap;
	}

	static HFONT AddFont(SplashScreenFontType type, uint32 size, bool bold = false)
	{
		if (s_Data.Fonts.find(type) != s_Data.Fonts.end())
		{
			LQ_WARNING_ARGS("Font already exists!");
			return s_Data.Fonts.at(type);
		}

		HFONT defaultFont = static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));
		LOGFONT font = {};
		GetObjectW(defaultFont, sizeof(LOGFONT), &font);

		font.lfHeight = size;
		font.lfQuality = CLEARTYPE_QUALITY;
		font.lfWeight = bold ? FW_BOLD : FW_DONTCARE;

		s_Data.Fonts[type] = CreateFontIndirect(&font);
		if (!s_Data.Fonts[type])
		{
			LQ_WARNING_ARGS("Using default font");
			s_Data.Fonts[type] = defaultFont;
		}

		return s_Data.Fonts[type];
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

		s_Data.Bitmap = LoadBitmapImage("Resources/Splash/Splash.bmp");
		if (s_Data.Bitmap)
		{
			BITMAP bitmap = {};
			GetObjectW(s_Data.Bitmap, sizeof(BITMAP), &bitmap);
			const uint32 windowWidth = bitmap.bmWidth;
			const uint32 windowHeight = bitmap.bmHeight;
			uint32 windowXPos = (GetSystemMetrics(SM_CXSCREEN) / 2) - (windowWidth / 2);
			uint32 windowYPos = (GetSystemMetrics(SM_CYSCREEN) / 2) - (windowHeight / 2);

			DWORD style = 0;
			if (s_Data.ShowInTaskbar)
				style |= WS_EX_APPWINDOW;
			else
				style |= WS_EX_TOOLWINDOW;
			if (s_Data.AllowFading)
				style |= WS_EX_LAYERED;

			s_Data.WindowHandle = CreateWindowExW(
				style,
				MAKEINTATOM(s_Data.WindowClass),
				L"SplashScreen",
				WS_POPUP,
				windowXPos,
				windowYPos,
				windowWidth,
				windowHeight,
				NULL,
				NULL,
				hInstance,
				NULL
			);

			if (s_Data.AllowFading)
				SetLayeredWindowAttributes(s_Data.WindowHandle, 0, 0, LWA_ALPHA);

			// Add fonts
			AddFont(SplashScreenFontType::Normal, 12);
			AddFont(SplashScreenFontType::Small, 11);
			AddFont(SplashScreenFontType::Title, 18, true);

			ShowWindow(s_Data.WindowHandle, SW_SHOW);
			UpdateWindow(s_Data.WindowHandle);

			uint64 timerFrequency;
			QueryPerformanceFrequency((LARGE_INTEGER*)&timerFrequency);

			uint64 timerValue;
			QueryPerformanceCounter((LARGE_INTEGER*)&timerValue);
			uint64 timerOffset = timerValue;

			BYTE currentAlphaByte = 0;

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

				if (s_Data.AllowFading && currentAlphaByte < 255)
				{
					QueryPerformanceCounter((LARGE_INTEGER*)&timerValue);
					float time = (float)(timerValue - timerOffset) / timerFrequency;

					BYTE newAlphaByte = BYTE(time * 500.0f);
					if (newAlphaByte < 0)
						newAlphaByte = 0;
					if (newAlphaByte > 255)
						newAlphaByte = 255;

					if (newAlphaByte != currentAlphaByte)
					{
						currentAlphaByte = newAlphaByte;
						SetLayeredWindowAttributes(s_Data.WindowHandle, 0, currentAlphaByte, LWA_ALPHA);
					}

					Sleep(0);
				}
				else
				{
					Sleep(1000 / 60);
				}
			}

			DeleteObject(s_Data.Bitmap);
		}

		// Delete fonts
		for (auto& [type, font] : s_Data.Fonts)
			DeleteObject(font);

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
		if (!s_Data.ThreadHandle)
		{
			LQ_ERROR_ARGS("Unable to create thread");
			return;
		}
		
		SetThreadDescription(s_Data.ThreadHandle, L"SplashScreen Thread");
		SetThreadPriority(s_Data.ThreadHandle, THREAD_PRIORITY_BELOW_NORMAL);
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