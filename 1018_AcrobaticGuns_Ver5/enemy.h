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
//==========================================

//===========================================
//エネミークラス
//===========================================
class CEnemy : public CObjectXAlive
{
public:
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
	//=================================================================================================================
protected:
	//================================================
    //静的メンバ
    //================================================
	//static const string m_aENEMY_FILENAME[static_cast<int>(ENEMYTYPE::MAX)];//敵のモデルファイル名
    //===============================================================================================

	//================================================
	//プロトタイプ宣言
	//================================================

	//===============================================================================================

	//================================================
	//変数宣言
	//================================================
	void SetEnemyType(ENEMYTYPE type) { m_Type = type; }//敵の種類を設定
	//===============================================================================================
private:
	//================================================
	//静的メンバ
	//================================================

	//===============================================================================================

	//================================================
	//変数宣言
	//================================================
	ENEMYTYPE m_Type;//敵の種類
	//===============================================================================================

	//================================================
	//プロトタイプ宣言
	//================================================
	void CollisionProcess();//当たり判定を行う
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
	static CShotWeakEnemy* Create(SHOTWEAKENEMYTYPE Type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);

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
	static CDiveWeakEnemy* Create(DIVEWEAKENEMYTYPE Type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);

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

	//===============================================================================================
};



#endif
