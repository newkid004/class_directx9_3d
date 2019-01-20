#pragma once
#include "example.h"

class example_01 : public example
{
protected :
	void init() override;

public :
	example_01 DEF_CREATE{};
	~example_01() {};
};