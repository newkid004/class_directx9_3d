
float4x4 _mWorld;
float4x4 _mView;
float4x4 _mProjection;

float4 _viewPosition;
float4 _lightDirection;
float4 _lightPosition;

float _time;

// 텍스쳐
texture _textureDiffuse;
texture _textureNormal;
texture _texture;
texture _textureBackground;

// 렌더 타겟
texture _renderBack;
texture _renderTarget;
texture _renderTargetGlow;
texture _renderTargetBlur;

// 기타
float _offsetInterval;
float _glowInterval;

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

sampler2D _samplerBackground = sampler_state
{
    texture = _textureBackground;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler _renderBackSampler = sampler_state
{
    texture = _renderBack;

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
};

sampler _renderGlowSampler = sampler_state
{
    texture = _renderTargetGlow;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler _renderBlurSampler = sampler_state
{
    texture = _renderTargetBlur;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

// 기타
float4 filter[7] = {
	-3.0f,	-3.0f,	0.0f,	1.0f / 64.0f,
	-2.0f,	-2.0f,	0.0f,	6.0f / 64.0f,
	-1.0f,	-1.0f,	0.0f,	15.0f / 64.0f,
	0.0f,	 0.0f,	0.0f,	20.0f / 64.0f,
	1.0f,	 1.0f,	0.0f,	15.0f / 64.0f,
	2.0f,	 2.0f,	0.0f,	6.0f / 64.0f,
	3.0f,	 3.0f,	0.0f,	1.0f / 64.0f,
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
output vsBack(input iput)
{
	output oput = (output)0;
	oput.pos = float4(iput.pos.xy, 1.0f, 1.0f);
	oput.uv = iput.uv;

	return oput;
}

float4 psBack(output iput) : COLOR
{
	float4 color = tex2D(_samplerBackground, iput.uv);
	return color;
}

// ----- pass 2 ----- // 알파값 추출
output vsScreen(input iput)
{
	output oput = (output)0;
	oput.pos = iput.pos;
	oput.uv = iput.uv;

	return oput;
}

output vsAlpha(input iput)
{
	output oput = (output)0;
	oput.pos = float4(iput.pos.xy, 0.0f, 1.0f);
	oput.uv = iput.uv;

	return oput;
}

float4 psAlpha(output iput) : COLOR
{
	float4 finalColor = tex2D(_renderSampler, iput.uv);
	return float4(finalColor.w, finalColor.w, finalColor.w, finalColor.w);
}

// ----- pass 3 ----- // 블러 효과

float texScaleX = 1.0f / 640.0f;
float texScaleY = 1.0f / 480.0f;

float4 psBlur(output iput) : COLOR
{
	float4 glowColor = 0.0f;

	for (int i = 0; i < 7; ++i)
	{
		glowColor += tex2D(_renderGlowSampler, iput.uv + filter[i].x * _offsetInterval * texScaleX) * filter[i].w;
		glowColor += tex2D(_renderGlowSampler, iput.uv + filter[i].y * _offsetInterval * texScaleY) * filter[i].w;
	}

	return glowColor * _glowInterval;
}

// ----- pass 4 ----- // 샘플러 블렌드
float4 psBlend(output iput) : COLOR
{
	float4 colorOrigin = tex2D(_renderSampler, iput.uv);
	float4 colorBlur = tex2D(_renderBlurSampler, iput.uv);

	return colorOrigin + colorBlur;
}

// ----- technique ----- //
technique techBackground
{
	pass p0
	{
		VertexShader = compile vs_3_0 vsBack();
		PixelShader = compile ps_3_0 psBack();
	}
}

technique techDrawMesh
{
	pass p0
	{
		VertexShader = compile vs_3_0 vsMain();
		PixelShader = compile ps_3_0 psMain();
	}
}

technique techGlow
{
	pass p0
	{
		VertexShader = compile vs_3_0 vsAlpha();
		PixelShader = compile ps_3_0 psAlpha();
	}
}

technique techBlur
{
	pass p0
	{
		VertexShader = compile vs_3_0 vsScreen();
		PixelShader = compile ps_3_0 psBlur();
	}
}

technique techBlend
{
	pass p0
	{
		VertexShader = compile vs_3_0 vsScreen();
		PixelShader = compile ps_3_0 psBlend();
	}
}