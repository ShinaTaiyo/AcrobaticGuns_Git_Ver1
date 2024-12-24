//============================================================================================================================================
//
//１１月２５日：敵を作る[enemy.cpp]
//Author:ShinaTaiyo
//
//============================================================================================================================================

//====================================================================================
//インクルード
//====================================================================================
#include "enemy.h"
#include "manager.h"
#include "objectXInfo.h"
#include "game.h"
#include "calculation.h"
#include "debugproc.h"
#include "collision.h"
#include "damage.h"
#include "particle.h"
#include "phasemanager.h"
#include "input.h"
#include "camera.h"
#include "attack.h"
//============================================================================================================================================

//====================================================================================
//静的メンバ宣言
//====================================================================================
//const string CEnemy::m_aENEMY_FILENAME[static_cast<int>(ENEMYTYPE::MAX)] =
//{
//	"data\\MODEL\\Enemy\\Normal\\noobSlime.x"
//};
int CEnemy::m_nNumEnemy = 0;

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemy::CEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectXAlive(nPri, bUseintPri, type, ObjType),
m_Type(ENEMYTYPE::SHOTWEAK), m_VecMoveAi(), m_MoveAiSavePos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),m_nCntTime(0),m_nIdxMoveAi(0), m_nPhaseNum(0),m_pEnemyMove(DBG_NEW CEnemyMove_AI()),
m_fRotMove(0.0f),m_fSensingRange(0.0f),m_fNormalSpeed(0.0f),m_Pattern(),m_bCollisoinDetection(true),m_bActivateCollisionDetection(false),m_bCollisionWall(false),
m_DefeatAttackType(CAttack::ATTACKTYPE::EXPLOSION)
{
	m_nNumEnemy++;//敵総数カウントアップ
}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemy::~CEnemy()
{
	m_nNumEnemy--;//敵総数カウントダウン
}
//============================================================================================================================================

//====================================================================================
//初期化処理
//====================================================================================
HRESULT CEnemy::Init()
{
	CObjectXAlive::Init();

	SetUseGravity(true, CObjectXMove::GetNormalGravity());
	SetUseUpdatePos(true);
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//終了処理
//====================================================================================
void CEnemy::Uninit()
{
	CObjectXAlive::Uninit();

	if (m_pEnemyMove != nullptr)
	{
		delete m_pEnemyMove;
		m_pEnemyMove = nullptr;
	}
}
//============================================================================================================================================

//====================================================================================
//更新処理
//====================================================================================
void CEnemy::Update()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		const D3DXVECTOR3& Rot = GetRot();
		const D3DXVECTOR3& Pos = GetPos();
		const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
		//float fElevaRot = atan2f(PlayerPos.y - Pos.y, sqrtf(powf(PlayerPos.x - Pos.x, 2) + powf(PlayerPos.z - Pos.z, 2)));
		//SetRot(D3DXVECTOR3(fElevaRot, atan2f(PlayerPos.x - Pos.x, PlayerPos.z - Pos.z) + D3DX_PI,0.0f));

		float fLength = CCalculation::CalculationLength(Pos, PlayerPos);
		m_pEnemyMove->Process(this);

		m_nCntTime++;//時間をカウントする

		AttackProcess();//攻撃処理

		CollisionDetectionProcess();

		CObjectXAlive::Update();

	    CollisionProcess();//当たり判定処理

		if (GetPos().y < -100.0f)
		{
			SetDeath();
		}

	}

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		for (auto it : m_VecMoveAi)
		{
			if (it->GetUseDraw())
			{
				it->SetUseDraw(false);
			}
		}
	}

}
//============================================================================================================================================

//====================================================================================
//描画処理
//====================================================================================
void CEnemy::Draw()
{
	CObjectXAlive::Draw();

	for (auto pObj : m_VecMoveAi)
	{
		pObj->ExtraDraw();
	}
}
//============================================================================================================================================

//====================================================================================
//死亡フラグ設定処理
//====================================================================================
void CEnemy::SetDeath()
{
	CObjectXAlive::SetDeath();

	if (GetUseDeath() == true)
	{
		for (auto pObj : m_VecMoveAi)
		{
			pObj->SetUseDeath(true);
			pObj->SetDeath();
		}
		m_VecMoveAi.clear();
	}
}
//============================================================================================================================================

//====================================================================================
//生成処理
//====================================================================================
//CEnemy* CEnemy::Create(ENEMYTYPE Type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
//{
//	CEnemy* pEnemy = DBG_NEW CEnemy;
//
//	pEnemy->Init();            //初期化処理
//	int nIdx = CManager::GetObjectXInfo()->Regist(m_aENEMY_FILENAME[int(Type)]);
//	pEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
//		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
//		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
//		CManager::GetObjectXInfo()->GetTexture(nIdx),
//		CManager::GetObjectXInfo()->GetColorValue(nIdx));
//
//	pEnemy->SetType(CObject::TYPE::ENEMY);//オブジェクトタイプを設定
//	pEnemy->SetEnemyType(Type);//敵の種類を設定
//	pEnemy->SetLife(nLife);    //体力
//	pEnemy->SetMaxLife(nLife); //最大体力
//	pEnemy->SetPos(pos);       //位置
//	pEnemy->SetSupportPos(pos);//支点位置
//	pEnemy->SetRot(rot);       //向き
//	pEnemy->SetScale(Scale);   //拡大率
//	pEnemy->SetFormarScale(Scale);//元の拡大率を設定
//	pEnemy->SetSize();//モデルサイズを設定
//	pEnemy->SetAutoSubLife(false);//自動的に体力を減らすかどうか
//
//	return pEnemy;
//}
////============================================================================================================================================

//====================================================================================
//テキストファイルに情報をセーブする
//====================================================================================
void CEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "ENEMYTYPE = " << static_cast<int>(m_Type);
	switch (m_Type)
	{
	case ENEMYTYPE::SHOTWEAK:
		WritingFile << " # SHOTWEAK" << endl;
		break;
	case ENEMYTYPE::DIVEWEAK:
		WritingFile << " # DIVEWEAK" << endl;
		break;
	default:
		assert(false);
		break;
	}

	WritingFile << "NORMALSPEED = " << m_fNormalSpeed << endl;
	WritingFile << "SENSINGRANGE = " << m_fSensingRange << endl;

	//フェーズ番号を設定
	WritingFile << "PHASENUM = " << m_nPhaseNum << endl;

	//移動AIの位置を保存
	WritingFile << "SETMOVEAI" << endl;

	int nCnt = 0;
	for (auto it = m_VecMoveAi.begin(); it != m_VecMoveAi.end(); ++it)
	{
		WritingFile << "SETNUM = " << nCnt << endl;

		WritingFile << "POS = " << fixed << setprecision(3) << (*it)->GetPos().x << " " <<
			fixed << setprecision(3) << (*it)->GetPos().y << " " <<
			fixed << setprecision(3) << (*it)->GetPos().z << " " << endl;//位置
		WritingFile << "ROT = " << fixed << setprecision(3) << (*it)->GetRot().x << " " <<
			fixed << setprecision(3) << (*it)->GetRot().y << " " <<
			fixed << setprecision(3) << (*it)->GetRot().z << " " << endl;//向き
		WritingFile << "SCALE = " << fixed << setprecision(3) << (*it)->GetScale().x << " " <<
			fixed << setprecision(3) << (*it)->GetScale().y << " " <<
			fixed << setprecision(3) << (*it)->GetScale().z << " " << endl;//拡大率

		WritingFile << "END_SETNUM" << endl;

		nCnt++;//カウントアップ
	}

	WritingFile << "END_SETMOVEAI" << endl;

	CObjectXAlive::SaveInfoTxt(WritingFile);
}
//============================================================================================================================================

