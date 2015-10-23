// インクルード
#include "wavread.h"

#include "SoundManager.h"

#pragma comment(lib, "dsound.lib")

// 静的メンバ実体化
CSoundManager*	CSoundManager::m_lpInst		= NULL;
const LPSTR		CSoundManager::UNION_PATH	= "Sound/";

// 生成
bool CSoundManager::Create()
{
	if( m_lpInst == NULL )
	{
		m_lpInst = NEW CSoundManager();

		return true;
	}

	return false;
}

// 破壊
void CSoundManager::Destroy()
{
	SAFE_DELETE( m_lpInst );
}

// 初期化
bool CSoundManager::Init( HWND hwnd )
{
	DirectSoundCreate8(NULL, &m_lpDSound, NULL);

	//協調レベルを設定
	m_lpDSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);

	// プライマリ・バッファの作成
	// DSBUFFERDESC構造体を設定
	DSBUFFERDESC dsbdesc; 
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
	// プライマリ・バッファを指定
	dsbdesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_PRIMARYBUFFER;
	dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat = NULL;

	// バッファを作る
	m_lpDSound->CreateSoundBuffer(&dsbdesc, &m_lpSPrimary, NULL); 

	// プライマリ・バッファのWaveフォーマットを設定
	// 　　　優先協調レベル以上の協調レベルが設定されている必要があります．
	WAVEFORMATEX pcmwf; 
	ZeroMemory(&pcmwf, sizeof(WAVEFORMATEX)); 
	pcmwf.wFormatTag = WAVE_FORMAT_PCM; 
	pcmwf.nChannels = 2;		// ２チャンネル（ステレオ）
	pcmwf.nSamplesPerSec = 44100;	// サンプリング・レート　44.1kHz
	pcmwf.nBlockAlign = 4;
	pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign; 
	pcmwf.wBitsPerSample = 16;		// 16ビット
	m_lpSPrimary->SetFormat(&pcmwf);

	return true;
}

// 読み込み
bool CSoundManager::Load( LPCSTR path )
{
	MAPTYPE::iterator It = m_SecondMap.find( path );

	// 既に読み込まれていれば
	if( It != m_SecondMap.end() )
	{
		return true;
	}

	// 読み込み
	LPDIRECTSOUNDBUFFER8 lpTemp = NULL;

	if( LoadWave( lpTemp, path ) )
	{
		// マップに追加
		m_SecondMap.insert( PAIRTYPE( path, NEW SOUNDBUFFERLIST() ) );

		m_SecondMap[ path ]->push_back( lpTemp );
		
		return true;
	
	}else{

		return false;
	}
}

