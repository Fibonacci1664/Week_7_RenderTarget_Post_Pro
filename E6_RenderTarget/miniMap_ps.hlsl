// MiniMap pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

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
    float4 playerPosition : TEXCOORD1;
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
    float4 textureColour;
    float4 lightColour;

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
    textureColour = texture0.Sample(sampler0, input.tex);
    lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour);
    
    float4 playerPosCol = float4(1.0f, 1.0f, 0.0f, 1.0f);
    
    // All of this code is just the formuala of a circle
    // (x - a)^2 + (y - b)^2 = r^2
    float radius = 40.0f;
    float xDist = pow(abs(input.position.x) - abs(input.playerPosition.x), 2);
    float yDist = pow(abs(input.position.y) - abs(input.playerPosition.y), 2);
    float distanceToPoint = xDist + yDist;
    float radSqrd = pow(radius, 2);
    
    // If the pixel is within the radius defined, colour it yellow
    if (distanceToPoint < radSqrd)
    {
        float4 finalCol = lightColour * playerPosCol;
        
        return finalCol;
    }
	
    return lightColour * textureColour;
}