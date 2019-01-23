#include "example_26.h"

#include "managerList.h"
#include "gFunc.h"

#include "staticMesh.h"

constexpr int DEBUG_TYPE_BOX = 1;
constexpr int DEBUG_TYPE_SPHERE = 2;
constexpr int DEBUG_TYPE = DEBUG_TYPE_SPHERE;

example_26::example_26(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

example_26::~example_26()
{
}

void example_26::init(void)
{
	direct3dApplication::init();

	_mesh = createMesh(_bBox, _bSphere);
	_meshBox = createMeshBox(_bBox);

	_staticMesh = createStaticMesh();

	switch (DEBUG_TYPE)
	{
	case DEBUG_TYPE_BOX: _staticMesh->setDebugEnalbe(true, EDebugDrawType::BOX); break;
	case DEBUG_TYPE_SPHERE: _staticMesh->setDebugEnalbe(true, EDebugDrawType::SPHERE); break;
	}
	
}

void example_26::update(void)
{
	direct3dApplication::update();
}

void example_26::draw(void)
{
	direct3dApplication::draw();

	_staticMesh->draw();
/*
	MN_DEV->SetRenderState(D3DRS_LIGHTING, FALSE);
	MN_DEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	_mesh->DrawSubset(0);
	_meshBox->DrawSubset(0);

	MN_DEV->SetRenderState(D3DRS_LIGHTING, TRUE);
	MN_DEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/
}

LPD3DXMESH example_26::createMesh(boundingBox & bBox, boundingSphere & bSphere)
{
	LPD3DXMESH result = nullptr;

	D3DXLoadMeshFromX(
		"resource/mesh/Elementalist/Elementalist.x",
		D3DXMESH_MANAGED,
		MN_DEV,
		NULL,
		NULL,
		NULL,
		NULL,
		&result);

	// 경계 정보 설정
	void* vertex = nullptr;

	if (SUCCEEDED(result->LockVertexBuffer(0, &vertex)))
	{
		D3DXComputeBoundingBox(
			(D3DXVECTOR3*)vertex,
			result->GetNumVertices(),
			result->GetNumBytesPerVertex(),
			&bBox.min,
			&bBox.max);

		D3DXComputeBoundingSphere(
			(D3DXVECTOR3*)vertex,
			result->GetNumVertices(),
			result->GetNumBytesPerVertex(),
			&bSphere.center,
			&bSphere.radius);

		result->UnlockVertexBuffer();
	}

	return result;
}

LPD3DXMESH example_26::createMeshBox(const boundingBox & bBox)
{
	LPD3DXMESH result = nullptr;

	D3DXCreateBox(
		MN_DEV,
		bBox.max.x - bBox.min.x,
		bBox.max.y - bBox.min.y,
		bBox.max.z - bBox.min.z,
		&result,
		NULL);

	return result;
}

staticMesh * example_26::createStaticMesh(void)
{
	staticMesh::mParam param = {
		"resource/mesh/Elementalist/Elementalist.x",
		"resource/effect/example_15.fx"
	};

	return new staticMesh(param);
}