//====================================================================================
//テキストファイルから情報をロードする
//====================================================================================
void CEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	//int nType = 0;//種類
	//int nLife = 0;//体力
	//D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //移動量
	//D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //位置
	//D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //拡大率
	//D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //向き
	//ENEMYTYPE Type = {};                               //ブロックの種類
	//while (Buff != "END_SETBLOCK")
	//{
	//	LoadingFile >> Buff;//単語を読み込む
	//	if (Buff == "#")
	//	{
	//		getline(LoadingFile, Buff);
	//	}
	//	else if (Buff == "TYPE")
	//	{
	//		LoadingFile >> Buff;//イコール
	//		LoadingFile >> nType;      //種類
	//	}
	//	else if (Buff == "LIFE")
	//	{
	//		LoadingFile >> Buff;//イコール
	//		LoadingFile >> nLife;      //体力
	//	}
	//	else if (Buff == "MOVE")
	//	{
	//		LoadingFile >> Buff;//イコール
	//		LoadingFile >> Move.x;      //移動量X
	//		LoadingFile >> Move.y;      //移動量Y
	//		LoadingFile >> Move.z;      //移動量Z
	//	}
	//	else if (Buff == "POS")
	//	{
	//		LoadingFile >> Buff;//イコール
	//		LoadingFile >> Pos.x;      //位置X
	//		LoadingFile >> Pos.y;      //位置Y
	//		LoadingFile >> Pos.z;      //位置Z
	//	}
	//	else if (Buff == "ROT")
	//	{
	//		LoadingFile >> Buff;//イコール
	//		LoadingFile >> Rot.x;      //位置X
	//		LoadingFile >> Rot.y;      //位置Y
	//		LoadingFile >> Rot.z;      //位置Z
	//	}
	//	else if (Buff == "SCALE")
	//	{
	//		LoadingFile >> Buff;//イコール
	//		LoadingFile >> Scale.x;      //拡大率X
	//		LoadingFile >> Scale.y;      //拡大率Y
	//		LoadingFile >> Scale.z;      //拡大率Z
	//	}
	//}
	//Type = static_cast<ENEMYTYPE>(nType);

	//

	//listSaveManager.push_back(CEnemy::Create(Type, nLife, Pos, Rot, Scale));//vectorに情報を保存する

}
//============================================================================================================================================

//====================================================================================
//テキストから情報をロードする
//====================================================================================
//CObject* CEnemy::ManagerChengeObject(bool bAim)
//{
//	int nNewType = static_cast<int>(m_Type);
//    ENEMYTYPE NewType = {};
//	//=======================================
//	//種類を変える
//	//=======================================
//	if (bAim == true)
//	{
//		nNewType++;
//	}
//	else
//	{
//		nNewType--;
//	}
//	if (nNewType >= static_cast<int>(ENEMYTYPE::MAX))
//	{
//		nNewType = 0;
//	}
//	if (nNewType < 0)
//	{
//		nNewType = static_cast<int>(ENEMYTYPE::MAX) - 1;
//	}
//	//======================================================================================
//
//	//=======================================
//	//設定する
//	//=======================================
//	NewType = ENEMYTYPE(nNewType);
//	//======================================================================================
//
//	//=======================================
//	//死亡フラグを設定する
//	//=======================================
//	SetUseDeath(true);
//	SetDeath();
//	//======================================================================================
//
//	return CEnemy::Create(NewType,GetLife,GetPos(), GetRot(), GetScale());//生成したオブジェクトを返す
//}
////============================================================================================================================================

//====================================================================================
//ステージマネージャーにセーブする
//====================================================================================
CObject* CEnemy::ManagerSaveObject()
{
	return nullptr;
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーから操作する
//====================================================================================
void CEnemy::ManagerChooseControlInfo()
{
	//ステージマネージャーに選択されている時だけ表示する
	for (auto it : m_VecMoveAi)
	{
		it->SetUseDraw(true);
	}

	SetMoveAiPoint();//移動AIの設定を行う

	PhaseNumDecision();//フェーズ番号の決定を行う

	EditNormalSpeed();//通常移動速度を編集

	EditSensingRange();//索敵範囲を編集
	CObjectXAlive::ManagerChooseControlInfo();
}
//============================================================================================================================================

//====================================================================================
//移動AIの情報を設定する
//====================================================================================
void CEnemy::SetVecMoveAiInfo(vector<CAIModel*>& vec)
{
	m_VecMoveAi = vec;
}
//============================================================================================================================================

//====================================================================================
//当たり判定の処理を行う
//====================================================================================
void CEnemy::CollisionProcess()
{
	D3DXVECTOR3 MyPos = GetPos();
	D3DXVECTOR3 MyPosOld = GetPosOld();
	D3DXVECTOR3 MyVtxMax = GetVtxMax();
	D3DXVECTOR3 MyVtxMin = GetVtxMin();
	const D3DXVECTOR3 Move = GetMove();
	bool bCollisionXOld = GetExtrusionCollisionSquareX();
	bool bCollisionYOld = GetExtrusionCollisionSquareY();
	bool bCollisionZOld = GetExtrusionCollisionSquareZ();

	bool bCollisionX = false;
	bool bCollisionY = false;
	bool bCollisionZ = false;
	bool bIsLanding = false;

	bool bSuccessCollision = false;//当たり判定が成功したかどうか
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = CObject::GetTopObject(nCntPri);

		while (pObj != nullptr)
		{
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextObject();

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
			{
				D3DXVECTOR3 ComPos = static_cast<CObjectX*>(pObj)->GetPos();
				D3DXVECTOR3 ComVtxMax = static_cast<CObjectX*>(pObj)->GetVtxMax();
				D3DXVECTOR3 ComVtxMin = static_cast<CObjectX*>(pObj)->GetVtxMin();

				bSuccessCollision = CCollision::ExtrusionCollisionSquare(MyPos, bCollisionX, bCollisionY, bCollisionZ, Move, MyPosOld, MyVtxMax, MyVtxMin,
					ComPos, ComVtxMax, ComVtxMin, bCollisionXOld, bCollisionYOld, bCollisionZOld,bIsLanding);

				if (bSuccessCollision == true)
				{
					SetPos(MyPos);
				}

				if (bCollisionY == true)
				{
					if (bIsLanding == true)
					{
						SetMove(D3DXVECTOR3(GetMove().x,0.1f, GetMove().z));
						SetIsLanding(true);
					}
				}

			}

			//オブジェクトを次に進める
			pObj = pNext;
		}

	}


	if (bCollisionX == true || bCollisionZ == true)
	{
		m_bCollisionWall = true;
	}
	else
	{
		m_bCollisionWall = false;
	}

	SetExtrusionCollisionSquareX(bCollisionX);
	SetExtrusionCollisionSquareY(bCollisionY);
	SetExtrusionCollisionSquareZ(bCollisionZ);

}
//============================================================================================================================================

