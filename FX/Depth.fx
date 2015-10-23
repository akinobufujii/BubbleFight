//////////////////////////////
// �O���[�o���ϐ�
//////////////////////////////
float4x4 	g_World;
float4x4 	g_View;
float4x4 	g_Proj;
float		g_BlurStart = 30.f;	// �ڂ����n�ߋ���

//////////////////////////////
// �\���̐錾
//////////////////////////////
struct VS_OUT
{
	float4	Pos		:POSITION;	// ���W
	float4	Depth	:TEXCOORD0;	// �[�x
};

//////////////////////////////
// ���_�V�F�[�_
//////////////////////////////
VS_OUT VS_Depth( float4 pos : POSITION )
{
	VS_OUT Out;
	
	// ���W�ϊ��Ɛ[�x�v�Z
	Out.Depth = mul( mul(pos, g_World), g_View );
	Out.Pos	  = mul(Out.Depth, g_Proj);
	
	return Out;
}

//////////////////////////////
// �s�N�Z���V�F�[�_
//////////////////////////////
float4 PS_Depth( VS_OUT vs ) : COLOR0
{
	return vs.Depth.z / g_BlurStart;
	
}


//////////////////////////////
// �e�N�j�b�N
//////////////////////////////
technique Tec
{
    pass P0
    {
        vertexShader = compile vs_2_0 VS_Depth();
        pixelShader  = compile ps_2_0 PS_Depth();
	}
}
