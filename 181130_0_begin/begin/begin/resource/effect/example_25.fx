
float4x4 _mWorld;
float4x4 _mView;
float4x4 _mProjection;

float4 _viewPosition;
float4 _lightDirection;
float4 _lightPosition;

// 텍스쳐
texture _textureDiffuse;
texture _textureNormal;
texture _texture;

// 렌더 타겟
texture _renderTarget;
texture _renderTargetGlow;

sampler2D _samplerDiffuse = sampler_state
{
    texture = _textureDiffuse;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler2D _samplerNormal = sampler_state
{
    texture = _textureNormal;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler2D _sampler = sampler_state
{
    texture = _texture;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler _renderSampler = sampler_state
{
    texture = _renderTarget;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
}

sampler _renderGlowSampler = sampler_state
{
    texture = _renderTargetGlow;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
}

// 기타
float4 filterH[7] = {
	0.0f,	-3.0f,	0.0f,	1.0f / 64.0f,
	0.0f,	-2.0f,	0.0f,	6.0f / 64.0f,
	0.0f,	-1.0f,	0.0f,	15.0f / 64.0f,
	0.0f,	 0.0f,	0.0f,	20.0f / 64.0f,
	0.0f,	 1.0f,	0.0f,	15.0f / 64.0f,
	0.0f,	 2.0f,	0.0f,	6.0f / 64.0f,
	0.0f,	 3.0f,	0.0f,	1.0f / 64.0f,
};

float4 filterV[7] = {
	-3.0f,	0.0f,	0.0f,	1.0f / 64.0f,
	-2.0f,	0.0f,	0.0f,	6.0f / 64.0f,
	-1.0f,	0.0f,	0.0f,	15.0f / 64.0f,
	0.0f,	0.0f,	0.0f,	20.0f / 64.0f,
	1.0f,	0.0f,	0.0f,	15.0f / 64.0f,
	2.0f,	0.0f,	0.0f,	6.0f / 64.0f,
	3.0f,	0.0f,	0.0f,	1.0f / 64.0f,
};

// IO
struct input
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
};

struct output
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 binormal : TEXCOORD2;
	float3 tangent : TEXCOORD3;
	
	float3 viewDirection : TEXCOORD4;
	float3 lightDirection : TEXCOORD5;
};

// ----- pass0 ----- //
output vsMain(input iput)
{
	output oput = (output)0;

	float4 worldPos = mul(float4(iput.pos.xyz, 1.0f), _mWorld);
	oput.pos = worldPos;

	oput.pos = mul(oput.pos, _mView);
	oput.pos = mul(oput.pos, _mProjection);

	oput.normal = normalize(mul(iput.normal, (float3x3)_mWorld));
	oput.binormal = normalize(mul(iput.binormal, (float3x3)_mWorld));
	oput.tangent = normalize(mul(iput.tangent, (float3x3)_mWorld));
	
	oput.uv = iput.uv;

	oput.viewDirection = normalize(worldPos.xyz - _viewPosition.xyz);
	oput.lightDirection = normalize(worldPos.xyz - _lightPosition.xyz);

	return oput;
}

float4 psMain(output iput) : COLOR0
{
	float3 normal = normalize(iput.normal);
	float3 binormal = normalize(iput.binormal);
	float3 tangent = normalize(iput.tangent);
	
	float3 viewDirection = normalize(iput.viewDirection);
	float3 lightDirection = normalize(iput.lightDirection);

	float3x3 mInverse = float3x3(
		tangent,
		binormal,
		normal);
	mInverse = transpose(mInverse);

	float3 tangentNormal = tex2D(_samplerNormal, iput.uv).xyz;
	tangentNormal = (tangentNormal * 2.0f) - 1.0f;
	tangentNormal = mul(mInverse, tangentNormal);

	float diffuse = saturate(dot(-lightDirection, tangentNormal));
	float specular = saturate(dot(reflect(lightDirection, tangentNormal), -viewDirection));
	specular = pow(specular, 20.0f);

	float4 baseColor = tex2D(_samplerDiffuse, iput.uv);

	float4 finalColor = baseColor;
	finalColor.rgb *= diffuse;
	finalColor.rgb += baseColor.rgb * specular;
	finalColor.rgb += baseColor.rgb * 0.1f;

	return finalColor;
}

// ----- pass 1 ----- // 스크린 메쉬 출력
output vsScreen(input iput)
{
	output oput = (output)0;
	oput.pos = iput.pos;
	oput.uv = iput.uv;

	return oput;
}

float4 psScreen(output oput) : COLOR
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	for(int i = 0; i < 7; ++i)
	{
		float2 offset = float2(
			filterH[i].x * (1.0f / 640.0f),
			filterH[i].y * (1.0f / 480.0f));

		color += tex2D(_sampler, iput.uv + offset) * filterH[i].w;
	}

	float4 finalColor = color;
	return finalColor;
}

// ----- pass 2 ----- // 알파값 추출
output vsAlpha(input iput)
{
	output oput = (output)0;
	oput.pos = iput.pos;
	oput.uv = iput.uv;

	return oput;
}

float4 psAlpha(output iput) : COLOR
{
	float4 finalColor = tex2D(_renderTarget, iput.uv);
	return float4(finalColor.w, finalColor.w, finalColor.w, finalColor.w);
}

// ----- pass 3 ----- //  백열광 효과
output vsGlow(input iput)
{
}

float4 psGlow(output iput) : COLOR
{
	float glowColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

	for (int i = 0;i < 7; ++i)
	{
		float2 offsetH = float2(
			filterH[i].x * (1.0f / 640.0f),
			filterH[i].y * (1.0f / 480.0f));
			
		float2 offsetV = float2(
			filterV[i].x * (1.0f / 640.0f),
			filterV[i].y * (1.0f / 480.0f));

		glowColor += tex2D(_renderGlowSampler, iput.uv + offsetH) * filterH[i].w;
		glowColor += tex2D(_renderGlowSampler, iput.uv + offsetV) * filterV[i].w;
	}

	float4 diffuseColor = tex2D(_renderTarget, iput.uv);
	return diffuseColor + (glowColor * diffuseColor) * 0.5f;
}

// ----- technique ----- //

technique myTechnique
{
	pass p0
	{
		VertexShader = compile vs_3_0 vsMain();
		PixelShader = compile ps_3_0 psMain();
	}

	pass p1
	{
		VertexShader = compile vs_3_0 vsScreen();
		PixelShader = compile ps_3_0 psScreen();
	}
}
