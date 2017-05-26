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
	m_pipelineState.m_buffer = nullptr;
	m_pipelineState.g_buffer = nullptr;
	m_pipelineState.m_indBuffer = nullptr;
	m_pipelineState.m_inputlayout = nullptr;

	m_attribute.m_device = nullptr;
	m_attribute.m_devicecontext = nullptr;
	m_attribute.m_rendertarget = nullptr;
	m_attribute.m_swapchain = nullptr;
	m_attribute.m_sampler = nullptr;
	m_attribute.m_resource = nullptr;
	m_attribute.m_shaderResource = nullptr;
	b_attribute = m_attribute;
	b_pipelineState = m_pipelineState;

	debugthis = nullptr;
}

DirectXStuff::~DirectXStuff()
{

	if (m_pipelineState.m_vertexshader != nullptr)
		m_pipelineState.m_vertexshader->Release();

	if (m_pipelineState.m_inputlayout != nullptr)
		m_pipelineState.m_inputlayout->Release();

	if (m_pipelineState.m_pixelshader != nullptr)
		m_pipelineState.m_pixelshader->Release();

	if (m_pipelineState.m_depthStencilView != nullptr)
		m_pipelineState.m_depthStencilView->Release();

	if (m_pipelineState.m_depthStencilState != nullptr)
		m_pipelineState.m_depthStencilState->Release();

	if (m_pipelineState.m_depthStencilBuffer != nullptr)
		m_pipelineState.m_depthStencilBuffer->Release();

	if (m_pipelineState.m_rasterState != nullptr)
		m_pipelineState.m_rasterState->Release();

	if (m_pipelineState.d_rasterState != nullptr)
		m_pipelineState.d_rasterState->Release();

	if (m_pipelineState.m_buffer != nullptr)
		m_pipelineState.m_buffer->Release();

	if (m_pipelineState.g_buffer != nullptr)
		m_pipelineState.g_buffer->Release();

	if (m_pipelineState.m_indBuffer != nullptr)
		m_pipelineState.m_indBuffer->Release();

	if (m_attribute.m_device != nullptr)
		m_attribute.m_device->Release();

	if (m_attribute.m_devicecontext != nullptr)
		m_attribute.m_devicecontext->Release();

	if (m_attribute.m_rendertarget != nullptr)
		m_attribute.m_rendertarget->Release();

	if (m_attribute.m_swapchain != nullptr)
		m_attribute.m_swapchain->Release();

	if (m_attribute.m_sampler != nullptr)
		m_attribute.m_sampler->Release();

	if (m_attribute.m_shaderResource != nullptr)
		m_attribute.m_shaderResource->Release();

	if (m_attribute.m_resource != nullptr)
		m_attribute.m_resource->Release();

	if (b_pipelineState.m_vertexshader != nullptr)
		b_pipelineState.m_vertexshader->Release();

	if (b_pipelineState.m_inputlayout != nullptr)
		b_pipelineState.m_inputlayout->Release();

	if (b_pipelineState.m_pixelshader != nullptr)
		b_pipelineState.m_pixelshader->Release();

	if (b_pipelineState.m_depthStencilView != nullptr)
		b_pipelineState.m_depthStencilView->Release();

	if (b_pipelineState.m_depthStencilState != nullptr)
		b_pipelineState.m_depthStencilState->Release();

	if (b_pipelineState.m_depthStencilBuffer != nullptr)
		b_pipelineState.m_depthStencilBuffer->Release();

	if (b_pipelineState.m_rasterState != nullptr)
		b_pipelineState.m_rasterState->Release();

	if (b_pipelineState.m_buffer != nullptr)
		b_pipelineState.m_buffer->Release();

	if (b_pipelineState.g_buffer != nullptr)
		b_pipelineState.g_buffer->Release();

	if (b_pipelineState.m_indBuffer != nullptr)
		b_pipelineState.m_indBuffer->Release();

	if (m_bufferMatrix != nullptr)
		m_bufferMatrix->Release();

	debugthis->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	debugthis->Release();
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
	m_pipelineState.g_buffer = nullptr;
	m_pipelineState.m_buffer = nullptr;

	b_pipelineState.m_depthStencilBuffer = nullptr;
	b_pipelineState.m_depthStencilState = nullptr;
	b_pipelineState.m_depthStencilView = nullptr;
	b_pipelineState.m_inputlayout = nullptr;
	b_pipelineState.m_rasterState = nullptr;
	b_pipelineState.m_vertexshader = nullptr;
	b_pipelineState.m_pixelshader = nullptr;
	b_pipelineState.m_buffer = nullptr;
	b_pipelineState.g_buffer = nullptr;


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

	D3D11_RASTERIZER_DESC desc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
	desc.FillMode = D3D11_FILL_SOLID;

	m_attribute.m_device->CreateRasterizerState(&desc, &m_pipelineState.m_rasterState);
	desc.FillMode = D3D11_FILL_WIREFRAME;
	m_attribute.m_device->CreateRasterizerState(&desc, &m_pipelineState.d_rasterState);

	b_attribute = m_attribute;

	m_attribute.m_device->QueryInterface(IID_PPV_ARGS(&debugthis));


	SetUpShader();
	BufferTriangle();
	XMStoreFloat4x4(&m_matrix.world, XMMatrixTranspose(DirectX::XMMatrixIdentity()));
	SetCamera();
}

