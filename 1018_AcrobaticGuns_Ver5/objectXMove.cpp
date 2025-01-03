//===========================================================================================
//
//１０月１８日：モデルを描画する[objectXMove.cpp]
//Author:ShinaTaiyo
//
//===========================================================================================

//===============================
//インクルード
//===============================
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "debugproc.h"
#include <stdio.h>
#include <string.h>
#include "objectXMove.h"
//===========================================================================================

//===========================================================================================
//コンストラクタ
//===========================================================================================
CObjectXMove::CObjectXMove(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectX(nPri,bUseintPri,type,ObjType),m_bUseAddSpeed(false), m_bUseGravity(false), m_bUseInteria(false), m_bUseMultiSpeed(false), m_fGravityPower(0.0f), m_fInertia(0.0f),
m_MultiSpeed(D3DXVECTOR3(0.0f,0.0f,0.0f)), m_AddSpeed(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_Move(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_bUseUpdatePos(true),
m_bAdjustPos(true),m_AddMove(D3DXVECTOR3(0.0f,0.0f,0.0f))
{

}
//===============================================================================================================================================================

//===========================================================================================
//デストラクタ
//===========================================================================================
CObjectXMove::~CObjectXMove()
{

}
//===============================================================================================================================================================

//===========================================================================================
//初期化処理
//===========================================================================================
HRESULT CObjectXMove::Init()
{
	//===========================================
	//慣性関係
	//===========================================
	m_fInertia = m_fNORMAL_INERTIA;                    //慣性
	m_bUseInteria = true;                              //慣性をかけるかどうか
	//==================================================================================

	//===========================================
	//重力関係
	//===========================================
	m_bUseGravity = true;                              //重力を使用するかどうか
	m_fGravityPower = 0.0f;                            //重力の大きさ
	//==================================================================================

	//===========================================
	//位置更新関係
	//===========================================
	m_bUseUpdatePos = true;//位置の更新を使用するかどうか
	//==================================================================================

	//初期化処理
	CObjectX::Init();
	return S_OK;
}
//===============================================================================================================================================================

//===========================================================================================
//終了処理
//===========================================================================================
void CObjectXMove::Uninit()
{
	CObjectX::Uninit();
}
//===============================================================================================================================================================

//===========================================================================================
//更新処理
//===========================================================================================
void CObjectXMove::Update()
{
	//==================================
	//乗算加速度がONになっていたら
	//==================================
	if (m_bUseMultiSpeed == true)
	{
		m_Move.x *= m_MultiSpeed.x;
		m_Move.y *= m_MultiSpeed.y;
		m_Move.z *= m_MultiSpeed.z;
	}
	//==========================================================

	//==================================
	//加速がONになっていたら
	//==================================
	if (m_bUseAddSpeed == true)
	{
		m_Move += m_AddSpeed;
	}
	//==========================================================

	GravityProcess();

	UpdatePos();

	//更新処理
	CObjectX::Update();
}
//===============================================================================================================================================================

//===========================================================================================
//描画処理
//===========================================================================================
void CObjectXMove::Draw()
{
	//描画処理
	CObjectX::Draw();
}
//===============================================================================================================================================================

//===========================================================================================
//重力処理
//===========================================================================================
void CObjectXMove::GravityProcess()
{
	if (m_bUseGravity == true)
	{
		const D3DXVECTOR3& Move = GetMove();
		SetMove(Move + D3DXVECTOR3(0.0f, -m_fGravityPower, 0.0f));
	}
}
//===============================================================================================================================================================

//===========================================================================================
//位置の更新処理
//===========================================================================================
void CObjectXMove::UpdatePos()
{
	if (m_bUseUpdatePos == true)
	{
		const D3DXVECTOR3& Pos = GetPos();

		//1f前の位置を設定
		SetPosOld(Pos);

		//慣性の処理
		if (m_bUseInteria == true)
		{
			m_Move.x += (0.0f - m_Move.x) * m_fInertia;
			m_Move.z += (0.0f - m_Move.z) * m_fInertia;
		}

		//位置の設定
		SetPos(Pos + m_Move + m_AddMove);
	}
}
//===============================================================================================================================================================


//===========================================================================================
//情報をテキストファイルに保存する
//===========================================================================================
void CObjectXMove::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "MOVE = " << (m_Move.x) <<" "<< m_Move.y <<" "<<m_Move.z << endl;//移動量

	CObjectX::SaveInfoTxt(WritingFile);
}
//===============================================================================================================================================================


//===========================================================================================
//ステージマネージャーが選択して操作する
//===========================================================================================
void CObjectXMove::ManagerChooseControlInfo()
{
	CObjectX::ManagerChooseControlInfo();
}
//===============================================================================================================================================================

