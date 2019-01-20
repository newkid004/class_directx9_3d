#include "practice_10.h"

#include "managerList.h"
#include "skinnedMesh.h"
#include "animationController.h"

#include "labelBase.h"
#include "spriteBase.h"

practice_10::practice_10(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

practice_10::~practice_10()
{
	SAFE_DELETE(_skinnedMesh);
}

void practice_10::init(void)
{
	direct3dApplication::init();

	_skinnedMesh = createSkinnedMesh();

	_sprite = new spriteBase("resource/texture/textureGradiant.png");
	_sprite->setPosition(D3DXVECTOR3(MN_WIN->getWindowSize().cx / 2.0f, MN_WIN->getWindowSize().cy / 2.0f, 0.0f));
	_sprite->setScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	_sprite->setColor(COLOR_WHITE(150));
}

void practice_10::update(void)
{
	direct3dApplication::update();

	_skinnedMesh->update();

	updateControl();
}

void practice_10::draw(void)
{
	direct3dApplication::draw();

	_skinnedMesh->draw();
}

void practice_10::drawUI(void)
{
	direct3dApplication::drawUI();

	_sprite->drawUI();
}

void practice_10::updateControl(void)
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
		auto ani = _skinnedMesh->getAniController();
		auto aniName = ani->getAnimationNameList()[currentAnimationIndex];

		ani->play(aniName, isLoop);
		ani->setPlayPosition(0.0);

		isPress = false;
	}
	if (MN_KEY->keyPress(DIK_SPACE))
		_skinnedMesh->getAniController()->stop();
}

skinnedMesh * practice_10::createSkinnedMesh(void)
{
	skinnedMesh::mParam param = {
		"resource/mesh/WitchApprentice/WitchApprentice.x",
		"resource/effect/skinnedMesh.fx"
	};

	return new skinnedMesh(param);
}
