#pragma once
#include <d3d11.h>
#include <dxgi1_6.h>
#include <winrt/base.h>

#include "GameDesc.h"
#include "CGmath.h"
#include "Error.h"

class Display {
private:
	HWND hwnd;
	winrt::com_ptr<ID3D11Device> p_device;
	winrt::com_ptr<ID3D11DeviceContext> p_context;
	winrt::com_ptr<IDXGISwapChain> p_swapchain;

	unsigned int width;
	unsigned int height;

public:
	Display(unsigned int width, unsigned int height);

	void swap(Color* panel);

};
