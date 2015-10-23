//////////////////////////////////////////
// グローバル変数
//////////////////////////////////////////
float2	g_CenterTexPos;		// 爆発の中心となる位置
float2	g_TexOffset;		// オフセット(1.f/画面サイズ)
float	g_BlurPower = 10.f;	// ブラーの強さ
float	g_AddColorRate = 1;	// 掛ける色倍率	

texture	g_Tex;				// シーンのテクスチャ

//////////////////////////////////////////
// テクスチャサンプラ
//////////////////////////////////////////
sampler TexSampler = sampler_state
{
	Texture = <g_Tex>;
	
	MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

	AddressU = Clamp;
	AddressV = Clamp;
};

//////////////////////////////////////////
// 構造体宣言
//////////////////////////////////////////
struct VS_OUT
{
	float4	Pos	: POSITION;		// 位置
	float2	Tex	: TEXCOORD0;	// テクセル
};

//////////////////////////////////////////
// 頂点シェーダ
//////////////////////////////////////////
VS_OUT VS_None( float4 pos : POSITION,
				float2 tex : TEXCOORD0 )
{
	VS_OUT Out;
	
	Out.Pos = pos;
	Out.Tex = tex;
	
	return Out;
}

//////////////////////////////////////////
// ピクセルシェーダ
//////////////////////////////////////////
float4	PS_BombBlur( VS_OUT vs ) : COLOR0
{	
	// 指定されたい位置から現在のテクセル座標への方向を見る
	float2 Dir = g_CenterTexPos - vs.Tex;
	
	// 長さと方向を抽出する
	float Length = length( Dir );
	Dir = normalize( Dir ) * g_TexOffset;
	
	// ブラーの強さと長さを掛け合わせる
	Dir *= Length * g_BlurPower;

	// 足し合わせる
	// 全て足して1.fになるようにするが、かなり直書き
	float4 Color = tex2D( TexSampler, vs.Tex ) * 0.19f;
	
	for( int i = 1; i < 10; i++ )
	{
		Color += tex2D( TexSampler, vs.Tex + Dir * i ) * (0.19f - 0.02f * i);
	}	
	
	return Color * g_AddColorRate;
}

//////////////////////////////////////////
// テクニック
//////////////////////////////////////////
technique Tec
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS_None();
		PixelShader	 = compile ps_2_0 PS_BombBlur();
	}
}