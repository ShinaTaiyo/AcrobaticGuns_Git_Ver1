//====================================================
//
//７月１６日：オブジェクトのリスト管理[Object.cpp]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _OBJECT_H_  
#define _OBJECT_H_

//======================
//インクルード
//======================
#include "main.h"

//==========================================

//==========================================
//オブジェクトクラス
//==========================================
class CObject
{
public:
	//===========================
	//オブジェクト種類列挙型
	//===========================
	enum class TYPE
	{
		NONE,            //タイプなし
		BG3D,            //3D背景
		FIELD,           //床
		BLOCK,           //ブロック
		BGMODEL,         //背景モデル
		WIREHEAD,        //ワイヤー頭
		WIRE,            //ワイヤー
		PLAYER,          //プレイヤー
		ENEMY,           //敵
		BULLET,          //弾
		ATTACK,          //攻撃
		MESHORBIT,       //軌跡メッシュ
		AIMODEL,         //AIモデル
		EFFECT,          //エフェクト
		PARTICLE,        //パーティクル
		UI3D,            //UI3D
		NUMBER,          //数字
		NUMBER3D,        //数字3D
	    PARTICLE2D,      //パーティクル２D
		LOCKON,          //ロックオン
		GAUGE,           //ゲージ
		UI,              //UI
		PHASEINFO,       //フェーズ情報
		STAGEMANAGER,    //ステージマネージャー
		PHASEMANAGER,    //フェーズマネージャー
		EVENTMANAGER,    //イベントマネージャー
		TUTORIAL,        //チュートリアル
		BG,              //背景
		FADE,            //フェード
		MAX
	};
	//===========================================

	//====================================================
	//ステージマネージャー管理オブジェクトタイプ列挙型
	//====================================================
	enum class MANAGEROBJECTTYPE
	{
		NONE = 0, //なし
		BLOCK,    //ブロック
		BGMODEL,  //背景モデル
		SHOTWEAKENEMY,//射撃に弱い敵
		DIVEWEAKENEMY,//ダイブに弱い敵
		MAX
	};
	//===========================================

	//===========================
	//オブジェクト分類列挙型
	//===========================
	enum class OBJECTTYPE
	{
		OBJECTTYPE_NONE = 0,
		OBJECTTYPE_2D,
		OBJECTTYPE_3D,
		OBJECTTYPE_BILLBOARD,
		OBJECTTYPE_X,
		OBJECTTYPE_MAX
	};
	//===========================================

	CObject(int nPriority = 2,bool bUseintPriority = false,TYPE Type = TYPE::NONE,OBJECTTYPE ObjType = OBJECTTYPE::OBJECTTYPE_NONE);//描画優先設定(５月２８日New!：デフォルト引数（呼び出し時に引数設定しなければ３が代入されて処理される
	virtual ~CObject();        //デストラクタ
	virtual HRESULT Init() = 0;//初期化処理
	virtual void Uninit() = 0; //終了処理
	virtual void ExtraUninit();//別枠の終了処理
	virtual void Update() = 0; //更新処理
	virtual void Draw() = 0;   //描画処理
	bool GetUseDeath() { return m_bUseDeath; }//死亡フラグを発動するか決めるフラグを取得
	static void ReleaseAll();  //全オブジェクト開放
	static void UpdateAll();   //全オブジェクト更新
	static void DrawAll();     //全オブジェクト描画
	virtual void SetDeath();                                              //死亡フラグを設定
	//=====================
	//取得系
	//=====================
	static int GetNumAll();                              //全オブジェクトの総数を取得する
	bool GetCreateSuccess() { return m_bCreateSuccess; } //オブジェクトの生成に成功したかどうか
	int GetId() { return m_nID; }                        //現在のIDを取得する
	static CObject* GetCObject(int nIdx,int nPriority);  //オブジェクト取得
	TYPE GetType();                                      //タイプ取得
	//==============================================================================================
	void SetType(TYPE type);                             //タイプ設定
	void SetStageManagerObjNum(int nNum) { m_nStageManagerObjNum = nNum; }            //ステージマネージャーで管理されているオブジェクトの番号を設定
	int GetStageManagerObjNum() { return m_nStageManagerObjNum; }                     //ステージマネージャーで管理されているオブジェクトの番号を取得する
	static const int m_nMAXOBJECT = 1024;                                             //オブジェクト最大数
	static const int m_nMAXPRIORITY = static_cast<int>(TYPE::MAX);                    //描画順最大数
	static void ReleaseProcess();                                                     //リストの破棄をする処理
	void Release();                                                                   //自分自身の開放
	void SetUseDeath(bool bDeath) { m_bUseDeath = bDeath; }                           //死亡フラグを発動するかどうかを設定する
	static void SetReleaseAll() { m_bActivationReleaseAll = true; }                   //ReleaseAllを発動させるフラグを設定する
	void SetObjectType(OBJECTTYPE type) { m_ObjectType = type; }                      //オブジェクトの分類を決める
	OBJECTTYPE GetObjectType() { return m_ObjectType; }                               //オブジェクトの分類を取得する
	CObject* GetNextObject() {return m_pNext; }                                       //次のオブジェクトを取得する
	CObject* GetPrevObject() { return m_pPrev; }                                      //前のオブジェクトを取得する

