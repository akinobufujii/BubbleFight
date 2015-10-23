#ifndef __AKI_PLAYERBASE_H__
#define __AKI_PLAYERBASE_H__

//////////////////////////////////////////////////
// 不完全型宣言
//////////////////////////////////////////////////
class CStoneManager;
class CSandSpray;

namespace Player
{
	//////////////////////////////////////////////////
	// プレイヤー(イルカ)
	//////////////////////////////////////////////////
	class CDolphin : virtual public IDoubleDispatch
	{
	public:	// 外部公開

		//////////////////////////////////////////////////
		// 型宣言
		//////////////////////////////////////////////////
		typedef USHORT	FLGTYPE;

		//////////////////////////////////////////////////
		// 定数
		//////////////////////////////////////////////////
		CONSTANT float		SPEED;						// 移動速度
		CONSTANT float		SPEED_MAX;					// 最大速度
		CONSTANT float		EVASION_MOVE_SPEED;			// 回避移動速度
		CONSTANT float		POWERCHARGE_SPEED;			// パワーを貯める速度
		CONSTANT float		HITPOWER_PLUS;				// 敵とかに当たった時の強さ
		CONSTANT float		RECOVERY_SPEED_LINE;		// 水面に飛び出るときの速さ判定
		CONSTANT float		POINTER_ANIME_RATE;			// アニメ動き量
		CONSTANT float		CHARGE_ANIME_RATE;			// 動き量
		CONSTANT float		CHARGEMAX_SCALE;			// 最大溜め時の大きさ
		CONSTANT float		CHARGESCALE_RATE;			// 溜め中の大きくなる加算値
		CONSTANT float		FLOAT_SPEED;				// 浮く速さ
		CONSTANT short		LEARROT_MAX;				// 後ろ回転最高角度	
		CONSTANT USHORT		BLESS_LOST_TIME;			// 息が1消費するまでの時間
		CONSTANT USHORT		HP_MAX;						// ライフ初期値
		CONSTANT USHORT		EVASION_EFFECTERACE_TIME;	// 回避エフェクトの消える時間
		CONSTANT BYTE		MOVE_BUBBLE_MAX;			// 動いた時の泡生産最大数
		CONSTANT BYTE		ROT_MAX;					// 回転最大角度
		CONSTANT BYTE		LEARROT_SPEED;				// 後ろ回転速さ
		CONSTANT BYTE		POWER_MAX;					// パワー最大値
		CONSTANT BYTE		BLESS_MAX;					// 息のゲージ最大値
		CONSTANT BYTE		AFTER_IMAGE_MAX;			// 残像保存数
		CONSTANT BYTE		SHOTLOST_BLESS_RATE;		// 弾で消費する息のレート
		CONSTANT BYTE		EVASION_ROT_SPEED;			// 回避回転速度	
		CONSTANT BYTE		OVERRUN_ROT;				// 範囲超えた時の回転
		CONSTANT BYTE		RETURN_POS;					// 戻る位置
		CONSTANT BYTE		POINTER_ANIME_NUM;			// ポインターのアニメ数
		CONSTANT BYTE		CHARGE_ANIME_NUM;			// チャージアニメ数
		CONSTANT BYTE		REFORM_SPEED;				// 姿勢を正す速さ
		CONSTANT BYTE		DIE_ROT_SPEED;				// 死んだ時に転がる速さ
		CONSTANT BYTE		RECOVERYBLESS_SPEED;		// 息の回復速度

		CONSTANT float	ATTACK_SPEED;		// 攻撃の速さ
		CONSTANT USHORT	ATTACK_ROT_BEGIN;	// 攻撃角度の始まり
		CONSTANT USHORT	ATTACK_ROT_END;		// 攻撃角度の終わり

