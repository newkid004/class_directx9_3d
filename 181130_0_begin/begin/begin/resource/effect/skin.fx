
// ½ºÅ°´×
int _numBlend;
float4x4 _mWorldArray[4];

struct input
{
    float3 pos : POSITION;
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
};

// first
output vsSkinned(input iput)
{
    output oput = (output)0;

    float leftWeight = 1.0f;
    for (int i = 1; i <= _numBlend; ++i)
    {
        float weight = iput.weight[i];
        float4x4 mWorld = __mWorldArray[i];

        leftWeight -= weight;

        oput.pos        += mul(float4(iput.pos, 1.0f), mWorld) * weight;
        oput.normal     += mul(iput.normal,     (float3x3)mWorld) * weight;
        oput.binormal   += mul(iput.binormal,   (float3x3)mWorld) * weight;
        oput.tangent    += mul(iput.tangent,    (float3x3)mWorld) * weight;
    }
    oput.pos        += mul(float4(iput.pos, 1.0f), _mWorldArray[0]) * leftWeight;
    oput.pos.w = 1.0f;
	oput.worldPos = oput.pos;

    oput.normal     += mul(iput.normal,     (float3x3)_mWorldArray[0]) * leftWeight;
    oput.binormal   += mul(iput.binormal,   (float3x3)_mWorldArray[0]) * leftWeight;
    oput.tangent    += mul(iput.tangent,    (float3x3)_mWorldArray[0]) * leftWeight;

    oput.uv = iput.uv;

	return oput;
}