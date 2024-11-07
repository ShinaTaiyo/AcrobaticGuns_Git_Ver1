//==============================================================================================================
//
//�P�O���Q�W���F���b�N�I�������ꏊ�ɒe����������[lockon.cpp]
//Author:ShinaTaiyo
//
//==============================================================================================================

//===============================================================
//�C���N���[�h
//===============================================================
#include "lockon.h"
#include "manager.h"
#include "camera.h"
#include "texture.h"
#include "collision.h"
#include "input.h"
#include "particle.h"
#include "particle2d.h"
#include "calculation.h"
#include "objectX.h"
#include "debugproc.h"
//==============================================================================================================

//===============================================================
//�R���X�g���N�^
//===============================================================
CLockon::CLockon() : m_LockOnPos(NULL_VECTOR3),m_NowRay(NULL_VECTOR3),m_FrontPos(NULL_VECTOR3),m_EndState(ENDSTATE::NONE),m_NearRayColObjPos(NULL_VECTOR3)
{

}
//==============================================================================================================

//===============================================================
//�f�X�g���N�^
//===============================================================
CLockon::~CLockon()
{

}
//==============================================================================================================

//===============================================================
//����������
//===============================================================
HRESULT CLockon::Init()
{
	CObject2D::Init();
	return S_OK;
}
//==============================================================================================================

//===============================================================
//�I������
//===============================================================
void CLockon::Uninit()
{
	CObject2D::Uninit();
}
//==============================================================================================================

//===============================================================
//�X�V����
//===============================================================
void CLockon::Update()
{
	D3DXVECTOR3 Rot = GetRot();
	Rot.z += 0.02f;
	SetRot(Rot);

	//�ړ�����
	MoveProcess();

	//���C���_���Ă��鉜�̕ǂ̔���ʒu���T�[�`����
	BackWallRayCollisionPosSearch();

	//���C���v�Z����
	CalcRay();

	//��ԋ߂��I�u�W�F�N�g�̃��C�����������ʒu�����߂�
	RayCollisionToObject();

	//�I�u�W�F�N�g2D�X�V����
	CObject2D::Update();
}
//==============================================================================================================

//===============================================================
//�`�揈��
//===============================================================
void CLockon::Draw()
{
	CObject2D::Draw();
}
//==============================================================================================================

//===============================================================
//���S�t���O�ݒ菈��
//===============================================================
void CLockon::SetDeath()
{
	CObject::SetDeath();
}
//==============================================================================================================

//===============================================================
//��������
//===============================================================
CLockon* CLockon::Create(D3DXVECTOR3 Pos, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight, D3DXCOLOR col)
{
	CTexture* pTexture = CManager::GetTexture();
	CLockon* pLockOn = DBG_NEW CLockon;

	pLockOn->Init();//����������
	pLockOn->SetPos(Pos);//�ʒu�ݒ�
	pLockOn->SetSupportPos(Pos);//�x�_�ʒu�ݒ�
	pLockOn->SetInfo(1, 1, fWidth, fHeight, col,PolygonType,false);//���ݒ�i�K���j

	//�e�N�X�`���ݒ�
	pLockOn->SetTextureIndex(pTexture->Regist("data\\TEXTURE\\LockOn\\Target_000.png"));
	pLockOn->BindTexture(pTexture->GetAddress(pLockOn->GetTextureIndex()));

	//�̗͂��g�p���Ȃ�
	pLockOn->SetUseLife(false, 1, 1);

	//�I�u�W�F�N�g�^�C�v�ݒ�
	pLockOn->SetType(CObject::TYPE::LOCKON);


	return pLockOn;
}
//==============================================================================================================

