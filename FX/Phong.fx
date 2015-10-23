//////////////////////////////////////////
// �O���[�o���ϐ�
//////////////////////////////////////////
float4x4	g_WVP;						// �ϊ��s��
float3		g_LightDir;					// ���̕���
float3		g_EyePos;					// ���_�ʒu
float4		g_Diffuse;					// �F
float		g_SpecularLimit = 15.f;		// �X�y�L�����͈�
float		g_SpecularPower = 2.f;		// �X�y�L��������
float4		g_AddColor = { 0, 0, 0, 0 };// ���Z����F(�G�t�F�N�g�p)
texture		g_Tex;						// �e�N�X�`��

//////////////////////////////////////////
// �e�N�X�`���T���v��
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
// �\����
//////////////////////////////////////////
struct VS_OUT
{
	float4	Pos			: POSITION;		// �ʒu
	float4	Color		: COLOR0;		// �F
	float2	Tex			: TEXCOORD0;	// �e�N�X�`�����W
	float4	Normal		: TEXCOORD1;	// �I�u�W�F�N�g�̖@���x�N�g��
	float3	LightDir	: TEXCOORD2;	// ���C�g�ʒu �x�N�g��
	float3	EyeDir		: TEXCOORD3;	// ���_ �x�N�g��
};

//////////////////////////////////////////
// ���_�V�F�[�_
//////////////////////////////////////////
VS_OUT VS_Phon(	float4 pos		: POSITION,
				float4 normal	: NORMAL,
				float2 tex		: TEXCOORD0 )
{
	VS_OUT Out = (VS_OUT)0;
	
	// ���W�ϊ�
	Out.Pos = mul( pos, g_WVP );
	
	// �e�N�X�`���͂��̂܂�
	Out.Tex = tex;
	
	// ���C�g�����̌����𔽓]�����K�����Ă���n��
	Out.LightDir = -normalize( g_LightDir );
	
	// �I�u�W�F�N�g�̖@����n��
	Out.Normal = normal;
	
	// �X�y�L�����v�Z
	Out.Color = min( dot( Out.Normal, Out.LightDir ), 1.0f );
	Out.Color.a = 1.f;
	
	// ���_�x�N�g���ɕϊ�����
	Out.EyeDir = normalize( g_EyePos - pos.xyz );
	
	return Out;
}

//////////////////////////////////////////
// �s�N�Z���V�F�[�_
//////////////////////////////////////////
float4 PS_Phon( VS_OUT vsin, uniform bool is_tex ) : COLOR0
{
	// ���_�x�N�g���v�Z
	float3 EyeDir = normalize( vsin.LightDir + vsin.EyeDir );
	
	// �X�y�L�����J���[�v�Z
	float Specular = pow( dot( vsin.Normal, EyeDir ), g_SpecularLimit ) * g_SpecularPower;
	
	// �e�N�X�`�����邩�ǂ����Ō��ʂ�ς���
	if( is_tex )
	{
		return tex2D( TexSamp, vsin.Tex ) * vsin.Color + g_AddColor + Specular;
		
	}else{
	
		return g_Diffuse * vsin.Color + Specular;
	}
}

//////////////////////////////////////////
// �e�N�j�b�N
//////////////////////////////////////////
technique Tec
{
	// �e�N�X�`���Ȃ�
	pass P0
	{
		VertexShader= compile vs_2_0 VS_Phon();
		PixelShader	= compile ps_2_0 PS_Phon( false );
	}
	
	// �e�N�X�`������
	pass P1
	{
		VertexShader= compile vs_2_0 VS_Phon();
		PixelShader	= compile ps_2_0 PS_Phon( true );
	}
}