#include "example_06.h"
#include "managerList.h"
#include "baseObject.h"
#include "camera.h"

example_06::example_06(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_06::~example_06()
{
	SAFE_RELEASE(_vertexBuffer);
	SAFE_DELETE(_objRoot);
	SAFE_DELETE(_camera);
	
	for (auto cube : _objCubes)
		SAFE_DELETE(cube);
}


void example_06::init(void)
{
	direct3dApplication::init();
	
	_vertexBuffer = createVertexBuffer();

	D3DXMATRIXA16 mOffset;
	
	// cube man
	_objRoot = new baseObject; _objRoot->setPosition(D3DXVECTOR3(0, 3, 0));
	_objBody = new baseObject; _objRoot->addChild(_objBody); _objBody->setScale(D3DXVECTOR3(1.0f, 1.25f, 0.75f));
	_objHead = new baseObject; _objRoot->addChild(_objHead); _objHead->setScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f)); _objHead->setPosition(D3DXVECTOR3(0, 1.75f, 0));

	D3DXMatrixTranslation(&mOffset, 0, -0.875f, 0);
	_objLArm = new baseObject; _objRoot->addChild(_objLArm); _objLArm->setScale(D3DXVECTOR3(0.375f, 1.25f, 0.375f)); _objLArm->setPosition(D3DXVECTOR3(-1.3125f, 1.5f, 0)); _objLArm->setMatrixOffset(mOffset);

	D3DXMatrixTranslation(&mOffset, 0, -0.875f, 0);
	_objRArm = new baseObject; _objRoot->addChild(_objRArm); _objRArm->setScale(D3DXVECTOR3(0.375f, 1.25f, 0.375f)); _objRArm->setPosition(D3DXVECTOR3(1.3125f, 1.5f, 0)); _objRArm->setMatrixOffset(mOffset); _objRArm->rotateX(180.f);

	D3DXMatrixTranslation(&mOffset, 0, 0.875f, 0);
	_objLLeg = new baseObject; _objRoot->addChild(_objLLeg); _objLLeg->setScale(D3DXVECTOR3(0.375f, -1.5f, 0.375f)); _objLLeg->setPosition(D3DXVECTOR3(-0.5f, -1.5f, 0)); _objLLeg->setMatrixOffset(mOffset);

	D3DXMatrixTranslation(&mOffset, 0, 0.875f, 0);
	_objRLeg = new baseObject; _objRoot->addChild(_objRLeg); _objRLeg->setScale(D3DXVECTOR3(0.375f, -1.5f, 0.375f)); _objRLeg->setPosition(D3DXVECTOR3(0.5f, -1.5f, 0)); _objRLeg->setMatrixOffset(mOffset);

	// cubes
	_objCubes[0] = new baseObject; _objCubes[0]->setScale(D3DXVECTOR3(1.3f, 3.0f, 1.2f)); _objCubes[0]->setPosition(D3DXVECTOR3(3, 3.0f, 9));
	_objCubes[1] = new baseObject; _objCubes[1]->setScale(D3DXVECTOR3(1.2f, 6.0f, 1.2f)); _objCubes[1]->setPosition(D3DXVECTOR3(19, 6.0f, -12));
	_objCubes[2] = new baseObject; _objCubes[2]->setScale(D3DXVECTOR3(1.3f, 9.0f, 1.2f)); _objCubes[2]->setPosition(D3DXVECTOR3(-38, 9.0f, 51));
	_objCubes[3] = new baseObject; _objCubes[3]->setScale(D3DXVECTOR3(1.3f, 12.0f, 1.2f)); _objCubes[3]->setPosition(D3DXVECTOR3(-42, 12.0f, -24));
	_objCubes[4] = new baseObject; _objCubes[4]->setScale(D3DXVECTOR3(1.2f, 15.0f, 1.2f)); _objCubes[4]->setPosition(D3DXVECTOR3(-24, 15.0f, -45));

	// camera
	_camera = new camera(MN_WIN->getAspect()); _camera->setState(camera::STATE_SWITCH::HORIZONTAL, true);
}

