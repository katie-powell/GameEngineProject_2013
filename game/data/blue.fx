// Let's make this BLUEEEE

// Globals
float4x4 gWorld; // World Transform Matrix
float4x4 gViewProj; // Combined View and Projection
	
// For now, vertex shader input is only the position
struct VS_INPUT
{
	float3 vPos : POSITION0;
};

// For now, our vertex shader only needs to ouput
// the correct transformed position.
struct VS_OUTPUT
{
    float4 vPos : POSITION0;
};

// Our basic vertex shader takes the model space
// position, and transforms it into screen space.
VS_OUTPUT VS_Blue(VS_INPUT IN)
{
    // Clear the output structure.
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	
	// Apply world transform, then view/projection
	OUT.vPos = mul(gWorld, float4(IN.vPos, 1.0f));
	OUT.vPos = mul(gViewProj, OUT.vPos);
	 
	// Done--return the output.
    return OUT;
}

// For now, our pixel shader just outputs blue
float4 PS_Blue() : COLOR
{
    return float4(0.0f, 0.0f, 1.0f, 1.0f);
}

technique DefaultTechnique
{
    pass P0
    {
        vertexShader = compile vs_2_0 VS_Blue();
        pixelShader  = compile ps_2_0 PS_Blue();

		FillMode = WireFrame; // Change to wireframe
    }
}
