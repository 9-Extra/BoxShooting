#include "Graphics.h"

void Graphcis::clear(Color color) {
	std::fill(panel.begin(), panel.end(), color);
}
void Graphcis::draw_point(Vector2f p, Color color) {
	int center_x = (int)(p.x / MAP_RATIO);
	int center_y = (int)(p.y / MAP_RATIO);
	if (center_x >= 0 && center_x < WINDOW_WIDTH && center_y >= 0 && center_y < WINDOW_HEIGHT) {
		panel[center_y * WINDOW_WIDTH + center_x] = color;
	}
}
void Graphcis::draw_box(Vector2f p, int width, int height, Color color) {
	//debug_log("Drawing box at (%f, %f)\n", x, y);
	int center_x = (int)(p.x / MAP_RATIO);
	int center_y = (int)(p.y / MAP_RATIO);
	int x_start = std::max<int>(0, center_x - width / 2);
	int y_start = std::max<int>(0, center_y - height / 2);
	int x_end = std::min<int>(WINDOW_WIDTH - 1, center_x + width / 2);
	int y_end = std::min<int>(WINDOW_HEIGHT - 1, center_y + height / 2);
	for (int y = y_start; y < y_end; y++) {
		for (int x = x_start; x < x_end; x++) {
			panel[y * WINDOW_WIDTH + x] = color;
		}
	}

}

void Graphcis::draw_texture_upleft(Vector2f p, const Texture& texture) {
	int left = (int)(p.x / MAP_RATIO);
	int up = (int)(p.y / MAP_RATIO);

	//todo
}

void Graphcis::draw_text_upleft(int up, int left, Color color,const wchar_t* str) {
	int pen_x = up;
	int pen_y = left;
	for (const wchar_t* c = str; *c != L'\0'; c++) {
		FT_GlyphSlot slot = font_engine.load_char(*c);
		unsigned int width = slot->bitmap.width;
		unsigned int height = slot->bitmap.rows;
		
		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				int tar_x = pen_x + x;
				int tar_y = pen_y + y;
				if (tar_x >= 0 && tar_x < WINDOW_WIDTH && tar_y >= 0 && tar_y < WINDOW_HEIGHT) {
					unsigned char gray = slot->bitmap.buffer[y * width + x];
					if (gray > 0) {
						Color& ori = panel[tar_y * WINDOW_WIDTH + tar_x];
						color.a = gray;
						ori = ori.blend(color);
					}
				}

			}
		}
		
		pen_x += slot->advance.x / 64;
		pen_y += slot->advance.y;

	}
}

void Graphcis::draw_texture_center(Vector2f p, const Texture& texture) {
#ifndef NDEBUG
	if (texture.pixels.size() == 0) {
		GameError(L"Use unloaded texture");
	}
#endif // !NDEBUG
	unsigned int width = texture.width;
	unsigned int height = texture.height;
	int center_x = (int)(p.x / MAP_RATIO);
	int center_y = (int)(p.y / MAP_RATIO);
	
	int tar_x_start = center_x - width / 2;
	int tar_y_start = center_y - height / 2;

	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			int tar_x = tar_x_start + x;
			int tar_y = tar_y_start + y;
			if (tar_x >= 0 && tar_x < WINDOW_WIDTH && tar_y >= 0 && tar_y < WINDOW_HEIGHT) {
				Color pixel = texture.pixels[y * width + x];
				if (pixel.a != 0) {
					panel[tar_y * WINDOW_WIDTH + tar_x] = pixel;
				}
			}
	
		}
	}


}

void Graphcis::init_pipeline()
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
	swapChainDesc.BufferDesc.Width = display.width;
	swapChainDesc.BufferDesc.Height = display.height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.OutputWindow = display.hwnd;
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

	frame_index = p_swapchain->GetCurrentBackBufferIndex();

	// This sample does not support fullscreen transitions.
	factory->MakeWindowAssociation(display.hwnd, DXGI_MWA_NO_ALT_ENTER);

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

