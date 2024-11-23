//===================================================================================================================================
//
//１１月２１日：ワイヤーを作る[wire_head.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================================================

//=================================================================
//インクルード
//=================================================================
#include "wire_head.h"
#include "renderer.h"
#include "game.h"
#include "objectXInfo.h"
#include "manager.h"
#include "debugproc.h"
#include "collision.h"
//===================================================================================================================================

//=================================================================
//コンストラクタ
//=================================================================
CWireHead::CWireHead(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectXAlive(nPri, bUseintPri, type, ObjType),
m_bCollision(false)
{

}
//===================================================================================================================================

//=================================================================
//デストラクタ
//=================================================================
CWireHead::~CWireHead()
{

}
//===================================================================================================================================

//=================================================================
//初期化処理
//=================================================================
HRESULT CWireHead::Init()
{
	CObjectXAlive::Init();
	return S_OK;
}
//===================================================================================================================================

//=================================================================
//終了処理
//=================================================================
void CWireHead::Uninit()
{
	CObjectXAlive::Uninit();
}
//===================================================================================================================================

//=================================================================
//更新処理
//=================================================================
void CWireHead::Update()
{
	//float fPitch = 0.0f;
	//float fYaw = 0.0f;
	//float fRoll = 0.0f;
	//if (CScene::GetMode() == CScene::MODE_GAME)
	//{
	//	const D3DXVECTOR3& Pos = GetPos();
	//	const D3DXVECTOR3& LockOnPos = CGame::GetPlayer()->GetLockOn()->GetLockOnPos();
	//	//SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, atan2f(PlayerPos.x - Pos.x, PlayerPos.z - Pos.z), 0.0f));
	//	D3DXVECTOR3 Vec = LockOnPos - Pos;
	//	fYaw = atan2f(Vec.x, Vec.z);
 //   	fPitch = -atan2f(Vec.y,sqrtf(powf(Vec.x,2) + powf(Vec.z,2)));//Y方向のベクトルの大きさに対するXZベクトルの距離
	//	fRoll = atan2f(Vec.x, Vec.y);
	//}



	//SetRot(D3DXVECTOR3(D3DX_PI * 0.5f + fPitch, fYaw,0.0f));
	//CManager::GetDebugProc()->PrintDebugProc("ワイヤーヘッドのPitch：%f\n", fPitch);

	CObjectXAlive::Update();

	CManager::GetDebugProc()->PrintDebugProc("ワイヤーヘッドの位置：%f %f %f\n", GetPos().x, GetPos().y, GetPos().z);

	m_bCollision = CollisionSquare();
}
//===================================================================================================================================

//=================================================================
//描画処理
//=================================================================
void CWireHead::Draw()
{
	CObjectXAlive::Draw();
}
//===================================================================================================================================

//=================================================================
//死亡フラグ設定処理
//=================================================================
void CWireHead::SetDeath()
{
	CObjectXAlive::SetDeath();
}
//===================================================================================================================================

//=================================================================
//生成処理
//=================================================================
CWireHead* CWireHead::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Move, D3DXVECTOR3 Scale, int nLife)
{
	CWireHead* pWireHead = DBG_NEW CWireHead();
	pWireHead->Init();
	pWireHead->SetPos(Pos);
	pWireHead->SetSupportPos(Pos);
	pWireHead->SetRot(Rot);
	pWireHead->SetMove(Move);
	pWireHead->SetScale(Scale);
	pWireHead->SetLife(nLife);
	pWireHead->SetMaxLife(nLife);
	pWireHead->SetAutoSubLife(true);
	//モデル情報設定
	int nIdx = CManager::GetObjectXInfo()->Regist("data\\MODEL\\Wire_Head\\Wire_Head_000.x");

	//モデル情報を割り当てる
	pWireHead->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pWireHead->SetSize();

	return pWireHead;
}
//===================================================================================================================================

//=================================================================
//正方形の当たり判定を行う
//=================================================================
bool CWireHead::CollisionSquare()
{
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = GetTopObject(nCntPri);
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();
			CObject::OBJECTTYPE ObjType = pObj->GetObjectType();
			CObject::TYPE type = pObj->GetType();
			if (ObjType == CObject::OBJECTTYPE::OBJECTTYPE_X && type != CObject::TYPE::WIREHEAD && type != CObject::TYPE::PLAYER)
			{
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);
				if (CCollision::CollisionSquare(GetPos(), GetVtxMax(), GetVtxMin(), pObjX->GetPos(), pObjX->GetVtxMax(), pObjX->GetVtxMin()) == true)
				{
					return true;
				}
			}
			pObj = pNext;
		}
	}

	CManager::GetDebugProc()->PrintDebugProc("正方形の当たり判定失敗！\n");

	return false;
}
//===================================================================================================================================