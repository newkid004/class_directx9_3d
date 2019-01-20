#include "practice_09.h"

#include "managerList.h"
#include "skinnedMesh.h"
#include "animationController.h"

practice_09::practice_09(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

practice_09::~practice_09()
{
	SAFE_DELETE(_skinnedMesh);
}

void practice_09::init(void)
{
	direct3dApplication::init();

	_skinnedMesh = createSkinnedMesh();
}

void practice_09::update(void)
{
	direct3dApplication::update();

	_skinnedMesh->update();
	updateControl();
}

void practice_09::draw(void)
{
	direct3dApplication::draw();

	MN_DEV->SetRenderState(D3DRS_LIGHTING, false);

	_skinnedMesh->draw();

	MN_DEV->SetRenderState(D3DRS_LIGHTING, true);
}

void practice_09::updateControl(void)
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

skinnedMesh * practice_09::createSkinnedMesh(void)
{
	skinnedMesh::mParam param = {
		"resource/mesh/WitchApprentice/WitchApprentice.x",
		"resource/effect/example_16.fx"
	};

	return new skinnedMesh(param);
}