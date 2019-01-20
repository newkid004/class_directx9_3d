#include "example_24.h"

#include "managerList.h"
#include "gFunc.h"

#include "camera.h"
#include "lightBase.h"

example_24::example_24(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

example_24::~example_24()
{
	SAFE_RELEASE(_meshDisc);
	SAFE_RELEASE(_meshTorus);

	SAFE_RELEASE(_depthStensil);
	SAFE_RELEASE(_renderTarget);
}

void example_24::init(void)
{
	direct3dApplication::init();

	_effect = MN_SRC->getEffect("resource/effect/example_24.fx");
	_meshDisc = createDisc();
	_meshTorus = createTorus();
	_renderTarget = createRenderTarget();

	MN_DEV->GetDepthStencilSurface(&_depthStensil);

	GET_LIGHT()->setPosition(D3DXVECTOR3(0, 20, -20));
	GET_LIGHT()->rotate2Pos(D3DXVECTOR3(0, 0, 0));

	_torusPos = D3DXVECTOR3(0, 0, 0);
}

void example_24::update(void)
{
	direct3dApplication::update();

	if (MN_KEY->keyDown(DIK_UP))	_torusPos.z += 3.0f * MN_TIME->getDeltaTime();
	if (MN_KEY->keyDown(DIK_DOWN))	_torusPos.z -= 3.0f * MN_TIME->getDeltaTime();
	if (MN_KEY->keyDown(DIK_LEFT))	_torusPos.x -= 3.0f * MN_TIME->getDeltaTime();
	if (MN_KEY->keyDown(DIK_RIGHT)) _torusPos.x += 3.0f * MN_TIME->getDeltaTime();
}

void example_24::draw(void)
{
	static float rotate;
	rotate += MN_TIME->getDeltaTime() * 90.0f;

	// 행렬
	D3DXMATRIXA16 mRotate;
	D3DXMatrixIdentity(&mRotate);
	D3DXMatrixRotationY(&mRotate, D3DXToRadian(rotate));

	D3DXMATRIXA16 mScaleDisc;
	D3DXMATRIXA16 mScaleTorus;
	D3DXMatrixScaling(&mScaleDisc, 0.1f, 0.1f, 0.1f);
	D3DXMatrixScaling(&mScaleTorus, 0.03f, 0.03f, 0.03f);

	D3DXMATRIXA16 mTranslate;
	D3DXMATRIXA16 mTranslateTorus;
	D3DXMatrixTranslation(&mTranslate, 0, -3, 0);
	D3DXMatrixTranslation(&mTranslateTorus, _torusPos.x, _torusPos.y, _torusPos.z);

	D3DXMATRIXA16 mDisc = mScaleDisc * mTranslate;
	D3DXMATRIXA16 mTorus = mScaleTorus * mRotate * mTranslateTorus;
	_effect->SetMatrix("_mView", GET_CAMERA()->getMatrixViewPoint());
	_effect->SetMatrix("_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	// 광원
	D3DXMATRIXA16 mRotation = GET_CAMERA()->getMatrixRotate();
	D3DXVECTOR3 offset = GET_CAMERA()->getOffset();
	D3DXVec3TransformCoord(&offset, &offset, &mRotation);

	// 위치
	D3DXVECTOR4 viewPosition(GET_CAMERA()->getPosition() - offset, 1.0f);
	D3DXVECTOR4 lightDirection(GET_LIGHT()->getDirectForward(), 0.0f);

	_effect->SetVector("_viewPosition", &viewPosition);
	_effect->SetVector("_lightDirection", &lightDirection);
	_effect->SetVector("_lightPosition", &D3DXVECTOR4(GET_LIGHT()->getPosition(), 1.0f));

	// 텍스쳐
	_effect->SetTexture("_renderTarget", _renderTarget);

	// 출력 : renderTarget
	_effect->SetMatrix("_mWorld", &mTorus);
	gFunc::runRenderTarget(_renderTarget, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, _depthStensil, [&]()->void {
		gFunc::runEffectLoop(_effect, "techLightMap", [&](int passNum)->void {
			_meshTorus->DrawSubset(0);
		});
	}, COLOR_WHITE(255));

	_effect->SetMatrix("_mWorld", &mDisc);
	gFunc::runRenderTarget(_renderTarget, NULL, _depthStensil, [&]()->void {
		gFunc::runEffectLoop(_effect, "techLightMap", [&](int passNum)->void {
			_meshDisc->DrawSubset(0);
		});
	}, COLOR_WHITE(255));

	// 출력 : world
	_effect->SetMatrix("_mWorld", &mTorus);
	gFunc::runEffectLoop(_effect, "techRenderLight", [&](int passNum)->void {
		_meshTorus->DrawSubset(0);
	});
	
	_effect->SetMatrix("_mWorld", &mDisc);
	gFunc::runEffectLoop(_effect, "techRenderLight", [&](int passNum)->void {
		_meshDisc->DrawSubset(0);
	});

	direct3dApplication::draw();
}

LPD3DXMESH example_24::createTorus(void)
{
	LPD3DXMESH result = nullptr;

	D3DXLoadMeshFromX(TEXT("resource/mesh/torus.x"),
		D3DXMESH_MANAGED,
		MN_DEV,
		NULL,
		NULL,
		NULL,
		NULL,
		&result);

	return result;
}

LPD3DXMESH example_24::createDisc(void)
{
	LPD3DXMESH result = nullptr;

	D3DXLoadMeshFromX(TEXT("resource/mesh/disc.x"),
		D3DXMESH_MANAGED,
		MN_DEV,
		NULL,
		NULL,
		NULL,
		NULL,
		&result);

	return result;
}

LPDIRECT3DTEXTURE9 example_24::createRenderTarget(void)
{
	LPDIRECT3DTEXTURE9 result = nullptr;

	MN_DEV->CreateTexture(WINSIZEX, WINSIZEY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &result, NULL);

	return result;
}