//====================================================================================
//移動AIの配置をする
//====================================================================================
void CEnemy::SetMoveAiPoint()
{
	float fX = 0;
	float fZ = 0;
	bool bMove = false;
	bool bMoveY = false;
	float fAddPosY = 0.0f;
	auto Input = CManager::GetInputKeyboard();
	float fCameraRot = CManager::GetCamera()->GetRot().y;
	if (Input->GetPress(DIK_LSHIFT))
	{
		if (Input->GetPress(DIK_I))
		{
			fAddPosY = 5.0f;
			bMoveY = true;
		}
		else if (Input->GetPress(DIK_K))
		{
			fAddPosY = -5.0f;
			bMoveY = true;
		}
	}
	else
	{
		if (Input->GetPress(DIK_I))
		{
			fZ = 1;
			bMove = true;
		}
		else if (Input->GetPress(DIK_K))
		{
			fZ = -1;
			bMove = true;
		}
	}
	if (Input->GetPress(DIK_L))
	{
		fX = 1;
		bMove = true;
	}
	else if (Input->GetPress(DIK_J))
	{
		fX = -1;
		bMove = true;
	}

	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CManager::GetDebugProc()->PrintDebugProc("移動AIの設置位置の移動：IKLJ\n");

	if (bMove == true)
	{
		float fRot = atan2f(fX, fZ);
		m_MoveAiSavePos += D3DXVECTOR3(sinf(fCameraRot + fRot) * 5.0f, 0.0f, cosf(fCameraRot + fRot) * 5.0f);
	}

	if (bMoveY == true)
	{
		m_MoveAiSavePos += D3DXVECTOR3(0.0f, fAddPosY, 0.0f);
	}

	D3DXVECTOR3 NowPos = m_MoveAiSavePos + GetPos();

	CManager::GetDebugProc()->PrintDebugProc("移動AIの位置：%f %f %f\n", NowPos.x,NowPos.y,NowPos.z);

	CManager::GetDebugProc()->PrintDebugProc("移動AIを保存：O\n");

	if (Input->GetPress(DIK_LSHIFT))
	{
		if (m_VecMoveAi.size() > 0)
		{
			if (Input->GetTrigger(DIK_O))
			{
				auto it = m_VecMoveAi.end() - 1;
				(*it)->SetUseDeath(true);
				(*it)->SetDeath();
				m_VecMoveAi.pop_back();
			}
		}
	}
	else
	{
		if (Input->GetTrigger(DIK_O))
		{
			m_VecMoveAi.push_back(CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, m_MoveAiSavePos + GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3.0f, 3.0f, 3.0f), nullptr));
		}
	}
}
//============================================================================================================================================

//====================================================================================
//AI移動処理
//====================================================================================
void CEnemy::AIMoveProcess()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		float fLengthPlayer = CCalculation::CalculationLength(CGame::GetPlayer()->GetPos(), GetPos());
		if (fLengthPlayer < m_fSensingRange)
		{
			ChengeMove(DBG_NEW CEnemyMove_Battle());
		}

		if (m_VecMoveAi.size() > 0)
		{
			auto it = m_VecMoveAi.begin();//最初のポインタを取得

			advance(it, m_nIdxMoveAi);//指定している番号まで進める

			float fLength = CCalculation::CalculationLength(GetPos(), (*it)->GetPos());//距離を測る
			float fRot = atan2f((*it)->GetPos().x - GetPos().x, (*it)->GetPos().z - GetPos().z);
			SetMove(D3DXVECTOR3(sinf(fRot) * m_fNormalSpeed, GetMove().y, cosf(fRot) * m_fNormalSpeed));

			if (fLength < 30.0f)
			{
				m_nIdxMoveAi++;//目的地を次の位置に変更
			}

			int nSize = m_VecMoveAi.size();
			if (m_nIdxMoveAi >= nSize)
			{
				m_nIdxMoveAi = 0;
			}
			if (m_nIdxMoveAi < 0)
			{
				m_nIdxMoveAi = nSize - 1;
			}
		}
	}
}
//============================================================================================================================================


//====================================================================================
//バトル移動処理
//====================================================================================
void CEnemy::BattleMoveProcess()
{
	float fLengthPlayer = CCalculation::CalculationLength(CGame::GetPlayer()->GetPos(), GetPos());
	const D3DXVECTOR3 & PlayerPos = CGame::GetPlayer()->GetPos();
	const D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3 Aim = PlayerPos - Pos;
	D3DXVec3Normalize(&Aim, &Aim);
	float fRot = atan2f(Aim.x,Aim.z);

	D3DXVECTOR3 Move = CCalculation::HormingVecRotXZ(m_fRotMove, GetPos(), CGame::GetPlayer()->GetSenterPos(), 0.1f, m_fNormalSpeed);
	SetRot(D3DXVECTOR3(GetRot().x, m_fRotMove + D3DX_PI, GetRot().z));
	SetMove(D3DXVECTOR3(Move.x, GetMove().y,Move.z));
	CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 1, 30, 20.0f, 20.0f, 100, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
	//if (fLengthPlayer > m_fSensingRange)
	//{
	//	ChengeMove(DBG_NEW CEnemyMove_AI());
	//}	

	RayCollision();

}
//============================================================================================================================================


//====================================================================================
//移動状態を変更する
//====================================================================================
void CEnemy::ChengeMove(CEnemyMove* pEnemyMove)
{
	//破棄する
	if (m_pEnemyMove != nullptr)
	{
		delete m_pEnemyMove;
		m_pEnemyMove = nullptr;
	}

	//代入する
	m_pEnemyMove = pEnemyMove;
}
//============================================================================================================================================

//====================================================================================
//レイが当たったかどうかを判定する
//====================================================================================
void CEnemy::RayCollision()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CObjectX* pObjX = nullptr;
		D3DXVECTOR3 Ray = CCalculation::RadToVec(D3DXVECTOR3(GetRot().x,GetRot().y,0.0f) - D3DXVECTOR3(D3DX_PI * 0.5f,0.0f,0.0f));//手前側にレイを飛ばす
		D3DXVec3Normalize(&Ray, &Ray);

		for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
		{
			CObject* pObj = GetTopObject(nCntPri);

			while (pObj != nullptr)
			{
				CObject* pNext = pObj->GetNextObject();
				D3DXVECTOR3 CollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				if (pObj->GetType() == CObject::TYPE::BGMODEL || pObj->GetType() == CObject::TYPE::BLOCK)
				{
					CObjectX * pComObjX = static_cast<CObjectX*>(pObj);
					if (CCollision::RayIntersectsAABBCollisionPos(GetSenterPos(), Ray, pComObjX->GetPos() + pComObjX->GetVtxMin(), pComObjX->GetPos() + pComObjX->GetVtxMax(),
						CollisionPos))
					{
						float fLength = sqrtf(powf(CollisionPos.x - GetSenterPos().x, 2) + 
							powf(CollisionPos.y - GetSenterPos().y, 2) + powf(CollisionPos.z - GetSenterPos().z,2));
						if (fLength < GetSize().x + 50.0f)
						{
							//敵のオブジェクトのサイズの少し上より距離が小さくなった
							pObjX = pComObjX;
						}
					}
				}

				pObj = pNext;
			}
		}

		if (pObjX != nullptr)
		{
			D3DXVECTOR3 ObjectAim = pObjX->GetPos() - GetPos();
			D3DXVec3Normalize(&ObjectAim, &ObjectAim);

			float fDot = CCalculation::DetermineSide3D(GetPos(), ObjectAim, D3DXVECTOR3(0.0f, 1.0f, 0.0f), CGame::GetPlayer()->GetPos());
			const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
			const D3DXVECTOR3& Pos = GetPos();
			D3DXVECTOR3 Aim = PlayerPos - Pos;
			D3DXVec3Normalize(&Aim, &Aim);
			float fRot = atan2f(Aim.x, Aim.z);

			if (fDot > 0.0f)
			{//ベクトルに対してプレイヤーが右側にいたら
				//SetMove(D3DXVECTOR3(sinf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed, GetMove().y, cosf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed));
				ChengeMove(DBG_NEW CEnemyMove_DodgeWall(this, D3DXVECTOR3(sinf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed, GetMove().y, cosf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed)));
			}
			else if (fDot < 0.0f)
			{//ベクトルに対してプレイヤーが左側にいたら
				ChengeMove(DBG_NEW CEnemyMove_DodgeWall(this, D3DXVECTOR3(sinf(fRot + D3DX_PI * 0.5f) * m_fNormalSpeed, GetMove().y, cosf(fRot + D3DX_PI * 0.5f) * m_fNormalSpeed)));
			}
		}
	}
}
//============================================================================================================================================


