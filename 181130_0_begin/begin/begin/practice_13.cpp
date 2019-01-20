#include "practice_13.h"

#include "managerList.h"

#include "sound.h"

practice_13::practice_13(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

practice_13::~practice_13()
{
}

void practice_13::init(void)
{
	direct3dApplication::init();

	_soundBGM = MN_SRC->getSoundBGM("resource/sound/testBGM.wav");
	_soundSE = MN_SRC->getSoundSE("resource/sound/testSE.wav");
}

void practice_13::update(void)
{
	direct3dApplication::update();

	if (MN_KEY->keyPress(DIK_1)) { _soundSE->play(); }
	if (MN_KEY->keyPress(DIK_2)) { _soundBGM->play(); }

	system("cls");
	printf("< BGM >\n");
	printSound(_soundBGM);

	printf("< SE >\n");
	printSound(_soundSE);
}

void practice_13::draw(void)
{
	direct3dApplication::draw();
}

void practice_13::printSound(sound * s)
{
	const auto & pInfo = s->getPlayInfo();
	
	printf("isRunable : %s\n", (pInfo->lastRunTime[pInfo->currentRunIndex] + pInfo->length < MN_TIME->getRunningTime()) ? "true" : "false");
	printf("isBGM : %s\n", pInfo->isBGM ? "true" : "false");
	printf("currentRunIndex : %d\n", pInfo->currentRunIndex);
	printf("maxRunIndex : %d\n", pInfo->maxRunIndex);
	printf("\n");
}