// 読み込みリストから読み込み
bool CSoundManager::LoadFromTextList(LPCSTR list_path)
{
	std::fstream	File;
	char			Path[ 0xff ];

	// ファイルオープン
	File.open( list_path, std::ios::in );

	// 読み込み
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

// 解放
void CSoundManager::Release(LPCSTR path)
{
	MAPTYPE::iterator It = m_SecondMap.find( path );

	if( It != m_SecondMap.end() )
	{
		// コピーされたもの全て解放
		BOOST_FOREACH( LPDIRECTSOUNDBUFFER8 List, *It->second )
		{
			SAFE_RELEASE( List );
		}

		// 音源を解放
		m_SecondMap.erase( It );
	}
}

// 全て開放
void CSoundManager::AllRelease()
{
	// セカンダリ解放
	BOOST_FOREACH( PAIRTYPE It, m_SecondMap )
	{
		BOOST_FOREACH( LPDIRECTSOUNDBUFFER8 List, *It.second )
		{
			SAFE_RELEASE( List );
		}

		// リスト削除
		SAFE_DELETE( It.second );
	}

	m_SecondMap.clear();
}

// 再生
void CSoundManager::Play( LPCSTR play_path, bool is_loop, bool is_synchronized, const GUID* lp_effect, DWORD set_pos )
{
	MAPTYPE::iterator It = m_SecondMap.find( play_path );
	
	// なければ読み込む
	if( It == m_SecondMap.end() )
	{
		Load( play_path );

		It = m_SecondMap.find( play_path );
	}

	// 再生する
	DWORD Flg = 0;

	BOOST_FOREACH( LPDIRECTSOUNDBUFFER8 List, *It->second )
	{
		List->GetStatus( &Flg );

		// 再生中でなければ
		if( (Flg & DSBSTATUS_PLAYING) == 0 )
		{
			//// エフェクト指定があれば
			//if( lp_effect )
			//{
			//	List->Stop();

			//	DSEFFECTDESC	Effect;		// サウンドエフェクト構造体
			//	DWORD			Codec = 0;	// ※使わない

			//	// 初期化
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

	// 非同期なら
	if( is_synchronized == false )
	{
		// リストがいっぱいならコピーして追加
		LPDIRECTSOUNDBUFFER		lpSTemp		= NULL;
		LPDIRECTSOUNDBUFFER8	lpBufTemp	= NULL;

		m_lpDSound->DuplicateSoundBuffer( (*It->second->begin()), &lpSTemp );
		lpSTemp->QueryInterface( IID_IDirectSoundBuffer8, (LPVOID*)&lpBufTemp );
		SAFE_RELEASE( lpSTemp );

		// 鳴らしてから登録
		lpBufTemp->SetCurrentPosition( set_pos );
		lpBufTemp->Play( 0, 0, (is_loop)? DSBPLAY_LOOPING : 0 );
		It->second->push_back( lpBufTemp );
	}
}

// 停止
void CSoundManager::Stop( LPCSTR path )
{
	MAPTYPE::iterator It = m_SecondMap.find( path );
	
	// なければ何もしない
	if( It == m_SecondMap.end() )
	{
		return;
	}

	BOOST_FOREACH( LPDIRECTSOUNDBUFFER8 List, *It->second )
	{
		List->Stop();
	}
}

// コンストラクタ
CSoundManager::CSoundManager():
m_lpSPrimary( NULL ),
m_lpDSound( NULL ),
m_Vol( 0L ),
m_Pan( 0L )
{
}

// デストラクタ
CSoundManager::~CSoundManager()
{
	// ボリュームとパンを戻す
	m_lpSPrimary->SetVolume( m_Vol );
	m_lpSPrimary->SetPan( m_Pan );

	// セカンダリバッファ解放
	AllRelease();

	SAFE_RELEASE( m_lpSPrimary );
	SAFE_RELEASE( m_lpDSound );
}

bool CSoundManager::LoadWave( LPDIRECTSOUNDBUFFER8 &pDSData, LPCSTR fname )
{
	// TODO: ココらへんのアルゴリズム簡略化
	HRESULT hr;

	// WAVEファイルを開く
	CWaveSoundRead WaveFile;
	WaveFile.Open( const_cast<char*>( fname ) );

	// セカンダリ・バッファを作成する
	// DSBUFFERDESC構造体を設定
	DSBUFFERDESC dsbdesc; 
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
	
	dsbdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS |
						DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;
	dsbdesc.dwBufferBytes = WaveFile.m_ckIn.cksize;
	dsbdesc.lpwfxFormat = WaveFile.m_pwfx;

	// バッファを作る
	LPDIRECTSOUNDBUFFER pDSTmp;
//	lpDSound->CreateSoundBuffer(&dsbdesc, &pDSData, NULL); 
	hr = m_lpDSound->CreateSoundBuffer(&dsbdesc, &pDSTmp, NULL); 
	pDSTmp->QueryInterface(IID_IDirectSoundBuffer8,(LPVOID*)&pDSData);
	pDSTmp->Release();

	// セカンダリ・バッファにWaveデータを書き込む
	LPVOID lpvPtr1;		// 最初のブロックのポインタ
	DWORD dwBytes1;		// 最初のブロックのサイズ
	LPVOID lpvPtr2;		// ２番目のブロックのポインタ
	DWORD dwBytes2;		// ２番目のブロックのサイズ

	hr = pDSData->Lock(0, WaveFile.m_ckIn.cksize, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0); 

	// DSERR_BUFFERLOSTが返された場合，Restoreメソッドを使ってバッファを復元する
	if(DSERR_BUFFERLOST == hr)
	{
		pDSData->Restore();
		hr = pDSData->Lock(0, WaveFile.m_ckIn.cksize, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0);
	} 
	if (SUCCEEDED(hr))
	{
		// ロック成功

		// ここで，バッファに書き込む
		// バッファにデータをコピーする
		UINT rsize;
		WaveFile.Read(dwBytes1, (LPBYTE)lpvPtr1, &rsize);
		if ( 0 != dwBytes2 )
			WaveFile.Read(dwBytes2, (LPBYTE)lpvPtr2, &rsize);

	   // 書き込みが終わったらすぐにUnlockする．
		hr = pDSData->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2); 
	}

	// ボリュームとパンを記憶しておく
	m_lpSPrimary->GetVolume( &m_Vol );
	m_lpSPrimary->GetPan( &m_Pan );

	return true;
}