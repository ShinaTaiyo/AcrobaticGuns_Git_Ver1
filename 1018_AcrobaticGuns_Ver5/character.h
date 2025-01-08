//===========================================================================
//
//２０２５年１月８日：モーション情報管理クラス実装[motionData.h]
//Author:ShinaTaiyo
//
//===========================================================================

//=====================================
//インクルード
//=====================================
#include "main.h"
#include "objectX.h"
#include "modelparts.h"
//========================================================================

//=====================================
//クラス
//=====================================
class CCharacter : public CObjectX
{
public:
	struct KEY
	{
		D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };//位置
		D3DXVECTOR3 Rot = { 0.0f,0.0f,0.0f };//向き
	};

	struct KEYSET
	{
		int nFrame = 0;    //フレーム数
		vector<KEY> VecKey;//各モデルパーツのキー情報
	};

	struct MOTION
	{
		bool bLoop = false;       //ループさせるかどうか
		int nNumKey = 0;          //キーの総数
		vector<KEYSET> VecKeySet; //キーセット情報
	};

	struct CHARACTERMOTIONDATA
	{
		string FileName = "";//モーションファイル名
		vector<MOTION> VecMotion;//モーション情報
	};

	CCharacter(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                                                           //コンストラクタ
	~CCharacter() override; //デストラクタ
	HRESULT Init() override;//初期化処理
	void Uninit() override; //終了処理
	void Update() override; //更新処理
	void Draw() override;   //描画処理
	void SetDeath() override;//死亡フラグ設定処理

	static int Regist(string String,CCharacter * pCharacter);//モーション情報のファイルパスを指定し、モーション情報のインデックスとモデルパーツを取得
private:
	//=======================
	//静的メンバ
	//=======================
	static vector<CHARACTERMOTIONDATA> s_VecMotionInfo;//モーション情報のベクター
	//===================================================================================

	//=======================
	//構造体
	//=======================
	struct NowMotionInfo
	{
		int nNowMotion = 0;//現在のモーション番号
		int nNextMotion = 0;//次のモーション
		int nMotionOld = 0;//1f前のモーション

		int nCntFrame = 0; //現在のフレーム数
		int nCntKey = 0;   //現在のキーをカウントする
	};
	//===================================================================================

	//=======================
	//変数
	//=======================
	int m_nIdxCharacter;//キャラクターモーション情報のインデックス
	NowMotionInfo m_NowMotionInfo;//モーション状態情報
	vector <CModelParts*> m_VecModelParts;//モデルパーツのベクター
	//===================================================================================

	//=======================
	//関数
	//=======================
	void MotionProcess();//モーション処理を行う
	void MotionInfoChengeProcess();//モーション情報を更新する処理

	//静的メンバ
	static void SaveMotion(string MotionFileName);//モーション情報を保存する
	static void LoadModelParts(string MotionFileName, CCharacter* pCharacter);//モデルパーツを読み込む
	//===================================================================================
};
//============================================================================================================
#pragma once
