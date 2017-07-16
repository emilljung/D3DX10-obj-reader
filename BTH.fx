//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
struct VSSceneIn
{
	float3 Pos	: POS;
	float3 Normal : NORMAL;
	float2 Tex : TEX;
};

struct PSSceneIn
{
	float4 Pos  : SV_Position;		// SV_Position is a (S)ystem (V)ariable that denotes transformed position
	float3 Normal : NORMAL;
	float2 Tex : TEX;
	float3 posw : POSW;
};


struct PLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 pos;
	float radius;
};

PLight light;

//-----------------------------------------------------------------------------------------
// Constant Buffers (where we store variables by frequency of update)
//-----------------------------------------------------------------------------------------
cbuffer cbEveryFrame
{
	matrix g_mWorldViewProjection;
	matrix g_WorldMatrix;
	float3 camPos;
};

Texture2D BTHtexture;

//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

DepthStencilState EnableDepthTestOnly
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = none;
	FillMode = solid;
};

SamplerState Filtering
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSSceneIn input)
{
	PSSceneIn output = (PSSceneIn)0;
	
	// transform the point into view space
	output.Pos = mul( float4(input.Pos,1.0), g_mWorldViewProjection );
	output.Normal = mul( float4(input.Normal, 0.0), g_WorldMatrix ).xyz;
	output.Tex = input.Tex;
	output.posw = mul( float4(input.Pos,1.0), g_WorldMatrix).xyz;

	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{
	float4 color = BTHtexture.Sample(Filtering, input.Tex);
	float4 result = float4( 0.0f, 0.0f, 0.0f, 1.0f);
	float3 distVec = light.pos - input.posw;
	float dist = length(distVec);
	
	//return float4( input.Normal, 0.0f );
	//float4 temporary_debug_color = float4( 1.0f, 0.0f, 0.0f, 0.0f );
	//return temporary_debug_color;
	if(dist <= light.radius)
	{
		//ambient
		result += light.ambient;

		//diffuse
		float3 distVecNorm = normalize(distVec);
		input.Normal = normalize(input.Normal);
		float DiffFac = abs(dot(distVecNorm,input.Normal));
		
			result += light.diffuse * DiffFac * color;

		//specular
			float3 camVec = normalize(camPos - input.posw);
			float3 reflection = reflect(-distVec, input.Normal);
			float specFac = pow(max(dot(reflection,camVec),0), max(light.specular.a,1.0f));

			result += light.specular * specFac * color;
	}
	return result;
}

//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique10 DrawBTH
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    
	    SetRasterizerState( NoCulling );

	    SetDepthStencilState( EnableDepth, 0 );
	    //SetDepthStencilState( DisableDepth, 0 );
	    //SetDepthStencilState( EnableDepthTestOnly, 0 );
    }  
}