		/////////////////////////////////
		// フラグ定数
		/////////////////////////////////
		CONSTANT FLGTYPE	FLG_RIGHT_EVASION;	// 右に回避
		CONSTANT FLGTYPE	FLG_OVER;			// バック
		CONSTANT FLGTYPE	FLG_LEFT_EVASION;	// 左に回避
		CONSTANT FLGTYPE	FLG_SHOT;			// 撃つ
		CONSTANT FLGTYPE	FLG_EVASION;		// 回避中
		CONSTANT FLGTYPE	FLG_ATTACK;			// 攻撃中
		CONSTANT FLGTYPE	FLG_ATTACK_OK;		// 攻撃判定を行える
		CONSTANT FLGTYPE	FLG_DAMAGE;			// ダメージを食らった
		CONSTANT FLGTYPE	FLG_HIT_STONE;		// 石に当たった
		CONSTANT FLGTYPE	FLG_JUMP;			// 息回復中(水面演出)
		CONSTANT FLGTYPE	FLG_CHARGE;			// 溜め中
		CONSTANT FLGTYPE	FLG_CHARGEMAX;		// 最大溜め
		CONSTANT FLGTYPE	FLG_DRAW_CHARGE;	// チャージアニメーション描画
		CONSTANT FLGTYPE	FLG_MAX_EMISSIVE;	// 自己証明光らせるかどうか
		CONSTANT FLGTYPE	FLG_DIE;			// 死んだ

		/////////////////////////////////
		// メソッド
		/////////////////////////////////

		// コンストラクタ
		CDolphin();

		// デストラクタ
		~CDolphin();

		//////////////////////////////////////////////////
		//	・関数名	Update
		//	・説明		更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Update();

		//////////////////////////////////////////////////
		//	・関数名	Draw
		//	・説明		描画を行う
		//	・引数		
		//		- lpDevice	デバイス
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Draw( const LPDIRECT3DDEVICE9 lpDevice );

		//////////////////////////////////////////////////
		//	・関数名	DrawEffect
		//	・説明		エフェクトの描画を行う
		//	・引数		
		//		- lpDevice	デバイス
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void DrawEffect( const LPDIRECT3DDEVICE9 lpdevice );

		//////////////////////////////////////////////////
		//	・関数名	Dispatch
		//	・説明		送られてきたオブジェクトに自分を送りつける
		//	・引数		
		//		- object	行動を起こして欲しいオブジェクト
		//	・戻り値	なし
		//////////////////////////////////////////////////
		inline void Dispatch( IDoubleDispatch* object )
		{
			object->Action( *this );
		}
		
		//////////////////////////////////////////////////
		//	・関数名	Action
		//	・説明		敵に対する行動(Dispatchからのみ呼ばれる)
		//	・引数		
		//		- object	行動を起こす対象のオブジェクト
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Action( Enemy::CBase& object );

		//////////////////////////////////////////////////
		//	・関数名	Action
		//	・説明		弾に対する行動(Dispatchからのみ呼ばれる)
		//	・引数		
		//		- object	行動を起こす対象のオブジェクト
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Action( Shot::CBase& object );

		//////////////////////////////////////////////////
		//	・関数名	Action
		//	・説明		ボスに対する行動(Dispatchからのみ呼ばれる)
		//	・引数		
		//		- object	行動を起こす対象のオブジェクト
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Action( Boss::CBase& object );

		//////////////////////////////////////////////////
		//	・関数名	GetMatrix
		//	・説明		行列を獲得する
		//	・引数		なし
		//	・戻り値	行列を返す
		//////////////////////////////////////////////////
		inline D3DXMATRIX GetMatrix() const
		{
			return m_mMat;
		}
		
		//////////////////////////////////////////////////
		//	・関数名	GetAfterMatrix
		//	・説明		残像の最後の行列を獲得
		//	・引数		なし
		//	・戻り値	残像の最後の行列を返す
		//////////////////////////////////////////////////
		inline const D3DXMATRIX GetAfterMatrix() const
		{
			return m_lpmAfter[ AFTER_IMAGE_MAX - 1 ];
		}
		
		//////////////////////////////////////////////////
		//	・関数名	GetPos
		//	・説明		位置を獲得
		//	・引数		なし
		//	・戻り値	位置を返す
		//////////////////////////////////////////////////
		inline D3DXVECTOR3 GetPos() const
		{
			return m_mMat.GetPos();
		}

		//////////////////////////////////////////////////
		//	・関数名	GetFlg
		//	・説明		フラグ(状態)を獲得
		//	・引数		なし
		//	・戻り値	フラグ(状態)を返す
		//////////////////////////////////////////////////
		inline FLGTYPE GetFlg() const
		{
			return m_Flg;
		}

