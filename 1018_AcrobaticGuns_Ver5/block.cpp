//==========================================================================================================
//
//�V���P�Q���F�S�M�~�b�N����������[block.cpp]
//Author:ShinaTaiyo
//
//==========================================================================================================

//=========================
//�C���N���[�h
//=========================
#include "block.h"
#include "manager.h"
#include "objectXInfo.h"
#include "calculation.h"
#include "stagemanager.h"
#include "player.h"
#include "game.h"
#include "edit.h"
#include "input.h"
#include "debugproc.h"
#include <stdio.h>
#include <string.h>
//=================================================

//=========================
//�ÓI�����o
//=========================
int CBlock::m_nNumFile = CBlock::BLOCKTYPE_MAX;                                              //�t�@�C�������i�[����
const float CBlock::m_fBLOCKCORRECTIONCOLLISION = 3.0f;      //1,2,3,4,5,6,7,8,9,10,11,12,13 
const char* CBlock::m_BLOCK_FILENAME[CBlock::BLOCKTYPE_MAX] =
{ "data\\MODEL\\Block\\Block00_Normal.x", 
};                                                  //�u���b�N��X�t�@�C���ւ̃|�C���^
//========================================================

//=========================
//�R���X�g���N�^
//=========================
CBlock::CBlock() : CObjectX(2),m_bCollision(false),m_type(BLOCKTYPE00_NORMAL)
{

}
//=================================================

//=========================
//�f�X�g���N�^
//=========================
CBlock::~CBlock()
{

}
//=================================================

//=========================
//����������
//=========================
HRESULT CBlock::Init()
{
	//===========================
	//X�I�u�W�F�N�g������
	//===========================
	CObjectX::Init();
	//=========================================
	m_type = BLOCKTYPE00_NORMAL;   //�u���b�N�̎��
	m_bCollision = true;           //�����蔻������邩�ǂ���
	return S_OK;
}
//=================================================

//=========================
//�I������
//=========================
void CBlock::Uninit()
{
	CObjectX::Uninit();//X�I�u�W�F�N�g�I������
}
//=================================================

//=========================
//�X�V����
//=========================
void CBlock::Update()
{
	GravityProcess();

	//========================================
    //X�I�u�W�F�N�g�̍X�V����
    //========================================
	CObjectX::Update();
	//===========================================================================================

	Collision();//�ڐG������s��
}
//=================================================

//===================================================
//�`�揈��
//===================================================
void CBlock::Draw()
{
	CObjectX::Draw();
}
//===========================================================================================

//===================================================
//���S�t���O��ݒ�
//===================================================
void CBlock::SetDeath()
{
	CObject::SetDeath();
}
//===========================================================================================

//==================================
//�u���b�N��������
//==================================
CBlock* CBlock::Create(BLOCKTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	CBlock* pBlock = DBG_NEW CBlock;                                                               //�u���b�N�𐶐�
	bool bSuccess = pBlock->CObject::GetCreateSuccess();
	int nIdx = 0;//���f�����̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pBlock != nullptr)
		{
			pBlock->Init();                               //����������
			pBlock->SetUseGravity(false,1.0f);            //�d�͂�ݒ肷��
			pBlock->SetRot(rot);                          //������ݒ�
			pBlock->SetUseDeath(true);                    //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pBlock->m_type = type;                        //�u���b�N�̎��
			pBlock->SetLife(nLife);                       //�u���b�N�̗̑�
			pBlock->SetMaxLife(nLife);                    //�u���b�N�̗̑�
			pBlock->SetAutoSubLife(false);                //�����I�ɑ̗͂����炷���ǂ���
			pBlock->SetSupportPos(pos);                   //�ݒu�ʒu
			pBlock->CObjectX::SetPos(pos);                //�I�u�W�F�N�gX�̈ʒu��ݒ�
			pBlock->SetPosOld(pos);                       //1f�O�̈ʒu��ݒ�
			pBlock->CObject::SetType(CObject::TYPE_BLOCK);//�I�u�W�F�N�g�̎�ނ����߂�
			pBlock->SetScale(scale);                      //�I�u�W�F�N�g�̊g�嗦
			pBlock->SetFormarScale(scale);                //���̊g�嗦��ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(m_BLOCK_FILENAME[(int)(type)]);
			pBlock->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
				CManager::GetObjectXInfo()->GetTexture(nIdx),
				CManager::GetObjectXInfo()->GetColorValue(nIdx));                     //���f���������蓖�Ă�

			pBlock->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BLOCK);               //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pBlock->CObjectX::SetTypeNum((int)(type));                                //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pBlock->SetSize();                                                        //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pBlock->SetManagerType((int)(CStageManager::MANAGEROBJECT_BLOCK));        //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�

		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pBlock;
		pBlock = nullptr;
		return nullptr;
	}
	return pBlock;
}
//================================================================================================

