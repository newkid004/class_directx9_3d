#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class example_22 : public direct3dApplication
{
public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public:
	example_22(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_22();
};

