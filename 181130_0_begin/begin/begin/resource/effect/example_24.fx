
float4x4 _mWorld;
float4x4 _mView;
float4x4 _mProjection;

float4 _viewPosition;
float4 _lightDirection;
float4 _lightPosition;

texture _renderTarget;

struct input
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
};

struct output
{
	float4 pos : POSITION;
	float3 normal : TEXCOORD0;
	float2 uv : TEXCOORD1;

	float3 viewDirection : TEXCOORD2;

	float4 clipPos : TEXCOORD3;
};

// ----- pass0 ----- //

// output vsMain(input iput)
// {
// 	output oput = (output)0;

// 	float4 worldPos = mul(float4(iput.pos.xyz, 1.0f), _mWorld);
// 	oput.pos = mul(worldPos, _mView);
// 	oput.pos = mul(oput.pos, _mProjection);

// 	float3 normal = mul(iput.normal, (float3x3)_mWorld);
// 	oput.normal = normalize(normal);

// 	oput.viewDirection = normalize(worldPos.xyz - _viewPosition.xyz);
// 	oput.clipPos = oput.pos;

// 	return oput;
// }

// float4 psMain(output iput) : COLOR0
// {
// 	float3 normal = normalize(iput.normal);
// 	float3 viewDirection = normalize(iput.viewDirection);
// 	float3 lightDirection = normalize(_lightDirection.xyz);

// 	float4 diffuseColor = tex2D(_sampler, iput.uv);

// 	// float4 environmentColor = texCUBE(_samplerCube, reflect(-viewDirection, normal));
// 	float diffuse = saturate(dot(-lightDirection, normal));
// 	float specular = saturate(dot(reflect(lightDirection, normal), viewDirection));

// 	float4 finalColor = diffuseColor;
// 	finalColor.rgb = finalColor.rgb * diffuse;
// 	finalColor.rgb = finalColor.rgb + (finalColor.rgb * specular);
// 	// finalColor.rgb = finalColor.rgb + (environmentColor.rgb * 0.5f);

// 	return finalColor;
// }

// ----- pass1 ----- //

sampler2D _renderSampler = sampler_state
{
	texture = _renderTarget;
	
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

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
	oput.uv = iput.uv;

	return oput;
}

float4 psLight(output iput) : COLOR0
{
	float depth = iput.clipPos.z / iput.clipPos.w;
	return float4(depth, depth, depth, 1.0f);
}

// ----- pass2 ----- //

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
	oput.uv = iput.uv;

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

// technique myTechnique
// {
// 	pass p0
// 	{
// 		VertexShader = compile vs_3_0 vsMain();
// 		PixelShader = compile ps_3_0 psMain();
// 	}
// }

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

