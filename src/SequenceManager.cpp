/////////////////////////////////
// �C���N���[�h
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
// ���O��ԏȗ�
//////////////////////////////////////////////////
using namespace Sequence;

//////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////

// �R���X�g���N�^
CManager::CManager():
#ifdef _DEBUG
m_lpRoot( NEW CStageSelect() )
#else
m_lpRoot( NEW CTitle() )
#endif
{
	m_lpRoot->FileLoad();
}

// �f�X�g���N�^
CManager::~CManager()
{
	SAFE_DELETE( m_lpRoot );
}

// �Q�[�����[�v
bool CManager::GameLoop()
{
	// ���g������Ύ��s
	if( m_lpRoot )
	{
		// FPS�X�V
		m_Fps.Update();

		// ���͍X�V
		Input::Keyboard::UpdateState();

		// �����������K�v�Ȃ����
		if( m_lpRoot->RunProc() == false )
		{
			IBase* lpTemp = m_lpRoot->GetNext();
			
			if( lpTemp )
			{
				// �}���`�X���b�h�N��
				LoadStart();
			
				SAFE_DELETE( m_lpRoot );	// �j��

				m_lpRoot = lpTemp;			// �����l��

				// �ǂݍ���
				m_lpRoot->FileLoad();
				
				// �}���`�X���b�h�I��
				LoadEnd();

			}else{

				// �����Ȃ���Ή�
				SAFE_DELETE( m_lpRoot );
			}
		}
	
	}else{
		return false;
	}

	return true;
}

// �ǂݍ��݊J�n
void CManager::LoadStart()
{
	// �t�H���g�̓ǂݍ���
	CSTexManager::GetInst()->Load( D3D::GetDevice(), CLoading::TEXPASS_LOADFONT );

	// ���[�f�B���O�N���X�쐬
	m_lpLoad = NEW CLoading();

	// �X���b�h�쐬�A�N��
	m_lpThread = NEW boost::thread( boost::ref( *m_lpLoad ) );
}

// �ǂݍ��ݏI��
void CManager::LoadEnd()
{
	// �I���ʍ�
	m_lpLoad->End();

	// �X���b�h�̕`�抮����҂�
	m_lpThread->join();

	// ���������
	SAFE_DELETE( m_lpLoad );
	SAFE_DELETE( m_lpThread );
}