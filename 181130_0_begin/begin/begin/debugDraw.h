#pragma once

#include "kGlobalDefine.h"
#include "renderObject.h"

class debugDraw : public renderObject
{
private :
	LPD3DXMESH _meshBoundingBox = nullptr;
	LPD3DXMESH _meshBoundingSphere = nullptr;

	renderObject* _bindObject = nullptr;
	EDebugDrawType _drawType;

protected:
	virtual void drawPre(void) override;
	virtual void drawDo(void) override;
	virtual void drawPost(void) override;

private :
	void drawBoundingBox(void);
	void drawBoundingSphere(void);
	LPD3DXMESH createBoundingBox(boundingBox & input);
	LPD3DXMESH createBoundingSphere(boundingSphere & input);

public:
	debugDraw(renderObject* bind, EDebugDrawType drawType);
	~debugDraw();
};

