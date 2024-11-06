//==============================================================================================================
//
//１０月２８日：ロックオンした場所に弾を撃ちたい[lockon.cpp]
//Author:ShinaTaiyo
//
//==============================================================================================================

//===============================================================
//インクルード
//===============================================================
#include "lockon.h"
#include "manager.h"
#include "camera.h"
#include "texture.h"
#include "input.h"
#include "calculation.h"
#include "debugproc.h"
//==============================================================================================================

//===============================================================
//コンストラクタ
//===============================================================
CLockon::CLockon() : m_LockOnPos(NULL_VECTOR3),m_NowRay(NULL_VECTOR3),m_FrontPos(NULL_VECTOR3),m_EndState(ENDSTATE::NONE)
{

}
//==============================================================================================================

//===============================================================
//デストラクタ
//===============================================================
CLockon::~CLockon()
{

}
//==============================================================================================================

//===============================================================
//初期化処理
//===============================================================
HRESULT CLockon::Init()
{
	CObject2D::Init();
	return S_OK;
}
//==============================================================================================================

//===============================================================
//終了処理
//===============================================================
void CLockon::Uninit()
{
	CObject2D::Uninit();
}
//==============================================================================================================

//===============================================================
//更新処理
//===============================================================
void CLockon::Update()
{
	D3DXVECTOR3 Rot = GetRot();
	Rot.z += 0.01f;
	SetRot(Rot);

	//移動処理
	MoveProcess();

	//レイの処理
	SearchProcess();

	//レイを計算する
	CalcRay();

	//オブジェクト2D更新処理
	CObject2D::Update();
}
//==============================================================================================================

//===============================================================
//描画処理
//===============================================================
void CLockon::Draw()
{
	CObject2D::Draw();
}
//==============================================================================================================

//===============================================================
//死亡フラグ設定処理
//===============================================================
void CLockon::SetDeath()
{
	CObject::SetDeath();
}
//==============================================================================================================

//===============================================================
//生成処理
//===============================================================
CLockon* CLockon::Create(D3DXVECTOR3 Pos, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight, D3DXCOLOR col)
{
	CTexture* pTexture = CManager::GetTexture();
	CLockon* pLockOn = DBG_NEW CLockon;

	pLockOn->Init();//初期化処理
	pLockOn->SetPos(Pos);//位置設定
	pLockOn->SetSupportPos(Pos);//支点位置設定
	pLockOn->SetInfo(1, 1, fWidth, fHeight, col,PolygonType,false);//情報設定（必ず）

	//テクスチャ設定
	pLockOn->SetTextureIndex(pTexture->Regist("data\\TEXTURE\\LockOn\\Target_000.png"));
	pLockOn->BindTexture(pTexture->GetAddress(pLockOn->GetTextureIndex()));

	//体力を使用しない
	pLockOn->SetUseLife(false, 1, 1);

	//オブジェクトタイプ設定
	pLockOn->SetType(CObject::TYPE_LOCKON);


	return pLockOn;
}
//==============================================================================================================

//===============================================================
//移動処理
//===============================================================
void CLockon::MoveProcess()
{
	D3DXVECTOR3 Pos = GetPos();
	const D3DXVECTOR3& CameraRot = CManager::GetCamera()->GetRot();

	m_EndState = ENDSTATE::NONE;

	if (CManager::GetInputJoypad()->GetRStickPress(16) == true)
	{
		Pos.x += sinf(CManager::GetInputJoypad()->GetRStickAimRot()) * m_fNORMAL_LOCKONMOVE;
		Pos.y += cosf(CManager::GetInputJoypad()->GetRStickAimRot()) * -m_fNORMAL_LOCKONMOVE;
	}

	if (Pos.x + GetWidth() * 0.5f > SCREEN_WIDTH)
	{//右
		Pos.x = SCREEN_WIDTH - GetWidth() * 0.5f;
		m_EndState = ENDSTATE::RIGHTEND;
		CManager::GetCamera()->SetRot(CameraRot + D3DXVECTOR3(0.0f, 0.01f, 0.0f));
	}
	if (Pos.x - GetWidth() * 0.5f < 0.0f)
	{//左
		Pos.x = 0.0f + GetWidth() * 0.5f;
		m_EndState = ENDSTATE::LEFTEND;
		CManager::GetCamera()->SetRot(CameraRot + D3DXVECTOR3(0.0f, -0.01f, 0.0f));

	}

	if (Pos.y + GetHeight() * 0.5f > SCREEN_HEIGHT)
	{//上
		Pos.y = SCREEN_HEIGHT - GetHeight() * 0.5f;
		m_EndState = ENDSTATE::UPEND;
	}
	if (Pos.y - GetHeight() * 0.5f < 0.0f)
	{//下
		Pos.y = 0.0f + GetHeight() * 0.5f;
		m_EndState = ENDSTATE::DOWNEND;
	}



	SetPos(Pos);
}
//==============================================================================================================

//===============================================================
//カーソルの先にあるオブジェクトをサーチ
//===============================================================
void CLockon::SearchProcess()
{
	D3DXVECTOR3 Pos = GetPos();//位置

	CCalculation::CalcScreenToXZ(&m_LockOnPos, int(Pos.x), int(Pos.y), SCREEN_WIDTH, SCREEN_HEIGHT,
		CManager::GetCamera()->GetMtxView(), CManager::GetCamera()->GetMtxProjection());

	CManager::GetDebugProc()->PrintDebugProc("床または壁との交点：%f %f %f\n", m_LockOnPos.x, m_LockOnPos.y, m_LockOnPos.z);

}
//==============================================================================================================

//===============================================================
//レイを計算する
//===============================================================
void CLockon::CalcRay()
{
	D3DXVECTOR3 FarPos = NULL_VECTOR3; //奥
	D3DXVECTOR3 Ray = NULL_VECTOR3;    //レイ
	//============================================
	//カメラ手前と奥のワールド座標を求める
	//============================================
	CCalculation::CalcScreenToWorld(&m_FrontPos, int(GetPos().x), int(GetPos().y), 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
		CManager::GetCamera()->GetMtxView(), CManager::GetCamera()->GetMtxProjection());//手前

	CCalculation::CalcScreenToWorld(&FarPos, int(GetPos().x), int(GetPos().y), 1.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
		CManager::GetCamera()->GetMtxView(), CManager::GetCamera()->GetMtxProjection());//奥
	//============================================================================================================================

	m_NowRay = FarPos - m_FrontPos;//ベクトルを求める
	D3DXVec3Normalize(&m_NowRay, &m_NowRay);//正規化

	CManager::GetDebugProc()->PrintDebugProc("レイの向き：%f %f %f\n", m_NowRay.x, m_NowRay.y, m_NowRay.z);
}
//==============================================================================================================