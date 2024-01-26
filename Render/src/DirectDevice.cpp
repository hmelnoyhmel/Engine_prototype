#include "DirectDevice.h"
#include "DirectHelper.h"
#include "DirectCommandQueue.h"

void DirectDevice::CreateDevice()
{
    unsigned int dxgiFactoryFlags = 0;
#if defined(_DEBUG) || defined (DEBUG)
    // Enable the debug layer (requires the Graphics Tools "optional feature").
    // NOTE: Enabling the debug layer after device creation will invalidate the active device.
    {
        ComPtr<ID3D12Debug> debugController;
        ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
        debugController->EnableDebugLayer();
        // Enable additional debug layers.
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
#endif

    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&nativeFactory)));

    // Try to create hardware device.
    HRESULT const hardwareResult = D3D12CreateDevice(
        nullptr,             // default adapter
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&nativeDevice));

    // Fallback to WARP device.
    if (FAILED(hardwareResult))
    {
        ComPtr<IDXGIAdapter> warpAdapter;
        ThrowIfFailed(nativeFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

        ThrowIfFailed(D3D12CreateDevice(
            warpAdapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&nativeDevice)));
    }

#ifdef _DEBUG
    LogAdapters();
#endif

	ThrowIfFailed(nativeDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&nativeFence)));

	nativeFenceEvent = CreateEvent(nullptr, false, false, nullptr);

}

void DirectDevice::LogAdapters()
{
	UINT i = 0;
	IDXGIAdapter* adapter = nullptr;
	std::vector<IDXGIAdapter*> adapterList;
	while (nativeFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		OutputDebugString(text.c_str());

		adapterList.push_back(adapter);

		++i;
	}

	for (const auto& loggedAdapter : adapterList)
	{
		LogAdapterOutputs(loggedAdapter);
		SAFE_RELEASE(loggedAdapter);
	}

}

void DirectDevice::LogAdapterOutputs(IDXGIAdapter* adapter)
{
	UINT i = 0;
	IDXGIOutput* output = nullptr;
	while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"\n";
		OutputDebugString(text.c_str());

		LogOutputDisplayModes(output, m_BackBufferFormat);

		SAFE_RELEASE(output);

		++i;
	}
}

void DirectDevice::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{
	unsigned int count = 0;
	unsigned int constexpr flags = 0;

	// Call with nullptr to get list count.
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	for (const auto& x : modeList)
	{
		unsigned int const n = x.RefreshRate.Numerator;
		unsigned int const d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
			L"\n";

		OutputDebugString(text.c_str());
	}
}


DirectCommandQueue& DirectDevice::GetCommandQueue(EQueueType type)
{
	auto const queue = queues.find(type);
	if (queue != queues.end())
		return *queue->second;

	queues[type] = std::make_shared<DirectCommandQueue>(*this, type);
	return *queues[type];
}

DirectCommandList DirectDevice::GetCommandList(EQueueType type)
{
	return GetCommandQueue(type).GetCommandList();
}