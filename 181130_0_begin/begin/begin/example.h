#pragma once
#include <Windows.h>
#include "iWindowApplication.h"

#define DEF_CREATE (HINSTANCE hInstance, const SIZE & windowSize, int option):example(hInstance, windowSize, option)

class example : public iWindowApplication
{
public :
	virtual LRESULT handleWindowMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override
	{
		return iWindowApplication::handleWindowMessage(hWnd, message, wParam, lParam);
	};

public :
	example(HINSTANCE hInstance, const SIZE & windowSize, int option) : iWindowApplication(hInstance, windowSize, option) {};
	virtual ~example() {};
};

