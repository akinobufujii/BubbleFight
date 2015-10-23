/////////////////////////////////
// グローバル変数
/////////////////////////////////
float4x4	g_VP;				// カメラ×投影
float		g_PointC;			// 距離
float		g_ViewHeight = 600;	// バックバッファの高さ
texture		g_Texture;			// テクスチャ

/////////////////////////////////
// テクスチャサンプラ
/////////////////////////////////
sampler TextureSampler = sampler_state
{
	Texture = <g_Texture>;
	
	MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

	AddressU = Wrap;
	AddressV = Wrap;
};

/////////////////////////////////
// 構造体宣言
/////////////////////////////////
struct VS_OUT
{
	float4	Pos 	: POSITION;	// 位置
	float2	Tex 	: TEXCOORD0;// テクスチャ座標
	float	Size	: PSIZE;	// ポイントスプライトの大きさ
	float4	Color	: COLOR0;	// 色
};

/////////////////////////////////
// 頂点シェーダ
/////////////////////////////////
VS_OUT VS_PointSprite(  float4	pos		: POSITION,
						float	size	: PSIZE,
						float4	color	: COLOR0 )
{
	VS_OUT Out;

	// 座標変換
	Out.Pos = mul( pos, g_VP );

	// 固定パイプライを使わないので
	// 点の大きさを手計算
	// まずは視点からの距離を出す
	float Distance = sqrt(pow(Out.Pos.x, 2) + pow(Out.Pos.y, 2) + pow(Out.Pos.z, 2));
	
	// 本当の計算式は
	// sqrt(1 / (A + B * Distance + C * pow(Distance, 2))を
	// バックバッファの高さ*点の大きさと掛け合わせるのだが
	// AとBは0で使わない＋Cで事足りるので計算を省く
	Out.Size = g_ViewHeight * size * sqrt( 1.f / (pow( Distance, 2 ) * g_PointC) );
	
	// 色はそのまま
	Out.Color= color;
	
	// テクセル座標は勝手に埋められる
	Out.Tex  = 0;

	return Out;
}

/////////////////////////////////
// ピクセルシェーダ
/////////////////////////////////
float4 PS_PointSprite( VS_OUT vs ) : COLOR0
{
	// 色出力
	return tex2D( TextureSampler, vs.Tex ) * vs.Color;
}

/////////////////////////////////
// テクニック
/////////////////////////////////
technique Tec
{
	pass P0
	{
		// ポイントスプライト設定
		POINTSPRITEENABLE	= TRUE;
		POINTSCALEENABLE	= TRUE;
		POINTSIZE_MIN		= 0.0;
		
		// アルファブレンド設定
		ALPHABLENDENABLE	= TRUE;
		SRCBLEND			= SRCALPHA;
		DESTBLEND			= ONE;
		
		// 深度書き込みさせない	    
		ZWRITEENABLE		= FALSE;
		ZENABLE				= FALSE;
		
		VertexShader = compile vs_2_0	VS_PointSprite();
		PixelShader  = compile ps_2_0	PS_PointSprite();
	}
}