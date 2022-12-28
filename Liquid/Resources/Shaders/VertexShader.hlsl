struct VertexInput
{
	float4 Position : POSITION;
	float4 Color : COLOR1;
};

cbuffer PushConstants
{
    float4x4 Transform;
};

struct VertexOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

VertexOutput main(VertexInput input)
{
	VertexOutput output;
	input.Position.w = 1.0f;
	output.Position = input.Position;
    output.Color = input.Color;
	return output;
}