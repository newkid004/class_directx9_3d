#include "practice_19.h"

#include "managerList.h"
#include "gFunc.h"

#include "camera.h"
#include "lightBase.h"

practice_19::practice_19(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

practice_19::~practice_19()
{
}

void practice_19::init(void)
{
	direct3dApplication::init();

	direct3dApplication::init();

	_effect = MN_SRC->getEffect("resource/effect/practice_19.fx");

	_renderBack = createRenderTarget();
	_renderTarget = createRenderTarget();
	_renderTargetGlow = createRenderTarget();
	_renderTargetBlur = createRenderTarget();
	
	_depthStensil = createDepthStensil();
	_depthStensilTemp = createDepthStensil();

	_teapot = createTeapot();
	_meshScreen = createMeshScreen();

	_textureDiffuse = MN_SRC->getTexture("resource/texture/GlowTextures/Fieldstone_Alpha.tga");
	_textureNormal = MN_SRC->getTexture("resource/texture/GlowTextures/FieldstoneBumpDOT3.tga");
	_textureBackground = MN_SRC->getTexture("resource/texture/GlowTextures/back.tga");

	GET_LIGHT()->setPosition(D3DXVECTOR3(0.0f, 10.0f, -20.0f));
}

void practice_19::update(void)
{
	direct3dApplication::update();

	D3DXMATRIXA16 mWorld;
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixScaling(&mWorld, 0.1f, 0.1f, 0.1f);

	_effect->SetMatrix("_mWorld", &mWorld);
	_effect->SetMatrix("_mView", GET_CAMERA()->getMatrixViewPoint());
	_effect->SetMatrix("_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	// view
	_effect->SetVector("_viewPosition", &D3DXVECTOR4(GET_CAMERA()->getOffsetPosition(), 1.0f));
	_effect->SetVector("_lightDirection", &D3DXVECTOR4(GET_LIGHT()->getDirectForward(), 0.0f));
	_effect->SetVector("_lightPosition", &D3DXVECTOR4(GET_LIGHT()->getPosition(), 1.0f));

	_effect->SetTexture("_textureDiffuse", _textureDiffuse);
	_effect->SetTexture("_textureNormal", _textureNormal);
	_effect->SetTexture("_textureBackground", _textureBackground);

	_effect->SetTexture("_renderBack", _renderBack);
	_effect->SetTexture("_renderTarget", _renderTarget);
	_effect->SetTexture("_renderTargetGlow", _renderTargetGlow);
	_effect->SetTexture("_renderTargetBlur", _renderTargetBlur);

	static float offsetInterval = 1.0f;
	static float glowInterval = 0.5f;
	if (MN_KEY->keyDown(DIK_LEFT)) offsetInterval -= MN_TIME->getDeltaTime() * 3.0f;
	if (MN_KEY->keyDown(DIK_RIGHT)) offsetInterval += MN_TIME->getDeltaTime() * 3.0f;
	if (MN_KEY->keyDown(DIK_UP)) glowInterval += MN_TIME->getDeltaTime();
	if (MN_KEY->keyDown(DIK_DOWN)) glowInterval -= MN_TIME->getDeltaTime();

	_effect->SetFloat("_offsetInterval", offsetInterval);
	_effect->SetFloat("_glowInterval", glowInterval);
}

void practice_19::draw(void)
{
	// renderTarget : 메쉬
	gFunc::runRenderTarget(_renderTarget, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, _depthStensil, [&](void)->void {
		gFunc::runEffectLoop(_effect, "techDrawMesh", [&](int passNum)->void {
			_teapot->DrawSubset(0);
		});
	}, COLOR_BLACK(0));

	// renderTarget -> renderTargetGlow : 강조
	gFunc::runRenderTarget(_renderTargetGlow, D3DCLEAR_TARGET, _depthStensilTemp, [&](void)->void {
		gFunc::runEffectLoop(_effect, "techGlow", [&](int passNum)->void {
			_meshScreen->DrawSubset(0);
		});
	});

	// renderTargetGlow -> renderTargetBlur : 강조된 부분 블러
	gFunc::runRenderTarget(_renderTargetBlur, D3DCLEAR_TARGET, _depthStensilTemp, [&](void)->void {
		gFunc::runEffectLoop(_effect, "techBlur", [&](int passNum)->void {
			_meshScreen->DrawSubset(0);
		});
	});

	// renderTarget + renderTargetBlur
	gFunc::runRenderTarget(NULL, NULL, _depthStensilTemp, [&](void)->void {
		gFunc::runEffectLoop(_effect, "techBlend", [&](int passNum)->void {
			_meshScreen->DrawSubset(0);
		});
	});

	// 기타 배경 등
	gFunc::runRenderTarget(NULL, NULL, _depthStensil, [&](void)->void {
		gFunc::runEffectLoop(_effect, "techBackground", [&](int passNum)->void {
			_meshScreen->DrawSubset(0);
		});
		direct3dApplication::draw();
	});
}

void practice_19::drawBack(void)
{
}

LPDIRECT3DTEXTURE9 practice_19::createRenderTarget(void)
{
	LPDIRECT3DTEXTURE9 result = nullptr;

	MN_DEV->CreateTexture(
		GET_WINDOW_SIZE().cx,
		GET_WINDOW_SIZE().cy,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8B8G8R8,
		D3DPOOL_DEFAULT,
		&result,
		NULL);

	return result;
}

LPDIRECT3DSURFACE9 practice_19::createDepthStensil(void)
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

LPD3DXMESH practice_19::createTeapot(void)
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

LPD3DXMESH practice_19::createMeshScreen(void)
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
