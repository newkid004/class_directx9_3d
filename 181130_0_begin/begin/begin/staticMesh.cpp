#include "staticMesh.h"

#include "managerList.h"
#include "direct3dApplication.h"
#include "gFunc.h"

#include "camera.h"
#include "lightBase.h"

staticMesh::staticMesh()
{
	ZeroMemory(&_param, sizeof(_param));
}

staticMesh::staticMesh(mParam & param) :
	_param(param)
{
	_effect = MN_SRC->getEffect(param.effectFilePath);
	_info = MN_SRC->getStaticMesh(param.meshFilePath);

	setBoundingBox(gFunc::createBoundingBox(_info->mesh));
	setBoundingSphere(gFunc::createBoundingSphere(_info->mesh));
}

staticMesh::~staticMesh()
{
}

void staticMesh::drawDo(void)
{
	renderObject::drawDo();

	_effect->SetMatrix("_mWorld", &getMatrixFinal());
	_effect->SetMatrix("_mView", GET_CAMERA()->getMatrixViewPoint());
	_effect->SetMatrix("_mProjection", GET_CAMERA()->getMatrixProjectionPoint());

	_effect->SetVector("_lightDirection", &D3DXVECTOR4(GET_LIGHT()->getDirectForward(), 0.0f));
	_effect->SetVector("_viewPosition", &D3DXVECTOR4(GET_CAMERA()->getOffsetPosition(), 1.0f));

	for (int i = 0; i < _info->numMaterial; ++i)
	{
		_effect->SetTexture("_texture", _info->vTextureList[i]);

		gFunc::runEffectLoop(_effect, "myTechnique", [&](int passNum)->void {
			_info->mesh->DrawSubset(i);
		});
	}
}
