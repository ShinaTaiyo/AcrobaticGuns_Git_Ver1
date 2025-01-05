//======================================================================================================================
//
//１０月２４日：攻撃用のクラスを作る[attack.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//==================================================================
//インクルード
//==================================================================
#include "attack.h"
#include "damage.h"
#include "manager.h"
#include "enemy.h"
#include "player.h"
#include"gauge.h"
#include "objectXInfo.h"
#include "block.h"
#include "bgModel.h"
#include "game.h"
#include "collision.h"
//======================================================================================================================

//==================================================================
//前方宣言
//==================================================================
const string CAttack::ATTACK_FILENAME[static_cast<int>(CAttack::ATTACKTYPE::MAX)] =
{
	"data\\MODEL\\Attack\\Fire_000.x",
	"data\\MODEL\\Attack\\Explosion_000.x",
	"data\\MODEL\\Attack\\MagicSword_000.x",
};
//======================================================================================================================

//==================================================================
//コンストラクタ
//==================================================================
CAttack::CAttack(int nPower, int nSetHitStopTime, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectXAlive(nPri, bUseintPri, type, ObjType),
m_Type(ATTACKTYPE::BULLET), m_nPower(nPower), m_HitStop({0,nSetHitStopTime}),m_bCollisionRelease(true),m_CollisionType(CAttack::COLLISIONTYPE::NONE),
m_TargetType(CAttack::TARGETTYPE::NONE),m_bHitOtherThanLiving(false),m_bAutoCollision(true),m_bCollisionSuccess(false)
{

}
//======================================================================================================================

//==================================================================
//デストラクタ
//==================================================================
CAttack::~CAttack()
{

}
//======================================================================================================================

//==================================================================
//初期化処理
//==================================================================
HRESULT CAttack::Init()
{
	CObjectXAlive::Init();

	SetAdjustPos(false);
	return S_OK;
}
//======================================================================================================================

//==================================================================
//終了処理
//==================================================================
void CAttack::Uninit()
{
	CObjectXAlive::Uninit();
}
//======================================================================================================================

//==================================================================
//更新処理
//==================================================================
void CAttack::Update()
{
	CObjectXAlive::Update();

	if (m_bAutoCollision == true)
	{
		Collision();
	}
	bool bCollision = false;

	if (m_bHitOtherThanLiving == true)
	{
		for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
		{
			CObject* pObj = GetTopObject(nCntPri);
			while (pObj != nullptr)
			{
				bool bNowCollision = false;
				CObject* pNext = pObj->GetNextObject();

				if (pObj->GetType() == CObject::TYPE::BGMODEL || pObj->GetType() == CObject::TYPE::BLOCK)
				{
					CObjectX* pObjX = static_cast<CObjectX*>(pObj);

					if (CCollision::CollisionSquare(GetPosInfo().GetPos(), GetSizeInfo().GetVtxMax(), GetSizeInfo().GetVtxMin(), pObjX->GetPosInfo().GetPos(), pObjX->GetSizeInfo().GetVtxMax(), pObjX->GetSizeInfo().GetVtxMin()))
					{
						bCollision = true;
					}
				}

				pObj = pNext;
			}
		}
	}
	if (bCollision == true && GetCollisionRelease() == true)
	{
		SetDeath();
	}
}
//======================================================================================================================

//==================================================================
//描画処理
//==================================================================
void CAttack::Draw()
{
	CObjectXAlive::Draw();
}
//======================================================================================================================

//==================================================================
//死亡フラグ設定処理
//==================================================================
void CAttack::SetDeath()
{
	CObject::SetDeath();
}
//======================================================================================================================

//==================================================================
//当たり判定処理
//==================================================================
void CAttack::Collision()
{
	bool bCollision = false;
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = GetTopObject(nCntPri);
		while (pObj != nullptr)
		{
		    bool bNowCollision = false;
			CObject* pNext = pObj->GetNextObject();

			if (pObj->GetType() == CObject::TYPE::ENEMY && m_TargetType == TARGETTYPE::ENEMY)
			{
				CEnemy* pEnemy = static_cast<CEnemy*>(pObj);
				CollisionProcess(bCollision, bNowCollision, pEnemy);
				if (pEnemy->GetLife() < 1)
				{
					pEnemy->SetDefeatAttack(m_Type);
				}
			}
			else if (pObj->GetType() == CObject::TYPE::PLAYER && m_TargetType == TARGETTYPE::PLAYER)
			{
				CObjectXAlive* pObjX = static_cast<CObjectXAlive*>(pObj);
				CollisionProcess(bCollision, bNowCollision, pObjX);
			}

			pObj = pNext;
		}
	}

	if (bCollision == true && GetCollisionRelease() == true)
	{
		m_bCollisionSuccess = true;
		SetDeath();
	}

}
//======================================================================================================================

