
/////////////////////////////
// �O���[�o���ϐ�
/////////////////////////////
static DWORD	g_CurrentTime	= 0;	// ���̎���
static DWORD	g_SaveTime		= 0;	// �O�̎���
static DWORD	g_StartTime		= 0;	// FPS�J�E���^���n�܂�������
static bool		g_IsSlow		= false;// �x����
static BYTE		g_Cnt			= 0;	// �J�E���^
static BYTE		g_SkipCnt		= 0;	// �t���[���X�L�b�v������

/////////////////////////////
// �ÓI�����o���̉�
/////////////////////////////
const double	CFPS::SECOND	= static_cast< double >( CLOCKS_PER_SEC );
bool			CFPS::m_SlowFlg	= false;

/////////////////////////////
// ����
/////////////////////////////

// �R���X�g���N�^
CFPS::CFPS():
m_Avg( 0 ),
m_Count( 0 ),
m_CountTime( 0 ),
m_SaveTime( 0 )
{
}

// �X�V
void CFPS::Update()
{
	DWORD	NowTime;	// ���݂̎��ԕۑ��p
	int		Wait;		// �҂ׂ�����

	// ����̍ŏ�
	if( m_Count == 0 )
	{
		// ���S�ȍŏ��Ȃ�
		if( m_SaveTime == 0 )
		{
			Wait = 0;
		
		}else{	// �O�t���[���L�����Ԃ���v�Z

			Wait = m_CountTime + CLOCKS_PER_SEC - timeGetTime();
		}
	
	}else{	// �҂ׂ����Ԃ��v�Z����

		Wait = static_cast< int >( m_CountTime + m_Count * (SECOND / FPS) ) - timeGetTime();
	}

	// �v�Z�����������҂�
	if( Wait > 0 )
	{
		Sleep( Wait );
		m_SlowFlg = false;
	
	}else{
	
		m_SlowFlg = true;
	}

	NowTime = timeGetTime();	// ���Ԃ��L��

	// 1������쐬
	if( m_Count == 0 )
	{
		m_CountTime = NowTime;
	}

	// ���ԕۑ�
	m_FlameAvg[ m_Count ] = NowTime - Wait;
	m_SaveTime = NowTime;

	// ���όv�Z
	if( m_Count == FPS - 1 )
	{
		m_Avg = 0.0;

		for( int i = 0; i < FPS; i++ )
		{
			m_Avg += m_FlameAvg[ i ];
		}

		m_Avg /= FPS;

#ifdef _DEBUG
		char Str[ 0xff ];

		sprintf_s( Str, sizeof( Str ), "FPS = %lf\n" , m_Avg );

		//OutputDebugStr( Str );
#endif
	}

	m_Count = (++m_Count) % FPS;
}