//===============================================================
//�u���b�N����������
//===============================================================
void CBlock::ReleaseBlock()
{
	Release();
}
//=========================================================================================================================

//===============================================================
//�u���b�N�̃^�C�v���擾����
//===============================================================
CBlock::BLOCKTYPE CBlock::GetType()
{
	return m_type;
}
//=========================================================================================================================

//===============================================================
//�u���b�N�Ƃ̓����蔻����s��
//===============================================================
void CBlock::Collision()
{
	//==================================
	//�I�u�W�F�N�gX��������擾
	//==================================
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //����Ώۂ̈ʒu���擾
	D3DXVECTOR3 ComparisonPosOld = NULL_VECTOR3;               //����Ώۂ�1f�O�̈ʒu���擾
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //����Ώۂ̍ő咸�_���擾
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //����Ώۂ̍ŏ����_���擾
	D3DXVECTOR3 MySize = NULL_VECTOR3;
	D3DXVECTOR3 BlockPos = GetPos();                          //�u���b�N�̈ʒu���擾
	D3DXVECTOR3 BlockVtxMax = GetVtxMax();                    //�u���b�N�̍ő咸�_���擾
	D3DXVECTOR3 BlockVtxMin = GetVtxMin();                    //�u���b�N�̍ŏ����_���擾
	//===========================================================

	//�I�u�W�F�N�g�擾
	CObject* pObj = nullptr;
	//==================================
	//�u���b�N�Ƃ̓����蔻��
	//==================================
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
		while (pObj != nullptr)
		{
			//���̃I�u�W�F�N�g���i�[
			CObject* pNext = pObj->GetNextObject();

			//��ނ̎擾�i�G�Ȃ瓖���蔻��j
			CObject::TYPE type = pObj->GetType();

			//�I�u�W�F�N�g�̎�ނ��G��������
			if (type == CObject::TYPE_PLAYER || type == CObject::TYPE_ENEMY || type == CObject::TYPE_BOSS)
			{
				ComparisonPos.x = (float)(floor(((CObjectX*)pObj)->GetPos().x));                   //�ʒu���擾
				ComparisonPos.y = (float)(floor(((CObjectX*)pObj)->GetPos().y));                   //�ʒu���擾
				ComparisonPos.z = (float)(floor(((CObjectX*)pObj)->GetPos().z));                   //�ʒu���擾]

				ComparisonVtxMax.x = (float)(floor(((CObjectX*)pObj)->GetVtxMax().x));             //�ő咸�_���擾
				ComparisonVtxMax.y = (float)(floor(((CObjectX*)pObj)->GetVtxMax().y));             //�ő咸�_���擾
				ComparisonVtxMax.z = (float)(floor(((CObjectX*)pObj)->GetVtxMax().z));             //�ő咸�_���擾

				ComparisonVtxMin.x = (float)(floor(((CObjectX*)pObj)->GetVtxMin().x));             //�ŏ����_���擾
				ComparisonVtxMin.y = (float)(floor(((CObjectX*)pObj)->GetVtxMin().y));             //�ŏ����_���擾
				ComparisonVtxMin.z = (float)(floor(((CObjectX*)pObj)->GetVtxMin().z));             //�ŏ����_���擾

				ComparisonPosOld.x = (float)(floor(((CObjectX*)pObj)->GetPosOld().x));             //1f�O�̈ʒu���擾
				ComparisonPosOld.y = (float)(floor(((CObjectX*)pObj)->GetPosOld().y));             //1f�O�̈ʒu���擾
				ComparisonPosOld.z = (float)(floor(((CObjectX*)pObj)->GetPosOld().z));             //1f�O�̈ʒu���擾

				MySize.x = (float)(floor(((CObjectX*)pObj)->GetSize().x));
				MySize.y = (float)(floor(((CObjectX*)pObj)->GetSize().y));
				MySize.z = (float)(floor(((CObjectX*)pObj)->GetSize().z));
				//((CObjectX*)pObj)->SetLiftLanding(false);                     //���t�g�ɏ���Ă��Ȃ���Ԃɏ�����
				//��
				if (ComparisonPos.x + ComparisonVtxMax.x > BlockPos.x + BlockVtxMin.x
					&& ComparisonPos.x + ComparisonVtxMin.x < BlockPos.x + BlockVtxMax.x
					&& ComparisonPos.y + ComparisonVtxMin.y - m_fBLOCKCORRECTIONCOLLISION < BlockPos.y + BlockVtxMax.y
					&& ComparisonPosOld.y + ComparisonVtxMin.y + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.y + BlockVtxMax.y
					&& ComparisonPos.z + ComparisonVtxMax.z > BlockPos.z + BlockVtxMin.z
					&& ComparisonPos.z + ComparisonVtxMin.z < BlockPos.z + BlockVtxMax.z)
				{//�Ώۂ̉��[�����f����+Y�ɓ����������̏���

				}
				if (ComparisonPos.x + ComparisonVtxMin.x - m_fBLOCKCORRECTIONCOLLISION <= BlockPos.x + BlockVtxMax.x &&
					ComparisonPos.x + ComparisonVtxMax.x + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.x + BlockVtxMin.x &&
					ComparisonPos.y + ComparisonVtxMin.y <= BlockPos.y + BlockVtxMax.y &&
					ComparisonPos.y + ComparisonVtxMax.y >= BlockPos.y + BlockVtxMin.y &&
					ComparisonPos.z + ComparisonVtxMin.z - m_fBLOCKCORRECTIONCOLLISION <= BlockPos.z + BlockVtxMax.z &&
					ComparisonPos.z + ComparisonVtxMax.z + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.z + BlockVtxMin.z)
				{//�u���b�N�ɓ���������

				}
			}
			//���X�g�����ɐi�߂�
			pObj = pNext;
		}
	}

}
//=========================================================================================================================

