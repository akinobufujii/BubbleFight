/////////////////////////////////
// インクルード
/////////////////////////////////
#include "Bubble.h"
#include "BubbleManager.h"

#include "SequenceBase.h"

#include "Title.h"

#include "Loading.h"

#ifdef	_DEBUG
#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "EnemyManager.h"
#include "UI.h"
#include "BubbleManager.h"
#include "GameMain.h"

#include "GameOver.h"

#include "Clear.h"

#include "ShotBase.h"
#include "ShotManager.h"

#include "BossBase.h"

#include "DOFText.h"
#include "TestBump.h"
#include "TestCaustic.h"

#include "StageSelect.h"
#endif

#include "SequenceManager.h"

//////////////////////////////////////////////////
// 名前空間省略
//////////////////////////////////////////////////
using namespace Sequence;

//////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////

// コンストラクタ
CManager::CManager():
#ifdef _DEBUG
m_lpRoot( NEW CStageSelect() )
#else
m_lpRoot( NEW CTitle() )
#endif
{
	m_lpRoot->FileLoad();
}

// デストラクタ
CManager::~CManager()
{
	SAFE_DELETE( m_lpRoot );
}

// ゲームループ
bool CManager::GameLoop()
{
	// 中身があれば実行
	if( m_lpRoot )
	{
		// FPS更新
		m_Fps.Update();

		// 入力更新
		Input::Keyboard::UpdateState();

		// もう処理が必要なければ
		if( m_lpRoot->RunProc() == false )
		{
			IBase* lpTemp = m_lpRoot->GetNext();
			
			if( lpTemp )
			{
				// マルチスレッド起動
				LoadStart();
			
				SAFE_DELETE( m_lpRoot );	// 破壊

				m_lpRoot = lpTemp;			// 次を獲得

				// 読み込み
				m_lpRoot->FileLoad();
				
				// マルチスレッド終了
				LoadEnd();

			}else{

				// 次がなければ壊す
				SAFE_DELETE( m_lpRoot );
			}
		}
	
	}else{
		return false;
	}

	return true;
}

// 読み込み開始
void CManager::LoadStart()
{
	// フォントの読み込み
	CSTexManager::GetInst()->Load( D3D::GetDevice(), CLoading::TEXPASS_LOADFONT );

	// ローディングクラス作成
	m_lpLoad = NEW CLoading();

	// スレッド作成、起動
	m_lpThread = NEW boost::thread( boost::ref( *m_lpLoad ) );
}

// 読み込み終了
void CManager::LoadEnd()
{
	// 終了通告
	m_lpLoad->End();

	// スレッドの描画完了を待つ
	m_lpThread->join();

	// メモリ解放
	SAFE_DELETE( m_lpLoad );
	SAFE_DELETE( m_lpThread );
}