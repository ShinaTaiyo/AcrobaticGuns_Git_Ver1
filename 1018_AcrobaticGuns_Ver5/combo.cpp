//=====================================================================================================================
//
//２０２５年３月１８日：コンボを実装する[combo.cpp]
//Author:ShinaTaiyo
//
//=====================================================================================================================

//=====================================================================================================================
//インクルード
//=====================================================================================================================
#include "combo.h"
//=====================================================================================================================================================

//=====================================================================================================================
//静的メンバ宣言
//=====================================================================================================================
const D3DXVECTOR3 CCombo::s_COMBODISP_POS = { SCREEN_WIDTH - 240.0f, 200.0f, 0.0f };//コンボ表示の位置
//=====================================================================================================================================================

//=====================================================================================================================
//コンストラクタ
//=====================================================================================================================
CCombo::CCombo(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri,bUseintPri,type,ObjType)
{

}
//=====================================================================================================================================================

//=====================================================================================================================
//デストラクタ
//=====================================================================================================================
CCombo::~CCombo()
{

}
//=====================================================================================================================================================

//=====================================================================================================================
//初期化処理
//=====================================================================================================================
HRESULT CCombo::Init()
{
	m_pComboDisp = CUi::Create(CUi::UITYPE::COMBOTEXT_000,false, CObject2D::POLYGONTYPE::SENTERROLLING,
        s_fCOMBODISP_WIDTH,s_fCOMBODISP_HEIGHT, 1, false, s_COMBODISP_POS, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//コンボ表示の生成
	m_pComboDisp->SetUseDeath(false);      //死亡フラグを使用しない
	m_pComboDisp->GetUiCompositeContainer()->Add(DBG_NEW CUIComposite_Numeric(m_pComboDisp, 0, s_fCOMBODISP_VALUEWIDTH, s_fCOMBODISP_VALUEHEIGHT));//数字表示機能をつける
	CObject::Init();//オブジェクト初期化処理//
	return S_OK;
}
//=====================================================================================================================================================

//=====================================================================================================================
//終了処理
//=====================================================================================================================
void CCombo::Uninit()
{
	CObject::Uninit();//オブジェクト終了処理
}
//=====================================================================================================================================================

//=====================================================================================================================
//更新処理
//=====================================================================================================================
void CCombo::Update()
{
	CObject::Update();//オブジェクト更新処理
}
//=====================================================================================================================================================

//=====================================================================================================================
//描画処理
//=====================================================================================================================
void CCombo::Draw()
{
	CObject::Draw();//オブジェクト描画処理
}
//=====================================================================================================================================================

//=====================================================================================================================
//死亡フラグ設定処理
//=====================================================================================================================
void CCombo::SetDeath()
{
	if (GetUseDeath() == true)
	{//死亡フラグを使用するなら

		if (m_pComboDisp != nullptr)
		{//コンボ表示が存在していたら
			m_pComboDisp->SetUseDeath(true);//コンボ表示の死亡フラグを使用する
			m_pComboDisp->SetDeath();       //コンボ表示の死亡フラグを設定する
			m_pComboDisp = nullptr;         //コンボ表示のポインタを初期化する
		}
	}

	CObject::SetDeath();//オブジェクト死亡フラグ設定処理
}
//=====================================================================================================================================================

//=====================================================================================================================
//生成処理
//=====================================================================================================================
CCombo* CCombo::Create(D3DXVECTOR3 Pos)
{
	CCombo* pCombo = DBG_NEW CCombo();//コンボの生成
	pCombo->Init();                   //コンボ初期化処理
	return pCombo;
}
//=====================================================================================================================================================

//=====================================================================================================================
//コンボ加算処理
//=====================================================================================================================
void CCombo::AddCombo(int nCombo)
{
	m_nCombo += nCombo;//コンボ数を加算

	CUIComposite_Container* pContainer = m_pComboDisp->GetUiCompositeContainer();//コンポジットパターンのコンテナを取得
	CUIComposite_Numeric* pNumeric = pContainer->GetChildren<CUIComposite_Numeric>();//数字表示機能を取得

	if (pNumeric != nullptr)
	{//数字表示機能が存在していたら
		pNumeric->SetValue(m_nCombo, m_pComboDisp);//コンボ数を数字表示機能に設定し表示する
	}
}
//=====================================================================================================================================================]

//=====================================================================================================================
//コンボリセット処理
//=====================================================================================================================
void CCombo::ResetCombo()
{
	m_nCombo = 0;//コンボ数リセット

	CUIComposite_Container* pContainer = m_pComboDisp->GetUiCompositeContainer();//コンポジットパターンのコンテナを取得
	CUIComposite_Numeric* pNumeric = pContainer->GetChildren<CUIComposite_Numeric>();//数字表示機能を取得

	if (pNumeric != nullptr)
	{//数字表示機能が存在していたら
		pNumeric->SetValue(m_nCombo, m_pComboDisp);//表示する数字もリセット
	}

}
//=====================================================================================================================================================