//====================================================================================
//フェーズ番号を決定する
//====================================================================================
void CEnemy::PhaseNumDecision()
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == false)
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4))
		{
			m_nPhaseNum++;
		}
	}
	else
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4))
		{
			m_nPhaseNum--;
		}
	}
	CManager::GetDebugProc()->PrintDebugProc("フェーズ番号の変更：%d\n", m_nPhaseNum);
}
//============================================================================================================================================


//====================================================================================
//攻撃処理
//====================================================================================
void CEnemy::AttackProcess()
{

}
//============================================================================================================================================

//====================================================================================
//衝突判定処理
//====================================================================================
void CEnemy::CollisionDetectionProcess()
{
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = GetTopObject(nCntPri);
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();
			if (pObj->GetType() == CObject::TYPE::ENEMY || pObj->GetType() == CObject::TYPE::PLAYER)
			{
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);
				if (pObjX != this)
				{
					D3DXVECTOR3 AimVec = D3DXVECTOR3(pObjX->GetPos().x, 0.0f, pObjX->GetPos().z) -
						D3DXVECTOR3(GetPos().x, 0.0f, GetPos().z);//XZ方向だけのベクトルを取る
					float fLength = CCalculation::CalculationLength(GetPos(), pObjX->GetPos());
					const D3DXVECTOR3& ComPos = pObjX->GetPos();
					const D3DXVECTOR3& ComVtxMax = ComPos + D3DXVECTOR3(pObjX->GetVtxMax().x, 0.0f, 0.0f);
					D3DXVec3Normalize(&AimVec, &AimVec);

					float fComCornarDistance = CCalculation::CalculationLength(ComPos,ComVtxMax);
					float fMyCornarDistance = CCalculation::CalculationLength(GetPos(), GetPos() + D3DXVECTOR3(GetVtxMax().x, 0.0f, 0.0f));
					float fTotalLength = (fComCornarDistance + fMyCornarDistance);
					if (fLength < fTotalLength &&
						GetPos().y + GetVtxMax().y >= pObjX->GetPos().y + pObjX->GetVtxMin().y &&
						GetPos().y + GetVtxMin().y <= pObjX->GetPos().y + pObjX->GetVtxMax().y)
					{
						SetMove(-AimVec * (fTotalLength - fLength) + GetMove());//攻撃時の動きよりも優先的にこの移動量を割り当てる
						break;
					}
				}
			}

			pObj = pNext;
		}
	}
}

//====================================================================================
//通常移動速度を編集
//====================================================================================
void CEnemy::EditNormalSpeed()
{
	CInputKeyboard * pInput = CManager::GetInputKeyboard();

	if (pInput->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (pInput->GetPress(DIK_5) == true)
			{
				m_fNormalSpeed -= 0.1f;
			}
		}
		else if (pInput->GetPress(DIK_5) == true)
		{
			m_fNormalSpeed += 0.1f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (pInput->GetTrigger(DIK_5) == true)
			{
				m_fNormalSpeed -= 0.1f;
			}
		}
		else if (pInput->GetTrigger(DIK_5) == true)
		{
			m_fNormalSpeed += 0.1f;
		}
	}

	CManager::GetDebugProc()->PrintDebugProc("通常移動速度変更（５）：%f\n",m_fNormalSpeed);
}
//============================================================================================================================================

//====================================================================================
//通常移動速度を編集
//====================================================================================
void CEnemy::EditSensingRange()
{
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	if (pInput->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (pInput->GetPress(DIK_6) == true)
			{
				m_fSensingRange -= 10.0f;
			}
		}
		else if (pInput->GetPress(DIK_6) == true)
		{
			m_fSensingRange += 10.0f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (pInput->GetTrigger(DIK_6) == true)
			{
				m_fSensingRange -= 10.0f;
			}
		}
		else if (pInput->GetTrigger(DIK_6) == true)
		{
			m_fSensingRange += 10.0f;
		}
	}

	CManager::GetDebugProc()->PrintDebugProc("索敵範囲変更（６）：%f\n", m_fSensingRange);
}
//============================================================================================================================================

//********************************************************************************************************************************************
//射撃に弱い敵クラス
//********************************************************************************************************************************************

//====================================================================================
//静的メンバ変数宣言
//====================================================================================
const string CShotWeakEnemy::s_aSHOTWEAKENEMY_FILENAME[static_cast<int>(SHOTWEAKENEMYTYPE::MAX)] =
{
	"data\\MODEL\\Enemy\\ShotWeak\\noobSlime.x"
};
const float CShotWeakEnemy::s_fATTACKSTART_LENGTH = 450.0f;//攻撃開始距離
const float CShotWeakEnemy::s_fNORMAL_SENSIINGRANGE = 1050.0f;//通常感知射程
const int CShotWeakEnemy::s_nATTACK_COOLTIME = 60;//攻撃のクールタイム
//============================================================================================================================================

//====================================================================================
//コンストラクタ
//====================================================================================
CShotWeakEnemy::CShotWeakEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CEnemy(nPri,bUseintPri,type,ObjType),
m_ShotWeakEnemyType(SHOTWEAKENEMYTYPE::NORMAL),m_pMagicSword(nullptr), m_SaveAimPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_nAttackCoolTime(0)
{

}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CShotWeakEnemy::~CShotWeakEnemy()
{

}
//============================================================================================================================================

