//===================================================================
//
//�U���R���F3D�I�u�W�F�N�g��`�悷��[Camera.cpp]
//Author:ShinaTaiyo
//
//===================================================================

//===================================================================
//�C���N���[�h
//===================================================================
#include "camera.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "object.h"
#include "particle.h"
#include "game.h"
#include "debugproc.h"
#include "edit.h"
#include "calculation.h"
#include "collision.h"
#include "object.h"
#include "objectX.h"
#include "title.h"
#include "tutorial.h"
//====================================================================================================

//====================================================================
//�ÓI�����o�錾
//====================================================================
const float CCamera::m_BESIDECAMERALENGTH = 570.0f;//�r�T�C�h�r���[�̃J�����̋���
const float CCamera::s_fINITIAL_LENGTH = 200.0f;   //�J�����Ƃ̍ŏ��̋���
//====================================================================================================

//====================================================================
//�R���X�g���N�^
//====================================================================
CCamera::CCamera() : m_SupportPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_fLength(s_fINITIAL_LENGTH), m_fTurningRotSpeed(0.0f),m_fTurningSpeedY(0.0f),m_PosV(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_PosR(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_VecU(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_Rot(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_mtxProjection(),m_mtxView(),m_CameraType(CAMERATYPE_BIRD),m_DifferenceLength(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_ZoomSpeed(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_nShakeFrame(0),m_ModeTime(0),m_fShakePower(0.0f),m_fAddLength(0.0f),m_AddPosR(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_AddPosV(D3DXVECTOR3(0.0f,0.0f,0.0f))
,m_bCustom(false),m_State(CAMERASTATE::NORMAL),m_pCameraState(DBG_NEW CCameraState_Normal())
{

}
//====================================================================================================

//====================================================================
//�f�X�g���N�^
//====================================================================
CCamera::~CCamera()
{

}
//====================================================================================================

//====================================================================
//����������
//====================================================================
HRESULT CCamera::Init()
{
	m_PosV = D3DXVECTOR3(0.0f, 200.0f,-350.0f);     //�x�_
	m_PosR = D3DXVECTOR3(0.0f,0.0f,0.0f);                          //�����_
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);         //������x�N�g���i�@���j
	m_Rot = D3DXVECTOR3(-D3DX_PI * 0.5f,0.0f,0.0f);                           //����
	m_mtxProjection = {};                           //�v���W�F�N�V�����}�g���b�N�X
	m_mtxView = {};                                 //�r���[�}�g���b�N�X
	m_CameraType = CAMERATYPE_BIRD;                 //�J�������[�h�̎��
	m_nShakeFrame = 0;                              //�J������h�炷�t���[����
	m_fShakePower = 0.0f;                           //�J������h�炷��
	m_DifferenceLength = D3DXVECTOR3(0.0f,0.0f,0.0f);              //�ΏۂƂ̋���
	m_ModeTime = 0;                                 //���[�h�̎���
	m_ZoomSpeed = D3DXVECTOR3(0.0f,0.0f,0.0f);                     //�Y�[�����鑬��
	return S_OK;
}
//====================================================================================================

//====================================================================
//�I������
//====================================================================
void CCamera::Uninit()
{
	if (m_pCameraState != nullptr)
	{
		delete m_pCameraState;
		m_pCameraState = nullptr;
	}
}
//====================================================================================================


//====================================================================
//�X�V����
//====================================================================
void CCamera::Update()
{
	m_pCameraState->Process(this);
	//========================================
	//�J�����̌�����␳����
	//========================================
	if (m_Rot.x < -D3DX_PI + 0.01f)
	{
		m_Rot.x = -D3DX_PI + 0.01f;
	}
	if (m_Rot.x > -0.01f)
	{
		m_Rot.x = -0.01f;
	}

	m_Rot.y = CCalculation::CorrectionRot(m_Rot.y);

	CManager::GetDebugProc()->PrintDebugProc("�J�����̌����F%f %f %f\n", m_Rot.x, m_Rot.y, m_Rot.z);
	CManager::GetDebugProc()->PrintDebugProc("�J�����̎��_�F%f %f %f\n", m_PosV.x, m_PosV.y, m_PosV.z);
	CManager::GetDebugProc()->PrintDebugProc("�J�����̒����_�F%f %f %f\n", m_PosR.x, m_PosR.y, m_PosR.z);
	CManager::GetDebugProc()->PrintDebugProc("�J�����Ƃ̋����F%f\n",m_fAddLength);

	//�J�����̒ʏ�̒����_��ݒ肵������
	NormalCameraMove();

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (m_PosV.y < 0.0f)
		{//���_���O�ȉ��Ȃ�S�Ẳe��`�悵�Ȃ�
			CObjectX::SetCommonDraw(false);
		}
		else
		{
			CObjectX::SetCommonDraw(true);
		}
	}
	MakeTransparent();//�J�����ƒ��~�_�Əd�Ȃ����I�u�W�F�N�g�𓧖��ɂ��鏈��

	//=================================================================================================================================

}
//====================================================================================================


//====================================================================
//�J�����ݒ菈��
//====================================================================
void CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice;                       //�f�o�C�X�ւ̃|�C���^
	pDevice = CManager::GetRenderer()->GetDevice();  //�f�o�C�X�̎擾

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	if (m_CameraType == CAMERATYPE_BIRD || m_CameraType == CAMERATYPE_BESIDE)
	{
		//�v���W�F�N�V�����}�g���b�N�X���쐬z
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
			D3DXToRadian(45.0f),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			10.0f,
			30000.0f);
	}
	else if (m_CameraType == CAMERATYPE_SIDE)
	{
		//���s���f�i���ˉe�j
		D3DXMatrixOrthoLH(
			&m_mtxProjection,
			(float)SCREEN_WIDTH / 2,
			(float)SCREEN_HEIGHT / 2,
			10.0f,
			2500.0f);
	}
	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//==========================
	//�J������h�炷����
	//==========================
	D3DXVECTOR3 adjust = D3DXVECTOR3(0.0f,0.0f,0.0f);//�h��̕␳
	D3DXVECTOR3 ResultPosV = m_PosV;//���ʓI�Ȏx�_
	D3DXVECTOR3 ResultPosR = m_PosR;//���ʓI�Ȓ����_
	if (m_nShakeFrame > 0)
	{
		m_nShakeFrame--;
		adjust.x += (float)(rand() % 100 - 50) * m_fShakePower;
		adjust.y += (float)(rand() % 100 - 50) * m_fShakePower;

		ResultPosV += adjust;
		ResultPosR += adjust;
	}
	//========================================================================================
	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&ResultPosV,
		&ResultPosR,
		&m_VecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW,&m_mtxView);

}
//====================================================================================================

//====================================================================
//�J�����ݒ菈��
//====================================================================
void CCamera::SetShake(int nShakeFrame, float fShakePower)
{
	m_nShakeFrame = nShakeFrame;
	m_fShakePower = fShakePower;
}
//====================================================================================================

//====================================================================
//�J�����̏�Ԃ�ς���
//====================================================================
void CCamera::ChengeState(CCameraState* pCameraState)
{
	delete m_pCameraState;
	m_pCameraState = pCameraState;
}
//====================================================================================================

//====================================================================
//���ʂ̃J�����̈ʒu��ݒ肵������
//====================================================================
void CCamera::NormalCameraMove()
{
	CObject* pManagerObject = nullptr;
	D3DXVECTOR3 RotVec = CCalculation::RadToVec(m_Rot);
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		m_PosR = CTitle::GetPlayer()->GetPosInfo().GetSenterPos() + D3DXVECTOR3(0.0f,50.0f,0.0f);
		m_PosV = m_PosR + RotVec * m_fLength;
		break;
	case CScene::MODE_GAME:
		if (CGame::GetPlayer() != nullptr)
		{
			m_PosR = CGame::GetPlayer()->GetPosInfo().GetPos() + D3DXVECTOR3(sinf(m_Rot.y + D3DX_PI * 0.5f) * 30.0f, 60.0f,cosf(m_Rot.y + D3DX_PI * 0.5f) * 30.0f) + m_AddPosR;
			m_PosV = m_PosR + RotVec * m_fLength;
		}
		break;
	case CScene::MODE_EDIT:
		m_PosV = m_PosR + RotVec * (m_fLength);
		break;
	default:
		break;
	}
}
//====================================================================================================

//====================================================================
//�������߂��I�u�W�F�N�g�𓧖��ɂ��鏈��
//====================================================================
void CCamera::MakeTransparent()
{
	D3DXVECTOR3 Ray = m_PosR - m_PosV;
	D3DXVec3Normalize(&Ray, &Ray);
	D3DXVECTOR3 RayCollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fLength = 0.0f;//���C�����������ꏊ�̋���
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = CObject::GetTopObject(nCntPri);

		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();
			if (pObj->GetObjectType() == CObject::OBJECTTYPE::OBJECTTYPE_X && pObj->GetType() != CObject::TYPE::PLAYER && pObj->GetType() != CObject::TYPE::ENEMY)
			{
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�̋@�\���g��
				if (CCollision::RayIntersectsAABBCollisionPos(m_PosV, Ray, pObjX->GetPosInfo().GetPos() + pObjX->GetSizeInfo().GetVtxMin(), pObjX->GetPosInfo().GetPos() + pObjX->GetSizeInfo().GetVtxMax(), RayCollisionPos))
				{
					fLength = CCalculation::CalculationLength(m_PosV,RayCollisionPos);

					if (m_fLength > fLength)
					{//�J�����̋��������I�u�W�F�N�g�ƃ��C�����������ʒu���߂��ꍇ�͓����ɂ���
						pObjX->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f), 3, true, true);
					}
				}
			}

			pObj = pNext;//���X�g��i�߂�
		}
	}
}
//====================================================================================================

