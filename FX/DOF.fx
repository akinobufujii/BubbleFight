///////////////////////////////////////////////
// �O���[�o���ϐ�
///////////////////////////////////////////////
float		g_Focus = 0.7f;

float		g_Width = 800;
float		g_Height= 600;
float		g_BlurRate	= 1.f;	// �ڂ�������

texture 	g_SceneTex;
texture 	g_BlurTex;
texture 	g_DepthTex;

///////////////////////////////////////////////
// �e�N�X�`���T���v��
///////////////////////////////////////////////
sampler SceneSampler = 
sampler_state
{
    Texture = <g_SceneTex>;
	MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;

	AddressU = Clamp;
	AddressV = Clamp;
};

sampler BlurSampler = 
sampler_state
{
    Texture = <g_BlurTex>;
	MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

	AddressU = Clamp;
	AddressV = Clamp;
};

sampler DepthSampler = 
sampler_state
{
    Texture = <g_DepthTex>;
	MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;

	AddressU = Clamp;
	AddressV = Clamp;
};

///////////////////////////////////////////////
// �\���̐錾
///////////////////////////////////////////////
struct VS_OUT
{
	float4	Pos	:POSITION;	// ���W
	float2	Tex	:TEXCOORD;	// �e�N�X�`�����W
};

///////////////////////////////////////////////
// ���_�V�F�[�_
///////////////////////////////////////////////
VS_OUT VS_DOF(	float4 pos : POSITION,
				float2 tex : TEXCOORD0)
{
	VS_OUT Out;
	
	// �������Ȃ��œn��
	Out.Pos = pos;
	Out.Tex = tex;

	return Out;
}

///////////////////////////////////////////////
// �s�N�Z���V�F�[�_
///////////////////////////////////////////////
float4 PS_DOF(float2 tex : TEXCOORD0): COLOR0
{	
	// �ʏ�V�[��
  	float4 SceneColor = tex2D(SceneSampler, tex);
  	
  	// �k���V�[��
 	float4 BlurColor = tex2D(BlurSampler, tex);
   	
   	// �[�x�L�^�V�[��
   	float4 DepthColor  = tex2D(DepthSampler, tex);

	// ��ʊE�[�x�̌v�Z 
	float4	OutColor = 0;
	
	// �w�肳�ꂽ�t�H�[�J�X�ȉ��Ȃ�
	if(DepthColor.r <= g_Focus)
	{
		// �������H�����ɐF�r�o
		OutColor = SceneColor;
	
	}else{
	
		// �ڂ����䗦���v�Z����
		float Ratio = saturate( (DepthColor.r - g_Focus) * g_BlurRate );
		
		// ���Ԃ̐F���v�Z����
		OutColor	= lerp( SceneColor, BlurColor, Ratio );
	}

	OutColor.w = 1.0;
	
	return OutColor;
}	

///////////////////////////////////////////////
// �e�N�j�b�N
///////////////////////////////////////////////
technique Tec
{
    pass P0
    {   
        vertexShader = compile vs_2_0 VS_DOF();
        pixelShader  = compile ps_2_0 PS_DOF();
	}
}