//====================================================================================
//初期化処理
//====================================================================================
HRESULT CShotWeakEnemy::Init()
{
	CEnemy::Init();

	float fRatioRot = static_cast<float>(rand() % 200 - 100) / 100;
	float fRotSpeed = static_cast<float>(rand() % 40 - 20) / 100;
	m_pMagicSword = CAttackEnemy::Create(CAttack::ATTACKTYPE::MAGICSWORD, CAttack::TARGETTYPE::PLAYER, CAttack::COLLISIONTYPE::RECTANGLE_XZ,
		false,false,1, 60, 200, GetPos(), D3DXVECTOR3(0.0f,D3DX_PI * fRatioRot, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pMagicSword->SetUseDeath(false);
	m_pMagicSword->SetUseAddRot(true,D3DXVECTOR3(0.0f,fRotSpeed, 0.0f));
	SetEnemyType(CEnemy::ENEMYTYPE::SHOTWEAK);//敵タイプを設定
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//終了処理
//====================================================================================
void CShotWeakEnemy::Uninit()
{
	CEnemy::Uninit();
}
//============================================================================================================================================

//====================================================================================
//更新処理
//====================================================================================
void CShotWeakEnemy::Update()
{
	CEnemy::Update();

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		m_pMagicSword->SetPos(GetPos());

		SwordCollision();//剣の当たり判定を行う
	}
}
//============================================================================================================================================

//====================================================================================
//描画処理
//====================================================================================
void CShotWeakEnemy::Draw()
{
	CEnemy::Draw();
}
//============================================================================================================================================

//====================================================================================
//死亡フラグ設定処理
//====================================================================================
void CShotWeakEnemy::SetDeath()
{
	CEnemy::SetDeath();
	if (GetUseDeath() == true)
	{
		if (m_pMagicSword != nullptr)
		{
			m_pMagicSword->SetUseDeath(true);
			m_pMagicSword->SetDeath();
		}
	}
}
//============================================================================================================================================

//====================================================================================
//生成処理
//====================================================================================
CShotWeakEnemy* CShotWeakEnemy::Create(SHOTWEAKENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CShotWeakEnemy* pShotWeakEnemy = DBG_NEW CShotWeakEnemy;

	pShotWeakEnemy->Init();            //初期化処理
	int nIdx = CManager::GetObjectXInfo()->Regist(s_aSHOTWEAKENEMY_FILENAME[int(Type)]);
	pShotWeakEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pShotWeakEnemy->SetPhaseNum(nPhaseNum);//フェーズ番号を設定する
	pShotWeakEnemy->m_ShotWeakEnemyType = Type;
	pShotWeakEnemy->SetLife(nLife);    //体力
	pShotWeakEnemy->SetMaxLife(nLife); //最大体力
	pShotWeakEnemy->SetAutoSubLife(false);
	pShotWeakEnemy->SetPos(pos);       //位置
	pShotWeakEnemy->SetSupportPos(pos);//支点位置
	pShotWeakEnemy->SetRot(rot);       //向き
	pShotWeakEnemy->SetScale(Scale);   //拡大率
	pShotWeakEnemy->SetFormarScale(Scale);//元の拡大率を設定
	pShotWeakEnemy->SetSensingRange(1250.0f);//感知射程
	pShotWeakEnemy->SetNormalSpeed(10.0f);//通常移動速度
	pShotWeakEnemy->SetUseInteria(false, GetNormalInertia());
	pShotWeakEnemy->m_pMagicSword->SetScale(Scale * 0.5f);

	pShotWeakEnemy->SetSize();//モデルサイズを設定
	pShotWeakEnemy->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::SHOTWEAKENEMY);           //マネージャーで呼び出す時の種類を設定
	return pShotWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//テキストファイルに情報を保存する
//====================================================================================
void CShotWeakEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETSHOTWEAKENEMY" << endl;
	WritingFile << "SHOTWEAKENEMYTYPE = " << static_cast<int>(m_ShotWeakEnemyType);
	switch (m_ShotWeakEnemyType)
	{
	case SHOTWEAKENEMYTYPE::NORMAL:
		WritingFile << " # NORMAL" << endl;
		break;
	default:
		assert(false);
		break;
	}

	CEnemy::SaveInfoTxt(WritingFile);

	WritingFile << "END_SETSHOTWEAKENEMY" << endl;
}
//============================================================================================================================================

//====================================================================================
//テキストファイルから情報を読み込む
//====================================================================================
void CShotWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	int nType = 0;                                     //種類
	int nShotWeakEnemyType = 0;                        //ショットに弱い敵タイプ   
	int nLife = 0;                                     //体力
	int nPhaseNum = 0;                                 //フェーズ番号
	int nDivisionNum = 0;                              //分裂回数
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //移動量
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //位置
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //拡大率
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //向き
	SHOTWEAKENEMYTYPE ShotWeakEnemyType = {};          //背景モデルの種類
	ENEMYTYPE EnemyType = {};

	vector<CAIModel*> VecMoveAi = {};
	vector<MoveAiInfo> VecMoveAiInfo = {};

	int nCntMoveAi = 0;
	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fNormalSpeed = 0.0f; //通常速度
	float fSensingRange = 0.0f;//索敵距離

	while (Buff != "END_SETSHOTWEAKENEMY")
	{
		LoadingFile >> Buff;//単語を読み込む
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "SHOTWEAKENEMYTYPE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nShotWeakEnemyType;
		}
		else if (Buff == "ENEMYTYPE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nType;
		}
		else if (Buff == "LIFE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nLife;
		}
		else if (Buff == "POS")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Pos.x;      //位置X
			LoadingFile >> Pos.y;      //位置Y
			LoadingFile >> Pos.z;      //位置Z
		}
		else if (Buff == "ROT")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Rot.x;      //位置X
			LoadingFile >> Rot.y;      //位置Y
			LoadingFile >> Rot.z;      //位置Z
		}
		else if (Buff == "SCALE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Scale.x;      //拡大率X
			LoadingFile >> Scale.y;      //拡大率Y
			LoadingFile >> Scale.z;      //拡大率Z
		}
		else if (Buff == "PHASENUM")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nPhaseNum;//フェーズ番号
		}
		else if (Buff == "NORMALSPEED")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> fNormalSpeed;//通常速度
		}
		else if (Buff == "SENSINGRANGE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> fSensingRange;
		}
		else if (Buff == "SETMOVEAI")
		{
			while (1)
			{
				LoadingFile >> Buff;
				if (Buff == "SETNUM")
				{
					LoadingFile >> Buff;//イコール
					LoadingFile >> nCntMoveAi;//番号
					while (1)
					{
						LoadingFile >> Buff;

						if (Buff == "POS")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiPos.x;      //位置X
							LoadingFile >> MoveAiPos.y;      //位置Y
							LoadingFile >> MoveAiPos.z;      //位置Z
						}
						else if (Buff == "ROT")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiRot.x;      //位置X
							LoadingFile >> MoveAiRot.y;      //位置Y
							LoadingFile >> MoveAiRot.z;      //位置Z
						}
						else if (Buff == "SCALE")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiScale.x;      //拡大率X
							LoadingFile >> MoveAiScale.y;      //拡大率Y
							LoadingFile >> MoveAiScale.z;      //拡大率Z
						}
						else if (Buff == "END_SETNUM")
						{
							if (CScene::GetMode() == CScene::MODE_EDIT)
							{
								CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr);
								pAiModel->SetUseDraw(false);
								pAiModel->SetUseShadow(false);
								VecMoveAi.push_back(pAiModel);
							}
							else if (CScene::GetMode() == CScene::MODE_GAME)
							{
								MoveAiInfo Info = {};
								Info.Pos = MoveAiPos;
								Info.Rot = MoveAiRot;
								Info.Scale = MoveAiScale;
								VecMoveAiInfo.push_back(Info);
							}
							break;
						}
					}
				}
				else if (Buff == "END_SETMOVEAI")
				{
					break;
				}
			}
		}
	}
	ShotWeakEnemyType = static_cast<SHOTWEAKENEMYTYPE>(nShotWeakEnemyType);
	EnemyType = static_cast<ENEMYTYPE>(nType);
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		CShotWeakEnemy* pShotWeakEnemy = CShotWeakEnemy::Create(ShotWeakEnemyType,nLife,nPhaseNum,Pos,Rot,Scale);
		pShotWeakEnemy->SetVecMoveAiInfo(VecMoveAi);
		pShotWeakEnemy->SetNormalSpeed(fNormalSpeed);
		pShotWeakEnemy->SetSensingRange(fSensingRange);
		listSaveManager.push_back(pShotWeakEnemy);      //vectorに情報を保存する

	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType), nShotWeakEnemyType, nPhaseNum,fNormalSpeed,fSensingRange,0,VecMoveAiInfo);
	}

}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーのオブジェクトをチェンジする
//====================================================================================
CObject* CShotWeakEnemy::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_ShotWeakEnemyType);
	SHOTWEAKENEMYTYPE NewType = {};
	//=======================================
	//種類を変える
	//=======================================
	if (bAim == true)
	{
		nNewType++;
	}
	else
	{
		nNewType--;
	}
	if (nNewType >= static_cast<int>(SHOTWEAKENEMYTYPE::MAX))
	{
		nNewType = 0;
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(SHOTWEAKENEMYTYPE::MAX) - 1;
	}
	//======================================================================================

	//=======================================
	//設定する
	//=======================================
	NewType = SHOTWEAKENEMYTYPE(nNewType);
	//======================================================================================

	//=======================================
	//死亡フラグを設定する
	//=======================================
	SetUseDeath(true);
	SetDeath();
	//======================================================================================

	return CShotWeakEnemy::Create(NewType, GetMaxLife(), GetPhaseNum(), GetPos(), GetRot(), GetScale());//生成したオブジェクトを返す
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーに現在のオブジェクトを保存する
//====================================================================================
CObject* CShotWeakEnemy::ManagerSaveObject()
{
	auto& Vec = GetVecAiModelInfo();
	auto Vec2 = move(Vec);
	CShotWeakEnemy * pShotWeakEnemy = CShotWeakEnemy::Create(m_ShotWeakEnemyType, GetMaxLife(),GetPhaseNum(),GetPos(), GetRot(), GetScale());//生成したオブジェクトを返す
	pShotWeakEnemy->SetVecMoveAiInfo(Vec2);
	pShotWeakEnemy->SetNormalSpeed(GetNormalSpeed());//現在の敵の通常速度を保存する
	pShotWeakEnemy->SetSensingRange(GetSensingRange());//現在の敵の索敵範囲を保存する
	return pShotWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//バトル移動処理
//====================================================================================
void CShotWeakEnemy::BattleMoveProcess()
{
	CEnemy::BattleMoveProcess();
}
//============================================================================================================================================

//====================================================================================
//攻撃処理
//====================================================================================
void CShotWeakEnemy::AttackProcess()
{
	float fLength = CCalculation::CalculationLength(GetPos(), CGame::GetPlayer()->GetPos());
	const bool& bAction = GetAction();
	const int& nPatternTime = GetPatternTime();
	const int& nPattern = GetPattern();
	m_nAttackCoolTime++;//クールタイムをカウントする
	if (fLength < s_fATTACKSTART_LENGTH && bAction == false && m_nAttackCoolTime > s_nATTACK_COOLTIME)
	{//攻撃が開始されていなければ
		ChengeMove(DBG_NEW CEnemyMove_None());//AI移動と攻撃処理を入れ替える
		SetAction(true);
	}

	if (bAction == true)
	{
		switch (nPattern)
		{
		case 0:
			SetMove(D3DXVECTOR3(0.0f, GetMove().y, 0.0f));
			if (nPatternTime == 15)
			{
				SetPattern(nPattern + 1);
				SetPatternTime(0);
				m_SaveAimPos = CGame::GetPlayer()->GetPos();
			}
			break;
		case 1:
			CParticle::SummonChargeParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 45, 5.0f, 20.0f, 20.0f, GetSize().x, 100, 10, false,
				GetSenterPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);

			if (nPatternTime == 45)
			{
				SetUseInteria(true, 0.05f);

				SetMove(CCalculation::Calculation3DVec(GetPos(),m_SaveAimPos, 30.0f));
				SetPattern(nPattern + 1);
				SetPatternTime(0);
			}
			break;
		case 2:
			if (nPatternTime == 60)
			{
				SetAction(false);
				SetUseInteria(true, CObjectXMove::GetNormalInertia());
				ChengeMove(DBG_NEW CEnemyMove_Battle());
				SetPattern(0);
				SetPatternTime(0);
				m_nAttackCoolTime = 0;
			}
			break;
		default:
			break;
		}
		SetPatternTime(nPatternTime + 1);
	}
}
//============================================================================================================================================

