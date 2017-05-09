#include "stdafx.h"
#include "DirectXStuff.h"


DirectXStuff::DirectXStuff()
{
	m_pipelineState.m_depthStencilBuffer = nullptr;
	m_pipelineState.m_depthStencilState = nullptr;
	m_pipelineState.m_depthStencilView = nullptr;
	m_pipelineState.m_inputlayout = nullptr;
	m_pipelineState.m_rasterState = nullptr;
	m_pipelineState.m_vertexshader = nullptr;
	m_pipelineState.m_pixelshader = nullptr;

	m_attribute.m_device = nullptr;
	m_attribute.m_devicecontext = nullptr;
	m_attribute.m_rendertarget = nullptr;
	m_attribute.m_swapchain = nullptr;
}

DirectXStuff::~DirectXStuff()
{
	if (m_pipelineState.m_depthStencilBuffer != nullptr)
		m_pipelineState.m_depthStencilBuffer->Release();

	if (m_pipelineState.m_depthStencilState != nullptr)
		m_pipelineState.m_depthStencilState->Release();

	if (m_pipelineState.m_depthStencilView != nullptr)
		m_pipelineState.m_depthStencilView->Release();

	if (m_pipelineState.m_inputlayout != nullptr)
		m_pipelineState.m_inputlayout->Release();

	if (m_pipelineState.m_vertexshader != nullptr)
		m_pipelineState.m_vertexshader->Release();

	if (m_pipelineState.m_pixelshader != nullptr)
		m_pipelineState.m_pixelshader->Release();

	if (m_pipelineState.m_rasterState != nullptr)
		m_pipelineState.m_rasterState->Release();

	if (m_attribute.m_device != nullptr)
		m_attribute.m_device->Release();

	if (m_attribute.m_devicecontext)
		m_attribute.m_devicecontext->Release();

	if (m_attribute.m_rendertarget != nullptr)
		m_attribute.m_rendertarget->Release();

	if (m_attribute.m_swapchain != nullptr)
		m_attribute.m_swapchain->Release();
}

void DirectXStuff::Start(HWND window, int width, int height)
{
	m_pipelineState.m_depthStencilBuffer = nullptr;
	m_pipelineState.m_depthStencilState = nullptr;
	m_pipelineState.m_depthStencilView = nullptr;
	m_pipelineState.m_inputlayout = nullptr;
	m_pipelineState.m_rasterState = nullptr;
	m_pipelineState.m_vertexshader = nullptr;
	m_pipelineState.m_pixelshader = nullptr;

	m_attribute.m_device = nullptr;
	m_attribute.m_devicecontext = nullptr;
	m_attribute.m_rendertarget = nullptr;
	m_attribute.m_swapchain = nullptr;

	UINT deviceFlags = 0;

#ifdef _DEBUG
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureArray[] = {
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT featureLvlAmount = ARRAYSIZE(featureArray);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = true;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceFlags, featureArray,
		featureLvlAmount, D3D11_SDK_VERSION, &swapChainDesc, &m_attribute.m_swapchain, &m_attribute.m_device,
		&m_attribute.m_featurelevels, &m_attribute.m_devicecontext);

	ID3D11Texture2D * renderTargetTemp = 0;
	m_attribute.m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTargetTemp);
	m_attribute.m_device->CreateRenderTargetView(renderTargetTemp, nullptr, &m_attribute.m_rendertarget);
	renderTargetTemp->Release();

	m_attribute.m_devicecontext->OMSetRenderTargets(1, &m_attribute.m_rendertarget, nullptr);

	ZeroMemory(&m_attribute.m_viewport, sizeof(D3D11_VIEWPORT));
	m_attribute.m_viewport.Width = width;
	m_attribute.m_viewport.Height = height;
	m_attribute.m_viewport.MaxDepth = 1.0f;
	m_attribute.m_viewport.MinDepth = 0.0f;
	m_attribute.m_viewport.TopLeftX = 0;
	m_attribute.m_viewport.TopLeftY = 0;

	m_attribute.m_devicecontext->RSSetViewports(1, &m_attribute.m_viewport);

	SetUpShader();
	BufferTriangle();
	XMStoreFloat4x4(&m_matrix.world, XMMatrixTranspose(DirectX::XMMatrixIdentity()));
	SetCamera();
}