//====================================================================
//�{�X��|�����Ƃ��̃J�����̏������s��
//====================================================================
void CCamera::BossDefeatCameraProcess()
{

}
//====================================================================================================

//====================================================================
//�J�����̐��񏈗����s��
//====================================================================
void CCamera::TurningCameraProcess()
{
	////�������
	//m_Rot.y += m_fTurningRotSpeed;

	////�x�_Y�̈ʒu
	//m_PosV.y += m_fTurningSpeedY;

	////�������X�V����
	//m_fLength += m_fAddLength;

	////�J�����̐��񏈗�
	//m_PosV.x = sinf(m_Rot.y) * m_fLength + m_PosR.x;
	//m_PosV.z = cosf(m_Rot.y) * m_fLength + m_PosR.z;
}
//===================================================================================================================================================

//���ȍ~�X�e�[�g�N���X

//****************************************************************************************************
//�J����������������ԃN���X
//****************************************************************************************************

//=================================================================
//�R���X�g���N�^
//=================================================================
CCameraState_TurnAround::CCameraState_TurnAround(D3DXVECTOR3 AimRot, float fAdjustTurnSpeed) : m_AimRot(AimRot),m_fAdjustTurnSpeed(fAdjustTurnSpeed)
{

}
//===================================================================================================================================================