//==================================================================
//当たり判定共通処理
//==================================================================
void CAttack::CollisionProcess(bool& bCollision, bool& bNowCollision, CObjectXAlive* pObjX)
{
	switch (GetCollisionType())
	{
	case CAttack::COLLISIONTYPE::SQUARE:
		if (CCollision::CollisionSquare(GetPosInfo().GetPos(), GetSizeInfo().GetVtxMax(), GetSizeInfo().GetVtxMin(), pObjX->GetPosInfo().GetPos(), pObjX->GetSizeInfo().GetVtxMax(), pObjX->GetSizeInfo().GetVtxMin()) == true)
		{
			bCollision = true;
			bNowCollision = true;
		}
		break;
	case CAttack::COLLISIONTYPE::RECTANGLE_XZ:
		if (CCollision::RectAngleCollisionXZ(this, pObjX) == true)
		{
			bCollision = true;
			bNowCollision = true;
		}
		break;
	default:
		break;
	}

	if (bNowCollision == true)
	{
		pObjX->SetDamage(GetPower(), m_HitStop.nSetTime);
		pObjX->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), m_HitStop.nSetTime, false, false);
	}

}
//======================================================================================================================

//=============================================================================プレイヤー攻撃クラス=============================================================================

//==================================================================
//コンストラクタ
//==================================================================
CAttackPlayer::CAttackPlayer(int nPower, int nSetHitStopTime) : CAttack(nPower,nSetHitStopTime)
{

}
//======================================================================================================================

//==================================================================
//デストラクタ
//==================================================================
CAttackPlayer::~CAttackPlayer()
{

}
//======================================================================================================================

//==================================================================
//初期化処理
//==================================================================
HRESULT CAttackPlayer::Init()
{
	CAttack::Init();
	return S_OK;
}
//======================================================================================================================

//==================================================================
//終了処理
//==================================================================
void CAttackPlayer::Uninit()
{
	CAttack::Uninit();
}
//======================================================================================================================

//==================================================================
//更新処理
//==================================================================
void CAttackPlayer::Update()
{
	CAttack::Update();

	if (GetCollisionSuccess() == true && GetAttackType() == ATTACKTYPE::BULLET)
	{
		if (CScene::GetMode() == CScene::MODE::MODE_GAME)
		{
			CGauge* pPlayerDiveGauge = CGame::GetPlayer()->GetDiveGauge();
			pPlayerDiveGauge->SetParam(pPlayerDiveGauge->GetParam() + 1);
		}
	}
}
//======================================================================================================================

//==================================================================
//描画処理
//==================================================================
void CAttackPlayer::Draw()
{
	CAttack::Draw();
}
//======================================================================================================================

//==================================================================
//死亡フラグ設定処理
//==================================================================
void CAttackPlayer::SetDeath()
{
	CAttack::SetDeath();
}
//======================================================================================================================

