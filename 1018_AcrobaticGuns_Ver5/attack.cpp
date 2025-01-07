//======================================================================================================================
//
//�P�O���Q�S���F�U���p�̃N���X�����[attack.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//==================================================================
//�C���N���[�h
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
//�O���錾
//==================================================================
const string CAttack::ATTACK_FILENAME[static_cast<int>(CAttack::ATTACKTYPE::MAX)] =
{
	"data\\MODEL\\Attack\\Fire_000.x",
	"data\\MODEL\\Attack\\Explosion_000.x",
	"data\\MODEL\\Attack\\MagicSword_000.x",
};
//======================================================================================================================

//==================================================================
//�R���X�g���N�^
//==================================================================
CAttack::CAttack(int nPower, int nSetHitStopTime, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectX(nPri, bUseintPri, type, ObjType),
m_Type(ATTACKTYPE::BULLET), m_nPower(nPower), m_HitStop({0,nSetHitStopTime}),m_bCollisionRelease(true),m_CollisionType(CAttack::COLLISIONTYPE::NONE),
m_TargetType(CAttack::TARGETTYPE::NONE),m_bHitOtherThanLiving(false),m_bAutoCollision(true),m_bCollisionSuccess(false)
{

}
//======================================================================================================================

//==================================================================
//�f�X�g���N�^
//==================================================================
CAttack::~CAttack()
{

}
//======================================================================================================================

//==================================================================
//����������
//==================================================================
HRESULT CAttack::Init()
{
	CObjectX::Init();
	return S_OK;
}
//======================================================================================================================

//==================================================================
//�I������
//==================================================================
void CAttack::Uninit()
{
	CObjectX::Uninit();
}
//======================================================================================================================

//==================================================================
//�X�V����
//==================================================================
void CAttack::Update()
{
	CObjectX::Update();

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
//�`�揈��
//==================================================================
void CAttack::Draw()
{
	CObjectX::Draw();
}
//======================================================================================================================

//==================================================================
//���S�t���O�ݒ菈��
//==================================================================
void CAttack::SetDeath()
{
	CObject::SetDeath();
}
//======================================================================================================================

//==================================================================
//�����蔻�菈��
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
				if (pEnemy->GetLifeInfo().GetLife() < 1)
				{
					pEnemy->SetDefeatAttack(m_Type);
				}
			}
			else if (pObj->GetType() == CObject::TYPE::PLAYER && m_TargetType == TARGETTYPE::PLAYER)
			{
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);
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
//�����蔻�苤�ʏ���
//==================================================================
void CAttack::CollisionProcess(bool& bCollision, bool& bNowCollision, CObjectX* pObjX)
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

//=============================================================================�v���C���[�U���N���X=============================================================================

//==================================================================
//�R���X�g���N�^
//==================================================================
CAttackPlayer::CAttackPlayer(int nPower, int nSetHitStopTime) : CAttack(nPower,nSetHitStopTime)
{

}
//======================================================================================================================

//==================================================================
//�f�X�g���N�^
//==================================================================
CAttackPlayer::~CAttackPlayer()
{

}
//======================================================================================================================

//==================================================================
//����������
//==================================================================
HRESULT CAttackPlayer::Init()
{
	CAttack::Init();
	return S_OK;
}
//======================================================================================================================

//==================================================================
//�I������
//==================================================================
void CAttackPlayer::Uninit()
{
	CAttack::Uninit();
}
//======================================================================================================================

//==================================================================
//�X�V����
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
//�`�揈��
//==================================================================
void CAttackPlayer::Draw()
{
	CAttack::Draw();
}
//======================================================================================================================

//==================================================================
//���S�t���O�ݒ菈��
//==================================================================
void CAttackPlayer::SetDeath()
{
	CAttack::SetDeath();
}
//======================================================================================================================

//==================================================================
//��������
//==================================================================
CAttackPlayer* CAttackPlayer::Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,
	bool bHitOtherThanLiving, bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
	CAttackPlayer* pAttackPlayer = nullptr;       //����
	pAttackPlayer = DBG_NEW CAttackPlayer(nPower,nSetHitStopTime);//����
	pAttackPlayer->Init();                        //����������
	pAttackPlayer->SetTargetType(TargetType);     //�^�[�Q�b�g�^�C�v��ݒ�
	pAttackPlayer->SetCollisionType(CollisionType);//����^�C�v��ݒ�
	pAttackPlayer->SetType(CObject::TYPE::ATTACK);//�I�u�W�F�N�g���Ƃ̃^�C�v��ݒ肷��
	pAttackPlayer->SetAttackType(AttackType);     //�U���̎�ނ�ݒ肷��
	pAttackPlayer->GetLifeInfo().SetLife(nLife);                //�̗͂�ݒ�
	pAttackPlayer->GetLifeInfo().SetMaxLife(nLife);             //�ő�̗͂�ݒ�
	pAttackPlayer->GetPosInfo().SetPos(pos);                   //�ʒu  
	pAttackPlayer->GetPosInfo().SetSupportPos(pos);            //�x�_�ʒu
	pAttackPlayer->GetRotInfo().SetRot(rot);                   //����
	pAttackPlayer->GetMoveInfo().SetMove(move);                 //�ړ���
	pAttackPlayer->GetSizeInfo().SetScale(Scale);               //�g�嗦
	pAttackPlayer->GetLifeInfo().SetAutoSubLife(true);          //�̗͂��g�p����
	pAttackPlayer->GetLifeInfo().SetAutoDeath(true);
	pAttackPlayer->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia());
	pAttackPlayer->GetMoveInfo().SetUseGravity(false,1.0f);
	pAttackPlayer->SetHitOtherThanLibing(bHitOtherThanLiving);
	pAttackPlayer->SetAutoCollision(bAutoCollision);//�U���̓����蔻����U���N���X�ɔC���邩�ǂ���
	//���f�����ݒ�
	int nIdx = CManager::GetObjectXInfo()->Regist(ATTACK_FILENAME[static_cast<int>(AttackType)]);

	//���f���������蓖�Ă�
	pAttackPlayer->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pAttackPlayer->SetSize();//�T�C�Y��ݒ肷��
	return pAttackPlayer;
}
//======================================================================================================================

