#pragma once
#include <wrl.h>
#include <d3d12.h>

using Microsoft::WRL::ComPtr;

class DirectDevice;
class DirectCommandQueue;

class DirectCommandList
{
private:
	ComPtr<ID3D12GraphicsCommandList6> m_list;

	DirectDevice& r_device;
	DirectCommandQueue& r_queue;

public:
	DirectCommandList(DirectDevice& device, DirectCommandQueue& queue);
	inline ComPtr<ID3D12GraphicsCommandList6> GetNativeList() const { return m_list; }

	void FlushCmdList() const;
};
