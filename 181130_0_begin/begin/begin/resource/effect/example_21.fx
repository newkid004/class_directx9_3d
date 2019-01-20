//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// example_21
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//

float4x4 _mWorld : World;
float4x4 _mView : View;
float4x4 _mProjection : ProjectionInverseTranspose;

float4 _viewPosition : ViewPosition;
float4 _lightDirection : ViewDirection;

texture _texture;
texture _textureCube;

struct input
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
};

struct output
{
	float4 pos : POSITION;
	float3 normal : TEXCOORD0;
	float2 uv : TEXCOORD1;
	float3 viewDirection : TEXCOORD2;
	float3 lightDirection : TEXCOORD3;
};

// ----- main ----- //

output vsMain(input iput)
{
	output oput = (output)0;
	float4 worldPos = mul(float4(iput.pos, 1.0f), _mWorld);
	
	oput.pos = mul(worldPos, _mView);
	oput.pos = mul(oput.pos, _mProjection);
	
	float3 normal = mul(iput.normal, (float3x3)_mWorld);
	float3 viewDirection = _viewPosition.xyz - worldPos.xyz;
	float3 lightDirection = _lightDirection.xyz;
	
	oput.normal = normalize(normal);
	oput.viewDirection = normalize(viewDirection);
	oput.lightDirection = normalize(lightDirection);
	
	oput.uv = iput.uv;
	
	return oput;
}

sampler2D _sampler = sampler_state
{
	texture = _texture;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};
samplerCUBE _samplerCube = sampler_state
{
	texture = _textureCube;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

float4 psMain(output iput) : COLOR
{
	float3 normal = normalize(iput.normal);
	float3 viewDirection = normalize(iput.viewDirection);
	float3 lightDirection = normalize(iput.lightDirection);
	
	float4 diffuseColor = tex2D(_sampler, iput.uv);
	float4 environmentColor = texCUBE(_samplerCube, reflect(-viewDirection, normal));
	
	float diffuse = saturate(dot(-lightDirection, normal));
	float specular = saturate(dot(reflect(lightDirection, normal), viewDirection));
	specular = pow(specular, 20.0f);
	
	float4 finalColor = diffuseColor;
	finalColor.rgb = finalColor.rgb * diffuse;
	finalColor.rgb = finalColor.rgb + (finalColor.rgb * specular);
	finalColor.rgb = finalColor.rgb + (environmentColor.rgb * 0.5f);
	
	return finalColor;
}

// ----- technique 2 ----- //

struct t2Input
{
	float3 pos : POSITION;
	float3 pos3D : TEXCOORD0;
};

struct t2Output
{
	float4 pos : POSITION;
	float3 pos3D : TEXCOORD0;
};

t2Output vsT2(t2Input iput)
{
	t2Output oput = (t2Output)0;
	
	float4 worldPos = mul(float4(iput.pos, 1.0f), _mWorld);
	oput.pos = mul(worldPos, _mView);
	oput.pos = mul(oput.pos, _mProjection);
	
	oput.pos3D = iput.pos;

	return oput;
}

samplerCUBE _samplerJustCube = sampler_state
{
	texture = _textureCube;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

float4 psT2(t2Output iput) : COLOR
{
	return texCUBE(_samplerJustCube, iput.pos3D);
}

// ----- skyboxTechnique ----- //
struct skyInput
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
};

struct skyOutput
{
	float4 pos : POSITION;
	float3 normal : TEXCOORD;
};

skyOutput vsSkybox(skyInput iput)
{
	skyOutput oput = (skyOutput)0;

	float4 worldPos = mul(iput.pos, _mWorld);
	oput.pos = mul(worldPos, _mView);
	oput.pos = mul(oput.pos, _mProjection);

	float3 normal = mul(iput.normal, (float3x3)_mWorld);
	oput.normal = normal;

	return oput;
}

float4 psSkybox(skyOutput iput) : COLOR0
{
	return texCUBE(_samplerCube, normalize(iput.normal));
}

//--------------------------------------------------------------//
// Technique Section for example_21
//--------------------------------------------------------------//
technique myTechnique
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 vsMain();
		PixelShader = compile ps_3_0 psMain();
	}
}

technique justTexture
{
	pass Pass_0
	{
		CULLMODE = CW;

		VertexShader = compile vs_3_0 vsT2();
		PixelShader = compile ps_3_0 psT2();
	}
}

technique skyboxTechnique
{
	pass Pass_0
	{
		cullMode = CW;

		VertexShader = compile vs_3_0 vsSkybox();
		PixelShader = compile ps_3_0 psSkybox();
	}
}