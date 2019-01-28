#include "example_27.h"

#include "managerList.h"

#include "gFunc.h"
#include "pickRay.h"

#include "staticMesh.h"
#include "debugDraw.h"

constexpr int DEBUG_TYPE_BOX = 1;
constexpr int DEBUG_TYPE_SPHERE = 2;
constexpr int DEBUG_TYPE = DEBUG_TYPE_BOX;

example_27::example_27(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

example_27::~example_27()
{
	SAFE_DELETE(_staticMesh);
}


void example_27::init(void)
{
	direct3dApplication::init();

	_staticMesh = createStaticMesh();
	_staticMesh->setScale(0.2f);
	_staticMesh->setPosition(D3DXVECTOR3(-10.0f, 0.0f, 0.0f));

	switch (DEBUG_TYPE)
	{
	case DEBUG_TYPE_BOX: _staticMesh->setDebugEnalbe(true, EDebugDrawType::BOX); break;
	case DEBUG_TYPE_SPHERE: _staticMesh->setDebugEnalbe(true, EDebugDrawType::SPHERE); break;
	}
	
}

void example_27::update(void)
{
	direct3dApplication::update();

	_staticMesh->update();

	if (MN_KEY->keyDown(DIK_LEFT))	_staticMesh->rotateY(-90.0f * MN_TIME->getDeltaTime());
	if (MN_KEY->keyDown(DIK_RIGHT))	_staticMesh->rotateY(90.0f * MN_TIME->getDeltaTime());
	if (MN_KEY->keyDown(DIK_UP))	_staticMesh->moveZ(10.0f * -MN_TIME->getDeltaTime());
	if (MN_KEY->keyDown(DIK_DOWN))	_staticMesh->moveZ(10.0f * MN_TIME->getDeltaTime());

	static int count = 0;
	if (MN_KEY->mouseDown())
	{
		pick::ray ray;
		pick::createPickRay(&ray);

		EDebugDrawType type;
		switch (DEBUG_TYPE)
		{
		case DEBUG_TYPE_BOX: type = EDebugDrawType::BOX; break;
		case DEBUG_TYPE_SPHERE: type = EDebugDrawType::SPHERE; break;
		}

		if (pick::chkPick(&ray, _staticMesh, type))
			printf("Ãæµ¹ %d\n", ++count);
	}
}

void example_27::draw(void)
{
	direct3dApplication::draw();

	_staticMesh->draw();
}

staticMesh * example_27::createStaticMesh(void)
{
	staticMesh::mParam param = {
		"resource/mesh/Elementalist/Elementalist.x",
		"resource/effect/example_15.fx"
	};

	return new staticMesh(param);
}
