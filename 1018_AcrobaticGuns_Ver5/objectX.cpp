//===========================================================================================
//
//�U���S���F���f����`�悷��[objectX.cpp]
//Author:ShinaTaiyo
//
//===========================================================================================

//===============================
//�C���N���[�h
//===============================
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "objectX.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "debugproc.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
//===========================================================================================

//================================================
//�R���X�g���N�^
//================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority), m_bUseDraw(true),m_ObjectXInfo(), m_bAutoSubLife(false),
m_bColorChenge(false),m_bHitStop(false), m_bIsJumping(false), m_bIsLanding(false), m_bIsWalling(false),m_bUseAddRot(false), m_bUseAddScaling(false),
m_bUseAddSpeed(false),m_bUseGravity(false), m_bUseInteria(false), m_bUseMultiSpeed(false),m_bUseRatioLifeAlpha(false),
m_bUseShadow(false), m_bUseUpdatePos(false), m_fGravityPower(0.0f), m_fInertia(0.0f), m_nChengeColorTime(0), m_nHitStopTime(0), m_nLife(0), m_nMaxLife(0), m_nManagerType(0),m_nObjXType(OBJECTXTYPE_BLOCK), m_nTypeNum(0), m_bUseMultiScale(false), m_MultiScale(NULL_VECTOR3),
m_bUseCulling(false), m_Pos(NULL_VECTOR3), m_SupportPos(NULL_VECTOR3),m_PosOld(NULL_VECTOR3),m_Rot(NULL_VECTOR3),m_Scale(NULL_VECTOR3),m_FormarScale(NULL_VECTOR3),
m_Size(NULL_VECTOR3),m_VtxMin(NULL_VECTOR3),m_OriginVtxMin(NULL_VECTOR3),m_VtxMax(NULL_VECTOR3),m_OriginVtxMax(NULL_VECTOR3),m_Move(NULL_VECTOR3),m_mtxWorld(),
m_AddRot(NULL_VECTOR3),m_SenterPos(NULL_VECTOR3), m_MultiSpeed(NULL_VECTOR3), m_AddSpeed(NULL_VECTOR3), m_AddScale(NULL_VECTOR3)
{

}
//================================================================================================================================================

//================================================
//�f�X�g���N�^
//================================================
CObjectX::~CObjectX()
{

}
//================================================================================================================================================

//================================================
//����������
//================================================
HRESULT CObjectX::Init()
{
	//==========================
	//�ϐ��̏�����
	//==========================
	m_mtxWorld = {};                                   //���[���h�}�g���b�N�X
	m_fInertia = m_fNORMAL_INERTIA;                    //����
	m_bUseInteria = true;                              //�����������邩�ǂ���
	//==================================================================================

	//===========================================
	//�̗͊֌W
	//===========================================
	m_nLife = 1;                                        //�̗�
	m_nMaxLife = 1;                                     //�ő�̗�
	//==================================================================================

	//===========================================
	//�e�֌W
	//===========================================
	m_bUseShadow = true;
	//==================================================================================

	//===========================================
	//�d�͊֌W
	//===========================================
	m_bUseGravity = true;         //�d�͂��g�p���邩�ǂ���
	//==================================================================================

	//===========================================
	//�ʒu�X�V�֌W
	//===========================================
	m_bUseUpdatePos = true;//�ʒu�̍X�V���g�p���邩�ǂ���
	//==================================================================================
	m_bUseGravity = false;                    //�d�͂��g�p���邩�ǂ����@
	m_fGravityPower = 0.0f;                   //�d�͂̑傫��

	m_bUseAddScaling = false;                 //�g�嗦�̉��Z���g�p���邩�ǂ���
	m_AddScale = NULL_VECTOR3;                //�g�嗦�̉��Z��    

	return S_OK;
}
//================================================================================================================================================

