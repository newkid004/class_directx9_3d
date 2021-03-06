#include "gFunc.h"
#include "managerList.h"

#include "direct3dApplication.h"
#include "camera.h"

using namespace std;

void gFunc::runEffectLoop(LPD3DXEFFECT effect, const string & technique, const function<void(int)> & callback)
{
	effect->SetTechnique(technique.c_str());

	UINT numPass = 0;
	effect->Begin(&numPass, 0);

	for (int i = 0; i < numPass; ++i)
	{
		effect->BeginPass(i);
		callback(i);
		effect->EndPass();
	}

	effect->End();
}

void gFunc::runRenderTarget(LPDIRECT3DTEXTURE9 renderTarget, int clearOption, LPDIRECT3DSURFACE9 depthStensil, const std::function<void(void)>& callback, D3DCOLOR backColor)
{
	LPDIRECT3DSURFACE9 originRenderTarget = nullptr;
	LPDIRECT3DSURFACE9 originDepthStensil = nullptr;

	// 원본 보존
	MN_DEV->GetRenderTarget(0, &originRenderTarget);
	if (depthStensil)
	{
		MN_DEV->GetDepthStencilSurface(&originDepthStensil);
		MN_DEV->SetDepthStencilSurface(depthStensil);
	}

	// 렌더타겟 설정
	LPDIRECT3DSURFACE9 targetSurface = nullptr;
	if (renderTarget)
	{
		if (SUCCEEDED(renderTarget->GetSurfaceLevel(0, &targetSurface)))
		{
			MN_DEV->SetRenderTarget(0, targetSurface);
			SAFE_RELEASE(targetSurface);
		}

		// 렌더타겟 초기화
		if (clearOption)
			MN_DEV->Clear(0, NULL, clearOption, backColor, 1, 0);
	}

	// 구문 실행
	callback();

	// 원본 복구
	MN_DEV->SetRenderTarget(0, originRenderTarget);
	if (depthStensil)
		MN_DEV->SetDepthStencilSurface(originDepthStensil);

	SAFE_RELEASE(originRenderTarget);
	SAFE_RELEASE(originDepthStensil);
}

LPDIRECT3DVERTEXBUFFER9 gFunc::createVertexBuffer(int size, DWORD options, DWORD FVF)
{
	LPDIRECT3DVERTEXBUFFER9 result = nullptr;

	MN_DEV->CreateVertexBuffer(
		size,
		options,
		FVF,
		D3DPOOL_MANAGED,
		&result,
		NULL);

	return result;
}

LPDIRECT3DINDEXBUFFER9 gFunc::createIndexBuffer(int size, DWORD options, DWORD format)
{
	LPDIRECT3DINDEXBUFFER9 result = nullptr;

	MN_DEV->CreateIndexBuffer(
		size,
		options,
		(D3DFORMAT)format,
		D3DPOOL_MANAGED,
		&result,
		NULL);

	return result;
}

LPD3DXMESH gFunc::createMesh(int faceSize, int vertexSize, DWORD options, DWORD FVF)
{
	D3DVERTEXELEMENT9 elements[MAX_FVF_DECL_SIZE];
	ZeroMemory(&elements, sizeof(elements));

	D3DXDeclaratorFromFVF(FVF, elements);

	return createMesh(faceSize, vertexSize, options, elements);
}
LPD3DXMESH gFunc::createMesh(int faceSize, int vertexSize, DWORD options, D3DVERTEXELEMENT9* elements)
{
	LPD3DXMESH result = nullptr;

	D3DXCreateMesh(
		faceSize,
		vertexSize,
		options,
		elements,
		MN_DEV,
		&result);

	return result;
}

pick::ray gFunc::createPickRay(const POINT & clickPos)
{
	// 뷰 -> 투영 변환
	D3DVIEWPORT9 viewPort;
	MN_DEV->GetViewport(&viewPort);

	D3DXVECTOR2 normalizePos(
		((clickPos.x * 2.0f) / viewPort.Width) - 1.0f,
		((clickPos.y * -2.0f) / viewPort.Height) + 1.0f);

	// 투영 -> 뷰 변환
	D3DXMATRIXA16 & mProjection = GET_CAMERA()->getMatrixProjection();

	pick::ray ray;
	ZeroMemory(&ray, sizeof(ray));

	ray.direction = D3DXVECTOR3(
		normalizePos.x / mProjection._11,
		normalizePos.y / mProjection._22,
		1.0f);

	// 뷰 -> 월드 변환
	D3DXMATRIXA16 mView;
	MN_DEV->GetTransform(D3DTS_VIEW, &mView);

	D3DXMATRIXA16 miView;
	D3DXMatrixInverse(&miView, NULL, &mView);

	D3DXVec3TransformCoord(&ray.origin, &ray.origin, &miView);
	D3DXVec3TransformNormal(&ray.direction, &ray.direction, &miView);

	D3DXVec3Normalize(&ray.direction, &ray.direction);

	return ray;
}

