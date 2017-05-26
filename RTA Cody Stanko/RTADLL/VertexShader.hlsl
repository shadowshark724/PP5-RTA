cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix projection;
	matrix view;
	matrix model;
};

struct VertOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	//float4 norm : NORMAL;
};

struct VertIn
{
	float4 position : POSITION;
	float4 color : COLOR;
	//float4 norm : NORMAL;
};

VertOut main(VertIn input)
{
	VertOut output;

	output.position = input.position;
	output.position = mul(output.position, model);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	output.color = input.color;

	return output;
}