#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class defaulteCube;

class practice_07 : public direct3dApplication
{
public :
	defaulteCube* _cube = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public :
	void updateControl(void);

public:
	practice_07(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~practice_07();
};

