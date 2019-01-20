#include "example_03.h"


example_03::example_03(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_03::~example_03()
{
}

void example_03::init(void)
{
	direct3dApplication::init();
}

LRESULT example_03::handleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
/*
	switch (message) {
	case WM_TIMER: { this->msgTimer(wParam, lParam); } break;
	case WM_PAINT: { this->msgPaint(wParam, lParam); } break;
	case WM_KEYDOWN: { this->msgKeyDown(wParam, lParam); } break;
	}
*/
	return direct3dApplication::handleWindowMessage(hWnd, message, wParam, lParam);
}
