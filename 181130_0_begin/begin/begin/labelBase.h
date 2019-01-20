#pragma once
#include "kGlobalDefine.h"
#include "UIBase.h"

using namespace std;

class labelBase : public UIBase
{
private :
	string _text;
	LPD3DXFONT _font = nullptr;

protected :
	virtual void drawDoUI(void);

private :
	LPD3DXFONT createFont(float size);

public:
	void setText(const string & input) { _text = input; }

public:
	labelBase(const std::string & text, float size);
	~labelBase();
};

