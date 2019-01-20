#include "practice_04.h"
#include "managerList.h"
#include "baseObject.h"

practice_04::practice_04(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

practice_04::~practice_04()
{
	for (auto v : _vertexBuffer)
		SAFE_RELEASE(v);

	for (auto obj : _objFigure)
		SAFE_DELETE(obj);
}

void practice_04::init(void)
{
	direct3dApplication::init();

	_vertexBuffer[0] = createVertex2Cube();
	_vertexBuffer[1] = createVertex2Horn();
	_vertexBuffer[2] = createVertex2Octa();

	baseObject** viewObject;
	viewObject = &_objFigure[0]; *viewObject = new baseObject; (*viewObject)->rotate2Pos(D3DXVECTOR3(0.3, 0.3, -5)); (*viewObject)->moveX(3.0f, false);
	viewObject = &_objFigure[1]; *viewObject = new baseObject; (*viewObject)->rotate2Pos(D3DXVECTOR3(0.3, 0.3, -5));
	viewObject = &_objFigure[2]; *viewObject = new baseObject; (*viewObject)->rotate2Pos(D3DXVECTOR3(0.3, 0.3, -5)); (*viewObject)->moveX(-3.0f, false);

	ZeroMemory(&_light, sizeof(_light));
	_light.Type = D3DLIGHTTYPE::D3DLIGHT_POINT;
	_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	_light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	_light.Direction = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	_light.Position = D3DXVECTOR3(-0.3f, 5.0f, -3.0f);
	_light.Attenuation0 = 0.025f;
	_light.Attenuation1 = 0.025f;
	_light.Attenuation2 = 0.025f;
	_light.Range = 8.0f;
	// D3DXVec3Normalize(&_light.Direction, &_light.Direction);

	initMaterial();
}

void practice_04::update(void)
{
	direct3dApplication::update();

	_yaw += 90.0f * MN_TIME->getDeltaTime();

	for (auto obj : _objFigure)
		obj->update();

	_objFigure[0]->rotateY(30.0f * MN_TIME->getDeltaTime(), false);
	_objFigure[1]->rotateY(30.0f * MN_TIME->getDeltaTime(), false);
	_objFigure[2]->rotateY(30.0f * MN_TIME->getDeltaTime(), false);

	updateControl();
}

void practice_04::draw(void)
{
	direct3dApplication::draw();

	// 정규화
	MN_DEV->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	MN_DEV->SetFVF(vertex::FVF);

	MN_DEV->LightEnable(0, true);
	MN_DEV->SetLight(0, &_light);

	int indexMaterial = _offsetMaterial;
	int indexShader = _offsetShader;

	setMaterial(indexMaterial); setShader(indexShader); drawCube(); indexMaterial = (indexMaterial + 1) % 3; indexShader = (indexShader + 1) % 3;
	setMaterial(indexMaterial); setShader(indexShader); drawHorn(); indexMaterial = (indexMaterial + 1) % 3; indexShader = (indexShader + 1) % 3;
	setMaterial(indexMaterial); setShader(indexShader); drawOcta(); indexMaterial = (indexMaterial + 1) % 3; indexShader = (indexShader + 1) % 3;
}

void practice_04::initMaterial(void)
{
	for (int i = 0; i < 3; ++i)
		ZeroMemory(&_material[i], sizeof(_material[i]));

	D3DMATERIAL9* viewMaterial;
	viewMaterial = &_material[0];
	viewMaterial->Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	viewMaterial->Ambient = D3DXCOLOR(0.0f, 0.33f, 0.0f, 1.0f);

	viewMaterial = &_material[1];
	viewMaterial->Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	viewMaterial->Ambient = D3DXCOLOR(0.0f, 0.0f, 0.33f, 1.0f);

	viewMaterial = &_material[2];
	viewMaterial->Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	viewMaterial->Ambient = D3DXCOLOR(0.33f, 0.0f, 0.0f, 1.0f);
}

void practice_04::updateControl(void)
{
	if (MN_KEY->keyPress(DIK_LEFT))		_offsetMaterial = (_offsetMaterial + 2) % 3;
	if (MN_KEY->keyPress(DIK_RIGHT))	_offsetMaterial = (_offsetMaterial + 1) % 3;
	if (MN_KEY->keyPress(DIK_UP))		_offsetShader = (_offsetShader + 1) % 3;
	if (MN_KEY->keyPress(DIK_DOWN))		_offsetShader = (_offsetShader + 2) % 3;
}

void practice_04::setMaterial(int index)
{
	switch (index)
	{
	case 0: MN_DEV->SetRenderState(D3DRS_SHADEMODE, D3DSHADEMODE::D3DSHADE_FLAT);	 break;
	case 1: MN_DEV->SetRenderState(D3DRS_SHADEMODE, D3DSHADEMODE::D3DSHADE_PHONG);	 break;
	case 2: MN_DEV->SetRenderState(D3DRS_SHADEMODE, D3DSHADEMODE::D3DSHADE_GOURAUD); break;
	}
}

void practice_04::setShader(int index)
{
	switch (index)
	{
	case 0: MN_DEV->SetMaterial(&_material[0]); break;
	case 1: MN_DEV->SetMaterial(&_material[1]); break;
	case 2: MN_DEV->SetMaterial(&_material[2]); break;
	}
}

void practice_04::drawCube(void)
{
	LPDIRECT3DVERTEXBUFFER9 & vBuffer = _vertexBuffer[0];
	baseObject* objFigure = _objFigure[0];

	MN_DEV->SetTransform(D3DTS_WORLD, &objFigure->getMatrixWorld());

	MN_DEV->SetStreamSource(0, vBuffer, 0, sizeof(vertex));
	MN_DEV->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 36);
}

