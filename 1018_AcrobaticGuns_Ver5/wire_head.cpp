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
	CObjectXAlive::Update();

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