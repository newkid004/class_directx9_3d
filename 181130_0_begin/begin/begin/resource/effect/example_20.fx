
// 행렬
float4x4 _mWorld;
float4x4 _mView;
float4x4 _mProjection;

// 카메라, 광원
float4 _viewPosition;
float4 _lightDirection;

// 텍스쳐
texture _textureDiffuse;
texture _textureNormal;
texture _textureDisplace;

// 왜곡정도
float _distort;

// 입력
struct input
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float3 binormal : BINORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
};

// 출력
struct output
{
    float4 pos : POSITION;
    float3 normal : TEXCOORD0;
    float3 binormal : TEXCOORD1;
    float3 tangent : TEXCOORD2;
    float2 uv : TEXCOORD3;

    float3 viewDirection : TEXCOORD4;
    float3 lightDirection : TEXCOORD5;
};

output vsMain(input iput)
{
    output oput = (output)0;

    float4 worldPos = mul(float4(iput.pos, 1.0f), _mWorld);
    oput.pos = mul(worldPos, _mView);
    oput.pos = mul(oput.pos, _mProjection);

    oput.normal     = normalize(mul(iput.normal,    (float3x3)_mWorld));
    oput.binormal   = normalize(mul(iput.binormal,  (float3x3)_mWorld));
    oput.tangent    = normalize(mul(iput.tangent,   (float3x3)_mWorld));

    oput.viewDirection  = normalize(_viewPosition - worldPos.xyz);
    oput.lightDirection	= normalize(_lightDirection.xyz);
    
    oput.uv = iput.uv;

    return oput;
}

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

sampler2D _samplerDisplace = sampler_state
{
    texture = _textureDisplace;

    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

float4 psMain(output iput) : COLOR0
{
    float3 normal = normalize(iput.normal);
    float3 binormal = normalize(iput.binormal);
    float3 tangent = normalize(iput.tangent);

    float3 viewDirection = normalize(iput.viewDirection);
    float3 lightDirection = normalize(iput.lightDirection);

    // baseColor
    float4 diffuseColor = tex2D(_samplerDiffuse, iput.uv);

    // 난반사
    float diffuse = saturate(dot(-lightDirection, normal));

    // 정반사
    float specular = saturate(dot(reflect(lightDirection, normal), viewDirection));
    specular = pow(specular, 20.0f);

    float4 finalColor = diffuseColor;
    finalColor.rgb = finalColor.rgb * diffuse;
    finalColor.rgb = finalColor.rgb + (diffuseColor.rgb * specular);

    return finalColor;
}

float4 psNormal(output iput) : COLOR0
{
    float3 normal = normalize(iput.normal);
    float3 binormal = normalize(iput.binormal);
    float3 tangent = normalize(iput.tangent);

    float3 viewDirection = normalize(iput.viewDirection);
    float3 lightDirection = normalize(iput.lightDirection);

    // baseColor
    float4 diffuseColor = tex2D(_samplerDiffuse, iput.uv);

    float3x3 mInverse = float3x3(
        tangent,
        binormal,
        normal);

    mInverse = transpose(mInverse);

    // 노말 매핑
    //*/
    float3 tangentNormal = tex2D(_samplerNormal, iput.uv).rgb;
    tangentNormal = normalize((tangentNormal * 2.0f) - 1.0f);
    tangentNormal = mul(tangentNormal, mInverse);

    // 시차 매핑
    /*/
    float height = tex2D(_samplerDisplace, iput.uv);
    float2 E = normalize(-iput.viewDirection).xy;
    float2 tex = iput.uv + 0.03f * height * E;

    float3 tangentNormal = tex2D(_samplerDisplace, tex).xyz;
    tangentNormal = normalize(tangentNormal * 2.0f - 1.0f);
    tangentNormal = mul(tangentNormal, mInverse);

    //*/

    normal = tangentNormal;

    // 난반사
    float diffuse = saturate(dot(-lightDirection, normal));

    // 정반사
    float specular = saturate(dot(reflect(lightDirection, normal), viewDirection));
    specular = pow(specular, 20.0f);

    float4 finalColor = diffuseColor;
    finalColor.rgb = finalColor.rgb * diffuse;
    finalColor.rgb = finalColor.rgb + (diffuseColor.rgb * specular);

    return finalColor;
}

float4 psHeight(output iput) : COLOR0
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

    // 노말 매핑
    /*/
    float3 tangentNormal = tex2D(_samplerNormal, iput.uv).rgb;
    tangentNormal = normalize((tangentNormal * 2.0f) - 1.0f);
    tangentNormal = mul(tangentNormal, mInverse);

    // 시차 매핑
    /*/
    float height = tex2D(_samplerNormal, iput.uv).w;
    float2 eye = normalize(iput.viewDirection).xy;
    float2 texUV = iput.uv + (height * _distort * eye);

    float3 tangentNormal = tex2D(_samplerNormal, texUV).xyz;
    tangentNormal = normalize(tangentNormal * 2.0f - 1.0f);
    tangentNormal = mul(tangentNormal, mInverse);

    //*/

    normal = tangentNormal;

    // baseColor
    float4 diffuseColor = tex2D(_samplerDiffuse, texUV);

    // 난반사
    float diffuse = saturate(dot(-lightDirection, normal));

    // 정반사
    float specular = saturate(dot(reflect(lightDirection, normal), viewDirection));
    specular = pow(specular, 20.0f);

    float4 finalColor = diffuseColor;
    finalColor.rgb = finalColor.rgb * diffuse;
    finalColor.rgb = finalColor.rgb + (diffuseColor.rgb * specular);

    return finalColor;
}

technique myTechnique
{
    pass P0
    {
        VertexShader = compile vs_3_0 vsMain();
        PixelShader = compile ps_3_0 psMain();
    }
}

technique tNormal
{
    pass P0
    {
        VertexShader = compile vs_3_0 vsMain();
        PixelShader = compile ps_3_0 psNormal();
    }
}

technique tHeight
{
    pass P0
    {
        VertexShader = compile vs_3_0 vsMain();
        PixelShader = compile ps_3_0 psHeight();
    }
}