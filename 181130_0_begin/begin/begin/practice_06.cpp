#include "practice_06.h"
#include "managerList.h"
#include "camera.h"
#include "gFunc.h"

practice_06::practice_06(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

practice_06::~practice_06()
{
	SAFE_RELEASE(_effect);

	for (auto m : _meshes) 
		SAFE_RELEASE(m);
}

void practice_06::init(void)
{
	direct3dApplication::init();

	_effect = createEffect();
	createMeshes();
	createDiffuses();

	for (int i = 0; i < e_MESH::COUNT; ++i)
	{
		_obj[i] = new baseObject();
		_obj[i]->setPosition(D3DXVECTOR3(i * 4 - 8, 0, 0));
	}
}

void practice_06::update(void)
{
	direct3dApplication::update();

	for (int i = 0; i < e_MESH::COUNT; ++i)
		_obj[i]->update();
}

void practice_06::draw(void)
{
	direct3dApplication::draw();

	// 행렬
	D3DXMATRIXA16 mWorld;
	D3DXMatrixIdentity(&mWorld);

	_effect->SetMatrix("_mView", GET_CAMERA()->getMatrixViewPoint());
	_effect->SetMatrix("_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	// 광원
	D3DXVECTOR4 lightDirection(0.0f, 0.0f, 1.0f, 0.0f);
	_effect->SetVector("_lightDirection", &lightDirection);

	// 카메라
	D3DXVECTOR4 viewPosition(GET_CAMERA()->getPosition(), 1.0f);
	_effect->SetVector("_viewPosition", &viewPosition);

	// 이펙트
	for (int oCount = 0; oCount < e_MESH::COUNT; ++oCount)
	{
		// 월드, 색상
		_effect->SetMatrix("_mWorld", _obj[oCount]->getMatrixWorldPoint());
		_effect->SetVector("_diffuse", &_diffuses[oCount]);

		// 테크닉
		gFunc::runEffectLoop(_effect, "myTechnique", [&](int index)->void { _meshes[oCount]->DrawSubset(0); });
	}
}

LPD3DXEFFECT practice_06::createEffect(void)
{
	LPD3DXEFFECT result = NULL;

	LPD3DXBUFFER bufError = NULL;

	D3DXCreateEffectFromFile(
		MN_DEV,
		TEXT("resource/effect/practice_06.fx"),
		NULL,
		NULL,
		D3DXSHADER_DEBUG,
		NULL,
		&result,
		&bufError);

	if (bufError != nullptr)
	{
		auto msgError = (char*)bufError->GetBufferPointer();
		printf("practice_06.createEffect : %s\n", msgError);
	}

	return result;
}

void practice_06::createMeshes(void)
{
	D3DXCreateCylinder(MN_DEV, 1.0f, 1.0f, 1.0f, 26, 1, &_meshes[e_MESH::CYLINDER], NULL);
	D3DXCreateTorus(MN_DEV, 0.5f, 1.0f, 36, 36, &_meshes[e_MESH::TORUS], NULL);
	D3DXCreateSphere(MN_DEV, 1.0f, 36, 36, &_meshes[e_MESH::SPHERE], NULL);
	D3DXCreateBox(MN_DEV, 1.0f, 1.0f, 1.0f, &_meshes[e_MESH::BOX], NULL);
	D3DXCreateTeapot(MN_DEV, &_meshes[e_MESH::TEAPOT], NULL);

	//for (int i = 0; i < e_MESH::COUNT; ++i)
	//	D3DXComputeNormals(_meshes[i], NULL);
}

void practice_06::createDiffuses(void)
{
	int i = 0;

	_diffuses[i++] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	_diffuses[i++] = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	_diffuses[i++] = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	_diffuses[i++] = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
	_diffuses[i++] = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);
}