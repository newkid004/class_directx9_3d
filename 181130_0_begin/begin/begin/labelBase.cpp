#include "labelBase.h"

#include "managerList.h"

labelBase::labelBase(const std::string & text, float size) :
	_text(text)
{
	_font = createFont(size);
}

labelBase::~labelBase()
{
	SAFE_RELEASE(_font);
}

void labelBase::drawDoUI(void)
{
	RECT rcText = { _position.x , _position.y, 0, 0 };

	_font->DrawTextA(
		NULL,
		_text.c_str(),
		-1,
		&rcText,
		DT_TOP | DT_LEFT | DT_CALCRECT,
		_diffuseColor);

	_font->DrawTextA(
		NULL,
		_text.c_str(),
		-1,
		&rcText,
		DT_TOP | DT_LEFT,
		_diffuseColor);
}

LPD3DXFONT labelBase::createFont(float size)
{
	LPD3DXFONT result = nullptr;
	D3DXFONT_DESC fontDesc;
	ZeroMemory(&fontDesc, sizeof(fontDesc));

	fontDesc.Height = size;
	fontDesc.Weight = 1000;

	D3DXCreateFontIndirect(
		MN_DEV,
		&fontDesc,
		&result);

	return result;
}