	//==============================
	//ステージマネージャー関係
	//==============================
	CObject* GetNextStageManagerObject() { return m_pStageManagerObj_Next; }          //ステージマネージャーで管理しているオブジェクトの次のオブジェクトを取得する
	CObject* GetPrevStageManagerObject() { return m_pStageManagerObj_Prev; }          //ステージマネージャーで管理しているオブジェクトの前のオブジェクトを取得する
	static CObject* GetTopStageManagerObject() { return m_pStageManagerObj_Top; }     //ステージマネージャーで管理しているオブジェクトのトップオブジェクトを取得する
	static CObject* GetCurStageManagerObject() { return m_pStageManagerObj_Cur; }     //ステージマネージャーで管理しているオブジェクトのカレントオブジェクトを取得する
	void SetStageManagerObj();                                                        //ステージマネージャーで管理するオブジェクトを設定

	static int GetTotalStageManagerObjNum() { return m_nNumStageManagerObject; }      //ステージマネージャーで管理しているオブジェクトの総数を取得
	static void StageManagerObjectReleaseAll();                                            //全てのマネージャー管理のオブジェクトを消す

	//分類用
	void SetManagerObjectType(MANAGEROBJECTTYPE Type) { m_ManagerObjectType = Type; }     //ステージマネージャー管理タイプを設定
	const MANAGEROBJECTTYPE & GetManagerObjectType() const { return m_ManagerObjectType; }  //ステージマネージャー管理タイプを取得
	//==============================================================================================

	//==============================
	//リスト関係
	//==============================
	static CObject* GetTopObject(int nPriority){ return m_pTop[nPriority];}           //トップオブジェクトを取得する
	static CObject* GetCurrentObject(int nPriority) { return m_pCur[nPriority];}      //カレントオブジェクトを取得する
	void SetStageManagerChoose(bool bChoose) { m_bStageManagerChooseState = bChoose; }//ステージマネージャーが選択しているかどうか
	bool GetStageManagerChoose() { return m_bStageManagerChooseState; }               //ステージマネージャーが選択しているかどうかを取得する
	//==============================================================================================

	//=================================================
	//フレーム関係
	//=================================================
	int GetCntFrame() { return m_nCntFrame; }                                         //出現してからのフレーム数をカウントする
	//================================================================================================

	//=================================================
	//エディタ関係
	//=================================================
	virtual void SaveInfoTxt(fstream& WritingFile);   //テキストファイルに情報を保存するための関数
	virtual void ManagerChooseControlInfo();          //情報操作
	virtual CObject * ManagerChengeObject(bool bAim); //ステージマネージャーに変更したオブジェクトを渡す
	virtual CObject * ManagerSaveObject();            //ステージマネージャーに今のオブジェクトを保存する
	//================================================================================================

	//=================================================
	//サウンド関係
	//=================================================
	void SetUseInitialSound(bool bUse) { m_bUseInitialSound = bUse; }//初期化時にサウンドを再生するかどうか
	bool GetUseInitialSound() { return m_bUseInitialSound; }         //初期化時にサウンドをさいせいするかどうかを取得
	//================================================================================================
protected:
private:
	void ReleaseStageManagerObj();                             //ステージマネージャーで管理しているオブジェクトを破棄
	static CObject* m_apObject[m_nMAXPRIORITY][m_nMAXOBJECT];//オブジェクト管理
	static bool m_bActivationReleaseAll;                     //ReleaseAllを発動するかどうか

	//===============================================
	//リスト管理
	//===============================================
	static CObject* m_pTop[m_nMAXPRIORITY];                  //先頭のオブジェクトのポインタ
	static CObject* m_pCur[m_nMAXPRIORITY];                  //現在（最後尾）のポインタ

	CObject* m_pNext;                                        //次のオブジェクトへのポインタ!
	CObject* m_pPrev;                                        //前のオブジェクトへのポインタ!

	bool m_bDeath;                                           //死亡フラグ!
	bool m_bUseDeath;                                        //死亡フラグを発動するかどうか!
	//===============================================================================================

	//===============================================
	//ステージマネージャー関係
	//===============================================
	static CObject* m_pStageManagerObj_Top;                  //ステージマネージャーで管理している先頭オブジェクトへのポインタ
	static CObject* m_pStageManagerObj_Cur;                  //ステージマネージャーで管理してい最後尾オブジェクトへのポインタ
	static int m_nNumStageManagerObject;                     //ステージマネージャーで管理されているオブジェクトの数

	CObject* m_pStageManagerObj_Next;                        //ステージマネージャーで管理している次のオブジェクトへのポインタ!
	CObject* m_pStageManagerObj_Prev;                        //ステージマネージャーで管理している前のオブジェクトへのポインタ!
	bool m_bIsStageManagerObj;                               //ステージマネージャーで管理されているオブジェクトかどうか!
	int m_nStageManagerObjNum;                               //ステージマネージャーで管理されているオブジェクトの番号!
	bool m_bStageManagerChooseState;                         //ステージマネージャーが選択しているかどうか!

	//分類用
	MANAGEROBJECTTYPE m_ManagerObjectType;                   //管理番号判別用

	//================================================================================================

	static int m_nNumAll;                                    //オブジェクト総数

    //=================================================
	//基本系
	//=================================================
	TYPE m_type;                                             //オブジェクト種類!
	OBJECTTYPE m_ObjectType;                                 //オブジェクト分類!
	int m_nID;                                               //自分自身のID!
	int m_nPriority;                                         //描画順を送る!
	bool m_bCreateSuccess;                                   //オブジェクトの生成が成功したかどうか!
	//================================================================================================

	//=================================================
	//フレーム関係
	//=================================================
	int m_nCntFrame;                                         //出現してからのフレーム数をカウントする!
	//================================================================================================

	//=================================================
	//サウンド関係
	//=================================================
	bool m_bUseInitialSound;//初期化時にサウンドを再生するかどうか!
	//================================================================================================
};

#endif