//================================================
//�I������
//================================================
void CObjectX::Uninit()
{
	if (m_ObjectXInfo.Diffuse != nullptr)
	{
		delete[] m_ObjectXInfo.Diffuse;//�F�����̓��I���������J��
		m_ObjectXInfo.Diffuse = nullptr;
	}

	if (m_ObjectXInfo.pTexture != nullptr)
	{
		delete[] m_ObjectXInfo.pTexture;//�e�N�X�`���̓��I�����������
		m_ObjectXInfo.pTexture = nullptr;
	}

	if (m_ObjectXInfo.FormarDiffuse != nullptr)
	{
		delete[] m_ObjectXInfo.FormarDiffuse;//�F�����̓��I���������J��
		m_ObjectXInfo.FormarDiffuse = nullptr;
	}

	if (m_ObjectXInfo.pMesh != nullptr)
	{//���b�V���ւ̃|�C���^��nullptr��
		m_ObjectXInfo.pMesh = nullptr;
	}

	if (m_ObjectXInfo.pBuffMat != nullptr)
	{//�}�e���A���ւ̃|�C���^��nullptr��
		m_ObjectXInfo.pBuffMat = nullptr;
	}
}
//================================================================================================================================================

//================================================
//�X�V����
//================================================
void CObjectX::Update()
{
	//==============================================
	//���S�_�����߂�
	//==============================================
	CalculateSenterPos();

	//==============================================
	//�̗͂����炷
	//==============================================
	if (m_bAutoSubLife == true)
	{
		m_nLife--;
	}
	//===========================================================

	//==============================================
	//�̗͂̊����ŐF�����̓����x��ς���
	//==============================================
	if (m_bUseRatioLifeAlpha == true)
	{
		float fRatio;
		fRatio = float(m_nLife) / float(m_nMaxLife);
	}
	//===========================================================

	//==================================
	//��Z�����x��ON�ɂȂ��Ă�����
	//==================================
	if (m_bUseMultiSpeed == true)
	{
		m_Move.x *= m_MultiSpeed.x;
		m_Move.y *= m_MultiSpeed.y;
		m_Move.z *= m_MultiSpeed.z;
	}
	//==========================================================

	//==================================
	//������ON�ɂȂ��Ă�����
	//==================================
	if (m_bUseAddSpeed == true)
	{
		m_Move += m_AddSpeed;
	}
	//==========================================================

	//==================================
	//�g�嗦�̉��Z��ON�ɂȂ��Ă�����
	//==================================
	if (m_bUseAddScaling == true)
	{
		m_Scale += m_AddScale;
	}
	//==========================================================

	//==================================
	//�g�嗦�̏�Z��ON�ɂȂ��Ă�����
	//==================================
	if (m_bUseMultiScale == true)
	{
		m_Scale.x *= m_MultiScale.x;
		m_Scale.y *= m_MultiScale.y;
		m_Scale.z *= m_MultiScale.z;
	}
	//==========================================================

	//==============================================
    //�q�b�g�X�g�b�v����
    //==============================================
	HitStopProcess();
	//===========================================================


	//��Ɋg�嗦���Q�Ƃ��čő�ŏ����_�����߂�
	m_VtxMax.x = m_OriginVtxMax.x * m_Scale.x;
	m_VtxMax.y = m_OriginVtxMax.y * m_Scale.y;
	m_VtxMax.z = m_OriginVtxMax.z * m_Scale.z;
	m_VtxMin.x = m_OriginVtxMin.x * m_Scale.x;
	m_VtxMin.y = m_OriginVtxMin.y * m_Scale.y;
	m_VtxMin.z = m_OriginVtxMin.z * m_Scale.z;

	m_Size = m_VtxMax - m_VtxMin;

	if (m_bColorChenge == true)
	{
		m_nChengeColorTime--;
	}

	if (m_nChengeColorTime <= 0 && m_bColorChenge == true )
	{
		m_nChengeColorTime = 0;
		SetFormarColor();//���̐F�����ɖ߂�
		m_bColorChenge = false;
	}

	if (m_bUseUpdatePos == true)
	{//�ʒu�X�V����
		UpdatePos();
	}

	if (m_bUseAddRot == true)
	{//�����̉��Z����
		m_Rot += m_AddRot;
	}

	if (GetStageManagerChoose() == true)
	{//�I�u�W�F�N�gX�̏���\��
	    DispInfo();
	}
}
//================================================================================================================================================

