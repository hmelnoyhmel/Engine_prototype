#include "DirectResource.h"

#include "DirectCommandList.h"
#include "DirectDevice.h"
#include "DirectHelper.h"

DirectResource::DirectResource(
    DirectDevice& device,
    const D3D12_RESOURCE_DESC& resourceDesc,
    const std::wstring name,
    D3D12_HEAP_TYPE heapType,
    D3D12_HEAP_FLAGS heapFlags,
    D3D12_RESOURCE_STATES initState
)
    : device{ device }
{
    ThrowIfFailed(device.GetNativeDevice()->CreateCommittedResource1(
        &CD3DX12_HEAP_PROPERTIES(heapType), //&heapProp
        heapFlags,
        &resourceDesc,
        initState,
        nullptr,
        nullptr,
        IID_PPV_ARGS(nativeResource.GetAddressOf())
    ));

    SetName(name);
}

/*
void DirectResource::ChangeState(DirectCommandList& cmdList, STATE_TYPE STATE_NAME)
{
    const auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(),
        STATE_BEFORE, STATE_NAME);

    cmdList.GetNativeList()->ResourceBarrier(1, barrier);
}
*/