#include <Windows.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fbxsdk.h>

//#include "fbx.h"

using namespace DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// ��̒��_�����i�[����\����


// GPU(�V�F�[�_��)�֑��鐔�l���܂Ƃ߂��\����
struct CONSTANT_BUFFER {
	XMMATRIX mWVP;
};

#define WIN_STYLE WS_OVERLAPPEDWINDOW
int CWIDTH;     // �N���C�A���g�̈�̕�
int CHEIGHT;    // �N���C�A���g�̈�̍���

HWND WHandle;
const char *ClassName = "Temp_Window";

IDXGISwapChain *pSwapChain;
ID3D11Device *pDevice;
ID3D11DeviceContext *pDeviceContext;

ID3D11RenderTargetView *pBackBuffer_RTV;

ID3D11RasterizerState *pRasterizerState;

#ifdef __FBX_DEBUG__
ID3D11InputLayout *pVertexLayout;

ID3D11VertexShader *pVertexShader;		//model �̃V�F�[�_�[���ƂɕK�v
ID3D11PixelShader *pPixelShader;		//model�̃V�F�[�_�[���ƂɕK�v
ID3D11Buffer *pConstantBuffer;			//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�ƍ��킹��
#endif

#ifndef __FBX_DEBUG__

#endif

//#define __FBX_DEBUG__

#ifndef __FBX_DEBUG__
#include "model.h"
#endif

#ifndef __FBX_DEBUG__
MyModel::Model DemonModel;
#endif


#ifndef __FBX_DEBUG__
#include "ShaderUtility.h"

MY_SHADER_UTILITY::ShaderUtility ShaderUtil;

#endif
static float x = 0;

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR pCmdLine, int nCmdShow) {

	// �E�B���h�E�N���X��o�^����
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WinProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = ClassName;
	RegisterClass(&wc);

	// �E�B���h�E�̍쐬
	WHandle = CreateWindow(ClassName, "FBX�̓ǂݍ���(���f���f�[�^)", WIN_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 800, NULL, NULL, hInstance, NULL);
	if (WHandle == NULL) return 0;
	ShowWindow(WHandle, nCmdShow);

	// ���b�Z�[�W���[�v�̎��s
	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// ----- DX�̏��� -----
			float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
			pDeviceContext->ClearRenderTargetView(pBackBuffer_RTV, clearColor);

			// �p�����[�^�̌v�Z
			XMVECTOR eye_pos = XMVectorSet(0.0f, 70.0f, 500.0f, 1.0f);
			XMVECTOR eye_lookat = XMVectorSet(0.0f, 70.0f, 0.0f, 1.0f);
			XMVECTOR eye_up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
			XMMATRIX World = XMMatrixRotationY(x += 0.001f);
			XMMATRIX View = XMMatrixLookAtLH(eye_pos, eye_lookat, eye_up);
			XMMATRIX Proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)CWIDTH / (FLOAT)CHEIGHT, 0.1f, 800.0f);

#ifdef __SHADER_DEBUG__
			// �p�����[�^�̎󂯓n��
			D3D11_MAPPED_SUBRESOURCE pdata;
			CONSTANT_BUFFER cb;
			cb.mWVP = XMMatrixTranspose(World * View * Proj);

			pDeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
			memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));
			pDeviceContext->Unmap(pConstantBuffer, 0);
#endif

#ifndef __SHADER_DEBUG__
			MY_SHADER_UTILITY::CONSTANT_BUFFER cb;
			cb.mWVP = XMMatrixTranspose(World * View * Proj);
#endif
			ShaderUtil.SetConstantBuff(pDeviceContext, cb);

#ifdef __FBX_DEBUG__
			pDeviceContext->DrawIndexed(DemonModel.GetNumFace(), 0, 0);
#endif
#ifndef __FBX_DEBUG__
			DemonModel.Draw(pDeviceContext);
#endif
			pSwapChain->Present(0, 0);
		}
	}

	return 0;
}


LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
	case WM_CREATE:
	{

		// ----- �p�C�v���C���̏��� -----
		RECT csize;
		GetClientRect(hwnd, &csize);
		CWIDTH = csize.right;
		CHEIGHT = csize.bottom;

		DXGI_SWAP_CHAIN_DESC scd = { 0 };
		scd.BufferCount = 1;
		scd.BufferDesc.Width = CWIDTH;
		scd.BufferDesc.Height = CHEIGHT;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = hwnd;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.Windowed = TRUE;
		D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_11_0;
		D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fl, 1, D3D11_SDK_VERSION, &scd, &pSwapChain, &pDevice, NULL, &pDeviceContext);

		ID3D11Texture2D *pbbTex;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pbbTex);
		pDevice->CreateRenderTargetView(pbbTex, NULL, &pBackBuffer_RTV);
		pbbTex->Release();

		// �r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)CWIDTH;
		vp.Height = (FLOAT)CHEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;


#ifdef __FBX_DEBUG__
		// �V�F�[�_�̐ݒ�
		ID3DBlob *pCompileVS = NULL;
		ID3DBlob *pCompilePS = NULL;
		D3DCompileFromFile(L"shader.hlsl", NULL, NULL, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
		pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &pVertexShader);
		D3DCompileFromFile(L"shader.hlsl", NULL, NULL, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
		pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &pPixelShader);

		// ���_���C�A�E�g
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		pDevice->CreateInputLayout(layout, 1, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout);
		pCompileVS->Release();
		pCompilePS->Release();

		// �萔�o�b�t�@�̐ݒ�
		D3D11_BUFFER_DESC cb;
		cb.ByteWidth = sizeof(CONSTANT_BUFFER);
		cb.Usage = D3D11_USAGE_DYNAMIC;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.StructureByteStride = 0;
		pDevice->CreateBuffer(&cb, NULL, &pConstantBuffer);
#endif

#ifndef __FBX_DEBUG__

		ShaderUtil.InitPS(pDevice, "shader.hlsl", "PS", "ps_5_0");
		
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		};
		ShaderUtil.InitVSandLayout(pDevice, "shader.hlsl", "VS", "vs_5_0", layout, 1);
		ShaderUtil.InitConstantBuffer(pDevice);
#endif

#ifndef __FBX_DEBUG__
		DemonModel.Load("demonwalking.fbx");
		
#endif


#ifndef __FBX_DEBUG__
		DemonModel.InitVertexBuffer(pDevice);
#endif




#ifndef __FBX_DEBUG__

		DemonModel.InitIndexBuffer(pDevice);
#endif

		// ���X�^���C�U�̐ݒ�
		D3D11_RASTERIZER_DESC rdc = {};
		rdc.CullMode = D3D11_CULL_BACK;
		rdc.FillMode = D3D11_FILL_SOLID;
		rdc.FrontCounterClockwise = TRUE;
		pDevice->CreateRasterizerState(&rdc, &pRasterizerState);

		// �I�u�W�F�N�g�̔��f
#ifndef __FBX_DEBUG__
		UINT stride = sizeof(MY_VERTEX); //Debugoff
#endif

		UINT offset = 0;

#ifndef __FBX_DEBUG__		//debugoff
		pDeviceContext->IASetVertexBuffers(0, 1, DemonModel.GetVertexBuffer(), &stride, &offset);
		pDeviceContext->IASetIndexBuffer(*(DemonModel.GetIndexBuffer()), DXGI_FORMAT_R32_UINT, 0);
#endif

#ifdef __SHADER_DEBUG__
		pDeviceContext->IASetInputLayout(pVertexLayout);
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);


		pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
		pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
#endif

#ifndef __SHADER_DEBUG__
		ShaderUtil.SetLayout(pDeviceContext);
		ShaderUtil.SetVS(pDeviceContext);
		ShaderUtil.SetPS(pDeviceContext);

#endif



		pDeviceContext->RSSetState(pRasterizerState);


		pDeviceContext->OMSetRenderTargets(1, &pBackBuffer_RTV, NULL);
		pDeviceContext->RSSetViewports(1, &vp);

		return 0;
	}
	case WM_DESTROY:

		pSwapChain->Release();
		pDeviceContext->Release();
		pDevice->Release();

		pBackBuffer_RTV->Release();

		pRasterizerState->Release();

#ifdef __FBX_DEBUG__
		pVertexShader->Release();
		pVertexLayout->Release();
		pPixelShader->Release();
		pConstantBuffer->Release();
		
#endif
		PostQuitMessage(0);

		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}