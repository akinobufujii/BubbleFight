//////////////////////////////////////////
// グローバル変数
//////////////////////////////////////////
float4x4	g_WVP;				// 変換行列
float3		g_LightDir;			// 光の方向
float3		g_EyePos;			// 視点位置
float4		g_Diffuse;			// 色
float		g_RimPower = 1.f;	// リムの強さ
texture		g_Tex;				// テクスチャ

//////////////////////////////////////////
// テクスチャサンプラ
//////////////////////////////////////////
sampler TexSamp	= sampler_state
{
	Texture = <g_Tex>;
	
	MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//////////////////////////////////////////
// 構造体
//////////////////////////////////////////
struct VS_OUT
{
	float4	Pos			: POSITION;		// 位置
	float4	Color		: COLOR0;		// 色
	float2	Tex			: TEXCOORD0;	// テクスチャ座標
	float4	Normal		: TEXCOORD1;	// オブジェクトの法線ベクトル
	float3	EyeDir		: TEXCOORD2;	// 視点ベクトル
	float	Power		: TEXCOORD3;	// リムの強さ
};

//////////////////////////////////////////
// 頂点シェーダ
//////////////////////////////////////////
VS_OUT VS_Rim(	float4 pos		: POSITION,
				float4 normal	: NORMAL,
				float2 tex		: TEXCOORD0 )
{
	VS_OUT Out = (VS_OUT)0;
	
	// 座標変換
	Out.Pos = mul( pos, g_WVP );
	
	// テクスチャはそのまま
	Out.Tex = tex;
	
	// オブジェクトの法線を渡す
	Out.Normal = normal;
	
	// ライティング計算
	Out.Color = dot( Out.Normal, -normalize( g_LightDir ) );
	Out.Color.a = 1.f;
	
	// 視点ベクトルに変換する
	float3 Eye = g_EyePos;
	if( Eye.y > 0 )
	{
		Eye.y = 0.f;
	}
	
	Out.EyeDir = normalize( Eye - pos.xyz );
	
	// ライトが正面にあるほどリムを強くする
	Out.Power = dot( Out.EyeDir, normalize( g_LightDir ) );
	
	return Out;
}

//////////////////////////////////////////
// ピクセルシェーダ
//////////////////////////////////////////
float4 PS_Rim( VS_OUT vsin, uniform bool is_tex ) : COLOR0
{
	// メッシュの輪郭強調
	float Power = 1.f - dot( vsin.Normal, vsin.EyeDir );
	
	Power = pow( Power * g_RimPower, 2 );
	
	// テクスチャあるかどうかで結果を変える
	float4 Color;
	
	if( is_tex )
	{
		Color = tex2D( TexSamp, vsin.Tex ) * vsin.Color + vsin.Power * Power;
	
	}else{
	
		Color = g_Diffuse * vsin.Color + vsin.Power * Power;
	}
	
	Color.a = 1.f;
	
	return Color;
}

//////////////////////////////////////////
// テクニック
//////////////////////////////////////////
technique Tec
{
	// テクスチャなし
	pass P0
	{
		VertexShader= compile vs_2_0 VS_Rim();
		PixelShader	= compile ps_2_0 PS_Rim( false );
	}
	
	// テクスチャあり
	pass P1
	{
		VertexShader= compile vs_2_0 VS_Rim();
		PixelShader	= compile ps_2_0 PS_Rim( true );
	}
}