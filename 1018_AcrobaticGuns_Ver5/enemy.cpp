//============================================================================================================================================
//
//�P�P���Q�T���F�G�����[enemy.cpp]
//Author:ShinaTaiyo
//
//============================================================================================================================================

//====================================================================================
//�C���N���[�h
//====================================================================================
#include "enemy.h"
#include "manager.h"
#include "objectXInfo.h"
#include "game.h"
#include "calculation.h"
#include "debugproc.h"
#include "collision.h"
#include "particle.h"
#include "phasemanager.h"
#include "input.h"
#include "attack.h"
//============================================================================================================================================

//====================================================================================
//�ÓI�����o�錾
//====================================================================================
//const string CEnemy::m_aENEMY_FILENAME[static_cast<int>(ENEMYTYPE::MAX)] =
//{
//	"data\\MODEL\\Enemy\\Normal\\noobSlime.x"
//};
int CEnemy::m_nNumEnemy = 0;

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CEnemy::CEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectXAlive(nPri, bUseintPri, type, ObjType),
m_Type(ENEMYTYPE::SHOTWEAK), m_VecMoveAi(), m_MoveAiSavePos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),m_nCntTime(0),m_nIdxMoveAi(0), m_nPhaseNum(0),m_pEnemyMove(DBG_NEW CEnemyMove_AI())
{
	m_nNumEnemy++;//�G�����J�E���g�A�b�v
}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CEnemy::~CEnemy()
{
	m_nNumEnemy--;//�G�����J�E���g�_�E��
}
//============================================================================================================================================

//====================================================================================
//����������
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
//�I������
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
//�X�V����
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

		float fLength = CCalculation::CalculationLength(Pos, PlayerPos);
		m_pEnemyMove->Process(this);

		m_nCntTime++;//���Ԃ��J�E���g����
	}

	CollisionProcess();
}
//============================================================================================================================================

//====================================================================================
//�`�揈��
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
//���S�t���O�ݒ菈��
//====================================================================================
void CEnemy::SetDeath()
{
	CObjectXAlive::SetDeath();

	for (auto pObj : m_VecMoveAi)
	{
		pObj->SetUseDeath(true);
		pObj->SetDeath();
	}

	m_VecMoveAi.clear();
	m_VecMoveAi.shrink_to_fit();
}
//============================================================================================================================================