boundingBox gFunc::createBoundingBox(LPD3DXMESH mesh)
{
	boundingBox result;

	// 경계 정보 설정
	void* vertex = nullptr;

	if (SUCCEEDED(mesh->LockVertexBuffer(0, &vertex)))
	{
		D3DXComputeBoundingBox(
			(D3DXVECTOR3*)vertex,
			mesh->GetNumVertices(),
			mesh->GetNumBytesPerVertex(),
			&result.min,
			&result.max);

		mesh->UnlockVertexBuffer();
	}

	return result;
}

boundingSphere gFunc::createBoundingSphere(LPD3DXMESH mesh)
{
	boundingSphere result;

	// 경계 정보 설정
	void* vertex = nullptr;

	if (SUCCEEDED(mesh->LockVertexBuffer(0, &vertex)))
	{
		D3DXComputeBoundingSphere(
			(D3DXVECTOR3*)vertex,
			mesh->GetNumVertices(),
			mesh->GetNumBytesPerVertex(),
			&result.center,
			&result.radius);

		mesh->UnlockVertexBuffer();
	}

	return result;
}

int gFunc::rndInt(int min, int max)
{
	random_device rDevice;
	uniform_int_distribution<int> uRandom(min, max);

	return uRandom(rDevice);
}

float gFunc::rndFloat(float min, float max)
{
	random_device rDevice;
	uniform_real_distribution<float> uRandom(min, max);

	return uRandom(rDevice);
}

bool gFunc::isIntersect(const boundingBox & boundA, const boundingBox & boundB)
{
	if (boundB.max.x < boundA.min.x || boundA.max.x < boundB.min.x ||
		boundB.max.y < boundA.min.y || boundA.max.y < boundB.min.y ||
		boundB.max.z < boundA.min.z || boundA.max.z < boundB.min.z)
		return false;

	return true;
}

bool gFunc::isIntersect(const boundingSphere & boundA, const boundingSphere & boundB)
{
	auto distance = boundA.center - boundB.center;
	return D3DXVec3Length(&distance) <= boundA.radius + boundB.radius;
}

void gFunc::obj2bound(boundingBox * outBoundingBox, objectBox * inObjectBox)
{
	ZeroMemory(outBoundingBox, sizeof(boundingBox));
	
	D3DXVECTOR3 & center = inObjectBox->center;
	D3DXVECTOR3 direction[] = {
		inObjectBox->direction[0] * inObjectBox->halfLength[0],
		inObjectBox->direction[1] * inObjectBox->halfLength[1],
		inObjectBox->direction[2] * inObjectBox->halfLength[2]
	};

	// 경계 볼륨
	D3DXVECTOR3 vertices[] = {
		center + direction[0] + direction[1] + direction[2],
		center + direction[0] - direction[1] + direction[2],
		center - direction[0] + direction[1] + direction[2],
		center - direction[0] - direction[1] + direction[2],

		center + direction[0] + direction[1] - direction[2],
		center + direction[0] - direction[1] - direction[2],
		center - direction[0] + direction[1] - direction[2],
		center - direction[0] - direction[1] - direction[2]
	};

	outBoundingBox->min = center;
	outBoundingBox->max = center;

	// 위치 계산
	for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); ++i)
	{
		D3DXVECTOR3 & v = vertices[i];

		if (v.x <	outBoundingBox->min.x)	outBoundingBox->min.x = v.x;
		if (v.x >=	outBoundingBox->max.x)	outBoundingBox->max.x = v.x;

		if (v.y <	outBoundingBox->min.y)	outBoundingBox->min.y = v.y;
		if (v.y >=	outBoundingBox->max.y)	outBoundingBox->max.y = v.y;

		if (v.z <	outBoundingBox->min.z)	outBoundingBox->min.z = v.z;
		if (v.z >=	outBoundingBox->max.z)	outBoundingBox->max.z = v.z;
	}
}
