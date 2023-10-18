#include "DirectSwapChain.h"
#include "DirectDevice.h"
#include "DirectCommandQueue.h"
#include "DirectHelper.h"
#include "Enums.h"
#include "Event.h"

DirectSwapChain::DirectSwapChain(DirectDevice& device) :
	r_device{ device }
{
    auto callback = [this](ScreenResizeMessage msg) { this->SetResolution(msg); };
    Event<ScreenResizeMessage>::Subscribe(callback);
    CreateSwapChain();
}

void DirectSwapChain::CreateSwapChain()
{
    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = SwapChainBufferCount;
    swapChainDesc.Width = m_width;
    swapChainDesc.Height = m_height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;
    ThrowIfFailed(r_device.GetNativeFactory()->CreateSwapChainForHwnd(
        r_device.GetCommandQueue(EQueueType::Graphics).GetNativeQueue().Get(),        // Swap chain needs the queue so that it can force a flush on it.
        r_device.GetMainWindowHandle(),
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain
    ));

    ThrowIfFailed(swapChain.As(&m_swapChain));
    m_currentFrameIndex = m_swapChain->GetCurrentBackBufferIndex();
}

void DirectSwapChain::SetResolution(ScreenResizeMessage msg)
{
    m_width = msg.newWidth;
    m_height = msg.newHeight;
}