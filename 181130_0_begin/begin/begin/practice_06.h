#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class baseObject;

class practice_06 : public direct3dApplication
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

	enum e_MESH
	{
		CYLINDER,
		TORUS,
		SPHERE,
		BOX,
		TEAPOT,
		COUNT
	};

private:
	LPD3DXEFFECT _effect = NULL;

	baseObject* _obj[e_MESH::COUNT];
	LPD3DXMESH _meshes[e_MESH::COUNT];
	D3DXVECTOR4 _diffuses[e_MESH::COUNT];

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public :
	LPD3DXEFFECT createEffect(void);
	void createMeshes(void);
	void createDiffuses(void);

public:
	practice_06(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~practice_06();
};

