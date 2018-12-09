//�w�b�_�[�t�@�C���̃C���N���[�h
#include <windows.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dCompiler.h>
#include <fbxsdk.h>
#include <string>
using namespace DirectX;

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
//�萔��`
#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����
int CWIDTH = WINDOW_WIDTH;
int CHEIGHT = WINDOW_HEIGHT;

//char testString[10] = "";

//�P�̒��_�����i�[����\����
struct VERTEX {
	XMFLOAT3 Pos;
};

//GPU(�V�F�[�_���֑��鐔�l���܂Ƃ߂��\����
struct CONSTANT_BUFFER {
	XMMATRIX mWVP;
};

//�O���[�o���ϐ�
HWND g_hWnd = NULL;

IDXGISwapChain *pSwapChain;
ID3D11Device *pDevice;
ID3D11DeviceContext *pDeviceContext;

ID3D11RenderTargetView *pBackBufferRTV;

ID3D11RasterizerState *pRasterizerState;
ID3D11VertexShader *pVertexShader;
ID3D11InputLayout *pVertexLayout;
ID3D11PixelShader *pPixelShader;
ID3D11Buffer *pConstantBuffer;

fbxsdk::FbxManager *fbxManager = NULL;

fbxsdk::FbxScene *fbxScene = NULL;
fbxsdk::FbxMesh *mesh = NULL;
ID3D11Buffer *VerBuffer = NULL;
ID3D11Buffer *IndBuffer = NULL;
VERTEX *vertices;

static float x = 0;



//�֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//
//�G���g���[�֐�
//
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	// �E�B���h�E�̏�����
	static char szAppName[] = "FBX�̓ǂݍ��݁i���o�C���f�[�^�j";

	WNDCLASSEX  wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = szAppName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

	g_hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInst, 0);

	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);

	// ���b�Z�[�W���[�v
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//�A�v���P�[�V�����̏����͂����ōs���B

			//float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
			//pDeviceContext->ClearRenderTargetView(pBackBufferRTV, clearColor);

			//////�p�����[�^�̌v�Z
			//XMVECTOR eye_pos = XMVectorSet(0.0f, 70.0f, 500.0f, 1.0f);
			//XMVECTOR eye_lookat = XMVectorSet(0.0f, 70.0f, 0.0f, 1.0f);
			//XMVECTOR eye_up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

			//XMMATRIX World = XMMatrixRotationY(x += 0.001f);
			//XMMATRIX View = XMMatrixLookAtLH(eye_pos, eye_lookat, eye_up);
			//XMMATRIX Proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)CWIDTH / (FLOAT)CHEIGHT, 0.1f, 800.0f);

			//// �`����s
			//pDeviceContext->DrawIndexed(mesh->GetPolygonVertexCount(), 0, 0);
			//pSwapChain->Present(0, 0);


		}
	}

	//�I��
	return (INT)msg.wParam;
}

