#include "practice_00.h"
#include "windowManager.h"
#include "deviceManager.h"

practice_00::practice_00(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

practice_00::~practice_00()
{
	SAFE_RELEASE(_vertexBuffer);
}

void practice_00::init(void)
{
	direct3dApplication::init();

	_vertexBuffer = createVertexBuffer();
	_deg = 0;
	_tPoint = system_clock::now();
}

LRESULT practice_00::handleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return direct3dApplication::handleWindowMessage(hWnd, message, wParam, lParam);
}

void practice_00::update(void)
{
	direct3dApplication::update();

	auto tNow = system_clock::now();
	float elipseTime = duration<float>(tNow - _tPoint).count();

	_deg += elipseTime * 90.0f;
	_tPoint = tNow;
}

void practice_00::draw(void)
{
	direct3dApplication::draw();

	static D3DXMATRIXA16 mView;
	static D3DXMATRIXA16 mProjection;
	static D3DXMATRIXA16 mTranslate;
	static D3DXMATRIXA16 mRotate;
	static D3DXMATRIXA16 mWorld;

	// 렌더 상태 세팅
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_LIGHTING, false);
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_NONE);

	// 뷰 행렬
	static D3DXVECTOR3 vView(0, 0, 5);
	static D3DXVECTOR3 vAt(0, 0, 0);
	static D3DXVECTOR3 vUp(0, 1, 0);
	D3DXMatrixLookAtLH(&mView, &vView, &vAt, &vUp);
	GET_DEVICE_MANAGER()->getDevice()->SetTransform(D3DTS_VIEW, &mView);

	// 투영 행렬
	D3DXMatrixPerspectiveFovLH(&mProjection, D3DXToRadian(50.f),
		GET_WINDOW_MANAGER()->getAspect(),
		0.0f, 1.0f);
	GET_DEVICE_MANAGER()->getDevice()->SetTransform(D3DTS_PROJECTION, &mProjection);

	// ----- left ----- //

	// 정점 버퍼 세팅
	GET_DEVICE_MANAGER()->getDevice()->SetStreamSource(0, _vertexBuffer, 0, sizeof(vertex));

	// 정점 포멧 세팅
	GET_DEVICE_MANAGER()->getDevice()->SetFVF(vertex::FVF);
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILLMODE::D3DFILL_WIREFRAME);

	D3DXMatrixTranslation(&mTranslate, 1.5, 0, 0);
	D3DXMatrixRotationX(&mRotate, D3DXToRadian(_deg));
	mWorld = mRotate * mTranslate;

	GET_DEVICE_MANAGER()->getDevice()->SetTransform(D3DTS_WORLD, &mWorld);
	GET_DEVICE_MANAGER()->getDevice()->DrawPrimitive(
		D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP,
		0, 2);

	// ----- right ----- //

	// 정점 포멧 세팅
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILLMODE::D3DFILL_SOLID);

	D3DXMatrixTranslation(&mTranslate, -1.5, 0, 0);
	D3DXMatrixRotationY(&mRotate, D3DXToRadian(_deg));
	mWorld = mRotate * mTranslate;

	GET_DEVICE_MANAGER()->getDevice()->SetTransform(D3DTS_WORLD, &mWorld);
	GET_DEVICE_MANAGER()->getDevice()->DrawPrimitive(
		D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP,
		0, 2);

	// 렌더 상태 초기화
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_LIGHTING, true);
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_CCW);
}

LPDIRECT3DVERTEXBUFFER9 practice_00::createVertexBuffer(void)
{
	LPDIRECT3DVERTEXBUFFER9 vBuffer = NULL;

	// VB 생성
	GET_DEVICE_MANAGER()->getDevice()->CreateVertexBuffer(sizeof(vertex) * 4,
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL::D3DPOOL_MANAGED,
		&vBuffer,
		NULL);

	// VB 설정
	vertex* pVertex = NULL;
	if (SUCCEEDED(vBuffer->Lock(0, 0, (void**)&pVertex, 0)))
	{
		pVertex[0].pos = D3DXVECTOR3(-1, -1, 0);
		pVertex[0].diffuse = D3DCOLOR_XRGB(255, 0, 0);

		pVertex[1].pos = D3DXVECTOR3(1, -1, 0);
		pVertex[1].diffuse = D3DCOLOR_XRGB(0, 255, 0);

		pVertex[2].pos = D3DXVECTOR3(-1, 1, 0);
		pVertex[2].diffuse = D3DCOLOR_XRGB(0, 0, 255);

		pVertex[3].pos = D3DXVECTOR3(1, 1, 0);
		pVertex[3].diffuse = D3DCOLOR_XRGB(255, 0, 255);

		vBuffer->Unlock();
	}

	return vBuffer;
}

void practice_00::initMatrix(void)
{
}