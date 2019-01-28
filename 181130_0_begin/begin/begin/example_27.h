#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class staticMesh;

class example_27 : public direct3dApplication
{
private :
	staticMesh* _staticMesh = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

private :
	staticMesh* createStaticMesh(void);

public:
	example_27(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_27();
};

