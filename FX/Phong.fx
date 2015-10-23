//////////////////////////////////////////
// グローバル変数
//////////////////////////////////////////
float4x4	g_WVP;						// 変換行列
float3		g_LightDir;					// 光の方向
float3		g_EyePos;					// 視点位置
float4		g_Diffuse;					// 色
float		g_SpecularLimit = 15.f;		// スペキュラ範囲
float		g_SpecularPower = 2.f;		// スペキュラ強さ
float4		g_AddColor = { 0, 0, 0, 0 };// 加算する色(エフェクト用)
texture		g_Tex;						// テクスチャ

//////////////////////////////////////////
// テクスチャサンプラ
//////////////////////////////////////////
sampler TexSamp	= sampler_state
{
	Texture = <g_Tex>;
	
	MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

	AddressU = Clamp;
	AddressV = Clamp;
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
	float3	LightDir	: TEXCOORD2;	// ライト位置 ベクトル
	float3	EyeDir		: TEXCOORD3;	// 視点 ベクトル
};

//////////////////////////////////////////
// 頂点シェーダ
//////////////////////////////////////////
VS_OUT VS_Phon(	float4 pos		: POSITION,
				float4 normal	: NORMAL,
				float2 tex		: TEXCOORD0 )
{
	VS_OUT Out = (VS_OUT)0;
	
	// 座標変換
	Out.Pos = mul( pos, g_WVP );
	
	// テクスチャはそのまま
	Out.Tex = tex;
	
	// ライト方向の向きを反転し正規化してから渡す
	Out.LightDir = -normalize( g_LightDir );
	
	// オブジェクトの法線を渡す
	Out.Normal = normal;
	
	// スペキュラ計算
	Out.Color = min( dot( Out.Normal, Out.LightDir ), 1.0f );
	Out.Color.a = 1.f;
	
	// 視点ベクトルに変換する
	Out.EyeDir = normalize( g_EyePos - pos.xyz );
	
	return Out;
}

//////////////////////////////////////////
// ピクセルシェーダ
//////////////////////////////////////////
float4 PS_Phon( VS_OUT vsin, uniform bool is_tex ) : COLOR0
{
	// 視点ベクトル計算
	float3 EyeDir = normalize( vsin.LightDir + vsin.EyeDir );
	
	// スペキュラカラー計算
	float Specular = pow( dot( vsin.Normal, EyeDir ), g_SpecularLimit ) * g_SpecularPower;
	
	// テクスチャあるかどうかで結果を変える
	if( is_tex )
	{
		return tex2D( TexSamp, vsin.Tex ) * vsin.Color + g_AddColor + Specular;
		
	}else{
	
		return g_Diffuse * vsin.Color + Specular;
	}
}

//////////////////////////////////////////
// テクニック
//////////////////////////////////////////
technique Tec
{
	// テクスチャなし
	pass P0
	{
		VertexShader= compile vs_2_0 VS_Phon();
		PixelShader	= compile ps_2_0 PS_Phon( false );
	}
	
	// テクスチャあり
	pass P1
	{
		VertexShader= compile vs_2_0 VS_Phon();
		PixelShader	= compile ps_2_0 PS_Phon( true );
	}
}