void Graphcis::init_assests()
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

	{
		winrt::com_ptr<ID3DBlob> vertexShader;
		winrt::com_ptr<ID3DBlob> pixelShader;

		D3DReadFileToBlob(L"resources\\shaders\\VSPass.cso", vertexShader.put());
		D3DReadFileToBlob(L"resources\\shaders\\PSSingleColor.cso", pixelShader.put());

		// Define the vertex input layout.
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = p_root_signature.get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.get());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;

		p_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(p_pipeline_state.put()));
	}

	p_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, p_command_allocator.get(), NULL, IID_PPV_ARGS(p_command_list.put()));
	p_command_list->Close();

	p_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(p_fence.put()));
	fence_value = 1;
	fence_event = CreateEventW(nullptr, FALSE, FALSE, nullptr);

	CD3DX12_HEAP_PROPERTIES heap_desc(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resource_desc = CD3DX12_RESOURCE_DESC::Buffer(display.width * display.height * sizeof(Color));
	p_device->CreateCommittedResource(&heap_desc, D3D12_HEAP_FLAG_CREATE_NOT_ZEROED, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&p_upload_buffer));
}

void Graphcis::render_panel()
{
	p_command_allocator->Reset();

	// However, when ExecuteCommandList() is called on a particular command 
	// list, that command list can then be reset at any time and must be before 
	// re-recording.
	p_command_list->Reset(p_command_allocator.get(), NULL);

	if (true) {
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(rendertargets[frame_index].get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_COPY_DEST);
		p_command_list->ResourceBarrier(1, &barrier);

		//CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(p_rtvHeap->GetCPUDescriptorHandleForHeapStart(), frame_index, rtvDescriptorSize);
		//p_command_list->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		ID3D12Resource* back_buffer;
		if (FAILED(p_swapchain->GetBuffer(frame_index, IID_PPV_ARGS(&back_buffer)))) {
			SysError(NULL);
		}

		CD3DX12_TEXTURE_COPY_LOCATION dst_location(back_buffer, 0);
		D3D12_PLACED_SUBRESOURCE_FOOTPRINT subresource = {
			.Offset = 0,
			.Footprint = {
				.Format = DXGI_FORMAT_R8G8B8A8_UINT,
				.Width = display.width,
				.Height = display.height,
				.Depth = 1u,
				.RowPitch = display.width * sizeof(Color)
			}
		};
		CD3DX12_TEXTURE_COPY_LOCATION src_location(p_upload_buffer.get(), subresource);

		p_command_list->CopyTextureRegion(&dst_location, 0, 0, 0, &src_location, nullptr);

		back_buffer->Release();

		// Record commands.
		//const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		//p_command_list->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		p_command_list->SetPipelineState(p_pipeline_state.get());


		// Indicate that the back buffer will now be used to present.
		barrier = CD3DX12_RESOURCE_BARRIER::Transition(rendertargets[frame_index].get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_RENDER_TARGET);
		p_command_list->ResourceBarrier(1, &barrier);
	}
}

void Graphcis::model_render_context()
{
	p_command_list->SetGraphicsRootSignature(p_root_signature.get());
	p_command_list->RSSetViewports(1, &viewport);
	p_command_list->RSSetScissorRects(1, &scissorRect);


	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(p_rtvHeap->GetCPUDescriptorHandleForHeapStart(), frame_index, rtvDescriptorSize);
	p_command_list->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	// Record commands.
	//const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	//p_command_list->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	p_command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphcis::model_render_end()
{
	// Indicate that the back buffer will now be used to present.
	p_command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rendertargets[frame_index].get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}

void Graphcis::wait_till_next_frame()
{
	unsigned int fence = fence_value++;
	p_command_queue->Signal(p_fence.get(), fence);

	if (p_fence->GetCompletedValue() < fence) {
		p_fence->SetEventOnCompletion(fence, fence_event);
		WaitForSingleObject(fence_event, INFINITE);
	}
}

void Graphcis::swap() {
	Color* data;
	p_upload_buffer->Map(0, nullptr, (void**)&data);
	memcpy(data, panel.data(), display.width * display.height * sizeof(Color));
	p_upload_buffer->Unmap(0, nullptr);

	p_command_list->Close();

	ID3D12CommandList* ppCommandLists[] = { p_command_list.get() };
	p_command_queue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	if (FAILED(p_swapchain->Present(1, 0))) {
		SysError(NULL);
	};

	wait_till_next_frame();

	frame_index = p_swapchain->GetCurrentBackBufferIndex();
}
