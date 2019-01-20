#include "practice_07.h"

#include "managerList.h"

#include "debugGrid.h"
#include "defaulteCube.h"

practice_07::practice_07(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

practice_07::~practice_07()
{
	SAFE_DELETE(_cube);
}

void practice_07::init(void)
{
	direct3dApplication::init();

	_cube = new defaulteCube;  _cube->setPosition(D3DXVECTOR3(3, 2, 4));
}

void practice_07::update(void)
{
	direct3dApplication::update();

	_cube->update();

	updateControl();
}

void practice_07::draw(void)
{
	direct3dApplication::draw();

	MN_DEV->SetTransform(D3DTS_WORLD, &_cube->getMatrixFinal());
	_cube->draw();
}

void practice_07::updateControl(void)
{
	if (MN_KEY->keyDown(DIK_I)) _grid->rotateX(90.0f * MN_TIME->getDeltaTime(), true);
	if (MN_KEY->keyDown(DIK_K)) _grid->rotateX(-90.0f * MN_TIME->getDeltaTime(), true);

	if (MN_KEY->keyDown(DIK_J)) _grid->rotateY(-90.0f * MN_TIME->getDeltaTime(), true);
	if (MN_KEY->keyDown(DIK_L)) _grid->rotateY(90.0f * MN_TIME->getDeltaTime(), true);

	if (MN_KEY->keyDown(DIK_U)) _grid->rotateZ(90.0f * MN_TIME->getDeltaTime(), true);
	if (MN_KEY->keyDown(DIK_O)) _grid->rotateZ(-90.0f * MN_TIME->getDeltaTime(), true);
}