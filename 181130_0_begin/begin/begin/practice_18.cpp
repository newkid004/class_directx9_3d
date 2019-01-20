#include "practice_18.h"

#include "managerList.h"
#include "gFunc.h"

#include "camera.h"
#include "lightBase.h"

#include "terrain.h"
#include "skinnedMesh.h"
#include "animationController.h"

practice_18::practice_18(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

practice_18::~practice_18()
{
	SAFE_RELEASE(_depthStensil);
	SAFE_RELEASE(_renderTarget);
}

void practice_18::init(void)
{
	direct3dApplication::init();

	_effect = MN_SRC->getEffect("resource/effect/practice_18.fx");
	_renderTarget = createRenderTarget();
	_depthStensil = createDepthStensil();

	_terrain = createTerrain();
	_terrain->moveY(-15);
	_terrain->setScale(0.3f);

	_character = createCharacter();
	_character->setRunTechnique("techCharacter");
	_character->setScale(0.3f);

	GET_LIGHT()->setPosition(D3DXVECTOR3(3, 20, -10));
	GET_LIGHT()->rotate2Pos(D3DXVECTOR3(0, 0, 0));
}

void practice_18::update(void)
{
	direct3dApplication::update();

	_terrain->update();
	_character->update();

	if (MN_KEY->keyDown(DIK_UP))	_character->moveZ(3.0f * MN_TIME->getDeltaTime());
	if (MN_KEY->keyDown(DIK_DOWN))	_character->moveZ(-3.0f * MN_TIME->getDeltaTime());
	if (MN_KEY->keyDown(DIK_LEFT))	_character->moveX(-3.0f * MN_TIME->getDeltaTime());
	if (MN_KEY->keyDown(DIK_RIGHT)) _character->moveX(3.0f * MN_TIME->getDeltaTime());

	if (MN_KEY->keyDown(DIK_J))		GET_LIGHT()->moveX(-5.0f * MN_TIME->getDeltaTime(), false);
	if (MN_KEY->keyDown(DIK_L))		GET_LIGHT()->moveX(5.0f * MN_TIME->getDeltaTime(), false);

	updateControl();
}

void practice_18::draw(void)
{
	static float rotate;
	rotate += MN_TIME->getDeltaTime() * 90.0f;

	// 메쉬
	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);

	D3DXMATRIXA16 mRotate;
	D3DXMatrixIdentity(&mRotate);
	D3DXMatrixRotationY(&mRotate, D3DXToRadian(rotate));

	D3DXMATRIXA16 mScaleDisc;
	D3DXMATRIXA16 mScaleTorus;
	D3DXMatrixScaling(&mScaleDisc, 0.1f, 0.1f, 0.1f);
	D3DXMatrixScaling(&mScaleTorus, 0.03f, 0.03f, 0.03f);

	_effect->SetMatrix("_mView", GET_CAMERA()->getMatrixViewPoint());
	_effect->SetMatrix("_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	// 카메라
	D3DXMATRIXA16 mRotation = GET_CAMERA()->getMatrixRotate();
	D3DXVECTOR3 offset = GET_CAMERA()->getOffset();
	D3DXVec3TransformCoord(&offset, &offset, &mRotation);

	// 광원
	D3DXMATRIXA16 & mLightView = GET_LIGHT()->getMatrixView();
	_effect->SetMatrix("_mLightView", &mLightView);

	// 위치
	D3DXVECTOR4 viewPosition(GET_CAMERA()->getPosition() - offset, 1.0f);
	D3DXVECTOR4 lightDirection(GET_LIGHT()->getDirectForward(), 0.0f);

	_effect->SetVector("_viewPosition", &viewPosition);
	_effect->SetVector("_lightDirection", &lightDirection);
	_effect->SetVector("_lightPosition", &D3DXVECTOR4(GET_LIGHT()->getPosition(), 1.0f));

	// 텍스쳐
	_effect->SetTexture("_renderTarget", _renderTarget);

	// 출력 : shadow map
	_effect->SetMatrix("_mWorld", &_terrain->getMatrixFinal());
	gFunc::runRenderTarget(_renderTarget, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, _depthStensil, [&]()->void {
		_terrain->setRunTechnique("techLightMap");
		_terrain->draw();
	});

	_effect->SetMatrix("_mWorld", &_character->getMatrixFinal());
	gFunc::runRenderTarget(_renderTarget, NULL, _depthStensil, [&]()->void {
		_character->setRunTechnique("techCharacterMap");
		_character->draw();
	});

	// 출력 : world
	_terrain->setRunTechnique("myTechnique");
	_terrain->draw();
	_character->setRunTechnique("techCharacter");
	_character->draw();

	direct3dApplication::draw();
}

void practice_18::updateControl(void)
{
	static bool isPress = false;
	static int currentAnimationIndex = 0;
	static bool isLoop = false;

	if (MN_KEY->keyPress(DIK_1)) { isPress = true; currentAnimationIndex = 0; isLoop = true; }
	if (MN_KEY->keyPress(DIK_2)) { isPress = true; currentAnimationIndex = 1; isLoop = true; }
	if (MN_KEY->keyPress(DIK_3)) { isPress = true; currentAnimationIndex = 2; isLoop = true; }
	if (MN_KEY->keyPress(DIK_4)) { isPress = true; currentAnimationIndex = 3; isLoop = false; }
	if (MN_KEY->keyPress(DIK_5)) { isPress = true; currentAnimationIndex = 4; isLoop = false; }

	if (isPress)
	{
		auto ani = _character->getAniController();
		auto aniName = ani->getAnimationNameList()[currentAnimationIndex];

		ani->play(aniName, isLoop);
		ani->setPlayPosition(0.0);

		isPress = false;
	}
	if (MN_KEY->keyPress(DIK_SPACE))
		_character->getAniController()->stop();
}

LPDIRECT3DTEXTURE9 practice_18::createRenderTarget(void)
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

LPDIRECT3DSURFACE9 practice_18::createDepthStensil(void)
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

terrain * practice_18::createTerrain(void)
{
	terrain::params param = {
		   30,
		   0.2f,
		   "resource/texture/terrain/SplatMap.png",
		   "resource/texture/terrain/HeightMap.bmp",
		   "resource/effect/practice_18.fx"
	};
	param.tileSize.cx = 1;
	param.tileSize.cy = 1;
	param.mapSize.cx = 256;
	param.mapSize.cy = 256;

	for (int i = 0; i < terrain::MAX_NUM_TEXTURES; ++i)
	{
		char filePath[MAX_PATH] = "";
		sprintf(filePath, "resource/texture/terrain/Terrain_%02d.jpg", i + 1);

		param.vPathTextureFile.push_back(filePath);
	}

	return new terrain(param);
}

skinnedMesh * practice_18::createCharacter(void)
{
	skinnedMesh::mParam param = {
		"resource/mesh/WitchApprentice/WitchApprentice.x",
		"resource/effect/practice_18.fx"
	};

	return new skinnedMesh(param);
}
