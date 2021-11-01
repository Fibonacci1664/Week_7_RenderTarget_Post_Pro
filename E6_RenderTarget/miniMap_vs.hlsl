// MiniMap vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer PlayerPositionBuffer : register(b1)
{
    float4 playerPos;
}; 

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 playerPosition : TEXCOORD1;
};

OutputType main(InputType input)
{
    OutputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    // Model space to world space
    output.position = mul(input.position, worldMatrix);
    // World space to camera space
    output.position = mul(output.position, viewMatrix);
    // Camera space to projection space
    output.position = mul(output.position, projectionMatrix);
    
    // AFTER THIS THE REST OF THE TRANSFORMATIONS ARE DONE FOR US BY THE PIPELINE FOR RASTERIZASTION
    // Buuuuut...
    // Here it is should you need it (YOU WILL!)
    
    /*
    position.xyz /= position.w; // To homogenous
    position.xy *= float2(0.5f, -0.5f);
    position.xy += 0.5f;
    position.xy *= float2(screen size);
    
    This is very similiar to how a lot of ShaderToy projects are started.
    */
    
    // Model space to world space
    //output.playerPosition = mul(playerPos, worldMatrix);
    // World space to camera space
    //output.playerPosition = mul(output.playerPosition, viewMatrix);
    output.playerPosition = mul(playerPos, viewMatrix);
    // Camera space to projection space
    output.playerPosition = mul(output.playerPosition, projectionMatrix);
    
    output.playerPosition.xyz /= output.playerPosition.w; // To homogenous
    output.playerPosition.xy *= float2(0.5f, -0.5f);
    output.playerPosition.xy += 0.5f;
    output.playerPosition.xy *= float2(1184, 636);
    
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Calculate the normal vector against the world matrix only and normalise.
    // Model space to world space
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);

    return output;
}