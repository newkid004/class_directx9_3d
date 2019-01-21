
// 행렬
float4x4 _mWorld;
float4x4 _mView;
float4x4 _mProjection;

// 광원
float4x4 _mLightView;
float4 _lightDirection;
float4 _lightPosition;

// 스키닝
int _numBlend;
float4x4 _mWorldArray[4];

// 기타
float4 _viewPosition;

// 텍스쳐
texture _texture;
texture _texTerrianA;
texture _texTerrianB;
texture _texTerrianC;
texture _texTerrianD;
texture _texTerrianSplat;

sampler2D _sampler = sampler_state 
{
    texture = _texture;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler2D _samplerTerrianA = sampler_state 
{
    texture = _texTerrianA;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler2D _samplerTerrianB = sampler_state 
{
    texture = _texTerrianB;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler2D _samplerTerrianC = sampler_state 
{
    texture = _texTerrianC;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler2D _samplerTerrianD = sampler_state 
{
    texture = _texTerrianD;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler2D _samplerTerrianSplat = sampler_state 
{
    texture = _texTerrianSplat;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

// ----- IO ----- //
struct input
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float3 binormal : BINORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
    float4 weight : BLENDWEIGHT;
};

struct output
{
    float4 pos : POSITION;
    float3 normal : TEXCOORD0;
    float3 binormal : TEXCOORD1;
    float3 tangent : TEXCOORD2;
    float2 uv : TEXCOORD3;
    
	float4 worldPos : TEXCOORD4;

	float4 finalColor : TEXCOORD5;
};

// ----- Vertex Shader ----- //
// * 1st : default
output vsDefault(input iput)
{
    output oput = (output)0;

	oput.pos = mul(float4(iput.pos.xyz, 1.0f), _mWorld);

	oput.normal = normailze(mul(iput.normal, _mWorld);
	oput.binormal = normailze(mul(iput.binormal, _mWorld);
	oput.tangent = normailze(mul(iput.tangent, _mWorld);

	oput.worldPos = oput.pos;
	oput.uv = iput.uv;

	return oput;
}

// * 1st : skinned mesh
output vsSkinned(input iput)
{
	output oput = (output)0;

    float leftWeight = 1.0f;
    for (int i = 1; i <= _numBlend; ++i)
    {
        float weight = iput.weight[i];
        float4x4 mWorld = _mWorldArray[i];

        leftWeight -= weight;

        oput.pos        += mul(float4(iput.pos.xyz, 1.0f), mWorld) * weight;
        oput.normal     += mul(iput.normal,     (float3x3)mWorld) * weight;
        oput.binormal   += mul(iput.binormal,   (float3x3)mWorld) * weight;
        oput.tangent    += mul(iput.tangent,    (float3x3)mWorld) * weight;
    }
    oput.pos        += mul(float4(iput.pos.xyz, 1.0f), _mWorldArray[0]) * leftWeight;
    oput.pos.w = 1.0f;

    oput.normal     += mul(iput.normal,     (float3x3)_mWorldArray[0]) * leftWeight;
    oput.binormal   += mul(iput.binormal,   (float3x3)_mWorldArray[0]) * leftWeight;
    oput.tangent    += mul(iput.tangent,    (float3x3)_mWorldArray[0]) * leftWeight;

	oput.worldPos = oput.pos;
    oput.uv = iput.uv;

	return oput;
}

output vsView(output iput)
{
	iput.pos = mul(iput.pos, _mView);
	return iput;
}

// ----- Pixel Shader ----- //
float4 psTexture(output iput) : COLOR
{
	iput.finalColor = tex2D(_sampler, iput.uv);

	float specular = saturate(dot(reflect(_lightDirection, normal), _viewDirection));
	specular = pow(specular, 20.0f);

	return iput.finalColor;
}

float4 psDiffuse(output iput) : COLOR
{
	float3 normal = normalize(iput.normal);
	
	float diffuse = saturate(dot(-_lightDirection.xyz, normal));
	'
	iput.finalColor.rgb = iput.finalColor.rgb * diffuse;
	return iput.finalColor;
}

float4 psSpecular(output iput) : COLOR
{
	float specular = saturate(dot(reflect(_lightDirection, normal), _viewDirection));
	specular = pow(specular, 20.0f);
	
	iput.finalColor.rgb = iput.finalColor.rgb + (iput.finalColor.rgb * specular);
	return iput.finalColor;
}

float4 psEnvironment(output iput) : COLOR
{
	float3 normal = normalize(iput.normal);

	float4 environmentColor = texCUBE(_samplerCube, reflect(-_viewDirection, normal));
	
	iput.finalColor.rgb = iput.finalColor.rgb + (environmentColor.rgb * 0.5f);
	return iput.finalColor;
}