#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class example_14 : public direct3dApplication
{
private :
	LPD3DXFONT _font = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public:
	LPD3DXFONT createFont(void);

public:
	example_14(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_14();
};

