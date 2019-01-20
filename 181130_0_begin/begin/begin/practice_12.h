#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class cameraControlable;
class terrain;
class skinnedMesh;

class practice_12 : public direct3dApplication
{
private:
	cameraControlable* _c = nullptr;

	skinnedMesh* _character = nullptr;
	int _charMovement = 0;
	bool _move2click = false;
	D3DXVECTOR2 _moveDir;
	D3DXVECTOR2 _movePos;

	terrain* _terrain = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

private:
	void updateControl(void);
	void updateCharacter(void);

private:
	terrain* createTerrian(void);
	skinnedMesh* createCharacter(void);

public:
	practice_12(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~practice_12();
};

