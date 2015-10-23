/////////////////////////////////
// �O���[�o���ϐ�
/////////////////////////////////
float4x4	g_VP;				// �J�����~���e
float		g_PointC;			// ����
float		g_ViewHeight = 600;	// �o�b�N�o�b�t�@�̍���
texture		g_Texture;			// �e�N�X�`��

/////////////////////////////////
// �e�N�X�`���T���v��
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
// �\���̐錾
/////////////////////////////////
struct VS_OUT
{
	float4	Pos 	: POSITION;	// �ʒu
	float2	Tex 	: TEXCOORD0;// �e�N�X�`�����W
	float	Size	: PSIZE;	// �|�C���g�X�v���C�g�̑傫��
	float4	Color	: COLOR0;	// �F
};

/////////////////////////////////
// ���_�V�F�[�_
/////////////////////////////////
VS_OUT VS_PointSprite(  float4	pos		: POSITION,
						float	size	: PSIZE,
						float4	color	: COLOR0 )
{
	VS_OUT Out;

	// ���W�ϊ�
	Out.Pos = mul( pos, g_VP );

	// �Œ�p�C�v���C���g��Ȃ��̂�
	// �_�̑傫������v�Z
	// �܂��͎��_����̋������o��
	float Distance = sqrt(pow(Out.Pos.x, 2) + pow(Out.Pos.y, 2) + pow(Out.Pos.z, 2));
	
	// �{���̌v�Z����
	// sqrt(1 / (A + B * Distance + C * pow(Distance, 2))��
	// �o�b�N�o�b�t�@�̍���*�_�̑傫���Ɗ|�����킹��̂���
	// A��B��0�Ŏg��Ȃ��{C�Ŏ������̂Ōv�Z���Ȃ�
	Out.Size = g_ViewHeight * size * sqrt( 1.f / (pow( Distance, 2 ) * g_PointC) );
	
	// �F�͂��̂܂�
	Out.Color= color;
	
	// �e�N�Z�����W�͏���ɖ��߂���
	Out.Tex  = 0;

	return Out;
}

/////////////////////////////////
// �s�N�Z���V�F�[�_
/////////////////////////////////
float4 PS_PointSprite( VS_OUT vs ) : COLOR0
{
	// �F�o��
	return tex2D( TextureSampler, vs.Tex ) * vs.Color;
}

/////////////////////////////////
// �e�N�j�b�N
/////////////////////////////////
technique Tec
{
	pass P0
	{
		// �|�C���g�X�v���C�g�ݒ�
		POINTSPRITEENABLE	= TRUE;
		POINTSCALEENABLE	= TRUE;
		POINTSIZE_MIN		= 0.0;
		
		// �A���t�@�u�����h�ݒ�
		ALPHABLENDENABLE	= TRUE;
		SRCBLEND			= SRCALPHA;
		DESTBLEND			= ONE;
		
		// �[�x�������݂����Ȃ�	    
		ZWRITEENABLE		= FALSE;
		ZENABLE				= FALSE;
		
		VertexShader = compile vs_2_0	VS_PointSprite();
		PixelShader  = compile ps_2_0	PS_PointSprite();
	}
}