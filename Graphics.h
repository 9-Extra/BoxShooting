#pragma once

#include <directx/d3dx12.h>
#include <dxgi1_6.h>
#include <winrt/base.h>
#include <d3dcompiler.h>

#include "FontEngine.h"
#include <vector>
#include "CGmath.h"
#include "GameDesc.h"
#include <algorithm>
#include "Error.h"
#include "Display.h"

struct Texture {
	std::vector<Color> pixels;
	unsigned int width;
	unsigned int height;
};

class Graphcis {
private:
	static const unsigned int FrameCount = 2;

	Display display;

	std::vector<Color> panel;
	FontEngine font_engine;

public:
	winrt::com_ptr<ID3D12Device> p_device;
	winrt::com_ptr<ID3D12GraphicsCommandList> p_command_list;
	winrt::com_ptr<ID3D12CommandQueue> p_command_queue;
	winrt::com_ptr<ID3D12CommandAllocator> p_command_allocator;

	void model_render_context();
	void model_render_end();
	void render_panel();

	void wait_till_next_frame();
private:

	winrt::com_ptr<ID3D12DescriptorHeap> p_rtvHeap;
	winrt::com_ptr<IDXGISwapChain4> p_swapchain;

	winrt::com_ptr<ID3D12RootSignature> p_root_signature;
	winrt::com_ptr<ID3D12Fence1> p_fence;

	winrt::com_ptr<ID3D12Resource> p_upload_buffer;

	CD3DX12_VIEWPORT viewport;
	CD3DX12_RECT scissorRect;
	winrt::com_ptr<ID3D12PipelineState> p_pipeline_state;
	winrt::com_ptr<ID3D12Resource> rendertargets[FrameCount];

	unsigned int rtvDescriptorSize;

	unsigned int fence_value;
	HANDLE fence_event;

	unsigned int frame_index;

	void init_pipeline();
	void init_assests();

public:

	Graphcis(unsigned int width, unsigned int height):
		display(width, height),
		panel(WINDOW_WIDTH * WINDOW_HEIGHT),
		viewport(0.0f, 0.0f, (float)width, (float)height),
		scissorRect(0, 0, width, height)
	{
		init_pipeline();
		init_assests();
		wait_till_next_frame();
	}

	void swap();

	void clear(Color color);
	void draw_point(Vector2f p, Color color);
	void draw_box(Vector2f p, int width, int height, Color color);
	void draw_texture_center(Vector2f p, const Texture& texture);
	void draw_texture_upleft(Vector2f p, const Texture& texture);

	void draw_text_upleft(int up, int left, Color color,const wchar_t* str);

};
