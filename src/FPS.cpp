
/////////////////////////////
// グローバル変数
/////////////////////////////
static DWORD	g_CurrentTime	= 0;	// 今の時間
static DWORD	g_SaveTime		= 0;	// 前の時間
static DWORD	g_StartTime		= 0;	// FPSカウンタが始まった時間
static bool		g_IsSlow		= false;// 遅いか
static BYTE		g_Cnt			= 0;	// カウンタ
static BYTE		g_SkipCnt		= 0;	// フレームスキップした回数

/////////////////////////////
// 静的メンバ実体化
/////////////////////////////
const double	CFPS::SECOND	= static_cast< double >( CLOCKS_PER_SEC );
bool			CFPS::m_SlowFlg	= false;

/////////////////////////////
// 実装
/////////////////////////////

// コンストラクタ
CFPS::CFPS():
m_Avg( 0 ),
m_Count( 0 ),
m_CountTime( 0 ),
m_SaveTime( 0 )
{
}

// 更新
void CFPS::Update()
{
	DWORD	NowTime;	// 現在の時間保存用
	int		Wait;		// 待つべき時間

	// 一周の最初
	if( m_Count == 0 )
	{
		// 完全な最初なら
		if( m_SaveTime == 0 )
		{
			Wait = 0;
		
		}else{	// 前フレーム記憶時間から計算

			Wait = m_CountTime + CLOCKS_PER_SEC - timeGetTime();
		}
	
	}else{	// 待つべき時間を計算する

		Wait = static_cast< int >( m_CountTime + m_Count * (SECOND / FPS) ) - timeGetTime();
	}

	// 計算した分だけ待つ
	if( Wait > 0 )
	{
		Sleep( Wait );
		m_SlowFlg = false;
	
	}else{
	
		m_SlowFlg = true;
	}

	NowTime = timeGetTime();	// 時間を記憶

	// 1周基準を作成
	if( m_Count == 0 )
	{
		m_CountTime = NowTime;
	}

	// 時間保存
	m_FlameAvg[ m_Count ] = NowTime - Wait;
	m_SaveTime = NowTime;

	// 平均計算
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