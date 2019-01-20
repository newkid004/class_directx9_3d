#include "practice_AA.h"

#include "managerList.h"

#include "lightBase.h"
#include "camera.h"
#include "gFunc.h"

practice_AA::practice_AA(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
	
}

practice_AA::~practice_AA()
{
	SAFE_RELEASE(_mesh);
	SAFE_RELEASE(_depthMap);
}

void practice_AA::init(void)
{
	direct3dApplication::init();

	GET_LIGHT()->setPosition(D3DXVECTOR3(0.0f, -3.0f, 3.0f));
	GET_LIGHT()->rotate2Pos(D3DXVECTOR3(0, 0, 0));

	_effect = MN_SRC->getEffect("resource/effect/practice_AA.fx");
	_mesh = createMesh();
	_depthMap = createDepthMap();
	_depthMap->GetSurfaceLevel(0, &_surface);
}

void practice_AA::update(void)
{
	direct3dApplication::update();
}

void practice_AA::draw(void)
{
	direct3dApplication::draw();

	D3DXMATRIXA16 mWorld;
	D3DXMatrixIdentity(&mWorld);

	// world
	_effect->SetMatrix("_mWorld", &mWorld);
	_effect->SetMatrix("_mView", GET_CAMERA()->getMatrixViewPoint());
	_effect->SetMatrix("_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	// light
	_effect->SetMatrix("_mLightWorld", &mWorld);
	_effect->SetMatrix("_mLightView", &GET_LIGHT()->getMatrixView());
	_effect->SetMatrix("_mLightProjection", &GET_LIGHT()->getMatrixProjection());

	_effect->SetMatrix("_mTexture", &getTexAdj());

	_effect->SetVector("_lightDirection", &D3DXVECTOR4(GET_LIGHT()->getDirectForward(), 0.0f));
	_effect->SetVector("_viewPosition", &D3DXVECTOR4(GET_CAMERA()->getOffsetPosition(), 1.0f));
	_effect->SetVector("_lightPosition", &D3DXVECTOR4(GET_LIGHT()->getPosition(), 1.0f));

	// 그림자 맵 생성
	LPDIRECT3DSURFACE9 defSurface = nullptr;
	MN_DEV->GetRenderTarget(0, &defSurface);
	MN_DEV->SetRenderTarget(0, _surface);

	gFunc::runEffectLoop(_effect, "shadow", [&](int passNum)->void {
		_mesh->DrawSubset(0);
	});

	MN_DEV->SetRenderTarget(0, defSurface);
	_effect->SetTexture("_depthMap", _depthMap);

	// 그림자 매핑
	gFunc::runEffectLoop(_effect, "myTechnique", [&](int passNum)->void {
		_mesh->DrawSubset(0);
	});
}

LPD3DXMESH practice_AA::createMesh(void)
{
	LPD3DXMESH result = nullptr;

	LPD3DXBUFFER bufAdjacency = nullptr;

	// 메쉬 생성 : X파일, 모델링으로 부터
	LPD3DXMESH origin = nullptr;
	D3DXCreateTeapot(
		MN_DEV,
		&origin,
		&bufAdjacency
	);

	D3DVERTEXELEMENT9 element[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		D3DDECL_END()
	};

	// 사본 생성
	origin->CloneMesh(origin->GetOptions(),
		element,
		MN_DEV,
		&result);

	D3DXComputeNormals(result, (DWORD*)bufAdjacency->GetBufferPointer());	// 인접 정보 계산

	SAFE_RELEASE(origin);
	SAFE_RELEASE(bufAdjacency);

	return result;
}

LPDIRECT3DTEXTURE9 practice_AA::createDepthMap(void)
{
	LPDIRECT3DTEXTURE9 result = nullptr;

	MN_DEV->CreateTexture(640, 640, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &result, NULL);

	return result;
}

D3DXMATRIXA16 practice_AA::getTexAdj(void)
{
	float texOffset = 0.5f + (0.5f / 640.0f);

	D3DXMATRIXA16 result(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f,-0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		texOffset, texOffset, 0.0f, 1.0f
	);

	return GET_LIGHT()->getMatrixView() * GET_LIGHT()->getMatrixProjection() * result;
}