//===============================================================
//�u���b�N�Ƃ̓����蔻����s��
//===============================================================
void CBlock::CollisionSquare(CObjectX* pObjX)
{
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;        //��r�p�ʒu
	D3DXVECTOR3 ComparisonPosOld = NULL_VECTOR3;     //��r�p1f�O�̈ʒu
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;     //��r�p�ō����_
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;     //��r�p�Œᒸ�_
	D3DXVECTOR3 ComparisonSize = NULL_VECTOR3;       //��r�T�C�Y
	D3DXVECTOR3 ComparisonMove = NULL_VECTOR3;       //��r�ړ���
	//==========================================================================================================
	bool& bIsLanding = pObjX->GetLanding();
	bool bIsLandingOld = pObjX->GetLanding();//1f�O�̒��n���
	bIsLanding = false;
	bool& bIsWalling = pObjX->GetWalling();          //�ǂɂ������Ă��邩�ǂ���
	bIsWalling = false;

	bool bCollision = true;//�����蔻������邩�ǂ���

	D3DXVECTOR3 Scale = NULL_VECTOR3;

	//====================================================================
	//����J�n
	//====================================================================
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		CObject * pObj = GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
		while (pObj != nullptr)
		{
			//���̃I�u�W�F�N�g���i�[
			CObject* pNext = pObj->GetNextObject();

			//��ނ̎擾�i�G�Ȃ瓖���蔻��j
			CObject::TYPE type = pObj->GetType();
			if (type == CObject::TYPE_BLOCK)
			{
				CBlock* pBlock = (CBlock*)pObj;

				float fLength = CCalculation::CalculationLength(pObjX->GetPos(), pBlock->GetPos());
				Scale = pBlock->GetScale();
				if (Scale.x >= Scale.y - 0.1f && Scale.x <= Scale.y + 0.1f)
				{//�g�嗦���قƂ�Ǔ����Ȃ�
					if (fLength > pBlock->GetSize().x * 3.0f)
					{
						bCollision = false;
					}
					else
					{
						bCollision = true;
					}
				}
				else
				{
					bCollision = true;
				}

				if (pBlock->m_bCollision == true && bCollision == true)
				{
					if (bIsLandingOld == true)
					{
						//�㉺
						pBlock->ExtrusionCollisionY(pObjX, pBlock);

						//���E
						pBlock->ExtrusionCollisionX(pObjX, pBlock);
						
					}
					else
					{
						//���E
						pBlock->ExtrusionCollisionX(pObjX, pBlock);

						//�㉺
						pBlock->ExtrusionCollisionY(pObjX, pBlock);
					}
				}
			}
			//���X�g�����ɐi�߂�
			pObj = pNext;
		}
	}
}
//=========================================================================================================================

