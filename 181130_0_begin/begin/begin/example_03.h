#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	D3D √ ±‚»≠
*/

class example_03 : public direct3dApplication
{
public :
	void init(void) override;
	LRESULT handleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

public:
	example_03(HINSTANCE hInstance, const SIZE & windowSize, int option);
	virtual ~example_03();
};