//
//�E�B���h�E�v���V�[�W���[�֐�
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_CREATE:
	{
		fbxManager = fbxsdk::FbxManager::Create();//test

		//RECT csize;
		//GetClientRect(g_hWnd, &csize);
		//CWIDTH = csize.right;
		//CHEIGHT = csize.bottom;

		////DXGI_SWAP_CHAIN_DESC scd = { 0 };

		////scd.BufferCount = 1;
		////scd.BufferDesc.Width = CWIDTH;
		////scd.BufferDesc.Height = CHEIGHT;
		////scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		////scd.BufferDesc.RefreshRate.Numerator = 60;
		////scd.BufferDesc.RefreshRate.Denominator = 1;
		////scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		////scd.OutputWindow = g_hWnd;
		////scd.SampleDesc.Count = 1; 
		////scd.SampleDesc.Quality = 0;
		////scd.Windowed = TRUE;
		////D3D_FEATURE_LEVEL f1 = D3D_FEATURE_LEVEL_11_0;
		////D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &f1, 1, D3D11_SDK_VERSION, &scd, &pSwapChain, &pDevice, NULL, &pDeviceContext);

		//DXGI_SWAP_CHAIN_DESC sd;
		//ZeroMemory(&sd, sizeof(sd));
		//sd.BufferCount = 1;
		//sd.BufferDesc.Width = WINDOW_WIDTH;
		//sd.BufferDesc.Height = WINDOW_HEIGHT;
		//sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//sd.BufferDesc.RefreshRate.Numerator = 60;
		//sd.BufferDesc.RefreshRate.Denominator = 1;
		//sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//sd.OutputWindow = g_hWnd;
		//sd.SampleDesc.Count = 1;
		//sd.SampleDesc.Quality = 0;
		//sd.Windowed = TRUE;

		//D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
		//D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

		//if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		//	0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice,
		//	pFeatureLevel, &pDeviceContext)))
		//{
		//	return FALSE;
		//}

		//ID3D11Texture2D *pbbTex;
		//pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pbbTex);
		//pDevice->CreateRenderTargetView(pbbTex, NULL, &pBackBufferRTV);
		//pbbTex->Release();

		//// �r���[�|�[�g�̐ݒ�
		//D3D11_VIEWPORT vp;
		//vp.Width = (float)CWIDTH;
		//vp.Height = (float)CHEIGHT;
		//vp.MinDepth = 0.0f;
		//vp.MaxDepth = 1.0f;
		//vp.TopLeftX = 0;
		//vp.TopLeftY = 0;

		////�V�F�[�_�̐ݒ�
		//ID3DBlob *pCompileVS = NULL;
		//ID3DBlob *pCompilePS = NULL;


		//D3DCompileFromFile(L"shader.hlsl", NULL, NULL, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
		//pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &pVertexShader);
		//D3DCompileFromFile(L"shader.hlsl", NULL, NULL, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
		//pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &pPixelShader);

		////���_���C�A�E�g
		//D3D11_INPUT_ELEMENT_DESC layout[] = {
		//	{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		//};

		//pDevice->CreateInputLayout(layout, 1, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout);
		//pCompileVS->Release();
		//pCompileVS->Release();


		////�萔�o�b�t�@�̐ݒ�
		//D3D11_BUFFER_DESC cb;
		//cb.ByteWidth = sizeof(CONSTANT_BUFFER);
		//cb.Usage = D3D11_USAGE_DYNAMIC;
		//cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		//cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//cb.MiscFlags = 0;
		//cb.StructureByteStride = 0;
		//pDevice->CreateBuffer(&cb, NULL, &pConstantBuffer);

		////FBX�̓ǂݍ���
		//fbxManager = FbxManager::Create();
		//fbxScene = FbxScene::Create(fbxManager, "fbxscene");
		//FbxString FileName("\\CChu\\Model\\CCChuBody.FBX");
		//FbxImporter *fbxImporter = FbxImporter::Create(fbxManager, "imp");


		//bool e = fbxImporter->Initialize(FileName.Buffer(), -1, fbxManager->GetIOSettings());
		//if (!e)
		//{
		//	//MessageBox(g_hWnd, "Load Failed.", "Import Error", 0);
		//}

		//fbxImporter->Import(fbxScene);
		//fbxImporter->Destroy();

		////���_�f�[�^�̎��o��
		//for (int i = 0; i < fbxScene->GetRootNode()->GetChildCount(); i++) {
		//	if (fbxScene->GetRootNode()->GetChild(i)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
		//		mesh = fbxScene->GetRootNode()->GetChild(i)->GetMesh();
		//		break;
		//	}
		//}

		//vertices = new VERTEX[mesh->GetControlPointsCount()];
		//for (int i = 0; i < mesh->GetControlPointsCount(); i++) {
		//	vertices[i].Pos.x = (FLOAT)mesh->GetControlPointAt(i)[0];
		//	vertices[i].Pos.y = (FLOAT)mesh->GetControlPointAt(i)[1];
		//	vertices[i].Pos.z = (FLOAT)mesh->GetControlPointAt(i)[2];
		//}

		////���_�f�[�^�p�o�b�t�@�̐ݒ�
		//D3D11_BUFFER_DESC bd_vertex;
		//bd_vertex.ByteWidth = sizeof(VERTEX) * mesh->GetControlPointsCount();
		//bd_vertex.Usage = D3D11_USAGE_DEFAULT;
		//bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//bd_vertex.CPUAccessFlags = 0;
		//bd_vertex.MiscFlags = 0;
		//bd_vertex.StructureByteStride = 0;
		//D3D11_SUBRESOURCE_DATA data_vertex;
		//data_vertex.pSysMem = vertices;
		//pDevice->CreateBuffer(&bd_vertex, &data_vertex, &VerBuffer);

		////�C���f�b�N�X�f�[�^�̎��o���ƃo�b�t�@�̐ݒ�
		//D3D11_BUFFER_DESC bd_index;
		//bd_index.ByteWidth = sizeof(int)*mesh->GetPolygonVertexCount();
		//bd_index.Usage = D3D11_USAGE_DEFAULT;
		//bd_index.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//bd_index.CPUAccessFlags = 0;
		//bd_index.MiscFlags = 0;
		//bd_index.StructureByteStride = 0;
		//D3D11_SUBRESOURCE_DATA data_index;
		//data_index.pSysMem = mesh->GetPolygonVertices();
		//pDevice->CreateBuffer(&bd_index, &data_index, &IndBuffer);

		////���X�^���C�U�̐ݒ�
		//D3D11_RASTERIZER_DESC rdc = {};
		//rdc.CullMode = D3D11_CULL_BACK;
		//rdc.FillMode = D3D11_FILL_SOLID;
		//rdc.FrontCounterClockwise = TRUE;
		//pDevice->CreateRasterizerState(&rdc, &pRasterizerState);

		//UINT stride = sizeof(VERTEX);
		//UINT offset = 0;
		//pDeviceContext->IASetVertexBuffers(0, 1, &VerBuffer, &stride, &offset);
		//pDeviceContext->IASetIndexBuffer(IndBuffer, DXGI_FORMAT_R32_UINT, 0);
		//pDeviceContext->IASetInputLayout(pVertexLayout);
		//pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//pDeviceContext->RSSetState(pRasterizerState);
		//pDeviceContext->OMSetRenderTargets(1, &pBackBufferRTV, NULL);
		//pDeviceContext->RSSetViewports(1, &vp);


		return 0;
	}
	case WM_KEYDOWN:
	{
		switch ((char)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	}
	case WM_DESTROY:
	{

		fbxManager->Destroy(); //test


		/*pSwapChain->Release();
		pDeviceContext->Release();
		pDevice->Release();
		pBackBufferRTV->Release();

		pRasterizerState->Release();
		pVertexShader->Release();
		pVertexLayout->Release();
		pPixelShader->Release();
		pConstantBuffer->Release();
		VerBuffer->Release();

		fbxScene->Destroy();
		fbxManager->Destroy();

		delete[] vertices;*/

		PostQuitMessage(0);
		break;
	}
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}