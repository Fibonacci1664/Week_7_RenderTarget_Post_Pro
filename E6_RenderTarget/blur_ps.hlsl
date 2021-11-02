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
    float4 kernel;
    float neighbourhoodSize;
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
    
    float2 texPosNorth = input.tex + float2(0.0f, texelHeight);
    float2 texPosNorthEast = input.tex + float2(texelWidth, texelHeight);
    float2 texPosEast = input.tex + float2(texelWidth, 0.0f);
    float2 texPosSouthEast = input.tex + float2(texelWidth, -texelHeight);
    float2 texPosSouth = input.tex + float2(0.0f, -texelHeight);
    float2 texPosSouthWest = input.tex + float2(-texelWidth, -texelHeight);
    float2 texPosWest = input.tex + float2(-texelWidth, 0.0f);
    float2 texPosNorthWest = input.tex + float2(-texelWidth, texelHeight);
       
    // --------------------------------------------------------------------------
    
    // 3x3 matrix structure
    
    //  -- m00 -- m01 -- m02
    //  -- m10 -- m11 -- m12
    //  -- m20 -- m21 -- m22
    
    // --------------------------------------------------------------------------
    
    colour += texCol * kernel.x;
    colour += texture0.Sample(Sampler0, texPosNorth * kernel.x);
    colour += texture0.Sample(Sampler0, texPosNorthEast * kernel.x);
    colour += texture0.Sample(Sampler0, texPosEast * kernel.x);
    colour += texture0.Sample(Sampler0, texPosSouthEast * kernel.x);
    colour += texture0.Sample(Sampler0, texPosSouth * kernel.x);
    colour += texture0.Sample(Sampler0, texPosSouthWest * kernel.x);
    colour += texture0.Sample(Sampler0, texPosWest * kernel.x);
    colour += texture0.Sample(Sampler0, texPosNorthWest * kernel.x);
    
    colour /= 9.0f;
    
    return colour;
    //return texCol;

}