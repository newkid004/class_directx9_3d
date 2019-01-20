#include "practice_01.h"
#include "managerList.h"

void practice_01::init(void)
{
	direct3dApplication::init();

	_vertexBuffer = createVertexBuffer();

	_position = D3DXVECTOR3(0, 0, 0);

	_indexSoft = createSoftCube();
	_indexHard = createHardCube();
}

practice_01::~practice_01()
{
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_indexSoft);
	SAFE_RELEASE(_indexHard);
}

void practice_01::update(void)
{
	if (GET_INPUT_MANAGER()->keyDown(DIK_A)) _position.x -= 0.03f;
	if (GET_INPUT_MANAGER()->keyDown(DIK_D)) _position.x += 0.03f;
	if (GET_INPUT_MANAGER()->keyDown(DIK_W)) _position.z += 0.03f;
	if (GET_INPUT_MANAGER()->keyDown(DIK_S)) _position.z -= 0.03f;
	if (GET_INPUT_MANAGER()->keyDown(DIK_R)) _position.y += 0.03f;
	if (GET_INPUT_MANAGER()->keyDown(DIK_F)) _position.y -= 0.03f;
}

void practice_01::draw(void)
{
	// view
	static D3DXVECTOR3 vPos = D3DXVECTOR3(0, 0, -10);
	static D3DXVECTOR3 vAt = D3DXVECTOR3(0, 0, 0);
	static D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);
	static D3DXMATRIX mView;
	D3DXMatrixLookAtLH(&mView, &vPos, &vAt, &vUp);
	GET_DEVICE_MANAGER()->getDevice()->SetTransform(D3DTS_VIEW, &mView);

	// projection
	static D3DXMATRIX mProjection;
	D3DXMatrixPerspectiveFovLH(&mProjection, D3DXToRadian(40.f), GET_WINDOW_MANAGER()->getAspect(), 0.1f, 1000.0f);
	GET_DEVICE_MANAGER()->getDevice()->SetTransform(D3DTS_PROJECTION, &mProjection);

	// cube - hard
	static D3DXMATRIX mHardTotal;
	static D3DXMATRIX mHardPosition;
	static D3DXMATRIX mHardRotation;

	static float rotate = 0.0f;
	rotate += GET_TIME_MANAGER()->getDeltaTime() * 45.0f;

	D3DXMatrixRotationYawPitchRoll(&mHardRotation, D3DXToRadian(rotate), D3DXToRadian(rotate), -D3DXToRadian(rotate));
	D3DXMatrixTranslation(&mHardPosition, _position.x, _position.y, _position.z);
	mHardTotal = mHardRotation * mHardPosition;

	// cube - sofe
	static D3DXMATRIX mSoftTotal;
	static D3DXMATRIX mSoftPosition;
	static D3DXMATRIX mSoftRotation;
	static D3DXMATRIX mSoftOrbit;

	D3DXMatrixRotationYawPitchRoll(&mSoftOrbit, D3DXToRadian(rotate * 2), D3DXToRadian(rotate * 2), D3DX_PI / 6.f);
	D3DXMatrixRotationYawPitchRoll(&mSoftRotation, D3DXToRadian(rotate * 8), D3DXToRadian(rotate * 8), D3DXToRadian(rotate * 8));
	D3DXMatrixTranslation(&mSoftPosition, 7.5f, 0, 0);
	mSoftTotal = mSoftRotation * mSoftPosition * mSoftOrbit * mHardPosition;

	drawCube(_indexHard, mHardTotal);
	drawCube(_indexSoft, mSoftTotal);
}

void practice_01::drawCube(LPDIRECT3DINDEXBUFFER9 indexBuffer, D3DXMATRIX & mTrans)
{
	// world
	static D3DXMATRIXA16 mTranslation;
	static D3DXMATRIXA16 mRotation;
	static D3DXMATRIXA16 mWorld;

	GET_DEVICE_MANAGER()->getDevice()->SetTransform(D3DTS_WORLD, &mTrans);

	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_LIGHTING, false);
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_NONE);
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILLMODE::D3DFILL_SOLID);

	GET_DEVICE_MANAGER()->getDevice()->SetStreamSource(0, _vertexBuffer, 0, sizeof(vertex));
	GET_DEVICE_MANAGER()->getDevice()->SetIndices(indexBuffer);
	GET_DEVICE_MANAGER()->getDevice()->SetFVF(vertex::FVF);
	GET_DEVICE_MANAGER()->getDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 36, 0, 12);

	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_LIGHTING, true);
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILLMODE::D3DFILL_WIREFRAME);
	GET_DEVICE_MANAGER()->getDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 36, 0, 12);

	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_LIGHTING, true);
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_CCW);
	GET_DEVICE_MANAGER()->getDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILLMODE::D3DFILL_SOLID);
}

