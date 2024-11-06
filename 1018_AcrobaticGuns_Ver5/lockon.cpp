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
#include "input.h"
#include "calculation.h"
#include "debugproc.h"
//==============================================================================================================

//===============================================================
//�R���X�g���N�^
//===============================================================
CLockon::CLockon() : m_LockOnPos(NULL_VECTOR3),m_NowRay(NULL_VECTOR3),m_FrontPos(NULL_VECTOR3),m_EndState(ENDSTATE::NONE)
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
	Rot.z += 0.01f;
	SetRot(Rot);

	//�ړ�����
	MoveProcess();

	//���C�̏���
	SearchProcess();

	//���C���v�Z����
	CalcRay();

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
	pLockOn->SetType(CObject::TYPE_LOCKON);


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
void CLockon::SearchProcess()
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