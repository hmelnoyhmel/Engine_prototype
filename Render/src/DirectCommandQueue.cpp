#include "DirectCommandQueue.h"
#include "DirectDevice.h"
#include "DirectHelper.h"
#include "Enums.h"

DirectCommandQueue::DirectCommandQueue(DirectDevice& device, EQueueType type) :
	r_device{ device }
{
	switch (type)
	{
	case EQueueType::Graphics:
		CreateGraphicsQueue();
		break;
	case EQueueType::Compute:
		CreateComputeQueue();
		break;
	}

}

void DirectCommandQueue::CreateGraphicsQueue()
{
	D3D12_COMMAND_QUEUE_DESC graphicsQueueDesc = {};
	graphicsQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	graphicsQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(r_device.GetNativeDevice()->CreateCommandQueue(&graphicsQueueDesc, IID_PPV_ARGS(&m_queue)));
}

void DirectCommandQueue::CreateComputeQueue()
{
	D3D12_COMMAND_QUEUE_DESC computeQueueDesc = {};
	computeQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	computeQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(r_device.GetNativeDevice()->CreateCommandQueue(&computeQueueDesc, IID_PPV_ARGS(&m_queue)));
}