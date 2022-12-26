#pragma once

#include "directx/d3dx12.h"
#include <winrt/base.h>

class Model {
	friend class ModelLoader;

	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
	D3D12_INDEX_BUFFER_VIEW index_buffer_view;
	UINT index_count;

	winrt::com_ptr<ID3D12Resource> resource;

public:
	void draw(ID3D12GraphicsCommandList* command_list) const{
		command_list->IASetVertexBuffers(0u, 1u, &vertex_buffer_view);
		command_list->IASetIndexBuffer(&index_buffer_view);

		command_list->DrawIndexedInstanced(index_count, 1u, 0u, 0, 0u);
	}

};
