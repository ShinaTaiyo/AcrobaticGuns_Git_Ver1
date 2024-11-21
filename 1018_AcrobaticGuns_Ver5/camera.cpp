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
#include "edit.h"
//====================================================================================================

//====================================================================
//�ÓI�����o�錾
//====================================================================
const float CCamera::m_BESIDECAMERALENGTH = 570.0f;//�r�T�C�h�r���[�̃J�����̋���
//====================================================================================================

//====================================================================
//�R���X�g���N�^
//====================================================================
CCamera::CCamera() : m_SupportPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_fLength(0.0f), m_fTurningRotSpeed(0.0f),m_fTurningSpeedY(0.0f),m_PosV(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_PosR(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_VecU(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_Rot(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_mtxProjection(),m_mtxView(),m_CameraType(CAMERATYPE_BIRD),m_DifferenceLength(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_ZoomSpeed(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_nShakeFrame(0),m_ModeTime(0),m_fShakePower(0.0f),m_fAddLength(0.0f),m_AddPosR(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_AddPosV(D3DXVECTOR3(0.0f,0.0f,0.0f))
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
	m_PosV = D3DXVECTOR3(0.0f, 200.0f,-400.0f);     //�x�_
	m_PosR = D3DXVECTOR3(0.0f,0.0f,0.0f);                          //�����_
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);         //������x�N�g���i�@���j
	m_Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);                           //����
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

}
//====================================================================================================


//====================================================================
//�X�V����
//====================================================================
void CCamera::Update()
{
	//========================================
	//�J�����̌�����ς���
	//========================================
	if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RB) == true)
	{
		m_Rot.y += 0.02f;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LB) == true)
	{
		m_Rot.y -= 0.02f;
	}

	//===========================
	//X�{�^���������Ă�����
	//===========================
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//�V�t�g�L�[�������Ȃ���E�E�E
		if (CManager::GetInputKeyboard()->GetPress(DIK_X) == true)
		{
			m_AddPosR.y -= 5.0f;
		}
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_X) == true)
	{
		m_AddPosR.y += 5.0f;
	}

	//===========================
    //C�{�^���������Ă�����
    //===========================
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//�V�t�g�L�[�������Ȃ���E�E�E
		if (CManager::GetInputKeyboard()->GetPress(DIK_C) == true)
		{
			m_AddPosV.y -= 5.0f;
		}
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_C) == true)
	{
		m_AddPosV.y += 5.0f;
	}



	//=========================o==============
	//�J������h�炷
	//========================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F1) == true)
	{
		SetShake(50, 0.2f);
	}

	//�J�����̒ʏ�̒����_��ݒ肵������
	NormalCameraMove();

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
//���ʂ̃J�����̈ʒu��ݒ肵������
//====================================================================
void CCamera::NormalCameraMove()
{
	CObject* pManagerObject = nullptr;
		switch (m_CameraType)
		{
		case CAMERATYPE_BIRD:
			switch (CScene::GetMode())
			{
			case CScene::MODE_GAME:
				if (CGame::GetPlayer() != nullptr)
				{

					m_PosR = CGame::GetPlayer()->GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f) + m_AddPosR;
					m_PosV = m_PosR + D3DXVECTOR3(sinf(m_Rot.y) * -200.0f, 0.0f, cosf(m_Rot.y) * -200.0f) + m_AddPosV;
					//m_PosV = m_PosR + D3DXVECTOR3(sinf(m_Rot.y) * -250.0f, 0.0f, cosf(m_Rot.y) * -250.0f); + m_AddPosV;

					//CParticle::SummonParticle(CParticle::TYPE00_NORMAL, 1, 30, 30.0f, 30.0f, 100, 10, false, m_PosR, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
				}
				break;
			case CScene::MODE_EDIT:
				pManagerObject = CEdit::GetStageManager()->GetStageManagerObject();
				if (pManagerObject != nullptr)
				{
					if (pManagerObject->GetObjectType() == CObject::OBJECTTYPE::OBJECTTYPE_X)
					{
						m_PosR = ((CObjectX*)pManagerObject)->GetPos() + m_AddPosR;
						m_PosV = m_PosR + D3DXVECTOR3(sinf(m_Rot.y) * -400.0f, 400.0f, cosf(m_Rot.y) * -400.0f) + m_AddPosV;
					}
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
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
//====================================================================================================