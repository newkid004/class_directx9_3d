#include "example_23.h"

#include "managerList.h"
#include "gFunc.h"

#include "camera.h"
#include "lightBase.h"

example_23::example_23(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

example_23::~example_23()
{
}

void example_23::init(void)
{
	direct3dApplication::init();

	_effect = MN_SRC->getEffect("resource/effect/example_23.fx");
	_texture = MN_SRC->getTexture("resource/texture/monkey/Earth.jpg");
	_textureCube = MN_SRC->getTextureCube("resource/texture/monkey/Mars.dds");
	_mesh = createMesh();
	_meshScreen = createMeshScreen();

	_renderOutline = createRenderTarget();
	_renderTarget = createRenderTarget();
	
	_depthStensil = createDepthStensil();
}

void example_23::update(void)
{
	direct3dApplication::update();
}

void example_23::draw(void)
{
	static float rotate;
	rotate += MN_TIME->getDeltaTime() * 90.0f;

	// 행렬
	D3DXMATRIXA16 mRotate;
	D3DXMatrixIdentity(&mRotate);
	D3DXMatrixRotationY(&mRotate, D3DXToRadian(rotate));

	D3DXMATRIXA16 mScale;
	D3DXMatrixScaling(&mScale, 0.03f, 0.03f, 0.03f);

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
	_effect->SetTexture("_renderOutline", _renderOutline);

	MN_DEV->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	MN_DEV->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	MN_DEV->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	gFunc::runRenderTarget(_renderOutline, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, _depthStensil, [&](void)->void
	{
		gFunc::runEffectLoop(_effect, "techOutline", [&](int passNum)->void {
			_mesh->DrawSubset(0);
		});
	});

	gFunc::runRenderTarget(_renderTarget, D3DCLEAR_TARGET, _depthStensil, [&](void)->void
	{
		gFunc::runEffectLoop(_effect, "techScreen", [&](int passNum)->void {
			_meshScreen->DrawSubset(0);
		});
	});

	gFunc::runRenderTarget(_renderTarget, NULL, NULL, [&](void)->void
	{
		gFunc::runEffectLoop(_effect, "myTechnique", [&](int passNum)->void {
			_mesh->DrawSubset(0);
		});
		direct3dApplication::draw();
	});

	_sprite->Begin(D3DXSPRITE_ALPHABLEND);
	{
		_sprite->Draw(_renderTarget, NULL, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));
	}
	_sprite->End();

	MN_DEV->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	
}

LPD3DXMESH example_23::createMesh(void)
{
	LPD3DXMESH result = nullptr;

	D3DXLoadMeshFromX(TEXT("resource/mesh/teapot.x"),
		D3DXMESH_MANAGED,
		MN_DEV,
		NULL,
		NULL,
		NULL,
		NULL,
		&result);

	return result;
}

LPD3DXMESH example_23::createMeshScreen(void)
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
		v[1].pos = D3DXVECTOR3(-1,  1, 0);
		v[2].pos = D3DXVECTOR3( 1,  1, 0);
		v[3].pos = D3DXVECTOR3( 1, -1, 0);

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

LPDIRECT3DTEXTURE9 example_23::createRenderTarget(void)
{
	LPDIRECT3DTEXTURE9 result = nullptr;

	MN_DEV->CreateTexture(WINSIZEX, WINSIZEY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &result, NULL);

	return result;
}

LPDIRECT3DSURFACE9 example_23::createDepthStensil(void)
{
	LPDIRECT3DSURFACE9 result = nullptr;

	// 깊이 스텐실을 생성한다
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