//================================================
//�`�揈��
//================================================
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;                  //�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;                                              //���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;                                               //�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_ObjectXInfo.Diffuse[0].a > 0.0f && m_Pos.y + m_VtxMax.y >= 0.0f && m_bUseShadow == true)
	{
		//�e�̕`��
		DrawShadow();
	}
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//=======================================
	//�`��̒���
	//=======================================
	//�A���t�@�e�X�g��L��(�A���t�@�l���O���傫���ꍇ�ɕ`�悷��j
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	if (m_bUseCulling == true)
	{
		//���ʂ�`�悷��
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
	//================================================================================================================

	//�}�e���A���ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_ObjectXInfo.pBuffMat->GetBufferPointer();

	if (m_bUseDraw == true)
	{
		//==========================================================================
		//�}�e���A���̐����A�e�N�X�`����ǂݍ��ށB
		//==========================================================================
		for (int nCntMat = 0; nCntMat < (int)m_ObjectXInfo.dwNumMat; nCntMat++)
		{
			//�F�����̐ݒ�
			pMat[nCntMat].MatD3D.Diffuse = m_ObjectXInfo.Diffuse[nCntMat];

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`����ݒ肷��
			pDevice->SetTexture(0, m_ObjectXInfo.pTexture[nCntMat]);

			//���f���i�p�[�c�j�̕`��
			m_ObjectXInfo.pMesh->DrawSubset(nCntMat);
		}
		//================================================================================================================
	}
	
	//=======================================
	//�`��̒��������ɖ߂�
	//=======================================
	//�A���t�@�e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);


	//�Жʂ����`�悷��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//================================================================================================================

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

}
//================================================================================================================================================

//==========================================================================
//�g�嗦�̉��Z��ݒ�
//==========================================================================
void CObjectX::SetUseAddScale(D3DXVECTOR3 AddScale, bool bUse)
{
	m_bUseAddScaling = bUse;
	m_AddScale = AddScale;
}
//===================================================================================================================================================

//================================================
//���f���������蓖�Ă�
//================================================
void CObjectX::BindObjectXInfo(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPDIRECT3DTEXTURE9* pTexture, D3DCOLORVALUE* pDiffuse)
{
	m_ObjectXInfo.pMesh = pMesh;
	m_ObjectXInfo.pBuffMat = pBuffMat;
	m_ObjectXInfo.dwNumMat = dwNumMat;
	m_ObjectXInfo.pTexture = DBG_NEW LPDIRECT3DTEXTURE9[int(dwNumMat)];
	m_ObjectXInfo.Diffuse = DBG_NEW D3DCOLORVALUE[int(dwNumMat)];
	m_ObjectXInfo.FormarDiffuse = DBG_NEW D3DCOLORVALUE[int(dwNumMat)];
	for (int nCnt = 0; nCnt < (int)(dwNumMat); nCnt++)
	{
		m_ObjectXInfo.pTexture[nCnt] = pTexture[nCnt];
		m_ObjectXInfo.Diffuse[nCnt] = pDiffuse[nCnt];
		m_ObjectXInfo.FormarDiffuse[nCnt] = pDiffuse[nCnt];
	}
}
//================================================================================================================================================

//================================================
//�F������ݒ肷��
//================================================
void CObjectX::SetColor(D3DXCOLOR col, int nColChengeTime, bool bChoose, bool bSetAlpha)
{
	m_bColorChenge = true;
	m_nChengeColorTime = nColChengeTime;

	if (bChoose == true)
	{
		if (bSetAlpha == false)
		{
			for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
			{
				m_ObjectXInfo.Diffuse[nCnt].r = col.r;
				m_ObjectXInfo.Diffuse[nCnt].g = col.g;
				m_ObjectXInfo.Diffuse[nCnt].b = col.b;
			}
		}
		else
		{
			for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
			{
				m_ObjectXInfo.Diffuse[nCnt].a = col.a;
			}
		}
	}
	else
	{
		for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
		{
			m_ObjectXInfo.Diffuse[nCnt] = col;
		}
	}
}
//================================================================================================================================================

//================================================
//���S�_�����߂�
//================================================
void CObjectX::CalculateSenterPos()
{
	m_SenterPos = m_Pos;
	m_SenterPos.y += (m_VtxMax.y + m_VtxMin.y) / 2;
}
//================================================================================================================================================

