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

	// ����
	D3DXMATRIXA16 mWorld;
	D3DXMatrixRotationY(&mWorld, D3DXToRadian(_yaw));

	//*/ ���̴� ���
	MN_DEV->SetVertexShader(_vertexShader);
	MN_DEV->SetPixelShader(_pixelShader);

	_constantTable->SetMatrix(MN_DEV, "_mWorld", &mWorld);
	_constantTable->SetMatrix(MN_DEV, "_mView", GET_CAMERA()->getMatrixViewPoint());
	_constantTable->SetMatrix(MN_DEV, "_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	/*/ // ���� ���
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

	// DX9�� D3DXBUFFER�� �������� Ÿ�԰� ������� ��� ������ �����͵� ���� ������ ���� ���۸� �ǹ�
	// ������ ��� ���ؼ� ������ ������ Ÿ������ ����ȯ�� �ʿ�
	LPD3DXBUFFER bufCompile = NULL;
	LPD3DXBUFFER bufError = NULL;

	// ���̴� �ڵ� ������
	D3DXCompileShaderFromFile(
		TEXT("resource/shader/vertexShader.vsh"),
		NULL,
		NULL,
		"vsMain",			// ���� �Լ� ��
		"vs_3_0",			// ������ ���� ���
		D3DXSHADER_DEBUG,	// ���� �ڵ带 �ޱ� ���� ����� �ɼ�
		&bufCompile,		// ���̴� �ڵ尡 ������ �� ���� �ڵ带 ���� ����
		&bufError,			// ������ ������ �߻����� ��� ���� �޼����� ���� ����
		outTable);			// �ش� ���̴��� �����͸� ������ �������

	if (bufError != NULL)	// ������ ���� �߻�
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
		"psMain",			// ���� �Լ� ��
		"ps_3_0",			// ������ ���� ���
		D3DXSHADER_DEBUG,	// ���� �ڵ带 �ޱ� ���� ����� �ɼ�
		&bufCompile,		// ���̴� �ڵ尡 ������ �� ���� �ڵ带 ���� ����
		&bufError,			// ������ ������ �߻����� ��� ���� �޼����� ���� ����
		NULL);				// �ش� ���̴��� �����͸� ������ �������

	if (bufError != NULL)	// ������ ���� �߻�
	{
		auto errorMessage = (const char*)bufError->GetBufferPointer();
		printf("Example_10.createPixelShader : %s\n", errorMessage);

		SAFE_RELEASE(bufError);
	}

	MN_DEV->CreatePixelShader((DWORD*)bufCompile->GetBufferPointer(), &result);

	SAFE_RELEASE(bufCompile);

	return result;
}