//====================================================================================
//剣の当たり判定を行う
//====================================================================================
void CShotWeakEnemy::SwordCollision()
{
	if (CCollision::RectAngleCollisionXZ(m_pMagicSword,CGame::GetPlayer()))
	{
		D3DXVECTOR3 Aim = D3DXVECTOR3(CGame::GetPlayer()->GetPos().x,0.0f,CGame::GetPlayer()->GetPos().z) - D3DXVECTOR3(GetPos().x, 0.0f, GetPos().z);

		CGame::GetPlayer()->SetDamage(5, 45);

		D3DXVec3Normalize(&Aim, &Aim);

		CGame::GetPlayer()->ChengeAbnormalState(DBG_NEW CPlayerAbnormalState_KnockBack(CGame::GetPlayer(), Aim * 100.0f, 0.1f));
	}
}
//============================================================================================================================================

//********************************************************************************************************************************************
//ダイブに弱い敵クラス
//********************************************************************************************************************************************

//====================================================================================
//静的メンバ変数宣言
//====================================================================================
const string CDiveWeakEnemy::s_aDIVEWEAKENEMY_FILENAME[static_cast<int>(CDiveWeakEnemy::DIVEWEAKENEMYTYPE::MAX)] =
{
	"data\\MODEL\\Enemy\\DiveWeak\\angrySlime.x"
};
const int CDiveWeakEnemy::s_nATTACK_FREQUENCY = 105;//攻撃頻度
const float CDiveWeakEnemy::s_fSENSINGRANGE = 600.0f;
const float CDiveWeakEnemy::s_fNORMAL_SPEED = 3.0f;
//====================================================================================
//コンストラクタ
//====================================================================================
CDiveWeakEnemy::CDiveWeakEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CEnemy(nPri, bUseintPri, type, ObjType),
m_DiveWeakEnemyType(DIVEWEAKENEMYTYPE::NORMAL),m_nDivisionNum(0)
{

}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CDiveWeakEnemy::~CDiveWeakEnemy()
{

}
//============================================================================================================================================

//====================================================================================
//初期化処理
//====================================================================================
HRESULT CDiveWeakEnemy::Init()
{
	CEnemy::Init();
	SetEnemyType(CEnemy::ENEMYTYPE::DIVEWEAK);//敵タイプを設定
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//終了処理
//====================================================================================
void CDiveWeakEnemy::Uninit()
{
	CEnemy::Uninit();
}
//============================================================================================================================================

//====================================================================================
//更新処理
//====================================================================================
void CDiveWeakEnemy::Update()
{
	CEnemy::Update();
}
//============================================================================================================================================

//====================================================================================
//描画処理
//====================================================================================
void CDiveWeakEnemy::Draw()
{
	CEnemy::Draw();
}
//============================================================================================================================================

//====================================================================================
//死亡フラグ設定処理
//====================================================================================
void CDiveWeakEnemy::SetDeath()
{
	if (m_nDivisionNum > 0 && GetDefeatAttackType() == CAttack::ATTACKTYPE::BULLET && GetLife() < 1)
	{ 
		float fPosX = static_cast<float>(rand() % 30 - 15);
		float fPosZ = static_cast<float>(rand() % 30 - 15);
		m_nDivisionNum--;
		CDiveWeakEnemy * pDiveWeakEnemy = CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE::NORMAL, GetMaxLife()/ 2, 0, GetPos() + D3DXVECTOR3(fPosX, 100.0f, fPosZ), GetRot(), GetScale() / 2, m_nDivisionNum);
		pDiveWeakEnemy->SetSensingRange(9999.0f);
		pDiveWeakEnemy = CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE::NORMAL, GetMaxLife()/ 2, 0, GetPos() + D3DXVECTOR3(fPosZ, 100.0f, fPosX), GetRot(), GetScale() / 2, m_nDivisionNum);
		pDiveWeakEnemy->SetSensingRange(9999.0f);
	}

	CEnemy::SetDeath();
}
//============================================================================================================================================

