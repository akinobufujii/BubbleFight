#ifndef __AKI_ENEMYBASE_H__
#define __AKI_ENEMYBASE_H__

////////////////////////////////
// 不完全型宣言
////////////////////////////////
class Player::CDolphin;
class CStoneManager;
class CFishManager;
class CDamageObjectBase;
class CWaterSpray;

namespace Enemy
{
	////////////////////////////////
	// 敵基底クラス
	////////////////////////////////
	class CBase : virtual public IDoubleDispatch
	{
	public:	// 外部公開

		////////////////////////////////
		// 定数
		////////////////////////////////
		CONSTANT float	PARTICLE_POWER;			// パーティクル移動の強さ
		CONSTANT float	FLOAT_POWER_LINE;		// 行動を変える浮力の力
		CONSTANT float	FLOAT_SPEED;			// 浮く速さ
		CONSTANT float	SCALE_UP_SPEED;			// 拡大の速さ
		CONSTANT float	DEFAULT_FLAOT_POWER;	// 標準の浮力
		CONSTANT float	DEFAULT_HITPOWER_OFF;	// 減る力
		CONSTANT float	DEFAULT_ROTSPEED;		// 回転する速さ
		CONSTANT BYTE	ERASE_LENGTH;			// 消える距離
		CONSTANT BYTE	BUBBLE_SCALE_RATE;		// 泡の拡大率
		CONSTANT LPSTR	BUBBLE_MESH_PASS;		// 敵を包む泡のメッシュパス

		////////////////////////////////
		// フラグ定数
		////////////////////////////////
		CONSTANT BYTE ERASE;		// 消す
		CONSTANT BYTE BREAK;		// 壊れている
		CONSTANT BYTE BUBBLE_HIT;	// 泡に当たっている
		CONSTANT BYTE SEARCH;		// 探している
		CONSTANT BYTE ATTACK;		// 攻撃中
		CONSTANT BYTE BUBBLE_WRAP;	// 泡につつまれきっている
		CONSTANT BYTE JUMP;			// ジャンプ中
	
		////////////////////////////////
		// メソッド
		////////////////////////////////
		
		// コンストラクタ
		CBase();

		// 仮想デストラクタ
		virtual ~CBase();		

		//////////////////////////////////////////////////
		//	・関数名	Update
		//	・説明		更新を行う(派生先で定義)
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		virtual void Update() = 0;

		//////////////////////////////////////////////////
		//	・関数名	Draw
		//	・説明		描画を行う(派生先で定義)
		//	・引数		
		//		- lp_device	デバイス
		//		- is_shader	プログラマブルシェーダで描画するかどうか
		//	・戻り値	なし
		//////////////////////////////////////////////////
		virtual void Draw( const LPDIRECT3DDEVICE9 lp_device, bool is_shader ) = 0;

		//////////////////////////////////////////////////
		//	・関数名	DrawEffect
		//	・説明		エフェクト描画を行う(派生先であれば定義)
		//	・引数		
		//		- lp_device	デバイス
		//	・戻り値	なし
		//////////////////////////////////////////////////
		virtual void DrawEffect( const LPDIRECT3DDEVICE9 lp_device ){}

		//////////////////////////////////////////////////
		//	・関数名	GetScore
		//	・説明		点数を獲得(派生先で決める)
		//	・引数		なし
		//	・戻り値	点数を返す
		//////////////////////////////////////////////////
		virtual ULONG GetScore() const = 0;

		//////////////////////////////////////////////////
		//	・関数名	GetMesh
		//	・説明		メッシュ情報を獲得
		//	・引数		なし
		//	・戻り値	メッシュ情報を返す
		//////////////////////////////////////////////////
		virtual const CXfile* GetMesh() const = 0;

		//////////////////////////////////////////////////
		//	・関数名	GetShotManager
		//	・説明		弾マネージャーを獲得
		//				持つかどうかは派生先で決める
		//	・引数		なし
		//	・戻り値	弾マネージャーを返す
		//////////////////////////////////////////////////
		virtual Shot::CManager* GetShotManager()
		{
			return NULL;
		}

		//////////////////////////////////////////////////
		//	・関数名	DrawRipples
		//	・説明		波紋を描画する
		//	・引数		
		//		- lp_device	デバイス
		//	・戻り値	なし
		//////////////////////////////////////////////////
		inline void DrawRipples( const LPDIRECT3DDEVICE9 lp_device )
		{
			m_Ripples.Draw( lp_device );
		}

