#include "example_07.h"
#include "managerList.h"

#include "baseObject.h"
#include "camera.h"

example_07::example_07(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_07::~example_07()
{
	SAFE_RELEASE(_vertexBuffer);

	SAFE_DELETE(_cube);
	SAFE_DELETE(_board);
}


void example_07::init(void)
{
	direct3dApplication::init();
	_vertexBuffer = createVertexBuffer();

	_cube = new baseObject; _cube->setPosition(D3DXVECTOR3(10, 0, 5));
	_board = new baseObject; _board->setScale(D3DXVECTOR3(2.0f, 2.0f, 0.1f)); _board->setPosition(D3DXVECTOR3(-2, 0, 3));
}

void example_07::update(void)
{
	direct3dApplication::update();

	_cube->update();

	// 빌보드
	static bool bill;
	if (MN_KEY->keyPress(DIK_F1)) bill = !bill;

	_board->rotateBillboard(bill);
	_board->update();
}

void example_07::draw(void)
{
	drawCube(_cube->getMatrixWorld());
	drawCube(_board->getMatrixWorld());
}

void example_07::drawCube(const D3DXMATRIXA16 & mWorld)
{
	MN_DEV->SetTransform(D3DTS_WORLD, &mWorld);

	MN_DEV->SetRenderState(D3DRS_LIGHTING, false);
	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_CCW);
	MN_DEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	MN_DEV->SetStreamSource(0, _vertexBuffer, 0, sizeof(vertex));
	MN_DEV->SetFVF(vertex::FVF);
	MN_DEV->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 12);

	MN_DEV->SetRenderState(D3DRS_LIGHTING, true);
	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_NONE);
	MN_DEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	MN_DEV->SetStreamSource(0, _vertexBuffer, 0, sizeof(vertex));
	MN_DEV->SetFVF(vertex::FVF);
	MN_DEV->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 12);

	// 복구
	MN_DEV->SetRenderState(D3DRS_LIGHTING, true);
	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_CCW);
	MN_DEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

LPDIRECT3DVERTEXBUFFER9 example_07::createVertexBuffer(void)
{
	IDirect3DVertexBuffer9* result = NULL;
	
	MN_DEV->CreateVertexBuffer(sizeof(vertex) * 36,
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL_MANAGED,
		&result,
		NULL);

	vertex* v = NULL;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&v, 0)))
	{
		D3DXVECTOR3 vertices[] =
		{
			// 왼쪽
			D3DXVECTOR3(-1, -1, -1),
			D3DXVECTOR3(-1,  1, -1),
			D3DXVECTOR3(1,  1, -1),

			// 오른쪽
			D3DXVECTOR3(-1, -1, -1),
			D3DXVECTOR3(1,  1, -1),
			D3DXVECTOR3(1, -1, -1),
		};

		for (int i = 0; i < 6; ++i)
		{
			D3DXVECTOR3 cVertex[6];

			// 회전 행렬 설정
			D3DXMATRIXA16 mRotation;
			if (i < 4)	D3DXMatrixRotationY(&mRotation, D3DXToRadian(i * 90.f));
			else		D3DXMatrixRotationX(&mRotation, D3DXToRadian(90.f + ((i - 4) * 180.f)));

			D3DXVec3TransformCoordArray(cVertex,
				sizeof(D3DXVECTOR3),
				vertices,
				sizeof(D3DXVECTOR3),
				&mRotation,
				sizeof(vertices) / sizeof(D3DXVECTOR3));

			// 정점 정보 설정
			int index = i * sizeof(cVertex) / sizeof(D3DXVECTOR3);

			for (int j = 0; j < 6; ++j)
			{
				v[index + j].pos = cVertex[j];
				v[index + j].color = D3DCOLOR_XRGB(255, 0, 0);
			}
		}

		result->Unlock();
	}

	return result;
}