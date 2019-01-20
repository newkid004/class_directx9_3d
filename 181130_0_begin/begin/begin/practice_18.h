#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class terrain;
class skinnedMesh;

class practice_18 : public direct3dApplication
{
private:
	LPD3DXEFFECT _effect = nullptr;

	LPDIRECT3DTEXTURE9 _renderTarget = nullptr;
	LPDIRECT3DSURFACE9 _depthStensil = nullptr;

	terrain* _terrain = nullptr;
	skinnedMesh* _character = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public :
	void updateControl(void);

private:
	LPDIRECT3DTEXTURE9 createRenderTarget(void);
	LPDIRECT3DSURFACE9 createDepthStensil(void);

	terrain* createTerrain(void);
	skinnedMesh* createCharacter(void);

public:
	practice_18(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~practice_18();
};