LPDIRECT3DVERTEXBUFFER9 practice_01::createVertexBuffer(void)
{
	LPDIRECT3DVERTEXBUFFER9 result = NULL;

	GET_DEVICE_MANAGER()->getDevice()->CreateVertexBuffer(
		sizeof(vertex) * 24,
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL::D3DPOOL_MANAGED,
		&result,
		NULL);

	vertex* v = NULL;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&v, 0)))
	{
		int i = -1;

		// 전 0
		++i; v[i].pos = D3DXVECTOR3(-1, -1, -1);
		++i; v[i].pos = D3DXVECTOR3(1, -1, -1);
		++i; v[i].pos = D3DXVECTOR3(-1, 1, -1);
		++i; v[i].pos = D3DXVECTOR3(1, 1, -1);

		// 후 4
		++i; v[i].pos = D3DXVECTOR3(-1, 1, 1);
		++i; v[i].pos = D3DXVECTOR3(1, 1, 1);
		++i; v[i].pos = D3DXVECTOR3(-1, -1, 1);
		++i; v[i].pos = D3DXVECTOR3(1, -1, 1);

		// 상 8
		++i; v[i].pos = D3DXVECTOR3(-1, 1, 1);
		++i; v[i].pos = D3DXVECTOR3(1, 1, 1);
		++i; v[i].pos = D3DXVECTOR3(-1, 1, -1);
		++i; v[i].pos = D3DXVECTOR3(1, 1, -1);

		// 하 12
		++i; v[i].pos = D3DXVECTOR3(-1, -1, -1);
		++i; v[i].pos = D3DXVECTOR3(1, -1, -1);
		++i; v[i].pos = D3DXVECTOR3(-1, -1, 1);
		++i; v[i].pos = D3DXVECTOR3(1, -1, 1);

		// 좌 16
		++i; v[i].pos = D3DXVECTOR3(-1, -1, 1);
		++i; v[i].pos = D3DXVECTOR3(-1, -1, -1);
		++i; v[i].pos = D3DXVECTOR3(-1, 1, 1);
		++i; v[i].pos = D3DXVECTOR3(-1, 1, -1);

		// 우 20
		++i; v[i].pos = D3DXVECTOR3(1, -1, -1);
		++i; v[i].pos = D3DXVECTOR3(1, -1, 1);
		++i; v[i].pos = D3DXVECTOR3(1, 1, -1);
		++i; v[i].pos = D3DXVECTOR3(1, 1, 1);



		for (int i = 0; i < 3; ++i)
		{
			D3DCOLOR c;
			switch (i)
			{
			case 0: c = D3DCOLOR_XRGB(255, 0, 0); break;
			case 1: c = D3DCOLOR_XRGB(0, 255, 0); break;
			case 2: c = D3DCOLOR_XRGB(0, 0, 255); break;
			}

			int viewIndex = i * 8;
			for (int j = 0; j < 4; ++j)
				v[viewIndex + j].color = v[viewIndex + j + 4].color = c;
		}
		result->Unlock();
	}

	return result;
}

LPDIRECT3DINDEXBUFFER9 practice_01::createHardCube(void)
{
	LPDIRECT3DINDEXBUFFER9 result = NULL;

	GET_DEVICE_MANAGER()->getDevice()->CreateIndexBuffer(
		sizeof(WORD) * 36,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL::D3DPOOL_MANAGED,
		&result,
		nullptr);

	WORD* index = NULL;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&index, 0)))
	{
		for (int i = 0; i < 6; ++i)
		{
			int viewIndex = i * 6;
			int lowNum = i * 4;
			index[viewIndex] = lowNum;
			index[viewIndex + 1] = lowNum + 1;
			index[viewIndex + 2] = lowNum + 2;
			index[viewIndex + 3] = lowNum + 2;
			index[viewIndex + 4] = lowNum + 1;
			index[viewIndex + 5] = lowNum + 3;
		}

		result->Unlock();
	}

	return result;
}

LPDIRECT3DINDEXBUFFER9 practice_01::createSoftCube(void)
{
	LPDIRECT3DINDEXBUFFER9 result = NULL;

	GET_DEVICE_MANAGER()->getDevice()->CreateIndexBuffer(
		sizeof(WORD) * 36,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL::D3DPOOL_MANAGED,
		&result,
		nullptr);

	WORD* index = NULL;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&index, 0)))
	{
		int i = -1;

		// 전
		++i; index[i] = 0;
		++i; index[i] = 10;
		++i; index[i] = 3;
		++i; index[i] = 3;
		++i; index[i] = 20;
		++i; index[i] = 0;

		// 후
		++i; index[i] = 4;
		++i; index[i] = 14;
		++i; index[i] = 7;
		++i; index[i] = 7;
		++i; index[i] = 23;
		++i; index[i] = 4;

		// 상
		++i; index[i] = 10;
		++i; index[i] = 18;
		++i; index[i] = 9;
		++i; index[i] = 9;
		++i; index[i] = 3;
		++i; index[i] = 10;

		// 하
		++i; index[i] = 14;
		++i; index[i] = 17;
		++i; index[i] = 13;
		++i; index[i] = 13;
		++i; index[i] = 7;
		++i; index[i] = 14;

		// 좌
		++i; index[i] = 16;
		++i; index[i] = 4;
		++i; index[i] = 19;
		++i; index[i] = 19;
		++i; index[i] = 12;
		++i; index[i] = 16;

		// 우
		++i; index[i] = 20;
		++i; index[i] = 3;
		++i; index[i] = 23;
		++i; index[i] = 23;
		++i; index[i] = 15;
		++i; index[i] = 20;

		result->Unlock();
	}

	return result;
}
