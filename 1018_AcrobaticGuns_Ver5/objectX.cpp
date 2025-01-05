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
#include "calculation.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "debugproc.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "collision.h"
#include "particle.h"
#include <algorithm>
//===========================================================================================

//================================================
//�ÓI�����o�錾
//================================================
bool CObjectX::s_bCOMMON_DRAWSHADOW = true;

//================================================
//�R���X�g���N�^
//================================================
CObjectX::CObjectX(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri, bUseintPri, type, ObjType)
, m_ObjectXInfo(), m_bUseAddRot(false),
m_bUseAddScaling(false), m_nIndexObjectX(0), m_nManagerType(0), m_nObjXType(OBJECTXTYPE_BLOCK),
m_nTypeNum(0), m_bUseMultiScale(false), m_MultiScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), m_PosInfo({}), m_Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), m_Scale(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), m_FormarScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), m_Size(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), m_VtxMin(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_OriginVtxMin(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), m_VtxMax(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), m_OriginVtxMax(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), m_AddRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_AddScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), m_fAxis(0.0f), m_VecAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f)), m_DrawInfo({})
{
	SetObjectType(CObject::OBJECTTYPE::OBJECTTYPE_X);
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
	m_bUseAddScaling = false;                 //�g�嗦�̉��Z���g�p���邩�ǂ���
	m_AddScale = D3DXVECTOR3(0.0f,0.0f,0.0f);                //�g�嗦�̉��Z��    

	CObject::Init();

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

	CObject::Uninit();
}
//================================================================================================================================================

//================================================
//�X�V����
//================================================
void CObjectX::Update()
{

	//m_fAxis += 0.01f;

	//==============================================
	//���S�_�����߂�
	//==============================================
	CalculateSenterPos();

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

	//��Ɋg�嗦���Q�Ƃ��čő�ŏ����_�����߂�
	if (m_bSwapVtxXZ == false)
	{
		m_VtxMax.x = m_OriginVtxMax.x * m_Scale.x;
		m_VtxMax.y = m_OriginVtxMax.y * m_Scale.y;
		m_VtxMax.z = m_OriginVtxMax.z * m_Scale.z;
		m_VtxMin.x = m_OriginVtxMin.x * m_Scale.x;
		m_VtxMin.y = m_OriginVtxMin.y * m_Scale.y;
		m_VtxMin.z = m_OriginVtxMin.z * m_Scale.z;
	}
	else
	{
		m_VtxMax.x = m_OriginVtxMax.x * m_Scale.z;
		m_VtxMax.y = m_OriginVtxMax.y * m_Scale.y;
		m_VtxMax.z = m_OriginVtxMax.z * m_Scale.x;
		m_VtxMin.x = m_OriginVtxMin.x * m_Scale.z;
		m_VtxMin.y = m_OriginVtxMin.y * m_Scale.y;
		m_VtxMin.z = m_OriginVtxMin.z * m_Scale.x;
	}
	m_Size = m_VtxMax - m_VtxMin;

	if (m_DrawInfo.bColorChenge == true)
	{
		m_DrawInfo.nChengeColorTime--;
	}

	if (m_DrawInfo.nChengeColorTime <= 0 && m_DrawInfo.bColorChenge == true )
	{
		m_DrawInfo.nChengeColorTime = 0;
		SetFormarColor();//���̐F�����ɖ߂�
		m_DrawInfo.bColorChenge = false;
	}

	if (m_bUseAddRot == true)
	{//�����̉��Z����
		m_Rot += m_AddRot;
	}

	CObject::Update();
}
//================================================================================================================================================

