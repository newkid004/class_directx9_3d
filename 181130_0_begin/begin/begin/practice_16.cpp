#include "practice_16.h"

#include "managerList.h"
#include "gFunc.h"

#include "camera.h"
#include "lightBase.h"

practice_16::practice_16(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

practice_16::~practice_16()
{
	SAFE_RELEASE(_sky);
	SAFE_RELEASE(_mesh);
	SAFE_RELEASE(_textureCube);
}

void practice_16::init(void)
{
	direct3dApplication::init();

	_texture = MN_SRC->getTexture("resource/texture/monkey/Earth.jpg");
	_effect = MN_SRC->getEffect("resource/effect/example_21.fx");

	_sky = createMesh();
	_mesh = createMesh();
	_textureCube = createTextureCube();
}

void practice_16::update(void)
{
	direct3dApplication::update();
}

void practice_16::draw(void)
{
	direct3dApplication::draw();

	static float rotate;
	rotate += MN_TIME->getDeltaTime() * 90.0f;

	// 행렬
	D3DXMATRIXA16 mRotate;
	D3DXMatrixRotationY(&mRotate, D3DXToRadian(rotate));

	D3DXMATRIXA16 mScale;
	D3DXMatrixScaling(&mScale, 0.1f, 0.1f, 0.1f);

	D3DXMATRIXA16 mWorld = mRotate * mScale;
	_effect->SetMatrix("_mWorld", &mWorld);
	_effect->SetMatrix("_mView", GET_CAMERA()->getMatrixViewPoint());
	_effect->SetMatrix("_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	// 광원
	D3DXMATRIXA16 mRotation = GET_CAMERA()->getMatrixRotate();
	D3DXVECTOR3 offset = GET_CAMERA()->getOffset();
	D3DXVec3TransformCoord(&offset, &offset, &mRotation);

	D3DXVECTOR4 viewPosition(GET_CAMERA()->getPosition() - offset, 1.0f);
	D3DXVECTOR4 lightDirection(GET_LIGHT()->getDirectForward(), 0.0f);

	_effect->SetVector("_viewPosition", &viewPosition);
	_effect->SetVector("_lightDirection", &lightDirection);

	// 텍스쳐
	_effect->SetTexture("_texture", _texture);
	_effect->SetTexture("_textureCube", _textureCube);

	gFunc::runEffectLoop(_effect, "myTechnique", [&](int passNum)->void {
		_mesh->DrawSubset(0);
	});

	// 스카이 박스
	D3DXMatrixScaling(&mScale, 10.0f, 10.0f, 10.0f);
	D3DXMATRIXA16 mTranslate;
	D3DXMatrixTranslation(&mTranslate, viewPosition.x, viewPosition.y, viewPosition.z);

	mWorld = mScale * mTranslate;
	_effect->SetMatrix("_mWorld", &mWorld);
	gFunc::runEffectLoop(_effect, "justTexture", [&](int passNum)->void {
		_sky->DrawSubset(0);
	});
}

LPD3DXMESH practice_16::createMesh(void)
{
	LPD3DXMESH result = nullptr;

	D3DXLoadMeshFromX(TEXT("resource/mesh/sphere.x"),
		D3DXMESH_MANAGED,
		MN_DEV,
		NULL,
		NULL,
		NULL,
		NULL,
		&result);

	return result;
}

LPDIRECT3DCUBETEXTURE9 practice_16::createTextureCube(void)
{
	LPDIRECT3DCUBETEXTURE9 result = nullptr;

	D3DXCreateCubeTextureFromFile(MN_DEV,
		TEXT("resource/texture/monkey/Mars.dds"),
		&result);

	return result;
}
