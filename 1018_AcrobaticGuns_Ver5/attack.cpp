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
#include "objectXInfo.h"
#include "block.h"
#include "bgModel.h"
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
CAttack::CAttack(int nPower, int nSetHitStopTime, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectXAlive(nPri, bUseintPri, type, ObjType),
m_Type(ATTACKTYPE::BULLET), m_nPower(nPower), m_HitStop({0,nSetHitStopTime}),m_bCollisionRelease(true),m_CollisionType(CAttack::COLLISIONTYPE::NONE),
m_TargetType(CAttack::TARGETTYPE::NONE),m_bHitOtherThanLiving(false),m_bAutoCollision(true)
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
	CObjectXAlive::Init();

	SetAdjustPos(false);
	return S_OK;
}
//======================================================================================================================

//==================================================================
//�I������
//==================================================================
void CAttack::Uninit()
{
	CObjectXAlive::Uninit();
}
//======================================================================================================================

//==================================================================
//�X�V����
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

					if (CCollision::CollisionSquare(GetPos(), GetVtxMax(), GetVtxMin(), pObjX->GetPos(), pObjX->GetVtxMax(), pObjX->GetVtxMin()))
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
	CObjectXAlive::Draw();
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
				CObjectXAlive* pObjX = static_cast<CObjectXAlive*>(pObj);
				CollisionProcess(bCollision, bNowCollision, pObjX);
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
		SetDeath();
	}

}
//======================================================================================================================

//==================================================================
//�����蔻�苤�ʏ���
//==================================================================
void CAttack::CollisionProcess(bool& bCollision, bool& bNowCollision, CObjectXAlive* pObjX)
{
	switch (GetCollisionType())
	{
	case CAttack::COLLISIONTYPE::SQUARE:
		if (CCollision::CollisionSquare(GetPos(), GetVtxMax(), GetVtxMin(), pObjX->GetPos(), pObjX->GetVtxMax(), pObjX->GetVtxMin()) == true)
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
		pObjX->SetDamage(GetPower(), GetHitStopTime());
		pObjX->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), GetHitStopTime(), false, false);
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
	pAttackPlayer->SetLife(nLife);                //�̗͂�ݒ�
	pAttackPlayer->SetMaxLife(nLife);             //�ő�̗͂�ݒ�
	pAttackPlayer->SetPos(pos);                   //�ʒu  
	pAttackPlayer->SetSupportPos(pos);            //�x�_�ʒu
	pAttackPlayer->SetRot(rot);                   //����
	pAttackPlayer->SetMove(move);                 //�ړ���
	pAttackPlayer->SetScale(Scale);               //�g�嗦
	pAttackPlayer->SetAutoSubLife(true);          //�̗͂��g�p����
	pAttackPlayer->SetUseInteria(false, CObjectXMove::GetNormalInertia());
	pAttackPlayer->SetUseGravity(false,1.0f);
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
	pAttackEnemy->SetLife(nLife);                //�̗͂�ݒ�
	pAttackEnemy->SetMaxLife(nLife);             //�ő�̗͂�ݒ�
	pAttackEnemy->SetAutoSubLife(true);          //�̗͂������I�Ɍ��炷
	pAttackEnemy->SetPos(pos);                   //�ʒu  
	pAttackEnemy->SetRot(rot);                   //����
	pAttackEnemy->SetMove(move);                 //�ړ���
	pAttackEnemy->SetScale(Scale);               //�g�嗦
	pAttackEnemy->SetUseInteria(false, CObjectXMove::GetNormalInertia());
	pAttackEnemy->SetUseGravity(false, 1.0f);
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