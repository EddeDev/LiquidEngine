struct VertexOutput
{
    float4 Position : SV_POSITION;
};

float4 main(VertexOutput input) : SV_TARGET
{
	return float4(0.2f, 0.8f, 0.8f, 1.0f);
}