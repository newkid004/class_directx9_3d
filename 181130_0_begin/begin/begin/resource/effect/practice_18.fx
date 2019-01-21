
float4x4 _mWorld;
float4x4 _mProjection;

float4x4 _mView;
float4x4 _mLightView;

float4 _viewPosition;
float4 _lightDirection;
float4 _lightPosition;

// ÁöÇü
texture _textureA;
texture _textureB;
texture _textureC;
texture _textureD;
texture _textureSplat;

// Ä³¸¯ÅÍ
int _numBlend;
float4x4 _mWorldA[4];
texture _texture;

// ·»´õ Å¸°Ù
texture _renderTarget;

sampler2D _samplerA = sampler_state
{
    texture = _textureA;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};
sampler2D _samplerB = sampler_state
{
    texture = _textureB;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};
sampler2D _samplerC = sampler_state
{
    texture = _textureC;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};
sampler2D _samplerD = sampler_state
{
    texture = _textureD;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};
sampler2D _samplerSplat = sampler_state
{
    texture = _textureSplat;

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

sampler2D _renderSampler = sampler_state
{
	texture = _renderTarget;
	
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

struct input
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;

	float4 weight : BLENDWEIGHT;
};

struct output
{
	float4 pos : POSITION;
	float3 normal : TEXCOORD0;
	float2 uv : TEXCOORD1;

	float3 viewDirection : TEXCOORD2;

	float4 clipPos : TEXCOORD3;
};

// ----- pass0 ----- // draw terrain to shadow map

output vsMain(input iput)
{
	output oput = (output)0;

	float4 worldPos = mul(float4(iput.pos.xyz, 1.0f), _mWorld);
	oput.pos = worldPos;
	
	float3 lightY = float3(0.0f, 1.0f, 0.0f);
	float3 lightZ = -normalize(_lightPosition.xyz);
	float3 lightX = cross(lightY, lightZ);
	lightY = cross(lightZ, lightX);

	float4x4 mLightView = float4x4(
		float4(lightX, -dot(_lightPosition.xyz, lightX)),
		float4(lightY, -dot(_lightPosition.xyz, lightY)),
		float4(lightZ, -dot(_lightPosition.xyz, lightZ)),
		float4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	mLightView = transpose(mLightView);

	oput.clipPos = mul(oput.pos, mLightView);
	oput.clipPos = mul(oput.clipPos, _mProjection);

	oput.pos = mul(oput.pos, _mView);
	oput.pos = mul(oput.pos, _mProjection);

	oput.normal = iput.normal;
	oput.uv = iput.uv;

	return oput;
}

float4 psMain(output iput) : COLOR0
{
	float3 normal = normalize(iput.normal);
	float4 lightDirection = normalize(_lightDirection);

	float diffuse = saturate(dot(-lightDirection.xyz, normal));

    float4 colorA = tex2D(_samplerA, iput.uv);  // Red
    float4 colorB = tex2D(_samplerB, iput.uv);  // Green
    float4 colorC = tex2D(_samplerC, iput.uv);  // Blue
    float4 colorD = tex2D(_samplerD, iput.uv);  // Black
    float4 colorSplat = tex2D(_samplerSplat, iput.uv);
	
    float blackPercent = 1.0f - saturate(colorSplat.r + colorSplat.g + colorSplat.b);
	
    float4 diffuseColor = 
        (colorA * colorSplat.r) +
        (colorB * colorSplat.g) +
        (colorC * colorSplat.b) +
        (colorD * blackPercent);

    float4 baseColor = diffuseColor;
	float4 finalColor = baseColor;
	
	finalColor.rgb = baseColor.rgb * diffuse;
	finalColor.rgb = finalColor.rgb + (baseColor.rgb * 0.2f);

	float2 mapUV = float2(
		iput.clipPos.x / iput.clipPos.w,
		iput.clipPos.y / -iput.clipPos.w);
	mapUV = mapUV * 0.5f + 0.5f;
	float ownDepth = iput.clipPos.z / iput.clipPos.w;
	float mapDepth = tex2D(_renderSampler, mapUV).r;

	if (mapDepth + 0.0001f < ownDepth)
	{
		finalColor.rgb = finalColor.rgb / 2.0f;
	}

	return finalColor;
}

// ----- pass1 ----- // : create shadow map

output vsLight(input iput)
{
	output oput = (output)0;

	float4 worldPos = mul(float4(iput.pos.xyz, 1.0f), _mWorld);
	oput.pos = worldPos;

	float3 lightY = float3(0.0f, 1.0f, 0.0f);
	float3 lightZ = -normalize(_lightPosition.xyz);
	float3 lightX = cross(lightY, lightZ);
	lightY = cross(lightZ, lightX);

	float4x4 mLightView = float4x4(
		float4(lightX, -dot(_lightPosition.xyz, lightX)),
		float4(lightY, -dot(_lightPosition.xyz, lightY)),
		float4(lightZ, -dot(_lightPosition.xyz, lightZ)),
		float4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	mLightView = transpose(mLightView);

	oput.pos = mul(oput.pos, mLightView);
	oput.pos = mul(oput.pos, _mProjection);

	oput.clipPos = oput.pos;

	oput.normal = normalize(mul(iput.normal, (float3x3)_mWorld));

	return oput;
}

float4 psLight(output iput) : COLOR0
{
	float depth = iput.clipPos.z / iput.clipPos.w;
	return float4(depth, depth, depth, 1.0f);
}

// ----- pass2 ----- // draw to shadow map

output vsLighting(input iput)
{
	output oput = (output)0;

	float4 worldPos = mul(float4(iput.pos.xyz, 1.0f), _mWorld);
	oput.pos = worldPos;

	float3 lightY = float3(0.0f, 1.0f, 0.0f);
	float3 lightZ = -normalize(_lightPosition.xyz);
	float3 lightX = cross(lightY, lightZ);
	lightY = cross(lightZ, lightX);

	float4x4 mLightView = float4x4(
		float4(lightX, -dot(_lightPosition.xyz, lightX)),
		float4(lightY, -dot(_lightPosition.xyz, lightY)),
		float4(lightZ, -dot(_lightPosition.xyz, lightZ)),
		float4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	mLightView = transpose(mLightView);

	oput.clipPos = mul(oput.pos, mLightView);
	oput.clipPos = mul(oput.clipPos, _mProjection);

	oput.pos = mul(oput.pos, _mView);
	oput.pos = mul(oput.pos, _mProjection);

	oput.normal = iput.normal;

	return oput;
}

float4 psLighting(output iput) : COLOR0
{
	float3 normal = normalize(iput.normal);
	float4 lightDirection = normalize(_lightDirection);

	float2 mapUV = float2(
		iput.clipPos.x / iput.clipPos.w,
		iput.clipPos.y / -iput.clipPos.w);
	mapUV = mapUV * 0.5f + 0.5f;
	float ownDepth = iput.clipPos.z / iput.clipPos.w;
	float mapDepth = tex2D(_renderSampler, mapUV).r;

	float diffuse = saturate(dot(-lightDirection.xyz, normal));

	float4 baseColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
	float4 finalColor = baseColor;
	
	finalColor.rgb = baseColor.rgb * diffuse;
	finalColor.rgb = finalColor.rgb + (baseColor.rgb * 0.2f);

	if (mapDepth + 0.0001f < ownDepth)
	{
		finalColor.rgb = finalColor.rgb / 2.0f;
	}

	return finalColor;
}

// ----- pass3 ----- // : draw skinnedMesh to shadow map
output vsCharacter(input iput)
{
	output oput = (output)0;
	
	float3 lightY = float3(0.0f, 1.0f, 0.0f);
	float3 lightZ = -normalize(_lightPosition.xyz);
	float3 lightX = cross(lightY, lightZ);
	lightY = cross(lightZ, lightX);

	float4x4 mLightView = float4x4(
		float4(lightX, -dot(_lightPosition.xyz, lightX)),
		float4(lightY, -dot(_lightPosition.xyz, lightY)),
		float4(lightZ, -dot(_lightPosition.xyz, lightZ)),
		float4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	mLightView = transpose(mLightView);
	
	float leftWeight = 1.0f;

	for (int i = 1; i <= _numBlend; ++i)
	{
		float weight = iput.weight[i];
		float4x4 mWorld = _mWorldA[i];

		leftWeight -= weight;

		oput.pos += mul(float4(iput.pos.xyz, 1.0f), mWorld) * weight;
		oput.normal += mul(iput.normal, (float3x3)mWorld) * weight;
	}
	oput.pos += mul(float4(iput.pos.xyz, 1.0f), _mWorldA[0]) * leftWeight;
	oput.normal += mul(iput.normal, (float3x3)_mWorldA[0]) * leftWeight;
	
	oput.pos.w = 1.0f;
	
	float4 worldPos = oput.pos;
	oput.pos = mul(oput.pos, _mView);
	oput.pos = mul(oput.pos, _mProjection);
	
	oput.clipPos = mul(worldPos, mLightView);
	oput.clipPos = mul(oput.clipPos, _mProjection);

	oput.normal = normalize(oput.normal);
	oput.uv = iput.uv;

	return oput;
}

float4 psCharacter(output iput) : COLOR0
{
	float3 normal = normalize(iput.normal);
	float4 lightDirection = normalize(_lightDirection);

	float2 mapUV = float2(
		iput.clipPos.x / iput.clipPos.w,
		iput.clipPos.y / -iput.clipPos.w);
	mapUV = mapUV * 0.5f + 0.5f;
	float ownDepth = iput.clipPos.z / iput.clipPos.w;
	float mapDepth = tex2D(_renderSampler, mapUV).r;

	//float diffuse = saturate(dot(-lightDirection.xyz, normal));

	float4 baseColor = tex2D(_sampler, iput.uv);
	float4 finalColor = baseColor;
	
	//finalColor.rgb = baseColor.rgb * diffuse;
	finalColor.rgb = finalColor.rgb + (baseColor.rgb * 0.2f);

	if (mapDepth + 0.0001f < ownDepth)
	{
		finalColor.rgb = finalColor.rgb / 2.0f;
	}

	return finalColor;
}

// ----- pass 4 ----- // : create skinnedMesh shadow map

output vsCharacterMap(input iput)
{
	output oput = (output)0;

	float3 lightY = float3(0.0f, 1.0f, 0.0f);
	float3 lightZ = -normalize(_lightPosition.xyz);
	float3 lightX = cross(lightY, lightZ);
	lightY = cross(lightZ, lightX);

	float4x4 mLightView = float4x4(
		float4(lightX, -dot(_lightPosition.xyz, lightX)),
		float4(lightY, -dot(_lightPosition.xyz, lightY)),
		float4(lightZ, -dot(_lightPosition.xyz, lightZ)),
		float4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	mLightView = transpose(mLightView);

	float leftWeight = 1.0f;

	for (int i = 1; i <= _numBlend; ++i)
	{
		float weight = iput.weight[i];
		float4x4 mWorld = _mWorldA[i];

		leftWeight -= weight;

		oput.pos += mul(float4(iput.pos.xyz, 1.0f), mWorld) * weight;
	}
	oput.pos += mul(float4(iput.pos.xyz, 1.0f), _mWorldA[0]) * leftWeight;

	oput.pos.w = 1.0f;
	
	oput.pos = mul(oput.pos, mLightView);
	oput.pos = mul(oput.pos, _mProjection);

	oput.clipPos = oput.pos;

	return oput;
}

float4 psCharacterMap(output iput) : COLOR0
{
	float depth = iput.clipPos.z / iput.clipPos.w;
	return float4(depth, depth, depth, 1.0f);
}

// ----- technique ----- //

technique myTechnique
{
	pass p0
	{
		VertexShader = compile vs_3_0 vsMain();
		PixelShader = compile ps_3_0 psMain();
	}
}

technique techLightMap
{
	pass p0
	{
		VertexShader = compile vs_3_0 vsLight();
		PixelShader = compile ps_3_0 psLight();
	}
}

technique techRenderLight
{
	pass p0
	{
		VertexShader = compile vs_3_0 vsLighting();
		PixelShader = compile ps_3_0 psLighting();
	}
}

technique techCharacter
{
	pass p0
	{
		VertexShader = compile vs_3_0 vsCharacter();
		PixelShader = compile ps_3_0 psCharacter();
	}
}

technique techCharacterMap
{
	pass p0
	{
		VertexShader = compile vs_3_0 vsCharacterMap();
		PixelShader = compile ps_3_0 psCharacterMap();
	}
}