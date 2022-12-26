#include "Display.h"

#include "GameDesc.h"
#include <windowsx.h>

#include "InputHandler.h"

static const DWORD WINDOW_STYLE = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch (Msg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE: {
		float xPos = GET_X_LPARAM(lParam) * MAP_RATIO;
		float yPos = GET_Y_LPARAM(lParam) * MAP_RATIO;
		//debug_log("Mouse: %f ,%f\n", xPos, yPos);
		bool l_button = wParam & MK_LBUTTON;
		bool r_button = wParam & MK_RBUTTON;
		input_handler.handle_mouse_move(xPos, yPos, l_button, r_button);
		return 0;
	}

	case WM_KEYDOWN: {
		input_handler.key_down(wParam);
		return 0;
	}

	case WM_KEYUP: {
		input_handler.key_up(wParam);
		return 0;
	}

	case WM_KILLFOCUS: {
		input_handler.clear_keyboard_state();
		input_handler.clear_mouse_state();
		break;
	}

	case WM_CLOSE: {
		DestroyWindow(hWnd);
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}

	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return true;
}

void Display::init_window()
{
	HINSTANCE hInstance = GetModuleHandleW(NULL);

	const WNDCLASSEXW window_class = {
		.cbSize = sizeof(WNDCLASSEXW),
		.style = CS_OWNDC,
		.lpfnWndProc = WindowProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hInstance,
		.hIcon = NULL,
		.hCursor = NULL,
		.hbrBackground = NULL,
		.lpszMenuName = NULL,
		.lpszClassName = L"MyWindow",
		.hIconSm = NULL
	};

	RegisterClassExW(&window_class);
	RECT rect = {
		.left = 100,
		.top = 100,
		.right = 100 + (LONG)width,
		.bottom = 100 + (LONG)height
	};
	AdjustWindowRectEx(&rect, WINDOW_STYLE, NULL, 0);

	hwnd = CreateWindowExW(
		0,
		L"MyWindow",
		WINDOW_TITLE,
		WINDOW_STYLE,
		0,
		0,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		hInstance,
		nullptr
	);

	if (hwnd == NULL) {
		SysError(NULL);
	}
}



Display::Display(unsigned int width, unsigned int height)
	:width(width), height(height)
{

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	init_window();

	ShowCursor(false);
	ShowWindow(hwnd, SW_SHOW);
}