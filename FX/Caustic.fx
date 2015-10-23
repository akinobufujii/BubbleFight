///////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////
float4x4	g_World;
float4x4	g_View;
float4x4	g_Proj;
float3		g_LightDir;					// ライト方向
float		g_BrightnessPower = 10.f;	// 輝きの強さ

texture		g_Texture;		// テクスチャ
texture		g_NormalTex;	// 法線テクスチャ
texture		g_CausticTex;	// コースティックテクスチャ

///////////////////////////////////////////////
// テクスチャサンプラ
///////////////////////////////////////////////
sampler TexSampler = sampler_state
{
    Texture = <g_Texture>;
	MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

	AddressU = Wrap;
	AddressV = Wrap;
};	

sampler NormalSampler = sampler_state
{
    Texture = <g_NormalTex>;
	MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;

	AddressU = Wrap;
	AddressV = Wrap;
};	

sampler CausticSampler = sampler_state
{
    Texture = <g_CausticTex>;
	MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

	AddressU = Wrap;
	AddressV = Wrap;
};	

///////////////////////////////////////////////
// 構造体宣言
///////////////////////////////////////////////
struct VS_OUT
{
	float4	Pos		: POSITION;		// 座標
	float2	Tex		: TEXCOORD0;	// テクスチャ座標
	float3	Normal	: TEXCOORD1;	// 法線
	float4	WPos	: TEXCOORD2;	// 頂点×ワールド
};

///////////////////////////////////////////////
// 何もしない頂点シェーダ
///////////////////////////////////////////////
VS_OUT VS_None(	float4 pos : POSITION,
				float2 tex : TEXCOORD0)
{
	VS_OUT Out = (VS_OUT)0;
	
	Out.Pos = pos;
	Out.Tex = tex;

	return Out;
}

///////////////////////////////////////////////
// 法線マップからコースティックマップ作成
// ピクセルシェーダ
///////////////////////////////////////////////
float4 PS_Caustic( VS_OUT vs ) : COLOR0
{	
	// 法線計算
	float3 Normal = tex2D( NormalSampler, vs.Tex );
	
	// 「0.f～1.f」から「-1.f～1.f」に情報を変える
	Normal.x = Normal.x * 2.f - 1.f;
	Normal.y = -(Normal.y * 2.f - 1.f);
	
	// 輝度計算をする
	float BrightNess = dot( normalize( Normal ), float3( 0.0f, 0.0f, 1.0f ) );
	
	// 範囲を「0.f～1.f」に変換し
	// 影を強調する
	BrightNess = pow( BrightNess * 0.5f + 0.5f, g_BrightnessPower );
	
	// 白黒反転して返す
	return 1.f - BrightNess;
}	

///////////////////////////////////////////////
// コースティックマップを適用したライティング
// 頂点シェーダ
///////////////////////////////////////////////
VS_OUT VS_Caustic(	float4 pos		: POSITION,
					float3 normal	: NORMAL,
					float2 tex		: TEXCOORD0)
{
	VS_OUT Out = (VS_OUT)0;
	
	// 座標変換
	Out.WPos = Out.Pos = mul( pos, g_World );
	Out.Pos = mul( Out.Pos, g_View );
	Out.Pos = mul( Out.Pos, g_Proj );
	
	// テクスチャ座標
	Out.Tex = tex;
	
	// 法線
	Out.Normal = normal;

	return Out;
}

///////////////////////////////////////////////
// コースティックマップを適用したライティング
// ピクセルシェーダ
///////////////////////////////////////////////
float4 PS_Lambert( VS_OUT vs ) : COLOR0
{	
	// ランバート処理
	// 法線に対する光度計算
	float4 Normal = dot( vs.Normal, -normalize( g_LightDir ) );
	
	// コースティックマップ最終色を算出する
	float4 Color = tex2D( CausticSampler, vs.Tex ) * 1.5f + 
					Normal * tex2D( TexSampler, vs.Tex );
			
	Color.a = 1.f;
	
	return Color;
}	

///////////////////////////////////////////////
// テクニック
///////////////////////////////////////////////
technique Tec
{
    pass P0
    {   
        VertexShader = compile vs_2_0 VS_None();
        PixelShader  = compile ps_2_0 PS_Caustic();
	}
	
	pass P1
	{
		CullMode = CCW;
		
		VertexShader = compile vs_2_0 VS_Caustic();
        PixelShader  = compile ps_2_0 PS_Lambert();
	}
}
