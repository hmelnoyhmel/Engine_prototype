#include "DirectMesh.h"

#include <DirectXColors.h>

#include "DirectCommandList.h"
#include "DirectHelper.h"
#include "DirectConstantBuffer.h"
#include "DirectDevice.h"
#include "DirectIndexBuffer.h"
#include "DirectVertexBuffer.h"
#include "DirectPSO.h"

// add arrays of vertices and indices as args
DirectMesh::DirectMesh(DirectDevice& device, DirectCommandList& list)
{
	vertices =
	{
		Vertex({ Vector3(-1.0f, -1.0f, -1.0f), Vector4(DirectX::Colors::White) }),
		Vertex({ Vector3(-1.0f, +1.0f, -1.0f), Vector4(DirectX::Colors::Black) }),
		Vertex({ Vector3(+1.0f, +1.0f, -1.0f), Vector4(DirectX::Colors::Red) }),
		Vertex({ Vector3(+1.0f, -1.0f, -1.0f), Vector4(DirectX::Colors::Green) }),
		Vertex({ Vector3(-1.0f, -1.0f, +1.0f), Vector4(DirectX::Colors::Blue) }),
		Vertex({ Vector3(-1.0f, +1.0f, +1.0f), Vector4(DirectX::Colors::Yellow) }),
		Vertex({ Vector3(+1.0f, +1.0f, +1.0f), Vector4(DirectX::Colors::Cyan) }),
		Vertex({ Vector3(+1.0f, -1.0f, +1.0f), Vector4(DirectX::Colors::Magenta) }),
	};

	indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7,


		10, 8, 9,
		10, 9, 11
	};

	vertexBuffer = std::make_shared<DirectVertexBuffer>(device, L"vertexBuffer", vertices.size(), sizeof(Vertex));
	indexBuffer = std::make_shared<DirectIndexBuffer>(device, L"indexBuffer", indices.size());

	vertexBuffer->LoadData(device.GetNativeDevice(), list.GetNativeList(), vertices.data());
	indexBuffer->LoadData(device.GetNativeDevice(), list.GetNativeList(), indices.data());

	CompileShaders();

	std::shared_ptr<DirectPSO> pso = std::make_shared<DirectPSO>(device, vsByteCode, psByteCode, inputLayout);
	psos.push_back(pso);

	objectCB = std::make_shared<DirectConstantBuffer>(device, L"objectCB", sizeof(ObjectData), 1, 0);
	cameraCB = std::make_shared<DirectConstantBuffer>(device, L"cameraCB", sizeof(CameraData), 1, 1);

	
}


void DirectMesh::Draw(DirectCommandList& list)
{
	auto cmdList = list.GetNativeList();

	// attach everything
	cmdList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (const auto& pso : psos)
		pso->Attach(cmdList);
	vertexBuffer->Attach(cmdList);
	indexBuffer->Attach(cmdList);

	objectCB->Attach(cmdList);
	cameraCB->Attach(cmdList);


	cmdList->DrawIndexedInstanced(54, 1, 0, 0, 0);
}


void DirectMesh::CompileShaders()
{
	D3D_SHADER_MACRO Shader_Macros_Wireframe_On[] = { "WIREFRAME", "1", NULL, NULL };
	D3D_SHADER_MACRO Shader_Macros_Wireframe_Off[] = { "WIREFRAME", "0", NULL, NULL };

	vsByteCode = CompileShader(L"C:\\Engine_Prototype\\Engine\\src\\Shaders\\shaders.hlsl", Shader_Macros_Wireframe_Off, "VS", "vs_5_1");
	psByteCode = CompileShader(L"C:\\Engine_Prototype\\Engine\\src\\Shaders\\shaders.hlsl", Shader_Macros_Wireframe_Off, "PS", "ps_5_1");
	//WireframePixelShaderCode = d3dUtil::CompileShader(L"Shaders\\color.hlsl", Shader_Macros_Wireframe_On, "PS", "ps_5_0");

	inputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}


void DirectMesh::Update(DirectCommandList& list)
{
	// OBJECT DATA
	ObjectData objData;
	auto scaleMatrix = Matrix::CreateScale(scale);
	Vector3 objectRotation(
		DirectX::XMConvertToRadians(eulerRotation.x),
		DirectX::XMConvertToRadians(eulerRotation.y),
		DirectX::XMConvertToRadians(eulerRotation.z)
	);
	auto rotation = Matrix::CreateFromYawPitchRoll(objectRotation);
	auto translation = Matrix::CreateTranslation(position);

	objData.World = scaleMatrix * rotation * translation;

	// CAMERA DATA
	CameraData camData;
	Vector3 cameraTranslation(position - Vector3::Forward * 5);
	float fov = 45;
	const float fovRadians = DirectX::XMConvertToRadians(fov);
	float aspectRatio = 1.0f;
	float nearZ = 0.01f;
	float farZ = 10000.0f;

	auto focusPosition = Vector3::Forward + cameraTranslation;

	Matrix view = DirectX::XMMatrixLookAtRH(cameraTranslation, focusPosition, Vector3::Up);
	Matrix proj = DirectX::XMMatrixPerspectiveFovRH(fovRadians, aspectRatio, nearZ, farZ);
	auto viewProj = (view * proj);
	auto invView = view.Invert();
	auto invProj = proj.Invert();
	auto invViewProj = viewProj.Invert();

	camData.View = view.Transpose();
	camData.InvView = invView.Transpose();
	camData.Proj = proj.Transpose();
	camData.InvProj = invProj.Transpose();
	camData.ViewProj = viewProj.Transpose();
	camData.InvViewProj = invViewProj.Transpose();

	objectCB->CopyData(0, objData);
	cameraCB->CopyData(0, camData);

	eulerRotation.y += 0.1f;

}