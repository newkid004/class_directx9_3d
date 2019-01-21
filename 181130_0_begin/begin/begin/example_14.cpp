#include "example_14.h"
#include "managerList.h"
#include "camera.h"
#include "gFunc.h"

example_14::example_14(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_14::~example_14()
{
	SAFE_RELEASE(_font);
}

void example_14::init(void)
{
	direct3dApplication::init();

	_font = createFont();
}

void example_14::update(void)
{
	direct3dApplication::update();
}

void example_14::draw(void)
{
	direct3dApplication::draw();

	RECT rcText;
	ZeroMemory(&rcText, sizeof(RECT));

	// 영역 계산
	_font->DrawTextA(
		NULL, 
		"ㅁㄴㅇㄹ", 
		-1, 
		&rcText, 
		DT_TOP | DT_LEFT | DT_CALCRECT, 
		D3DCOLOR_XRGB(255, 255, 255));

	// 출력
	_font->DrawTextA(
		NULL,
		"ㅁㄴㅇㄹ",
		-1,
		&rcText,
		DT_TOP | DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));
}

LPD3DXFONT example_14::createFont(void)
{
	LPD3DXFONT result = nullptr;
	D3DXFONT_DESC fontDesc;
	ZeroMemory(&fontDesc, sizeof(D3DXFONT_DESC));

	fontDesc.Height = 30;
	fontDesc.Weight = 1000;

	D3DXCreateFontIndirect(MN_DEV, &fontDesc, &result);

	return result;
}