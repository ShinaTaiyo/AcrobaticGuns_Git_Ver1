//====================================================
//
//１０月２９日：敵を作る[enemy.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _ENEMY_H_  
#define _ENEMY_H_

//======================
//インクルード
//======================
#include "main.h"
#include "objectXAlive.h"
#include "lockon.h"
#include "aimodel.h"
//==========================================

//===========================================
//前方宣言
//===========================================
class CEnemyMove;

//===========================================
//エネミークラス
//===========================================
class CEnemy : public CObjectXAlive
{
public:
	struct MoveAiInfo
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Rot;
		D3DXVECTOR3 Scale;
	};

	enum class ENEMYTYPE
	{
		SHOTWEAK = 0,
		DIVEWEAK,
		MAX
	};

	CEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                   //コンストラクタ
	~CEnemy();                  //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	//static CEnemy* Create(ENEMYTYPE Type,int nLife,D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 Scale);

	//==========================================================
    //エディタ関係
    //==========================================================
    //関数
	void SaveInfoTxt(fstream& WritingFile) override;  //テキストファイルに情報を保存するための関数
	static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //テキストファイルから情報を読み込むための関数   
	//CObject* ManagerChengeObject(bool bAim) override; //ステージマネージャーに変更したオブジェクトを渡す
	CObject* ManagerSaveObject() override;             //ステージマネージャーに今のオブジェクトを保存する
	void ManagerChooseControlInfo() override;          //ステージマネージャーから操作する
	void SetPhaseNum(int nNum) { m_nPhaseNum = nNum; } //フェーズ番号を設定する
	const int& GetPhaseNum() const { return m_nPhaseNum; }//フェーズ番号を取得する   
 	//=================================================================================================================

	//==========================================================
	//移動AI
	//==========================================================
	void SetVecMoveAiInfo(vector<CAIModel*>& vec);
	vector<CAIModel*>& GetVecAiModelInfo() { return m_VecMoveAi; }
	void AIMoveProcess();//AI移動処理
	virtual void BattleMoveProcess();//バトル移動処理
	void ChengeMove(CEnemyMove* pEnemyMove);//移動状態を変える
	void RayCollision();                               //レイがオブジェクトに当たっているかどうか
	//=================================================================================================================

	static int GetNumEnemy() { return m_nNumEnemy; }
protected:
	//struct  MoveAiInfo
	//{
	//	D3DXVECTOR3 Pos;
	//	D3DXVECTOR3 Rot;
	//	D3DXVECTOR3 Scale;
	//};

	//struct PhaseSaveInfo
	//{
	//	D3DXVECTOR3 Pos;     //位置
	//	D3DXVECTOR3 Rot;     //向き
	//	D3DXVECTOR3 Scale;   //拡大率
	//	int nLife;           //体力
	//	ENEMYTYPE EnemyType; //敵タイプ
	//	int nTypeNum;        //クラスごとのタイプ
	//	int nPhaseNum;       //フェーズ番号を設定

	//	vector<MoveAiInfo> VecMoveAi;//移動AIのVector
	//};
	////================================================
 //   //静的メンバ
 //   //================================================
	//static list<PhaseSaveInfo>
 //   //===============================================================================================

	//================================================
	//変数宣言
	//================================================
	void SetEnemyType(ENEMYTYPE type) { m_Type = type; }//敵の種類を設定
	//===============================================================================================
	
	//================================================
	//プロトタイプ宣言
	//================================================

	//===============================================================================================

private:
	//================================================
	//静的メンバ
	//================================================
	static int m_nNumEnemy;//敵の総数
	//===============================================================================================

	//================================================
	//変数宣言
	//================================================
	ENEMYTYPE m_Type;//敵の種類
	vector<CAIModel*> m_VecMoveAi;//移動AIモデル
	D3DXVECTOR3 m_MoveAiSavePos;//移動AIの保存する位置
	int m_nIdxMoveAi;           //移動AIの移動位置の配列を指定する
	int m_nPhaseNum;            //フェーズ番号
	CEnemyMove* m_pEnemyMove;   //敵の移動ステート
	//===============================================================================================

	//================================================
	//プロトタイプ宣言
	//================================================
	void CollisionProcess();//当たり判定を行う
	void SetMoveAiPoint();  //移動AIの配置をする
	void PhaseNumDecision();//フェーズ番号を設定する
	//===============================================================================================
};

