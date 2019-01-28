#include "debugDraw.h"

#include "managerList.h"
#include "gFunc.h"

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
	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	MN_DEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

void debugDraw::drawDo(void)
{
	renderObject::drawDo();

	if (_drawType == EDebugDrawType::BOX)
		drawBoundingBox();
	else if (_drawType == EDebugDrawType::SPHERE)
		drawBoundingSphere();

	drawGuideLine();
}

void debugDraw::drawPost(void)
{
	renderObject::drawPost();

	MN_DEV->SetRenderState(D3DRS_LIGHTING, TRUE);
	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	MN_DEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void debugDraw::drawBoundingBox(void)
{
	if (_meshBoundingBox == nullptr) return;

	D3DXMATRIXA16 mWorld;
	getMatrixBound(&mWorld, EDebugDrawType::BOX);
	MN_DEV->SetTransform(D3DTS_WORLD, &mWorld);

	_meshBoundingBox->DrawSubset(0);

}

void debugDraw::drawBoundingSphere(void)
{
	if (_meshBoundingSphere == nullptr) return;

	D3DXMATRIXA16 mWorld;
	getMatrixBound(&mWorld, EDebugDrawType::SPHERE);
	MN_DEV->SetTransform(D3DTS_WORLD, &mWorld);

	_meshBoundingSphere->DrawSubset(0);
}

void debugDraw::drawGuideLine(void)
{
	vertex vertices[2];
	ZeroMemory(vertices, sizeof(vertices));

	vertices[0].diffuse = COLOR_YELLOW(255);
	vertices[1].diffuse = COLOR_YELLOW(255);

	D3DXMATRIXA16 mWorld;
	D3DXMatrixIdentity(&mWorld);
	MN_DEV->SetTransform(D3DTS_WORLD, &mWorld);

	switch (_drawType)
	{
	case EDebugDrawType::BOX: {
		boundingBox bound;
		_bindObject->getBoundingBoxFinal(&bound);

		vertices[0].pos = bound.min;
		vertices[1].pos = bound.max;

		drawGuideLineBox();
	} break;

	case EDebugDrawType::SPHERE: {
		boundingSphere bound;
		_bindObject->getBoundingSphereFinal(&bound);

		vertices[0].pos = bound.center;
		vertices[1].pos = bound.center + (D3DXVECTOR3(0.707f, 0.707f, -0.707f) * bound.radius);
	} break;
	}

	MN_DEV->SetFVF(vertex::FVF);
	MN_DEV->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(vertex));
}

void debugDraw::drawGuideLineBox(void)
{
	vertex vertices[24];
	ZeroMemory(vertices, sizeof(vertices));

	for (auto & v : vertices) 
		v.diffuse = COLOR_GREEN(255);

	objectBox objBox;
	boundingBox bndBox;
	
	_bindObject->getObjectBox(&objBox);	
	gFunc::obj2bound(&bndBox, &objBox);

	D3DXVECTOR3 boxInterval(
		bndBox.max.x - bndBox.min.x,
		bndBox.max.y - bndBox.min.y,
		bndBox.max.z - bndBox.min.z);

	// ----- 하단 ----- // ↑ → ↓ ←
	vertices[0].pos = bndBox.min;
	vertices[1].pos = vertices[0].pos + D3DXVECTOR3(0.0f, 0.0f, boxInterval.z);

	vertices[2].pos = vertices[1].pos;
	vertices[3].pos = vertices[2].pos + D3DXVECTOR3(boxInterval.x, 0.0f, 0.0f);

	vertices[4].pos = vertices[3].pos;
	vertices[5].pos = vertices[4].pos + D3DXVECTOR3(0.0f, 0.0f, -boxInterval.z);

	vertices[6].pos = vertices[5].pos;
	vertices[7].pos = vertices[6].pos + D3DXVECTOR3(-boxInterval.x, 0.0f, 0.0f);

	// ----- 상단 ----- // ↑ → ↓ ←
	vertices[8 + 0].pos = bndBox.min + D3DXVECTOR3(0.0f, boxInterval.y, 0.0f);
	vertices[8 + 1].pos = vertices[8 + 0].pos + D3DXVECTOR3(0.0f, 0.0f, boxInterval.z);

	vertices[8 + 2].pos = vertices[8 + 1].pos;
	vertices[8 + 3].pos = vertices[8 + 2].pos + D3DXVECTOR3(boxInterval.x, 0.0f, 0.0f);

	vertices[8 + 4].pos = vertices[8 + 3].pos;
	vertices[8 + 5].pos = vertices[8 + 4].pos + D3DXVECTOR3(0.0f, 0.0f, -boxInterval.z);

	vertices[8 + 6].pos = vertices[8 + 5].pos;
	vertices[8 + 7].pos = vertices[8 + 6].pos + D3DXVECTOR3(-boxInterval.x, 0.0f, 0.0f);

	// ----- 세로 선 ----- // ↙ ↖ ↗ ↘
	vertices[16].pos = vertices[0 + 0].pos;
	vertices[17].pos = vertices[0 + 8].pos;

	vertices[18].pos = vertices[2 + 0].pos;
	vertices[19].pos = vertices[2 + 8].pos;

	vertices[20].pos = vertices[4 + 0].pos;
	vertices[21].pos = vertices[4 + 8].pos;

	vertices[22].pos = vertices[6 + 0].pos;
	vertices[23].pos = vertices[6 + 8].pos;

	MN_DEV->SetFVF(vertex::FVF);
	MN_DEV->DrawPrimitiveUP(D3DPT_LINELIST, 12, vertices, sizeof(vertex));
}

LPD3DXMESH debugDraw::createBoundingBox(boundingBox & input)
{
	LPD3DXMESH result = nullptr;

	_bBox = input;

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

	_bSphere = input;

	D3DXCreateSphere(
		MN_DEV,
		input.radius,
		10,
		10,
		&result,
		NULL);

	return result;
}

LPD3DXMESH debugDraw::getBoundMesh(EDebugDrawType type)
{
	if (type == EDebugDrawType::NONE)
		type = _drawType;

	switch (type)
	{
	case EDebugDrawType::BOX:		return _meshBoundingBox;	break;
	case EDebugDrawType::SPHERE:	return _meshBoundingSphere;	break;
	}

	return nullptr;
}

void debugDraw::getMatrixBound(D3DXMATRIXA16 * outMat, EDebugDrawType type)
{
	if (type == EDebugDrawType::NONE)
		type = _drawType;

	switch (type)
	{
	case EDebugDrawType::BOX: {

		D3DXMATRIXA16 mTranslate;
		D3DXMatrixTranslation(
			&mTranslate,
			(_bBox.min.x + _bBox.max.x) / 2.0f,
			(_bBox.min.y + _bBox.max.y) / 2.0f,
			(_bBox.min.z + _bBox.max.z) / 2.0f);

		*outMat = mTranslate * _bindObject->getMatrixFinal();
	} break;

	case EDebugDrawType::SPHERE: {
		D3DXMATRIXA16 mTranslate;
		D3DXMatrixTranslation(
			&mTranslate,
			_bSphere.center.x,
			_bSphere.center.y,
			_bSphere.center.z);

		*outMat = mTranslate * _bindObject->getMatrixFinal();
	} break;
	}
}