//================================================
//���̐F�����ɖ߂�
//================================================
void CObjectX::SetFormarColor()
{
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{
		m_ObjectXInfo.Diffuse[nCnt] = m_ObjectXInfo.FormarDiffuse[nCnt];
	}
}
//================================================================================================================================================

//================================================
//�q�b�g�X�g�b�v�̏���
//================================================
void CObjectX::HitStopProcess()
{
	if (m_bHitStop == true)
	{
		if (m_nHitStopTime > 0)
		{
			m_nHitStopTime--;
		}
		else
		{//�q�b�g�X�g�b�v����
			m_bHitStop = false;
			m_nHitStopTime = 0;
		}
	}
}
//================================================================================================================================================

//================================================
//�T�C�Y��ݒ肷��
//================================================
void CObjectX::SetSize()
{
	BYTE* pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	int nNumVtx = m_ObjectXInfo.pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	DWORD sizeFVF = D3DXGetFVFVertexSize(m_ObjectXInfo.pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
    m_ObjectXInfo.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//====================================================
		//���_���W���r���ă��f���̍ŏ��l�ő�l���擾
		//====================================================
		if (vtx.x > m_OriginVtxMax.x)
		{
			m_OriginVtxMax.x = vtx.x;
		}
		else if (vtx.y > m_OriginVtxMax.y)
		{//����ǂݍ��񂾒��_���A��ԑ傫�����_���傫���ꍇ
			m_OriginVtxMax.y = vtx.y;
		}
		else if (vtx.z > m_OriginVtxMax.z)
		{//����ǂݍ��񂾒��_���A��ԑ傫�����_���傫���ꍇ
			m_OriginVtxMax.z = vtx.z;
		}
		else if (vtx.x < m_OriginVtxMin.x)
		{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
			m_OriginVtxMin.x = vtx.x;
		}
		else if (vtx.y < m_OriginVtxMin.y)
		{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
			m_OriginVtxMin.y = vtx.y;
		}
		else if (vtx.z < m_OriginVtxMin.z)
		{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
			m_OriginVtxMin.z = vtx.z;
		}
		//=============================================================================================================

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;

	}
	//���_�o�b�t�@�̃A�����b�N
	m_ObjectXInfo.pMesh->UnlockVertexBuffer();

	//==========================================================
	//��̏����ŏo�����ŏ��ő�̒��_���g�嗦�Ōv�Z������
	//==========================================================
	m_VtxMax.x = m_OriginVtxMax.x * m_Scale.x;
	m_VtxMax.y = m_OriginVtxMax.y * m_Scale.y;
	m_VtxMax.z = m_OriginVtxMax.z * m_Scale.z;
	m_VtxMin.x = m_OriginVtxMin.x * m_Scale.x;
	m_VtxMin.y = m_OriginVtxMin.y * m_Scale.y;
	m_VtxMin.z = m_OriginVtxMin.z * m_Scale.z;
	//================================================================================================================================================

	//==========================================================
	//�ő�ŏ����Q�Ƃ��ăT�C�Y��ݒ肷��
	//==========================================================
	m_Size = m_VtxMax - m_VtxMin;
	//================================================================================================================================================

}
//================================================================================================================================================

//===================================================================================================================
//�ʒu���X�V������i�ʒu���X�V������Ƀu���b�N�Ƃ̓����蔻����������̂ŁA�C�ӂ̃^�C�~���O�ŌĂׂ�悤�ɂ���j
//===================================================================================================================
void CObjectX::UpdatePos()
{
	m_PosOld = m_Pos;   
	m_Pos += m_Move;   //�ʒu�̍X�V
	if (m_bUseInteria == true)
	{
		m_Move.x += (0.0f - m_Move.x) * m_fInertia;
		m_Move.z += (0.0f - m_Move.z) * m_fInertia;
	}
}
//================================================================================================================================================

//===================================================================================================================
//�G�f�B�^����g�嗦��ύX����
//===================================================================================================================
void CObjectX::ChengeEditScale()
{
	D3DXVECTOR3& Scale = GetScale();//�g�嗦

	ChengeEditScaleX();

	ChengeEditScaleY();

	ChengeEditScaleZ();

	CManager::GetDebugProc()->PrintDebugProc("�g�嗦(YUI�L�[) %f %f %f\n", Scale.x,Scale.y,Scale.z);
}
//================================================================================================================================================

