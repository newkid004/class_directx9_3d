#pragma once
#include "kGlobalDefine.h"
#include "baseObject.h"

class lightBase : public baseObject
{
private :
	int _index = 0;
	bool _isEnable = false;

	D3DLIGHT9 _light;

	D3DXMATRIXA16 _mView;
	D3DXMATRIXA16 _mProjection;

public :
	virtual void update(void) override;

public :
	bool getLightable(void)			{ return _isEnable; }
	void setLightable(bool input)	{ _isEnable = input; }

	void setDiffuseColor(const D3DXCOLOR & color) { _light.Diffuse = color; }

	constexpr D3DXMATRIXA16 & getMatrixView(void) { return _mView; }
	constexpr D3DXMATRIXA16 & getMatrixProjection(void) { return _mProjection; }

private :
	void setView(D3DXMATRIXA16 & input);
	void setProjection(D3DXMATRIXA16 & input);

public:
	lightBase(int index);
	~lightBase();
};