		//////////////////////////////////////////////////
		//	・関数名	SetPlayerInfo
		//	・説明		様々なことに使うために
		//				プレイヤーを設定する
		//	・引数		
		//		- info	プレイヤーのポインタ
		//	・戻り値	なし
		//////////////////////////////////////////////////
		inline static void SetPlayerInfo( Player::CDolphin* info )
		{
			m_lpPlayer = info;
		}

		//////////////////////////////////////////////////
		//	・関数名	SetStoneManager
		//	・説明		衝突判定を行うための
		//				岩マネージャーをを設定する
		//	・引数		
		//		- lp_stones	岩マネージャーのポインタ
		//	・戻り値	なし
		//////////////////////////////////////////////////
		inline static void SetStoneManager( CStoneManager* lp_stones )
		{
			m_lpStone = lp_stones;
		}

		//////////////////////////////////////////////////
		//	・関数名	SetFishManager
		//	・説明		襲うために
		//				魚マネージャーをを設定する
		//	・引数		
		//		- lp_fishes	魚マネージャーのポインタ
		//	・戻り値	なし
		//////////////////////////////////////////////////
		inline static void SetFishManager( CFishManager* lp_fishes )
		{
			m_lpFishes = lp_fishes;
		}

		//////////////////////////////////////////////////
		//	・関数名	GetFlg
		//	・説明		フラグ(情報)を獲得
		//	・引数		なし
		//	・戻り値	フラグ(情報)を返す
		//////////////////////////////////////////////////
		inline BYTE GetFlg() const
		{
			return m_Flg;
		}

		//////////////////////////////////////////////////
		//	・関数名	GetMatrix
		//	・説明		行列を獲得
		//	・引数		なし
		//	・戻り値	行列のポインタを返す
		//////////////////////////////////////////////////
		inline D3DXMATRIX* GetMatrix()
		{
			return &m_mMat;
		}

		//////////////////////////////////////////////////
		//	・関数名	GetPos
		//	・説明		位置を獲得
		//	・引数		なし
		//	・戻り値	位置を返す
		//////////////////////////////////////////////////
		D3DXVECTOR3 GetPos();

		//////////////////////////////////////////////////
		//	・関数名	VacuumBubble
		//	・説明		BubbleBreakerだけが使う
		//				泡を吸い取る
		//	・引数		
		//		- rate	どれだけ吸い取るかの値
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void VacuumBubble( float rate );

		//////////////////////////////////////////////////
		//	・関数名	GetBubbleScale
		//	・説明		泡の大きさを獲得
		//	・引数		なし
		//	・戻り値	泡の大きさを返す
		//////////////////////////////////////////////////
		inline float GetBubbleScale() const 
		{
			return m_FloatPower;
		}

		//////////////////////////////////////////////////
		//	・関数名	Action
		//	・説明		弾に対しての行動を起こす
		//				Dispatchから呼ばれる
		//	・引数		行動を起こす対象のオブジェクト
		//	・戻り値	なし
		//////////////////////////////////////////////////
		virtual void Action( Shot::CBase& object );

		//////////////////////////////////////////////////
		//	・関数名	Action
		//	・説明		敵に対しての行動を起こす
		//				Dispatchから呼ばれる
		//	・引数		行動を起こす対象のオブジェクト
		//	・戻り値	なし
		//////////////////////////////////////////////////
		virtual void Action( Enemy::CBase& object );

	protected:	// 共通メンバ
		
		//////////////////////////////////////////////////
		// 定数
		//////////////////////////////////////////////////
		CONSTANT int	BURST_WIDTH;			// 爆発画像、幅
		CONSTANT int	BURST_HEIGHT;			// 爆発画像、高さ
		CONSTANT int	BURST_ANIME_SIZE;		// 爆発画像、アニメ１つの大きさ
		CONSTANT int	BURST_ANIME_WIDTH_NUM;	// 爆発画像、幅アニメ数
		CONSTANT int	BURST_ANIME_HEIGHT_NUM;	// 爆発画像、高さアニメ数
		CONSTANT int	BURST_ANIME_NUM;		// 爆発画像、アニメ総数

		//////////////////////////////////////////////////
		// フィールド
		//////////////////////////////////////////////////
		CMatrix					m_mMat;			// 行列(ローカル前提)
		CMatrix					m_mBalloon;		// 風船行列
		BYTE					m_Flg;			// フラグ
		BYTE					m_AnimeCnt;		// アニメカウンタ
		float					m_FloatPower;	// 浮力
		CBubbleManager			m_Bubbles;		// 泡マネージャ
		CPointSpriteManager		m_Particle;		// パーティクル用
		CWaterSpray*			m_lpSpray;		// 水しぶき
		CVector					m_HitDir;		// 当たって飛んでいく方向
		float					m_HitPower;		// 吹っ飛びの力
		CRippleManager			m_Ripples;		// 波紋
		CDamageObjectBase*		m_lpTargetFish;	// ターゲット格納用

