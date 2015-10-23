//////////////////////////////////////
// �O���[�o���ϐ�
//////////////////////////////////////
float2		g_TexMove		= { 0.007f, 0.007f };	// �e�N�X�`���̓�������
float		g_WavePower		= 1.f;				// �g�̋���
float2		g_AddWavePos	= { 0.5f, 0.5f };	// �������ޔg�̗�
float		g_AddWaveHeight = 2.f;			// �������ޔg�̍���

texture	g_Texture;		// �e�N�X�`��

//////////////////////////////////////
// �e�N�X�`���T���v��
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
// �\���̐錾
//////////////////////////////////////
struct VS_OUT
{
	float4	Pos	: POSITION;	// �ʒu
	float2	Tex	: TEXCOORD0;// �e�N�X�`�����W
};

//////////////////////////////////////
// �������Ȃ����_�V�F�[�_
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
// �g�}�b�v�s�N�Z���V�F�[�_
//////////////////////////////////////
float4 PS_WaveMap( VS_OUT vs ) : COLOR0
{
	// r = ����
	// g = ���x
	// �Ƃ��Ĉ���
	
	// ���݂̍��������o��
	float4 WaveHeight = tex2D( TexSampler, vs.Tex );
	
	// ���ȏ�̍����ɂȂ�΍ő�܂ŏグ��
	if( WaveHeight.r >= 0.4f )
	{
		WaveHeight.r = 1;
	}
	
	// ���ꂼ��̍��������o��
	float HeightAvg[ 4 ] = 
	{
		tex2D( TexSampler, vs.Tex + float2(  g_TexMove.x,			 0.f) ).r,
		tex2D( TexSampler, vs.Tex + float2(			  0.f,  g_TexMove.y ) ).r,
		tex2D( TexSampler, vs.Tex + float2( -g_TexMove.x,	 		 0.f) ).r,
		tex2D( TexSampler, vs.Tex + float2(			  0.f, -g_TexMove.y ) ).r
	};
	
	// ���x�X�V
	// (�����̕��ρ|���݂̔g�̍���)�~�g�̋����{���݂̔g�̑���
	float Speed = ( ( HeightAvg[ 0 ] + HeightAvg[ 1 ] + HeightAvg[ 2 ] + HeightAvg[ 3 ] ) * 0.25f - WaveHeight.r )
					 * g_WavePower + WaveHeight.g;
	
	// �����X�V
	float Height = WaveHeight.r + Speed;
	
	// ���ȉ��ɂȂ�Δg��ǉ�
	if( distance( vs.Tex, g_AddWavePos ) < 0.005f )
	{
		Speed += g_AddWaveHeight;
	}
	
	Height -= Height * 0.1f;
	
	// ���ʂ�Ԃ�
	return float4( Height, Speed, 1.f, 1.f );
}

//////////////////////////////////////
// �@���}�b�v�쐬
//////////////////////////////////////
float4 PS_BumpMap( VS_OUT vs ) : COLOR0
{
	// ���ꂼ��̍��������o��
	float HeightAvg[ 4 ] = 
	{
		tex2D( TexSampler, vs.Tex + float2(  g_TexMove.x,			 0.f) ).r,
		tex2D( TexSampler, vs.Tex + float2(			  0.f,  g_TexMove.y ) ).r,
		tex2D( TexSampler, vs.Tex + float2( -g_TexMove.x,	 		 0.f) ).r,
		tex2D( TexSampler, vs.Tex + float2(			  0.f, -g_TexMove.y ) ).r
	};
	
	// �������̕ω��ʂ��v�Z����
	// �g�̏����u-1.f�`1.f�v����u0.f�`1.f�v�ɕϊ�����
	float Width = 0.5f * ( HeightAvg[ 2 ] - HeightAvg[ 0 ] ) + 0.5f;
	
	// �c�����̕ω��ʂ��v�Z����
	// �����ϊ�������
	float Height = 0.5f * ( HeightAvg[ 3 ] - HeightAvg[ 1 ] ) + 0.5f;
	
	// �A���t�@�l�t���Ōv�Z���ʂ�Ԃ�
	return float4( Width, Height, 1.f, tex2D( TexSampler, vs.Tex ).r * 0.5f + 0.5f );
}

//////////////////////////////////////
// �e�N�j�b�N
//////////////////////////////////////
technique	Tec
{
	// �g�X�V
	pass P0
	{
		VertexShader= compile vs_2_0 VS_None();
		PixelShader	= compile ps_2_0 PS_WaveMap();
	}
	
	// �o���v�}�b�v�쐬
	pass P1
	{
		VertexShader= compile vs_2_0 VS_None();
		PixelShader	= compile ps_2_0 PS_BumpMap();
	}
}