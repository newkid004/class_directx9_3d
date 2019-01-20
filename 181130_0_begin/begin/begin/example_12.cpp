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

	// ���
	D3DXMATRIXA16 mWorld;
	D3DXMatrixRotationY(&mWorld, D3DXToRadian(_yaw));

	_effect->SetMatrix("_mWorld", &mWorld);
	_effect->SetMatrix("_mView", GET_CAMERA()->getMatrixViewPoint());
	_effect->SetMatrix("_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	// ����
	D3DXVECTOR4 lightDirection(0.0f, 0.0f, 1.0f, 0.0f);
	_effect->SetVector("_lightDirection", &lightDirection);

	// ī�޶�
	D3DXVECTOR4 viewPosition(GET_CAMERA()->getPosition(), 1.0f);
	_effect->SetVector("_viewPosition", &viewPosition);

	// �ؽ���
	_effect->SetTexture("_texture", _texture);

	// ����Ʈ
	_effect->SetTechnique("myTechnique");

	UINT numPasses = 0;
	_effect->Begin(&numPasses, 0);

	for (int i = 0; i < numPasses; ++i)
	{
		_effect->BeginPass(i);

		/*
			�޽ð� N�� �̻��� ��������� �����Ǿ��� ���
			�ش� ������ŭ DrawSubset�Լ��� ȣ���ؾ� ���ϴ� ����� ��� ����
			�� �̿ܿ� 0
		*/
		_mesh->DrawSubset(0);

		_effect->EndPass();
	}

	_effect->End();
	// �޽�

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
	// mesh�� ������Ƽ�� Ÿ���� �ﰢ������ ����
	// �޽ø� ���� ����, �� ���� ��ü�� �����ϴ� ���� ��ȿ����
	D3DXCreateMeshFVF(
		sizeof(WORD) * 2,	// �ﰢ���� ����(�ȵ��� ����), �⺻������ 2 byte
		sizeof(vertex) * 4,	// ��������
		D3DXMESH_MANAGED,	//
		vertex::FVF, 		//
		MN_DEV,				//
		&result);			//
#else
	/*
		WORD    Stream;     // ��Ʈ�� ��ȣ
		WORD    Offset;     // �޸��� ù ��ġ���� �� ����Ʈ��ŭ ������ �ִ����� ����
		BYTE    Type;       // �ڷ��� Ÿ��
		BYTE    Method;     // �׷��� ī���� �׼����̼ǿ� ���� ����
		BYTE    Usage;      // �ø�ƽ
		BYTE    UsageIndex; // �ø�ƽ ��ȣ(�ߺ��� ���)

		D3DVERTEXELEMENT9 Ư¡
		- �ߺ� �ø�ƽ ���� ���� (FVF�� �Ұ�)
		- FVF�� �������� �ʴ� �ø�ƽ ���� ���� (Ex. TANGENT(����), BINORMAL(������), ...)
		- FVF�� �޸� ����ü �ɹ��� ���� ������ ������� ����
	*/
	D3DVERTEXELEMENT9 verElement[] = {
		/* ��ġ ���� ���� */	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		/* ���� ���� ���� */	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		/* UV ���� ���� */		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		/* ���� ��*/			D3DDECL_END()
	};

	D3DXCreateMesh(
		sizeof(WORD) * 2,	
		sizeof(vertex) * 4,	
		D3DXMESH_MANAGED,	
		verElement,
		MN_DEV,				
		&result);			

	// D3DXComputeNormals : ���� ���� ���	

#endif	// VERTEX_FORMAT_TYPE == VERTEX_FORMAT_TYPE_DECLARATION


	// ���� ����
	vertex* v = NULL;

	// �޽ô� �Ϲ����� ����/�ε��� ���ۿʹ� �޸� ������ �Ϻκи� ������ ���� �Ұ���
	// �Ϻκ� ���� ��, ���� ���� �ʿ�
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

	// index ����
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