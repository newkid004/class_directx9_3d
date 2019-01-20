#include "example_15.h"
#include "managerList.h"
#include "gFunc.h"

#include "camera.h"
#include "staticMesh.h"

example_15::example_15(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_15::~example_15()
{
	SAFE_DELETE(_staticMesh);
}

void example_15::init(void)
{
	direct3dApplication::init();

	_staticMesh = createStaticMesh();
	_staticMesh->setScale(D3DXVECTOR3(0.2f, 0.2f, 0.2f));
}

void example_15::update(void)
{
	direct3dApplication::update();

	_staticMesh->update();
}

void example_15::draw(void)
{
	direct3dApplication::draw();

	static LPD3DXEFFECT effect = MN_SRC->getEffect("resource/effect/example_15.fx");

	static float yaw;
	D3DXMATRIXA16 rotate;
	yaw += MN_TIME->getDeltaTime() * 90.0f;
	D3DXMatrixIdentity(&rotate);
	D3DXMatrixRotationY(&rotate, D3DXToRadian(yaw));

	D3DXVECTOR4 lightDirection(0.0f, -0.3f, 1.0f, 0.0f);
	D3DXVec4Normalize(&lightDirection, &lightDirection);
	D3DXVec4Transform(&lightDirection, &lightDirection, &rotate);
	effect->SetVector("_lightDirection", &lightDirection);

	D3DXVECTOR4 viewPosition(GET_CAMERA()->getPosition(), 1.0f);
	effect->SetVector("_viewPosition", &viewPosition);

	_staticMesh->draw();
}

staticMesh* example_15::createStaticMesh(void)
{
	staticMesh::mParam param;
	param.effectFilePath = "resource/effect/example_15.fx";
	param.meshFilePath = "resource/mesh/Elementalist/Elementalist.x";

	return new staticMesh(param);
}