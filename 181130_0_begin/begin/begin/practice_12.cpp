#include "practice_12.h"

#include "managerList.h"
#include "cameraControlable.h"
#include "gFunc.h"

#include "debugGrid.h"
#include "lightBase.h"

#include "terrain.h"
#include "skinnedMesh.h"
#include "animationController.h"

#include "pickRay.h"

practice_12::practice_12(HINSTANCE hInstance, const SIZE & windowSize, int option) :
	direct3dApplication(hInstance, windowSize, option)
{
}

practice_12::~practice_12()
{
	SAFE_DELETE(_terrain);
	SAFE_DELETE(_character);
}

void practice_12::init(void)
{
	direct3dApplication::init();

	_c = (cameraControlable*)_camera;
	_c->setVelocity(_c->getVelocity() * 3.0f);

	_terrain = createTerrian();
	_character = createCharacter();
	_character->setScale(0.1f);
}

void practice_12::update(void)
{
	direct3dApplication::update();

	_terrain->update();
	_character->update();

	updateControl();
	updateCharacter();
}

void practice_12::draw(void)
{
	direct3dApplication::draw();
	_terrain->draw();
	_character->draw();
}

void practice_12::updateControl(void)
{
	// ----- 키 이동 ----- //
	_charMovement = 0;

	if (MN_KEY->keyDown(DIK_W)) _charMovement |= 0b1000;
	if (MN_KEY->keyDown(DIK_S)) _charMovement |= 0b0100;
	if (MN_KEY->keyDown(DIK_A)) _charMovement |= 0b0010;
	if (MN_KEY->keyDown(DIK_D)) _charMovement |= 0b0001;
	
	//D3DXIntersectSubset()

	// 캐릭터 방향 조절
	if (_charMovement)
	{
		// 전방 고정
		_character->rotateBillboard(true, true);
		_character->rotateY(-90.0f, true);

		// 클릭이동 취소
		_move2click = false;
	}

	switch (_charMovement)
	{
	case 0b1000: break;
	case 0b0100: _character->rotateY(180.0f, true); break;
	case 0b0010: _character->rotateY(-90.0f, true); break;
	case 0b0001: _character->rotateY(90.0f, true); break;

	case 0b1010: _character->rotateY(-45.0f, true); break;
	case 0b1001: _character->rotateY(45.0f, true); break;
	case 0b0110: _character->rotateY(-135.0f, true); break;
	case 0b0101: _character->rotateY(135.0f, true); break;
	}

	// ----- 클릭 이동 ----- //
	if (!MN_KEY->mousePress(EMouseInput::LEFT))
		return;

	// 픽킹 확인
	pick::ray pickRay;
	pick::info pickInfo;

	pick::createPickRay(&pickRay);
	pick::chkPick(&pickInfo, &pickRay, _terrain->getMesh());
	if (!pickInfo.isHit) 
		return;
	
	// 픽킹 되었을 경우
	_move2click = true;

	// ----- 과제 ----- //
	/*/
	LPDIRECT3DINDEXBUFFER9 indexBuffer = nullptr;
	DWORD* index;
	DWORD pickedIndex[3];
	pickInfo.mesh->GetIndexBuffer(&indexBuffer);
	if (SUCCEEDED(indexBuffer->Lock(0, 0, (void**)&index, 0)))
	{
		pickedIndex[0] = index[3 * pickInfo.faceIndex + 0];
		pickedIndex[1] = index[3 * pickInfo.faceIndex + 1];
		pickedIndex[2] = index[3 * pickInfo.faceIndex + 2];

		indexBuffer->Unlock();
	}

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;
	terrain::vertex* v;
	terrain::vertex pickedVertex[3];
	pickInfo.mesh->GetVertexBuffer(&vertexBuffer);
	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&v, 0)))
	{
		pickedVertex[0] = v[pickedIndex[0]];
		pickedVertex[1] = v[pickedIndex[1]];
		pickedVertex[2] = v[pickedIndex[2]];

		vertexBuffer->Unlock();
	}
	_movePos.x = (pickedVertex[1].position.x - pickedVertex[0].position.x) * pickInfo.uv.x + pickedVertex[0].position.x;
	_movePos.y = (pickedVertex[2].position.z - pickedVertex[0].position.z) * pickInfo.uv.y + pickedVertex[0].position.z;

	// ----- 수업 ----- //
	/*/
	D3DXVECTOR3 pickPos = pickRay.origin + pickRay.direction * pickInfo.distance;
	_movePos.x = pickPos.x;
	_movePos.y = pickPos.z;

	//*/

	// 캐릭터 회전
	D3DXVECTOR3 charDirection = D3DXVECTOR3(_movePos.x, _c->getPosition().y, _movePos.y);

	D3DXVECTOR3 dir = charDirection - _c->getPosition();
	D3DXVec3Normalize(&dir, &dir);

	_character->rotate2Dir(dir, true, true);
	_character->rotateY(-90.0f, true);

	// 이동 보정
	_moveDir.x = _movePos.x - _c->getPosition().x;
	_moveDir.y = _movePos.y - _c->getPosition().z;
}

void practice_12::updateCharacter(void)
{
	// 지형 걷기
	D3DXVECTOR3 pos = GET_CAMERA()->getPosition();
	pos.y = _terrain->getHeight(D3DXVECTOR2(pos.x, pos.z));
	pos.y += 3.0f;
	GET_CAMERA()->setPosition(pos);
	pos.y -= 3.0f;
	_character->setPosition(pos);

	// 모션 조절
	auto ani = _character->getAniController();
	auto aniList = ani->getAnimationNameList();
	
	if (_charMovement == 0 && !_move2click)
		ani->play(aniList[0], true);
	else
		ani->play(aniList[2], true);

	if (_move2click)
	{
		D3DXVECTOR2 charDirection = D3DXVECTOR2(_moveDir.x, _moveDir.y);
		D3DXVec2Normalize(&charDirection, &charDirection);
		charDirection *= _c->getVelocity() * MN_TIME->getDeltaTime();

		D3DXVECTOR2 distance = D3DXVECTOR2(_movePos.x, _movePos.y) - D3DXVECTOR2(_c->getPosition().x, _c->getPosition().z);
		FLOAT moveDistance = D3DXVec2Length(&charDirection);
		FLOAT totalDistance = D3DXVec2Length(&distance);

		if (totalDistance <= moveDistance)
		{
			_move2click = false;
			_c->setPosition(D3DXVECTOR3(_movePos.x, _c->getPosition().y, _movePos.y));
		}
		else
			_c->setPosition(_c->getPosition() + D3DXVECTOR3(charDirection.x, 0.0f, charDirection.y));
	}
}

terrain * practice_12::createTerrian(void)
{
	terrain::params param = {
		30,
		0.2f,
		"resource/texture/terrain/SplatMap.png",
		"resource/texture/terrain/HeightMap.bmp",
		"resource/effect/example_17.fx"
	};
	param.tileSize.cx = 1;
	param.tileSize.cy = 1;

	for (int i = 0; i < terrain::MAX_NUM_TEXTURES; ++i)
	{
		char filePath[MAX_PATH] = "";
		sprintf(filePath, "resource/texture/terrain/Terrain_%02d.jpg", i + 1);

		param.vPathTextureFile.push_back(filePath);
	}

	return new terrain(param);
}

skinnedMesh * practice_12::createCharacter(void)
{
	skinnedMesh::mParam param = {
		"resource/mesh/WitchApprentice/WitchApprentice.x",
		"resource/effect/skinnedMesh.fx"
	};

	return new skinnedMesh(param);
}
