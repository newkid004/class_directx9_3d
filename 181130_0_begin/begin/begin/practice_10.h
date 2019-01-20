#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class skinnedMesh;
class spriteBase;

class practice_10 : public direct3dApplication
{
private :
	skinnedMesh* _skinnedMesh = nullptr;
	spriteBase* _sprite = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;
	void drawUI(void) override;

	void updateControl(void);

private:
	skinnedMesh* createSkinnedMesh(void);

public:
	practice_10(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~practice_10();
};