//===============================================================
//�ړ�����
//===============================================================
void CLockon::MoveProcess()
{
	D3DXVECTOR3 Pos = GetPos();
	const D3DXVECTOR3& CameraRot = CManager::GetCamera()->GetRot();

	m_EndState = ENDSTATE::NONE;

	if (CManager::GetInputJoypad()->GetRStickPress(16) == true)
	{
		Pos.x += sinf(CManager::GetInputJoypad()->GetRStickAimRot()) * m_fNORMAL_LOCKONMOVE;
		Pos.y += cosf(CManager::GetInputJoypad()->GetRStickAimRot()) * -m_fNORMAL_LOCKONMOVE;
	}

	if (Pos.x + GetWidth() * 0.5f > SCREEN_WIDTH)
	{//�E
		Pos.x = SCREEN_WIDTH - GetWidth() * 0.5f;
		m_EndState = ENDSTATE::RIGHTEND;
		CManager::GetCamera()->SetRot(CameraRot + D3DXVECTOR3(0.0f, 0.01f, 0.0f));
	}
	if (Pos.x - GetWidth() * 0.5f < 0.0f)
	{//��
		Pos.x = 0.0f + GetWidth() * 0.5f;
		m_EndState = ENDSTATE::LEFTEND;
		CManager::GetCamera()->SetRot(CameraRot + D3DXVECTOR3(0.0f, -0.01f, 0.0f));

	}

	if (Pos.y + GetHeight() * 0.5f > SCREEN_HEIGHT)
	{//��
		Pos.y = SCREEN_HEIGHT - GetHeight() * 0.5f;
		m_EndState = ENDSTATE::UPEND;
	}
	if (Pos.y - GetHeight() * 0.5f < 0.0f)
	{//��
		Pos.y = 0.0f + GetHeight() * 0.5f;
		m_EndState = ENDSTATE::DOWNEND;
	}



	SetPos(Pos);
}
//==============================================================================================================

//===============================================================
//�J�[�\���̐�ɂ���I�u�W�F�N�g���T�[�`
//===============================================================
void CLockon::BackWallRayCollisionPosSearch()
{
	D3DXVECTOR3 Pos = GetPos();//�ʒu

	CCalculation::CalcScreenToXZ(&m_LockOnPos, int(Pos.x), int(Pos.y), SCREEN_WIDTH, SCREEN_HEIGHT,
		CManager::GetCamera()->GetMtxView(), CManager::GetCamera()->GetMtxProjection());
	CManager::GetDebugProc()->PrintDebugProc("���܂��͕ǂƂ̌�_�F%f %f %f\n", m_LockOnPos.x, m_LockOnPos.y, m_LockOnPos.z);

}
//==============================================================================================================

//===============================================================
//���C���v�Z����
//===============================================================
void CLockon::CalcRay()
{
	D3DXVECTOR3 FarPos = NULL_VECTOR3; //��
	D3DXVECTOR3 Ray = NULL_VECTOR3;    //���C
	//============================================
	//�J������O�Ɖ��̃��[���h���W�����߂�
	//============================================
	CCalculation::CalcScreenToWorld(&m_FrontPos, int(GetPos().x), int(GetPos().y), 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
		CManager::GetCamera()->GetMtxView(), CManager::GetCamera()->GetMtxProjection());//��O

	CCalculation::CalcScreenToWorld(&FarPos, int(GetPos().x), int(GetPos().y), 1.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
		CManager::GetCamera()->GetMtxView(), CManager::GetCamera()->GetMtxProjection());//��
	//============================================================================================================================

	m_NowRay = FarPos - m_FrontPos;//�x�N�g�������߂�
	D3DXVec3Normalize(&m_NowRay, &m_NowRay);//���K��

	CManager::GetDebugProc()->PrintDebugProc("���C�̌����F%f %f %f\n", m_NowRay.x, m_NowRay.y, m_NowRay.z);
}
//==============================================================================================================

