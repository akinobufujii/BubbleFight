//////////////////////////////
// グローバル変数
//////////////////////////////
float4x4 	g_World;
float4x4 	g_View;
float4x4 	g_Proj;
float		g_BlurStart = 30.f;	// ぼかし始め距離

//////////////////////////////
// 構造体宣言
//////////////////////////////
struct VS_OUT
{
	float4	Pos		:POSITION;	// 座標
	float4	Depth	:TEXCOORD0;	// 深度
};

//////////////////////////////
// 頂点シェーダ
//////////////////////////////
VS_OUT VS_Depth( float4 pos : POSITION )
{
	VS_OUT Out;
	
	// 座標変換と深度計算
	Out.Depth = mul( mul(pos, g_World), g_View );
	Out.Pos	  = mul(Out.Depth, g_Proj);
	
	return Out;
}

//////////////////////////////
// ピクセルシェーダ
//////////////////////////////
float4 PS_Depth( VS_OUT vs ) : COLOR0
{
	return vs.Depth.z / g_BlurStart;
	
}


//////////////////////////////
// テクニック
//////////////////////////////
technique Tec
{
    pass P0
    {
        vertexShader = compile vs_2_0 VS_Depth();
        pixelShader  = compile ps_2_0 PS_Depth();
	}
}