		//////////////////////////////////////////////////
		//	・関数名	GetShots
		//	・説明		弾マネージャーを獲得
		//	・引数		なし
		//	・戻り値	弾マネージャーを返す
		//////////////////////////////////////////////////
		inline Shot::CManager* GetShots()
		{
			return &m_Shots;
		}

		//////////////////////////////////////////////////
		//	・関数名	GetBubbles
		//	・説明		泡マネージャーを獲得
		//	・引数		なし
		//	・戻り値	泡マネージャーを返す
		//////////////////////////////////////////////////
		inline CBubbleManager* GetBubbles()
		{
			return &m_Bubbles;
		}

		//////////////////////////////////////////////////
		//	・関数名	GetLife
		//	・説明		ライフを獲得
		//	・引数		なし
		//	・戻り値	ライフを返す
		//////////////////////////////////////////////////
		inline USHORT GetLife() const
		{
			return m_HP;
		}

		//////////////////////////////////////////////////
		//	・関数名	GetPower
		//	・説明		溜めている力の強さを獲得
		//	・引数		なし
		//	・戻り値	溜めている力の強さを返す
		//////////////////////////////////////////////////
		inline float GetPower() const
		{
			return m_Power;
		}

		//////////////////////////////////////////////////
		//	・関数名	GetFinDir
		//	・説明		現在の向いている尾ひれの方向を
		//				計算して返す
		//	・引数		なし
		//	・戻り値	尾ひれの方向を返す
		//////////////////////////////////////////////////
		D3DXVECTOR3 GetFinDir() const;

		//////////////////////////////////////////////////
		//	・関数名	GetBless
		//	・説明		現在息の残量を獲得
		//	・引数		なし
		//	・戻り値	現在息の残量を返す
		//////////////////////////////////////////////////
		inline USHORT GetBless() const
		{
			return m_Bless;
		}

		//////////////////////////////////////////////////
		//	・関数名	GetSaveMat
		//	・説明		保存行列を獲得(範囲オーバーの時に使用)
		//	・引数		なし
		//	・戻り値	保存行列を返す
		//////////////////////////////////////////////////
		inline D3DXMATRIX GetSaveMat() const 
		{
			return m_mSave;
		}

		//////////////////////////////////////////////////
		//	・関数名	RecoveryBless
		//	・説明		息ゲージを回復する
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		inline void RecoveryBless()
		{
			if( (m_Bless += RECOVERYBLESS_SPEED) > BLESS_MAX )
			{
				m_Bless = BLESS_MAX;
			}
		}
		
		//////////////////////////////////////////////////
		//	・関数名	GetNowSpeed
		//	・説明		現在の移動の速さを獲得
		//	・引数		なし
		//	・戻り値	現在の移動の速さを返す
		//////////////////////////////////////////////////
		inline float GetNowSpeed() const
		{
			return m_NowSpeed;
		}

		//////////////////////////////////////////////////
		//	・関数名	SetStoneManager
		//	・説明		移動時に衝突判定を行う
		//				岩マネージャーを設定する
		//	・引数		
		//		- lp_stones	岩マネージャーのポインタ
		//	・戻り値	なし
		//////////////////////////////////////////////////
		inline void SetStoneManager( CStoneManager* lp_stones )
		{
			m_lpStones = lp_stones;
		}

		//////////////////////////////////////////////////
		//	・関数名	DrawRipples
		//	・説明		波紋の描画を行う
		//	・引数		
		//		- lp_device	デバイス
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void DrawRipples( const LPDIRECT3DDEVICE9 lp_device );
	
		//////////////////////////////////////////////////
		//	・関数名	SetMatrix
		//	・説明		行列を設定する(クリアシーケンスのみ)
		//	・引数		
		//		- mat	行列
		//	・戻り値	なし
		//////////////////////////////////////////////////
		inline void SetMatrix( const D3DXMATRIX& mat )
		{
			m_mMat = mat;
		}

	private:	// 非公開	

		//friend Sequence::CClear;

