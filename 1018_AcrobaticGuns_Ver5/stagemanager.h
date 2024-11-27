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
#include "main.h"
#include "object.h"
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
	enum class FOCUSTYPE
	{
		FOCUSTYPE_NORMAL = 0,//カメラが追う位置が普通
		FOCUSTYPE_WARPPOS,   //カメラがワープ先の位置を追う
	};

	enum class WORLDTYPE
	{
		EASY = 0,
		NORMAL,
		BOSSRUSH,
		MAX
	};

	CStageManager(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::STAGEMANAGER, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);  //コンストラクタ
	~CStageManager(); //デストラクタ
	HRESULT Init() override;   //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;    //更新処理
	void Draw() override;      //描画処理
	void SetDeath() override;  //死亡フラグを設定
	void LoadMapTxt(int nMapNum);               //マップをテキストファイルからロードする
	void LoadMapFilePass(WORLDTYPE type);           //マップのファイルパスをロードする
	void SaveMapTxt(int nMapNum);               //マップをテキストファイルにセーブする
	void LoadMapBin(int nMapNum);               //マップをバイナリファイルからロードする
	void SaveMapBin();                          //マップをバイナリファイルにセーブする
	int GetMapIndex() { return m_nMapIndex; }   //現在のマップ番号を取得する

	CObject* GetStageManagerObject() { return m_pManagerObject; }//操作オブジェクトの取得

	static CStageManager* Create();             //生成処理

	static const int m_nMAXMANAGEROBJECT = 1024;//管理するブロックの最大数
private:

	enum class MANAGERMODE
	{
		ALREADYSTAGE = 0,//既にあるステージを編集するモード
		NEWSTAGE,        //新しくステージを作るモード
		MAX
	};

	static const int m_nMAX_MAP = 20;
	static const int m_nMAX_WORD = 126;
	static const char* m_apWORLDMAP_TXT[static_cast<int>(WORLDTYPE::MAX)];
	static const string m_aSAVE_FILENAME;//保存するファイル名

	void TypeChenge();                          //オブジェクトXの種類を変える
	void SetObjectX();                          //オブジェクトXを設定する
	void DeleteManagerObject();                       //オブジェクトXを消す
	void ChengeObject(CObject::MANAGEROBJECTTYPE ManagerObjectType);                        //オブジェクトの種類を消す
	void ReleaseObject();                       //オブジェクトをリリースする
	void MapChenge();                           //マップを変える
	void DispInfo();                            //現在のマップエディタの情報を表示
	void ChooseObject();                        //オブジェクト選択処理

	//====================
	//基本系
	//====================
	D3DXVECTOR3 m_SaveRot;                          //向き
	D3DXVECTOR3 m_SavePos;                          //位置
	D3DXVECTOR3 m_SaveScale;                        //拡大率
	D3DXVECTOR3 m_SaveBeforeChoosePos;          //選択処理をする前のする位置
	CObject* m_pManagerObject;                  //色々な派生クラスにキャストするオブジェクト
	MANAGERMODE m_ManagerMode;                  //現在のステージマネーシャーのモード
	char m_aMapFilePass[m_nMAX_MAP][m_nMAX_WORD];//マップのファイルパス
	//========================================================================================

	//====================
	//番号
	//====================
	int m_nMapIndex;                            //マップのインデックス
	int m_nWorldIndex;                          //ワールドのインデックス
	int m_nMapNum;                              //マップの総数
	//========================================================================================
	
	//====================
	//選択系
	//====================
	CObject* m_pChooseObject;                   //選択オブジェクト
	bool m_bChooseObject;                       //オブジェクトを選択しているかどうか
	//========================================================================================

	//===================
	//モード関係
	//===================
	bool m_bMakeMapMode;                        //マップを作るモード
	bool m_bUseSizeMove;//現在のオブジェクトのサイズ分移動するかどうか
	//========================================================================================

	//===================
	//拡大率関係
	//===================
	void ResetScale();//拡大率をリセットする処理
	//========================================================================================

	//===================
	//オブジェクトリスト
	//===================
	list <CObject*> m_StgObjList;//オブジェクトのリスト
	list <CObject*>::iterator m_StgObjIt;//ステージオブジェクトリストへのイテレータ
	//========================================================================================

	//===================
	//3D背景
	//===================
	CBg3D* m_pBg3D;//背景へのポインタ
	//========================================================================================
};
#endif