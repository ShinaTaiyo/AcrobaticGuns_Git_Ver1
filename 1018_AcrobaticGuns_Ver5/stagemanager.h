//==========================================================================================
//
//７月１７日：配置ツールを作る[stagemanager.h]
//Author:ShinaTaiyo
//
//==========================================================================================

#ifndef _STAGEMANAGER_H_  
#define _STAGEMANAGER_H_

//============================
//インクルード
//============================
#include "player.h"
//========================================================

//============================
//前方宣言
//============================
class CObjectXInfo;
class CBg3D;
//======================
//マクロ定義
//======================
#define SAVE_BIN "data\\BINARYFILE\\Save.bin"
#define SAVE_TXT "data\\TEXTFILE\\stagemanager\\Save.txt"
//============================
//ステージ管理クラス
//============================
class CStageManager : public CObject
{
public:
	typedef enum
	{
		MANAGEROBJECT_BLOCK = 0,//ブロック
		MANAGEROBJECT_MODEL,    //モデル
		MANAGEROBJECT_ENEMY,    //敵
		MANAGEROBJECT_ITEM,     //アイテム
		MANAGEROBJECT_MARKER,   //マーカー
		MANAGEROBJECT_BOSS,     //ボス
		MANAGEROBJECT_TRAP,     //トラップ
		MANAGEROBJECT_SIGNBOARD,//立て看板
		MANAGEROBJECT_MAX
	}MANAGEROBJECT;

	typedef enum
	{
		FOCUSTYPE_NORMAL = 0,//カメラが追う位置が普通
		FOCUSTYPE_WARPPOS,   //カメラがワープ先の位置を追う
	}FOCUSTYPE;

	typedef enum
	{
		WORLDTYPE00_EASY = 0,
		WORLDTYPE01_NORMAL,
		WORLDTYPE02_BOSSRUSH,
		WORLDTYPE_MAX
	}WORLDTYPE;

	CStageManager();  //コンストラクタ
	~CStageManager(); //デストラクタ
	HRESULT Init() override;   //初期化処理
	void Uninit() override;     //終了処理
	void ExtraUninit() override;//別枠の終了処理
	void Update() override;    //更新処理
	void Draw() override;      //描画処理
	void SetDeath() override;  //死亡フラグを設定
	void LoadMapTxt(int nMapNum);               //マップをテキストファイルからロードする
	void LoadMapFilePass(WORLDTYPE type);           //マップのファイルパスをロードする
	void SaveMapTxt(int nMapNum);               //マップをテキストファイルにセーブする
	void LoadMapBin(int nMapNum);               //マップをバイナリファイルからロードする
	void SaveMapBin();                          //マップをバイナリファイルにセーブする
	int GetMapIndex() { return m_nMapIndex; }   //現在のマップ番号を取得する
	D3DXVECTOR3& GetPos() { return m_Pos; }      //位置を取得する
	FOCUSTYPE GetFocusType() { return m_FocusType; }//集中する位置を取得する
	static CStageManager* Create();             //生成処理
	static const int m_nMAXMANAGEROBJECT = 1024;//管理するブロックの最大数
private:

	typedef enum
	{
		MANAGERMODE_ALREADYSTAGE = 0,//既にあるステージを編集するモード
		MANAGERMODE_NEWSTAGE,        //新しくステージを作るモード
		MANAGERMODE_MAX
	}MANAGERMDOE;

	typedef enum
	{
		MOVEMODE00_XY = 0,//移動モード０：XY
		MOVEMODE01_XZ,    //移動モード１：XZ
		MOVEMODE_MAX
	}MOVEMODE;

	static const int m_nMAX_MAP = 20;
	static const int m_nMAX_WORD = 126;
	static const char* m_apWORLDMAP_TXT[WORLDTYPE_MAX];

	void MoveManager();                         //ステージマネージャーを動かす
	void LifeSet();                             //体力を設定する
	void TypeChenge();                          //オブジェクトXの種類を変える
	void SetObjectX();                          //オブジェクトXを設定する
	void DeleteObjectX();                       //オブジェクトXを消す
	void ChengeObject();                        //オブジェクトの種類を消す
	void ReleaseObject();                       //オブジェクトをリリースする
	void ColorChenge();                         //ステージマネージャーの色合いを設定する
	void MapChenge();                           //マップを変える
	void DispInfo();                            //現在のマップエディタの情報を表示
	void ChooseObject();                        //オブジェクト選択処理
	//====================
	//基本系
	//====================
	D3DXVECTOR3 m_Move;                         //移動量
	D3DXVECTOR3 m_Rot;                          //向き
	D3DXVECTOR3 m_SaveBeforeChoosePos;          //選択処理をする前のする位置
	CObject* m_pManagerObject;                  //色々な派生クラスにキャストするオブジェクト
	MANAGERMDOE m_ManagerMode;                  //現在のステージマネーシャーのモード
	D3DXVECTOR3 m_Pos;                          //位置
	D3DXVECTOR3 m_Scale;                        //拡大率
	char m_aMapFilePass[m_nMAX_MAP][m_nMAX_WORD];//マップのファイルパス
	//========================================================================================

	//====================
	//番号
	//====================
	int m_nMapIndex;                            //マップのインデックス
	int m_nWorldIndex;                          //ワールドのインデックス
	int m_nMapNum;                              //マップの総数
	int m_nIndexObject;                         //管理するオブジェクトの番号
	//========================================================================================
	
	//====================
	//選択系
	//====================
	CObject* m_pChooseObject;                   //選択オブジェクト
	bool m_bChooseObject;                       //オブジェクトを選択しているかどうか
	int m_nIndexChooseObject;                   //選択しているオブジェクトの番号
	//========================================================================================

	//===================
	//モード関係
	//===================
	MOVEMODE m_MoveMode;//移動モード
	FOCUSTYPE m_FocusType;//カメラが追う物の種類
	bool m_bMakeMapMode;                        //マップを作るモード
	bool m_bUseSizeMove;//現在のオブジェクトのサイズ分移動するかどうか
	D3DXVECTOR3 NotSizeMoveProcess(float fMoveX,float fMoveY);//サイズ分動かさないときの処理
	D3DXVECTOR3 SizeMoveProcess(float fMoveX,float fMoveY,D3DXVECTOR3 Size);//サイズ分動かすときの処理
	//========================================================================================

	//===================
	//拡大率関係
	//===================
	void ResetScale();//拡大率をリセットする処理
	//========================================================================================

	//===================
	//オブジェクトリスト
	//===================


	//===================
	//3D背景
	//===================
	CBg3D* m_pBg3D;//背景へのポインタ
	//========================================================================================
};
#endif