//=================================================================
//�f�X�g���N�^
//=================================================================
CCameraState_TurnAround::~CCameraState_TurnAround()
{

}
//===================================================================================================================================================

//=================================================================
//����
//=================================================================
void CCameraState_TurnAround::Process(CCamera* pCamera)
{
	const D3DXVECTOR3 & NowRot = pCamera->GetRot();

	//�����̍��������߂�
	float fRotDiffX = m_AimRot.x - NowRot.x;
	float fRotDiffY = m_AimRot.y - NowRot.y;

	fRotDiffX = CCalculation::CorrectionRot(fRotDiffX);
	fRotDiffY = CCalculation::CorrectionRot(fRotDiffY);

	CManager::GetDebugProc()->PrintDebugProc("�J�����̌����̍���:%f\n");

	//���Z��������̗ʂ�␳���ċ��߂�
	float fAddRotX = fRotDiffX * m_fAdjustTurnSpeed;
	float fAddRotY = fRotDiffY * m_fAdjustTurnSpeed;
	fAddRotX = CCalculation::CorrectionRot(fAddRotX);
	fAddRotY = CCalculation::CorrectionRot(fAddRotY);
	pCamera->SetRot(D3DXVECTOR3(pCamera->GetRot().x + fAddRotX, pCamera->GetRot().y + fAddRotY, pCamera->GetRot().z));

	//�����̐�Βl��0.01f�ȉ��Ȃ�
	if (fabsf(fAddRotX) < 0.005f && fabsf(fAddRotY) < 0.005f)//Yaw��Pitch�̌������Z�ʂ�0.01f�ȉ��ɂȂ�����X�e�[�g��߂�
	{
		pCamera->ChengeState(DBG_NEW CCameraState_Normal());
	}
}
//===================================================================================================================================================

//****************************************************************************************************
//���ʂ̏��
//****************************************************************************************************

