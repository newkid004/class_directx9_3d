#include "example_18.h"

#include "managerList.h"

#include "sound.h"

example_18::example_18(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

example_18::~example_18()
{
}

void example_18::init(void)
{
	direct3dApplication::init();

	_soundBGM = MN_SRC->getSoundBGM("resource/sound/testBGM.wav");
	_soundSE = MN_SRC->getSoundSE("resource/sound/testSE.wav");
}

void example_18::update(void)
{
	direct3dApplication::update();

	if (MN_KEY->keyPress(DIK_1)) { _soundSE->play(); }
	if (MN_KEY->keyPress(DIK_2)) { _soundBGM->play(); }
}

void example_18::draw(void)
{
	direct3dApplication::draw();
}
