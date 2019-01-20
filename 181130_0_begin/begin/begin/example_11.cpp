#include "example_11.h"
#include "managerList.h"
#include "camera.h"

example_11::example_11(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_11::~example_11()
{
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_indexBuffer);
	SAFE_RELEASE(_effect);
}

void example_11::init(void)
{
	direct3dApplication::init();

	_vertexBuffer = createVertexBuffer();
	_indexBuffer = createIndexBuffer();
	_effect = createEffect();
}

void example_11::update(void)
{
	direct3dApplication::update();
	_yaw += 90.0f * MN_TIME->getDeltaTime();
}

void example_11::draw(void)
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

	// ���
	MN_DEV->SetStreamSource(0, _vertexBuffer, 0, sizeof(vertex));
	MN_DEV->SetIndices(_indexBuffer);
	MN_DEV->SetFVF(vertex::FVF);

	// ����Ʈ
	_effect->SetTechnique("myTechnique");
	
	// ����Ʈ : ��ũ�� ���� (������ ����)
	UINT numPasses = 0;
	_effect->Begin(&numPasses, 0);

	for (int i = 0; i < numPasses; ++i)
	{
		// ��ũ�п� �ش��ϴ� pass�� �ܰ踦 ����
		_effect->BeginPass(i);

		/*
			BeginPass ȣ�� ��, effect��ü�� ������ ���� ������ �׷���ī��� �Ѿ
			���� ȣ�� �� effect��ü�� ���� �����ϸ� ���氪�� ������� ����
			-> CommitChanges �Լ��� ���� ��������� ������ �� ����
		*/

		MN_DEV->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 5, 0, 4);

		_effect->EndPass();
	}

	_effect->End();
}

LPDIRECT3DVERTEXBUFFER9 example_11::createVertexBuffer(void)
{
	LPDIRECT3DVERTEXBUFFER9 result = NULL;

	MN_DEV->CreateVertexBuffer(
		sizeof(vertex) * 5,
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL_MANAGED,
		&result,
		NULL);

	vertex* v;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&v, 0)))
	{
		v[0].pos = D3DXVECTOR3(-1, -1, -1);
		v[1].pos = D3DXVECTOR3(0, 1, 0);
		v[2].pos = D3DXVECTOR3(1, -1, -1);
		v[3].pos = D3DXVECTOR3(-1, -1, 1);
		v[4].pos = D3DXVECTOR3(1, -1, 1);

		float nDis = 0.707f;
		v[0].normal = D3DXVECTOR3(-nDis, nDis, -nDis);
		v[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		v[2].normal = D3DXVECTOR3(nDis, nDis, -nDis);
		v[3].normal = D3DXVECTOR3(-nDis, nDis, nDis);
		v[4].normal = D3DXVECTOR3(nDis, nDis, nDis);

		result->Unlock();
	}

	return result;
}

LPDIRECT3DINDEXBUFFER9 example_11::createIndexBuffer(void)
{
	LPDIRECT3DINDEXBUFFER9 result = NULL;

	MN_DEV->CreateIndexBuffer(
		sizeof(WORD) * 12,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&result,
		NULL);

	WORD* i;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&i, 0)))
	{
		int index = -1;
		i[++index] = 0;	i[++index] = 1;	i[++index] = 2;
		i[++index] = 4;	i[++index] = 1;	i[++index] = 3;
		i[++index] = 3;	i[++index] = 1;	i[++index] = 0;
		i[++index] = 2;	i[++index] = 1;	i[++index] = 4;

		result->Unlock();
	}

	return result;
}

LPD3DXEFFECT example_11::createEffect(void)
{
	LPD3DXEFFECT result = NULL;

	LPD3DXBUFFER bufError = NULL;

	D3DXCreateEffectFromFile(
		MN_DEV,
		TEXT("resource/effect/example_11.fx"),
		NULL,
		NULL,
		D3DXSHADER_DEBUG,
		NULL,
		&result,
		&bufError);

	if (bufError != nullptr)
	{
		auto msgError = (char*)bufError->GetBufferPointer();
		printf("Example_11.createEffect : %s\n", msgError);
	}

	return result;
}
