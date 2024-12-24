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
#include "damage.h"
#include "particle.h"
#include "phasemanager.h"
#include "input.h"
#include "camera.h"
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
m_Type(ENEMYTYPE::SHOTWEAK), m_VecMoveAi(), m_MoveAiSavePos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),m_nCntTime(0),m_nIdxMoveAi(0), m_nPhaseNum(0),m_pEnemyMove(DBG_NEW CEnemyMove_AI()),
m_fRotMove(0.0f),m_fSensingRange(0.0f),m_fNormalSpeed(0.0f),m_Pattern(),m_bCollisoinDetection(true),m_bActivateCollisionDetection(false),m_bCollisionWall(false),
m_DefeatAttackType(CAttack::ATTACKTYPE::EXPLOSION)
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
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		const D3DXVECTOR3& Rot = GetRot();
		const D3DXVECTOR3& Pos = GetPos();
		const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPos();
		//float fElevaRot = atan2f(PlayerPos.y - Pos.y, sqrtf(powf(PlayerPos.x - Pos.x, 2) + powf(PlayerPos.z - Pos.z, 2)));
		//SetRot(D3DXVECTOR3(fElevaRot, atan2f(PlayerPos.x - Pos.x, PlayerPos.z - Pos.z) + D3DX_PI,0.0f));

		float fLength = CCalculation::CalculationLength(Pos, PlayerPos);
		m_pEnemyMove->Process(this);

		m_nCntTime++;//���Ԃ��J�E���g����

		AttackProcess();//�U������

		CollisionDetectionProcess();

		CObjectXAlive::Update();

	    CollisionProcess();//�����蔻�菈��

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

	WritingFile << "NORMALSPEED = " << m_fNormalSpeed << endl;
	WritingFile << "SENSINGRANGE = " << m_fSensingRange << endl;

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
	//�X�e�[�W�}�l�[�W���[�ɑI������Ă��鎞�����\������
	for (auto it : m_VecMoveAi)
	{
		it->SetUseDraw(true);
	}

	SetMoveAiPoint();//�ړ�AI�̐ݒ���s��

	PhaseNumDecision();//�t�F�[�Y�ԍ��̌�����s��

	EditNormalSpeed();//�ʏ�ړ����x��ҏW

	EditSensingRange();//���G�͈͂�ҏW
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
	bool bIsLanding = false;

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

			//�I�u�W�F�N�g�����ɐi�߂�
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
//�ړ�AI�̔z�u������
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
	CManager::GetDebugProc()->PrintDebugProc("�ړ�AI�̐ݒu�ʒu�̈ړ��FIKLJ\n");

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

	CManager::GetDebugProc()->PrintDebugProc("�ړ�AI�̈ʒu�F%f %f %f\n", NowPos.x,NowPos.y,NowPos.z);

	CManager::GetDebugProc()->PrintDebugProc("�ړ�AI��ۑ��FO\n");

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
		if (fLengthPlayer < m_fSensingRange)
		{
			ChengeMove(DBG_NEW CEnemyMove_Battle());
		}

		if (m_VecMoveAi.size() > 0)
		{
			auto it = m_VecMoveAi.begin();//�ŏ��̃|�C���^���擾

			advance(it, m_nIdxMoveAi);//�w�肵�Ă���ԍ��܂Ői�߂�

			float fLength = CCalculation::CalculationLength(GetPos(), (*it)->GetPos());//�����𑪂�
			float fRot = atan2f((*it)->GetPos().x - GetPos().x, (*it)->GetPos().z - GetPos().z);
			SetMove(D3DXVECTOR3(sinf(fRot) * m_fNormalSpeed, GetMove().y, cosf(fRot) * m_fNormalSpeed));

			if (fLength < 30.0f)
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
		D3DXVECTOR3 Ray = CCalculation::RadToVec(D3DXVECTOR3(GetRot().x,GetRot().y,0.0f) - D3DXVECTOR3(D3DX_PI * 0.5f,0.0f,0.0f));//��O���Ƀ��C���΂�
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
							//�G�̃I�u�W�F�N�g�̃T�C�Y�̏������苗�����������Ȃ���
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
			{//�x�N�g���ɑ΂��ăv���C���[���E���ɂ�����
				//SetMove(D3DXVECTOR3(sinf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed, GetMove().y, cosf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed));
				ChengeMove(DBG_NEW CEnemyMove_DodgeWall(this, D3DXVECTOR3(sinf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed, GetMove().y, cosf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed)));
			}
			else if (fDot < 0.0f)
			{//�x�N�g���ɑ΂��ăv���C���[�������ɂ�����
				ChengeMove(DBG_NEW CEnemyMove_DodgeWall(this, D3DXVECTOR3(sinf(fRot + D3DX_PI * 0.5f) * m_fNormalSpeed, GetMove().y, cosf(fRot + D3DX_PI * 0.5f) * m_fNormalSpeed)));
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


//====================================================================================
//�U������
//====================================================================================
void CEnemy::AttackProcess()
{

}
//============================================================================================================================================

//====================================================================================
//�Փ˔��菈��
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
						D3DXVECTOR3(GetPos().x, 0.0f, GetPos().z);//XZ���������̃x�N�g�������
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
						SetMove(-AimVec * (fTotalLength - fLength) + GetMove());//�U�����̓��������D��I�ɂ��̈ړ��ʂ����蓖�Ă�
						break;
					}
				}
			}

			pObj = pNext;
		}
	}
}

