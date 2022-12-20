#pragma once
#include "directx/d3dx12.h"
#include <d3d11.h>
#include <dxgi1_6.h>
#include <winrt/base.h>

#include "GameDesc.h"
#include "CGmath.h"
#include "Error.h"

class Display {
private:
	static const unsigned int FrameCount = 2;

	HWND hwnd;
	winrt::com_ptr<ID3D12Device> p_device;
	winrt::com_ptr<ID3D12CommandQueue> p_command_queue;
	winrt::com_ptr<ID3D12CommandAllocator> p_command_allocator;
	winrt::com_ptr<ID3D12DescriptorHeap> p_rtvHeap;
	winrt::com_ptr<IDXGISwapChain4> p_swapchain;

	winrt::com_ptr<ID3D12RootSignature> p_root_signature;
	winrt::com_ptr<ID3D12GraphicsCommandList> p_command_list;
	winrt::com_ptr<ID3D12Fence1> p_fence;

	winrt::com_ptr<ID3D12Resource> p_upload_buffer;

	winrt::com_ptr<ID3D12Resource> rendertargets[FrameCount];

	unsigned int rtvDescriptorSize;

	unsigned int fence_value;
	HANDLE fence_event;

	unsigned int width;
	unsigned int height;

	void init_window();
	void init_pipeline();
	void init_assests();
	void populate_command_list();

	void wait_till_next_frame();

public:
	Display(unsigned int width, unsigned int height);
	~Display() {
		wait_till_next_frame();
	}

	void swap(Color* panel);

};
