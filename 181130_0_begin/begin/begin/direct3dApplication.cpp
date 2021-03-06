#include "kGlobalDefine.h"
#include "direct3dApplication.h"
#include "managerList.h"

#include "cameraControlable.h"
#include "debugGrid.h"
#include "debugGizmo.h"
#include "lightBase.h"
#include "labelBase.h"
#include "spriteBase.h"
#include "skyBox.h"

direct3dApplication::~direct3dApplication()
{
	SAFE_DELETE(_camera);
	SAFE_DELETE(_grid);
	SAFE_DELETE(_light);
	SAFE_DELETE(_label);

	SAFE_RELEASE(_sprite);
	SAFE_DELETE(_skyBox);
}

void direct3dApplication::init(void)
{
	iWindowApplication::init();
	GET_DEVICE_MANAGER()->init();
	GET_SOUND_MANAGER()->init();

	_camera = new cameraControlable(MN_WIN->getAspect(), 60.0f, 1000.0f);
	_camera->setPosition(D3DXVECTOR3(0, 0, -5));

	_grid = new debugGrid(50);
	_grid->setPosition(D3DXVECTOR3(0, 0, 0));
	_grid->addChild(new debugGizmo(12.0f));

	_light = new lightBase(0);
	_light->setPosition(D3DXVECTOR3(0.0f, 0.0f, -5.0f));
	_light->setRotation(D3DXVECTOR3(45.0f, 0.0f, 0.0f));
	_light->setLightable(true);

	_label = new labelBase("", 16.0f);
	_label->setPosition(D3DXVECTOR3(1, 1, 0));

	_sprite = createSprite();

	_skyBox = createSkyBox();
}

void direct3dApplication::update(void)
{
	_camera->update();
	_grid->update();
	_light->update();
	_label->update();
	if (_skyBox) 
		_skyBox->update();

	if (MN_KEY->keyPress(DIK_F1)) 
		_grid->ToggleVisible();
}
void direct3dApplication::draw(void)
{
	if (_skyBox) 
		_skyBox->draw();

	_camera->draw();
	_grid->draw();
}

void direct3dApplication::drawUI(void)
{
	char text[MAX_PATH] = "";

	sprintf(text, "fps : %d\ndel : %.4f sec\nrun : %.4f sec",
		MN_TIME->getFPS(),
		MN_TIME->getDeltaTime(),
		MN_TIME->getRunningTime());

	_label->setPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	_label->setText(text);
	_label->drawUI();
}

LRESULT direct3dApplication::handleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return iWindowApplication::handleWindowMessage(hWnd, message, wParam, lParam);
}

int direct3dApplication::msgLoop(void)
{
	// 관리자 초기화
	GET_TIME_MANAGER()->init();
	GET_INPUT_MANAGER()->init();

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 관리자 갱신
		GET_TIME_MANAGER()->update();
		GET_INPUT_MANAGER()->update();

		// 버퍼 클리어
		GET_DEVICE_MANAGER()->getDevice()->Clear(
			0, NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
			BACK_COLOR(255),
			1.0f, 0);

		if (SUCCEEDED(GET_DEVICE_MANAGER()->getDevice()->BeginScene()))
		{
			update();
			draw();

			_sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT);
			{
				drawUI();
			}
			_sprite->End();
			
			GET_DEVICE_MANAGER()->getDevice()->EndScene();
		}

		// 버퍼 교체
		GET_DEVICE_MANAGER()->getDevice()->Present(0, 0, 0, 0);
	}

	return (int)msg.wParam;
}

LPD3DXSPRITE direct3dApplication::createSprite(void)
{
	LPD3DXSPRITE result = nullptr;

	D3DXCreateSprite(
		MN_DEV,
		&result);

	return result;
}

skyBox * direct3dApplication::createSkyBox(void)
{
	return nullptr;
}

void direct3dApplication::setWindowSize(const SIZE & input)
{
	iWindowApplication::setWindowSize(input);
	_camera->setAspect(input.cx / (float)input.cy);
}