//===============================================================
//�w�艺�u���b�N�̏�ɏ���Ă���ꍇ�A�ʒu��␳����
//===============================================================
void CBlock::LandingCorrection(D3DXVECTOR3& Pos, CObject* pSaveObj,D3DXVECTOR3 VtxMin)
{
	CObject::TYPE type = (CObject::TYPE)(0);
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;        //��r�p�ʒu
	D3DXVECTOR3 ComparisonPosOld = NULL_VECTOR3;     //��r�p1f�O�̈ʒu
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;     //��r�p�ō����_
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;     //��r�p�Œᒸ�_
	D3DXVECTOR3 ComparisonSize = NULL_VECTOR3;       //�T�C�Y
	D3DXVECTOR3 ComparisonMove = NULL_VECTOR3;

	D3DXVECTOR3 MyVtxMin = NULL_VECTOR3;
	MyVtxMin.x = (float)(floor(VtxMin.x));
	MyVtxMin.y = (float)(floor(VtxMin.y));
	MyVtxMin.z = (float)(floor(VtxMin.z));

	if (pSaveObj != nullptr)
	{
		//��ނ̎擾
		type = pSaveObj->GetType();
		if (type == CObject::TYPE_BLOCK)
		{
			CBlock* pBlock = (CBlock*)pSaveObj;

			ComparisonPos.x = (float)(floor(pBlock->GetPos().x));
			ComparisonPos.y = (float)(floor(pBlock->GetPos().y));
			ComparisonPos.z = (float)(floor(pBlock->GetPos().z));

			ComparisonPosOld.x = (float)(floor(pBlock->GetPosOld().x));
			ComparisonPosOld.y = (float)(floor(pBlock->GetPosOld().y));
			ComparisonPosOld.z = (float)(floor(pBlock->GetPosOld().z));

			ComparisonVtxMax.x = (float)(floor(pBlock->GetVtxMax().x));
			ComparisonVtxMax.y = (float)(floor(pBlock->GetVtxMax().y));
			ComparisonVtxMax.z = (float)(floor(pBlock->GetVtxMax().z));

			ComparisonVtxMin.x = (float)(floor(pBlock->GetVtxMin().x));
			ComparisonVtxMin.y = (float)(floor(pBlock->GetVtxMin().y));
			ComparisonVtxMin.z = (float)(floor(pBlock->GetVtxMin().z));

			ComparisonSize.x = (float)(floor(pBlock->GetSize().x));
			ComparisonSize.y = (float)(floor(pBlock->GetSize().y));
			ComparisonSize.z = (float)(floor(pBlock->GetSize().z));
			 

			float fPosY = fabsf(MyVtxMin.y);
			Pos.y = ComparisonPos.y + ComparisonVtxMax.y + fPosY;
		}
	}

}
//=========================================================================================================================

//=======================================================================
//X�����̉����o��������s��
//=======================================================================
void CBlock::ExtrusionCollisionX(CObjectX* pMyObjX, CBlock* pBlock)
{
	D3DXVECTOR3 MyPos = pMyObjX->GetPos();
	D3DXVECTOR3& Pos = pMyObjX->GetPos();              //�ʒu���擾
	D3DXVECTOR3& PosOld = pMyObjX->GetPosOld();        //1f�O�̈ʒu���擾
	D3DXVECTOR3 MyVtxMax = NULL_VECTOR3;             //�������g�̍ő咸�_
	D3DXVECTOR3 MyVtxMin = NULL_VECTOR3;             //�������g�̍ŏ����_

	D3DXVECTOR3 ComPos = pBlock->GetPos();             //�u���b�N�̈ʒu
	D3DXVECTOR3 ComVtxMax = pBlock->GetVtxMax();       //�u���b�N�̍ő咸�_
	D3DXVECTOR3 ComVtxMin = pBlock->GetVtxMin();       //�u���b�N�̍ŏ����_
	D3DXVECTOR3 ComMove = pBlock->GetMove();           //�u���b�N�̍ŏ����_

	bool& bIsWalling = pMyObjX->GetWalling();          //�ǂɂ������Ă��邩�ǂ���
	//�����o���Ɏg���l�̏����_��؂�̂Ă�i�����ȍ��Ŏ��s���Ȃ����߁j
	MyVtxMax.x = (float)(floor(pMyObjX->GetVtxMax().x));
	MyVtxMax.y = (float)(floor(pMyObjX->GetVtxMax().y));
	MyVtxMax.z = (float)(floor(pMyObjX->GetVtxMax().z));

	MyVtxMin.x = (float)(floor(pMyObjX->GetVtxMin().x));
	MyVtxMin.y = (float)(floor(pMyObjX->GetVtxMin().y));
	MyVtxMin.z = (float)(floor(pMyObjX->GetVtxMin().z));

	ComPos.x = float(floor(pBlock->GetPos().x));
	ComPos.y = float(floor(pBlock->GetPos().y));
	ComPos.z = float(floor(pBlock->GetPos().z));

	ComVtxMax.x = (float)(floor(ComVtxMax.x));
	ComVtxMax.y = (float)(floor(ComVtxMax.y));
	ComVtxMax.z = (float)(floor(ComVtxMax.z));

	ComVtxMin.x = (float)(floor(ComVtxMin.x));
	ComVtxMin.y = (float)(floor(ComVtxMin.y));
	ComVtxMin.z = (float)(floor(ComVtxMin.z));
	// 					//��
	if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& PosOld.x + MyVtxMax.x <= ComPos.x + ComVtxMin.x
		&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//�Ώۂ��u���b�N��-X�ɓ����������̏���
		Pos.x = ComPos.x + ComVtxMin.x - MyVtxMax.x;
		bIsWalling = true;
	}

	//�E
	else if (Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& PosOld.x + MyVtxMin.x >= ComPos.x + ComVtxMax.x
		&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//�Ώۂ��u���b�N��+X�ɓ����������̏���
		Pos.x = ComPos.x + ComVtxMax.x - MyVtxMin.x;
		bIsWalling = true;
	}
}
//======================================================================================================================================

