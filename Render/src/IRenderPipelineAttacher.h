#pragma once

#include <d3d12.h>
using Microsoft::WRL::ComPtr;

class IRenderPipelineAttacher
{
public:
	virtual void Attach(ComPtr<ID3D12GraphicsCommandList6> list) = 0;
};