// box blur pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

cbuffer ScreenSize : register(b0)
{
    float2 scrSize;
    float2 scrPadding;
};

cbuffer Kernel : register(b1)
{
    float4x4 kernel;
    float coeff;
    float3 nhPadding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    float4 texCol = texture0.Sample(Sampler0, input.tex);
	
    // Do box blur here
    float4 colour = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float texelWidth = 1.0f / scrSize.x;
    float texelHeight = 1.0f / scrSize.y;
    
    //float texelWidth = 1.0f / 1184;
    //float texelHeight = 1.0f / 636;
    
    float2 texPosNorth = input.tex + float2(0.0f, texelHeight);
    float2 texPosNorthEast = input.tex + float2(texelWidth, texelHeight);
    float2 texPosEast = input.tex + float2(texelWidth, 0.0f);
    float2 texPosSouthEast = input.tex + float2(texelWidth, -texelHeight);
    float2 texPosSouth = input.tex + float2(0.0f, -texelHeight);
    float2 texPosSouthWest = input.tex + float2(-texelWidth, -texelHeight);
    float2 texPosWest = input.tex + float2(-texelWidth, 0.0f);
    float2 texPosNorthWest = input.tex + float2(-texelWidth, texelHeight);
    
    // --------------------------------------------------------------------------
    
    colour += texCol * kernel[1].y;
    colour += texture0.Sample(Sampler0, texPosNorth) * kernel[0].y;
    colour += texture0.Sample(Sampler0, texPosNorthEast) * kernel[0].z;
    colour += texture0.Sample(Sampler0, texPosEast) * kernel[1].z;
    colour += texture0.Sample(Sampler0, texPosSouthEast) * kernel[2].z;
    colour += texture0.Sample(Sampler0, texPosSouth) * kernel[2].y;
    colour += texture0.Sample(Sampler0, texPosSouthWest) * kernel[2].x;
    colour += texture0.Sample(Sampler0, texPosWest) * kernel[1].x;
    colour += texture0.Sample(Sampler0, texPosNorthWest) * kernel[0].x;
     
    colour *= coeff;
    
    return colour;
}