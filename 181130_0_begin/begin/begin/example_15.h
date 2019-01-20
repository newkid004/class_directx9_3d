#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< X 파일 >
	: directX9에서 공식적으로 제공되며 3차원 모델링이 표현 가능한 파일 형식

	X 파일에서 표현할 수 있는 데이터 구조
	- 모델링 ( 물체를 이루고 있는 정점 )
	- 재질 ( 물체를 렌더링 하는 데이터 )
	- 스키닝 애니메이션 ( 물체를 애니메이션 하기위한 데이터

*/

using namespace std;

class staticMesh;

class example_15 : public direct3dApplication
{
private :
	staticMesh* _staticMesh;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public :
	staticMesh* createStaticMesh(void);

public:
	example_15(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_15();
};

