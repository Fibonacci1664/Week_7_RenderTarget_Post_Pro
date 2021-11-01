// box blur pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

cbuffer ScreenSize : register(b0)
{
    float screen_width;
    float screen_height;
    float2 scrPadding;
};

cbuffer Kernel : register(b1)
{
    matrix kernel;
    float kernelCoefficient;
    float3 coeffPadding;
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
    float texelWidth = 1.0f / screen_width;
    float texelHeight = 1.0f / screen_height;
    
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
    
    colour += texCol * kernel._m11;
    colour += texture0.Sample(Sampler0, texPosNorth * kernel._m01);
    colour += texture0.Sample(Sampler0, texPosNorthEast * kernel._m02);
    colour += texture0.Sample(Sampler0, texPosEast * kernel._m12);
    colour += texture0.Sample(Sampler0, texPosSouthEast * kernel._m22);
    colour += texture0.Sample(Sampler0, texPosSouth * kernel._m21);
    colour += texture0.Sample(Sampler0, texPosSouthWest * kernel._m20);
    colour += texture0.Sample(Sampler0, texPosWest * kernel._m10);
    colour += texture0.Sample(Sampler0, texPosNorthWest * kernel._m00);
    
    colour *= kernelCoefficient;
    
    return colour;
    //return texCol;

}