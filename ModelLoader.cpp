#include "ModelLoader.h"

void ModelLoader::model_from_memory(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const unsigned int model_id) {
	assert(indices.size() % 3 == 0);
	Model& model = mng.models[model_id];

	winrt::com_ptr<ID3D12Resource> upload_buffer;
	CD3DX12_RESOURCE_DESC resource_desc = CD3DX12_RESOURCE_DESC::Buffer(vertices.size() * sizeof(Vertex) + indices.size() * sizeof(unsigned int));
	graphics.p_device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_CREATE_NOT_ZEROED, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(upload_buffer.put()));

	unsigned char* ptr;
	if (FAILED(upload_buffer->Map(0u, nullptr, (void**)&ptr))) {
		SysError(NULL);
	}
	memcpy(ptr, vertices.data(), vertices.size() * sizeof(Vertex));
	memcpy(ptr + vertices.size() * sizeof(Vertex), indices.data(), indices.size() * sizeof(unsigned int));
	upload_buffer->Unmap(0u, nullptr);

	graphics.p_device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_CREATE_NOT_ZEROED, &resource_desc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(model.resource.put()));

	graphics.p_command_allocator->Reset();
	graphics.p_command_list->Reset(graphics.p_command_allocator.get(), NULL);

	graphics.p_command_list->CopyBufferRegion(model.resource.get(), 0u, upload_buffer.get(), 0u, vertices.size() * sizeof(Vertex) + indices.size() * sizeof(unsigned int));
	graphics.p_command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(model.resource.get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER));
	graphics.p_command_list->Close();

	ID3D12CommandList* ppCommandLists[] = { graphics.p_command_list.get() };
	graphics.p_command_queue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	graphics.wait_till_next_frame();

	model.index_count = indices.size();

	model.vertex_buffer_view.BufferLocation = model.resource->GetGPUVirtualAddress();
	model.vertex_buffer_view.SizeInBytes = (UINT)vertices.size() * sizeof(Vertex);
	model.vertex_buffer_view.StrideInBytes = (UINT)sizeof(Vertex);

	model.index_buffer_view.BufferLocation = model.vertex_buffer_view.BufferLocation + model.vertex_buffer_view.SizeInBytes;
	model.index_buffer_view.SizeInBytes = (UINT)indices.size() * sizeof(unsigned int);
	model.index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
}