void example_06::update(void)
{
	_objRoot->update();
	updateControl();

	_pitch += _pitchVel * MN_TIME->getDeltaTime();
	_pitch = min(_pitch, 30.f);
	_pitch = max(_pitch, -30.f);

	if (_pitch <= -30.0f || 30.0f <= _pitch)
		_pitchVel = -_pitchVel;

	_objLArm->setRotation(D3DXVECTOR3(-_pitch, 0, 0));
	_objRArm->setRotation(D3DXVECTOR3(_pitch, 0, 0));

	_objLLeg->setRotation(D3DXVECTOR3(_pitch, 0, 0));
	_objRLeg->setRotation(D3DXVECTOR3(-_pitch, 0, 0));

	// view
	// static D3DXVECTOR3 vPos = D3DXVECTOR3(0, 10, -20);
	// static D3DXVECTOR3 vAt = D3DXVECTOR3(0, 0, 0);
	// static D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);
	// static D3DXMATRIX mView;
	// D3DXMatrixLookAtLH(&mView, &vPos, &vAt, &vUp);
	_camera->setState(camera::STATE_SWITCH::HORIZONTAL, _horizontal);
	_camera->setPosition(_objRoot->getPosition());
	_camera->update(_objRoot);
	MN_DEV->SetTransform(D3DTS_VIEW, _camera->getMatrixWorldPoint());

	// projection
	static D3DXMATRIX mProjection;
	D3DXMatrixPerspectiveFovLH(&mProjection, D3DXToRadian(50.f), GET_WINDOW_MANAGER()->getAspect(), 0.1f, 1000.0f);
	MN_DEV->SetTransform(D3DTS_PROJECTION, &mProjection);

	// cubes
	for (auto c : _objCubes) c->update();
}

void example_06::draw(void)
{
	drawCube(_objBody->getMatrixFinal());
	drawCube(_objHead->getMatrixFinal());
	drawCube(_objLArm->getMatrixFinal());
	drawCube(_objRArm->getMatrixFinal());
	drawCube(_objLLeg->getMatrixFinal());
	drawCube(_objRLeg->getMatrixFinal());

	for (auto c : _objCubes) drawCube(c->getMatrixFinal());
}

void example_06::updateControl(void)
{
	if (MN_KEY->keyDown(DIK_W)) _objRoot->moveZ(30.0f * MN_TIME->getDeltaTime());
	if (MN_KEY->keyDown(DIK_S)) _objRoot->moveZ(-30.0f * MN_TIME->getDeltaTime());

	if (MN_KEY->keyDown(DIK_A)) _objRoot->rotateY(-120.0f * MN_TIME->getDeltaTime());
	if (MN_KEY->keyDown(DIK_D)) _objRoot->rotateY(120.0f * MN_TIME->getDeltaTime());

	if (MN_KEY->keyDown(DIK_Q)) _objRoot->rotateZ(90.0f * MN_TIME->getDeltaTime());
	if (MN_KEY->keyDown(DIK_E)) _objRoot->rotateZ(-90.0f * MN_TIME->getDeltaTime());

	if (MN_KEY->keyDown(DIK_UP)) _objRoot->rotateX(90.0f * MN_TIME->getDeltaTime());
	if (MN_KEY->keyDown(DIK_DOWN)) _objRoot->rotateX(-90.0f * MN_TIME->getDeltaTime());

	if (MN_KEY->keyPress(DIK_F1)) _horizontal = !_horizontal;
	if (MN_KEY->keyPress(DIK_F2)) _thirdP = !_thirdP;
}

void example_06::drawCube(const D3DXMATRIXA16 & mWorld)
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

IDirect3DVertexBuffer9* example_06::createVertexBuffer(void)
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
			D3DXVECTOR3( 1,  1, -1),
			
			// 오른쪽
			D3DXVECTOR3(-1, -1, -1),
			D3DXVECTOR3( 1,  1, -1),
			D3DXVECTOR3( 1, -1, -1),
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