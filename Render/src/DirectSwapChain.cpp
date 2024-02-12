#include "DirectSwapChain.h"
#include "DirectDevice.h"
#include "DirectCommandQueue.h"
#include "DirectHelper.h"
#include "Event.h"

#include "DirectXColors.h"

DirectSwapChain::DirectSwapChain(DirectDevice& device, HWND hwnd, const unsigned int width, const unsigned int height) :
	device{ device }, hwnd{ hwnd }, width{ width }, height{ height }
{

    if (nativeSwapChain == nullptr)
    {
        // Describe and create the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

        swapChainDesc.Width = width;
        swapChainDesc.Height = height;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.Stereo = false;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = swapChainBufferCount;
        swapChainDesc.Scaling = DXGI_SCALING_NONE;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        ComPtr<IDXGISwapChain1> swapChain;
        ThrowIfFailed(device.GetNativeFactory()->CreateSwapChainForHwnd(
            device.GetCommandQueue(EQueueType::Graphics).GetNativeQueue().Get(), // Swap chain needs the queue so that it can force a flush on it.
            hwnd,
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain
        ));

        ThrowIfFailed(swapChain.As(&nativeSwapChain));
    }

}

void DirectSwapChain::Present() const
{
    nativeSwapChain->Present(1, 0);
}

void DirectSwapChain::Resize(const unsigned int newWidth, const unsigned int newHeight)
{
    width = newWidth;
    height = newHeight;

    // Flush the queue before resizing.
    device.GetCommandQueue(EQueueType::Graphics).FlushCmdQueue();

    // Resize the swap chain.
    ThrowIfFailed(nativeSwapChain->ResizeBuffers(
        swapChainBufferCount,
        width, height,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

    // DEBUG
    std::wstring text = L"Swapchain was resized successfully. New Size is: "
	+ std::to_wstring(width)
	+ L"x"
	+ std::to_wstring(height)
	+ L"\n";
    OutputDebugString(text.c_str());
}
