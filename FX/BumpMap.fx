//////////////////////////////////////
// グローバル変数
//////////////////////////////////////
float4x4 	g_World;
float4x4 	g_WorldInv;
float4x4 	g_View;
float4x4 	g_Proj;
float3		g_LightDir;		// ライト方向
float		g_Height = -3;	// 高さ
float3		g_FogColor = { 0, 0.8, 1 };
float		g_FogStart	= 15.f;
float		g_FogEnd= 50.f;

texture		g_Texture;
texture		g_NormalTex;	// 法線テクスチャ

//////////////////////////////////////
// テクスチャサンプラ
//////////////////////////////////////
sampler TextureSampler = sampler_state
{
    Texture = <g_Texture>;
	
	MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

	AddressU =Wrap;
	AddressV =Wrap;
};	

sampler NormalSampler = sampler_state
{
    Texture = <g_NormalTex>;
	
	MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

	AddressU =Wrap;
	AddressV =Wrap;
};	

//////////////////////////////////////
// 構造体宣言
//////////////////////////////////////
struct VS_OUT
{
	float4	Pos			:POSITION;	// 座標
	float2	Tex			:TEXCOORD0;	// テクスチャ座標
	float3	LightDir	:TEXCOORD1;	// ライト方向
	float4	VPos		:TEXCOORD2;	// 視点の位置
	float4	Color		:COLOR0;	// 色
};

//////////////////////////////////////
// 頂点シェーダ(板ポリゴン用)
//////////////////////////////////////
VS_OUT VS_Bump( float4 pos : POSITION,
				float2 tex : TEXCOORD0 )
{
	VS_OUT Out;
	
	// 座標変換
	Out.VPos = mul( mul( pos, g_World ), g_View );
	Out.Pos  = mul( Out.VPos, g_Proj );

	// テクスチャはそのまま
	Out.Tex = tex;
	
	// ライト方向計算
	Out.LightDir = normalize( mul(g_LightDir, g_WorldInv) ).xyz;
	
	// 色計算しない
	Out.Color = 1;
	
	return Out;
}

//////////////////////////////////////
// 頂点シェーダ(メッシュ用)
//////////////////////////////////////
VS_OUT VS_BumpMesh( float4 pos		: POSITION,
					float3 normal	: NORMAL0,
					float2 tex		: TEXCOORD0,
					float3 tangent	: TANGENT0 )
{
	VS_OUT Out;
	
	// 座標変換
	Out.VPos = mul( mul( pos, g_World ), g_View );
	Out.Pos  = mul( Out.VPos, g_Proj );

	// テクスチャはそのまま
	Out.Tex = tex;
	
	// 頂点座標への変換(xyzベクトルの3つの配列と考える)
	float3x3 mBump;
	
	mBump[ 0 ] = tangent;					// 接戦ベクトル
	mBump[ 1 ] = cross( normal, tangent );	// 従法線ベクトル
	mBump[ 2 ] = normal;					// 法線ベクトル
	
	// ライト方向計算
	float4 LocalLightDir = normalize( mul(g_LightDir, g_WorldInv) );
	
	Out.LightDir = mul(LocalLightDir, transpose( mBump ) );
	
	// 色計算しない
	Out.Color = 1;
	
	return Out;
}

//////////////////////////////////////
// フォグ付きピクセルシェーダ
//////////////////////////////////////
float4 PS_Bump( VS_OUT vs, uniform bool is_board ): COLOR0
{
	// 法線マップの法線ベクトルを取得する
	float3 Normal = tex2D(NormalSampler, vs.Tex).xyz;
	
	// 法線マップの値の補正(反射なのでベクトルは反転させる)
	Normal = 2.f * Normal - 1.f;
	
	// もし板ポリゴンなら-1をかける
	if( is_board )
	{
		Normal *= -1;
	}
	
	// 渡された分だけ高さを変化させる
	Normal.y += g_Height;
	
	// 法線から内積を計算し、色を計算
	float4 Color = vs.Color * tex2D(TextureSampler, vs.Tex) * dot(vs.LightDir, Normal);
	
	// 距離計算
	float FogDepth = max( 0.0, (vs.VPos.z - g_FogStart) / g_FogEnd );
	
	return (Color + pow(FogDepth, 2)) * float4( g_FogColor, 1.f );
}

//////////////////////////////////////
// テクニック
//////////////////////////////////////
technique Tec
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS_Bump();
        PixelShader  = compile ps_2_0 PS_Bump( true );
	}
	
	pass P1
	{
		VertexShader = compile vs_2_0 VS_BumpMesh();
        PixelShader  = compile ps_2_0 PS_Bump( false );
	}
}
