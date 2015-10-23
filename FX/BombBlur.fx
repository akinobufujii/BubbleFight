//////////////////////////////////////////
// �O���[�o���ϐ�
//////////////////////////////////////////
float2	g_CenterTexPos;		// �����̒��S�ƂȂ�ʒu
float2	g_TexOffset;		// �I�t�Z�b�g(1.f/��ʃT�C�Y)
float	g_BlurPower = 10.f;	// �u���[�̋���
float	g_AddColorRate = 1;	// �|����F�{��	

texture	g_Tex;				// �V�[���̃e�N�X�`��

//////////////////////////////////////////
// �e�N�X�`���T���v��
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
// �\���̐錾
//////////////////////////////////////////
struct VS_OUT
{
	float4	Pos	: POSITION;		// �ʒu
	float2	Tex	: TEXCOORD0;	// �e�N�Z��
};

//////////////////////////////////////////
// ���_�V�F�[�_
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
// �s�N�Z���V�F�[�_
//////////////////////////////////////////
float4	PS_BombBlur( VS_OUT vs ) : COLOR0
{	
	// �w�肳�ꂽ���ʒu���猻�݂̃e�N�Z�����W�ւ̕���������
	float2 Dir = g_CenterTexPos - vs.Tex;
	
	// �����ƕ����𒊏o����
	float Length = length( Dir );
	Dir = normalize( Dir ) * g_TexOffset;
	
	// �u���[�̋����ƒ������|�����킹��
	Dir *= Length * g_BlurPower;

	// �������킹��
	// �S�đ�����1.f�ɂȂ�悤�ɂ��邪�A���Ȃ蒼����
	float4 Color = tex2D( TexSampler, vs.Tex ) * 0.19f;
	
	for( int i = 1; i < 10; i++ )
	{
		Color += tex2D( TexSampler, vs.Tex + Dir * i ) * (0.19f - 0.02f * i);
	}	
	
	return Color * g_AddColorRate;
}

//////////////////////////////////////////
// �e�N�j�b�N
//////////////////////////////////////////
technique Tec
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS_None();
		PixelShader	 = compile ps_2_0 PS_BombBlur();
	}
}