#include "kGlobalDefine.h"
#include "example_02.h"
#include "windowManager.h"

using namespace std::chrono;

example_02::example_02(HINSTANCE hInstance, const SIZE & windowSize, int option)
	:
	example(hInstance, windowSize, option),
	_viewPos(3.0, 3.0, -10.0)
{
}

example_02::~example_02()
{
	KillTimer(GET_WINDOW_HANDLE(), 0);
}

void example_02::init(void)
{
	iWindowApplication::init();
	SetTimer(GET_WINDOW_HANDLE(), 0, 0, NULL);

	// 현재 시스템 시간
	_prevTime = std::chrono::system_clock::now();

	// 점 위치 초기화
	// _vertices[0] = c3DVector(1, 1, 1);
	// _vertices[1] = c3DVector(1, -1, 1);
	// _vertices[2] = c3DVector(-1, -1, 1);
	// _vertices[3] = c3DVector(-1, 1, 1);

	// 행렬 초기화
	_transM = c3DMatrix::getIdentity();

	// 속성 초기화
	//*/
	_pos[0] = WINSIZEX / 2;
	_pos[1] = WINSIZEY / 2;
	/*/
	_pos[0] = 0;
	_pos[1] = 0;
	//*/

	_scale = 100;
}

LRESULT example_02::handleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_TIMER:		{ this->msgTimer(wParam, lParam); } break;
	case WM_PAINT:		{ this->msgPaint(wParam, lParam); } break;
	case WM_KEYDOWN:	{ this->msgKeyDown(wParam, lParam); } break;
	}

	return iWindowApplication::handleWindowMessage(hWnd, message, wParam, lParam);
}

void example_02::msgTimer(WPARAM wParam, LPARAM lParam)
{
	auto curTime = system_clock::now();
	NUM_REAL ellipseTime = duration_cast<duration<double, std::ratio<1, 360>>>(curTime - _prevTime).count();
	_prevTime = curTime;

	// _transM = c3DMatrix::getRotate(_yaw);

	// 기본 사각형
	static D3DXVECTOR3 sqVector[4] = {
		D3DXVECTOR3( 1,  1, 0),
		D3DXVECTOR3( 1, -1, 0),
		D3DXVECTOR3(-1, -1, 0),
		D3DXVECTOR3(-1,  1, 0)
	};
	
	// 회전 행렬
	_yaw += D3DX_PI * ellipseTime / 1000.0;
	if (D3DX_PI < _yaw) _yaw -= D3DX_PI;

	static D3DXMATRIX mRotation;
	D3DXMatrixRotationYawPitchRoll(&mRotation, _yaw * 2, _yaw * 2, _yaw / 2);

	// 시야 행렬
	static D3DXMATRIX mCamera;
	static D3DXVECTOR3 vLook	= D3DXVECTOR3(0, 0, 0);
	static D3DXVECTOR3 vUp		= D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&mCamera, &_viewPos, &vLook, &vUp);

	// 투영 행렬
	static D3DXMATRIX mProjection;
	D3DXMatrixPerspectiveFovLH(&mProjection,
		D3DXToRadian(30.0f),
		GET_WINDOW_MANAGER()->getWindowSize().cx / (float)GET_WINDOW_MANAGER()->getWindowSize().cy,
		10.0f,
		1000.0f);

	// 뷰포트 행렬
	static D3DXMATRIX mViewPort;
	static D3DVIEWPORT9 vViewPort;
	ZeroMemory(&vViewPort, sizeof(D3DVIEWPORT9));
	vViewPort.Width = GET_WINDOW_MANAGER()->getWindowSize().cx;
	vViewPort.Height = GET_WINDOW_MANAGER()->getWindowSize().cy;
	vViewPort.MinZ = 0.0f;
	vViewPort.MaxZ = 1.0f;

	mViewPort(0, 0) = vViewPort.Width / 2.0f;
	mViewPort(3, 0) = vViewPort.X + vViewPort.Width / 2.0f;

	mViewPort(1, 1) = -(vViewPort.Height / 2.0f);
	mViewPort(3, 1) = vViewPort.Y + vViewPort.Height / 2.0f;

	mViewPort(2, 2) = vViewPort.MaxZ - vViewPort.MinZ;
	mViewPort(3, 2) = vViewPort.MinZ;

	mViewPort(3, 3) = 1;

	static D3DXMATRIX mTotal;
	mTotal = mRotation * mCamera * mProjection * mViewPort;
	D3DXVec3TransformCoordArray(_vertices, sizeof(D3DXVECTOR3), sqVector, sizeof(D3DXVECTOR3), &mTotal, 4);

	InvalidateRect(GET_WINDOW_HANDLE(), NULL, true);
}

void example_02::msgPaint(WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	HDC hdc = BeginPaint(GET_WINDOW_HANDLE(), &ps);

	/*/
	c3DMatrix mScale = c3DMatrix::getScale(_scale);
	c3DMatrix mLocation = c3DMatrix::getTranslate(_pos);

	c3DMatrix mTransform = (_transM * mScale * mLocation);
	c3DVector transVertices[4];
	for (int i = 0; i < 4; ++i)
		transVertices[i] = _vertices[i].getTransform(mTransform);

	MoveToEx(hdc, transVertices[0].x, transVertices[0].y, NULL);
	for (int i = 1; i <= 4; ++i)
	{
		c3DVector & v = transVertices[i % 4];
		LineTo(hdc, v.x, v.y);
	}
	/*/

	MoveToEx(hdc, _vertices[0].x, _vertices[0].y, NULL);
	for (int i = 1; i <= 4; ++i)
	{
		D3DXVECTOR3 & v = _vertices[i % 4];
		LineTo(hdc, v.x, v.y);
	}

	//*/

	EndPaint(GET_WINDOW_HANDLE(), &ps);
}

void example_02::msgKeyDown(WPARAM wParam, LPARAM lParam)
{
	//*/

	switch (wParam)
	{
	case 'W': _viewPos.z += 0.66f; break;
	case 'S': _viewPos.z -= 0.66f; break;
	}

	/*/
	switch (wParam)
	{
	case 'A': _pos[0] -= 3; break;
	case 'D': _pos[0] += 3; break;
	case 'W': _pos[1] -= 3; break;
	case 'S': _pos[1] += 3; break;

	case 'Q': _scale -= 10; break;
	case 'E': _scale += 10; break;
	}

	//*/
}