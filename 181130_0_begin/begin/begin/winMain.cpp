#include "kGlobalDefine.h"

#include "exampleList.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	SIZE size = { WINSIZEX, WINSIZEY };

	//*/
	example_26 exam(hInstance, size, nCmdShow);
	/*/
	practice_19 exam(hInstance, size, nCmdShow);
	//*/
	
	return exam.run();
}
