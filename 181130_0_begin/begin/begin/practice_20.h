#pragma once

#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class staticMesh;

class practice_20 : public direct3dApplication
{
private :
	staticMesh* _staticMesh[2] = { nullptr, };
	bool _isPick[2];

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;
	void drawUI(void) override;

private :
	staticMesh* createStaticMesh(void);

private :
	void updateClick(void);

public:
	practice_20(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~practice_20();
};