//====================================================================================
//生成処理
//====================================================================================
CDiveWeakEnemy* CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale, int nDivisionNum)
{
	CDiveWeakEnemy* pDiveWeakEnemy = DBG_NEW CDiveWeakEnemy;

	pDiveWeakEnemy->Init();            //初期化処理
	int nIdx = CManager::GetObjectXInfo()->Regist(s_aDIVEWEAKENEMY_FILENAME[int(Type)]);
	pDiveWeakEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pDiveWeakEnemy->SetPhaseNum(nPhaseNum);//フェーズ番号を設定する
	pDiveWeakEnemy->m_DiveWeakEnemyType = Type;
	pDiveWeakEnemy->SetLife(nLife);    //体力
	pDiveWeakEnemy->SetMaxLife(nLife); //最大体力
	pDiveWeakEnemy->SetPos(pos);       //位置
	pDiveWeakEnemy->SetSupportPos(pos);//支点位置
	pDiveWeakEnemy->SetRot(rot);       //向き
	pDiveWeakEnemy->SetScale(Scale);   //拡大率
	pDiveWeakEnemy->SetFormarScale(Scale);//元の拡大率を設定
	pDiveWeakEnemy->SetSensingRange(550.0f);//感知射程
	pDiveWeakEnemy->SetNormalSpeed(s_fNORMAL_SPEED);//通常移動速度
	pDiveWeakEnemy->SetUseInteria(false, GetNormalInertia());
	pDiveWeakEnemy->SetCntTime(rand() % 100 + 1);
	pDiveWeakEnemy->SetDivisionNum(nDivisionNum);

	pDiveWeakEnemy->SetSize();//モデルサイズを設定
	pDiveWeakEnemy->SetAutoSubLife(false);//自動的に体力を減らすかどうか
	pDiveWeakEnemy->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::DIVEWEAKENEMY);           //マネージャーで呼び出す時の種類を設定
	return pDiveWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//テキストファイルに情報を保存する
//====================================================================================
void CDiveWeakEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETDIVEWEAKENEMY" << endl;
	WritingFile << "DIVEWEAKENEMYTYPE = " << static_cast<int>(m_DiveWeakEnemyType);
	switch (m_DiveWeakEnemyType)
	{
	case DIVEWEAKENEMYTYPE::NORMAL:
		WritingFile << " # NORMAL" << endl;
		break;
	default:
		assert(false);
		break;
	}

	WritingFile << "DIVISIONNUM = " << m_nDivisionNum << endl;

	CEnemy::SaveInfoTxt(WritingFile);

	WritingFile << "END_SETDIVEWEAKENEMY" << endl;
}
//============================================================================================================================================

//====================================================================================
//テキストファイルから情報を読み込む
//====================================================================================
void CDiveWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	int nType = 0;                                     //種類
	int nDiveWeakEnemyType = 0;                        //ダイブに弱い敵タイプ   
	int nLife = 0;                                     //体力
	int nPhaseNum = 0;                                 //フェーズ番号
	int nDivisionNum = 0;                              //分裂回数
 	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //移動量
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //位置
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //拡大率
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //向き
    DIVEWEAKENEMYTYPE DiveWeakEnemyType = {};          //背景モデルの種類
	ENEMYTYPE EnemyType = {};

	vector<CAIModel*> VecMoveAi = {};
	vector<MoveAiInfo> VecMoveAiInfo = {};

	int nCntMoveAi = 0;
	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f,0.0f,0.0f);

	float fNormalSpeed = 0.0f; //通常速度
	float fSensingRange = 0.0f;//索敵距離
	while (Buff != "END_SETDIVEWEAKENEMY")
	{
		LoadingFile >> Buff;//単語を読み込む
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "DIVEWEAKENEMYTYPE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nDiveWeakEnemyType;
		}
		else if (Buff == "ENEMYTYPE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nType;
		}
		else if (Buff == "LIFE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nLife;
		}
		else if (Buff == "POS")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Pos.x;      //位置X
			LoadingFile >> Pos.y;      //位置Y
			LoadingFile >> Pos.z;      //位置Z
		}
		else if (Buff == "ROT")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Rot.x;      //位置X
			LoadingFile >> Rot.y;      //位置Y
			LoadingFile >> Rot.z;      //位置Z
		}
		else if (Buff == "SCALE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Scale.x;      //拡大率X
			LoadingFile >> Scale.y;      //拡大率Y
			LoadingFile >> Scale.z;      //拡大率Z
		}
		else if (Buff == "PHASENUM")
		{
			LoadingFile >> Buff;      //イコール
			LoadingFile >> nPhaseNum; //フェーズ番号
		}
		else if (Buff == "NORMALSPEED")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> fNormalSpeed;//通常速度
		}
		else if (Buff == "SENSINGRANGE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> fSensingRange;
		}
		else if (Buff == "DIVISIONNUM")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nDivisionNum;//分裂回数
		}
		else if (Buff == "SETMOVEAI")
		{
			while (1)
			{
				LoadingFile >> Buff;
				if (Buff == "SETNUM")
				{
					LoadingFile >> Buff;//イコール
					LoadingFile >> nCntMoveAi;//番号
					while (1)
					{
						LoadingFile >> Buff;

						if (Buff == "POS")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiPos.x;      //位置X
							LoadingFile >> MoveAiPos.y;      //位置Y
							LoadingFile >> MoveAiPos.z;      //位置Z
						}
						else if (Buff == "ROT")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiRot.x;      //位置X
							LoadingFile >> MoveAiRot.y;      //位置Y
							LoadingFile >> MoveAiRot.z;      //位置Z
						}
						else if (Buff == "SCALE")
						{
							LoadingFile >> Buff;//イコール
							LoadingFile >> MoveAiScale.x;      //拡大率X
							LoadingFile >> MoveAiScale.y;      //拡大率Y
							LoadingFile >> MoveAiScale.z;      //拡大率Z
						}
						else if (Buff == "END_SETNUM")
						{
							if (CScene::GetMode() == CScene::MODE_EDIT)
							{
								CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr);
								pAiModel->SetUseDraw(true);
								pAiModel->SetUseShadow(true);
								VecMoveAi.push_back(pAiModel);
							}
							else if (CScene::GetMode() == CScene::MODE_GAME)
							{
								MoveAiInfo Info = {};
								Info.Pos = MoveAiPos;
								Info.Rot = MoveAiRot;
								Info.Scale = MoveAiScale;
								VecMoveAiInfo.push_back(Info);
							}
							break;
						}
					}
				}
				else if (Buff == "END_SETMOVEAI")
				{
					break;
				}
			}
		}
	}

	DiveWeakEnemyType = static_cast<DIVEWEAKENEMYTYPE>(nDiveWeakEnemyType);
	EnemyType = static_cast<ENEMYTYPE>(nType);
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		CDiveWeakEnemy* pDiveWeakEnemy = CDiveWeakEnemy::Create(DiveWeakEnemyType, nLife, nPhaseNum, Pos, Rot, Scale,3);
		pDiveWeakEnemy->SetUseDraw(true);
		pDiveWeakEnemy->SetUseShadow(true);
		pDiveWeakEnemy->SetVecMoveAiInfo(VecMoveAi);
		pDiveWeakEnemy->SetNormalSpeed(fNormalSpeed);
		pDiveWeakEnemy->SetSensingRange(fSensingRange);
		listSaveManager.push_back(pDiveWeakEnemy);      //vectorに情報を保存する
	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType), nDiveWeakEnemyType, nPhaseNum,fNormalSpeed,fSensingRange,3,VecMoveAiInfo);
	}
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーのオブジェクトをチェンジする
//====================================================================================
CObject* CDiveWeakEnemy::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_DiveWeakEnemyType);
	DIVEWEAKENEMYTYPE NewType = {};
	//=======================================
	//種類を変える
	//=======================================
	if (bAim == true)
	{
		nNewType++;
	}
	else
	{
		nNewType--;
	}
	if (nNewType >= static_cast<int>(DIVEWEAKENEMYTYPE::MAX))
	{
		nNewType = 0;
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(DIVEWEAKENEMYTYPE::MAX) - 1;
	}
	//======================================================================================

	//=======================================
	//設定する
	//=======================================
	NewType = static_cast<DIVEWEAKENEMYTYPE>(nNewType);
	//======================================================================================

	//=======================================
	//死亡フラグを設定する
	//=======================================
	SetUseDeath(true);
	SetDeath();
	//======================================================================================

	return CDiveWeakEnemy::Create(NewType, GetLife(),GetPhaseNum(),GetPos(), GetRot(), GetScale(),GetDivisionNum());//生成したオブジェクトを返す
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーに現在のオブジェクトを保存する
//====================================================================================
CObject* CDiveWeakEnemy::ManagerSaveObject()
{
	auto& Vec = GetVecAiModelInfo();
	auto Vec2 = move(Vec);
	CDiveWeakEnemy * pDiveWeakEnemy = CDiveWeakEnemy::Create(m_DiveWeakEnemyType, GetMaxLife(),GetPhaseNum(),GetPos(), GetRot(), GetScale(),GetDivisionNum());//生成したオブジェクトを返す
	pDiveWeakEnemy->SetSensingRange(GetSensingRange());//現在の敵の索敵範囲を保存する
	pDiveWeakEnemy->SetNormalSpeed(GetNormalSpeed());//現在の敵の通常速度を保存する
	pDiveWeakEnemy->SetVecMoveAiInfo(Vec2);
	return pDiveWeakEnemy;//生成したオブジェクトを返す
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーから操作する
//====================================================================================
void CDiveWeakEnemy::ManagerChooseControlInfo()
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
			{
				m_nDivisionNum -= 1;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{
			m_nDivisionNum += 1;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
			{
				m_nDivisionNum -= 1;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{
			m_nDivisionNum += 1;
		}
	}

	CManager::GetDebugProc()->PrintDebugProc("分裂回数(U)：%d\n", m_nDivisionNum);

	CEnemy::ManagerChooseControlInfo();
}
//============================================================================================================================================

//====================================================================================
//バトル移動処理
//====================================================================================
void CDiveWeakEnemy::BattleMoveProcess()
{
	CEnemy::BattleMoveProcess();
}
//============================================================================================================================================

//====================================================================================
//攻撃処理
//====================================================================================
void CDiveWeakEnemy::AttackProcess()
{
	if (GetCntTime() % s_nATTACK_FREQUENCY == 0)
	{
		D3DXVECTOR3 Aim = CCalculation::Calculation3DVec(GetPos() + D3DXVECTOR3(0.0f, GetVtxMax().y + GetVtxMax().y / 2, 0.0f), CGame::GetPlayer()->GetSenterPos(), 20.0f);

		CAttackEnemy::Create(CAttack::ATTACKTYPE::EXPLOSION, CAttack::TARGETTYPE::PLAYER, CAttack::COLLISIONTYPE::SQUARE, true, true, 1, 60, 200, GetPos() + D3DXVECTOR3(0.0f, GetVtxMax().y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), Aim, GetScale() * 0.5f);
	}
}
//============================================================================================================================================

//====================================================================================
//AI移動処理
//====================================================================================
void CDiveWeakEnemy::AIMoveProcess()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CEnemy::AIMoveProcess();
	}
}
//============================================================================================================================================