void DirectXStuff::Render()
{
	//if (GetAsyncKeyState('S') & 0x001)
	//{
	//	Randomize();
	//}
	m_attribute.m_devicecontext->UpdateSubresource(m_bufferMatrix, 0, NULL, &m_matrix, 0, 0);
	m_attribute.m_devicecontext->VSSetConstantBuffers(0, 1, &m_bufferMatrix);
	m_attribute.m_devicecontext->ClearRenderTargetView(m_attribute.m_rendertarget, DirectX::Colors::Black);
	
	m_attribute.m_devicecontext->PSSetShader(m_pipelineState.m_pixelshader, NULL, 0);
	m_attribute.m_devicecontext->VSSetShader(m_pipelineState.m_vertexshader, NULL, 0);

	UINT stride = sizeof(vertex);
	UINT offset = 0;
	m_attribute.m_devicecontext->IASetVertexBuffers(0, 1, &m_pipelineState.m_buffer, &stride, &offset);

	m_attribute.m_devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	if (bonesOn == true)
	{
		m_attribute.m_devicecontext->Draw(vSize, 0);
	}
	else
	{
		m_attribute.m_devicecontext->Draw(vSize - boneSize, 0);
	}
	//// select which vertex buffer to display
	//UINT stride = sizeof(vertex);
	//UINT offset = 0;
	////// select which primtive type we are using
	//m_attribute.m_devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	////// draw the vertex buffer to the back buffer
	//m_attribute.m_devicecontext->IASetVertexBuffers(0, 1, &m_bufferXline, &stride, &offset);
	//m_attribute.m_devicecontext->Draw(2, 0);

	//m_attribute.m_devicecontext->IASetVertexBuffers(0, 1, &m_bufferYline, &stride, &offset);
	//m_attribute.m_devicecontext->Draw(2, 0);

	//m_attribute.m_devicecontext->IASetVertexBuffers(0, 1, &m_bufferZline, &stride, &offset);
	//m_attribute.m_devicecontext->Draw(2, 0);

	m_attribute.m_swapchain->Present(0, 0);
}

void DirectXStuff::UpdateCamera(float _x, float _y)
{
	camrotX -= _y;
	camrotY -= _x;

	if (camrotX >= DirectX::XM_PIDIV2)
	{
		camrotX = DirectX::XM_PIDIV2 - 0.001;
	}
	if (camrotX <= -DirectX::XM_PIDIV2)
	{
		camrotX = -DirectX::XM_PIDIV2 + 0.001;
	}

	if (GetAsyncKeyState('W'))
	{
		cameraPos.Position.x += cos(camrotY)*cos(camrotX)*0.013f;
		cameraPos.Position.z += sin(camrotY)*cos(camrotX)*0.013f;
		cameraPos.Position.y += sin(camrotX)*0.013f;
	}
	if (GetAsyncKeyState('A'))
	{
		cameraPos.Position.z += cos(camrotY)*cos(camrotX)*0.013f;
		cameraPos.Position.x -= sin(camrotY)*cos(camrotX)*0.013f;
	}
	if (GetAsyncKeyState('S'))
	{
		cameraPos.Position.x -= cos(camrotY)*cos(camrotX)*0.013f;
		cameraPos.Position.z -= sin(camrotY)*cos(camrotX)*0.013f;
		cameraPos.Position.y -= sin(camrotX)*0.013f;
	}
	if (GetAsyncKeyState('D'))
	{
		cameraPos.Position.z -= cos(camrotY)*cos(camrotX)*0.013f;
		cameraPos.Position.x += sin(camrotY)*cos(camrotX)*0.013f;
	}

	if (GetAsyncKeyState(VK_SPACE))
	{
		cameraPos.Position.y += 0.013f;
	}
	if (GetAsyncKeyState(VK_LSHIFT))
	{
		cameraPos.Position.y -= 0.013f;
	}

	const DirectX::XMVECTORF32 eye = { cameraPos.Position.x, cameraPos.Position.y, cameraPos.Position.z, 0 };
	const DirectX::XMVECTORF32 at = { cos(camrotY)*cos(camrotX) + cameraPos.Position.x, 
									sin(camrotX) + cameraPos.Position.y,
									sin(camrotY)*cos(camrotX) + cameraPos.Position.z, 0 };
	const DirectX::XMVECTORF32 up = { 0,1,0,0 };
	DirectX::XMMATRIX look = DirectX::XMMatrixLookAtLH(eye, at, up);
	DirectX::XMStoreFloat4x4(&m_matrix.view, DirectX::XMMatrixTranspose(look));

	float aspectRatio = 500 / 500;
	float fovAngleY = 70.0f * 3.14f / 180.0f;

	DirectX::XMMATRIX projj = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 1000.0f);
	XMStoreFloat4x4(&m_matrix.projection, XMMatrixTranspose(projj));
}

