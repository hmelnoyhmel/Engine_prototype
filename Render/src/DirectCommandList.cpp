#include "DirectCommandList.h"
#include "DirectHelper.h"
#include "DirectCommandQueue.h"
#include "DirectDevice.h"

DirectCommandList::DirectCommandList(DirectDevice& device, DirectCommandQueue& queue) :
	r_device { device }, r_queue { queue }
{
	ThrowIfFailed(r_device.GetNativeDevice()->CreateCommandList1(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		D3D12_COMMAND_LIST_FLAG_NONE,
		IID_PPV_ARGS(&m_list)));

	// First close, then reset
	// CreateCommandList1 creates closed list

	FlushCmdList();

	// Debug
	std::wstring text = L"Command list was created successfully.\n";
	OutputDebugString(text.c_str());

}

void DirectCommandList::FlushCmdList() const
{
	ThrowIfFailed(r_queue.GetNativeAllocator()->Reset());
	ThrowIfFailed(m_list.Get()->Reset(r_queue.GetNativeAllocator().Get(), nullptr));
}

