// Colour pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
//Texture2D texture0 : register(t0);
//SamplerState Sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 diffuseColour;
	float3 lightDirection;
	float padding;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	//float4 textureColor = texture0.Sample(Sampler0, input.tex);

	float4 lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour);
	float4 diffCol = float4(0.0f, 1.0f, 0.0f, 1.0f);

	return diffCol * lightColour;
}