void practice_04::drawHorn(void)
{
	LPDIRECT3DVERTEXBUFFER9 & vBuffer = _vertexBuffer[1];
	baseObject* objFigure = _objFigure[1];

	MN_DEV->SetTransform(D3DTS_WORLD, &objFigure->getMatrixWorld());

	MN_DEV->SetStreamSource(0, vBuffer, 0, sizeof(vertex));
	MN_DEV->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 12);
}

void practice_04::drawOcta(void)
{
	LPDIRECT3DVERTEXBUFFER9 & vBuffer = _vertexBuffer[2];
	baseObject* objFigure = _objFigure[2];

	MN_DEV->SetTransform(D3DTS_WORLD, &objFigure->getMatrixWorld());

	MN_DEV->SetStreamSource(0, vBuffer, 0, sizeof(vertex));
	MN_DEV->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 24);
}

LPDIRECT3DVERTEXBUFFER9 practice_04::createVertex2Cube(void)
{
	LPDIRECT3DVERTEXBUFFER9 result = NULL;

	MN_DEV->CreateVertexBuffer(
		sizeof(vertex) * 36,
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL_MANAGED,
		&result,
		NULL);

	vertex* v;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&v, 0)))
	{
		D3DXVECTOR3 vertices[] =
		{
			// 왼쪽
			D3DXVECTOR3(-1, -1, -1),
			D3DXVECTOR3(-1,  1, -1),
			D3DXVECTOR3(1,  1, -1),

			// 오른쪽
			D3DXVECTOR3(-1, -1, -1),
			D3DXVECTOR3(1,  1, -1),
			D3DXVECTOR3(1, -1, -1),
		};
		D3DXVECTOR3 normal = D3DXVECTOR3(0, 0, -1);

		for (int i = 0; i < 6; ++i)
		{
			D3DXVECTOR3 cVertex[6];
			D3DXVECTOR3 cNormal;

			// 회전 행렬 설정
			D3DXMATRIXA16 mRotation;
			if (i < 4)	D3DXMatrixRotationY(&mRotation, D3DXToRadian(i * 90.f));
			else		D3DXMatrixRotationX(&mRotation, D3DXToRadian(90.f + ((i - 4) * 180.f)));

			D3DXVec3TransformCoordArray(cVertex,
				sizeof(D3DXVECTOR3),
				vertices,
				sizeof(D3DXVECTOR3),
				&mRotation,
				sizeof(vertices) / sizeof(D3DXVECTOR3));

			D3DXVec3TransformCoord(&cNormal, &normal, &mRotation);

			// 정점 정보 설정
			int index = i * sizeof(cVertex) / sizeof(D3DXVECTOR3);

			for (int j = 0; j < 6; ++j)
			{
				v[index + j].pos = cVertex[j];
				v[index + j].normal = cVertex[j];
			}
		}

		result->Unlock();
	}

	return result;
}

