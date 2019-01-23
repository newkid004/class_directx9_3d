#include "debugDraw.h"

#include "managerList.h"

debugDraw::debugDraw(renderObject * bind, EDebugDrawType drawType) :
	_bindObject(bind),
	_drawType(drawType)
{

	switch (drawType)
	{ 
	case EDebugDrawType::BOX: _meshBoundingBox = createBoundingBox(bind->getBoundingBox()); break;
	case EDebugDrawType::SPHERE: _meshBoundingSphere = createBoundingSphere(bind->getBoundingSphere()); break;
	}
}

debugDraw::~debugDraw()
{
	SAFE_RELEASE(_meshBoundingBox);
	SAFE_RELEASE(_meshBoundingSphere);
}

void debugDraw::drawPre(void)
{
	renderObject::drawPre();

	MN_DEV->SetRenderState(D3DRS_LIGHTING, FALSE);
	MN_DEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

void debugDraw::drawDo(void)
{
	renderObject::drawDo();

	if (_drawType == EDebugDrawType::BOX)
		drawBoundingBox();
	else if (_drawType == EDebugDrawType::SPHERE)
		drawBoundingSphere();
}

void debugDraw::drawPost(void)
{
	renderObject::drawPost();

	MN_DEV->SetRenderState(D3DRS_LIGHTING, TRUE);
	MN_DEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void debugDraw::drawBoundingBox(void)
{
	if (_meshBoundingBox == nullptr) return;

	D3DXMATRIXA16 mWorld = _bindObject->getMatrixFinal();
	MN_DEV->SetTransform(D3DTS_WORLD, &mWorld);

	_meshBoundingBox->DrawSubset(0);
}

void debugDraw::drawBoundingSphere(void)
{
	if (_meshBoundingSphere == nullptr) return;

	D3DXMATRIXA16 mWorld = _bindObject->getMatrixFinal();
	MN_DEV->SetTransform(D3DTS_WORLD, &mWorld);

	_meshBoundingSphere->DrawSubset(0);
}

LPD3DXMESH debugDraw::createBoundingBox(boundingBox & input)
{
	LPD3DXMESH result = nullptr;

	D3DXCreateBox(
		MN_DEV,
		input.max.x - input.min.x,
		input.max.y - input.min.y,
		input.max.z - input.min.z,
		&result,
		NULL);

	return result;
}

LPD3DXMESH debugDraw::createBoundingSphere(boundingSphere & input)
{
	LPD3DXMESH result = nullptr;

	D3DXCreateSphere(
		MN_DEV,
		input.radius,
		10,
		10,
		&result,
		NULL);

	return result;
}