//====================================================================================
//��������
//====================================================================================
//CEnemy* CEnemy::Create(ENEMYTYPE Type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
//{
//	CEnemy* pEnemy = DBG_NEW CEnemy;
//
//	pEnemy->Init();            //����������
//	int nIdx = CManager::GetObjectXInfo()->Regist(m_aENEMY_FILENAME[int(Type)]);
//	pEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
//		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
//		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
//		CManager::GetObjectXInfo()->GetTexture(nIdx),
//		CManager::GetObjectXInfo()->GetColorValue(nIdx));
//
//	pEnemy->SetType(CObject::TYPE::ENEMY);//�I�u�W�F�N�g�^�C�v��ݒ�
//	pEnemy->SetEnemyType(Type);//�G�̎�ނ�ݒ�
//	pEnemy->SetLife(nLife);    //�̗�
//	pEnemy->SetMaxLife(nLife); //�ő�̗�
//	pEnemy->SetPos(pos);       //�ʒu
//	pEnemy->SetSupportPos(pos);//�x�_�ʒu
//	pEnemy->SetRot(rot);       //����
//	pEnemy->SetScale(Scale);   //�g�嗦
//	pEnemy->SetFormarScale(Scale);//���̊g�嗦��ݒ�
//	pEnemy->SetSize();//���f���T�C�Y��ݒ�
//	pEnemy->SetAutoSubLife(false);//�����I�ɑ̗͂����炷���ǂ���
//
//	return pEnemy;
//}
////============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���ɏ����Z�[�u����
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

	//�t�F�[�Y�ԍ���ݒ�
	WritingFile << "PHASENUM = " << m_nPhaseNum << endl;

	//�ړ�AI�̈ʒu��ۑ�
	WritingFile << "SETMOVEAI" << endl;

	int nCnt = 0;
	for (auto it = m_VecMoveAi.begin(); it != m_VecMoveAi.end(); ++it)
	{
		WritingFile << "SETNUM = " << nCnt << endl;

		WritingFile << "POS = " << fixed << setprecision(3) << (*it)->GetPos().x << " " <<
			fixed << setprecision(3) << (*it)->GetPos().y << " " <<
			fixed << setprecision(3) << (*it)->GetPos().z << " " << endl;//�ʒu
		WritingFile << "ROT = " << fixed << setprecision(3) << (*it)->GetRot().x << " " <<
			fixed << setprecision(3) << (*it)->GetRot().y << " " <<
			fixed << setprecision(3) << (*it)->GetRot().z << " " << endl;//����
		WritingFile << "SCALE = " << fixed << setprecision(3) << (*it)->GetScale().x << " " <<
			fixed << setprecision(3) << (*it)->GetScale().y << " " <<
			fixed << setprecision(3) << (*it)->GetScale().z << " " << endl;//�g�嗦

		WritingFile << "END_SETNUM" << endl;

		nCnt++;//�J�E���g�A�b�v
	}

	WritingFile << "END_SETMOVEAI" << endl;

	CObjectXAlive::SaveInfoTxt(WritingFile);
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C������������[�h����
//====================================================================================
void CEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	//int nType = 0;//���
	//int nLife = 0;//�̗�
	//D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //�ړ���
	//D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ʒu
	//D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�g�嗦
	//D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //����
	//ENEMYTYPE Type = {};                               //�u���b�N�̎��
	//while (Buff != "END_SETBLOCK")
	//{
	//	LoadingFile >> Buff;//�P���ǂݍ���
	//	if (Buff == "#")
	//	{
	//		getline(LoadingFile, Buff);
	//	}
	//	else if (Buff == "TYPE")
	//	{
	//		LoadingFile >> Buff;//�C�R�[��
	//		LoadingFile >> nType;      //���
	//	}
	//	else if (Buff == "LIFE")
	//	{
	//		LoadingFile >> Buff;//�C�R�[��
	//		LoadingFile >> nLife;      //�̗�
	//	}
	//	else if (Buff == "MOVE")
	//	{
	//		LoadingFile >> Buff;//�C�R�[��
	//		LoadingFile >> Move.x;      //�ړ���X
	//		LoadingFile >> Move.y;      //�ړ���Y
	//		LoadingFile >> Move.z;      //�ړ���Z
	//	}
	//	else if (Buff == "POS")
	//	{
	//		LoadingFile >> Buff;//�C�R�[��
	//		LoadingFile >> Pos.x;      //�ʒuX
	//		LoadingFile >> Pos.y;      //�ʒuY
	//		LoadingFile >> Pos.z;      //�ʒuZ
	//	}
	//	else if (Buff == "ROT")
	//	{
	//		LoadingFile >> Buff;//�C�R�[��
	//		LoadingFile >> Rot.x;      //�ʒuX
	//		LoadingFile >> Rot.y;      //�ʒuY
	//		LoadingFile >> Rot.z;      //�ʒuZ
	//	}
	//	else if (Buff == "SCALE")
	//	{
	//		LoadingFile >> Buff;//�C�R�[��
	//		LoadingFile >> Scale.x;      //�g�嗦X
	//		LoadingFile >> Scale.y;      //�g�嗦Y
	//		LoadingFile >> Scale.z;      //�g�嗦Z
	//	}
	//}
	//Type = static_cast<ENEMYTYPE>(nType);

	//

	//listSaveManager.push_back(CEnemy::Create(Type, nLife, Pos, Rot, Scale));//vector�ɏ���ۑ�����

}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g����������[�h����
//====================================================================================
//CObject* CEnemy::ManagerChengeObject(bool bAim)
//{
//	int nNewType = static_cast<int>(m_Type);
//    ENEMYTYPE NewType = {};
//	//=======================================
//	//��ނ�ς���
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
//	//�ݒ肷��
//	//=======================================
//	NewType = ENEMYTYPE(nNewType);
//	//======================================================================================
//
//	//=======================================
//	//���S�t���O��ݒ肷��
//	//=======================================
//	SetUseDeath(true);
//	SetDeath();
//	//======================================================================================
//
//	return CEnemy::Create(NewType,GetLife,GetPos(), GetRot(), GetScale());//���������I�u�W�F�N�g��Ԃ�
//}
////============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�ɃZ�[�u����
//====================================================================================
CObject* CEnemy::ManagerSaveObject()
{
	return nullptr;
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[���瑀�삷��
//====================================================================================
void CEnemy::ManagerChooseControlInfo()
{
	SetMoveAiPoint();//�ړ�AI�̐ݒ���s��

	PhaseNumDecision();//�t�F�[�Y�ԍ��̌�����s��

	CObjectXAlive::ManagerChooseControlInfo();
}
//============================================================================================================================================

//====================================================================================
//�ړ�AI�̏���ݒ肷��
//====================================================================================
void CEnemy::SetVecMoveAiInfo(vector<CAIModel*>& vec)
{
	m_VecMoveAi = vec;
}
//============================================================================================================================================

//====================================================================================
//�����蔻��̏������s��
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

	bool bSuccessCollision = false;//�����蔻�肪�����������ǂ���
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = CObject::GetTopObject(nCntPri);

		while (pObj != nullptr)
		{
			//���̃I�u�W�F�N�g���i�[
			CObject* pNext = pObj->GetNextObject();

			//��ނ̎擾�i�G�Ȃ瓖���蔻��j
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

			//�I�u�W�F�N�g�����ɐi�߂�
			pObj = pNext;
		}

	}

	SetExtrusionCollisionSquareX(bCollisionX);
	SetExtrusionCollisionSquareY(bCollisionY);
	SetExtrusionCollisionSquareZ(bCollisionZ);

}
//============================================================================================================================================

//====================================================================================
//�ړ�AI�̔z�u������
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

	CManager::GetDebugProc()->PrintDebugProc("�ړ�AI�̐ݒu�ʒu�̈ړ��FIKLJ\n");

	if (bMove == true)
	{
		float fRot = atan2f(fX, fZ);
		m_MoveAiSavePos += D3DXVECTOR3(sinf(fRot) * 5.0f, 0.0f, cosf(fRot) * 5.0f);
	}

	CManager::GetDebugProc()->PrintDebugProc("�ړ�AI��ۑ��FO\n");

	CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 45, 30.0f, 30.0f, 100, 10, false, m_MoveAiSavePos + GetPos(), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);

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
//AI�ړ�����
//====================================================================================
void CEnemy::AIMoveProcess()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		float fLengthPlayer = CCalculation::CalculationLength(CGame::GetPlayer()->GetPos(), GetPos());
		if (fLengthPlayer < 300.0f)
		{
			ChengeMove(DBG_NEW CEnemyMove_Battle());
		}

		if (m_VecMoveAi.size() > 0)
		{
			auto it = m_VecMoveAi.begin();//�ŏ��̃|�C���^���擾

			advance(it, m_nIdxMoveAi);//�w�肵�Ă���ԍ��܂Ői�߂�

			float fLength = CCalculation::CalculationLength(GetPos(), (*it)->GetPos());//�����𑪂�
			float fRot = atan2f((*it)->GetPos().x - GetPos().x, (*it)->GetPos().z - GetPos().z);
			SetMove(D3DXVECTOR3(sinf(fRot) * 5.0f, GetMove().y, cosf(fRot) * 5.0f));

			if (fLength < 20.0f)
			{
				m_nIdxMoveAi++;//�ړI�n�����̈ʒu�ɕύX
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
//�o�g���ړ�����
//====================================================================================
void CEnemy::BattleMoveProcess()
{
	float fLengthPlayer = CCalculation::CalculationLength(CGame::GetPlayer()->GetPos(), GetPos());
	const D3DXVECTOR3 & PlayerPos = CGame::GetPlayer()->GetPos();
	const D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3 Aim = PlayerPos - Pos;
	D3DXVec3Normalize(&Aim, &Aim);
	float fRot = atan2f(Aim.x,Aim.z);

	SetMove(D3DXVECTOR3(sinf(fRot) * 5.0f, GetMove().y, cosf(fRot) * 5.0f));

	CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 1, 30, 20.0f, 20.0f, 100, 10, false, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
	if (fLengthPlayer > 600.0f)
	{
		ChengeMove(DBG_NEW CEnemyMove_AI());
	}

	RayCollision();

}
//============================================================================================================================================


//====================================================================================
//�ړ���Ԃ�ύX����
//====================================================================================
void CEnemy::ChengeMove(CEnemyMove* pEnemyMove)
{
	//�j������
	if (m_pEnemyMove != nullptr)
	{
		delete m_pEnemyMove;
		m_pEnemyMove = nullptr;
	}

	//�������
	m_pEnemyMove = pEnemyMove;
}
//============================================================================================================================================

//====================================================================================
//���C�������������ǂ����𔻒肷��
//====================================================================================
void CEnemy::RayCollision()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CObjectX* pObjX = nullptr;
		D3DXVECTOR3 Ray = D3DXVECTOR3(CGame::GetPlayer()->GetPos().x,0.0f,CGame::GetPlayer()->GetPos().z) - D3DXVECTOR3(GetPos().x,0.0f,GetPos().z);
		D3DXVec3Normalize(&Ray, &Ray);

		for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
		{
			CObject* pObj = GetTopObject(nCntPri);

			while (pObj != nullptr)
			{
				CObject* pNext = pObj->GetNextObject();
				D3DXVECTOR3 CollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				if (pObj->GetType() == CObject::TYPE::BGMODEL)
				{
					CBgModel* pBgModel = static_cast<CBgModel*>(pObj);
					float fLength = CCalculation::CalculationLength(GetPos(), pBgModel->GetPos());
					float r1 = GetSize().x / 2;
					float r2 = pBgModel->GetSize().x / 2;
					float r3 = r1 + r2;
					if (CCollision::RayIntersectsAABBCollisionPos(GetPos(), Ray, pBgModel->GetPos() + pBgModel->GetVtxMin(), pBgModel->GetPos() + pBgModel->GetVtxMax(),
						CollisionPos))
					{
						CManager::GetDebugProc()->PrintDebugProc("���C�����������I�u�W�F�N�g�Ƃ̋���:%f\n", fLength);
						CManager::GetDebugProc()->PrintDebugProc("���a�̘a�F%f\n", r3);
						if (fLength < r3 + 100.0f)
						{
							pObjX = pBgModel;
							CManager::GetDebugProc()->PrintDebugProc("�G�̃��C�����������I\n");
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
			{
				SetMove(D3DXVECTOR3(sinf(fRot - D3DX_PI * 0.5f) * 5.0f, GetMove().y, cosf(fRot - D3DX_PI * 0.5f) * 5.0f));
			}
			else if (fDot < 0.0f)
			{
				SetMove(D3DXVECTOR3(sinf(fRot + D3DX_PI * 0.5f) * 5.0f, GetMove().y, cosf(fRot + D3DX_PI * 0.5f) * 5.0f));
			}
		}
	}

	
}
//============================================================================================================================================


//====================================================================================
//�t�F�[�Y�ԍ������肷��
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
	CManager::GetDebugProc()->PrintDebugProc("�t�F�[�Y�ԍ��̕ύX�F%d\n", m_nPhaseNum);
}
//============================================================================================================================================

//********************************************************************************************************************************************
//�ˌ��Ɏア�G�N���X
//********************************************************************************************************************************************

//====================================================================================
//�ÓI�����o�ϐ��錾
//====================================================================================
const string CShotWeakEnemy::s_aSHOTWEAKENEMY_FILENAME[static_cast<int>(SHOTWEAKENEMYTYPE::MAX)] =
{
	"data\\MODEL\\Enemy\\ShotWeak\\noobSlime.x"
};
const int CShotWeakEnemy::s_nATTACK_FREQUENCY = 90;//�U���p�x
//============================================================================================================================================

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CShotWeakEnemy::CShotWeakEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CEnemy(nPri,bUseintPri,type,ObjType),
m_ShotWeakEnemyType(SHOTWEAKENEMYTYPE::NORMAL)
{

}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CShotWeakEnemy::~CShotWeakEnemy()
{

}
//============================================================================================================================================

//====================================================================================
//����������
//====================================================================================
HRESULT CShotWeakEnemy::Init()
{
	CEnemy::Init();

	SetEnemyType(CEnemy::ENEMYTYPE::SHOTWEAK);//�G�^�C�v��ݒ�
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//�I������
//====================================================================================
void CShotWeakEnemy::Uninit()
{
	CEnemy::Uninit();
}
//============================================================================================================================================

//====================================================================================
//�X�V����
//====================================================================================
void CShotWeakEnemy::Update()
{
	CEnemy::Update();

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (GetCntTime() % s_nATTACK_FREQUENCY == 0)
		{
			D3DXVECTOR3 Aim = CCalculation::Calculation3DVec(GetSenterPos(), CGame::GetPlayer()->GetSenterPos(), 20.0f);

			CAttackEnemy::Create(CAttack::ATTACKTYPE::EXPLOSION,CAttack::TARGETTYPE::PLAYER,CAttack::COLLISIONTYPE::SQUARE, 1, 60, 200, GetSenterPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), Aim, D3DXVECTOR3(2.0f, 2.0f, 2.0f));
		}
	}
}
//============================================================================================================================================

//====================================================================================
//�`�揈��
//====================================================================================
void CShotWeakEnemy::Draw()
{
	CEnemy::Draw();
}
//============================================================================================================================================

//====================================================================================
//���S�t���O�ݒ菈��
//====================================================================================
void CShotWeakEnemy::SetDeath()
{
	CEnemy::SetDeath();
}
//============================================================================================================================================

//====================================================================================
//��������
//====================================================================================
CShotWeakEnemy* CShotWeakEnemy::Create(SHOTWEAKENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CShotWeakEnemy* pShotWeakEnemy = DBG_NEW CShotWeakEnemy;

	pShotWeakEnemy->Init();            //����������
	int nIdx = CManager::GetObjectXInfo()->Regist(s_aSHOTWEAKENEMY_FILENAME[int(Type)]);
	pShotWeakEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pShotWeakEnemy->SetPhaseNum(nPhaseNum);//�t�F�[�Y�ԍ���ݒ肷��
	pShotWeakEnemy->m_ShotWeakEnemyType = Type;
	pShotWeakEnemy->SetLife(nLife);    //�̗�
	pShotWeakEnemy->SetMaxLife(nLife); //�ő�̗�
	pShotWeakEnemy->SetAutoSubLife(false);
	pShotWeakEnemy->SetPos(pos);       //�ʒu
	pShotWeakEnemy->SetSupportPos(pos);//�x�_�ʒu
	pShotWeakEnemy->SetRot(rot);       //����
	pShotWeakEnemy->SetScale(Scale);   //�g�嗦
	pShotWeakEnemy->SetFormarScale(Scale);//���̊g�嗦��ݒ�

	pShotWeakEnemy->SetSize();//���f���T�C�Y��ݒ�
	pShotWeakEnemy->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::SHOTWEAKENEMY);           //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
	return pShotWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���ɏ���ۑ�����
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
//�e�L�X�g�t�@�C���������ǂݍ���
//====================================================================================
void CShotWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	int nType = 0;                                     //���
	int nShotWeakEnemyType = 0;                        //�V���b�g�Ɏア�G�^�C�v   
	int nLife = 0;                                     //�̗�
	int nPhaseNum = 0;                                 //�t�F�[�Y�ԍ�
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //�ړ���
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ʒu
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�g�嗦
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //����
	SHOTWEAKENEMYTYPE ShotWeakEnemyType = {};          //�w�i���f���̎��
	ENEMYTYPE EnemyType = {};

	vector<CAIModel*> VecMoveAi = {};
	vector<MoveAiInfo> VecMoveAiInfo = {};

	int nCntMoveAi = 0;
	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	while (Buff != "END_SETSHOTWEAKENEMY")
	{
		LoadingFile >> Buff;//�P���ǂݍ���
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "SHOTWEAKENEMYTYPE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nShotWeakEnemyType;
		}
		else if (Buff == "ENEMYTYPE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nType;
		}
		else if (Buff == "LIFE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nLife;
		}
		else if (Buff == "POS")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Pos.x;      //�ʒuX
			LoadingFile >> Pos.y;      //�ʒuY
			LoadingFile >> Pos.z;      //�ʒuZ
		}
		else if (Buff == "ROT")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Rot.x;      //�ʒuX
			LoadingFile >> Rot.y;      //�ʒuY
			LoadingFile >> Rot.z;      //�ʒuZ
		}
		else if (Buff == "SCALE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Scale.x;      //�g�嗦X
			LoadingFile >> Scale.y;      //�g�嗦Y
			LoadingFile >> Scale.z;      //�g�嗦Z
		}
		else if (Buff == "PHASENUM")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nPhaseNum;//�t�F�[�Y�ԍ�
		}
		else if (Buff == "SETMOVEAI")
		{
			while (1)
			{
				LoadingFile >> Buff;
				if (Buff == "SETNUM")
				{
					LoadingFile >> Buff;//�C�R�[��
					LoadingFile >> nCntMoveAi;//�ԍ�
					while (1)
					{
						LoadingFile >> Buff;

						if (Buff == "POS")
						{
							LoadingFile >> Buff;//�C�R�[��
							LoadingFile >> MoveAiPos.x;      //�ʒuX
							LoadingFile >> MoveAiPos.y;      //�ʒuY
							LoadingFile >> MoveAiPos.z;      //�ʒuZ
						}
						else if (Buff == "ROT")
						{
							LoadingFile >> Buff;//�C�R�[��
							LoadingFile >> MoveAiRot.x;      //�ʒuX
							LoadingFile >> MoveAiRot.y;      //�ʒuY
							LoadingFile >> MoveAiRot.z;      //�ʒuZ
						}
						else if (Buff == "SCALE")
						{
							LoadingFile >> Buff;//�C�R�[��
							LoadingFile >> MoveAiScale.x;      //�g�嗦X
							LoadingFile >> MoveAiScale.y;      //�g�嗦Y
							LoadingFile >> MoveAiScale.z;      //�g�嗦Z
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
	ShotWeakEnemyType = static_cast<SHOTWEAKENEMYTYPE>(nShotWeakEnemyType);
	EnemyType = static_cast<ENEMYTYPE>(nType);
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		CShotWeakEnemy* pShotWeakEnemy = CShotWeakEnemy::Create(ShotWeakEnemyType,nLife,nPhaseNum,Pos,Rot,Scale);
		pShotWeakEnemy->SetVecMoveAiInfo(VecMoveAi);
		listSaveManager.push_back(pShotWeakEnemy);      //vector�ɏ���ۑ�����
	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType), nShotWeakEnemyType, nPhaseNum, VecMoveAiInfo);
	}

}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�̃I�u�W�F�N�g���`�F���W����
//====================================================================================
CObject* CShotWeakEnemy::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_ShotWeakEnemyType);
	SHOTWEAKENEMYTYPE NewType = {};
	//=======================================
	//��ނ�ς���
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
	//�ݒ肷��
	//=======================================
	NewType = SHOTWEAKENEMYTYPE(nNewType);
	//======================================================================================

	//=======================================
	//���S�t���O��ݒ肷��
	//=======================================
	SetUseDeath(true);
	SetDeath();
	//======================================================================================

	return CShotWeakEnemy::Create(NewType, GetLife(), GetPhaseNum(), GetPos(), GetRot(), GetScale());//���������I�u�W�F�N�g��Ԃ�
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�Ɍ��݂̃I�u�W�F�N�g��ۑ�����
//====================================================================================
CObject* CShotWeakEnemy::ManagerSaveObject()
{
	auto& Vec = GetVecAiModelInfo();
	auto Vec2 = move(Vec);
	CShotWeakEnemy * pShotWeakEnemy = CShotWeakEnemy::Create(m_ShotWeakEnemyType, GetLife(),GetPhaseNum(),GetPos(), GetRot(), GetScale());//���������I�u�W�F�N�g��Ԃ�
	pShotWeakEnemy->SetVecMoveAiInfo(Vec2);
	return pShotWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//�o�g���ړ�����
//====================================================================================
void CShotWeakEnemy::BattleMoveProcess()
{
	CEnemy::BattleMoveProcess();
}
//============================================================================================================================================

//********************************************************************************************************************************************
//�_�C�u�Ɏア�G�N���X
//********************************************************************************************************************************************

//====================================================================================
//�ÓI�����o�ϐ��錾
//====================================================================================
const string CDiveWeakEnemy::s_aDIVEWEAKENEMY_FILENAME[static_cast<int>(CDiveWeakEnemy::DIVEWEAKENEMYTYPE::MAX)] =
{
	"data\\MODEL\\Enemy\\DiveWeak\\angrySlime.x"
};

//====================================================================================
//�ÓI�����o�ϐ��錾
//====================================================================================

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CDiveWeakEnemy::CDiveWeakEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CEnemy(nPri, bUseintPri, type, ObjType),
m_DiveWeakEnemyType(DIVEWEAKENEMYTYPE::NORMAL)
{

}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CDiveWeakEnemy::~CDiveWeakEnemy()
{

}
//============================================================================================================================================

//====================================================================================
//����������
//====================================================================================
HRESULT CDiveWeakEnemy::Init()
{
	CEnemy::Init();

	SetEnemyType(CEnemy::ENEMYTYPE::DIVEWEAK);//�G�^�C�v��ݒ�
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//�I������
//====================================================================================
void CDiveWeakEnemy::Uninit()
{
	CEnemy::Uninit();
}
//============================================================================================================================================

//====================================================================================
//�X�V����
//====================================================================================
void CDiveWeakEnemy::Update()
{
	CEnemy::Update();
}
//============================================================================================================================================

//====================================================================================
//�`�揈��
//====================================================================================
void CDiveWeakEnemy::Draw()
{
	CEnemy::Draw();
}
//============================================================================================================================================

//====================================================================================
//���S�t���O�ݒ菈��
//====================================================================================
void CDiveWeakEnemy::SetDeath()
{
	CEnemy::SetDeath();
}
//============================================================================================================================================

//====================================================================================
//��������
//====================================================================================
CDiveWeakEnemy* CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CDiveWeakEnemy* pDiveWeakEnemy = DBG_NEW CDiveWeakEnemy;

	pDiveWeakEnemy->Init();            //����������
	int nIdx = CManager::GetObjectXInfo()->Regist(s_aDIVEWEAKENEMY_FILENAME[int(Type)]);
	pDiveWeakEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pDiveWeakEnemy->SetPhaseNum(nPhaseNum);//�t�F�[�Y�ԍ���ݒ肷��
	pDiveWeakEnemy->m_DiveWeakEnemyType = Type;
	pDiveWeakEnemy->SetLife(nLife);    //�̗�
	pDiveWeakEnemy->SetMaxLife(nLife); //�ő�̗�
	pDiveWeakEnemy->SetPos(pos);       //�ʒu
	pDiveWeakEnemy->SetSupportPos(pos);//�x�_�ʒu
	pDiveWeakEnemy->SetRot(rot);       //����
	pDiveWeakEnemy->SetScale(Scale);   //�g�嗦
	pDiveWeakEnemy->SetFormarScale(Scale);//���̊g�嗦��ݒ�

	pDiveWeakEnemy->SetSize();//���f���T�C�Y��ݒ�
	pDiveWeakEnemy->SetAutoSubLife(false);//�����I�ɑ̗͂����炷���ǂ���
	pDiveWeakEnemy->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::DIVEWEAKENEMY);           //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
	return pDiveWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���ɏ���ۑ�����
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
//�e�L�X�g�t�@�C���������ǂݍ���
//====================================================================================
void CDiveWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	int nType = 0;                                     //���
	int nDiveWeakEnemyType = 0;                        //�_�C�u�Ɏア�G�^�C�v   
	int nLife = 0;                                     //�̗�
	int nPhaseNum = 0;                                 //�t�F�[�Y�ԍ�
 	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //�ړ���
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ʒu
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�g�嗦
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //����
    DIVEWEAKENEMYTYPE DiveWeakEnemyType = {};          //�w�i���f���̎��
	ENEMYTYPE EnemyType = {};

	vector<CAIModel*> VecMoveAi = {};
	vector<MoveAiInfo> VecMoveAiInfo = {};

	int nCntMoveAi = 0;
	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f,0.0f,0.0f);
	while (Buff != "END_SETDIVEWEAKENEMY")
	{
		LoadingFile >> Buff;//�P���ǂݍ���
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "DIVEWEAKENEMYTYPE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nDiveWeakEnemyType;
		}
		else if (Buff == "ENEMYTYPE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nType;
		}
		else if (Buff == "LIFE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nLife;
		}
		else if (Buff == "POS")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Pos.x;      //�ʒuX
			LoadingFile >> Pos.y;      //�ʒuY
			LoadingFile >> Pos.z;      //�ʒuZ
		}
		else if (Buff == "ROT")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Rot.x;      //�ʒuX
			LoadingFile >> Rot.y;      //�ʒuY
			LoadingFile >> Rot.z;      //�ʒuZ
		}
		else if (Buff == "SCALE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Scale.x;      //�g�嗦X
			LoadingFile >> Scale.y;      //�g�嗦Y
			LoadingFile >> Scale.z;      //�g�嗦Z
		}
		else if (Buff == "PHASENUM")
		{
			LoadingFile >> Buff;      //�C�R�[��
			LoadingFile >> nPhaseNum; //�t�F�[�Y�ԍ�
		}
		else if (Buff == "SETMOVEAI")
		{
			while (1)
			{
				LoadingFile >> Buff;
				if (Buff == "SETNUM")
				{
					LoadingFile >> Buff;//�C�R�[��
					LoadingFile >> nCntMoveAi;//�ԍ�
					while (1)
					{
						LoadingFile >> Buff;

						if (Buff == "POS")
						{
							LoadingFile >> Buff;//�C�R�[��
							LoadingFile >> MoveAiPos.x;      //�ʒuX
							LoadingFile >> MoveAiPos.y;      //�ʒuY
							LoadingFile >> MoveAiPos.z;      //�ʒuZ
						}
						else if (Buff == "ROT")
						{
							LoadingFile >> Buff;//�C�R�[��
							LoadingFile >> MoveAiRot.x;      //�ʒuX
							LoadingFile >> MoveAiRot.y;      //�ʒuY
							LoadingFile >> MoveAiRot.z;      //�ʒuZ
						}
						else if (Buff == "SCALE")
						{
							LoadingFile >> Buff;//�C�R�[��
							LoadingFile >> MoveAiScale.x;      //�g�嗦X
							LoadingFile >> MoveAiScale.y;      //�g�嗦Y
							LoadingFile >> MoveAiScale.z;      //�g�嗦Z
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
		CDiveWeakEnemy* pDiveWeakEnemy = CDiveWeakEnemy::Create(DiveWeakEnemyType, nLife, nPhaseNum, Pos, Rot, Scale);
		pDiveWeakEnemy->SetUseDraw(true);
		pDiveWeakEnemy->SetUseShadow(true);
		pDiveWeakEnemy->SetVecMoveAiInfo(VecMoveAi);
		listSaveManager.push_back(pDiveWeakEnemy);      //vector�ɏ���ۑ�����
	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType), nDiveWeakEnemyType, nPhaseNum,VecMoveAiInfo);
	}
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�̃I�u�W�F�N�g���`�F���W����
//====================================================================================
CObject* CDiveWeakEnemy::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_DiveWeakEnemyType);
	DIVEWEAKENEMYTYPE NewType = {};
	//=======================================
	//��ނ�ς���
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
	//�ݒ肷��
	//=======================================
	NewType = static_cast<DIVEWEAKENEMYTYPE>(nNewType);
	//======================================================================================

	//=======================================
	//���S�t���O��ݒ肷��
	//=======================================
	SetUseDeath(true);
	SetDeath();
	//======================================================================================

	return CDiveWeakEnemy::Create(NewType, GetLife(),GetPhaseNum(),GetPos(), GetRot(), GetScale());//���������I�u�W�F�N�g��Ԃ�
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�Ɍ��݂̃I�u�W�F�N�g��ۑ�����
//====================================================================================
CObject* CDiveWeakEnemy::ManagerSaveObject()
{
	auto& Vec = GetVecAiModelInfo();
	auto Vec2 = move(Vec);
	CDiveWeakEnemy * pDiveWeakEnemy = CDiveWeakEnemy::Create(m_DiveWeakEnemyType, GetLife(),GetPhaseNum(),GetPos(), GetRot(), GetScale());//���������I�u�W�F�N�g��Ԃ�
	pDiveWeakEnemy->SetVecMoveAiInfo(Vec2);
	return pDiveWeakEnemy;//���������I�u�W�F�N�g��Ԃ�
}
//============================================================================================================================================

//====================================================================================
//�o�g���ړ�����
//====================================================================================
void CDiveWeakEnemy::BattleMoveProcess()
{
	CEnemy::BattleMoveProcess();
}
//============================================================================================================================================

//********************************************************************************************************************************************
//�G�ړ��^�C�v�N���X
//********************************************************************************************************************************************

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CEnemyMove::CEnemyMove()
{

}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CEnemyMove::~CEnemyMove()
{

}
//============================================================================================================================================

//====================================================================================
//����
//====================================================================================
void CEnemyMove::Process(CEnemy* pEnemy)
{

}
//============================================================================================================================================

//********************************************************************************************************************************************
//�G�ړ��^�C�v�FAI�N���X
//********************************************************************************************************************************************

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CEnemyMove_AI::CEnemyMove_AI()
{

}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CEnemyMove_AI::~CEnemyMove_AI()
{

}
//============================================================================================================================================

//====================================================================================
//����
//====================================================================================
void CEnemyMove_AI::Process(CEnemy* pEnemy)
{
	pEnemy->AIMoveProcess();//AI�ړ��������s��
}
//============================================================================================================================================

//********************************************************************************************************************************************
//�G�ړ��^�C�v�F�o�g���N���X
//********************************************************************************************************************************************

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CEnemyMove_Battle::CEnemyMove_Battle()
{

}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CEnemyMove_Battle::~CEnemyMove_Battle()
{

}
//============================================================================================================================================

//====================================================================================
//����
//====================================================================================
void CEnemyMove_Battle::Process(CEnemy* pEnemy)
{
	pEnemy->BattleMoveProcess();
}
//============================================================================================================================================