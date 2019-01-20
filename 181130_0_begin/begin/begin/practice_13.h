#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class sound;

class practice_13 : public direct3dApplication
{
private:
	int index = 0;

	sound* _soundSE = nullptr;
	sound* _soundBGM = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

	void printSound(sound* s);

public:
	practice_13(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~practice_13();
};