//=================================================================
//�ÓI�����o�錾
//=================================================================
const float CCameraState_Normal::s_fNORMAL_AROUNDROTSPEED = 0.02f;//�J�����̒ʏ��]���x
//===================================================================================================================================================


//=================================================================
//�R���X�g���N�^
//=================================================================
CCameraState_Normal::CCameraState_Normal()
{

}
//===================================================================================================================================================

//=================================================================
//�f�X�g���N�^
//=================================================================
CCameraState_Normal::~CCameraState_Normal()
{

}
//===================================================================================================================================================

//=================================================================
//����
//=================================================================
void CCameraState_Normal::Process(CCamera* pCamera)
{
	//========================================
    //�J�����̌�����ς���
    //========================================
	if (CManager::GetInputKeyboard()->GetPress(DIK_Q))
	{
		pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, -s_fNORMAL_AROUNDROTSPEED, 0.0f));
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_E))
	{
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT))
		{
			pCamera->SetRot(D3DXVECTOR3(pCamera->GetRot().x,0.0f,pCamera->GetRot().z));
		}
		else
		{
			pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, s_fNORMAL_AROUNDROTSPEED, 0.0f));
		}
	}

	if (CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LB))
	{
		pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, -s_fNORMAL_AROUNDROTSPEED, 0.0f));
	}
	if (CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RB))
	{
		pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, s_fNORMAL_AROUNDROTSPEED, 0.0f));
	}

	//===========================
	//X�{�^���������Ă�����
	//===========================
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//�V�t�g�L�[�������Ȃ���E�E�E
		if (CManager::GetInputKeyboard()->GetPress(DIK_X) == true)
		{
			pCamera->SetAddPosR(pCamera->GetAddPosR() + D3DXVECTOR3(0.0f, -5.0f, 0.0f));
		}
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_X) == true)
	{
		pCamera->SetAddPosR(pCamera->GetAddPosR() + D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	}

	//=================================================
	//�Q�[���p�b�h�A���̓}�E�X�ŃJ�����̌�����ς���
	//=================================================
	if (CManager::GetInputJoypad()->GetRStickPress(16) == true)
	{
		if (CScene::GetMode() == CScene::MODE::MODE_GAME)
		{
			CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::CAMERACONTROLL);
		}

		CManager::GetCamera()->SetRot(pCamera->GetRot() + D3DXVECTOR3(cosf(CManager::GetInputJoypad()->GetRStickAimRot() + D3DX_PI) * 0.04f,
			sinf(CManager::GetInputJoypad()->GetRStickAimRot()) * 0.04f, 0.0f));
	}
	float fAngle = 0.0f;
	if (CManager::GetInputMouse()->GetMouseMoveAngle(fAngle))
	{
		if (CScene::GetMode() == CScene::MODE::MODE_GAME)
		{
			CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::CAMERACONTROLL);
		}
		CManager::GetCamera()->SetRot(pCamera->GetRot() + D3DXVECTOR3(cosf(fAngle) * 0.06f,
			sinf(fAngle) * 0.06f, 0.0f));
	}

	//===========================
	//C�{�^���������Ă�����
	//===========================
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//�V�t�g�L�[�������Ȃ���E�E�E
		if (CManager::GetInputKeyboard()->GetPress(DIK_C) == true)
		{
			pCamera->SetAddPosV(pCamera->GetAddPosV() + D3DXVECTOR3(0.0f, -5.0f, 0.0f));
		}
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_C) == true)
	{
		pCamera->SetAddPosV(pCamera->GetAddPosV() + D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	}

	//===========================
	//V�{�^���������Ă�����
	//===========================
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//�V�t�g�L�[�������Ȃ���E�E�E
		if (CManager::GetInputKeyboard()->GetPress(DIK_V) == true)
		{
			pCamera->SetAddLength(pCamera->GetAddLength() - 5.0f);
		}
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_V) == true)
	{
		pCamera->SetAddLength(pCamera->GetAddLength() + 5.0f);
	}


	//========================================
	//�J������h�炷
	//========================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F1) == true)
	{
		pCamera->SetShake(50, 0.2f);
	}

	//========================================
	//��������
	//========================================
	if (CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::Y))
	{
		if (CScene::GetMode() == CScene::MODE::MODE_GAME)
		{
			CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::TURNAROUND);
		}
		pCamera->ChengeState(DBG_NEW CCameraState_TurnAround(pCamera->GetRot() + D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 0.15f));
	}

}
//===================================================================================================================================================