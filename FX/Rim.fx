//////////////////////////////////////////
// �O���[�o���ϐ�
//////////////////////////////////////////
float4x4	g_WVP;				// �ϊ��s��
float3		g_LightDir;			// ���̕���
float3		g_EyePos;			// ���_�ʒu
float4		g_Diffuse;			// �F
float		g_RimPower = 1.f;	// �����̋���
texture		g_Tex;				// �e�N�X�`��

//////////////////////////////////////////
// �e�N�X�`���T���v��
//////////////////////////////////////////
sampler TexSamp	= sampler_state
{
	Texture = <g_Tex>;
	
	MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//////////////////////////////////////////
// �\����
//////////////////////////////////////////
struct VS_OUT
{
	float4	Pos			: POSITION;		// �ʒu
	float4	Color		: COLOR0;		// �F
	float2	Tex			: TEXCOORD0;	// �e�N�X�`�����W
	float4	Normal		: TEXCOORD1;	// �I�u�W�F�N�g�̖@���x�N�g��
	float3	EyeDir		: TEXCOORD2;	// ���_�x�N�g��
	float	Power		: TEXCOORD3;	// �����̋���
};

//////////////////////////////////////////
// ���_�V�F�[�_
//////////////////////////////////////////
VS_OUT VS_Rim(	float4 pos		: POSITION,
				float4 normal	: NORMAL,
				float2 tex		: TEXCOORD0 )
{
	VS_OUT Out = (VS_OUT)0;
	
	// ���W�ϊ�
	Out.Pos = mul( pos, g_WVP );
	
	// �e�N�X�`���͂��̂܂�
	Out.Tex = tex;
	
	// �I�u�W�F�N�g�̖@����n��
	Out.Normal = normal;
	
	// ���C�e�B���O�v�Z
	Out.Color = dot( Out.Normal, -normalize( g_LightDir ) );
	Out.Color.a = 1.f;
	
	// ���_�x�N�g���ɕϊ�����
	float3 Eye = g_EyePos;
	if( Eye.y > 0 )
	{
		Eye.y = 0.f;
	}
	
	Out.EyeDir = normalize( Eye - pos.xyz );
	
	// ���C�g�����ʂɂ���قǃ�������������
	Out.Power = dot( Out.EyeDir, normalize( g_LightDir ) );
	
	return Out;
}

//////////////////////////////////////////
// �s�N�Z���V�F�[�_
//////////////////////////////////////////
float4 PS_Rim( VS_OUT vsin, uniform bool is_tex ) : COLOR0
{
	// ���b�V���̗֊s����
	float Power = 1.f - dot( vsin.Normal, vsin.EyeDir );
	
	Power = pow( Power * g_RimPower, 2 );
	
	// �e�N�X�`�����邩�ǂ����Ō��ʂ�ς���
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
// �e�N�j�b�N
//////////////////////////////////////////
technique Tec
{
	// �e�N�X�`���Ȃ�
	pass P0
	{
		VertexShader= compile vs_2_0 VS_Rim();
		PixelShader	= compile ps_2_0 PS_Rim( false );
	}
	
	// �e�N�X�`������
	pass P1
	{
		VertexShader= compile vs_2_0 VS_Rim();
		PixelShader	= compile ps_2_0 PS_Rim( true );
	}
}