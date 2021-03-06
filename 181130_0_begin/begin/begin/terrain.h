#pragma once
#include "kGlobalDefine.h"
#include "renderObject.h" 
#include "pickRay.h"

using namespace std;

class terrain : public renderObject
{
public :
	enum
	{
		MAX_NUM_TEXTURES = 4
	};

	struct vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 biNormal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR2 uv;
	};

	struct params
	{
		int smoothLevel;
		float heightScale;

		string pathSplat;
		string pathHeight;
		string pathEffect;

		SIZE tileSize;
		SIZE mapSize;
		vector<string> vPathTextureFile;
	};

private :
	SIZE _sizeMap;
	SIZE _sizeTile;
	SIZE _sizeTotal;

	LPD3DXMESH _mesh = nullptr;
	LPD3DXEFFECT _effect = nullptr;
	LPDIRECT3DTEXTURE9 _textureSplat = nullptr;
	std::string _runTechnique;

	float _heightScale = 0.0f;
	params _param;

	vector<float> _vHeightList;
	vector<LPDIRECT3DTEXTURE9> _vTextureList;

protected :
	virtual void drawDo(void) override;

private :
	void smoothTerrainMesh(int smoothLevel);
	void setupTerrainInfo(void);
	LPD3DXMESH createTerrainMesh(void);
	vector<LPDIRECT3DTEXTURE9> createTerrainTextures(LPDIRECT3DTEXTURE9* outSplatTexture);

	float lerp(float a, float b, float t) { return a - a * t + b * t; }

public :
	float getHeight(D3DXVECTOR2 pos);
	LPD3DXMESH getMesh(void) { return _mesh; }
	void setRunTechnique(const std::string & input) { _runTechnique = input; }

	bool getIntersectPoint(const pick::ray & ray, D3DXVECTOR3 & out_position);

public:
	terrain(const params & param);
	virtual ~terrain();
};