void DirectXStuff::Render()
{
	m_attribute.m_devicecontext->ClearRenderTargetView(m_attribute.m_rendertarget, DirectX::Colors::Black);
	if (update == true)
	{
		UpdateBones();
	}
	

	b_attribute.m_devicecontext->UpdateSubresource(m_bufferMatrix, 0, NULL, &m_matrix, 0, 0);
	b_attribute.m_devicecontext->VSSetConstantBuffers(0, 1, &m_bufferMatrix);

	b_attribute.m_devicecontext->PSSetShader(b_pipelineState.m_pixelshader, NULL, 0);
	b_attribute.m_devicecontext->VSSetShader(b_pipelineState.m_vertexshader, NULL, 0);

	UINT stride = sizeof(vertex);
	UINT offset = 0;
	b_attribute.m_devicecontext->IASetVertexBuffers(0, 1, &b_pipelineState.g_buffer, &stride, &offset);

	b_attribute.m_devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	b_attribute.m_devicecontext->Draw(grid.size(), 0);

	if (bonesOn)
	{
		b_attribute.m_devicecontext->UpdateSubresource(m_bufferMatrix, 0, NULL, &m_matrix, 0, 0);
		b_attribute.m_devicecontext->VSSetConstantBuffers(0, 1, &m_bufferMatrix);

		b_attribute.m_devicecontext->PSSetShader(b_pipelineState.m_pixelshader, NULL, 0);
		b_attribute.m_devicecontext->VSSetShader(b_pipelineState.m_vertexshader, NULL, 0);
		
		stride = sizeof(vertex);
		offset = 0;
		b_attribute.m_devicecontext->IASetVertexBuffers(0, 1, &b_pipelineState.m_buffer, &stride, &offset);

		b_attribute.m_devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		
		b_attribute.m_devicecontext->Draw(bonesHere.size(), 0);
	}

	if (bonesOn)
		m_attribute.m_devicecontext->RSSetState(m_pipelineState.d_rasterState);
	else
		m_attribute.m_devicecontext->RSSetState(m_pipelineState.m_rasterState);

	m_attribute.m_devicecontext->UpdateSubresource(m_bufferMatrix, 0, NULL, &m_matrix, 0, 0);
	m_attribute.m_devicecontext->VSSetConstantBuffers(0, 1, &m_bufferMatrix);
	

	m_attribute.m_devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_attribute.m_devicecontext->PSSetShader(m_pipelineState.m_pixelshader, NULL, 0);
	m_attribute.m_devicecontext->VSSetShader(m_pipelineState.m_vertexshader, NULL, 0);

	stride = sizeof(vertex);
	offset = 0;
	m_attribute.m_devicecontext->IASetVertexBuffers(0, 1, &m_pipelineState.m_buffer, &stride, &offset);

	m_attribute.m_devicecontext->IASetIndexBuffer(m_pipelineState.m_indBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_attribute.m_devicecontext->IASetInputLayout(m_pipelineState.m_inputlayout);

	m_attribute.m_devicecontext->PSSetShaderResources(0, 1, &m_attribute.m_shaderResource);
	m_attribute.m_devicecontext->PSSetSamplers(0, 1, &m_attribute.m_sampler);

	m_attribute.m_devicecontext->DrawIndexed(index.size(),0, 0);

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

void DirectXStuff::SetUpShader()
{
	m_attribute.m_device->CreatePixelShader(PixelShader, sizeof(PixelShader), NULL, &m_pipelineState.m_pixelshader);
	m_attribute.m_device->CreateVertexShader(VertexShader, sizeof(VertexShader), NULL, &m_pipelineState.m_vertexshader);

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_attribute.m_device->CreateInputLayout(ied, 2, VertexShader, sizeof(VertexShader), &m_pipelineState.m_inputlayout);
	m_attribute.m_devicecontext->IASetInputLayout(m_pipelineState.m_inputlayout);

	b_attribute.m_device->CreatePixelShader(PixelShader, sizeof(PixelShader), NULL, &b_pipelineState.m_pixelshader);
	b_attribute.m_device->CreateVertexShader(VertexShader, sizeof(VertexShader), NULL, &b_pipelineState.m_vertexshader);

	D3D11_INPUT_ELEMENT_DESC ied2[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	b_attribute.m_device->CreateInputLayout(ied2, 2, VertexShader, sizeof(VertexShader), &b_pipelineState.m_inputlayout);
	b_attribute.m_devicecontext->IASetInputLayout(b_pipelineState.m_inputlayout);
}

void DirectXStuff::BufferTriangle()
{
	if (vertsIn.size() > 0)
	{
		D3D11_BUFFER_DESC bufDesc;
		ZeroMemory(&bufDesc, sizeof(bufDesc));

		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.ByteWidth = sizeof(vertex)*vertsIn.size();
		bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SAMPLER_DESC textsample_desc;
		ZeroMemory(&textsample_desc, sizeof(textsample_desc));
		textsample_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		textsample_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		textsample_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		textsample_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		m_attribute.m_device->CreateSamplerState(&textsample_desc, &m_attribute.m_sampler);
		
		DirectX::CreateWICTextureFromFile(m_attribute.m_device, m_attribute.m_devicecontext, L"Teddy_D.png", NULL, &m_attribute.m_shaderResource);

		m_attribute.m_device->CreateBuffer(&bufDesc, NULL, &m_pipelineState.m_buffer);

		//D3D11_MAPPED_SUBRESOURCE mapSub;
		//m_attribute.m_devicecontext->Map(m_pipelineState.m_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
		//memcpy(mapSub.pData, &vertsIn[0], vertsIn.size() *  sizeof(vertex));
		//m_attribute.m_devicecontext->Unmap(m_pipelineState.m_buffer, NULL);

		D3D11_SUBRESOURCE_DATA vertBuffData = { 0 };
		vertBuffData.pSysMem = &vertsIn[0];
		vertBuffData.SysMemPitch = 0;
		vertBuffData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC bdmv(sizeof(vertex)*vertsIn.size(), D3D11_BIND_VERTEX_BUFFER);
		m_attribute.m_device->CreateBuffer(&bdmv, &vertBuffData, &m_pipelineState.m_buffer);

		CD3D11_BUFFER_DESC bdm(sizeof(m_matrix), D3D11_BIND_CONSTANT_BUFFER);
		m_attribute.m_device->CreateBuffer(&bdm, NULL, &m_bufferMatrix);

		D3D11_BUFFER_DESC bufInd;
		ZeroMemory(&bufInd, sizeof(bufInd));

		bufInd.Usage = D3D11_USAGE_DYNAMIC;
		bufInd.ByteWidth = sizeof(int)*index.size();
		bufInd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufInd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		m_attribute.m_device->CreateBuffer(&bufInd, NULL, &m_pipelineState.m_indBuffer);

		//D3D11_MAPPED_SUBRESOURCE inmapSub;
		//m_attribute.m_devicecontext->Map(m_pipelineState.m_indBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &inmapSub);
		//memcpy(inmapSub.pData, &index[0], index.size() * sizeof(int));
		//m_attribute.m_devicecontext->Unmap(m_pipelineState.m_indBuffer, NULL);
		
		D3D11_SUBRESOURCE_DATA indBuffData = { 0 };
		indBuffData.pSysMem = &index[0];
		indBuffData.SysMemPitch = 0;
		indBuffData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC bdmi(sizeof(int)*index.size(), D3D11_BIND_INDEX_BUFFER);
		m_attribute.m_device->CreateBuffer(&bdmi, &indBuffData, &m_pipelineState.m_indBuffer);
	}

	if (grid.size() > 0)
	{
		D3D11_BUFFER_DESC bufDesc;
		ZeroMemory(&bufDesc, sizeof(bufDesc));

		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.ByteWidth = sizeof(vertex)*grid.size();
		bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		b_attribute.m_device->CreateBuffer(&bufDesc, NULL, &b_pipelineState.g_buffer);

		D3D11_MAPPED_SUBRESOURCE mapSub;
		b_attribute.m_devicecontext->Map(b_pipelineState.g_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
		memcpy(mapSub.pData, &grid[0], grid.size() * sizeof(vertex));
		b_attribute.m_devicecontext->Unmap(b_pipelineState.g_buffer, NULL);
	}

	
	
	//bonesHere.clear();
	bonesHere = boneanims[animFrame];
	

	if (bonesHere.size() > 0)
	{
		D3D11_BUFFER_DESC bufDesc;
		ZeroMemory(&bufDesc, sizeof(bufDesc));

		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.ByteWidth = sizeof(vertex)*bonesHere.size();
		bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		b_attribute.m_device->CreateBuffer(&bufDesc, NULL, &b_pipelineState.m_buffer);

		D3D11_MAPPED_SUBRESOURCE mapSub;
		b_attribute.m_devicecontext->Map(b_pipelineState.m_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
		memcpy(mapSub.pData, &bonesHere[0], bonesHere.size() * sizeof(vertex));
		b_attribute.m_devicecontext->Unmap(b_pipelineState.m_buffer, NULL);

		//CD3D11_BUFFER_DESC bdm(sizeof(m_matrix), D3D11_BIND_CONSTANT_BUFFER);
		//b_attribute.m_device->CreateBuffer(&bdm, NULL, &m_bufferMatrix);

	}
}

void DirectXStuff::UpdateBones()
{
	//b_attribute.m_device->Release();
	//b_pipelineState.m_buffer->Release();
	//D3D11_BUFFER_DESC bufDesc;
	//ZeroMemory(&bufDesc, sizeof(bufDesc));
	//
	//bufDesc.Usage = D3D11_USAGE_DYNAMIC;
	//bufDesc.ByteWidth = sizeof(vertex)*bonesHere.size();
	//bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//
	//b_attribute.m_device->CreateBuffer(&bufDesc, NULL, &b_pipelineState.m_buffer);

	bonesHere.clear();
	bonesHere = bonetemp;//boneanims[animFrame];

	D3D11_MAPPED_SUBRESOURCE mapSub;
	b_attribute.m_devicecontext->Map(b_pipelineState.m_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
	memcpy(mapSub.pData, &bonesHere[0], bonesHere.size() * sizeof(vertex));
	b_attribute.m_devicecontext->Unmap(b_pipelineState.m_buffer, NULL);

	//CD3D11_BUFFER_DESC bdm(sizeof(m_matrix), D3D11_BIND_CONSTANT_BUFFER);
	//b_attribute.m_device->CreateBuffer(&bdm, NULL, &m_bufferMatrix);
	
	update = false;
}

void DirectXStuff::SetCamera()
{
	DirectX::XMVECTORF32 eye = { -100.5f, 100.7f, -1.5f, 0.0f };
	DirectX::XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_matrix.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));

	float aspectRatio = 500 / 500;
	float fovAngleY = 70.0f * 3.14f / 180.0f;

	DirectX::XMMATRIX projj = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 1000.0f);
	XMStoreFloat4x4(&m_matrix.projection, XMMatrixTranspose(projj));
	cameraPos.Position.x = -100.5;
	cameraPos.Position.y = 100.7f;
	cameraPos.Position.z = -1.5f;
	cameraPos.Position.w = 1.0f;
}

