//////////////////////////////////////
// �O���[�o���ϐ�
//////////////////////////////////////
float4x4 	g_World;
float4x4 	g_WorldInv;
float4x4 	g_View;
float4x4 	g_Proj;
float3		g_LightDir;		// ���C�g����
float		g_Height = -3;	// ����
float3		g_FogColor = { 0, 0.8, 1 };
float		g_FogStart	= 15.f;
float		g_FogEnd= 50.f;

texture		g_Texture;
texture		g_NormalTex;	// �@���e�N�X�`��

//////////////////////////////////////
// �e�N�X�`���T���v��
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
// �\���̐錾
//////////////////////////////////////
struct VS_OUT
{
	float4	Pos			:POSITION;	// ���W
	float2	Tex			:TEXCOORD0;	// �e�N�X�`�����W
	float3	LightDir	:TEXCOORD1;	// ���C�g����
	float4	VPos		:TEXCOORD2;	// ���_�̈ʒu
	float4	Color		:COLOR0;	// �F
};

//////////////////////////////////////
// ���_�V�F�[�_(�|���S���p)
//////////////////////////////////////
VS_OUT VS_Bump( float4 pos : POSITION,
				float2 tex : TEXCOORD0 )
{
	VS_OUT Out;
	
	// ���W�ϊ�
	Out.VPos = mul( mul( pos, g_World ), g_View );
	Out.Pos  = mul( Out.VPos, g_Proj );

	// �e�N�X�`���͂��̂܂�
	Out.Tex = tex;
	
	// ���C�g�����v�Z
	Out.LightDir = normalize( mul(g_LightDir, g_WorldInv) ).xyz;
	
	// �F�v�Z���Ȃ�
	Out.Color = 1;
	
	return Out;
}

//////////////////////////////////////
// ���_�V�F�[�_(���b�V���p)
//////////////////////////////////////
VS_OUT VS_BumpMesh( float4 pos		: POSITION,
					float3 normal	: NORMAL0,
					float2 tex		: TEXCOORD0,
					float3 tangent	: TANGENT0 )
{
	VS_OUT Out;
	
	// ���W�ϊ�
	Out.VPos = mul( mul( pos, g_World ), g_View );
	Out.Pos  = mul( Out.VPos, g_Proj );

	// �e�N�X�`���͂��̂܂�
	Out.Tex = tex;
	
	// ���_���W�ւ̕ϊ�(xyz�x�N�g����3�̔z��ƍl����)
	float3x3 mBump;
	
	mBump[ 0 ] = tangent;					// �ڐ�x�N�g��
	mBump[ 1 ] = cross( normal, tangent );	// �]�@���x�N�g��
	mBump[ 2 ] = normal;					// �@���x�N�g��
	
	// ���C�g�����v�Z
	float4 LocalLightDir = normalize( mul(g_LightDir, g_WorldInv) );
	
	Out.LightDir = mul(LocalLightDir, transpose( mBump ) );
	
	// �F�v�Z���Ȃ�
	Out.Color = 1;
	
	return Out;
}

//////////////////////////////////////
// �t�H�O�t���s�N�Z���V�F�[�_
//////////////////////////////////////
float4 PS_Bump( VS_OUT vs, uniform bool is_board ): COLOR0
{
	// �@���}�b�v�̖@���x�N�g�����擾����
	float3 Normal = tex2D(NormalSampler, vs.Tex).xyz;
	
	// �@���}�b�v�̒l�̕␳(���˂Ȃ̂Ńx�N�g���͔��]������)
	Normal = 2.f * Normal - 1.f;
	
	// �����|���S���Ȃ�-1��������
	if( is_board )
	{
		Normal *= -1;
	}
	
	// �n���ꂽ������������ω�������
	Normal.y += g_Height;
	
	// �@��������ς��v�Z���A�F���v�Z
	float4 Color = vs.Color * tex2D(TextureSampler, vs.Tex) * dot(vs.LightDir, Normal);
	
	// �����v�Z
	float FogDepth = max( 0.0, (vs.VPos.z - g_FogStart) / g_FogEnd );
	
	return (Color + pow(FogDepth, 2)) * float4( g_FogColor, 1.f );
}

//////////////////////////////////////
// �e�N�j�b�N
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
