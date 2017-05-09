#pragma once
#include <d3d11.h>
#include <D3DX11.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include "VertexShader.csh"
#include "PixelShader.csh"
#include <vector>

class DirectXStuff
{
public:
	DirectXStuff();
	~DirectXStuff();

	struct pipeline_state_t
	{

		ID3D11InputLayout *m_inputlayout;
		ID3D11VertexShader * m_vertexshader;
		ID3D11PixelShader * m_pixelshader;
		ID3D11Texture2D * m_depthStencilBuffer;
		ID3D11DepthStencilState * m_depthStencilState;
		ID3D11DepthStencilView * m_depthStencilView;
		ID3D11RasterizerState * m_rasterState;
		ID3D11Buffer * m_buffer;

	}default_pipeline;

	struct Attributes
	{

		ID3D11Device * m_device;
		ID3D11DeviceContext * m_devicecontext;
		IDXGISwapChain * m_swapchain;
		ID3D11RenderTargetView * m_rendertarget;
		D3D_DRIVER_TYPE m_drivertype;
		D3D_FEATURE_LEVEL m_featurelevels;
		D3D11_VIEWPORT m_viewport;

	};

	struct vertex
	{
		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT4 Color;
	};

	struct matrix
	{
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 world;
	};

	pipeline_state_t m_pipelineState;
	Attributes m_attribute;
	matrix m_matrix;

	ID3D11Buffer * m_bufferMatrix;
	ID3D11Buffer * m_bufferXline;
	ID3D11Buffer * m_bufferYline;
	ID3D11Buffer * m_bufferZline;

	int * index = nullptr;

	UINT thig;

	vertex * vertsIn = nullptr;
	std::vector<vertex> bonesHere;
	bool bonesOn = true;
	int boneSize = 0;


	int vSize;

	float camrotX = 0, camrotY = 0;
	vertex cameraPos;

	std::vector<vertex> bones;

	void Start(HWND window, int width, int height);
	void Render();
	void UpdateCamera(float _x, float _y);
	//int SetThings(int * index, float _vert0, float _vert1, float _vert2, float _vert3);
	void SetUpShader();
	void BufferTriangle();
	void SetCamera();
	void Randomize();
	void DrawTriangle();
};