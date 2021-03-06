#pragma once
#include "kGlobalDefine.h"
#include "pickRay.h"

class gFunc
{
public :
	// 이펙트 루프 실행
	static void runEffectLoop(LPD3DXEFFECT effect, const std::string & technique, const std::function<void(int)> & callback);
	static void runRenderTarget(LPDIRECT3DTEXTURE9 renderTarget, int clearOption, LPDIRECT3DSURFACE9 depthStensil, const std::function<void(void)> & callback, D3DCOLOR backColor = BACK_COLOR(255));

	// creater
	static LPDIRECT3DVERTEXBUFFER9	createVertexBuffer(int size, DWORD options, DWORD FVF);
	static LPDIRECT3DINDEXBUFFER9	createIndexBuffer(int size, DWORD options, DWORD format);
	static LPD3DXMESH				createMesh(int faceSize, int vertexSize, DWORD options, DWORD FVF);
	static LPD3DXMESH				createMesh(int faceSize, int vertexSize, DWORD options, D3DVERTEXELEMENT9* elements);
	static pick::ray				createPickRay(const POINT & clickPos);

	static boundingBox				createBoundingBox(LPD3DXMESH mesh);
	static boundingSphere			createBoundingSphere(LPD3DXMESH mesh);

	// 변환
	static DWORD					float2DWORD(float input) { float* f = &input; return *((DWORD*)f); };

	// 난수
	static int						rndInt(int min, int max);
	static float					rndFloat(float min, float max);

	// 충돌
	static bool						isIntersect(const boundingBox & boundA, const boundingBox & boundB);
	static bool						isIntersect(const boundingSphere & boundA, const boundingSphere & boundB);
	static void						obj2bound(boundingBox* outBoundingBox, objectBox* inOjbectBox);

private :
	gFunc() {};
	~gFunc() {};
};

