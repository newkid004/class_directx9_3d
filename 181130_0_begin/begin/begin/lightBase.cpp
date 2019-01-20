#include "lightBase.h"

#include "managerList.h"

lightBase::lightBase(int index) :
	_index(index)
{
	ZeroMemory(&_light, sizeof(_light));

	_light.Type = D3DLIGHT_DIRECTIONAL;
	_light.Diffuse = D3DXCOLOR(COLOR_WHITE(255));

	D3DXMatrixIdentity(&_mView);

	setView(_mView);
	setProjection(_mProjection);
}

lightBase::~lightBase()
{
}

void lightBase::update(void)
{
	baseObject::update();
	_light.Direction = _directionForward;

	MN_DEV->SetLight(_index, &_light);
	MN_DEV->LightEnable(_index, _isEnable);

	setView(_mView);
}

void lightBase::setView(D3DXMATRIXA16 & input)
{
	input(0, 0) = _directionRight.x;
	input(1, 0) = _directionRight.y;
	input(2, 0) = _directionRight.z;
	input(3, 0) = -D3DXVec3Dot(&_position, &_directionRight);

	input(0, 1) = _directionUp.x;
	input(1, 1) = _directionUp.y;
	input(2, 1) = _directionUp.z;
	input(3, 1) = -D3DXVec3Dot(&_position, &_directionUp);

	input(0, 2) = _directionForward.x;
	input(1, 2) = _directionForward.y;
	input(2, 2) = _directionForward.z;
	input(3, 2) = -D3DXVec3Dot(&_position, &_directionForward);
}

void lightBase::setProjection(D3DXMATRIXA16 & input)
{
	D3DXMatrixPerspectiveFovLH(
		&input,
		D3DX_PI / 2.0f,
		1.0f,
		0.1f,
		1000.0f
	);
}