//=======================================================================
//Y�����̉����o��������s��
//=======================================================================
void CBlock::ExtrusionCollisionY(CObjectX* pMyObjX, CBlock* pBlock)
{
	D3DXVECTOR3 MyVtxMax = NULL_VECTOR3;             //�������g�̍ő咸�_
	D3DXVECTOR3 MyVtxMin = NULL_VECTOR3;             //�������g�̍ŏ����_

	D3DXVECTOR3 ComPos = pBlock->GetPos();             //�u���b�N�̈ʒu
	D3DXVECTOR3 ComVtxMax = pBlock->GetVtxMax();       //�u���b�N�̍ő咸�_
	D3DXVECTOR3 ComVtxMin = pBlock->GetVtxMin();       //�u���b�N�̍ŏ����_[
	D3DXVECTOR3 ComMove = pBlock->GetMove();           //�u���b�N�̍ŏ����_[

	D3DXVECTOR3& Pos = pMyObjX->GetPos();              //�ʒu���擾
	D3DXVECTOR3& Move = pMyObjX->GetMove();             //�ړ��ʂ��擾
	D3DXVECTOR3 PosOld = pMyObjX->GetPosOld();        //1f�O�̈ʒu���擾

	bool& bIsLanding = pMyObjX->GetLanding();          //�n�ʂɂ��邩�ǂ���

	bool& bIsJumping = pMyObjX->GetJumping();          //�W�����v���Ă��邩�ǂ���

	//�����o�����s�΍�
	MyVtxMax.x = (float)(floor(pMyObjX->GetVtxMax().x));
	MyVtxMax.y = (float)(floor(pMyObjX->GetVtxMax().y));
	MyVtxMax.z = (float)(floor(pMyObjX->GetVtxMax().z));

	MyVtxMin.x = (float)(floor(pMyObjX->GetVtxMin().x));
	MyVtxMin.y = (float)(floor(pMyObjX->GetVtxMin().y));
	MyVtxMin.z = (float)(floor(pMyObjX->GetVtxMin().z));

	ComPos.x = float(floor(pBlock->GetPos().x));
	ComPos.y = float(floor(pBlock->GetPos().y));
	ComPos.z = float(floor(pBlock->GetPos().z));

	ComVtxMax.x = (float)(floor(ComVtxMax.x));
	ComVtxMax.y = (float)(floor(ComVtxMax.y));
	ComVtxMax.z = (float)(floor(ComVtxMax.z));

	ComVtxMin.x = (float)(floor(ComVtxMin.x));
	ComVtxMin.y = (float)(floor(ComVtxMin.y));
	ComVtxMin.z = (float)(floor(ComVtxMin.z));

	//��
	if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& PosOld.y + MyVtxMin.y - Move.y >= ComPos.y + ComVtxMax.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//�Ώۂ̉��[�����f����+Y�ɓ����������̏���
		float fPosY = fabsf(MyVtxMin.y);
		Pos.y = ComPos.y + ComVtxMax.y + fPosY;
		Pos += ComMove;

		bIsLanding = true;
	}
	//��
	else if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& PosOld.y + MyVtxMax.y <= ComPos.y + ComVtxMin.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//�Ώۂ̉��[�����f����+Y�ɓ����������̏���
		Pos.y = ComPos.y + ComVtxMin.y - MyVtxMax.y;
		bIsJumping = false;

		if (pMyObjX->GetType() == CObject::TYPE_PLAYER)
		{
			Move.y = 0.0f;
		}
	}
}
//======================================================================================================================================