		//////////////////////////////////////////////////
		// フィールド
		//////////////////////////////////////////////////
		CMatrix				m_mMat;				// 行列(ローカル座標前提)
		CMatrix				m_mSave;			// 保存行列
		CMatrix*			m_lpmAfter;			// 残像保存(動的配列)
		CVector				m_MoveVec;			// 吹っ飛ぶ方向
		CVector				m_EvasionVec;		// 回避方向ベクトル
		CVector				m_AttackMoveDir;	// 攻撃中に進む方向
		Shot::CManager		m_Shots;			// 弾マネージャー
		CBubbleManager		m_Bubbles;			// 泡マネージャー
		CFinTail			m_Fin;				// 尾ひれ
		CXfile*				m_lpMesh;			// メッシュ
		FLGTYPE				m_Flg;				// フラグ(状態など)
		DWORD				m_SaveBlessTime;	// 息が減った時間
		POINT				m_SaveMousePos;		// マウスの座標
		CStoneManager*		m_lpStones;			// 岩のポインタ
		CRippleManager		m_Ripples;			// 波紋
		CPointSpriteManager	m_Spray;			// しぶき用ポイントスプライト
		CPointSpriteManager	m_Evasion;			// 回避エフェクト
		//CSandSpray*			m_lpSandSplay;		// 砂しぶき
		CBoard				m_Pointer;			// 発射位置を明確にする
		CBoard				m_ChargeEffect;		// チャージエフェクト
		BYTE				m_PointerAnime;		// ポインターのアニメカウンタ
		DWORD				m_PointerSaveTime;	// ポインターの時間保存用
		CGraphicAnime		m_ChargeMaxAnime;	// チャージMaxアニメーション
		CGraphicAnime		m_ChargeAnime;		// チャージアニメーション
		DWORD				m_EmissiveSaveTime;	// エミッシブ時間保存
		short				m_HP;				// ライフ
		short				m_RotCnt;			// 回転カウンタ
		short				m_Bless;			// 息
		float				m_Power;			// パワー(弾に影響)
		float				m_NowSpeed;			// 今の速さ
		float				m_NowRotSpeed;		// 今回転している速さ
		float				m_HitPower;			// 吹っ飛びの強さ
		float				m_NowPlayerScale;	// プレイヤー拡大率	
		float				m_NowFloatPower;	// 浮く力
		
		//////////////////////////////////////////////////
		// メソッド
		//////////////////////////////////////////////////
		
		//////////////////////////////////////////////////
		//	・関数名	UpdateMove
		//	・説明		動きの更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateMove();

		//////////////////////////////////////////////////
		//	・関数名	UpdateKey
		//	・説明		入力更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateKey();

		//////////////////////////////////////////////////
		//	・関数名	UpdatePowerAndShot
		//	・説明		弾とその強さ更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdatePowerAndShot();

		//////////////////////////////////////////////////
		//	・関数名	UpdateBless
		//	・説明		息の更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateBless();

		//////////////////////////////////////////////////
		//	・関数名	UpdateAttack
		//	・説明		フィンの攻撃更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateAttack();

		//////////////////////////////////////////////////
		//	・関数名	UpdateEvasion
		//	・説明		回避更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateEvasion();

		//////////////////////////////////////////////////
		//	・関数名	UpdateAction
		//	・説明		行動の更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateAction();

		//////////////////////////////////////////////////
		//	・関数名	UpdateOverRun
		//	・説明		範囲オーバーの更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateOverRun();

		//////////////////////////////////////////////////
		//	・関数名	UpdateSurfaceSea
		//	・説明		海面に出た時の更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateSurfaceSea();

		//////////////////////////////////////////////////
		//	・関数名	UpdateCollisionMove
		//	・説明		衝突判定を行いながら
		//				移動更新を行う
		//	・引数		
		//		- lp_move	移動する量
		//		- is_move	動くかどうか
		//	・戻り値	衝突したならtrue
		//				そうでないならfalseを返す
		//////////////////////////////////////////////////
		bool UpdateCollisionMove( const D3DXVECTOR3* lp_move, bool is_move = true );

		//////////////////////////////////////////////////
		//	・関数名	ReformPose
		//	・説明		海面に足して背を向けるように
		//				現在の姿勢を正していく
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void ReformPose();

		//////////////////////////////////////////////////
		//	・関数名	ReformPose
		//	・説明		自分自身の描画を行う
		//	・引数		
		//		- lpdevice	デバイス
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void DrawPlayer( const LPDIRECT3DDEVICE9 lpdevice );

		//////////////////////////////////////////////////
		//	・関数名	UpdateDie
		//	・説明		敗北したときの更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateDie();
	};
}

#endif // __AKI_PLAYERBASE_H__