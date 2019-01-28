#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class staticMesh;

class example_26 : public direct3dApplication
{
private :
	LPD3DXMESH _mesh = nullptr;
	LPD3DXMESH _meshBox = nullptr;
	LPD3DXMESH _meshSphere = nullptr;

	staticMesh* _staticMesh = nullptr;
	staticMesh* _staticMeshA = nullptr;

	boundingBox _bBox;
	boundingSphere _bSphere;

public :
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public :
	LPD3DXMESH createMesh(boundingBox & bBox, boundingSphere & bSphere);
	LPD3DXMESH createMeshBox(const boundingBox & bBox);

	staticMesh* createStaticMesh(void);

public:
	example_26(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_26();
};

