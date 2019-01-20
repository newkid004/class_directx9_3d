#include "practice_05.h"
#include "managerList.h"
#include "camera.h"

practice_05::practice_05(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

practice_05::~practice_05()
{
	SAFE_RELEASE(_vertexBuffer);

	SAFE_RELEASE(_vertexShader);
	SAFE_RELEASE(_constantTable);

	SAFE_RELEASE(_pixelShader);
	SAFE_RELEASE(_pixelTable);

	SAFE_RELEASE(_texture);
}


void practice_05::init(void)
{
	direct3dApplication::init();

	_vertexBuffer = createVertexBuffer();
	_pixelShader = createPixelShader(&_pixelTable);
	_vertexShader = createVertexShader(&_constantTable);
	_texture = createTexture();
}

void practice_05::update(void)
{
	direct3dApplication::update();
	_yaw += 90.0f * MN_TIME->getDeltaTime();
}

void practice_05::draw(void)
{
	direct3dApplication::draw();

	MN_DEV->SetRenderState(D3DRS_LIGHTING, false);
	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ---------------------------- //

	// ����
	D3DXMATRIXA16 mWorld;
	D3DXMatrixRotationY(&mWorld, D3DXToRadian(_yaw));

	// ���̴� ���
	MN_DEV->SetVertexShader(_vertexShader);
	MN_DEV->SetPixelShader(_pixelShader);

	_constantTable->SetMatrix(MN_DEV, "_mWorld", &mWorld);
	_constantTable->SetMatrix(MN_DEV, "_mView", GET_CAMERA()->getMatrixViewPoint());
	_constantTable->SetMatrix(MN_DEV, "_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	MN_DEV->SetTexture(0, _texture);

	MN_DEV->SetStreamSource(0, _vertexBuffer, 0, sizeof(vertex));
	MN_DEV->SetFVF(vertex::FVF);

	MN_DEV->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	// ---------------------------- //
	MN_DEV->SetRenderState(D3DRS_LIGHTING, true);
	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

LPDIRECT3DVERTEXBUFFER9 practice_05::createVertexBuffer(void)
{
	LPDIRECT3DVERTEXBUFFER9 result = NULL;

	MN_DEV->CreateVertexBuffer(sizeof(vertex) * 6,
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL_MANAGED,
		&result,
		NULL);

	vertex* v = NULL;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&v, 0)))
	{
		v[0].pos = D3DXVECTOR3(-1, -1, 0);
		v[1].pos = D3DXVECTOR3(-1, 1, 0);
		v[2].pos = D3DXVECTOR3(1, 1, 0);
		v[3].pos = D3DXVECTOR3(1, 1, 0);
		v[4].pos = D3DXVECTOR3(1, -1, 0);
		v[5].pos = D3DXVECTOR3(-1, -1, 0);

		v[0].UV = D3DXVECTOR2(0, 1);
		v[1].UV = D3DXVECTOR2(0, 0);
		v[2].UV = D3DXVECTOR2(1, 0);
		v[3].UV = D3DXVECTOR2(1, 0);
		v[4].UV = D3DXVECTOR2(1, 1);
		v[5].UV = D3DXVECTOR2(0, 1);
	}
	return result;
}

LPDIRECT3DTEXTURE9 practice_05::createTexture(void)
{
	LPDIRECT3DTEXTURE9 result = NULL;

	D3DXCreateTextureFromFile(MN_DEV,
		TEXT("resource/texture/practice_05.png"),
		&result);

	return result;
}

LPDIRECT3DPIXELSHADER9 practice_05::createPixelShader(LPD3DXCONSTANTTABLE *outTable)
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
		outTable);			// �ش� ���̴��� �����͸� ������ �������

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

LPDIRECT3DVERTEXSHADER9 practice_05::createVertexShader(LPD3DXCONSTANTTABLE *outTable)
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