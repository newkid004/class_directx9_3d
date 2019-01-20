#include "example_05.h"
#include "managerList.h"

example_05::example_05(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_05::~example_05()
{
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_indexBuffer);
}

void example_05::init(void)
{
	direct3dApplication::init();

	_vertexBuffer = createVertexBuffer();
	_indexBuffer = createIndexBuffer();
}

LRESULT example_05::handleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return direct3dApplication::handleWindowMessage(hWnd, message, wParam, lParam);
}

void example_05::update(void)
{
	_yaw += 90.f * GET_TIME_MANAGER()->getDeltaTime();
	_pitch += 90.f * GET_TIME_MANAGER()->getDeltaTime();

	// if (GET_INPUT_MANAGER()->keyDown(DIK_SPACE));
}

void example_05::draw(void)
{
	// view
	static D3DXVECTOR3 vPos = D3DXVECTOR3(0, 0, -5);
	static D3DXVECTOR3 vAt = D3DXVECTOR3(0, 0, 0);
	static D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);
	static D3DXMATRIX mView;
	D3DXMatrixLookAtLH(&mView, &vPos, &vAt, &vUp);
	GET_DEVICE_MANAGER()->getDevice()->SetTransform(D3DTS_VIEW, &mView);

	// projection
	static D3DXMATRIX mProjection;
	D3DXMatrixPerspectiveFovLH(&mProjection, D3DXToRadian(50.f), GET_WINDOW_MANAGER()->getAspect(), 0.1f, 1000.0f);
	GET_DEVICE_MANAGER()->getDevice()->SetTransform(D3DTS_PROJECTION, &mProjection);

	// left
	static float posX = -1.5f;
	if (GET_INPUT_MANAGER()->keyDown(DIK_A)) posX -= 0.01f;
	if (GET_INPUT_MANAGER()->keyDown(DIK_D)) posX += 0.01f;

	drawQuad(D3DXVECTOR3(posX, 0, 0), D3DXVECTOR3(_pitch, 0, 0));

	// right
	drawQuad(D3DXVECTOR3(1.5, 0, 0), D3DXVECTOR3(0, _yaw, 0));
}

LPDIRECT3DVERTEXBUFFER9 example_05::createVertexBuffer(void)
{
	LPDIRECT3DVERTEXBUFFER9 vBuffer = NULL;

	GET_DEVICE_MANAGER()->getDevice()->CreateVertexBuffer(
		sizeof(vertex) * 4,
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL::D3DPOOL_MANAGED,
		&vBuffer,
		NULL);

	vertex* v = NULL;
	if (SUCCEEDED(vBuffer->Lock(0, 0, (void**)&v, 0)))
	{
		v[0].pos = D3DXVECTOR3(-1, -1, 0);
		v[1].pos = D3DXVECTOR3(-1, 1, 0);
		v[2].pos = D3DXVECTOR3(1, 1, 0);
		v[3].pos = D3DXVECTOR3(1, -1, 0);

		v[0].color = D3DCOLOR_XRGB(255, 0, 0);
		v[1].color = D3DCOLOR_XRGB(255, 0, 0);
		v[2].color = D3DCOLOR_XRGB(255, 0, 0);
		v[3].color = D3DCOLOR_XRGB(255, 0, 0);

		vBuffer->Unlock();
	}

	return vBuffer;
}

LPDIRECT3DINDEXBUFFER9 example_05::createIndexBuffer(void)
{
	LPDIRECT3DINDEXBUFFER9 iBuffer = NULL;

	GET_DEVICE_MANAGER()->getDevice()->CreateIndexBuffer(
		sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL::D3DPOOL_MANAGED,
		&iBuffer,
		nullptr);

	WORD* i = NULL;
	if (SUCCEEDED(iBuffer->Lock(0, 0, (void**)&i, 0)))
	{
		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 0; i[4] = 2; i[5] = 3;
		iBuffer->Unlock();
	}

	return iBuffer;
}
void example_05::drawQuad(D3DXVECTOR3 & position, D3DXVECTOR3 & rotation)
{
	// world
	static D3DXMATRIXA16 mTranslation;
	static D3DXMATRIXA16 mRotation;
	static D3DXMATRIXA16 mWorld;

	D3DXMatrixTranslation(&mTranslation, position.x, position.y, position.z);
	D3DXMatrixRotationYawPitchRoll(&mRotation, 
		D3DXToRadian(rotation.y), 
		D3DXToRadian(rotation.x), 
		D3DXToRadian(rotation.z));
	mWorld = mRotation * mTranslation;
	GET_DEVICE_MANAGER()->getDevice()->SetTransform(D3DTS_WORLD, &mWorld);

	// square
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_LIGHTING, false);
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_NONE);

	GET_DEVICE_MANAGER()->getDevice()->SetStreamSource(0, _vertexBuffer, 0, sizeof(vertex));
	GET_DEVICE_MANAGER()->getDevice()->SetIndices(_indexBuffer);
	GET_DEVICE_MANAGER()->getDevice()->SetFVF(vertex::FVF);

	GET_DEVICE_MANAGER()->getDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_LIGHTING, true);
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILLMODE::D3DFILL_WIREFRAME);

	GET_DEVICE_MANAGER()->getDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_CCW);
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILLMODE::D3DFILL_SOLID);
}