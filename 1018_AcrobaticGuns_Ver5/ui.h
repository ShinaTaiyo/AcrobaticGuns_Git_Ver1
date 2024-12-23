//=====================================================================================
//
//９月１０日：タイトルを作る[ui.h]
//Author:ShinaTaiyo
//
//=====================================================================================1

#ifndef _UI_H_  
#define _UI_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
#include "number.h"
//==========================================

//======================
//前方宣言
//======================
class CObject2D;
class CUi;

//==========================================
//UIステート
//==========================================
class CUiState
{
public:
	CUiState();                 //コンストラクタ
	virtual ~CUiState();        //デストラクタ
	virtual void Process(CUi* pUi);//処理         
};
//=======================================================================================

//==========================================
//UIステート：数字保持
//==========================================
class CUiState_Numeric : public CUiState
{
public:
	CUiState_Numeric(CUi* pUi, int nValue, float fWidth, float fHeight);//コンストラクタ
	~CUiState_Numeric() override;//デストラクタ
	void Process(CUi* pUi) override;//処理
private:
	vector<CNumber*> m_VecNum;//数字のベクター
	int m_nValue;              //値
};
//=======================================================================================

//==========================================
//UIクラス
//==========================================
class CUi : public CObject2D
{
public:
	enum class UITYPE
	{
		LOCKON = 0,      //ロックオン
		ACTIONMODEWINDOW,//アクションモードウインドウ
	    ACTIONMODE_GUN,  //ショットモードUI
		ACTIONMODE_DIVE, //ダイブモードUI
		PHASETEXT,       //フェーズ文字
		STAGETEXT,       //ステージ文字
	    TARGET_000,      //ターゲット
		MAX
	};

	typedef enum
	{
		UIMOVETYPE_NORMAL = 0,//普通の動き
		UIMOVETYPE_FLOATING,  //浮遊させる
		UIMOVETYPE_MAX
	}UIMOVETYPE;

	CUi(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::UI, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);                      //コンストラクタ
	~CUi() override;             //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CUi * Create(UITYPE type,CObject2D::POLYGONTYPE PolygonType,float fWidth, float fHeight,int nLife,bool bUseLife,D3DXVECTOR3 Pos,D3DXVECTOR3 Move,D3DXVECTOR3 Rot,D3DXCOLOR col);//背景の生成
	void SetUiType(UITYPE type);
	void SetUseUiEffect(bool bUse, int nSetEffectLife, D3DXCOLOR col) { m_bUseUiEffect = bUse; m_nSetUiEffectLife = nSetEffectLife; m_SetUiEffectColor = col; }
	UITYPE GetUiType() { return m_Type; }//UIの種類を取得
	void SetNumericState(int nValue, float fWidth, float fHeight);//数字状態を設定する
protected:
	static const string UI_FILENAME[int(UITYPE::MAX)];//UIのテクスチャファイル名
private:
	UITYPE m_Type;                 //背景の種類
	UIMOVETYPE m_MoveType;         //UIの動き方の種類
	bool m_bUseUiEffect;           //UIでエフェクトを出すかどうか

	D3DXCOLOR m_SetUiEffectColor;  //UIで出すエフェクトの色合い設定用
	int m_nSetUiEffectLife;        //UIのエフェクトの体力設定用
	CUiState* m_pUiState;          //UIの状態
};
//=======================================================================================

//==========================================
//UIエフェクトクラス
//==========================================
class CUiEffect : public CUi
{
public:
	CUiEffect();                //コンストラクタ
	virtual ~CUiEffect();       //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CUiEffect* Create(UITYPE type, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight,int nLife,D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, D3DXCOLOR col);//背景の生成
private:
};
//=======================================================================================
#endif