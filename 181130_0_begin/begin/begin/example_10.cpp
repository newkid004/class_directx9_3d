#include "example_10.h"
#include "managerList.h"

#include "camera.h"

example_10::example_10(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_10::~example_10()
{
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_vertexShader);
	SAFE_RELEASE(_constantTable);
	SAFE_RELEASE(_pixelShader);
}

void example_10::init(void)
{
	direct3dApplication::init();

	_vertexBuffer = createVertexBuffer();
	_vertexShader = createVertexShader(&_constantTable);

	_pixelShader = createPixelShader();
}

void example_10::update(void)
{
	direct3dApplication::update();

	_yaw += 90.0f * MN_TIME->getDeltaTime();
}

void example_10::draw(void)
{
	direct3dApplication::draw();

	MN_DEV->SetRenderState(D3DRS_LIGHTING, false);
	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ---------------------------- //

	// 월드
	D3DXMATRIXA16 mWorld;
	D3DXMatrixRotationY(&mWorld, D3DXToRadian(_yaw));

	//*/ 셰이더 사용
	MN_DEV->SetVertexShader(_vertexShader);
	MN_DEV->SetPixelShader(_pixelShader);

	_constantTable->SetMatrix(MN_DEV, "_mWorld", &mWorld);
	_constantTable->SetMatrix(MN_DEV, "_mView", GET_CAMERA()->getMatrixViewPoint());
	_constantTable->SetMatrix(MN_DEV, "_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	/*/ // 월드 출력
	MN_DEV->SetTransform(D3DTS_WORLD, &mWorld);
	//*/

	MN_DEV->SetStreamSource(0, _vertexBuffer, 0, sizeof(vertex));
	MN_DEV->SetFVF(vertex::FVF);

	MN_DEV->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	// ---------------------------- //
	MN_DEV->SetRenderState(D3DRS_LIGHTING, true);
	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

LPDIRECT3DVERTEXBUFFER9 example_10::createVertexBuffer(void)
{
	LPDIRECT3DVERTEXBUFFER9 result = NULL;

	MN_DEV->CreateVertexBuffer(sizeof(vertex) * 3,
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL_MANAGED,
		&result,
		NULL);

	vertex* v = NULL;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&v, 0)))
	{
		v[0].pos = D3DXVECTOR3(1, -1, 0);
		v[1].pos = D3DXVECTOR3(-1, -1, 0);
		v[2].pos = D3DXVECTOR3(0, 1, 0);

		v[0].diffuse = D3DCOLOR_XRGB(255, 0, 0);
		v[1].diffuse = D3DCOLOR_XRGB(255, 0, 0);
		v[2].diffuse = D3DCOLOR_XRGB(255, 0, 0);
	}
	return result;
}

LPDIRECT3DVERTEXSHADER9 example_10::createVertexShader(LPD3DXCONSTANTTABLE *outTable)
{
	LPDIRECT3DVERTEXSHADER9 result = NULL;

	// DX9의 D3DXBUFFER는 데이터의 타입과 상관없이 어떠한 종류의 데이터도 보관 가능한 공용 버퍼를 의미
	// 데이터 제어를 위해선 적절한 데이터 타입으로 형변환이 필요
	LPD3DXBUFFER bufCompile = NULL;
	LPD3DXBUFFER bufError = NULL;

	// 셰이더 코드 컴파일
	D3DXCompileShaderFromFile(
		TEXT("resource/shader/vertexShader.vsh"),
		NULL,
		NULL,
		"vsMain",			// 진입 함수 명
		"vs_3_0",			// 컴파일 버전 명시
		D3DXSHADER_DEBUG,	// 에러 코드를 받기 위한 디버그 옵션
		&bufCompile,		// 셰이더 코드가 컴파일 된 이진 코드를 담을 버퍼
		&bufError,			// 컴파일 에러가 발생했을 경우 에러 메세지를 담을 버퍼
		outTable);			// 해당 셰이더와 데이터를 공유할 상수버퍼

	if (bufError != NULL)	// 컴파일 에러 발생
	{
		auto errorMessage = (const char*)bufError->GetBufferPointer();
		printf("Example_10.createVertexShader : %s\n", errorMessage);
		
		SAFE_RELEASE(bufError);
	}

	MN_DEV->CreateVertexShader((DWORD*)bufCompile->GetBufferPointer(), &result);

	SAFE_RELEASE(bufCompile);

	return result;
}

LPDIRECT3DPIXELSHADER9 example_10::createPixelShader(void)
{
	LPDIRECT3DPIXELSHADER9 result = NULL;

	LPD3DXBUFFER bufCompile = NULL;
	LPD3DXBUFFER bufError = NULL;

	D3DXCompileShaderFromFile(
		TEXT("resource/shader/pixelShader.psh"),
		NULL,
		NULL,
		"psMain",			// 진입 함수 명
		"ps_3_0",			// 컴파일 버전 명시
		D3DXSHADER_DEBUG,	// 에러 코드를 받기 위한 디버그 옵션
		&bufCompile,		// 셰이더 코드가 컴파일 된 이진 코드를 담을 버퍼
		&bufError,			// 컴파일 에러가 발생했을 경우 에러 메세지를 담을 버퍼
		NULL);				// 해당 셰이더와 데이터를 공유할 상수버퍼

	if (bufError != NULL)	// 컴파일 에러 발생
	{
		auto errorMessage = (const char*)bufError->GetBufferPointer();
		printf("Example_10.createPixelShader : %s\n", errorMessage);

		SAFE_RELEASE(bufError);
	}

	MN_DEV->CreatePixelShader((DWORD*)bufCompile->GetBufferPointer(), &result);

	SAFE_RELEASE(bufCompile);

	return result;
}