//==================================================================
//生成処理
//==================================================================
CAttackPlayer* CAttackPlayer::Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,
	bool bHitOtherThanLiving, bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
	CAttackPlayer* pAttackPlayer = nullptr;       //生成
	pAttackPlayer = DBG_NEW CAttackPlayer(nPower,nSetHitStopTime);//生成
	pAttackPlayer->Init();                        //初期化処理
	pAttackPlayer->SetTargetType(TargetType);     //ターゲットタイプを設定
	pAttackPlayer->SetCollisionType(CollisionType);//判定タイプを設定
	pAttackPlayer->SetType(CObject::TYPE::ATTACK);//オブジェクトごとのタイプを設定する
	pAttackPlayer->SetAttackType(AttackType);     //攻撃の種類を設定する
	pAttackPlayer->SetLife(nLife);                //体力を設定
	pAttackPlayer->SetMaxLife(nLife);             //最大体力を設定
	pAttackPlayer->GetPosInfo().SetPos(pos);                   //位置  
	pAttackPlayer->GetPosInfo().SetSupportPos(pos);            //支点位置
	pAttackPlayer->GetRotInfo().SetRot(rot);                   //向き
	pAttackPlayer->SetMove(move);                 //移動量
	pAttackPlayer->GetSizeInfo().SetScale(Scale);               //拡大率
	pAttackPlayer->SetAutoSubLife(true);          //体力を使用する
	pAttackPlayer->SetUseInteria(false, CObjectXMove::GetNormalInertia());
	pAttackPlayer->SetUseGravity(false,1.0f);
	pAttackPlayer->SetHitOtherThanLibing(bHitOtherThanLiving);
	pAttackPlayer->SetAutoCollision(bAutoCollision);//攻撃の当たり判定を攻撃クラスに任せるかどうか
	//モデル情報設定
	int nIdx = CManager::GetObjectXInfo()->Regist(ATTACK_FILENAME[static_cast<int>(AttackType)]);

	//モデル情報を割り当てる
	pAttackPlayer->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pAttackPlayer->SetSize();//サイズを設定する
	return pAttackPlayer;
}
//======================================================================================================================

//=============================================================================敵攻撃クラス=============================================================================

//==================================================================
//コンストラクタ
//==================================================================
CAttackEnemy::CAttackEnemy(int nPower, int nSetHitStopTime) : CAttack(nPower,nSetHitStopTime)
{

}
//======================================================================================================================

//==================================================================
//デストラクタ
//==================================================================
CAttackEnemy::~CAttackEnemy()
{

}
//======================================================================================================================

//==================================================================
//初期化処理
//==================================================================
HRESULT CAttackEnemy::Init()
{
	CAttack::Init();
	return S_OK;
}
//======================================================================================================================

//==================================================================
//終了処理
//==================================================================
void CAttackEnemy::Uninit()
{
	CAttack::Uninit();
}
//======================================================================================================================

//==================================================================
//更新処理
//==================================================================
void CAttackEnemy::Update()
{
	CAttack::Update();
}
//======================================================================================================================

//==================================================================
//描画処理
//==================================================================
void CAttackEnemy::Draw()
{
	CAttack::Draw();
}
//======================================================================================================================

//==================================================================
//死亡フラグ設定処理
//==================================================================
void CAttackEnemy::SetDeath()
{
	CAttack::SetDeath();
}
//======================================================================================================================

//==================================================================
//敵の攻撃を生成
//==================================================================
CAttackEnemy* CAttackEnemy::Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,
	bool bHitOtherThanLiving, bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
	CAttackEnemy* pAttackEnemy = DBG_NEW CAttackEnemy(nPower, nSetHitStopTime);

	pAttackEnemy->Init();                        //初期化処理
	pAttackEnemy->SetType(CObject::TYPE::ATTACK); //オブジェクトごとのタイプを設定する
	pAttackEnemy->SetAttackType(AttackType);     //攻撃の種類を設定する
	pAttackEnemy->SetTargetType(TargetType);     //ターゲットタイプを設定する
	pAttackEnemy->SetCollisionType(CollisionType);//判定タイプを設定する
	pAttackEnemy->SetLife(nLife);                //体力を設定
	pAttackEnemy->SetMaxLife(nLife);             //最大体力を設定
	pAttackEnemy->SetAutoSubLife(true);          //体力を自動的に減らす
	pAttackEnemy->GetPosInfo().SetPos(pos);                   //位置  
	pAttackEnemy->GetRotInfo().SetRot(rot);                   //向き
	pAttackEnemy->SetMove(move);                 //移動量
	pAttackEnemy->GetSizeInfo().SetScale(Scale);               //拡大率
	pAttackEnemy->SetUseInteria(false, CObjectXMove::GetNormalInertia());
	pAttackEnemy->SetUseGravity(false, 1.0f);
	pAttackEnemy->SetHitOtherThanLibing(bHitOtherThanLiving);
	pAttackEnemy->SetAutoCollision(bAutoCollision);//攻撃クラスに判定を任せるかどうか
	//モデル情報設定
	int nIdx = CManager::GetObjectXInfo()->Regist(ATTACK_FILENAME[static_cast<int>(AttackType)]);

	//モデル情報を割り当てる
	pAttackEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));
	pAttackEnemy->SetSize();
	return pAttackEnemy;
}
//======================================================================================================================