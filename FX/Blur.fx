///////////////////////////////////////////////
// �O���[�o���ϐ�
///////////////////////////////////////////////
float2		g_Offset;			// �e�N�X�`���T�C�Y
float		g_BlurLength = 1.f;

texture 	g_Texture;

///////////////////////////////////////////////
// �e�N�X�`���T���v��
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
// �\���̐錾
///////////////////////////////////////////////
struct VS_OUT
{
	float4	Pos	:POSITION;	// ���W
	float2	Tex	:TEXCOORD0;	// �e�N�X�`�����W	
};

///////////////////////////////////////////////
// ���_�V�F�[�_�[
///////////////////////////////////////////////
VS_OUT VS_None( float4 pos : POSITION0,
				float2 tex : TEXCOORD0)
{
	VS_OUT Out;
	
	// �������Ȃ��œn��
	Out.Pos = pos;
	Out.Tex = tex;
	
	return Out;
}

///////////////////////////////////////////////
// �s�N�Z���V�F�[�_�[
///////////////////////////////////////////////
float4 PS_Blur( VS_OUT vs, uniform bool is_blur ): COLOR0
{	
	// �e�N�X�`���̃s�N�Z���␳
	vs.Tex.x += g_Offset.x;
	vs.Tex.y += g_Offset.y;

	// ���炩���ߌv�Z����
	float X, Y;
	
	// �u���[��������Ȃ�
	if( is_blur )
	{
		X = g_BlurLength * g_Offset.x;
		Y = g_BlurLength * g_Offset.y;
		
	}else{
	
		X = 0.5f * g_Offset.x;
		Y = 0.5f * g_Offset.y;
	}
		
	float2 Tex[ 4 ];
	
	//����
	Tex[ 0 ].x = vs.Tex.x - X;
	Tex[ 0 ].y = vs.Tex.y - Y;

	//�E��
	Tex[ 1 ].x = vs.Tex.x + X;
	Tex[ 1 ].y = vs.Tex.y - Y;

	//�E��
	Tex[ 2 ].x = vs.Tex.x + X;
	Tex[ 2 ].y = vs.Tex.y + Y;

	//����
	Tex[ 3 ].x = vs.Tex.x - X;
	Tex[ 3 ].y = vs.Tex.y + Y;

	// �{���͌J��Ԃ����������������������̂��߂ɑ�������	
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
// �e�N�j�b�N
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
