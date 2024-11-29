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
#include "particle.h"
#include "input.h"
//============================================================================================================================================

//====================================================================================
//静的メンバ宣言
//====================================================================================
//const string CEnemy::m_aENEMY_FILENAME[static_cast<int>(ENEMYTYPE::MAX)] =
//{
//	"data\\MODEL\\Enemy\\Normal\\noobSlime.x"
//};

//====================================================================================
//コンストラクタ
//====================================================================================
CEnemy::CEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectXAlive(nPri, bUseintPri, type, ObjType),
m_Type(ENEMYTYPE::SHOTWEAK), m_VecMoveAi(), m_MoveAiSavePos(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{

}
//============================================================================================================================================

//====================================================================================
//デストラクタ
//====================================================================================
CEnemy::~CEnemy()
{

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
}
//============================================================================================================================================

//====================================================================================
//更新処理
//====================================================================================
void CEnemy::Update()
{
	CObjectXAlive::Update();

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		const D3DXVECTOR3& Rot = GetRot();
		const D3DXVECTOR3& Pos = GetPos();
		const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
		float fElevaRot = atan2f(PlayerPos.y - Pos.y, sqrtf(powf(PlayerPos.x - Pos.x, 2) + powf(PlayerPos.z - Pos.z, 2)));
		SetRot(D3DXVECTOR3(fElevaRot, atan2f(PlayerPos.x - Pos.x, PlayerPos.z - Pos.z) + D3DX_PI,0.0f));
	}

	CollisionProcess();
}
//============================================================================================================================================

//====================================================================================
//描画処理
//====================================================================================
void CEnemy::Draw()
{
	CObjectXAlive::Draw();

	//if (CScene::GetMode() == CScene::MODE_EDIT)
	//{
		for (auto pObj : m_VecMoveAi)
		{
			pObj->ExtraDraw();
		}
	//}
}
//============================================================================================================================================

//====================================================================================
//死亡フラグ設定処理
//====================================================================================
void CEnemy::SetDeath()
{
	CObjectXAlive::SetDeath();

	for (auto pObj : m_VecMoveAi)
	{
		pObj->SetUseDeath(true);
		pObj->SetDeath();
	}

	m_VecMoveAi.clear();         //メモリを初期化
	m_VecMoveAi.shrink_to_fit(); //確保メモリを開放
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

	SetMoveAiPoint();//移動AIの設定を行う

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
					ComPos, ComVtxMax, ComVtxMin, bCollisionXOld, bCollisionYOld, bCollisionZOld);

				if (bCollisionY == true)
				{
					SetMove(D3DXVECTOR3(GetMove().x, 0.0f, GetMove().z));
				}

				if (bSuccessCollision == true)
				{
					SetPos(MyPos);
				}
			}

			//オブジェクトを次に進める
			pObj = pNext;
		}

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
	auto Input = CManager::GetInputKeyboard();
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

	CManager::GetDebugProc()->PrintDebugProc("移動AIの設置位置の移動：IKLJ\n");

	if (bMove == true)
	{
		float fRot = atan2f(fX, fZ);
		m_MoveAiSavePos += D3DXVECTOR3(sinf(fRot) * 5.0f, 0.0f, cosf(fRot) * 5.0f);
	}

	CManager::GetDebugProc()->PrintDebugProc("移動AIを保存：O\n");

	CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 45, 30.0f, 30.0f, 100, 10, false, m_MoveAiSavePos + GetPos(), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);

	if (Input->GetTrigger(DIK_O))
	{
		m_VecMoveAi.push_back(CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, m_MoveAiSavePos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3.0f, 3.0f, 3.0f), &GetMatrixWorld()));
	}
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
//============================================================================================================================================

//====================================================================================
//コンストラクタ
//====================================================================================
CShotWeakEnemy::CShotWeakEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CEnemy(nPri,bUseintPri,type,ObjType),
m_ShotWeakEnemyType(SHOTWEAKENEMYTYPE::NORMAL)
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
		const D3DXVECTOR3& Rot = GetRot();
		const D3DXVECTOR3& Pos = GetPos();
		const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
		float fElevaRot = atan2f(PlayerPos.y - Pos.y, sqrtf(powf(PlayerPos.x - Pos.x, 2) + powf(PlayerPos.z - Pos.z, 2)));
		SetRot(D3DXVECTOR3(fElevaRot, atan2f(PlayerPos.x - Pos.x, PlayerPos.z - Pos.z) + D3DX_PI,0.0f));
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
}
//============================================================================================================================================

