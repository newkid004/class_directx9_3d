#include "example_20.h"

#include "managerList.h"
#include "gFunc.h"

#include "cameraControlable.h"
#include "lightBase.h"

#include "labelBase.h"

example_20::example_20(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

example_20::~example_20()
{
	SAFE_RELEASE(_mesh);
}

void example_20::init(void)
{
	direct3dApplication::init();

	c = (cameraControlable*)GET_CAMERA();

	GET_LIGHT()->setRotation(D3DXVECTOR3(0, 0, 0));

	_effect = MN_SRC->getEffect("resource/effect/example_20.fx");

	_mesh = createMesh();
	_textureDiffuse = MN_SRC->getTexture("resource/texture/mapping/rocks.jpg");
	_textureNormal = MN_SRC->getTexture("resource/texture/mapping/rocksNH.tga");
}

void example_20::update(void)
{
	direct3dApplication::update();

	if (MN_KEY->keyDown(DIK_LEFT)) { GET_LIGHT()->rotateY(-90.0f * MN_TIME->getDeltaTime(), false); }
	if (MN_KEY->keyDown(DIK_RIGHT)) { GET_LIGHT()->rotateY(90.0f * MN_TIME->getDeltaTime(), false); }
}

void example_20::draw(void)
{
	direct3dApplication::draw();

	// 행렬
	D3DXMATRIXA16 mWorld;
	D3DXMatrixIdentity(&mWorld);

	_effect->SetMatrix("_mWorld", &mWorld);
	_effect->SetMatrix("_mView", GET_CAMERA()->getMatrixViewPoint());
	_effect->SetMatrix("_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	// 카메라, 광원
	D3DXMATRIXA16 mRotation = GET_CAMERA()->getMatrixRotate();
	D3DXVECTOR3 offset = GET_CAMERA()->getOffset();
	D3DXVec3TransformCoord(&offset, &offset, &mRotation);

	D3DXVECTOR4 viewPosition(GET_CAMERA()->getPosition() - offset, 1.0f);
	D3DXVECTOR4 lightDirection(GET_LIGHT()->getDirectForward(), 0.0f);

	_effect->SetVector("_viewPosition", &viewPosition);
	_effect->SetVector("_lightDirection", &lightDirection);

	// 텍스쳐
	_effect->SetTexture("_textureDiffuse", _textureDiffuse);
	_effect->SetTexture("_textureNormal", _textureNormal);

	// 왜곡
	_effect->SetValue("_distort", &_distort, sizeof(float));
	if (MN_KEY->keyDown(DIK_I)) { _distort = max(0.0f, _distort - MN_TIME->getDeltaTime() / 10.0f); }
	if (MN_KEY->keyDown(DIK_O)) { _distort = min(1.0f, _distort + MN_TIME->getDeltaTime() / 10.0f); }

	// 출력
	if (MN_KEY->keyPress(DIK_P)) _drawType = (_drawType + 1) % 3;

	switch (_drawType)
	{
	case 0: gFunc::runEffectLoop(_effect, "myTechnique", [&](int passNum)->void { _mesh->DrawSubset(0); }); break;
	case 1: gFunc::runEffectLoop(_effect, "tNormal", [&](int passNum)->void { _mesh->DrawSubset(0); });	break;
	case 2: gFunc::runEffectLoop(_effect, "tHeight", [&](int passNum)->void { _mesh->DrawSubset(0); }); break;
	}
}

void example_20::drawUI(void)
{
	direct3dApplication::drawUI();

	char text[MAX_PATH] = "";

	sprintf(text, "\n\n\n\n\n%s%f\n%s%s",
		"distort : ",
		_distort,
		"state : ",
		_drawType == 0 ? "none" : (_drawType == 1 ? "normal" : "parallax"));

	_label->setText(text);
	_label->drawUI();
}

LPD3DXMESH example_20::createMesh(void)
{
	D3DVERTEXELEMENT9 elements[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	LPD3DXMESH result = nullptr;

	D3DXCreateMesh(
		2, 4,
		D3DXMESH_MANAGED,
		elements,
		MN_DEV,
		&result);

	// vertex
	vertex* v = nullptr;
	if (SUCCEEDED(result->LockVertexBuffer(0, (void**)&v)))
	{
		v[0].pos = D3DXVECTOR3(-1.0f, -1.0f,  0.0f);
		v[1].pos = D3DXVECTOR3(-1.0f,  1.0f,  0.0f);
		v[2].pos = D3DXVECTOR3( 1.0f,  1.0f,  0.0f);
		v[3].pos = D3DXVECTOR3( 1.0f, -1.0f,  0.0f);
		
		v[0].uv = D3DXVECTOR2(0.0f, 1.0f);
		v[1].uv = D3DXVECTOR2(0.0f, 0.0f);
		v[2].uv = D3DXVECTOR2(1.0f, 0.0f);
		v[3].uv = D3DXVECTOR2(1.0f, 1.0f);

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

	// normal
	D3DXComputeNormals(result, NULL);

	// binormal, tangent
	D3DXComputeTangent(result, 0, 0, 0, TRUE, NULL);

	return result;
}

