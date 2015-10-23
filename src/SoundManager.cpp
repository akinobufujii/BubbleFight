// �C���N���[�h
#include "wavread.h"

#include "SoundManager.h"

#pragma comment(lib, "dsound.lib")

// �ÓI�����o���̉�
CSoundManager*	CSoundManager::m_lpInst		= NULL;
const LPSTR		CSoundManager::UNION_PATH	= "Sound/";

// ����
bool CSoundManager::Create()
{
	if( m_lpInst == NULL )
	{
		m_lpInst = NEW CSoundManager();

		return true;
	}

	return false;
}

// �j��
void CSoundManager::Destroy()
{
	SAFE_DELETE( m_lpInst );
}

// ������
bool CSoundManager::Init( HWND hwnd )
{
	DirectSoundCreate8(NULL, &m_lpDSound, NULL);

	//�������x����ݒ�
	m_lpDSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);

	// �v���C�}���E�o�b�t�@�̍쐬
	// DSBUFFERDESC�\���̂�ݒ�
	DSBUFFERDESC dsbdesc; 
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
	// �v���C�}���E�o�b�t�@���w��
	dsbdesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_PRIMARYBUFFER;
	dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat = NULL;

	// �o�b�t�@�����
	m_lpDSound->CreateSoundBuffer(&dsbdesc, &m_lpSPrimary, NULL); 

	// �v���C�}���E�o�b�t�@��Wave�t�H�[�}�b�g��ݒ�
	// �@�@�@�D�拦�����x���ȏ�̋������x�����ݒ肳��Ă���K�v������܂��D
	WAVEFORMATEX pcmwf; 
	ZeroMemory(&pcmwf, sizeof(WAVEFORMATEX)); 
	pcmwf.wFormatTag = WAVE_FORMAT_PCM; 
	pcmwf.nChannels = 2;		// �Q�`�����l���i�X�e���I�j
	pcmwf.nSamplesPerSec = 44100;	// �T���v�����O�E���[�g�@44.1kHz
	pcmwf.nBlockAlign = 4;
	pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign; 
	pcmwf.wBitsPerSample = 16;		// 16�r�b�g
	m_lpSPrimary->SetFormat(&pcmwf);

	return true;
}

// �ǂݍ���
bool CSoundManager::Load( LPCSTR path )
{
	MAPTYPE::iterator It = m_SecondMap.find( path );

	// ���ɓǂݍ��܂�Ă����
	if( It != m_SecondMap.end() )
	{
		return true;
	}

	// �ǂݍ���
	LPDIRECTSOUNDBUFFER8 lpTemp = NULL;

	if( LoadWave( lpTemp, path ) )
	{
		// �}�b�v�ɒǉ�
		m_SecondMap.insert( PAIRTYPE( path, NEW SOUNDBUFFERLIST() ) );

		m_SecondMap[ path ]->push_back( lpTemp );
		
		return true;
	
	}else{

		return false;
	}
}

// �ǂݍ��݃��X�g����ǂݍ���
bool CSoundManager::LoadFromTextList(LPCSTR list_path)
{
	std::fstream	File;
	char			Path[ 0xff ];

	// �t�@�C���I�[�v��
	File.open( list_path, std::ios::in );

	// �ǂݍ���
	if( File.is_open() )
	{
		while( File.eof() == false )
		{
			File >> Path;

			Load( Path );
		}

	}else{

		return false;
	}

	return true; 
}

// ���
void CSoundManager::Release(LPCSTR path)
{
	MAPTYPE::iterator It = m_SecondMap.find( path );

	if( It != m_SecondMap.end() )
	{
		// �R�s�[���ꂽ���̑S�ĉ��
		BOOST_FOREACH( LPDIRECTSOUNDBUFFER8 List, *It->second )
		{
			SAFE_RELEASE( List );
		}

		// ���������
		m_SecondMap.erase( It );
	}
}

// �S�ĊJ��
void CSoundManager::AllRelease()
{
	// �Z�J���_�����
	BOOST_FOREACH( PAIRTYPE It, m_SecondMap )
	{
		BOOST_FOREACH( LPDIRECTSOUNDBUFFER8 List, *It.second )
		{
			SAFE_RELEASE( List );
		}

		// ���X�g�폜
		SAFE_DELETE( It.second );
	}

	m_SecondMap.clear();
}