//===================================================================================================================
//�G�f�B�^����g�嗦X��ύX����
//===================================================================================================================
void CObjectX::ChengeEditScaleX()
{
	D3DXVECTOR3& Scale = GetScale();                                //�g�嗦

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
		{
			Scale.x += 0.1f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
		{
			Scale.x += 0.1f;
		}
	}

}
//================================================================================================================================================

//===================================================================================================================
//�G�f�B�^����g�嗦Y��ύX����
//===================================================================================================================
void CObjectX::ChengeEditScaleY()
{
	D3DXVECTOR3& Scale = GetScale();                                //�g�嗦

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
		{
			Scale.y += 0.1f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{
			Scale.y += 0.1f;
		}
	}
}
//================================================================================================================================================

//===================================================================================================================
//�G�f�B�^����g�嗦Z��ύX����
//===================================================================================================================
void CObjectX::ChengeEditScaleZ()
{
	D3DXVECTOR3& Scale = GetScale();                                //�g�嗦
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
		{
			Scale.z += 0.1f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
		{
			Scale.z += 0.1f;
		}
	}

}
//================================================================================================================================================

//===================================================================================================================
//�ʒu���ړ�������
//===================================================================================================================
void CObjectX::ChengeEditPos()
{
	D3DXVECTOR3& Pos = GetPos();                                    //���f���̈ʒu�̎擾
	D3DXVECTOR3& SupportPos = GetSupportPos();                      //���f���̎x�_�ʒu�̎擾
	D3DXVECTOR3& Rot = GetRot();                                    //���f���̌����̎擾
	D3DXVECTOR3 Size = GetSize();                                   //�T�C�Y���擾
	float fMoveX = 0.0f;                                            //X�����̈ړ���
	float fMoveZ = 0.0f;                                            //Z�����̈ړ���
	bool bMove = false;                                             //�ړ����Ă��邩�ǂ��� 
	 
	SetColor(D3DXCOLOR(1.0f,0.0f,0.0f,0.5f),3,true,true);           //�F�𔼓����ɂ���

	//�ʒu���x�_�ɌŒ�
	Pos = SupportPos;
	//=====================================================
    //�ړ�����
    //=====================================================
   	if (CManager::GetInputKeyboard()->GetPress(DIK_UP) == true)
	{
		fMoveZ = 1.0f;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_DOWN) == true)
	{
		fMoveZ = -1.0f;
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_RIGHT) == true)
	{
		fMoveX = 1.0f;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_LEFT) == true)
	{
		fMoveX = -1.0f;
	}

	//============================
	// �ړ��{�^���������Ă�����
	//============================
	if (fMoveX != 0.0f || fMoveZ != 0.0f)
	{
		bMove = true;//�ړ����
	}
	else
	{
		bMove = false;//�ҋ@���
	}
	if (bMove == true)
	{//�ړ����Ă�����
		Pos.x += sinf(atan2f(fMoveX, fMoveZ)) * 5.0f;
		Pos.y += cosf(atan2f(fMoveX, fMoveZ)) * 5.0f;
	}

	//===========================
	//RT�{�^���������Ă�����
	//===========================
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_F) == true)
			{
				Rot.z -= 0.01f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_G) == true)
		{
			Rot.z += 0.01f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_F) == true)
			{
				Rot.z -= 0.01f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_G) == true)
		{
			Rot.z += 0.01f;
		}
	}

	//�x�_���ꏏ�Ɉړ�
	SupportPos = Pos;
	CManager::GetDebugProc()->PrintDebugProc("�x�_�ʒu(���L�[) %f %f %f\n", SupportPos.x, SupportPos.y, SupportPos.z);
	CManager::GetDebugProc()->PrintDebugProc("����Z(FG�L�[) %f\n",Rot.z);
	//================================================================================================================================================

}
//================================================================================================================================================

