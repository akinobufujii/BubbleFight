///////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////
float2		g_Offset;			// テクスチャサイズ
float		g_BlurLength = 1.f;

texture 	g_Texture;

///////////////////////////////////////////////
// テクスチャサンプラ
///////////////////////////////////////////////
sampler TextureSampler = sampler_state
{
    Texture = <g_Texture>;
    
    MinFilter = LINEAR;
    MagFilter = LINEAR;

	AddressU =Clamp;
	AddressV =Clamp;
};	

///////////////////////////////////////////////
// 構造体宣言
///////////////////////////////////////////////
struct VS_OUT
{
	float4	Pos	:POSITION;	// 座標
	float2	Tex	:TEXCOORD0;	// テクスチャ座標	
};

///////////////////////////////////////////////
// 頂点シェーダー
///////////////////////////////////////////////
VS_OUT VS_None( float4 pos : POSITION0,
				float2 tex : TEXCOORD0)
{
	VS_OUT Out;
	
	// 何もしないで渡す
	Out.Pos = pos;
	Out.Tex = tex;
	
	return Out;
}

///////////////////////////////////////////////
// ピクセルシェーダー
///////////////////////////////////////////////
float4 PS_Blur( VS_OUT vs, uniform bool is_blur ): COLOR0
{	
	// テクスチャのピクセル補正
	vs.Tex.x += g_Offset.x;
	vs.Tex.y += g_Offset.y;

	// あらかじめ計算する
	float X, Y;
	
	// ブラーをかけるなら
	if( is_blur )
	{
		X = g_BlurLength * g_Offset.x;
		Y = g_BlurLength * g_Offset.y;
		
	}else{
	
		X = 0.5f * g_Offset.x;
		Y = 0.5f * g_Offset.y;
	}
		
	float2 Tex[ 4 ];
	
	//左上
	Tex[ 0 ].x = vs.Tex.x - X;
	Tex[ 0 ].y = vs.Tex.y - Y;

	//右上
	Tex[ 1 ].x = vs.Tex.x + X;
	Tex[ 1 ].y = vs.Tex.y - Y;

	//右下
	Tex[ 2 ].x = vs.Tex.x + X;
	Tex[ 2 ].y = vs.Tex.y + Y;

	//左下
	Tex[ 3 ].x = vs.Tex.x - X;
	Tex[ 3 ].y = vs.Tex.y + Y;

	// 本当は繰り返し処理をしたいが高速化のために足しこむ	
	float4 TexColor =	  tex2D( TextureSampler, Tex[ 0 ] )
						+ tex2D( TextureSampler, Tex[ 1 ] )
						+ tex2D( TextureSampler, Tex[ 2 ] )
						+ tex2D( TextureSampler, Tex[ 3 ] );
 
	//for( int i = 0; i < 4; i++ )
	//{
		//TexColor += tex2D( TextureSampler, Tex[ i ] );
	//}
	
	return TexColor * 0.25;
}


///////////////////////////////////////////////
// テクニック
///////////////////////////////////////////////
technique Tec
{
	pass P0
	{
		vertexShader = compile vs_2_0 VS_None();
        pixelShader  = compile ps_2_0 PS_Blur( false );
	}
	
    pass P1
    {
        vertexShader = compile vs_2_0 VS_None();
        pixelShader  = compile ps_2_0 PS_Blur( true );
	}
}
