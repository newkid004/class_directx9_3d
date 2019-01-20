#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class baseObject;
class practice_04 : public direct3dApplication
{
public:
	struct vertex
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_NORMAL
		};

		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
	};

private:
	LPDIRECT3DVERTEXBUFFER9 _vertexBuffer[3];

	float _yaw = 0.0f;

	D3DLIGHT9 _light;

	int _offsetMaterial = 0;
	int _offsetShader = 0;
	baseObject* _objFigure[3];
	D3DMATERIAL9 _material[3];

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

	void initMaterial(void);
	void updateControl(void);

	void setMaterial(int index);
	void setShader(int index);

	void drawCube(void);
	void drawHorn(void);
	void drawOcta(void);

private:
	LPDIRECT3DVERTEXBUFFER9 createVertex2Cube(void);
	LPDIRECT3DVERTEXBUFFER9 createVertex2Horn(void);
	LPDIRECT3DVERTEXBUFFER9 createVertex2Octa(void);

public:
	practice_04(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~practice_04();
};

