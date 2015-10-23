///////////////////////////////////////////////
// �O���[�o���ϐ�
///////////////////////////////////////////////
float4x4	g_World;
float4x4	g_View;
float4x4	g_Proj;
float3		g_LightDir;					// ���C�g����
float		g_BrightnessPower = 10.f;	// �P���̋���

texture		g_Texture;		// �e�N�X�`��
texture		g_NormalTex;	// �@���e�N�X�`��
texture		g_CausticTex;	// �R�[�X�e�B�b�N�e�N�X�`��

///////////////////////////////////////////////
// �e�N�X�`���T���v��
///////////////////////////////////////////////
sampler TexSampler = sampler_state
{
    Texture = <g_Texture>;
	MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

	AddressU = Wrap;
	AddressV = Wrap;
};	

sampler NormalSampler = sampler_state
{
    Texture = <g_NormalTex>;
	MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;

	AddressU = Wrap;
	AddressV = Wrap;
};	

sampler CausticSampler = sampler_state
{
    Texture = <g_CausticTex>;
	MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

	AddressU = Wrap;
	AddressV = Wrap;
};	

///////////////////////////////////////////////
// �\���̐錾
///////////////////////////////////////////////
struct VS_OUT
{
	float4	Pos		: POSITION;		// ���W
	float2	Tex		: TEXCOORD0;	// �e�N�X�`�����W
	float3	Normal	: TEXCOORD1;	// �@��
	float4	WPos	: TEXCOORD2;	// ���_�~���[���h
};

///////////////////////////////////////////////
// �������Ȃ����_�V�F�[�_
///////////////////////////////////////////////
VS_OUT VS_None(	float4 pos : POSITION,
				float2 tex : TEXCOORD0)
{
	VS_OUT Out = (VS_OUT)0;
	
	Out.Pos = pos;
	Out.Tex = tex;

	return Out;
}

///////////////////////////////////////////////
// �@���}�b�v����R�[�X�e�B�b�N�}�b�v�쐬
// �s�N�Z���V�F�[�_
///////////////////////////////////////////////
float4 PS_Caustic( VS_OUT vs ) : COLOR0
{	
	// �@���v�Z
	float3 Normal = tex2D( NormalSampler, vs.Tex );
	
	// �u0.f�`1.f�v����u-1.f�`1.f�v�ɏ���ς���
	Normal.x = Normal.x * 2.f - 1.f;
	Normal.y = -(Normal.y * 2.f - 1.f);
	
	// �P�x�v�Z������
	float BrightNess = dot( normalize( Normal ), float3( 0.0f, 0.0f, 1.0f ) );
	
	// �͈͂��u0.f�`1.f�v�ɕϊ���
	// �e����������
	BrightNess = pow( BrightNess * 0.5f + 0.5f, g_BrightnessPower );
	
	// �������]���ĕԂ�
	return 1.f - BrightNess;
}	

///////////////////////////////////////////////
// �R�[�X�e�B�b�N�}�b�v��K�p�������C�e�B���O
// ���_�V�F�[�_
///////////////////////////////////////////////
VS_OUT VS_Caustic(	float4 pos		: POSITION,
					float3 normal	: NORMAL,
					float2 tex		: TEXCOORD0)
{
	VS_OUT Out = (VS_OUT)0;
	
	// ���W�ϊ�
	Out.WPos = Out.Pos = mul( pos, g_World );
	Out.Pos = mul( Out.Pos, g_View );
	Out.Pos = mul( Out.Pos, g_Proj );
	
	// �e�N�X�`�����W
	Out.Tex = tex;
	
	// �@��
	Out.Normal = normal;

	return Out;
}

///////////////////////////////////////////////
// �R�[�X�e�B�b�N�}�b�v��K�p�������C�e�B���O
// �s�N�Z���V�F�[�_
///////////////////////////////////////////////
float4 PS_Lambert( VS_OUT vs ) : COLOR0
{	
	// �����o�[�g����
	// �@���ɑ΂�����x�v�Z
	float4 Normal = dot( vs.Normal, -normalize( g_LightDir ) );
	
	// �R�[�X�e�B�b�N�}�b�v�ŏI�F���Z�o����
	float4 Color = tex2D( CausticSampler, vs.Tex ) * 1.5f + 
					Normal * tex2D( TexSampler, vs.Tex );
			
	Color.a = 1.f;
	
	return Color;
}	

///////////////////////////////////////////////
// �e�N�j�b�N
///////////////////////////////////////////////
technique Tec
{
    pass P0
    {   
        VertexShader = compile vs_2_0 VS_None();
        PixelShader  = compile ps_2_0 PS_Caustic();
	}
	
	pass P1
	{
		CullMode = CCW;
		
		VertexShader = compile vs_2_0 VS_Caustic();
        PixelShader  = compile ps_2_0 PS_Lambert();
	}
}
