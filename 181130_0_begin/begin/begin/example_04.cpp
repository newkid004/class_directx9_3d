#include "example_04.h"
#include "windowManager.h"
#include "deviceManager.h"

example_04::example_04(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_04::~example_04()
{
	SAFE_RELEASE(_vertexBuffer);
}

void example_04::init(void)
{
	direct3dApplication::init();

	_vertexBuffer = createVertexBuffer();
	_yaw = 0;
	_tPoint = system_clock::now();
}

LRESULT example_04::handleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return direct3dApplication::handleWindowMessage(hWnd, message, wParam, lParam);
}

void example_04::update(void)
{
	direct3dApplication::update();

	auto tNow = system_clock::now();
	float elipseTime = duration<float>(tNow - _tPoint).count();

	_yaw += elipseTime * 90.0f;
	_tPoint = tNow;
}

void example_04::draw(void)
{
	direct3dApplication::draw();

	static D3DXMATRIXA16 mWorld;
	static D3DXMATRIXA16 mView;
	static D3DXMATRIXA16 mProjection;
	
	// 월드 행렬
	D3DXMatrixRotationY(&mWorld, D3DXToRadian(_yaw));
	GET_DEVICE_MANAGER()->getDevice()->SetTransform(D3DTS_WORLD, &mWorld);

	// 뷰 행렬
	static D3DXVECTOR3 vView(0, 3, 3);
	static D3DXVECTOR3 vAt(0, 0, 0);
	static D3DXVECTOR3 vUp(0, 1, 0);
	D3DXMatrixLookAtLH(&mView, &vView, &vAt, &vUp);
	GET_DEVICE_MANAGER()->getDevice()->SetTransform(D3DTS_VIEW, &mView);

	// 투영 행렬
	D3DXMatrixPerspectiveFovLH(&mProjection, D3DXToRadian(50.f),
		GET_WINDOW_MANAGER()->getAspect(),
		0.0f, 1.0f);
	GET_DEVICE_MANAGER()->getDevice()->SetTransform(D3DTS_PROJECTION, &mProjection);

	// 렌더상태 세팅
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_LIGHTING, false);
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_NONE);

	// 정점 버퍼 세팅
	GET_DEVICE_MANAGER()->getDevice()->SetStreamSource(0, _vertexBuffer, 0, sizeof(vertex));

	// 정점 포멧 세팅
	GET_DEVICE_MANAGER()->getDevice()->SetFVF(vertex::FVF);

	// 그린다
	GET_DEVICE_MANAGER()->getDevice()->DrawPrimitive(
		D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST,
		0, 1);

	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_LIGHTING, true);
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_CCW);
}

LPDIRECT3DVERTEXBUFFER9 example_04::createVertexBuffer(void)
{
	LPDIRECT3DVERTEXBUFFER9 vBuffer = NULL;

	// VB 생성
	GET_DEVICE_MANAGER()->getDevice()->CreateVertexBuffer(sizeof(vertex) * 3,
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL::D3DPOOL_MANAGED,
		&vBuffer,
		NULL);

	// VB 설정
	vertex* pVertex = NULL;
	if (SUCCEEDED(vBuffer->Lock(0, 0, (void**)&pVertex, 0)))
	{
		pVertex[0].pos = D3DXVECTOR3(0, 0, 0);
		pVertex[0].diffuse = D3DCOLOR_XRGB(255, 0, 0);

		pVertex[1].pos = D3DXVECTOR3(0, 1, 0);
		pVertex[1].diffuse = D3DCOLOR_XRGB(0, 255, 0);

		pVertex[2].pos = D3DXVECTOR3(1, 0, 0);
		pVertex[2].diffuse = D3DCOLOR_XRGB(0, 0, 255);

		vBuffer->Unlock();
	}

	return vBuffer;
}