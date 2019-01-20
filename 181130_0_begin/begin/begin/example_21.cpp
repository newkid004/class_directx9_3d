#include "example_21.h"

#include "managerList.h"
#include "gFunc.h"

#include "camera.h"
#include "lightBase.h"
#include "skyBox.h"

example_21::example_21(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

example_21::~example_21()
{
	SAFE_RELEASE(_mesh);
	SAFE_RELEASE(_textureCube);
}

void example_21::init(void)
{
	direct3dApplication::init();

	_texture = MN_SRC->getTexture("resource/texture/monkey/Earth.jpg");
	_effect = MN_SRC->getEffect("resource/effect/example_21.fx");

	_mesh = createMesh();
	_textureCube = createTextureCube();
}

void example_21::update(void)
{
	direct3dApplication::update();
}

void example_21::draw(void)
{
	direct3dApplication::draw();

	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_NONE);

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

	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_CCW);
}

LPD3DXMESH example_21::createMesh(void)
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

LPDIRECT3DCUBETEXTURE9 example_21::createTextureCube(void)
{
	LPDIRECT3DCUBETEXTURE9 result = nullptr;

	D3DXCreateCubeTextureFromFile(MN_DEV,
		TEXT("resource/texture/monkey/Mars.dds"),
		&result);

	return result;
}

skyBox * example_21::createSkyBox(void)
{
	skyBox::mParam param = {
		"resource/effect/example_21.fx",
		"resource/texture/monkey/Mars.dds"
	};

	return new skyBox(param);
}