//====================================================================================
//�ʏ�ړ����x��ҏW
//====================================================================================
void CEnemy::EditNormalSpeed()
{
	CInputKeyboard * pInput = CManager::GetInputKeyboard();

	if (pInput->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
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
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
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

	CManager::GetDebugProc()->PrintDebugProc("�ʏ�ړ����x�ύX�i�T�j�F%f\n",m_fNormalSpeed);
}
//============================================================================================================================================

//====================================================================================
//�ʏ�ړ����x��ҏW
//====================================================================================
void CEnemy::EditSensingRange()
{
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	if (pInput->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
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
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
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

	CManager::GetDebugProc()->PrintDebugProc("���G�͈͕ύX�i�U�j�F%f\n", m_fSensingRange);
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
const float CShotWeakEnemy::s_fATTACKSTART_LENGTH = 450.0f;//�U���J�n����
const float CShotWeakEnemy::s_fNORMAL_SENSIINGRANGE = 1050.0f;//�ʏ튴�m�˒�
const int CShotWeakEnemy::s_nATTACK_COOLTIME = 60;//�U���̃N�[���^�C��
//============================================================================================================================================

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CShotWeakEnemy::CShotWeakEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CEnemy(nPri,bUseintPri,type,ObjType),
m_ShotWeakEnemyType(SHOTWEAKENEMYTYPE::NORMAL),m_pMagicSword(nullptr), m_SaveAimPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_nAttackCoolTime(0)
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

	float fRatioRot = static_cast<float>(rand() % 200 - 100) / 100;
	float fRotSpeed = static_cast<float>(rand() % 40 - 20) / 100;
	m_pMagicSword = CAttackEnemy::Create(CAttack::ATTACKTYPE::MAGICSWORD, CAttack::TARGETTYPE::PLAYER, CAttack::COLLISIONTYPE::RECTANGLE_XZ,
		false,false,1, 60, 200, GetPos(), D3DXVECTOR3(0.0f,D3DX_PI * fRatioRot, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pMagicSword->SetUseDeath(false);
	m_pMagicSword->SetUseAddRot(true,D3DXVECTOR3(0.0f,fRotSpeed, 0.0f));
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
		m_pMagicSword->SetPos(GetPos());

		SwordCollision();//���̓����蔻����s��
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
	pShotWeakEnemy->SetSensingRange(1250.0f);//���m�˒�
	pShotWeakEnemy->SetNormalSpeed(10.0f);//�ʏ�ړ����x
	pShotWeakEnemy->SetUseInteria(false, GetNormalInertia());
	pShotWeakEnemy->m_pMagicSword->SetScale(Scale * 0.5f);

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
	int nDivisionNum = 0;                              //�����
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

	float fNormalSpeed = 0.0f; //�ʏ푬�x
	float fSensingRange = 0.0f;//���G����

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
		else if (Buff == "NORMALSPEED")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> fNormalSpeed;//�ʏ푬�x
		}
		else if (Buff == "SENSINGRANGE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> fSensingRange;
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
		listSaveManager.push_back(pShotWeakEnemy);      //vector�ɏ���ۑ�����

	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType), nShotWeakEnemyType, nPhaseNum,fNormalSpeed,fSensingRange,0,VecMoveAiInfo);
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

	return CShotWeakEnemy::Create(NewType, GetMaxLife(), GetPhaseNum(), GetPos(), GetRot(), GetScale());//���������I�u�W�F�N�g��Ԃ�
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�Ɍ��݂̃I�u�W�F�N�g��ۑ�����
//====================================================================================
CObject* CShotWeakEnemy::ManagerSaveObject()
{
	auto& Vec = GetVecAiModelInfo();
	auto Vec2 = move(Vec);
	CShotWeakEnemy * pShotWeakEnemy = CShotWeakEnemy::Create(m_ShotWeakEnemyType, GetMaxLife(),GetPhaseNum(),GetPos(), GetRot(), GetScale());//���������I�u�W�F�N�g��Ԃ�
	pShotWeakEnemy->SetVecMoveAiInfo(Vec2);
	pShotWeakEnemy->SetNormalSpeed(GetNormalSpeed());//���݂̓G�̒ʏ푬�x��ۑ�����
	pShotWeakEnemy->SetSensingRange(GetSensingRange());//���݂̓G�̍��G�͈͂�ۑ�����
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

//====================================================================================
//�U������
//====================================================================================
void CShotWeakEnemy::AttackProcess()
{
	float fLength = CCalculation::CalculationLength(GetPos(), CGame::GetPlayer()->GetPos());
	const bool& bAction = GetAction();
	const int& nPatternTime = GetPatternTime();
	const int& nPattern = GetPattern();
	m_nAttackCoolTime++;//�N�[���^�C�����J�E���g����
	if (fLength < s_fATTACKSTART_LENGTH && bAction == false && m_nAttackCoolTime > s_nATTACK_COOLTIME)
	{//�U�����J�n����Ă��Ȃ����
		ChengeMove(DBG_NEW CEnemyMove_None());//AI�ړ��ƍU�����������ւ���
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
//���̓����蔻����s��
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
//�_�C�u�Ɏア�G�N���X
//********************************************************************************************************************************************

//====================================================================================
//�ÓI�����o�ϐ��錾
//====================================================================================
const string CDiveWeakEnemy::s_aDIVEWEAKENEMY_FILENAME[static_cast<int>(CDiveWeakEnemy::DIVEWEAKENEMYTYPE::MAX)] =
{
	"data\\MODEL\\Enemy\\DiveWeak\\angrySlime.x"
};
const int CDiveWeakEnemy::s_nATTACK_FREQUENCY = 105;//�U���p�x
const float CDiveWeakEnemy::s_fSENSINGRANGE = 600.0f;
const float CDiveWeakEnemy::s_fNORMAL_SPEED = 3.0f;
//====================================================================================
//�R���X�g���N�^
//====================================================================================
CDiveWeakEnemy::CDiveWeakEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CEnemy(nPri, bUseintPri, type, ObjType),
m_DiveWeakEnemyType(DIVEWEAKENEMYTYPE::NORMAL),m_nDivisionNum(0)
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
//��������
//====================================================================================
CDiveWeakEnemy* CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale, int nDivisionNum)
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
	pDiveWeakEnemy->SetSensingRange(550.0f);//���m�˒�
	pDiveWeakEnemy->SetNormalSpeed(s_fNORMAL_SPEED);//�ʏ�ړ����x
	pDiveWeakEnemy->SetUseInteria(false, GetNormalInertia());
	pDiveWeakEnemy->SetCntTime(rand() % 100 + 1);
	pDiveWeakEnemy->SetDivisionNum(nDivisionNum);

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

	WritingFile << "DIVISIONNUM = " << m_nDivisionNum << endl;

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
	int nDivisionNum = 0;                              //�����
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

	float fNormalSpeed = 0.0f; //�ʏ푬�x
	float fSensingRange = 0.0f;//���G����
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
		else if (Buff == "NORMALSPEED")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> fNormalSpeed;//�ʏ푬�x
		}
		else if (Buff == "SENSINGRANGE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> fSensingRange;
		}
		else if (Buff == "DIVISIONNUM")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nDivisionNum;//�����
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
		CDiveWeakEnemy* pDiveWeakEnemy = CDiveWeakEnemy::Create(DiveWeakEnemyType, nLife, nPhaseNum, Pos, Rot, Scale,3);
		pDiveWeakEnemy->SetUseDraw(true);
		pDiveWeakEnemy->SetUseShadow(true);
		pDiveWeakEnemy->SetVecMoveAiInfo(VecMoveAi);
		pDiveWeakEnemy->SetNormalSpeed(fNormalSpeed);
		pDiveWeakEnemy->SetSensingRange(fSensingRange);
		listSaveManager.push_back(pDiveWeakEnemy);      //vector�ɏ���ۑ�����
	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType), nDiveWeakEnemyType, nPhaseNum,fNormalSpeed,fSensingRange,3,VecMoveAiInfo);
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

	return CDiveWeakEnemy::Create(NewType, GetLife(),GetPhaseNum(),GetPos(), GetRot(), GetScale(),GetDivisionNum());//���������I�u�W�F�N�g��Ԃ�
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�Ɍ��݂̃I�u�W�F�N�g��ۑ�����
//====================================================================================
CObject* CDiveWeakEnemy::ManagerSaveObject()
{
	auto& Vec = GetVecAiModelInfo();
	auto Vec2 = move(Vec);
	CDiveWeakEnemy * pDiveWeakEnemy = CDiveWeakEnemy::Create(m_DiveWeakEnemyType, GetMaxLife(),GetPhaseNum(),GetPos(), GetRot(), GetScale(),GetDivisionNum());//���������I�u�W�F�N�g��Ԃ�
	pDiveWeakEnemy->SetSensingRange(GetSensingRange());//���݂̓G�̍��G�͈͂�ۑ�����
	pDiveWeakEnemy->SetNormalSpeed(GetNormalSpeed());//���݂̓G�̒ʏ푬�x��ۑ�����
	pDiveWeakEnemy->SetVecMoveAiInfo(Vec2);
	return pDiveWeakEnemy;//���������I�u�W�F�N�g��Ԃ�
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[���瑀�삷��
//====================================================================================
void CDiveWeakEnemy::ManagerChooseControlInfo()
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
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
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
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

	CManager::GetDebugProc()->PrintDebugProc("�����(U)�F%d\n", m_nDivisionNum);

	CEnemy::ManagerChooseControlInfo();
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

//====================================================================================
//�U������
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
//AI�ړ�����
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

//********************************************************************************************************************************************
//�G�ړ��^�C�v�F�Ȃ��N���X
//********************************************************************************************************************************************

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CEnemyMove_None::CEnemyMove_None()
{

}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CEnemyMove_None::~CEnemyMove_None()
{

}
//============================================================================================================================================

//====================================================================================
//����
//====================================================================================
void CEnemyMove_None::Process(CEnemy* pEnemy)
{

}
//============================================================================================================================================

//************************************************************************************
//�����ړ��N���X
//************************************************************************************

//====================================================================================
//�R���X�g���N�^
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
//�f�X�g���N�^
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
//����
//====================================================================================
void CEnemyMove_Frightened::Process(CEnemy* pEnemy)
{
	float fX = static_cast<float>(rand() % 10 - 5);
	float fY = static_cast<float>(rand() % 10 - 5);
	float fZ = static_cast<float>(rand() % 10 - 5);
	pEnemy->SetPos(m_StopPos + D3DXVECTOR3(fX, fY, fZ));//�k��������
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
		pEnemy->ChengeMove(DBG_NEW CEnemyMove_AI());//AI�ړ������ɖ߂�
	}
}
//============================================================================================================================================

//************************************************************************************
//�ǉ���ړ��N���X
//************************************************************************************

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CEnemyMove_DodgeWall::CEnemyMove_DodgeWall(CEnemy* pEnemy, D3DXVECTOR3 DodgeMove)
{
	m_DodgeMove = DodgeMove;
}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CEnemyMove_DodgeWall::~CEnemyMove_DodgeWall()
{

}
//============================================================================================================================================

//====================================================================================
//����
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