#include "example_16.h"

#include "managerList.h"
#include "skinnedMesh.h"
#include "animationController.h"

example_16::example_16(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_16::~example_16()
{
	SAFE_DELETE(_skinnedMesh);
}

void example_16::init(void)
{
	direct3dApplication::init();

	_skinnedMesh = createSkinnedMesh();
}

void example_16::update(void)
{
	direct3dApplication::update();

	_skinnedMesh->update();
	updateControl();
}

void example_16::draw(void)
{
	direct3dApplication::draw();

	MN_DEV->SetRenderState(D3DRS_LIGHTING, false);

	_skinnedMesh->draw();

	MN_DEV->SetRenderState(D3DRS_LIGHTING, true);
}

void example_16::updateControl(void)
{
	static int beforeAnimationIndex = 0;
	static int currentAnimationIndex = 0;
	static bool isLoop = false;

	if (MN_KEY->keyDown(DIK_1)) { currentAnimationIndex = 0; isLoop = true; }
	if (MN_KEY->keyDown(DIK_2)) { currentAnimationIndex = 1; isLoop = true; }
	if (MN_KEY->keyDown(DIK_3)) { currentAnimationIndex = 2; isLoop = true; }
	if (MN_KEY->keyDown(DIK_4)) { currentAnimationIndex = 3; isLoop = false; }
	if (MN_KEY->keyDown(DIK_5)) { currentAnimationIndex = 4; isLoop = false; }

	if (beforeAnimationIndex != currentAnimationIndex)
	{
		auto ani = _skinnedMesh->getAniController();
		auto aniName = ani->getAnimationNameList()[currentAnimationIndex];

		ani->play(aniName, isLoop);

		beforeAnimationIndex = currentAnimationIndex;
	}
	if (MN_KEY->keyPress(DIK_SPACE))
		_skinnedMesh->getAniController()->stop();
}

skinnedMesh* example_16::createSkinnedMesh(void)
{
	skinnedMesh::mParam param = {
		"resource/mesh/WitchApprentice/WitchApprentice.x",
		"resource/effect/skinnedMesh.fx"
	};

	return new skinnedMesh(param);
}