//======================================================================================================================
//
//�P�P���Q�X���F�t�F�[�Y�}�l�[�W���[�����[phasemanager.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//===============================================================
//�C���N���[�h
//===============================================================
#include "phasemanager.h"
#include "aimodel.h"
#include "manager.h"
#include "fade.h"
//======================================================================================================================

//===============================================================
//�ÓI�����o�錾
//===============================================================
list<CPhaseManager::PhaseSaveInfo> CPhaseManager::s_PhaseList = {};
int CPhaseManager::s_nNowPhase = 0;
int CPhaseManager::s_MaxPhase = 0;
bool CPhaseManager::s_bStartFade = false;
//======================================================================================================================

//===============================================================
//�R���X�g���N�^
//===============================================================
CPhaseManager::CPhaseManager(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) :CObject(nPri, bUseintPri, type, ObjType)
{
	s_nNowPhase = 0;
	s_MaxPhase = 0;
	s_bStartFade = false;
}
//======================================================================================================================

//===============================================================
//�f�X�g���N�^
//===============================================================
CPhaseManager::~CPhaseManager()
{

}
//======================================================================================================================

//===============================================================
//����������
//===============================================================
HRESULT CPhaseManager::Init()
{
	CObject::Init();
	return S_OK;
}
//======================================================================================================================

//===============================================================
//�I������
//===============================================================
void CPhaseManager::Uninit()
{
	CObject::Uninit();
}
//======================================================================================================================

//===============================================================
//�X�V����
//===============================================================
void CPhaseManager::Update()
{
	AdvancePhase();
	CObject::Update();
}
//======================================================================================================================

//===============================================================
//�`�揈��
//===============================================================
void CPhaseManager::Draw()
{
	CObject::Draw();
}
//======================================================================================================================

//===============================================================
//���S�t���O�ݒ菈��
//===============================================================
void CPhaseManager::SetDeath()
{
	CObject::SetDeath();

	s_PhaseList.clear();
}
//======================================================================================================================

//===============================================================
//�t�F�[�Y�}�l�[�W���[�ɏ���ݒ肷��
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

	//�t�F�[�Y�ő吔�𒴂��Ă�����X�V
	if (nPhaseNum > s_MaxPhase)
	{
		s_MaxPhase = nPhaseNum;
	}

	s_PhaseList.push_back(Info);
}
//======================================================================================================================

//===============================================================
//���̃t�F�[�Y�Ɉڍs���鏈��
//===============================================================
void CPhaseManager::AdvancePhase()
{
	if (CEnemy::GetNumEnemy() <= 0 && s_nNowPhase <= s_MaxPhase)
	{

		CEnemy::ENEMYTYPE EnemyType = {};
		CEnemy* pEnemy = nullptr;
		vector<CAIModel*> VecMoveAi = {};//�ړ�AI��vector
		for (auto it : s_PhaseList)
		{
			if (it.nPhaseNum == s_nNowPhase)
			{
				EnemyType = static_cast<CEnemy::ENEMYTYPE>(it.nEnemyType);

				//�ړ�AI���ݒ肳��Ă�����
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

				//�G�̎�ނɂ���Đ���������̂�ς���
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
				//�ړ�AI��ݒ肷��
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
//��������
//===============================================================
CPhaseManager* CPhaseManager::Create()
{
	CPhaseManager* pPhaseManager = DBG_NEW CPhaseManager();

	pPhaseManager->Init();//����������

	return pPhaseManager;
}
//======================================================================================================================