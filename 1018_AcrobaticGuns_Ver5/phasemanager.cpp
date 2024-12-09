//======================================================================================================================
//
//１１月２９日：フェーズマネージャーを作る[phasemanager.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//===============================================================
//インクルード
//===============================================================
#include "phasemanager.h"
#include "aimodel.h"
#include "manager.h"
#include "fade.h"
//======================================================================================================================

//===============================================================
//静的メンバ宣言
//===============================================================
list<CPhaseManager::PhaseSaveInfo> CPhaseManager::s_PhaseList = {};
int CPhaseManager::s_nNowPhase = 0;
int CPhaseManager::s_MaxPhase = 0;
bool CPhaseManager::s_bStartFade = false;
//======================================================================================================================

//===============================================================
//コンストラクタ
//===============================================================
CPhaseManager::CPhaseManager(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) :CObject(nPri, bUseintPri, type, ObjType)
{
	s_nNowPhase = 0;
	s_MaxPhase = 0;
	s_bStartFade = false;
}
//======================================================================================================================

//===============================================================
//デストラクタ
//===============================================================
CPhaseManager::~CPhaseManager()
{

}
//======================================================================================================================

//===============================================================
//初期化処理
//===============================================================
HRESULT CPhaseManager::Init()
{
	CObject::Init();
	return S_OK;
}
//======================================================================================================================

//===============================================================
//終了処理
//===============================================================
void CPhaseManager::Uninit()
{
	CObject::Uninit();
}
//======================================================================================================================

//===============================================================
//更新処理
//===============================================================
void CPhaseManager::Update()
{
	AdvancePhase();
	CObject::Update();
}
//======================================================================================================================

//===============================================================
//描画処理
//===============================================================
void CPhaseManager::Draw()
{
	CObject::Draw();
}
//======================================================================================================================

//===============================================================
//死亡フラグ設定処理
//===============================================================
void CPhaseManager::SetDeath()
{
	CObject::SetDeath();

	s_PhaseList.clear();
}
//======================================================================================================================

//===============================================================
//フェーズマネージャーに情報を設定する
//===============================================================
void CPhaseManager::PushPhaseInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale,
	int nLife, int nEnemyType, int nTypeNum, int nPhaseNum, vector<CEnemy::MoveAiInfo> VecMoveAi)
{
	PhaseSaveInfo Info = {};
	Info.Pos = Pos;
	Info.Rot = Rot;
	Info.Scale = Scale;
	Info.nLife = nLife;
	Info.nEnemyType = nEnemyType;
	Info.nTypeNum = nTypeNum;
	Info.nPhaseNum = nPhaseNum;
	Info.VecMoveAi = VecMoveAi;

	//フェーズ最大数を超えていたら更新
	if (nPhaseNum > s_MaxPhase)
	{
		s_MaxPhase = nPhaseNum;
	}

	s_PhaseList.push_back(Info);
}
//======================================================================================================================

//===============================================================
//次のフェーズに移行する処理
//===============================================================
void CPhaseManager::AdvancePhase()
{
	if (CEnemy::GetNumEnemy() <= 0 && s_nNowPhase <= s_MaxPhase)
	{

		CEnemy::ENEMYTYPE EnemyType = {};
		CEnemy* pEnemy = nullptr;
		vector<CAIModel*> VecMoveAi = {};//移動AIのvector
		for (auto it : s_PhaseList)
		{
			if (it.nPhaseNum == s_nNowPhase)
			{
				EnemyType = static_cast<CEnemy::ENEMYTYPE>(it.nEnemyType);

				//移動AIが設定されていたら
				if (it.VecMoveAi.size() > 0)
				{
					for (const auto it2 : it.VecMoveAi)
					{
						CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, it2.Pos, it2.Rot, it2.Scale, nullptr);
						pAiModel->SetUseDraw(false);
						pAiModel->SetUseShadow(false);
						VecMoveAi.push_back(pAiModel);
					}
				}

				//敵の種類によって生成するものを変える
				switch (EnemyType)
				{
				case CEnemy::ENEMYTYPE::SHOTWEAK:
					pEnemy = CShotWeakEnemy::Create(static_cast<CShotWeakEnemy::SHOTWEAKENEMYTYPE>(it.nTypeNum), it.nLife, it.nPhaseNum, it.Pos, it.Rot, it.Scale);
					break;
				case CEnemy::ENEMYTYPE::DIVEWEAK:
					pEnemy = CDiveWeakEnemy::Create(static_cast<CDiveWeakEnemy::DIVEWEAKENEMYTYPE>(it.nTypeNum), it.nLife, it.nPhaseNum, it.Pos, it.Rot, it.Scale);
					break;
				default:
					break;
				}

				auto CopyVec = move(VecMoveAi);
				//移動AIを設定する
				pEnemy->SetVecMoveAiInfo(CopyVec);
			}

		}
		
		s_nNowPhase++;
	}

	if (CEnemy::GetNumEnemy() <= 0 && s_nNowPhase == s_MaxPhase + 1 && s_bStartFade == false)
	{
		s_bStartFade = true;
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	}
}
//======================================================================================================================

//===============================================================
//生成処理
//===============================================================
CPhaseManager* CPhaseManager::Create()
{
	CPhaseManager* pPhaseManager = DBG_NEW CPhaseManager();

	pPhaseManager->Init();//初期化処理

	return pPhaseManager;
}
//======================================================================================================================