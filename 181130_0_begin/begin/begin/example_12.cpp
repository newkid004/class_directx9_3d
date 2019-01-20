#include "example_12.h"
#include "managerList.h"
#include "camera.h"

#define VERTEX_FORMAT_TYPE_FVF			1
#define VERTEX_FORMAT_TYPE_DECLARATION	2

#define VERTEX_FORMAT_TYPE		VERTEX_FORMAT_TYPE_DECLARATION

example_12::example_12(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_12::~example_12()
{
	SAFE_RELEASE(_effect);
	SAFE_RELEASE(_mesh);
	SAFE_RELEASE(_texture);
}

void example_12::init(void)
{
	direct3dApplication::init();

	_effect = createEffect();
	_mesh = createMesh();
	_texture = createTexture();
}

void example_12::update(void)
{
	direct3dApplication::update();
	// _yaw += 90.0f * MN_TIME->getDeltaTime();
}

void example_12::draw(void)
{
	direct3dApplication::draw();

	// 행렬
	D3DXMATRIXA16 mWorld;
	D3DXMatrixRotationY(&mWorld, D3DXToRadian(_yaw));

	_effect->SetMatrix("_mWorld", &mWorld);
	_effect->SetMatrix("_mView", GET_CAMERA()->getMatrixViewPoint());
	_effect->SetMatrix("_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	// 광원
	D3DXVECTOR4 lightDirection(0.0f, 0.0f, 1.0f, 0.0f);
	_effect->SetVector("_lightDirection", &lightDirection);

	// 카메라
	D3DXVECTOR4 viewPosition(GET_CAMERA()->getPosition(), 1.0f);
	_effect->SetVector("_viewPosition", &viewPosition);

	// 텍스쳐
	_effect->SetTexture("_texture", _texture);

	// 이펙트
	_effect->SetTechnique("myTechnique");

	UINT numPasses = 0;
	_effect->Begin(&numPasses, 0);

	for (int i = 0; i < numPasses; ++i)
	{
		_effect->BeginPass(i);

		/*
			메시가 N개 이상의 서브셋으로 구성되었을 경우
			해당 개수만큼 DrawSubset함수를 호출해야 원하는 결과를 출력 가능
			그 이외엔 0
		*/
		_mesh->DrawSubset(0);

		_effect->EndPass();
	}

	_effect->End();
	// 메쉬

}

LPD3DXEFFECT example_12::createEffect(void)
{
	LPD3DXEFFECT result = NULL;

	LPD3DXBUFFER bufError = NULL;

	D3DXCreateEffectFromFile(
		MN_DEV,
		TEXT("resource/effect/example_12.fx"),
		NULL,
		NULL,
		D3DXSHADER_DEBUG,
		NULL,
		&result,
		&bufError);

	if (bufError != nullptr)
	{
		auto msgError = (char*)bufError->GetBufferPointer();
		printf("Example_12.createEffect : %s\n", msgError);
	}

	return result;
}

LPD3DXMESH example_12::createMesh(void)
{
	LPD3DXMESH result = NULL;

#if VERTEX_FORMAT_TYPE == VERTEX_FORMAT_TYPE_FVF
	// mesh의 프리미티브 타입은 삼각형으로 고정
	// 메시를 통해 라인, 점 등의 물체를 구성하는 것은 비효율적
	D3DXCreateMeshFVF(
		sizeof(WORD) * 2,	// 삼각형의 괴수(안덱스 개수), 기본적으로 2 byte
		sizeof(vertex) * 4,	// 정점개수
		D3DXMESH_MANAGED,	//
		vertex::FVF, 		//
		MN_DEV,				//
		&result);			//
#else
	/*
		WORD    Stream;     // 스트림 번호
		WORD    Offset;     // 메모리의 첫 위치부터 몇 바이트만큼 떨어져 있는지의 간격
		BYTE    Type;       // 자료형 타입
		BYTE    Method;     // 그래픽 카드의 테셀레이션에 관한 정보
		BYTE    Usage;      // 시멘틱
		BYTE    UsageIndex; // 시멘틱 번호(중복될 경우)

		D3DVERTEXELEMENT9 특징
		- 중복 시멘틱 선언 가능 (FVF는 불가)
		- FVF엔 존재하지 않는 시멘틱 선언 가능 (Ex. TANGENT(접선), BINORMAL(종법선), ...)
		- FVF와 달리 구조체 맴버의 선언 순서에 영향받지 않음
	*/
	D3DVERTEXELEMENT9 verElement[] = {
		/* 위치 정보 설정 */	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		/* 법선 정보 설정 */	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		/* UV 정보 설정 */		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		/* 설정 끝*/			D3DDECL_END()
	};

	D3DXCreateMesh(
		sizeof(WORD) * 2,	
		sizeof(vertex) * 4,	
		D3DXMESH_MANAGED,	
		verElement,
		MN_DEV,				
		&result);			

	// D3DXComputeNormals : 정점 정보 계산	

#endif	// VERTEX_FORMAT_TYPE == VERTEX_FORMAT_TYPE_DECLARATION


	// 정점 설정
	vertex* v = NULL;

	// 메시는 일반적인 정점/인덱스 버퍼와는 달리 버퍼의 일부분만 얻어오는 것이 불가능
	// 일부분 제어 시, 별도 로직 필요
	if (SUCCEEDED(result->LockVertexBuffer(0, (void**)&v)))
	{
		v[0].pos = D3DXVECTOR3(-1, -1, 0);
		v[1].pos = D3DXVECTOR3(-1,  1, 0);
		v[2].pos = D3DXVECTOR3( 1,  1, 0);
		v[3].pos = D3DXVECTOR3( 1, -1, 0);
		
		float fDir = 0.707f;
		v[0].normal = D3DXVECTOR3(0, 0, -fDir); D3DXVec3Normalize(&v[0].normal, &v[0].normal);
		v[1].normal = D3DXVECTOR3(0, 0, -fDir); D3DXVec3Normalize(&v[1].normal, &v[1].normal);
		v[2].normal = D3DXVECTOR3(0, 0, -fDir); D3DXVec3Normalize(&v[2].normal, &v[2].normal);
		v[3].normal = D3DXVECTOR3(0, 0, -fDir); D3DXVec3Normalize(&v[3].normal, &v[3].normal);

		v[0].uv = D3DXVECTOR2(0.0f, 1.0f);
		v[1].uv = D3DXVECTOR2(0.0f, 0.0f);
		v[2].uv = D3DXVECTOR2(1.0f, 0.0f);
		v[3].uv = D3DXVECTOR2(1.0f, 1.0f);

		result->UnlockVertexBuffer();
	}

	// index 설절
	WORD* i = NULL;
	if (SUCCEEDED(result->LockIndexBuffer(0, (void**)&i)))
	{
		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 2; i[4] = 3; i[5] = 0;

		result->UnlockIndexBuffer();
	}

	return result;
}

LPDIRECT3DTEXTURE9 example_12::createTexture(void)
{
	LPDIRECT3DTEXTURE9 result = NULL;

	D3DXCreateTextureFromFile(MN_DEV,
		TEXT("resource/texture/practice_05.png"),
		&result);

	return result;
}