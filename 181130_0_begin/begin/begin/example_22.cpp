#include "example_22.h"

#include "managerList.h"

example_22::example_22(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

example_22::~example_22()
{
}

void example_22::init(void)
{
	direct3dApplication::init();
}

void example_22::update(void)
{
	direct3dApplication::update();
}

void example_22::draw(void)
{
	direct3dApplication::draw();
}
