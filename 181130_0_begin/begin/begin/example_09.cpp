#include "example_09.h"
#include "managerList.h"

#include "baseObject.h"

example_09::example_09(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_09::~example_09()
{
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_indexBuffer);
	SAFE_DELETE(_objLight);
}

void example_09::init(void)
{
	direct3dApplication::init();

	_vertexBuffer = createVertexBuffer();
	_indexBuffer = createIndexBuffer();

	_objLight = new baseObject;
}

void example_09::update(void)
{
	direct3dApplication::update();

	_yaw += 90.0f * MN_TIME->getDeltaTime();

	updateControl();
}

void example_09::draw(void)
{
	direct3dApplication::draw();

	// �����Ͷ����� �ܰ迡�� ������ ���ԵǾ��ִ� ���� ������ ������ ���ؼ�
	// �ְ�� ���� ������ ���� �ٽ� ����ȭ ��Ű�� �ɼ�
	MN_DEV->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	// ���ݻ� ������ Ȱ��ȭ
	MN_DEV->SetRenderState(D3DRS_SPECULARENABLE, true);

	/*
		DX9�� �����ϴ� ���̵� ��
		1. Flat		: �ܻ� ���̵� ó��
		2. Gouraud	: ���� ���� ���̵� ó��
		3. Phong	: �ȼ� ���� ���̵� ó��
	*/
	// �ȼ� ������ ���� ���� �ɼ� Ȱ��ȭ
	MN_DEV->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_PHONG);

	// world
	D3DXMATRIXA16 mWorld;
	D3DXMatrixRotationY(&mWorld, D3DXToRadian(_yaw));
	MN_DEV->SetTransform(D3DTS_WORLD, &mWorld);

	// light
	/*
		1. ���Ɽ(Direction)
		2. ����(Point)
		3. ���߱�(Spot)

		1. ���Ɽ
		- ���� �ٿ����� ���Ѵ�� �ָ� �ִٰ� �����ϴ� ������ ���� ��
		  3���� ������ �����ϴ� ��� ��ü���� ������ �������� ���� ����

		2. ����
		- ���� �ٿ����� �������� ������� ���������� ���� �𵨷ν� ���� �ٿ����� ��ġ�� ����
		  3���� ���� �� �����ϴ� ��ü���� �޴� ������ ������ �޶���

		3. ���߱�
		- ���ǻ��� ������� ����� ���� �𵨷ν� ���� �ٿ����κ��� �򶧱� ������� ������ ����
	*/
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
	light.Direction = _objLight->getDirectForward();
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	/*
		DX9�� ���������� �ε����� ������ ���� ���� ���� ���� �Ӽ� ���� �� Ȱ��ȭ�� ������
		���� ���� ������ �����ϸ� ������ ���������� ���� ����� ���� ���� ������ ���� �����
		�������� ���� ������ ȥ���ϴ� ���� ����
	*/
	MN_DEV->LightEnable(0, true);
	MN_DEV->SetLight(0, &light);

	// material
	/*
		������ ���� ���� : ������ ���ԵǾ� �ִ� ���� ������ �ݻ� ����
		Ex. light * material
	*/
	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	material.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Power = 50.0f;
	MN_DEV->SetMaterial(&material);

	// draw
	MN_DEV->SetStreamSource(0, _vertexBuffer, 0, sizeof(vertex));
	MN_DEV->SetIndices(_indexBuffer);
	MN_DEV->SetFVF(vertex::FVF);

	MN_DEV->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 5, 0, 4);
}

void example_09::updateControl(void)
{
	_objLight->update();

	if (MN_KEY->keyDown(DIK_LEFT))	_objLight->rotateY(200.0f * MN_TIME->getDeltaTime(), false);
	if (MN_KEY->keyDown(DIK_RIGHT))	_objLight->rotateY(-200.0f * MN_TIME->getDeltaTime(), false);
	if (MN_KEY->keyDown(DIK_UP))	_objLight->rotateX(-200.0f * MN_TIME->getDeltaTime(), false);
	if (MN_KEY->keyDown(DIK_DOWN))	_objLight->rotateX(200.0f * MN_TIME->getDeltaTime(), false);
}

LPDIRECT3DVERTEXBUFFER9 example_09::createVertexBuffer(void)
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
		v[1].pos = D3DXVECTOR3( 0,  1,  0);
		v[2].pos = D3DXVECTOR3( 1, -1, -1);
		v[3].pos = D3DXVECTOR3(-1, -1,  1);
		v[4].pos = D3DXVECTOR3( 1, -1,  1);

		float nDis = 0.707f;
		v[0].normal = D3DXVECTOR3(-nDis, nDis, -nDis);
		v[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		v[2].normal = D3DXVECTOR3( nDis, nDis, -nDis);
		v[3].normal = D3DXVECTOR3(-nDis, nDis,  nDis);
		v[4].normal = D3DXVECTOR3( nDis, nDis,  nDis);

		result->Unlock();
	}

	return result;
}

LPDIRECT3DINDEXBUFFER9 example_09::createIndexBuffer(void)
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