//********************************************************************************************************************************************
//敵移動タイプクラス
//********************************************************************************************************************************************

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemyMove::CEnemyMove()
{

}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemyMove::~CEnemyMove()
{

}
//============================================================================================================================================

//====================================================================================
//処理
//====================================================================================
void CEnemyMove::Process(CEnemy* pEnemy)
{

}
//============================================================================================================================================

//********************************************************************************************************************************************
//敵移動タイプ：AIクラス
//********************************************************************************************************************************************

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemyMove_AI::CEnemyMove_AI()
{

}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemyMove_AI::~CEnemyMove_AI()
{

}
//============================================================================================================================================

//====================================================================================
//処理
//====================================================================================
void CEnemyMove_AI::Process(CEnemy* pEnemy)
{
	pEnemy->AIMoveProcess();//AI移動処理を行う
}
//============================================================================================================================================

//********************************************************************************************************************************************
//敵移動タイプ：バトルクラス
//********************************************************************************************************************************************

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemyMove_Battle::CEnemyMove_Battle()
{

}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemyMove_Battle::~CEnemyMove_Battle()
{

}
//============================================================================================================================================

//====================================================================================
//処理
//====================================================================================
void CEnemyMove_Battle::Process(CEnemy* pEnemy)
{
	pEnemy->BattleMoveProcess();
}
//============================================================================================================================================

//********************************************************************************************************************************************
//敵移動タイプ：なしクラス
//********************************************************************************************************************************************

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemyMove_None::CEnemyMove_None()
{

}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemyMove_None::~CEnemyMove_None()
{

}
//============================================================================================================================================

//====================================================================================
//処理
//====================================================================================
void CEnemyMove_None::Process(CEnemy* pEnemy)
{

}
//============================================================================================================================================

//************************************************************************************
//怯え移動クラス
//************************************************************************************

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemyMove_Frightened::CEnemyMove_Frightened(CEnemy* pEnemy, D3DXVECTOR3 StopPos, int nStateTime) : m_StopPos(StopPos),m_nStateTime(nStateTime),m_pLockOn(nullptr)
{
	D3DXVECTOR3 ScreenPos = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pEnemy->GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
		SCREEN_WIDTH,SCREEN_HEIGHT);

	m_pLockOn = CUi::Create(CUi::UITYPE::TARGET_000, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 200.0f, 100, true, D3DXVECTOR3(ScreenPos.x, ScreenPos.y, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));

	m_pLockOn->SetPolygonRotSpeed(0.1f);
	m_pLockOn->SetUseAddScale(D3DXVECTOR2(-0.01f, -0.01f), true);
	m_pLockOn->SetUseDeath(false);
}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemyMove_Frightened::~CEnemyMove_Frightened()
{
	if (m_pLockOn != nullptr)
	{
		m_pLockOn->SetUseDeath(true);
		m_pLockOn->SetDeath();
		m_pLockOn = nullptr;
	}
}
//============================================================================================================================================

//====================================================================================
//処理
//====================================================================================
void CEnemyMove_Frightened::Process(CEnemy* pEnemy)
{
	float fX = static_cast<float>(rand() % 10 - 5);
	float fY = static_cast<float>(rand() % 10 - 5);
	float fZ = static_cast<float>(rand() % 10 - 5);
	pEnemy->SetPos(m_StopPos + D3DXVECTOR3(fX, fY, fZ));//震えさせる
	pEnemy->SetMove(D3DXVECTOR3(0.0f, 0.0f,0.0f));
	m_nStateTime--;

	if (m_pLockOn != nullptr)
	{
		D3DXVECTOR3 ScreenPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pEnemy->GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
			SCREEN_WIDTH, SCREEN_HEIGHT);
		m_pLockOn->SetPos(ScreenPos);
		if (m_pLockOn->GetScale().x < 0.0f && m_pLockOn->GetScale().y < 0.0f)
		{
			m_pLockOn->SetUseDeath(true);
			m_pLockOn->SetDeath();
			m_pLockOn = nullptr;
		}
	}
	if (m_nStateTime < 1)
	{
		pEnemy->SetPos(m_StopPos + D3DXVECTOR3(0.0f,50.0f,0.0f));
		pEnemy->ChengeMove(DBG_NEW CEnemyMove_AI());//AI移動処理に戻す
	}
}
//============================================================================================================================================

//************************************************************************************
//壁回避移動クラス
//************************************************************************************

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemyMove_DodgeWall::CEnemyMove_DodgeWall(CEnemy* pEnemy, D3DXVECTOR3 DodgeMove)
{
	m_DodgeMove = DodgeMove;
}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemyMove_DodgeWall::~CEnemyMove_DodgeWall()
{

}
//============================================================================================================================================

//====================================================================================
//処理
//====================================================================================
void CEnemyMove_DodgeWall::Process(CEnemy* pEnemy)
{
	pEnemy->SetMove(m_DodgeMove);

	if (pEnemy->GetCollisionWall() == false)
	{
		pEnemy->ChengeMove(DBG_NEW CEnemyMove_Battle());
	}
}
//============================================================================================================================================