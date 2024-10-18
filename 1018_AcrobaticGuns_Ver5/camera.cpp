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
CCamera::CCamera() : m_SupportPos(NULL_VECTOR3),m_fLength(0.0f), m_fTurningRotSpeed(0.0f),m_fTurningSpeedY(0.0f),m_PosV(NULL_VECTOR3),
m_PosR(NULL_VECTOR3),m_VecU(NULL_VECTOR3),m_Rot(NULL_VECTOR3),m_mtxProjection(),m_mtxView(),m_CameraType(CAMERATYPE_BIRD),m_DifferenceLength(NULL_VECTOR3),
m_ZoomSpeed(NULL_VECTOR3),m_nShakeFrame(0),m_ModeTime(0),m_fShakePower(0.0f),m_fAddLength(0.0f)
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
	m_PosR = NULL_VECTOR3;                          //�����_
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);         //������x�N�g���i�@���j
	m_Rot = NULL_VECTOR3;                           //����
	m_mtxProjection = {};                           //�v���W�F�N�V�����}�g���b�N�X
	m_mtxView = {};                                 //�r���[�}�g���b�N�X
	m_CameraType = CAMERATYPE_BIRD;                 //�J�������[�h�̎��
	m_nShakeFrame = 0;                              //�J������h�炷�t���[����
	m_fShakePower = 0.0f;                           //�J������h�炷��
	m_DifferenceLength = NULL_VECTOR3;              //�ΏۂƂ̋���
	m_ModeTime = 0;                                 //���[�h�̎���
	m_ZoomSpeed = NULL_VECTOR3;                     //�Y�[�����鑬��
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
	//�J�����̈ʒu��ς���
	//========================================
	if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true)
	{
		m_Rot.y += 0.02f;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true)
	{
		m_Rot.y -= 0.02f;
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
			20000.0f);
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
	D3DXVECTOR3 adjust = NULL_VECTOR3;//�h��̕␳
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
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		switch (m_CameraType)
		{
		case CAMERATYPE_BIRD:
			if (CGame::GetPlayer() != nullptr)
			{
				m_PosR = CGame::GetPlayer()->GetPos();
				m_PosV = m_PosR + D3DXVECTOR3(sinf(m_Rot.y) * -300.0f, 300.0f, cosf(m_Rot.y) * -300.0f);
			}
			break;
		default:
			break;
		}
	}
}
//====================================================================================================

//====================================================================
//�{�X��|�����Ƃ��̃J�����̏������s��
//====================================================================
void CCamera::BossDefeatCameraProcess()
{
	float fVXaim = 0.0f;//X�����x�N�g��
	float fVYaim = 0.0f;//Y�����x�N�g��
	float fVZaim = 0.0f;//Z�����x�N�g��
	float fVLaim = 0.0f;//�����x�N�g��
	float fLength = 0.0f;
	CObject* pObj = nullptr;
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;

	m_ModeTime++;
}
//====================================================================================================

//====================================================================
//�J�����̐��񏈗����s��
//====================================================================
void CCamera::TurningCameraProcess()
{
	//�������
	m_Rot.y += m_fTurningRotSpeed;

	//�x�_Y�̈ʒu
	m_PosV.y += m_fTurningSpeedY;

	//�������X�V����
	m_fLength += m_fAddLength;

	//�J�����̐��񏈗�
	m_PosV.x = sinf(m_Rot.y) * m_fLength + m_PosR.x;
	m_PosV.z = cosf(m_Rot.y) * m_fLength + m_PosR.z;
}
//====================================================================================================