//===========================================
//射撃に弱いエネミークラス
//===========================================
class CShotWeakEnemy : public CEnemy
{
public:
	enum class SHOTWEAKENEMYTYPE
	{
		NORMAL = 0,
		MAX
	};

	CShotWeakEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                   //コンストラクタ
	~CShotWeakEnemy();                  //デストラクタ
	HRESULT Init() override;            //初期化処理
	void Uninit() override;             //終了処理
	void Update() override;             //更新処理
	void Draw() override;               //描画処理
	void SetDeath() override;           //死亡フラグを設定
	static CShotWeakEnemy* Create(SHOTWEAKENEMYTYPE Type, int nLife,int nPhaseNum,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);

	//==========================================================
    //エディタ関係
    //==========================================================
    //関数
	void SaveInfoTxt(fstream& WritingFile) override;  //テキストファイルに情報を保存するための関数
	static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //テキストファイルから情報を読み込むための関数   
	CObject* ManagerChengeObject(bool bAim) override; //ステージマネージャーに変更したオブジェクトを渡す
	CObject* ManagerSaveObject() override;             //ステージマネージャーに今のオブジェクトを保存する
	//=================================================================================================================
protected:
private:
	//================================================
	//静的メンバ
	//================================================
	static const string s_aSHOTWEAKENEMY_FILENAME[static_cast<int>(SHOTWEAKENEMYTYPE::MAX)];//敵のモデルファイル名
	//===============================================================================================

	//================================================
	//変数宣言
	//================================================
	SHOTWEAKENEMYTYPE m_ShotWeakEnemyType;//敵の種類
	//===============================================================================================

	//================================================
	//プロトタイプ宣言
	//================================================

	//*移動AI
	void BattleMoveProcess() override;//バトル移動処理

	//===============================================================================================
};

//===========================================
//ダイブに弱いエネミークラス
//===========================================
class CDiveWeakEnemy : public CEnemy
{
public:
	enum class DIVEWEAKENEMYTYPE
	{
		NORMAL = 0,
		MAX
	};

	CDiveWeakEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                   //コンストラクタ
	~CDiveWeakEnemy();                  //デストラクタ
	HRESULT Init() override;            //初期化処理
	void Uninit() override;             //終了処理
	void Update() override;             //更新処理
	void Draw() override;               //描画処理
	void SetDeath() override;           //死亡フラグを設定
	static CDiveWeakEnemy* Create(DIVEWEAKENEMYTYPE Type, int nLife,int nPhaseNum,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);

	//==========================================================
    //エディタ関係
    //==========================================================
    //関数
	void SaveInfoTxt(fstream& WritingFile) override;  //テキストファイルに情報を保存するための関数
	static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //テキストファイルから情報を読み込むための関数   
	CObject* ManagerChengeObject(bool bAim) override; //ステージマネージャーに変更したオブジェクトを渡す
	CObject* ManagerSaveObject() override;             //ステージマネージャーに今のオブジェクトを保存する
	//=================================================================================================================
protected:
private:
	//================================================
	//静的メンバ
	//================================================
	static const string s_aDIVEWEAKENEMY_FILENAME[static_cast<int>(DIVEWEAKENEMYTYPE::MAX)];//敵のモデルファイル名
	//===============================================================================================

	//================================================
	//変数宣言
	//================================================
	DIVEWEAKENEMYTYPE m_DiveWeakEnemyType;//敵の種類
	//===============================================================================================

	//================================================
	//プロトタイプ宣言
	//================================================
	
	//*移動AI
	void BattleMoveProcess() override;//バトル移動処理

	//===============================================================================================
};

//**************************************************************************************
//敵の状態クラス
//**************************************************************************************

//================================
//移動クラス
//================================

//スーパークラス
class CEnemyMove
{
public:
	CEnemyMove();//コンストラクタ
	virtual ~CEnemyMove();//デストラクタ
	virtual void Process(CEnemy* pEnemy);//処理
};

//移動タイプ：AI
class CEnemyMove_AI : public CEnemyMove
{
public:
	CEnemyMove_AI();//コンストラクタ
	~CEnemyMove_AI() override;//デストラクタ
	void Process(CEnemy* pEnemy) override;
};

//移動タイプ：バトル
class CEnemyMove_Battle : public CEnemyMove
{
public:
	CEnemyMove_Battle();//コンストラクタ
	~CEnemyMove_Battle() override;//デストラクタ
	void Process(CEnemy* pEnemy) override;
};
//===================================================================================================================================

#endif
