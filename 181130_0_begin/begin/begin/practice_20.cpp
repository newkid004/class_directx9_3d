#include "practice_20.h"

#include "managerList.h"
#include "gFunc.h"

#include "labelBase.h"
#include "staticMesh.h"
#include "pickRay.h"
#include "debugDraw.h"

practice_20::practice_20(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

practice_20::~practice_20()
{
	for (auto & m : _staticMesh)
		SAFE_DELETE(m);
}

void practice_20::init(void)
{
	direct3dApplication::init();

	_staticMesh[0] = createStaticMesh();
	_staticMesh[1] = createStaticMesh();

	_staticMesh[0]->setScale(0.1f);
	_staticMesh[1]->setScale(0.1f);

	_staticMesh[0]->moveX(-10);
	_staticMesh[1]->moveX(10);

	_staticMesh[0]->setDebugEnalbe(true, EDebugDrawType::BOX);
	_staticMesh[1]->setDebugEnalbe(true, EDebugDrawType::SPHERE);

	_isPick[0] = false;
	_isPick[1] = false;
}

void practice_20::update(void)
{
	direct3dApplication::update();

	for (auto & m : _staticMesh)
		m->update();

	static int target = 0;
	if (MN_KEY->keyPress(DIK_P)) target = (target + 1) % 2;

	if (MN_KEY->keyDown(DIK_LEFT))	_staticMesh[target]->rotateY(-90.0f * MN_TIME->getDeltaTime());
	if (MN_KEY->keyDown(DIK_RIGHT))	_staticMesh[target]->rotateY(90.0f * MN_TIME->getDeltaTime());
	if (MN_KEY->keyDown(DIK_UP))	_staticMesh[target]->moveZ(10.0f * -MN_TIME->getDeltaTime());
	if (MN_KEY->keyDown(DIK_DOWN))	_staticMesh[target]->moveZ(10.0f * MN_TIME->getDeltaTime());

	updateClick();
}

void practice_20::draw(void)
{
	direct3dApplication::draw();

	for (auto & m : _staticMesh)
		m->draw();
}

void practice_20::drawUI(void)
{
	direct3dApplication::drawUI();

	for (int i = 0; i < 2; ++i)
	{
		char text[MAX_PATH] = "";

		sprintf(text, "pick%d : %s",
			i + 1,
			_isPick[i] ? "true" : "false");

		_label->setText(text);
		_label->setPosition(D3DXVECTOR3(0.0f, 80.0f + i * 20.0f, 0.0f));
		_label->drawUI();
	}
}

staticMesh * practice_20::createStaticMesh(void)
{
	staticMesh::mParam param = {
		"resource/mesh/Elementalist/Elementalist.x",
		"resource/effect/example_15.fx"
	};

	return new staticMesh(param);
}

void practice_20::updateClick(void)
{
	for (int i = 0; i < 2; ++i)
	{
		pick::ray pRay;
		pick::info pInfo;
		D3DXMATRIXA16 mBound;

		_staticMesh[i]->getDebugDraw()->getMatrixBound(&mBound);
		pick::createPickRay(&pRay, &mBound);

		LPD3DXMESH meshBound = _staticMesh[i]->getDebugDraw()->getBoundMesh();
		_isPick[i] = pick::chkPick(&pInfo, &pRay, meshBound);
	}
}