		//////////////////////////////////////////////////
		// メソッド
		//////////////////////////////////////////////////
		
		//////////////////////////////////////////////////
		//	・関数名	UpdateRot
		//	・説明		プレイヤーの方向に回転する
		//	・引数		
		//		- rotmax	回転最大角度
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateRot( float rotmax = DEFAULT_ROTSPEED );

		//////////////////////////////////////////////////
		//	・関数名	UpdateFloatPower
		//	・説明		浮力更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateFloatPower();

		//////////////////////////////////////////////////
		//	・関数名	UpdateBubbleScale
		//	・説明		包まれる泡の大きさ更新
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateBubbleScale();

		//////////////////////////////////////////////////
		//	・関数名	UpdateHitMove
		//	・説明		吹き飛び更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateHitMove();

		//////////////////////////////////////////////////
		//	・関数名	DrawBreakEnemy
		//	・説明		死んだ敵を描画する
		//	・引数		
		//		- lpdevice	デバイス
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void DrawBreakEnemy( const LPDIRECT3DDEVICE9 lpdevice );
		
		//////////////////////////////////////////////////
		//	・関数名	DrawBubble
		//	・説明		泡を描画する
		//	・引数		
		//		- lpdevice	デバイス
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void DrawBubble( const LPDIRECT3DDEVICE9 lpdevice );

		//////////////////////////////////////////////////
		//	・関数名	DrawShader
		//	・説明		プログラマブルシェーダで描画する
		//	・引数		
		//		- lpdevice	デバイス
		//		- mesh		メッシュ
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void DrawShader( const LPDIRECT3DDEVICE9 lpdevice, CXfile* mesh );

		//////////////////////////////////////////////////
		//	・関数名	UpdateMust
		//	・説明		派生先で必ず呼び出して欲しい処理
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateMust();

		//////////////////////////////////////////////////
		//	・関数名	UpdateCollisionMove
		//	・説明		衝突判定を行いながら移動更新を行う
		//	・引数		
		//		- move	移動量
		//		- limit	上限
		//	・戻り値	衝突すればtrue
		//				そうでなければfalseを返す
		//////////////////////////////////////////////////
		bool UpdateCollisionMove( const D3DXVECTOR3* move, const float limit );

		//////////////////////////////////////////////////
		//	・関数名	SearchTarget
		//	・説明		襲う目標を決める
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void SearchTarget();

		//////////////////////////////////////////////////
		//	・関数名	UpdateJump
		//	・説明		ジャンプ更新を行う
		//				水面より下回ればフラグをたたむ
		//	・引数		
		//		- speed	進む速さ
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateJump( float speed );

		//////////////////////////////////////////////////
		//	・関数名	JudgeJump
		//	・説明		ジャンプ処理を行うかどうか
		//				ジャンプフラグを立てたり
		//				水しぶき初期化したりする
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void JudgeJump();
		
		//////////////////////////////////////////////////
		//	・関数名	GetPlayerInfo
		//	・説明		派生先がプレイヤー情報を獲得
		//	・引数		なし
		//	・戻り値	プレイヤーのポインタを返す
		//////////////////////////////////////////////////
		inline Player::CDolphin* GetPlayerInfo() const
		{
			return m_lpPlayer;
		}

		//////////////////////////////////////////////////
		//	・関数名	GetStoneManager
		//	・説明		派生先が岩マネージャー情報を獲得
		//	・引数		なし
		//	・戻り値	岩マネージャーのポインタを返す
		//////////////////////////////////////////////////
		inline CStoneManager* GetStoneManager()
		{
			return m_lpStone;
		}

		//////////////////////////////////////////////////
		//	・関数名	GetFishManager
		//	・説明		派生先が魚マネージャー情報を獲得
		//	・引数		なし
		//	・戻り値	魚マネージャーのポインタを返す
		//////////////////////////////////////////////////
		inline CFishManager* GetFishManager()
		{
			return m_lpFishes;
		}

	private:	// 非公開

		//////////////////////////////////////////////////
		// フィールド
		//////////////////////////////////////////////////
		static Player::CDolphin*	m_lpPlayer;		// プレイヤー情報
		static CStoneManager*		m_lpStone;		// 岩の情報
		static CFishManager*		m_lpFishes;		// 魚情報
		float						m_NowFloatSpeed;// 浮力の加速度
		float						m_NowScale;		// 今の大きさ
		float						m_JumpSpeed;	// ジャンプ中の速さ
	};
}

#endif	// __AKI_ENEMYBASE_H__