// �Đ�
void CSoundManager::Play( LPCSTR play_path, bool is_loop, bool is_synchronized, const GUID* lp_effect, DWORD set_pos )
{
	MAPTYPE::iterator It = m_SecondMap.find( play_path );
	
	// �Ȃ���Γǂݍ���
	if( It == m_SecondMap.end() )
	{
		Load( play_path );

		It = m_SecondMap.find( play_path );
	}

	// �Đ�����
	DWORD Flg = 0;

	BOOST_FOREACH( LPDIRECTSOUNDBUFFER8 List, *It->second )
	{
		List->GetStatus( &Flg );

		// �Đ����łȂ����
		if( (Flg & DSBSTATUS_PLAYING) == 0 )
		{
			//// �G�t�F�N�g�w�肪�����
			//if( lp_effect )
			//{
			//	List->Stop();

			//	DSEFFECTDESC	Effect;		// �T�E���h�G�t�F�N�g�\����
			//	DWORD			Codec = 0;	// ���g��Ȃ�

			//	// ������
			//	memset( &Effect, 0, sizeof(DSEFFECTDESC) );

			//	Effect.dwSize		= sizeof(DSEFFECTDESC);
			//	Effect.guidDSFXClass= *lp_effect;

			//	hr = List->SetFX( 1, &Effect, &Codec );
			//}

			List->SetCurrentPosition( set_pos );
			List->Play( 0, 0, (is_loop)? DSBPLAY_LOOPING : 0 );
			return;
		}
	}

	// �񓯊��Ȃ�
	if( is_synchronized == false )
	{
		// ���X�g�������ς��Ȃ�R�s�[���Ēǉ�
		LPDIRECTSOUNDBUFFER		lpSTemp		= NULL;
		LPDIRECTSOUNDBUFFER8	lpBufTemp	= NULL;

		m_lpDSound->DuplicateSoundBuffer( (*It->second->begin()), &lpSTemp );
		lpSTemp->QueryInterface( IID_IDirectSoundBuffer8, (LPVOID*)&lpBufTemp );
		SAFE_RELEASE( lpSTemp );

		// �炵�Ă���o�^
		lpBufTemp->SetCurrentPosition( set_pos );
		lpBufTemp->Play( 0, 0, (is_loop)? DSBPLAY_LOOPING : 0 );
		It->second->push_back( lpBufTemp );
	}
}

// ��~
void CSoundManager::Stop( LPCSTR path )
{
	MAPTYPE::iterator It = m_SecondMap.find( path );
	
	// �Ȃ���Ή������Ȃ�
	if( It == m_SecondMap.end() )
	{
		return;
	}

	BOOST_FOREACH( LPDIRECTSOUNDBUFFER8 List, *It->second )
	{
		List->Stop();
	}
}

// �R���X�g���N�^
CSoundManager::CSoundManager():
m_lpSPrimary( NULL ),
m_lpDSound( NULL ),
m_Vol( 0L ),
m_Pan( 0L )
{
}

// �f�X�g���N�^
CSoundManager::~CSoundManager()
{
	// �{�����[���ƃp����߂�
	m_lpSPrimary->SetVolume( m_Vol );
	m_lpSPrimary->SetPan( m_Pan );

	// �Z�J���_���o�b�t�@���
	AllRelease();

	SAFE_RELEASE( m_lpSPrimary );
	SAFE_RELEASE( m_lpDSound );
}

bool CSoundManager::LoadWave( LPDIRECTSOUNDBUFFER8 &pDSData, LPCSTR fname )
{
	// TODO: �R�R��ւ�̃A���S���Y���ȗ���
	HRESULT hr;

	// WAVE�t�@�C�����J��
	CWaveSoundRead WaveFile;
	WaveFile.Open( const_cast<char*>( fname ) );

	// �Z�J���_���E�o�b�t�@���쐬����
	// DSBUFFERDESC�\���̂�ݒ�
	DSBUFFERDESC dsbdesc; 
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
	
	dsbdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS |
						DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;
	dsbdesc.dwBufferBytes = WaveFile.m_ckIn.cksize;
	dsbdesc.lpwfxFormat = WaveFile.m_pwfx;

	// �o�b�t�@�����
	LPDIRECTSOUNDBUFFER pDSTmp;
//	lpDSound->CreateSoundBuffer(&dsbdesc, &pDSData, NULL); 
	hr = m_lpDSound->CreateSoundBuffer(&dsbdesc, &pDSTmp, NULL); 
	pDSTmp->QueryInterface(IID_IDirectSoundBuffer8,(LPVOID*)&pDSData);
	pDSTmp->Release();

	// �Z�J���_���E�o�b�t�@��Wave�f�[�^����������
	LPVOID lpvPtr1;		// �ŏ��̃u���b�N�̃|�C���^
	DWORD dwBytes1;		// �ŏ��̃u���b�N�̃T�C�Y
	LPVOID lpvPtr2;		// �Q�Ԗڂ̃u���b�N�̃|�C���^
	DWORD dwBytes2;		// �Q�Ԗڂ̃u���b�N�̃T�C�Y

	hr = pDSData->Lock(0, WaveFile.m_ckIn.cksize, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0); 

	// DSERR_BUFFERLOST���Ԃ��ꂽ�ꍇ�CRestore���\�b�h���g���ăo�b�t�@�𕜌�����
	if(DSERR_BUFFERLOST == hr)
	{
		pDSData->Restore();
		hr = pDSData->Lock(0, WaveFile.m_ckIn.cksize, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0);
	} 
	if (SUCCEEDED(hr))
	{
		// ���b�N����

		// �����ŁC�o�b�t�@�ɏ�������
		// �o�b�t�@�Ƀf�[�^���R�s�[����
		UINT rsize;
		WaveFile.Read(dwBytes1, (LPBYTE)lpvPtr1, &rsize);
		if ( 0 != dwBytes2 )
			WaveFile.Read(dwBytes2, (LPBYTE)lpvPtr2, &rsize);

	   // �������݂��I������炷����Unlock����D
		hr = pDSData->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2); 
	}

	// �{�����[���ƃp�����L�����Ă���
	m_lpSPrimary->GetVolume( &m_Vol );
	m_lpSPrimary->GetPan( &m_Pan );

	return true;
}