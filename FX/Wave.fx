//////////////////////////////////////
// グローバル変数
//////////////////////////////////////
float2		g_TexMove		= { 0.007f, 0.007f };	// テクスチャの動かす量
float		g_WavePower		= 1.f;				// 波の強さ
float2		g_AddWavePos	= { 0.5f, 0.5f };	// 足しこむ波の量
float		g_AddWaveHeight = 2.f;			// 足しこむ波の高さ

texture	g_Texture;		// テクスチャ

//////////////////////////////////////
// テクスチャサンプラ
//////////////////////////////////////
sampler TexSampler = sampler_state
{
	Texture	= <g_Texture>;
	
	MinFilter	= POINT;
	MagFilter	= POINT;
	MipFilter	= NONE;
	
	AddressU	= Clamp;
	AddressV	= Clamp;
};

//////////////////////////////////////
// 構造体宣言
//////////////////////////////////////
struct VS_OUT
{
	float4	Pos	: POSITION;	// 位置
	float2	Tex	: TEXCOORD0;// テクスチャ座標
};

//////////////////////////////////////
// 何もしない頂点シェーダ
//////////////////////////////////////
VS_OUT VS_None(	float4 pos	 : POSITION,
				float2 tex	 : TEXCOORD0 )
{
	VS_OUT Out;
	
	Out.Pos = pos;
	
	Out.Tex = tex;
	
	return Out;
}

//////////////////////////////////////
// 波マップピクセルシェーダ
//////////////////////////////////////
float4 PS_WaveMap( VS_OUT vs ) : COLOR0
{
	// r = 高さ
	// g = 速度
	// として扱う
	
	// 現在の高さを取り出す
	float4 WaveHeight = tex2D( TexSampler, vs.Tex );
	
	// 一定以上の高さになれば最大まで上げる
	if( WaveHeight.r >= 0.4f )
	{
		WaveHeight.r = 1;
	}
	
	// それぞれの高さを取り出す
	float HeightAvg[ 4 ] = 
	{
		tex2D( TexSampler, vs.Tex + float2(  g_TexMove.x,			 0.f) ).r,
		tex2D( TexSampler, vs.Tex + float2(			  0.f,  g_TexMove.y ) ).r,
		tex2D( TexSampler, vs.Tex + float2( -g_TexMove.x,	 		 0.f) ).r,
		tex2D( TexSampler, vs.Tex + float2(			  0.f, -g_TexMove.y ) ).r
	};
	
	// 速度更新
	// (高さの平均－現在の波の高さ)×波の強さ＋現在の波の速さ
	float Speed = ( ( HeightAvg[ 0 ] + HeightAvg[ 1 ] + HeightAvg[ 2 ] + HeightAvg[ 3 ] ) * 0.25f - WaveHeight.r )
					 * g_WavePower + WaveHeight.g;
	
	// 高さ更新
	float Height = WaveHeight.r + Speed;
	
	// 一定以下になれば波を追加
	if( distance( vs.Tex, g_AddWavePos ) < 0.005f )
	{
		Speed += g_AddWaveHeight;
	}
	
	Height -= Height * 0.1f;
	
	// 結果を返す
	return float4( Height, Speed, 1.f, 1.f );
}

//////////////////////////////////////
// 法線マップ作成
//////////////////////////////////////
float4 PS_BumpMap( VS_OUT vs ) : COLOR0
{
	// それぞれの高さを取り出す
	float HeightAvg[ 4 ] = 
	{
		tex2D( TexSampler, vs.Tex + float2(  g_TexMove.x,			 0.f) ).r,
		tex2D( TexSampler, vs.Tex + float2(			  0.f,  g_TexMove.y ) ).r,
		tex2D( TexSampler, vs.Tex + float2( -g_TexMove.x,	 		 0.f) ).r,
		tex2D( TexSampler, vs.Tex + float2(			  0.f, -g_TexMove.y ) ).r
	};
	
	// 横方向の変化量を計算する
	// 波の情報を「-1.f～1.f」から「0.f～1.f」に変換する
	float Width = 0.5f * ( HeightAvg[ 2 ] - HeightAvg[ 0 ] ) + 0.5f;
	
	// 縦方向の変化量を計算する
	// 同じ変換をする
	float Height = 0.5f * ( HeightAvg[ 3 ] - HeightAvg[ 1 ] ) + 0.5f;
	
	// アルファ値付きで計算結果を返す
	return float4( Width, Height, 1.f, tex2D( TexSampler, vs.Tex ).r * 0.5f + 0.5f );
}

//////////////////////////////////////
// テクニック
//////////////////////////////////////
technique	Tec
{
	// 波更新
	pass P0
	{
		VertexShader= compile vs_2_0 VS_None();
		PixelShader	= compile ps_2_0 PS_WaveMap();
	}
	
	// バンプマップ作成
	pass P1
	{
		VertexShader= compile vs_2_0 VS_None();
		PixelShader	= compile ps_2_0 PS_BumpMap();
	}
}