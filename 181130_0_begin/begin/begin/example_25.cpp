#include "example_25.h"

#include "managerList.h"
#include "gFunc.h"

#include "camera.h"
#include "lightBase.h"

example_25::example_25(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

example_25::~example_25()
{
}

void example_25::init(void)
{
	direct3dApplication::init();

	_effect = MN_SRC->getEffect("resource/effect/example_25.fx");
	_renderTarget = createRenderTarget();
	_renderTargetGlow = createRenderTarget();
	_depthStensil = createDepthStensil();
	
	_teapot = createTeapot();
	_meshScreen = createMeshScreen();

	_textureDiffuse = MN_SRC->getTexture("resource/texture/GlowTextures/Fieldstone.tga");
	_textureNormal = MN_SRC->getTexture("resource/texture/GlowTextures/FieldstoneBumpDOT3.tga");
	_textureBackground = MN_SRC->getTexture("resource/texture/GlowTextures/back.tga");

	GET_LIGHT()->setPosition(D3DXVECTOR3(0.0f, 10.0f, -20.0f));
}

void example_25::update(void)
{
	direct3dApplication::update();

	D3DXMATRIXA16 mWorld;
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixScaling(&mWorld, 0.2f, 0.2f, 0.2f);

	_effect->SetMatrix("_mWorld", &mWorld);
	_effect->SetMatrix("_mView", GET_CAMERA()->getMatrixViewPoint());
	_effect->SetMatrix("_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	// view
	_effect->SetVector("_viewPosition", &D3DXVECTOR4(GET_CAMERA()->getOffsetPosition(), 1.0f));
	_effect->SetVector("_lightDirection", &D3DXVECTOR4(GET_LIGHT()->getDirectForward(), 0.0f));
	_effect->SetVector("_lightPosition", &D3DXVECTOR4(GET_LIGHT()->getPosition(), 1.0f));

	_effect->SetTexture("_textureDiffuse", _textureDiffuse);
	_effect->SetTexture("_textureNormal", _textureNormal);

	_effect->SetTexture("_renderTarget", _renderTarget);
	_effect->SetTexture("_renderTargetGlow", _renderTargetGlow);

	_effect->SetFloat("_time", MN_TIME->getRunningTime());
}

void example_25::draw(void)
{
	direct3dApplication::draw();
}

LPDIRECT3DTEXTURE9 example_25::createRenderTarget(void)
{
	LPDIRECT3DTEXTURE9 result = nullptr;

	MN_DEV->CreateTexture(
		GET_WINDOW_SIZE().cx,
		GET_WINDOW_SIZE().cy,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_R32F,
		D3DPOOL_DEFAULT,
		&result,
		NULL);

	return result;
}

LPDIRECT3DSURFACE9 example_25::createDepthStensil(void)
{
	LPDIRECT3DSURFACE9 result = nullptr;

	MN_DEV->CreateDepthStencilSurface(
		GET_WINDOW_SIZE().cx,
		GET_WINDOW_SIZE().cy,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&result,
		NULL);

	return result;
}

LPD3DXMESH example_25::createTeapot(void)
{
	LPD3DXMESH result = nullptr;

	D3DXLoadMeshFromX(TEXT("resource/mesh/teapot5.x"),
		D3DXMESH_MANAGED,
		MN_DEV,
		NULL,
		NULL,
		NULL,
		NULL,
		&result);

	return result;
}

LPD3DXMESH example_25::createMeshScreen(void)
{
	LPD3DXMESH result = nullptr;

	D3DXCreateMeshFVF(
		2, 4,
		D3DXMESH_MANAGED,
		vertex::FVF,
		MN_DEV,
		&result);

	// vertex
	vertex* v = nullptr;
	if (SUCCEEDED(result->LockVertexBuffer(0, (void**)&v)))
	{
		v[0].pos = D3DXVECTOR3(-1, -1, 0);
		v[1].pos = D3DXVECTOR3(-1, 1, 0);
		v[2].pos = D3DXVECTOR3(1, 1, 0);
		v[3].pos = D3DXVECTOR3(1, -1, 0);

		v[0].uv = D3DXVECTOR2(0, 1);
		v[1].uv = D3DXVECTOR2(0, 0);
		v[2].uv = D3DXVECTOR2(1, 0);
		v[3].uv = D3DXVECTOR2(1, 1);

		result->UnlockVertexBuffer();
	}

	// index
	WORD* index = nullptr;
	if (SUCCEEDED(result->LockIndexBuffer(0, (void**)&index)))
	{
		index[0] = 0; index[1] = 1; index[2] = 2;
		index[3] = 0; index[4] = 2; index[5] = 3;

		result->UnlockIndexBuffer();
	}

	return result;
}
