
// ���
float4x4 _mWorld;
float4x4 _mView;
float4x4 _mProjection;

// ����
float4 _lightDirection;
float4 _lightPosition;
float4x4 _mLightWorld;
float4x4 _mLightView;
float4x4 _mLightProjection;

// ī�޶�
float4 _viewPosition;

// �ؽ���
float4x4 _mTexture;

// �Է�
struct input
{
    float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
};

// ���
struct output
{
    float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
	float4 lightViewPos : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
};

// ���� ���̴�
output vsMain(input iput)
{
    output oput = (output)0;

	// ��ġ
	float4 worldPos = mul(float4(iput.pos, 1.0f), _mWorld);
    oput.pos = mul(worldPos, _mView);
    oput.pos = mul(oput.pos, _mProjection);

	// ����
	oput.lightViewPos = mul(float4(iput.pos, 1.0f), _mWorld);
	oput.lightViewPos = mul(oput.lightViewPos, _mLightView);
	oput.lightViewPos = mul(oput.lightViewPos, _mLightProjection);

	oput.lightPos = normalize(_lightPosition.xyz - worldPos.xyz);
	
	// ��Ÿ
    oput.normal = normalize(mul(iput.normal, (float3x3)_mWorld));
	oput.uv = iput.uv;

    return oput;
}

// ���÷�
Texture2D _depthMap;
sampler2D _depthSampler = sampler_state
{
	texture = _depthMap;
};

// �ȼ� ���̴�
float4 psMain(output iput) : COLOR0
{
	// ���� ���� �� ����
	float lightIntensity;
	float4 lightDirection = -_lightDirection;

	// �۽Ƿ�
	float bias = 0.001f;

	// �⺻ ����
	float4 baseColor = float4(0.5f, 0.9f, 0.7f, 1.0f);
	float4 finalColor = baseColor * 0.2f;	// ambient

	// ���� �ؽ��� ��ǥ ���
	float2 projectPos;
	projectPos.x = iput.lightViewPos.x / iput.lightViewPos.w / 2.0f + 0.5f;
	projectPos.y = iput.lightViewPos.y / iput.lightViewPos.w / 2.0f + 0.5f;

	if ((saturate(projectPos.x) == projectPos.x) && (saturate(projectPos.y) == projectPos.y))
	{
		float depthValue = tex2D(_depthSampler, projectPos).r;
		float lightDepthValue = (iput.lightViewPos.z / iput.lightViewPos.w) - bias;

		// ���� ó�� ����
		if (lightDepthValue < depthValue)
		{
			lightIntensity = saturate(dot(iput.normal, lightDirection.xyz));
			if (0.0f < lightIntensity)
			{
				finalColor += baseColor * lightIntensity;
				finalColor = saturate(finalColor);
			}
		}
	}
	else
	{
		// �׸��� �� �ٱ�
		lightIntensity = saturate(dot(iput.normal, lightDirection.xyz));
		if (0.0f < lightIntensity)
		{
				finalColor += baseColor * lightIntensity;
				finalColor = saturate(finalColor);
		}
	}

	return finalColor;
}

// ----- shadow ----- //
struct outputShadow
{
	float4 pos : POSITION;
	float depth : TEXCOORD;
};

outputShadow vsShadow(float4 inPos : POSITION)
{
	outputShadow oput = (outputShadow)0;

	oput.pos = mul(inPos, _mLightWorld);
	oput.pos = mul(oput.pos, _mLightView);
	oput.pos = mul(oput.pos, _mLightProjection);

	return oput;
}

float4 psShadow(outputShadow iput) : COLOR
{
	return float4(iput.depth, iput.depth, iput.depth, 1.0f);
}

// ----- mapping ----- //
struct outputMapping
{
	float4 pos : POSITION;
	float4 uv : TEXCOORD0;
	float depth : TEXCOORD1;
};

outputMapping vsMapping(float4 pos : POSITION)
{
	outputMapping oput = (outputMapping)0;

	oput.pos = mul(pos, _mWorld);
	oput.pos = mul(oput.pos, _mView);
	oput.pos = mul(oput.pos, _mProjection);

	oput.uv = mul(pos, _mTexture);
	
	float4 mDepth = mul(pos, _mLightView);
	oput.depth = mul(mDepth, _mLightProjection).z;

	return oput;
}

float4 psMapping(outputMapping iput) : COLOR0
{
	float4 texCoord[9];
	float texelSize = 1.0f / 640.0f;

	texCoord[0] = iput.uv;

	texCoord[1] = iput.uv + float4(-texelSize, 0.0f, 0.0f, 0.0f);
	texCoord[2] = iput.uv + float4( texelSize, 0.0f, 0.0f, 0.0f);
	texCoord[3] = iput.uv + float4(0.0f, -texelSize, 0.0f, 0.0f);
	texCoord[6] = iput.uv + float4(0.0f,  texelSize, 0.0f, 0.0f);

	texCoord[4] = iput.uv + float4(-texelSize, -texelSize, 0.0f, 0.0f);
	texCoord[5] = iput.uv + float4( texelSize, -texelSize, 0.0f, 0.0f);
	texCoord[7] = iput.uv + float4(-texelSize,  texelSize, 0.0f, 0.0f);
	texCoord[8] = iput.uv + float4( texelSize,  texelSize, 0.0f, 0.0f);

	float shadowTerm = 0.0f;
	for (int i = 0 ; i < 9; ++i)
	{
		float distance1 = tex2Dproj(_depthSampler, texCoord[i]).r;
		float distance2 = iput.depth - 0.1f;

		shadowTerm += distance1 < distance2 ? 0.0f : 1.0f;
	}

	return shadowTerm / 9.0f;
}

technique myTechnique
{
    pass P0
    {
        VertexShader = compile vs_3_0 vsMain();
        PixelShader = compile ps_3_0 psMain();
    }
}

technique shadow
{
	pass P0
	{
		cullMode = NONE;
		
		VertexShader = compile vs_3_0 vsShadow();
		PixelShader = compile ps_3_0 psShadow();
	}
}

technique mapping
{
	pass P0
	{
		VertexShader = compile vs_3_0 vsMapping();
		PixelShader = compile ps_3_0 psMapping();
	}
}