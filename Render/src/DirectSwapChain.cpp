#include "DirectSwapChain.h"
#include "DirectDevice.h"
#include "DirectCommandQueue.h"
#include "DirectHelper.h"
#include "Event.h"

DirectSwapChain::DirectSwapChain(DirectDevice& device, HWND hwnd, unsigned int width, unsigned int height) :
	r_device{ device }, m_hwnd{ hwnd }, m_width{ width }, m_height{ height }
{

    if (m_swapChain == nullptr)
    {
        // Describe and create the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

        swapChainDesc.Width = m_width;
        swapChainDesc.Height = m_height;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.Stereo = false;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = SwapChainBufferCount;
        swapChainDesc.Scaling = DXGI_SCALING_NONE;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        ComPtr<IDXGISwapChain1> swapChain;
        ThrowIfFailed(r_device.GetNativeFactory()->CreateSwapChainForHwnd(
            r_device.GetCommandQueue(EQueueType::Graphics).GetNativeQueue().Get(), // Swap chain needs the queue so that it can force a flush on it.
            m_hwnd,
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain
        ));

        ThrowIfFailed(swapChain.As(&m_swapChain));
    }

}

void DirectSwapChain::Present()
{
    m_swapChain->Present(1, 0);
}

void DirectSwapChain::Resize(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;

    // Flush the queue before resizing.
    r_device.GetCommandQueue(EQueueType::Graphics).FlushCmdQueue();

    // Resize the swap chain.
    ThrowIfFailed(m_swapChain->ResizeBuffers(
        SwapChainBufferCount,
        width, height,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

    // Debug
    std::wstring text = L"Swapchain resized successfully. New Size is: "
	+ std::to_wstring(width)
	+ L"x"
	+ std::to_wstring(height)
	+ L"\n";
    OutputDebugString(text.c_str());
}
