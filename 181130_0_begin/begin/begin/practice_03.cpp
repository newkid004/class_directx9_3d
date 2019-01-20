#include "practice_03.h"
#include "managerList.h"

practice_03::practice_03(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

practice_03::~practice_03()
{
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_texture);
}

void practice_03::init(void)
{
	direct3dApplication::init();

	_vertexBuffer = createVertexBuffer();
	_texture = createTexture();
}

void practice_03::update(void)
{
	direct3dApplication::update();

	updateUV();
	updateControl();
}

void practice_03::draw(void)
{
	direct3dApplication::draw();

	MN_DEV->SetRenderState(D3DRS_LIGHTING, false);
	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_NONE);

	// -------------------- //

	MN_DEV->SetStreamSource(0, _vertexBuffer, 0, sizeof(vertex));
	MN_DEV->SetFVF(vertex::FVF);

	// 텍스쳐는 스테이지 개념으로서 렌더링 파이프라인에서 이용하기 위해 대기 상태를 성정하는 것이 가능
	// -> 여러 텍스쳐를 하나로 혼합하는것이 가능
	MN_DEV->SetTexture(0, _texture);

	// 필터링
	MN_DEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	MN_DEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	MN_DEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// 어드레스 모드
	MN_DEV->SetSamplerState(0, D3DSAMP_ADDRESSV, _addressMode);
	MN_DEV->SetSamplerState(0, D3DSAMP_ADDRESSU, _addressMode);

	MN_DEV->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	// -------------------- //

	MN_DEV->SetRenderState(D3DRS_LIGHTING, true);
	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_CCW);
}

void practice_03::updateUV(void)
{
	vertex* v = NULL;
	if (SUCCEEDED(_vertexBuffer->Lock(0, 0, (void**)&v, 0)))
	{
		v[0].UV = D3DXVECTOR2(0, 1);
		v[1].UV = D3DXVECTOR2(0, 0);
		v[2].UV = D3DXVECTOR2(1, 0);

		v[3].UV = D3DXVECTOR2(0, 1);
		v[4].UV = D3DXVECTOR2(1, 0);
		v[5].UV = D3DXVECTOR2(1, 1);

		if (_isTiled)
		{
			for (int i = 0; i < 6; ++i)
			{
				v[i].UV.x *= _setUV.x;
				v[i].UV.y *= _setUV.y;
			}
		}

		if (!_isTiled)
		{
			for (int i = 0; i < 6; ++i)
				v[i].UV += _offsetUV;
		}

		_vertexBuffer->Unlock();
	}
}

void practice_03::updateControl(void)
{
	// uv divide
	if (MN_KEY->keyPress(DIK_LEFT))		_setUV.x = 1.0f < _setUV.x ? _setUV.x - 1 : 1.0f;
	if (MN_KEY->keyPress(DIK_RIGHT))	_setUV.x = _setUV.x < 5.0f ? _setUV.x + 1 : 5.0f;
	if (MN_KEY->keyPress(DIK_UP))		_setUV.y = 1.0f < _setUV.y ? _setUV.y - 1 : 1.0f;
	if (MN_KEY->keyPress(DIK_DOWN))		_setUV.y = _setUV.y < 5.0f ? _setUV.y + 1 : 5.0f;

	// address
	if (MN_KEY->keyPress(DIK_1))		_addressMode = 1;
	if (MN_KEY->keyPress(DIK_2))		_addressMode = 2;
	if (MN_KEY->keyPress(DIK_3))		_addressMode = 3;

	// offset
	if (MN_KEY->keyDown(DIK_W))			_offsetUV.y += MN_TIME->getDeltaTime();
	if (MN_KEY->keyDown(DIK_S))			_offsetUV.y -= MN_TIME->getDeltaTime();
	if (MN_KEY->keyDown(DIK_A))			_offsetUV.x -= MN_TIME->getDeltaTime();
	if (MN_KEY->keyDown(DIK_D))			_offsetUV.x += MN_TIME->getDeltaTime();

	// tile
	if (MN_KEY->keyPress(DIK_SPACE))	_isTiled = !_isTiled;
}

LPDIRECT3DVERTEXBUFFER9 practice_03::createVertexBuffer(void)
{
	LPDIRECT3DVERTEXBUFFER9 result = NULL;
	MN_DEV->CreateVertexBuffer(sizeof(vertex) * 6,
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL_MANAGED,
		&result,
		NULL);

	vertex* v = NULL;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&v, 0)))
	{
		v[0].pos = D3DXVECTOR3(-1, -1, 0);
		v[1].pos = D3DXVECTOR3(-1, 1, 0);
		v[2].pos = D3DXVECTOR3(1, 1, 0);

		v[3].pos = D3DXVECTOR3(-1, -1, 0);
		v[4].pos = D3DXVECTOR3(1, 1, 0);
		v[5].pos = D3DXVECTOR3(1, -1, 0);

		v[0].UV = D3DXVECTOR2(0, 10);
		v[1].UV = D3DXVECTOR2(0, 0);
		v[2].UV = D3DXVECTOR2(10, 0);

		v[3].UV = D3DXVECTOR2(0, 10);
		v[4].UV = D3DXVECTOR2(10, 0);
		v[5].UV = D3DXVECTOR2(10, 10);

		result->Unlock();
	}
	return result;
}

LPDIRECT3DTEXTURE9 practice_03::createTexture(void)
{
	LPDIRECT3DTEXTURE9 result = NULL;

	D3DXCreateTextureFromFile(MN_DEV,
		TEXT("resource/texture/texture.png"),
		&result);

	return result;
}
