#include "practice_11.h"

#include "managerList.h"
#include "cameraControlable.h"
#include "gFunc.h"

#include "debugGrid.h"
#include "lightBase.h"

practice_11::practice_11(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option),
	_heightScale(0.2f)
{
	_mapSize = { 512, 512 };
	_cellSize = { 1, 1 };

	_totalSize.cx = _mapSize.cx * _cellSize.cx;
	_totalSize.cy = _mapSize.cy * _cellSize.cy;
}

practice_11::~practice_11()
{
	SAFE_RELEASE(_mesh);
	SAFE_RELEASE(_meshNormalize);
}

void practice_11::init(void)
{
	direct3dApplication::init();

	_texture = createTexture();
	_viewTexture = createViewTexture();

	_mesh = createMesh();
	normalizeHeight();
	_meshNormalize = createMesh();

	_currentMesh = _mesh;

	cameraControlable* c = (cameraControlable*)_camera;
	c->setVelocity(c->getVelocity() * 10.0f);
}

void practice_11::update(void)
{
	direct3dApplication::update();

	MN_DEV->SetRenderState(D3DRS_LIGHTING, true);

	updateControl();
}

void practice_11::draw(void)
{
	direct3dApplication::draw();

	D3DXMATRIXA16 stWorldMatrix;
	D3DXMatrixIdentity(&stWorldMatrix);

	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(material));

	material.Diffuse = D3DXCOLOR(COLOR_WHITE(255)) * 10.0f;
	MN_DEV->SetMaterial(&material);

	// 텍스쳐 적용
	MN_DEV->SetTexture(0, _viewTexture);

	MN_DEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	MN_DEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	MN_DEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	MN_DEV->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	MN_DEV->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);

	_currentMesh->DrawSubset(0);
}

void practice_11::drawUI(void)
{
	direct3dApplication::drawUI();
}

void practice_11::updateControl(void)
{
	if (MN_KEY->keyDown(DIK_LEFT)) GET_LIGHT()->rotateY(-90.0f * MN_TIME->getDeltaTime(), false);
	if (MN_KEY->keyDown(DIK_RIGHT)) GET_LIGHT()->rotateY(90.0f * MN_TIME->getDeltaTime(), false);

	if (MN_KEY->keyPress(DIK_F1))
	{
		if (_currentMesh == _mesh)	_currentMesh = _meshNormalize;
		else						_currentMesh = _mesh;
	}
}

LPD3DXMESH practice_11::createMesh(void)
{
	LPD3DXMESH result = nullptr;

	D3DXCreateMeshFVF(
		_mapSize.cx * _mapSize.cy * 2,
		_mapSize.cx * _mapSize.cy,
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		vertex::FVF,
		MN_DEV,
		&result);

	vertex* v = nullptr;
	if (SUCCEEDED(result->LockVertexBuffer(0, (void**)&v)))
	{
		D3DXVECTOR3 basePos = D3DXVECTOR3(
			(_totalSize.cx / -2.0f) + (_cellSize.cx / 2.0),
			0,
			(_totalSize.cy / 2.0f) - (_cellSize.cy / 2.0));

		for (int y = 0; y < _mapSize.cy; ++y)
		{
			for (int x = 0; x < _mapSize.cx; ++x)
			{
				int index = y * _mapSize.cx + x;

				v[index].position = basePos + D3DXVECTOR3(
					x * _cellSize.cx,
					_vHeightList[index],
					y * -_cellSize.cy);

				v[index].uv = D3DXVECTOR2((float)x / _mapSize.cx, (float)y / _mapSize.cy);
			}
		}
		result->UnlockVertexBuffer();
	}

	DWORD* i = nullptr;
	if (SUCCEEDED(result->LockIndexBuffer(0, (void**)&i)))
	{
		for (int y = 0; y < _mapSize.cy - 1; ++y)
		{
			for (int x = 0; x < _mapSize.cx - 1; ++x)
			{
				int vertexIndex = y * _mapSize.cx + x;
				int index = vertexIndex * 6;

				// 좌
				i[index + 0] = vertexIndex;
				i[index + 1] = vertexIndex + 1;
				i[index + 2] = vertexIndex + _mapSize.cx;

				// 우
				i[index + 3] = vertexIndex + _mapSize.cx;
				i[index + 4] = vertexIndex + 1;
				i[index + 5] = vertexIndex + _mapSize.cx + 1;
			}
		}
		result->UnlockIndexBuffer();
	}

	D3DXComputeNormals(result, NULL);

	return result;
}

LPDIRECT3DTEXTURE9 practice_11::createTexture(void)
{
	LPDIRECT3DTEXTURE9 result = MN_SRC->getTexture("resource/texture/terrain/HeightMap.bmp");

	// 높이 정보 추출
	// bits : 색상 값
	// pitch : 각 픽셀의 색상 값 크기(byte)
	D3DLOCKED_RECT rcLocked;
	ZeroMemory(&rcLocked, sizeof(rcLocked));

	if (SUCCEEDED(result->LockRect(0, &rcLocked, NULL, 0)))
	{
		D3DCOLOR* cByte = (D3DCOLOR*)rcLocked.pBits;

		for (int y = 0; y < _mapSize.cy; ++y)
		{
			for (int x = 0; x < _mapSize.cx; ++x)
			{
				int index = y * _mapSize.cx + x;
				D3DCOLOR color = cByte[index];

				int colorR = (color >> 0x10) & 0xff;
				int colorG = (color >> 0x08) & 0xff;
				int colorB = (color >> 0x00) & 0xff;

				int height = colorR + colorG + colorB;
				_vHeightList.push_back(height * _heightScale);
			}
		}
		result->UnlockRect(0);
	}

	return result;
}

LPDIRECT3DTEXTURE9 practice_11::createViewTexture(void)
{
	auto result = MN_SRC->getTexture("resource/texture/bonobono.png");

	return result;
}

void practice_11::normalizeHeight(void)
{
	float ratio = 1.0f;

	vector<float> vNormalize(_vHeightList);
	for (int y = 1; y < _mapSize.cy - 1; ++y)
	{
		for (int x = 1; x < _mapSize.cx - 1; ++x)
		{
			int index = x * _mapSize.cy + y;

			float sizeHeight[4] = {
				vNormalize[index - 1] ,
				vNormalize[index + 1] ,
				vNormalize[index - _mapSize.cx] ,
				vNormalize[index + _mapSize.cx] };

			float normalHeight = 0;
			for (int i = 0; i < 4; ++i)
				normalHeight += sizeHeight[i] / 4.0f;

			_vHeightList[index] =
				normalHeight * ratio +
				_vHeightList[index] * (1.0f - ratio);
		}
	}
}
