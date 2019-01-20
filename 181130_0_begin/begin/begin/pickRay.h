#pragma once
#include "kGlobalDefine.h"

class pickRay
{
public :
	struct info
	{
		D3DXVECTOR3 rayDir;
		D3DXVECTOR3 rayOrigin;

		LPD3DXMESH mesh;
		BOOL isHit;
		DWORD faceIndex;
		D3DXVECTOR2 uv;
		FLOAT distance;
	};

public :
	static bool chkPick(LPD3DXMESH mesh, info* out_info);

public:
	pickRay();
	~pickRay();
};

