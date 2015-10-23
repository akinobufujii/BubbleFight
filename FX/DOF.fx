///////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////
float		g_Focus = 0.7f;

float		g_Width = 800;
float		g_Height= 600;
float		g_BlurRate	= 1.f;	// ぼかす強さ

texture 	g_SceneTex;
texture 	g_BlurTex;
texture 	g_DepthTex;

///////////////////////////////////////////////
// テクスチャサンプラ
///////////////////////////////////////////////
sampler SceneSampler = 
sampler_state
{
    Texture = <g_SceneTex>;
	MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;

	AddressU = Clamp;
	AddressV = Clamp;
};

sampler BlurSampler = 
sampler_state
{
    Texture = <g_BlurTex>;
	MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

	AddressU = Clamp;
	AddressV = Clamp;
};

sampler DepthSampler = 
sampler_state
{
    Texture = <g_DepthTex>;
	MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;

	AddressU = Clamp;
	AddressV = Clamp;
};

///////////////////////////////////////////////
// 構造体宣言
///////////////////////////////////////////////
struct VS_OUT
{
	float4	Pos	:POSITION;	// 座標
	float2	Tex	:TEXCOORD;	// テクスチャ座標
};

///////////////////////////////////////////////
// 頂点シェーダ
///////////////////////////////////////////////
VS_OUT VS_DOF(	float4 pos : POSITION,
				float2 tex : TEXCOORD0)
{
	VS_OUT Out;
	
	// 何もしないで渡す
	Out.Pos = pos;
	Out.Tex = tex;

	return Out;
}

///////////////////////////////////////////////
// ピクセルシェーダ
///////////////////////////////////////////////
float4 PS_DOF(float2 tex : TEXCOORD0): COLOR0
{	
	// 通常シーン
  	float4 SceneColor = tex2D(SceneSampler, tex);
  	
  	// 縮小シーン
 	float4 BlurColor = tex2D(BlurSampler, tex);
   	
   	// 深度記録シーン
   	float4 DepthColor  = tex2D(DepthSampler, tex);

	// 被写界深度の計算 
	float4	OutColor = 0;
	
	// 指定されたフォーカス以下なら
	if(DepthColor.r <= g_Focus)
	{
		// 何も加工せずに色排出
		OutColor = SceneColor;
	
	}else{
	
		// ぼかす比率を計算する
		float Ratio = saturate( (DepthColor.r - g_Focus) * g_BlurRate );
		
		// 中間の色を計算する
		OutColor	= lerp( SceneColor, BlurColor, Ratio );
	}

	OutColor.w = 1.0;
	
	return OutColor;
}	

///////////////////////////////////////////////
// テクニック
///////////////////////////////////////////////
technique Tec
{
    pass P0
    {   
        vertexShader = compile vs_2_0 VS_DOF();
        pixelShader  = compile ps_2_0 PS_DOF();
	}
}
