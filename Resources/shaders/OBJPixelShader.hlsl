#include "OBJShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp, input.uv);
	//return input.color;
	//return input.color * texcolor;

	//float3 light = normalize(float3(1,-1,1)); // 右下奥　向きのライト
	//float light_diffuse = saturate(dot(-light, input.normal));
	//float3 shade_color;
	//shade_color = m_ambient; // アンビエント項
	//shade_color += m_diffuse * light_diffuse;	// ディフューズ項
	//float4 texcolor = tex.Sample(smp, input.uv);
	//return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);

	//float4 shadecolor;

	const float shininess = 4.0f;

	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	//float3 dotightnormal = dot(lightv,input.normal);

	//float3 reflect = normalize(-lightv + 2 * dotightnormal * input.normal);

	float3 ambient = m_ambient;
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	//float3 diffuse = dotightnormal * m_diffuse;

	//float3 specular = pow(saturate(dot(reflect,eyedir)),shininess) * m_specular;

	//shadecolor.rgb = (ambient + diffuse + specular) * lightcolor;
	//shadecolor.a = m_alpha;

	for (int i = 0; i < DIR_LIGHT_NUM; i++)
	{
		if (dirLights[i].active)
		{
			float3 dotightnormal = dot(dirLights[i].lightv, input.normal);
			float3 reflect = normalize(-dirLights[i].lightv + 2 * dotightnormal * input.normal);
			float3 diffuse = dotightnormal * m_diffuse;
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
		}
	}

	return shadecolor * texcolor;
}