//int DirectXStuff::SetThings(int * index, float _vert0, float _vert1, float _vert2, float _vert3)
//{
//	
//	return -1;
//}

void DirectXStuff::SetUpShader()
{
	m_attribute.m_device->CreatePixelShader(PixelShader, sizeof(PixelShader), NULL, &m_pipelineState.m_pixelshader);
	m_attribute.m_device->CreateVertexShader(VertexShader, sizeof(VertexShader), NULL, &m_pipelineState.m_vertexshader);

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_attribute.m_device->CreateInputLayout(ied, 2, VertexShader, sizeof(VertexShader), &m_pipelineState.m_inputlayout);
	m_attribute.m_devicecontext->IASetInputLayout(m_pipelineState.m_inputlayout);
}

void DirectXStuff::BufferTriangle()
{
	
#pragma region oldtriangle
	/*
	vertex verts[] = {
		{ DirectX::XMFLOAT4(0,.5f,0,1), DirectX::XMFLOAT4(1,0,0,1) },
		{ DirectX::XMFLOAT4(0.5f,-.5f,0,1), DirectX::XMFLOAT4(0,1,0,1) },
		{ DirectX::XMFLOAT4(-0.5f,-.5f,0,1), DirectX::XMFLOAT4(0,0,1,1) },
	};

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(vertex) * 3;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	m_attribute.m_device->CreateBuffer(&bufferDesc, NULL, &m_pipelineState.m_buffer);

	D3D11_MAPPED_SUBRESOURCE mapSub;
	m_attribute.m_devicecontext->Map(m_pipelineState.m_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
	memcpy(mapSub.pData, verts, sizeof(verts));
	m_attribute.m_devicecontext->Unmap(m_pipelineState.m_buffer, NULL);

	vertex OurVerticesx[] =
	{
		{ DirectX::XMFLOAT4(0.0f,0.0f,0.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.5f,0.0f,0.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }
	};

	vertex OurVerticesy[] =
	{
		{ DirectX::XMFLOAT4(0.0f,0.0f,0.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.0f,0.5f,0.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }
	};

	vertex OurVerticesz[] =
	{
		{ DirectX::XMFLOAT4(0.0f,0.0f,0.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.0f,0.0f,-0.5f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC bufferDesc2;
	ZeroMemory(&bufferDesc2, sizeof(bufferDesc2));

	bufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc2.ByteWidth = sizeof(vertex) * 2;
	bufferDesc2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subResX;
	subResX.pSysMem = OurVerticesx;
	subResX.SysMemPitch = 0;
	subResX.SysMemSlicePitch = 0;

	D3D11_SUBRESOURCE_DATA subResY;
	subResY.pSysMem = OurVerticesy;
	subResY.SysMemPitch = 0;
	subResY.SysMemSlicePitch = 0;

	D3D11_SUBRESOURCE_DATA subResZ;
	subResZ.pSysMem = OurVerticesz;
	subResZ.SysMemPitch = 0;
	subResZ.SysMemSlicePitch = 0;

	m_attribute.m_device->CreateBuffer(&bufferDesc2, &subResX, &m_bufferXline);
	m_attribute.m_device->CreateBuffer(&bufferDesc2, &subResY, &m_bufferYline);
	m_attribute.m_device->CreateBuffer(&bufferDesc2, &subResZ, &m_bufferZline);

	CD3D11_BUFFER_DESC bdm(sizeof(m_matrix), D3D11_BIND_CONSTANT_BUFFER);
	m_attribute.m_device->CreateBuffer(&bdm, NULL, &m_bufferMatrix);
	*/
#pragma endregion
	//for (int i = 0; &vertsIn[i+1]; i++)


	if (vertsIn)
	{
		D3D11_BUFFER_DESC bufDesc;
		ZeroMemory(&bufDesc, sizeof(bufDesc));

		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.ByteWidth = sizeof(vertex)*vSize;
		bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		m_attribute.m_device->CreateBuffer(&bufDesc, NULL, &m_pipelineState.m_buffer);

		D3D11_MAPPED_SUBRESOURCE mapSub;
		m_attribute.m_devicecontext->Map(m_pipelineState.m_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
		memcpy(mapSub.pData, vertsIn, vSize*sizeof(vertex));
		m_attribute.m_devicecontext->Unmap(m_pipelineState.m_buffer, NULL);

		CD3D11_BUFFER_DESC bdm(sizeof(m_matrix), D3D11_BIND_CONSTANT_BUFFER);
		m_attribute.m_device->CreateBuffer(&bdm, NULL, &m_bufferMatrix);
	}

	if (bonesHere.size() > 0)
	{

	}
}

void DirectXStuff::SetCamera()
{
	DirectX::XMVECTORF32 eye = { 0.5f, 0.7f, -1.5f, 0.0f };
	DirectX::XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_matrix.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));

	float aspectRatio = 500 / 500;
	float fovAngleY = 70.0f * 3.14f / 180.0f;

	DirectX::XMMATRIX projj = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 1000.0f);
	XMStoreFloat4x4(&m_matrix.projection, XMMatrixTranspose(projj));
	cameraPos.Position.x = 0.5f;
	cameraPos.Position.y = 0.7f;
	cameraPos.Position.z = -1.5f;
	cameraPos.Position.w = 1.0f;
}