//=============================================================================�G�U���N���X=============================================================================

//==================================================================
//�R���X�g���N�^
//==================================================================
CAttackEnemy::CAttackEnemy(int nPower, int nSetHitStopTime) : CAttack(nPower,nSetHitStopTime)
{

}
//======================================================================================================================

//==================================================================
//�f�X�g���N�^
//==================================================================
CAttackEnemy::~CAttackEnemy()
{

}
//======================================================================================================================

//==================================================================
//����������
//==================================================================
HRESULT CAttackEnemy::Init()
{
	CAttack::Init();
	return S_OK;
}
//======================================================================================================================

//==================================================================
//�I������
//==================================================================
void CAttackEnemy::Uninit()
{
	CAttack::Uninit();
}
//======================================================================================================================

//==================================================================
//�X�V����
//==================================================================
void CAttackEnemy::Update()
{
	CAttack::Update();
}
//======================================================================================================================

//==================================================================
//�`�揈��
//==================================================================
void CAttackEnemy::Draw()
{
	CAttack::Draw();
}
//======================================================================================================================

//==================================================================
//���S�t���O�ݒ菈��
//==================================================================
void CAttackEnemy::SetDeath()
{
	CAttack::SetDeath();
}
//======================================================================================================================

//==================================================================
//�G�̍U���𐶐�
//==================================================================
CAttackEnemy* CAttackEnemy::Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,
	bool bHitOtherThanLiving, bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
	CAttackEnemy* pAttackEnemy = DBG_NEW CAttackEnemy(nPower, nSetHitStopTime);

	pAttackEnemy->Init();                        //����������
	pAttackEnemy->SetType(CObject::TYPE::ATTACK); //�I�u�W�F�N�g���Ƃ̃^�C�v��ݒ肷��
	pAttackEnemy->SetAttackType(AttackType);     //�U���̎�ނ�ݒ肷��
	pAttackEnemy->SetTargetType(TargetType);     //�^�[�Q�b�g�^�C�v��ݒ肷��
	pAttackEnemy->SetCollisionType(CollisionType);//����^�C�v��ݒ肷��
	pAttackEnemy->GetLifeInfo().SetLife(nLife);                //�̗͂�ݒ�
	pAttackEnemy->GetLifeInfo().SetMaxLife(nLife);             //�ő�̗͂�ݒ�
	pAttackEnemy->GetLifeInfo().SetAutoSubLife(true);          //�̗͂������I�Ɍ��炷
	pAttackEnemy->GetPosInfo().SetPos(pos);                   //�ʒu  
	pAttackEnemy->GetRotInfo().SetRot(rot);                   //����
	pAttackEnemy->GetMoveInfo().SetMove(move);                 //�ړ���
	pAttackEnemy->GetSizeInfo().SetScale(Scale);               //�g�嗦
	pAttackEnemy->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia());
	pAttackEnemy->GetLifeInfo().SetAutoDeath(true);
	pAttackEnemy->GetMoveInfo().SetUseGravity(false, 1.0f);
	pAttackEnemy->SetHitOtherThanLibing(bHitOtherThanLiving);
	pAttackEnemy->SetAutoCollision(bAutoCollision);//�U���N���X�ɔ����C���邩�ǂ���
	//���f�����ݒ�
	int nIdx = CManager::GetObjectXInfo()->Regist(ATTACK_FILENAME[static_cast<int>(AttackType)]);

	//���f���������蓖�Ă�
	pAttackEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));
	pAttackEnemy->SetSize();
	return pAttackEnemy;
}
//======================================================================================================================