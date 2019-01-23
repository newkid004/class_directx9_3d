#include "direct3dApplication.h"
#include "renderObject.h"
#include "managerList.h"
#include "gFunc.h"
#include "camera.h"
#include "debugGizmo.h"
#include "debugDraw.h"

renderObject::renderObject() :
	_isVisible(true)
{
}

renderObject::~renderObject()
{
}

void renderObject::draw(void)
{
	if (_isVisible)
	{
		drawPre();
		drawDo();
		drawPost();

		for (auto child : _vChildren)
		{
			auto rObject = dynamic_cast<renderObject*>(child);

			if (rObject != nullptr)
				rObject->draw();
		}
	}
}

void renderObject::getBoundingBoxFinal(boundingBox * out)
{
	D3DXMATRIXA16 mWorld = getMatrixFinal();

	D3DXVec3TransformCoord(
		&out->min,
		&_bBox.min,
		&mWorld);

	D3DXVec3TransformCoord(
		&out->max,
		&_bBox.max,
		&mWorld);
}

void renderObject::getBoundingSphereFinal(boundingSphere * out)
{
	D3DXMATRIXA16 mWorld = getMatrixFinal();

	D3DXVec3TransformCoord(
		&out->center,
		&_bSphere.center,
		&mWorld);

	out->radius = max(max(mWorld(0, 0), mWorld(1, 1)), mWorld(2, 2));
}

void renderObject::setDebugEnalbe(bool input, EDebugDrawType drawType)
{
	if (_isDebugEnable != input)
	{
		_isDebugEnable = input;

		if (!_isDebugEnable)
		{
			removeChild(_debugDraw);
			SAFE_DELETE(_debugDraw);
		}
		else
		{
			_debugDraw = new debugDraw(this, drawType);
			_debugDraw->setPosition(D3DXVECTOR3(0, 0, 0));

			addChild(_debugDraw);
		}
	}
}
