#include "example_17.h"

#include "managerList.h"
#include "cameraControlable.h"
#include "gFunc.h"

#include "debugGrid.h"
#include "lightBase.h"

#include "terrain.h"
#include "skinnedMesh.h"

example_17::example_17(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
	
}

example_17::~example_17()
{
	SAFE_DELETE(_terrain);
	SAFE_DELETE(_character);
}

void example_17::init(void)
{
	direct3dApplication::init();

	cameraControlable* c = (cameraControlable*)_camera;
	c->setVelocity(c->getVelocity() * 3.0f);
	// c->setMoveable(false);

	_terrain = createTerrian();
	_character = createSkinnedMesh();
	_character->setScale(0.2f);
}

void example_17::update(void)
{
	direct3dApplication::update();

	_terrain->update();
	_character->update();

	updateControl();
}

void example_17::draw(void)
{
	direct3dApplication::draw();
	_terrain->draw();
	_character->draw();
}

void example_17::drawUI(void)
{
	direct3dApplication::drawUI();
}

void example_17::updateControl(void)
{
	if (MN_KEY->keyDown(DIK_LEFT)) GET_LIGHT()->rotateY(-90.0f * MN_TIME->getDeltaTime(), false);
	if (MN_KEY->keyDown(DIK_RIGHT)) GET_LIGHT()->rotateY(90.0f * MN_TIME->getDeltaTime(), false);

	if (MN_KEY->mouseDown(EMouseInput::LEFT))
	{
		float velocity = 5.0f * MN_TIME->getDeltaTime();
		if (MN_KEY->keyDown(DIK_W)) _character->moveZ(velocity, true);
		if (MN_KEY->keyDown(DIK_S)) _character->moveZ(-velocity, true);
		if (MN_KEY->keyDown(DIK_A)) _character->rotateY(-velocity * 10.0f, false);
		if (MN_KEY->keyDown(DIK_D)) _character->rotateY(velocity * 10.0f, false);
	}
	if (MN_KEY->mousePress(EMouseInput::LEFT))
	{
		auto ray = gFunc::createPickRay(MN_KEY->getMousePos());

		D3DXVECTOR3 intersectPos;
		if (_terrain->getIntersectPoint(ray, intersectPos))
		{
			_character->setPosition(intersectPos);
		}
	}

	D3DXVECTOR3 pos = _character->getPosition();
	pos.y = _terrain->getHeight(D3DXVECTOR2(pos.x, pos.z));
	_character->setPosition(pos);
}

terrain * example_17::createTerrian(void)
{
	terrain::params param = {
		30,
		0.2f,
		"resource/texture/terrain/SplatMap.png",
		"resource/texture/terrain/HeightMap.bmp",
		"resource/effect/example_17.fx"
	};
	param.tileSize.cx = 1;
	param.tileSize.cy = 1;

	for (int i = 0; i < terrain::MAX_NUM_TEXTURES; ++i)
	{
		char filePath[MAX_PATH] = "";
		sprintf(filePath, "resource/texture/terrain/Terrain_%02d.jpg", i + 1);

		param.vPathTextureFile.push_back(filePath);
	}

	return new terrain(param);
}

skinnedMesh * example_17::createSkinnedMesh(void)
{
	skinnedMesh::mParam param = {
		"resource/mesh/WitchApprentice/WitchApprentice.x",
		"resource/effect/skinnedMesh.fx"
	};

	return new skinnedMesh(param);
}
