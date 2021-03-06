// shadertype=hlsl
#include "common/common.hlsl"

struct PixelInputType
{
	float4 posCS : SV_POSITION;
	float4 color : COLOR;
};

struct PixelOutputType
{
	float4 froxelVisualizationPassRT0 : SV_Target0;
};

PixelOutputType main(PixelInputType input)
{
	PixelOutputType output;

	if (input.color.a == 0.0)
	{
		discard;
	}

	output.froxelVisualizationPassRT0 = input.color;

	return output;
}