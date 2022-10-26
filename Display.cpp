#include "Display.h"

#include "GameDesc.h"
#include <windowsx.h>

#include "InputHandler.h"

static const DWORD WINDOW_STYLE = WS_CAPTION | WS_SYSMENU;

static LRESULT WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
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

Display::Display(unsigned int width, unsigned int height)
	:width(width), height(height)
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

	HRESULT hr;

	const DXGI_SWAP_CHAIN_DESC scd = {
		.BufferDesc = DXGI_MODE_DESC{
			.Width = width,
			.Height = height,
			.RefreshRate = DXGI_RATIONAL{.Numerator = 0u, .Denominator = 0u},
			.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_CENTERED,
		},
		.SampleDesc = DXGI_SAMPLE_DESC{
			.Count = 1,
			.Quality = 0
		},
		.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 2,
		.OutputWindow = hwnd,
		.Windowed = TRUE,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
		.Flags = 0
	};

	UINT device_flag = 0;
#ifndef NDEBUG
	device_flag = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif // !NDEBUG

	hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		device_flag,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		p_swapchain.put(),
		p_device.put(),
		NULL,
		p_context.put()
	);

	if (FAILED(hr)) {
		SysError(L"Failed to create device");
	}

	ShowWindow(hwnd, SW_SHOW);
}

void Display::swap(Color* panel) {


	ID3D11Texture2D* back_buffer;
	if (FAILED(p_swapchain->GetBuffer(0, IID_PPV_ARGS(&back_buffer)))) {
		SysError(NULL);
	}
	
	p_context->UpdateSubresource(back_buffer, 0, NULL, panel, WINDOW_WIDTH * sizeof(Color), WINDOW_HEIGHT * sizeof(Color));

	if (FAILED(p_swapchain->Present(1, 0))) {
		SysError(NULL);
	};

	back_buffer->Release();
}