void DirectXStuff::Randomize()
{
	DirectX::XMMATRIX model = DirectX::XMLoadFloat4x4(&m_matrix.world);

	float rX = (float)rand() / RAND_MAX * 2.0f - 1.0f;
	float rY = (float)rand() / RAND_MAX * 2.0f - 1.0f;
	float rZ = (float)rand() / RAND_MAX * 2.0f - 1.0f;

	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(rX, rY, rZ);

	DirectX::XMMATRIX rotatex = DirectX::XMMatrixRotationX((rand() % 360)*DirectX::XM_PI / 180.0f);
	DirectX::XMMATRIX rotatey = DirectX::XMMatrixRotationY((rand() % 360)*DirectX::XM_PI / 180.0f);
	DirectX::XMMATRIX rotatez = DirectX::XMMatrixRotationZ((rand() % 360)*DirectX::XM_PI / 180.0f);

	model = rotatex;
	model *= rotatey;
	model *= rotatez;
	model *= trans;
	DirectX::XMStoreFloat4x4(&m_matrix.world, DirectX::XMMatrixTranspose(model));

}

void DirectXStuff::DrawTriangle()
{
	m_attribute.m_devicecontext->PSSetShader(m_pipelineState.m_pixelshader, NULL, 0);
	m_attribute.m_devicecontext->VSSetShader(m_pipelineState.m_vertexshader, NULL, 0);

	UINT stride = sizeof(vertex);
	UINT offset = 0;
	m_attribute.m_devicecontext->IASetVertexBuffers(0, 1, &m_pipelineState.m_buffer, &stride, &offset);

	m_attribute.m_devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_attribute.m_devicecontext->Draw(3, 0);
}