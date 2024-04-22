// Light pixel shader
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
    float3 worldcoord : TEXCOORD1;
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
    input.tex *= 10;
	float4 textureColour;
	float4 lightColour;

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	textureColour = texture0.Sample(sampler0, input.tex);
	lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour);
	
    
    //if (abs(input.normal.x) >= 0.3f || abs(input.normal.z) >= 0.3f)
    //{
       
    //    textureColour = float4(110 / 255.f, 38 / 255.f, 14 / 255.f, 1);
    //}
    //else if (input.worldcoord.y > 1.9)
    //{
    //    textureColour = float4(1, 1, 1, 1);
    //}
    if (input.worldcoord.y == 0)
    {
        textureColour = float4(0, 0, 0, 1);

    }
    else if (input.worldcoord.y < 0.8)
    {
        textureColour = float4(0, 0, 1, 1);

    }

 
	
        return lightColour * textureColour;
}



