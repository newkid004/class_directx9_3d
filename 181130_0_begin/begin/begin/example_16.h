#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< 스키닝 애니메이션 >
	: 기존의 본 애니메이션이 지닌 단점을 보완하기 위해 나온 애니메이션 기법
	  본의 움직임에 따라 본에 종속된 정점이 가중치의 영향을 받아
	  자연스럽게 관절부위를 이어주는 애니메이션 기법
*/

class skinnedMesh;

class example_16 : public direct3dApplication
{
private :
	skinnedMesh* _skinnedMesh = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

	void updateControl(void);

private :
	skinnedMesh* createSkinnedMesh(void);

public:
	example_16(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_16();
};