//===============================================================
//���C���ǂ̃I�u�W�F�N�g�ɓ������Ă��邩���v�Z����
//===============================================================
void CLockon::RayCollisionToObject()
{
	bool bCollision = false;//�����蔻��
	vector<D3DXVECTOR3> VecCollisionSuccess;     //�����蔻�肪���������ʒu��vector
	D3DXVECTOR3 NearCollisionPos = NULL_VECTOR3; //�����蔻�肪���������ʒu�̒��ň�ԋ߂��ʒu

	//���C�ƈ�v�����S�ẴI�u�W�F�N�g�����߁A���S�_��Vector�ɕۑ�
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = CObject::GetTopObject(nCntPri);//�擪�I�u�W�F�N�g���擾
		D3DXVECTOR3 CollisionStartPos = NULL_VECTOR3;//�Փ˔���J�n�ʒu
		D3DXVECTOR3 CollisionEndPos = NULL_VECTOR3;  //�Փ˔���I���ʒu
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();//���̃I�u�W�F�N�g�̃|�C���^���擾

			if (pObj->GetType() == CObject::TYPE::ENEMY || pObj->GetType() == CObject::TYPE::BLOCK)
			{
				CObjectX* pObjX = (CObjectX*)pObj;
				//�w�肵�����f���̈ʒu
				//bCollision = CCollision::RayIntersectsAABB(NearPos, Ray, pEnemy->GetVtxMax() + pEnemy->GetPos(),pEnemy->GetVtxMin() + pEnemy->GetPos());
				bCollision = CCollision::RayIntersectsAABBCollisionPos(m_FrontPos, m_NowRay, pObjX->GetVtxMin() + pObjX->GetPos(), pObjX->GetVtxMax() + pObjX->GetPos(),
					CollisionStartPos);
				//bCollision = CCalculation::CalcRaySphere(NearPos, Ray, pEnemy->GetSenterPos(), pEnemy->GetSize().y, CollisionStartPos, CollisionEndPos);

				if (bCollision == true)
				{//���C�ƃT�C�Y/�Q���̋��̓����蔻�萬��
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 1, 20, 30.0f, 30.0f, 100, 10, false, CollisionStartPos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
					CManager::GetDebugProc()->PrintDebugProc("�Փ˂����ʒu�F%f %f %f\n", CollisionStartPos.x, CollisionStartPos.y, CollisionStartPos.z);

					//�G�̍ő咸�_�̃X�N���[�����W�����߂�
					D3DXVECTOR3 ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pObjX->GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
						float(SCREEN_WIDTH), float(SCREEN_HEIGHT));

					float fRot = CCalculation::CalculationRandVecXY();
					CParticle2D::Create(ScreenPos, D3DXVECTOR3(sinf(fRot) * 10.0f, cosf(fRot) * 10.0f, 0.0f), NULL_VECTOR3, CObject2D::POLYGONTYPE01_SENTERROLLING,
						15, 55.0f, 55.0f, CCalculation::CalRaibowColor());

					VecCollisionSuccess.push_back(CollisionStartPos);//�����蔻�肪���������I�u�W�F�N�g�̔���J�n�ʒu��ۑ�����
					CManager::GetDebugProc()->PrintDebugProc("���萬���������ǂ���:%d\n", bCollision);
				}
			}

			pObj = pNext;//�I�u�W�F�N�g�����ɐi�߂�
		}
	}

	//���C�̕�������v�����I�u�W�F�N�g�����݂�����A���̒��ň�ԋ������߂��I�u�W�F�N�g�̒��S�_�����߁A������ڊ|�����ړ��ʂ����߂�
	if (VecCollisionSuccess.size() != 0)
	{//�_���Ă���I�u�W�F�N�g�̒��S�_�Ɍ������Č���
		float fLength = 0.0f;//����
		float fMinLength = 0.0f;//��ԋ߂������i�[�p
		for (auto it = VecCollisionSuccess.begin(); it != VecCollisionSuccess.end(); it++)
		{
			fLength = CCalculation::CalculationLength(m_FrontPos, *it);//���C�̔��肪���������I�u�W�F�N�g�̈ʒu�ƃv���C���[�̒��S�_�̋����𑪂�

			if (it == VecCollisionSuccess.begin())
			{
				fMinLength = fLength;
				NearCollisionPos = *it;
			}
			else
			{
				if (fLength < fMinLength)
				{//��ԋ߂��������߂�������
					NearCollisionPos = *it;//��ԋ߂��I�u�W�F�N�g���i�[
				}
			}
		}

		m_NearRayColObjPos = NearCollisionPos;//���C������������ԋ߂��I�u�W�F�N�g�Ɍ������Č���
	}
	else
	{//�_���Ă�������̉��̕ǂɌ������Č���
		m_NearRayColObjPos = m_LockOnPos;//���C�����������I�u�W�F�N�g���Ȃ��̂ŁA���̕ǂɌ������Č���
	}
	//====================================================================================================================================================================

	//Vector���N���A
	VecCollisionSuccess.clear();

}
//==============================================================================================================