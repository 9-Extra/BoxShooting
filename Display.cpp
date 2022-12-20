#include "Display.h"

#include "GameDesc.h"
#include <windowsx.h>

#include "InputHandler.h"

static const DWORD WINDOW_STYLE = WS_CAPTION | WS_SYSMENU;

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

void Display::init_pipeline()
{
#ifndef NDEBUG
	// Enable the D3D12 debug layer.
	{
		winrt::com_ptr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
		}
	}
#endif

	winrt::com_ptr<IDXGIFactory4> factory;
	CreateDXGIFactory1(IID_PPV_ARGS(&factory));

	
	winrt::com_ptr<IDXGIAdapter1> hardwareAdapter;
	factory->EnumAdapters1(0, hardwareAdapter.put());
	DXGI_ADAPTER_DESC adapter_desc;
	hardwareAdapter->GetDesc(&adapter_desc);
	debug_log("%ws\n", adapter_desc.Description);

	D3D12CreateDevice(
		hardwareAdapter.get(),
		D3D_FEATURE_LEVEL_12_0,
		IID_PPV_ARGS(p_device.put())
	);
	

	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	p_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(p_command_queue.put()));

	// Describe and create the swap chain.
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	winrt::com_ptr<IDXGISwapChain> swapChain;
	factory->CreateSwapChain(
		p_command_queue.get(),        // Swap chain needs the queue so that it can force a flush on it.
		&swapChainDesc,
		swapChain.put()
	);

	swapChain.as(p_swapchain);

	// This sample does not support fullscreen transitions.
	factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

	//unsigned int m_frameIndex = p_swapchain->GetCurrentBackBufferIndex();

	// Create descriptor heaps.
	{
		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = FrameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		p_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(p_rtvHeap.put()));

		rtvDescriptorSize = p_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	// Create frame resources.
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(p_rtvHeap->GetCPUDescriptorHandleForHeapStart());

		// Create a RTV for each frame.
		for (UINT n = 0; n < FrameCount; n++)
		{
			p_swapchain->GetBuffer(n, IID_PPV_ARGS(rendertargets[n].put()));
			p_device->CreateRenderTargetView(rendertargets[n].get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, rtvDescriptorSize);
		}
	}

	p_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&p_command_allocator));
}

void Display::init_assests()
{
	// Create an empty root signature.
	{
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		winrt::com_ptr<ID3DBlob> signature;
		winrt::com_ptr<ID3DBlob> error;
		D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, signature.put(), error.put());
		p_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(p_root_signature.put()));
	}
	
	p_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, p_command_allocator.get(), NULL, IID_PPV_ARGS(p_command_list.put()));
	p_command_list->Close();

	p_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(p_fence.put()));
	fence_value = 1;
	fence_event = CreateEventW(nullptr, FALSE, FALSE, nullptr);

	CD3DX12_HEAP_PROPERTIES heap_desc(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resource_desc = CD3DX12_RESOURCE_DESC::Buffer(width * height * sizeof(Color));
	p_device->CreateCommittedResource(&heap_desc, D3D12_HEAP_FLAG_CREATE_NOT_ZEROED, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&p_upload_buffer));
}

void Display::populate_command_list()
{
	// Command list allocators can only be reset when the associated 
	// command lists have finished execution on the GPU; apps should use 
	// fences to determine GPU execution progress.
	p_command_allocator->Reset();

	// However, when ExecuteCommandList() is called on a particular command 
	// list, that command list can then be reset at any time and must be before 
	// re-recording.
	p_command_list->Reset(p_command_allocator.get(), NULL);


	// Indicate that the back buffer will be used as a render target.
	unsigned int frame_index = p_swapchain->GetCurrentBackBufferIndex();

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(rendertargets[frame_index].get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_COPY_DEST);
	p_command_list->ResourceBarrier(1, &barrier);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(p_rtvHeap->GetCPUDescriptorHandleForHeapStart(), frame_index, rtvDescriptorSize);
	p_command_list->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	ID3D12Resource* back_buffer;
	if (FAILED(p_swapchain->GetBuffer(frame_index, IID_PPV_ARGS(&back_buffer)))) {
		SysError(NULL);
	}

	CD3DX12_TEXTURE_COPY_LOCATION dst_location(back_buffer, 0);
	D3D12_TEXTURE_COPY_LOCATION src_location = {
		.pResource = p_upload_buffer.get(),
		.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT,
		.PlacedFootprint = {
			.Offset = 0,
			.Footprint = {
				.Format = DXGI_FORMAT_R8G8B8A8_UINT,
				.Width = width,
				.Height = height,
				.Depth = 1u,
				.RowPitch = width * sizeof(Color)
			}
		}
	};

	p_command_list->CopyTextureRegion(&dst_location, 0, 0, 0, &src_location, nullptr);

	back_buffer->Release();

	// Record commands.
	//const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	//p_command_list->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);


	// Indicate that the back buffer will now be used to present.
	barrier = CD3DX12_RESOURCE_BARRIER::Transition(rendertargets[frame_index].get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT);
	p_command_list->ResourceBarrier(1, &barrier);

	p_command_list->Close();

}

void Display::wait_till_next_frame()
{
	unsigned int fence = fence_value++;
	p_command_queue->Signal(p_fence.get(), fence);

	if (p_fence->GetCompletedValue() < fence) {
		p_fence->SetEventOnCompletion(fence, fence_event);
		WaitForSingleObject(fence_event, INFINITE);
	}
}

Display::Display(unsigned int width, unsigned int height)
	:width(width), height(height)
{

	init_window();
	init_pipeline();
	init_assests();
	wait_till_next_frame();

	ShowCursor(false);
	ShowWindow(hwnd, SW_SHOW);
}

void Display::swap(Color* panel) {
	Color* data;
	p_upload_buffer->Map(0, nullptr, (void**) & data);
	memcpy(data, panel, width * height * sizeof(Color));
	p_upload_buffer->Unmap(0, nullptr);

	populate_command_list();

	ID3D12CommandList* ppCommandLists[] = { p_command_list.get() };
	p_command_queue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	if (FAILED(p_swapchain->Present(1, 0))) {
		SysError(NULL);
	};

	wait_till_next_frame();
}
