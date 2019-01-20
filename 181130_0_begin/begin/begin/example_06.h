#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class baseObject;
class camera;

class example_06 : public direct3dApplication
{
public:
	struct vertex
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE
		};

		D3DXVECTOR3 pos;
		D3DCOLOR color;
	};

private :
	IDirect3DVertexBuffer9* _vertexBuffer = NULL;

	baseObject* _objRoot = NULL;
	baseObject* _objBody = NULL;
	baseObject* _objHead = NULL;
	baseObject* _objLArm = NULL;
	baseObject* _objRArm = NULL;
	baseObject* _objLLeg = NULL;
	baseObject* _objRLeg = NULL;

	baseObject* _objCubes[5];

	camera* _camera = NULL;

	bool _horizontal = true;
	bool _thirdP = true;

	float _pitch = 0.f;
	float _pitchVel = 90.f;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public :
	void updateControl(void);
	void drawCube(const D3DXMATRIXA16 & mWorld);

public :
	IDirect3DVertexBuffer9* createVertexBuffer(void);

public:
	example_06(HINSTANCE hInstance, const SIZE & windowSize, int option);
	virtual ~example_06();
};

