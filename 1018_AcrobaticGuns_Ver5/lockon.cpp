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
#include "collision.h"
#include "input.h"
#include "particle.h"
#include "particle2d.h"
#include "calculation.h"
#include "objectX.h"
#include "debugproc.h"
//==============================================================================================================

//===============================================================
//コンストラクタ
//===============================================================
CLockon::CLockon() : m_LockOnPos(NULL_VECTOR3),m_NowRay(NULL_VECTOR3),m_FrontPos(NULL_VECTOR3),m_EndState(ENDSTATE::NONE),m_NearRayColObjPos(NULL_VECTOR3)
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
	Rot.z += 0.02f;
	SetRot(Rot);

	//移動処理
	MoveProcess();

	//レイが狙っている奥の壁の判定位置をサーチする
	BackWallRayCollisionPosSearch();

	//レイを計算する
	CalcRay();

	//一番近いオブジェクトのレイが当たった位置を求める
	RayCollisionToObject();

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
	pLockOn->SetType(CObject::TYPE::LOCKON);


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
void CLockon::BackWallRayCollisionPosSearch()
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

//===============================================================
//レイがどのオブジェクトに当たっているかを計算する
//===============================================================
void CLockon::RayCollisionToObject()
{
	bool bCollision = false;//当たり判定
	vector<D3DXVECTOR3> VecCollisionSuccess;     //当たり判定が成功した位置のvector
	D3DXVECTOR3 NearCollisionPos = NULL_VECTOR3; //当たり判定が成功した位置の中で一番近い位置

	//レイと一致した全てのオブジェクトを求め、中心点をVectorに保存
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = CObject::GetTopObject(nCntPri);//先頭オブジェクトを取得
		D3DXVECTOR3 CollisionStartPos = NULL_VECTOR3;//衝突判定開始位置
		D3DXVECTOR3 CollisionEndPos = NULL_VECTOR3;  //衝突判定終了位置
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();//次のオブジェクトのポインタを取得

			if (pObj->GetType() == CObject::TYPE::ENEMY || pObj->GetType() == CObject::TYPE::BLOCK)
			{
				CObjectX* pObjX = (CObjectX*)pObj;
				//指定したモデルの位置
				//bCollision = CCollision::RayIntersectsAABB(NearPos, Ray, pEnemy->GetVtxMax() + pEnemy->GetPos(),pEnemy->GetVtxMin() + pEnemy->GetPos());
				bCollision = CCollision::RayIntersectsAABBCollisionPos(m_FrontPos, m_NowRay, pObjX->GetVtxMin() + pObjX->GetPos(), pObjX->GetVtxMax() + pObjX->GetPos(),
					CollisionStartPos);
				//bCollision = CCalculation::CalcRaySphere(NearPos, Ray, pEnemy->GetSenterPos(), pEnemy->GetSize().y, CollisionStartPos, CollisionEndPos);

				if (bCollision == true)
				{//レイとサイズ/２分の球の当たり判定成功
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 1, 20, 30.0f, 30.0f, 100, 10, false, CollisionStartPos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
					CManager::GetDebugProc()->PrintDebugProc("衝突した位置：%f %f %f\n", CollisionStartPos.x, CollisionStartPos.y, CollisionStartPos.z);

					//敵の最大頂点のスクリーン座標を求める
					D3DXVECTOR3 ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pObjX->GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
						float(SCREEN_WIDTH), float(SCREEN_HEIGHT));

					float fRot = CCalculation::CalculationRandVecXY();
					CParticle2D::Create(ScreenPos, D3DXVECTOR3(sinf(fRot) * 10.0f, cosf(fRot) * 10.0f, 0.0f), NULL_VECTOR3, CObject2D::POLYGONTYPE01_SENTERROLLING,
						15, 55.0f, 55.0f, CCalculation::CalRaibowColor());

					VecCollisionSuccess.push_back(CollisionStartPos);//当たり判定が成功したオブジェクトの判定開始位置を保存する
					CManager::GetDebugProc()->PrintDebugProc("判定成功したかどうか:%d\n", bCollision);
				}
			}

			pObj = pNext;//オブジェクトを次に進める
		}
	}

	//レイの方向が一致したオブジェクトが存在したら、その中で一番距離が近いオブジェクトの中心点を求め、そこを目掛けた移動量を求める
	if (VecCollisionSuccess.size() != 0)
	{//狙っているオブジェクトの中心点に向かって撃つ
		float fLength = 0.0f;//距離
		float fMinLength = 0.0f;//一番近い距離格納用
		for (auto it = VecCollisionSuccess.begin(); it != VecCollisionSuccess.end(); it++)
		{
			fLength = CCalculation::CalculationLength(m_FrontPos, *it);//レイの判定が成功したオブジェクトの位置とプレイヤーの中心点の距離を測る

			if (it == VecCollisionSuccess.begin())
			{
				fMinLength = fLength;
				NearCollisionPos = *it;
			}
			else
			{
				if (fLength < fMinLength)
				{//一番近い距離より近かったら
					NearCollisionPos = *it;//一番近いオブジェクトを格納
				}
			}
		}

		m_NearRayColObjPos = NearCollisionPos;//レイが当たった一番近いオブジェクトに向かって撃つ
	}
	else
	{//狙っている方向の奥の壁に向かって撃つ
		m_NearRayColObjPos = m_LockOnPos;//レイが当たったオブジェクトがないので、奥の壁に向かって撃つ
	}
	//====================================================================================================================================================================

	//Vectorをクリア
	VecCollisionSuccess.clear();

}
//==============================================================================================================