//====================================================================================
//生成処理
//====================================================================================
CShotWeakEnemy* CShotWeakEnemy::Create(SHOTWEAKENEMYTYPE Type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CShotWeakEnemy* pShotWeakEnemy = DBG_NEW CShotWeakEnemy;

	pShotWeakEnemy->Init();            //初期化処理
	int nIdx = CManager::GetObjectXInfo()->Regist(s_aSHOTWEAKENEMY_FILENAME[int(Type)]);
	pShotWeakEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pShotWeakEnemy->m_ShotWeakEnemyType = Type;
	pShotWeakEnemy->SetLife(nLife);    //体力
	pShotWeakEnemy->SetMaxLife(nLife); //最大体力
	pShotWeakEnemy->SetPos(pos);       //位置
	pShotWeakEnemy->SetSupportPos(pos);//支点位置
	pShotWeakEnemy->SetRot(rot);       //向き
	pShotWeakEnemy->SetScale(Scale);   //拡大率
	pShotWeakEnemy->SetFormarScale(Scale);//元の拡大率を設定

	pShotWeakEnemy->SetSize();//モデルサイズを設定
	pShotWeakEnemy->SetAutoSubLife(false);//自動的に体力を減らすかどうか
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
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //移動量
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //位置
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //拡大率
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //向き
	SHOTWEAKENEMYTYPE ShotWeakEnemyType = {};          //背景モデルの種類
	ENEMYTYPE EnemyType = {};

	vector<CAIModel*> VecMoveAi = {};
	int nCntMoveAi = 0;
	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
							VecMoveAi.push_back(CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr));
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
	EnemyType = static_cast<ENEMYTYPE>(nLife);
	//listSaveManager.push_back(CShotWeakEnemy::Create(ShotWeakEnemyType, nLife, Pos, Rot, Scale));//vectorに情報を保存する

	//DiveWeakEnemyType = static_cast<DIVEWEAKENEMYTYPE>(nDiveWeakEnemyType);
	EnemyType = static_cast<ENEMYTYPE>(nLife);
	CShotWeakEnemy* pShotWeakEnemy = CShotWeakEnemy::Create(ShotWeakEnemyType, nLife, Pos, Rot, Scale);
	for (auto Ai : VecMoveAi)
	{
		Ai->SetMtxParent(&pShotWeakEnemy->GetMatrixWorld());//親マトリックスをAIモデルに設定
	}
	pShotWeakEnemy->SetVecMoveAiInfo(VecMoveAi);
	listSaveManager.push_back(pShotWeakEnemy);//vectorに情報を保存する

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

	return CShotWeakEnemy::Create(NewType,GetLife(), GetPos(), GetRot(), GetScale());//生成したオブジェクトを返す
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーに現在のオブジェクトを保存する
//====================================================================================
CObject* CShotWeakEnemy::ManagerSaveObject()
{
	CShotWeakEnemy * pShotWeakEnemy = CShotWeakEnemy::Create(m_ShotWeakEnemyType, GetLife(), GetPos(), GetRot(), GetScale());//生成したオブジェクトを返す
	pShotWeakEnemy->SetVecMoveAiInfo(GetVecAiModelInfo());
	return pShotWeakEnemy;
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

//====================================================================================
//静的メンバ変数宣言
//====================================================================================

//====================================================================================
//コンストラクタ
//====================================================================================
CDiveWeakEnemy::CDiveWeakEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CEnemy(nPri, bUseintPri, type, ObjType),
m_DiveWeakEnemyType(DIVEWEAKENEMYTYPE::NORMAL)
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

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		const D3DXVECTOR3& Rot = GetRot();
		const D3DXVECTOR3& Pos = GetPos();
		const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
		float fElevaRot = atan2f(PlayerPos.y - Pos.y, sqrtf(powf(PlayerPos.x - Pos.x, 2) + powf(PlayerPos.z - Pos.z, 2)));
		SetRot(D3DXVECTOR3(fElevaRot, atan2f(PlayerPos.x - Pos.x, PlayerPos.z - Pos.z) + D3DX_PI, 0.0f));
	}
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
	CEnemy::SetDeath();
}
//============================================================================================================================================

//====================================================================================
//生成処理
//====================================================================================
CDiveWeakEnemy* CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE Type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CDiveWeakEnemy* pDiveWeakEnemy = DBG_NEW CDiveWeakEnemy;

	pDiveWeakEnemy->Init();            //初期化処理
	int nIdx = CManager::GetObjectXInfo()->Regist(s_aDIVEWEAKENEMY_FILENAME[int(Type)]);
	pDiveWeakEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pDiveWeakEnemy->m_DiveWeakEnemyType = Type;
	pDiveWeakEnemy->SetLife(nLife);    //体力
	pDiveWeakEnemy->SetMaxLife(nLife); //最大体力
	pDiveWeakEnemy->SetPos(pos);       //位置
	pDiveWeakEnemy->SetSupportPos(pos);//支点位置
	pDiveWeakEnemy->SetRot(rot);       //向き
	pDiveWeakEnemy->SetScale(Scale);   //拡大率
	pDiveWeakEnemy->SetFormarScale(Scale);//元の拡大率を設定

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
 	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //移動量
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //位置
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //拡大率
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //向き
    DIVEWEAKENEMYTYPE DiveWeakEnemyType = {};          //背景モデルの種類
	ENEMYTYPE EnemyType = {};

	vector<CAIModel*> VecMoveAi = {};
	int nCntMoveAi = 0;
	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f,0.0f,0.0f);
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
							VecMoveAi.push_back(CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr));
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
	EnemyType = static_cast<ENEMYTYPE>(nLife);
	CDiveWeakEnemy* pDiveWeakEnemy = CDiveWeakEnemy::Create(DiveWeakEnemyType, nLife, Pos, Rot, Scale);
	for (auto Ai : VecMoveAi)
	{
		Ai->SetMtxParent(&pDiveWeakEnemy->GetMatrixWorld());//親マトリックスをAIモデルに設定
	}
	pDiveWeakEnemy->SetVecMoveAiInfo(VecMoveAi);
	listSaveManager.push_back(pDiveWeakEnemy);//vectorに情報を保存する

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

	return CDiveWeakEnemy::Create(NewType, GetLife(), GetPos(), GetRot(), GetScale());//生成したオブジェクトを返す
}
//============================================================================================================================================

//====================================================================================
//ステージマネージャーに現在のオブジェクトを保存する
//====================================================================================
CObject* CDiveWeakEnemy::ManagerSaveObject()
{
	CDiveWeakEnemy * pDiveWeakEnemy = CDiveWeakEnemy::Create(m_DiveWeakEnemyType, GetLife(), GetPos(), GetRot(), GetScale());//生成したオブジェクトを返す
	pDiveWeakEnemy->SetVecMoveAiInfo(GetVecAiModelInfo());
	return pDiveWeakEnemy;//生成したオブジェクトを返す
}
//============================================================================================================================================