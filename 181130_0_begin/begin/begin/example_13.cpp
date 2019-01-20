#include "example_13.h"

#include "managerList.h"
#include "camera.h"
#include "gFunc.h"

#define EXAMPLE_TYPE_ALPHABLENDING	1
#define EXAMPLE_TYPE_SPRITE			2

#define EXAPLE_TYPE					EXAMPLE_TYPE_SPRITE

example_13::example_13(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_13::~example_13()
{
	SAFE_RELEASE(_mesh[0]);
	SAFE_RELEASE(_mesh[1]);

	SAFE_RELEASE(_effect);
	SAFE_RELEASE(_sprite);
	SAFE_RELEASE(_texture);
}

void example_13::init(void)
{
	direct3dApplication::init();

	_mesh[0] = createMesh();
	_mesh[1] = createMesh();

	_effect = createEffect();
	_sprite = createSprite();
	_texture = createTexture();
}

void example_13::update(void)
{
	direct3dApplication::update();
}

void example_13::draw(void)
{
	direct3dApplication::draw();

#if EXAPLE_TYPE == EXAMPLE_TYPE_ALPHABLENDING
	// 행렬
	_effect->SetMatrix("_mView", GET_CAMERA()->getMatrixViewPoint());
	_effect->SetMatrix("_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	// mesh 0
	gFunc::runEffectLoop(_effect, "myTechnique", [&](int index)->void {
		// 위치
		D3DXMATRIXA16 mWorld;
		D3DXMatrixIdentity(&mWorld);
		_effect->SetMatrix("_mWorld", &mWorld);

		// 색상
		D3DXVECTOR4 diffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
		_effect->SetVector("_diffuseColor", &diffuseColor);

		_effect->CommitChanges();

		_mesh[0]->DrawSubset(0);
	});

	/*
		< 알파 블렌딩 연산자 >
		srcAlpha		: 픽셀 셰이더에서 출력 된 색상이 지니고 있는 알파 값
		invSrcAlpha		: 1.0 - srcAlpha
		one				: 1.0
		zero			: 0
		destAlpha		: 버퍼에 있는 색상이 지니고 있는 알파 값
		invDestAlpha	: 1.0 - destAlpha
	*/
	// 알파 블렌딩
	MN_DEV->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	MN_DEV->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	MN_DEV->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// mesh 1
	gFunc::runEffectLoop(_effect, "myTechnique", [&](int index)->void {
		// 위치
		D3DXMATRIXA16 mWorld;
		D3DXMatrixTranslation(&mWorld, 0, 0, -0.25f);

		// 크기
		D3DXMATRIXA16 mScale;
		D3DXMatrixScaling(&mScale, 2.0f, 2.0f, 1.0f);

		mWorld = mScale * mWorld;
		_effect->SetMatrix("_mWorld", &mWorld);

		// 색상
		D3DXVECTOR4 diffuseColor(0.0f, 1.0f, 0.0f, 0.9f);
		_effect->SetVector("_diffuseColor", &diffuseColor);

		_effect->CommitChanges();

		_mesh[1]->DrawSubset(0);
	});

	MN_DEV->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	
#else

	_sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT);
	{
		RECT rcTexture = { 0, 0, 200, 100 };
		_sprite->Draw(_texture, 
			&rcTexture,
			&D3DXVECTOR3(rcTexture.right / 2.0f, rcTexture.bottom / 2.0f, 0),
			&D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0),
			D3DCOLOR_XRGB(255, 255, 255));
	}
	_sprite->End();

#endif	// EXAPLE_TYPE == EXAMPLE_TYPE_ALPHABLENDING

}

LPD3DXMESH example_13::createMesh(void)
{
	LPD3DXMESH result = nullptr;

	D3DXCreateMeshFVF(
		sizeof(WORD) * 2,
		sizeof(vertex) * 4,
		D3DXMESH_MANAGED,
		vertex::FVF,
		MN_DEV,
		&result);

	// vertex
	vertex* v = nullptr;
	if (SUCCEEDED(result->LockVertexBuffer(0, (void**)&v)))
	{
		v[0].pos = D3DXVECTOR3(-1, -1, 0);
		v[1].pos = D3DXVECTOR3(-1,  1, 0);
		v[2].pos = D3DXVECTOR3( 1,  1, 0);
		v[3].pos = D3DXVECTOR3( 1, -1, 0);

		v[0].diffuse = D3DCOLOR_XRGB(255, 255, 255);
		v[1].diffuse = D3DCOLOR_XRGB(255, 255, 255);
		v[2].diffuse = D3DCOLOR_XRGB(255, 255, 255);
		v[3].diffuse = D3DCOLOR_XRGB(255, 255, 255);

		result->UnlockVertexBuffer();
	}

	// index
	WORD* i = nullptr;
	if (SUCCEEDED(result->LockIndexBuffer(0, (void**)&i)))
	{
		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 2; i[4] = 3; i[5] = 0;

		result->UnlockIndexBuffer();
	}

	return result;
}

LPD3DXEFFECT example_13::createEffect(void)
{
	LPD3DXEFFECT result = nullptr;
	LPD3DXBUFFER bufError = nullptr;

	D3DXCreateEffectFromFile(
		MN_DEV,
		TEXT("resource/effect/example_13.fx"),
		NULL,
		NULL,
		D3DXSHADER_DEBUG,
		NULL,
		&result,
		&bufError);

	if (bufError != nullptr)
	{
		auto msgError = (char*)bufError->GetBufferPointer();
		printf("example_13.createEffect : %s\n", msgError);
	}

	return result;
}

LPD3DXSPRITE example_13::createSprite(void)
{
	LPD3DXSPRITE result = nullptr;

	D3DXCreateSprite(MN_DEV, &result);

	return result;
}

LPDIRECT3DTEXTURE9 example_13::createTexture(void)
{
	LPDIRECT3DTEXTURE9 result = nullptr;

	char* filePath = "resource/texture/textureGradiant.png";

	/*/
	D3DXCreateTextureFromFile(
		MN_DEV,
		TEXT(filePath),
		&result);

	/*/

	D3DXCreateTextureFromFileEx(
		MN_DEV,
		TEXT(filePath),
		D3DX_DEFAULT_NONPOW2, 
		D3DX_DEFAULT_NONPOW2,
		1, 
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0),
		NULL,
		NULL,
		&result);
	
	//*/
	return result;
}