//================================================
//�`�揈��
//================================================
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;                            //�v�Z�p�}�g���b�N�X
	D3DXQUATERNION quat;                                              //�N�H�[�^�j�I��
	D3DXVECTOR3 vecAxis = D3DXVECTOR3(0.0f, 0.0, 1.0f);               //��]��
	D3DMATERIAL9 matDef;                                              //���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;                                               //�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	if (m_DrawInfo.bUseShadow == true && s_bCOMMON_DRAWSHADOW == true)
	{
		DrawShadow();
	}

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_DrawInfo.mtxWorld);

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_DrawInfo.mtxWorld, &m_DrawInfo.mtxWorld, &mtxScale);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_DrawInfo.mtxWorld, &m_DrawInfo.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_PosInfo.Pos.x, m_PosInfo.Pos.y, m_PosInfo.Pos.z);
	D3DXMatrixMultiply(&m_DrawInfo.mtxWorld, &m_DrawInfo.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_DrawInfo.mtxWorld);
	//=======================================
	//�`��̒���
	//=======================================
	//�A���t�@�e�X�g��L��(�A���t�@�l���O���傫���ꍇ�ɕ`�悷��j
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF,50);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	//�@���̒������P�ɂ���B�i�X�P�[���Ȃǂ��g�������́A�K���g���B)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	if (m_DrawInfo.Color.a < 1.0f)
	{
		//Z�o�b�t�@�ɏ������܂Ȃ�
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	if (m_DrawInfo.bUseCulling == true)
	{
		//���ʂ�`�悷��
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
	//================================================================================================================

	//�}�e���A���ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_ObjectXInfo.pBuffMat->GetBufferPointer();

	if (m_DrawInfo.bUseDraw == true)
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

	//Z�o�b�t�@�ɏ�������
    pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�Жʂ����`�悷��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//================================================================================================================

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	CObject::Draw();

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
	m_DrawInfo.bColorChenge = true;
	m_DrawInfo.nChengeColorTime = nColChengeTime;
	m_DrawInfo.Color = col;
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
	m_PosInfo.SenterPos = m_PosInfo.Pos;
	m_PosInfo.SenterPos.y += (m_VtxMax.y + m_VtxMin.y) / 2;
}
//================================================================================================================================================

//================================================
//���̐F�����ɖ߂�
//================================================
void CObjectX::SetFormarColor()
{
	m_DrawInfo.Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{
		m_ObjectXInfo.Diffuse[nCnt] = m_ObjectXInfo.FormarDiffuse[nCnt];
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
//�G�f�B�^����g�嗦��ύX����
//===================================================================================================================
void CObjectX::ChengeEditScale()
{
	D3DXVECTOR3& Scale = GetScale();//�g�嗦

	ChengeEditScaleX();

	ChengeEditScaleY();

	ChengeEditScaleZ();

	CManager::GetDebugProc()->PrintDebugProc("�g�嗦(RTY�L�[) %f %f %f\n", Scale.x,Scale.y,Scale.z);
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
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
			{
				Scale.x -= 10.0f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
		{
			Scale.x += 10.0f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
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
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
			{
				Scale.y -= 10.0f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
		{
			Scale.y += 10.0f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
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
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
			{
				Scale.z -= 10.0f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
		{
			Scale.z += 10.0f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
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
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	SetColor(D3DXCOLOR(1.0f,0.0f,0.0f,0.5f),3,true,true);           //�F�𔼓����ɂ���
	CInputKeyboard* pInput = CManager::GetInputKeyboard();
	//===========================
	//�ʒu���x�_�ɌŒ�
	//===========================
	m_PosInfo.Pos = m_PosInfo.SupportPos;
	//========================================================================================

	//===========================
	//�ʒu��ύX
	//===========================
	if (pInput->GetPress(DIK_LSHIFT) == true)
	{
		if (pInput->GetPress(DIK_LCONTROL))
		{
			if (pInput->GetTrigger(DIK_W) == true)
			{
				m_PosInfo.Pos.y += 0.1f;
			}
			else if (pInput->GetTrigger(DIK_S) == true)
			{
				m_PosInfo.Pos.y -= 0.1f;
			}
		}
		else
		{
			if (pInput->GetPress(DIK_W) == true)
			{
				m_PosInfo.Pos.y += 5.0f;
			}
			else if (pInput->GetPress(DIK_S) == true)
			{
				m_PosInfo.Pos.y -= 5.0f;
			}
		}
	}
	else
	{
		CCalculation::CaluclationMove(false, Move, 5.0f, CCalculation::MOVEAIM_XZ, m_Rot.y);
	}
	//�x�_���ꏏ�Ɉړ�
	m_PosInfo.Pos += Move;
	m_PosInfo.SupportPos = m_PosInfo.Pos;
	CManager::GetDebugProc()->PrintDebugProc("�x�_�ʒu(���L�[) %f %f %f\n", m_PosInfo.SupportPos.x,m_PosInfo.SupportPos.y, m_PosInfo.SupportPos.z);
	CManager::GetDebugProc()->PrintDebugProc("����Z(FG�L�[) %f\n", m_Rot.z);
	CManager::GetCamera()->SetPosR(m_PosInfo.Pos);
	//================================================================================================================================================

}
//================================================================================================================================================

//============================================================================
//�ő咸�_�ƍŏ����_�����ւ��邩�ǂ���
//============================================================================
void CObjectX::ChengeEditSwapVtxXZ()
{
	CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

	if (pInputKeyBoard->GetTrigger(DIK_7))
	{
		m_bSwapVtxXZ = m_bSwapVtxXZ ? false : true;
	}

	CManager::GetDebugProc()->PrintDebugProc("���_��XZ�����ւ��邩�ǂ����i�V�j�F%d\n", m_bSwapVtxXZ);
}
//================================================================================================================================================

//============================================================================
//�X�e�[�W�}�l�[�W���[�����𑀍삷��
//============================================================================
void CObjectX::ManagerChooseControlInfo()
{
	ChengeEditPos();//�ʒu��ς���

	ChengeEditScale();//�g�嗦��ς���

	ChengeEditSwapVtxXZ();

	CManager::GetCamera()->SetPosR(m_PosInfo.Pos);//�J�����̒����_�����݂̈ʒu�ɐݒ�
}
//================================================================================================================================================

//============================================================================
//�e�L�X�g�t�@�C���ɏ���ۑ�����
//============================================================================
void CObjectX::SaveInfoTxt(fstream & WritingFile)
{
	WritingFile << "POS = " << fixed << setprecision(3)<< m_PosInfo.Pos.x << " " <<
		fixed << setprecision(3) << m_PosInfo.Pos.y << " " << 
		fixed << setprecision(3) << m_PosInfo.Pos.z << " " << endl;//�ʒu
	WritingFile << "ROT = " << fixed << setprecision(3) << m_Rot.x << " " <<
		fixed << setprecision(3) << m_Rot.y << " " <<
		fixed << setprecision(3) << m_Rot.z << " " << endl;//����
	WritingFile << "SCALE = " << fixed << setprecision(3) << m_Scale.x << " " <<
		fixed << setprecision(3) << m_Scale.y << " " <<
		fixed << setprecision(3) << m_Scale.z << " " << endl;//�g�嗦

	WritingFile << "SWAPVTXXZ = " << m_bSwapVtxXZ << endl;
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

	D3DXVECTOR3 RayCollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 CalcRayCollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fResultNearLength = 0.0f;

    //�e�̃}�g���b�N�X��������
	D3DXMatrixIdentity(&mtxShadow);

	//���C�g�̋t�����x�N�g����ݒ�
	vecLight = D3DXVECTOR4(-100.0f, 300.0f,300.0f, 0.0f);
	ShadowPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
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

	int nCntColRay = 0;//���C�����������񐔂��J�E���g����
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = CObject::GetTopObject(nCntPri);

		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();

			if (pObj->GetType() == CObject::TYPE::BLOCK || pObj->GetType() == CObject::TYPE::BGMODEL)
			{
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);
				if (CCollision::RayIntersectsAABBCollisionPos(m_PosInfo.Pos + D3DXVECTOR3(0.0f,0.1f,0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
					pObjX->GetPosInfo().GetPos() + pObjX->GetVtxMin(),pObjX->GetPosInfo().GetPos() + pObjX->GetVtxMax(), CalcRayCollisionPos))
				{
					float fLength = sqrtf(powf(CalcRayCollisionPos.y - m_PosInfo.Pos.y,2));//���C�����������ʒu��Y���̋��������
					nCntColRay++;
					if (nCntColRay == 1)
					{//�ŏ��̓��������I�u�W�F�N�g�Ȃ̂ŁA�������ŋ����ƃ��C�����������ʒu���L�^����
						fResultNearLength = fLength;
						RayCollisionPos = CalcRayCollisionPos;
					}
					else
					{
						if (fResultNearLength > fLength)
						{//���C�����������I�u�W�F�N�g�̒��ň�ԋ߂��̂ŁA�����ƃ��C�����������ʒu���X�V����
							fResultNearLength = fLength;
							RayCollisionPos = CalcRayCollisionPos;
						}
					}
				}
			}

			pObj = pNext;//���X�g�����ɐi�߂�
		}
	}

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,m_PosInfo.Pos.x,RayCollisionPos.y, m_PosInfo.Pos.z);
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
		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f,0.0f,0.0f,m_ObjectXInfo.Diffuse[nCntMat].a);

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