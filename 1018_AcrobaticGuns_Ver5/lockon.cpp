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
#include "game.h"
#include "debugproc.h"
#include "player.h"
#include "ui.h"
//==============================================================================================================

//===============================================================
//�R���X�g���N�^
//===============================================================
CLockon::CLockon(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject2D(nPri,bUseintPri,type,ObjType),
m_LockOnPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_NowRay(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_FrontPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_EndState(ENDSTATE::NONE),m_NearRayColObjPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_bRayCollision(false)
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
	pLockOn->SetAnimInfo(1, 1,false);//���ݒ�i�K���j
	pLockOn->SetPolygonType(PolygonType);
	pLockOn->SetWidth(fWidth);
	pLockOn->SetMaxWidth(fWidth);
	pLockOn->SetHeight(fHeight);
	pLockOn->SetMaxHeight(fHeight);

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

}
//==============================================================================================================

//===============================================================
//�J�[�\���̐�ɂ���I�u�W�F�N�g���T�[�`
//===============================================================
void CLockon::BackWallRayCollisionPosSearch()
{
	D3DXVECTOR3 Pos = GetPos();//�ʒu

	CCalculation::CalcScreenToWorld(&m_LockOnPos,GetPos().x,GetPos().y, 1.0f,SCREEN_WIDTH,SCREEN_HEIGHT,CManager::GetCamera()->GetMtxView(),
		CManager::GetCamera()->GetMtxProjection()); //�i�Ŗ��j�����`��͈͂̈�ԉ��̈ʒu
}
//==============================================================================================================

//===============================================================
//���C���v�Z����
//===============================================================
void CLockon::CalcRay()
{
	D3DXVECTOR3 FarPos = D3DXVECTOR3(0.0f,0.0f,0.0f); //��
	CCamera* pCamera = CManager::GetCamera();
	//============================================
	//�J������O�Ɖ��̃��[���h���W�����߂�
	//============================================
	CCalculation::CalcScreenToWorld(&m_FrontPos,GetPos().x,GetPos().y, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
		CManager::GetCamera()->GetMtxView(), CManager::GetCamera()->GetMtxProjection());//��O

	//m_FrontPos = CManager::GetCamera()->GetPosV();
	CManager::GetDebugProc()->PrintDebugProc("�J������O���W�F%f %f %f\n", m_FrontPos.x, m_FrontPos.y, m_FrontPos.z);
	CManager::GetDebugProc()->PrintDebugProc("�J�����x�_�F%f %f %f\n", pCamera->GetPosV().x, pCamera->GetPosV().y, pCamera->GetPosV().z);
	CCalculation::CalcScreenToWorld(&FarPos,GetPos().x,GetPos().y, 1.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
		CManager::GetCamera()->GetMtxView(), CManager::GetCamera()->GetMtxProjection());//��
	//============================================================================================================================

	m_NowRay = FarPos - m_FrontPos;//�x�N�g�������߂�y
	D3DXVec3Normalize(&m_NowRay, &m_NowRay);//���K��
}
//==============================================================================================================

//===============================================================
//���C���ǂ̃I�u�W�F�N�g�ɓ������Ă��邩���v�Z����
//===============================================================
void CLockon::RayCollisionToObject()
{
	//*�ϐ��錾
	bool bRayCollision = false;//���C�������������̏�Ԃ����Z�b�g
	vector<D3DXVECTOR3> VecCollisionSuccess;     //�����蔻�肪���������ʒu��vector
	CPlayer* pPlayer = CGame::GetPlayer();

	//���C�ƈ�v�����S�ẴI�u�W�F�N�g�����߁A���S�_��Vector�ɕۑ�
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = CObject::GetTopObject(nCntPri);//�擪�I�u�W�F�N�g���擾
		D3DXVECTOR3 CollisionStartPos = D3DXVECTOR3(0.0f,0.0f,0.0f);//�Փ˔���J�n�ʒu
		D3DXVECTOR3 CollisionEndPos = D3DXVECTOR3(0.0f,0.0f,0.0f);  //�Փ˔���I���ʒu
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();//���̃I�u�W�F�N�g�̃|�C���^���擾

			if (pObj->GetType() == CObject::TYPE::ENEMY || pObj->GetType() == CObject::TYPE::BLOCK || pObj->GetType() == CObject::TYPE::BGMODEL)
			{
				CObjectX* pObjX = dynamic_cast<CObjectX*>(pObj);
				//�w�肵�����f���̈ʒu
				bRayCollision = CCollision::RayIntersectsAABBCollisionPos(m_FrontPos, m_NowRay, pObjX->GetVtxMin() + pObjX->GetPos(), pObjX->GetVtxMax() + pObjX->GetPos(),
					CollisionStartPos);

				// ��`�͈͓����ǂ����𔻒�
				if (bRayCollision == true)
				{//���C�ƃT�C�Y/�Q���̋��̓����蔻�萬��
					//�J�����̈ʒu������I�u�W�F�N�g�̃T�C�Y�̒��ɓ����Ă�����
					CManager::GetDebugProc()->PrintDebugProc("���C�̎x�_���J�����ɔ���Ă��Ȃ�\n");
					CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 1, 20, 30.0f, 30.0f, 100, 10, false, CollisionStartPos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), true);
					//�G�̒��S�̃X�N���[�����W�����߂�
					D3DXVECTOR3 ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pObjX->GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
						float(SCREEN_WIDTH), float(SCREEN_HEIGHT));
					VecCollisionSuccess.push_back(CollisionStartPos);//�����蔻�肪���������I�u�W�F�N�g�̔���J�n�ʒu��ۑ�����
				}
			}

			pObj = pNext;//�I�u�W�F�N�g�����ɐi�߂�
		}
	}

	D3DXVECTOR3 NearCollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�����蔻�肪���������ʒu�̒��ň�ԋ߂��ʒu

	//���C�̕�������v�����I�u�W�F�N�g�����݂�����A���̒��ň�ԋ������߂��I�u�W�F�N�g�̒��S�_�����߁A������ڊ|�����ړ��ʂ����߂�
	if (VecCollisionSuccess.size() != 0)
	{//�_���Ă���I�u�W�F�N�g�̒��S�_�Ɍ������Č���
		float fLength = 0.0f;//����
		float fMinLength = 0.0f;//��ԋ߂������i�[�p
		for (auto it = VecCollisionSuccess.begin(); it != VecCollisionSuccess.end(); ++it)
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
		m_bRayCollision = true;//���C�̓����蔻�肪����
		m_NearRayColObjPos = NearCollisionPos;//���C������������ԋ߂��I�u�W�F�N�g�Ɍ������Č���
	}
	else
	{//�_���Ă�������̉��̕ǂɌ������Č���
		m_bRayCollision = false;//���C�̓����蔻�肪���s
		m_NearRayColObjPos = m_LockOnPos;//���C�����������I�u�W�F�N�g���Ȃ��̂ŁA���̕ǂɌ������Č���
	}
	//====================================================================================================================================================================

	//Vector���N���A
	VecCollisionSuccess.clear();         //�m�ۂ����z�����ɂ���
	VecCollisionSuccess.shrink_to_fit(); //���݂̃T�C�Y�ɍ��킹�ă������̈���R���p�N�g�ɂ���

}
//==============================================================================================================