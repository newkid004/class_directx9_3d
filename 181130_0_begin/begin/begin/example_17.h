#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

using namespace std;

class terrain;
class skinnedMesh;

class example_17 : public direct3dApplication
{
private:
	skinnedMesh* _character = nullptr;
	terrain* _terrain = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;
	void drawUI(void) override;

private :
	void updateControl(void);

private :
	terrain* createTerrian(void);
	skinnedMesh* createSkinnedMesh(void);

public:
	example_17(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_17();
};

