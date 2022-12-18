struct VertexInput
{
	float4 Position : POSITION;
};

struct VertexOutput
{
	float4 Position : SV_POSITION;
};

VertexOutput main(VertexInput input)
{
	VertexOutput output;
	input.Position.w = 1.0f;
	output.Position = input.Position;
	return output;
}