LPDIRECT3DVERTEXBUFFER9 practice_04::createVertex2Horn(void)
{
	LPDIRECT3DVERTEXBUFFER9 result = NULL;

	MN_DEV->CreateVertexBuffer(
		sizeof(vertex) * 12,
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL_MANAGED,
		&result,
		NULL);

	vertex* v;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&v, 0)))
	{
		D3DXVECTOR3 vertices[] =
		{
			D3DXVECTOR3(-1, -1, -1),
			D3DXVECTOR3(0,  1, 0),
			D3DXVECTOR3( 1, -1, -1),
		};
		D3DXVECTOR3 normals[] =
		{
			D3DXVECTOR3(-1, 1, -1),
			D3DXVECTOR3(0,  1, 0),
			D3DXVECTOR3(1,  1, -1),
		};

		for (int i = 0; i < 4; ++i)
		{
			D3DXVECTOR3 cVertex[3];
			D3DXVECTOR3 cNormal[3];

			// 회전 행렬 설정
			D3DXMATRIXA16 mRotation;
			D3DXMatrixRotationY(&mRotation, D3DXToRadian(i * 90.f));

			D3DXVec3TransformCoordArray(cVertex,
				sizeof(D3DXVECTOR3),
				vertices,
				sizeof(D3DXVECTOR3),
				&mRotation,
				sizeof(vertices) / sizeof(D3DXVECTOR3));

			D3DXVec3TransformCoordArray(cNormal,
				sizeof(D3DXVECTOR3),
				normals,
				sizeof(D3DXVECTOR3),
				&mRotation,
				sizeof(normals) / sizeof(D3DXVECTOR3));

			// 정점 정보 설정
			int index = i * sizeof(cVertex) / sizeof(D3DXVECTOR3);

			for (int j = 0; j < 3; ++j)
			{
				v[index + j].pos = cVertex[j];
				v[index + j].normal = cNormal[j];
			}
		}

		result->Unlock();
	}

	return result;
}

LPDIRECT3DVERTEXBUFFER9 practice_04::createVertex2Octa(void)
{
	LPDIRECT3DVERTEXBUFFER9 result = NULL;

	MN_DEV->CreateVertexBuffer(
		sizeof(vertex) * 24,
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL_MANAGED,
		&result,
		NULL);

	vertex* v;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&v, 0)))
	{
		// 전면 ↙
		D3DXVECTOR3 vertices[] =
		{
			D3DXVECTOR3(-1, 0, 0),
			D3DXVECTOR3(0, 0, -1),
			D3DXVECTOR3(0, -1, 0),
		};

		for (int i = 0; i < 4; ++i)
		{
			D3DXVECTOR3 cVertex[3];

			// 회전 행렬 설정
			D3DXMATRIXA16 mRotation;
			D3DXMatrixRotationY(&mRotation, D3DXToRadian(i * 90.f));

			D3DXVec3TransformCoordArray(
				cVertex, sizeof(D3DXVECTOR3), 
				vertices, sizeof(D3DXVECTOR3),
				&mRotation, sizeof(vertices) / sizeof(D3DXVECTOR3));

			// 정점 정보 설정
			int index = i * sizeof(cVertex) / sizeof(D3DXVECTOR3);

			for (int j = 0; j < 3; ++j)
			{
				v[index + j].pos = cVertex[j];
				v[index + j].normal = cVertex[j];
			}
		}

		// 반전
		D3DXMATRIXA16 mRotationFlip;
		D3DXMatrixRotationZ(&mRotationFlip, D3DXToRadian(180.0f));
		D3DXVec3TransformCoordArray(
			vertices, sizeof(D3DXVECTOR3),
			vertices, sizeof(D3DXVECTOR3),
			&mRotationFlip, sizeof(vertices) / sizeof(D3DXVECTOR3));

		for (int i = 0; i < 4; ++i)
		{
			D3DXVECTOR3 cVertex[3];

			// 회전 행렬 설정
			D3DXMATRIXA16 mRotation;
			D3DXMatrixRotationY(&mRotation, D3DXToRadian(i * 90.f));

			D3DXVec3TransformCoordArray(
				cVertex, sizeof(D3DXVECTOR3),
				vertices, sizeof(D3DXVECTOR3),
				&mRotation, sizeof(vertices) / sizeof(D3DXVECTOR3));

			// 정점 정보 설정
			int index = (4 + i) * sizeof(cVertex) / sizeof(D3DXVECTOR3);

			for (int j = 0; j < 3; ++j)
			{
				v[index + j].pos = cVertex[j];
				v[index + j].normal = cVertex[j];
			}
		}

		result->Unlock();
	}

	return result;
}