//============================================================================
//�_���[�W��^����
//============================================================================
void CObjectX::SetDamage(int nDamage, int nHitStopTime)
{
	if (m_bHitStop == false && nDamage > 0)
	{//�q�b�g�X�g�b�v��Ԃ���Ȃ����
		m_bHitStop = true;              //�q�b�g�X�g�b�v��Ԃɂ���
		m_nHitStopTime = nHitStopTime;  //�q�b�g�X�g�b�v����
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			D3DXVECTOR3 Pos = CObjectX::GetPos();//�ʒu���擾
			//CParticle::Create(50, 30.0f, 30.0f, Pos, D3DXVECTOR3(sinf(fRandRot) * fRandSpeed, cosf(fRandRot) * fRandSpeed, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		m_nLife -= nDamage;
	}
}
//================================================================================================================================================

//============================================================================
//�񕜗ʂ����蓖�Ă�
//============================================================================
void CObjectX::SetHeal(int nHeal, D3DXCOLOR col, float fWidth, float fHeight)
{
	m_nLife += nHeal;
	//CDamage::Create(nHeal, m_Pos, col, fWidth, fHeight);
}
//================================================================================================================================================

//============================================================================
//�_�����}�e���A���ԍ��̃e�N�X�`�������蓖�Ă�
//============================================================================
void CObjectX::SetLockOnMatBindTexture(int nNumMat, LPDIRECT3DTEXTURE9 pTexture)
{
	m_ObjectXInfo.pTexture[nNumMat] = pTexture;
}
//================================================================================================================================================

//============================================================================
//�e��`�悷��
//============================================================================
void CObjectX::DrawShadow()
{
	D3DXMATRIX mtxShadow,mtxScale,mtxRot,mtxTrans;                  //�v�Z�p�}�g���b�N�X
	D3DXPLANE plane;                       //�v���[��
	D3DXVECTOR4 vecLight;                  //���C�g�t����
	D3DXVECTOR3 ShadowPos, ShadowNor;      //���ʏ�̈�]��
	D3DXMATERIAL* pMat;                                               //�}�e���A���f�[�^�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DMATERIAL9 matDef;                                              //���݂̃}�e���A���ۑ��p

    //�e�̃}�g���b�N�X��������
	D3DXMatrixIdentity(&mtxShadow);

	//���C�g�̋t�����x�N�g����ݒ�
	vecLight = D3DXVECTOR4(-100.0f, 300.0f,300.0f, 0.0f);
	ShadowPos = NULL_VECTOR3;
	ShadowNor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�@���ƕ��ʏ�̈�_���畽�ʏ����쐬
	D3DXPlaneFromPointNormal(&plane, &ShadowPos, &ShadowNor);

	//���C�g�̕��ʏ�񂩂�e�s����쐬
	D3DXMatrixShadow(&mtxShadow, &vecLight, &plane);

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxScale);

	////�����𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot,m_Rot.y, m_Rot.x, m_Rot.z);
	//D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x,0.0f, m_Pos.z);
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//=======================================
    //�`��̒���
    //=======================================

    //�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	//================================================================================================================

	//�}�e���A���ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_ObjectXInfo.pBuffMat->GetBufferPointer();

	//==========================================================================
	//�}�e���A���̐����A�e�N�X�`����ǂݍ��ށB
	//==========================================================================
	for (int nCntMat = 0; nCntMat < (int)m_ObjectXInfo.dwNumMat; nCntMat++)
	{
		//�F�����̐ݒ�
		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`����ݒ肷��
		pDevice->SetTexture(0,NULL);

		//���f���i�p�[�c�j�̕`��
		m_ObjectXInfo.pMesh->DrawSubset(nCntMat);
	}
	//================================================================================================================

	//=======================================
	//�`��̒��������ɖ߂�
	//=======================================

	//�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	//================================================================================================================

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}
//================================================================================================================================================

//============================================================================
//�d�͂̏���
//============================================================================
void CObjectX::GravityProcess()
{
	if (m_bIsLanding == true)
	{
		m_Move.y = 0.0f;
	}
	if (m_bUseGravity == true)
	{
		m_Move.y += -m_fNORMAL_GRAVITY;
	}

}
//================================================================================================================================================

//============================================================================
//���\������
//============================================================================
void CObjectX::DispInfo()
{
	ChengeEditPos();//�ʒu��ς���

	ChengeEditScale();//�g�嗦��ς���
}
//================================================================================================================================================