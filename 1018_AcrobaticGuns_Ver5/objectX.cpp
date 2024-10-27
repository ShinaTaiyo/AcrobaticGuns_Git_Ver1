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
#include <algorithm>
//===========================================================================================

//================================================
//�R���X�g���N�^
//================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority), m_bUseDraw(true),m_ObjectXInfo(),m_bColorChenge(false),m_bUseAddRot(false),
m_bUseAddScaling(false),m_bUseShadow(false),m_nChengeColorTime(0),m_nIndexObjectX(0),m_nManagerType(0),m_nObjXType(OBJECTXTYPE_BLOCK), 
m_nTypeNum(0), m_bUseMultiScale(false), m_MultiScale(NULL_VECTOR3),m_bUseCulling(false), m_Pos(NULL_VECTOR3), m_SupportPos(NULL_VECTOR3),
m_PosOld(NULL_VECTOR3),m_Rot(NULL_VECTOR3),m_Scale(NULL_VECTOR3),m_FormarScale(NULL_VECTOR3),m_Size(NULL_VECTOR3),m_VtxMin(NULL_VECTOR3),
m_OriginVtxMin(NULL_VECTOR3),m_VtxMax(NULL_VECTOR3),m_OriginVtxMax(NULL_VECTOR3),m_mtxWorld(),m_AddRot(NULL_VECTOR3),m_SenterPos(NULL_VECTOR3),
m_AddScale(NULL_VECTOR3)
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
	//==========================
	//�ϐ��̏�����
	//==========================
	m_mtxWorld = {};                                   //���[���h�}�g���b�N�X
	//==================================================================================

	//===========================================
	//�e�֌W
	//===========================================
	m_bUseShadow = true;
	//==================================================================================

	m_bUseAddScaling = false;                 //�g�嗦�̉��Z���g�p���邩�ǂ���
	m_AddScale = NULL_VECTOR3;                //�g�嗦�̉��Z��    

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

	if (m_bUseAddRot == true)
	{//�����̉��Z����
		m_Rot += m_AddRot;
	}

	if (GetStageManagerChoose() == true)
	{//�I�u�W�F�N�gX�̏���\��

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
			if (CManager::GetInputKeyboard()->GetPress(DIK_R) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_R) == true)
		{
			Scale.x += 0.1f;
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
			if (CManager::GetInputKeyboard()->GetPress(DIK_T) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_T) == true)
		{
			Scale.y += 0.1f;
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
			if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
		{
			Scale.z += 0.1f;
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
	float fMoveX = 0.0f;                                            //X�����̈ړ���
	float fMoveZ = 0.0f;                                            //Z�����̈ړ���
	bool bMove = false;                                             //�ړ����Ă��邩�ǂ��� 
	D3DXVECTOR3 Move = NULL_VECTOR3;
	SetColor(D3DXCOLOR(1.0f,0.0f,0.0f,0.5f),3,true,true);           //�F�𔼓����ɂ���

	//===========================
	//�ʒu���x�_�ɌŒ�
	//===========================
	m_Pos = m_SupportPos;
	//========================================================================================

	//===========================
	//�ʒu��ύX
	//===========================
	CCalculation::CaluclationMove(Move, 5.0f, CCalculation::MOVEAIM_XZ, m_Rot.y);

	////===========================
	////RT�{�^���������Ă�����
	////===========================
	//if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	//{//L�R���g���[���L�[�������Ȃ���
	//	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	//	{//�V�t�g�L�[�������Ȃ���E�E�E
	//		if (CManager::GetInputKeyboard()->GetPress(DIK_F) == true)
	//		{
	//			m_Rot.z -= 0.01f;
	//		}
	//	}
	//	else if (CManager::GetInputKeyboard()->GetPress(DIK_G) == true)
	//	{
	//		m_Rot.z += 0.01f;
	//	}
	//}
	//else
	//{//L�R���g���[���L�[�������Ă��Ȃ�
	//	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	//	{//�V�t�g�L�[�������Ȃ���E�E�E
	//		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F) == true)
	//		{
	//			m_Rot.z -= 0.01f;
	//		}
	//	}
	//	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_G) == true)
	//	{
	//		m_Rot.z += 0.01f;
	//	}
	//}

	//�x�_���ꏏ�Ɉړ�
	m_Pos += Move;
	m_SupportPos = m_Pos;
	CManager::GetDebugProc()->PrintDebugProc("�x�_�ʒu(���L�[) %f %f %f\n", m_SupportPos.x, m_SupportPos.y, m_SupportPos.z);
	CManager::GetDebugProc()->PrintDebugProc("����Z(FG�L�[) %f\n", m_Rot.z);
	//================================================================================================================================================

}
//================================================================================================================================================

//============================================================================
//�X�e�[�W�}�l�[�W���[�����𑀍삷��
//============================================================================
void CObjectX::ManagerChooseControlInfo()
{
	ChengeEditPos();//�ʒu��ς���

	ChengeEditScale();//�g�嗦��ς���
}
//================================================================================================================================================

//============================================================================
//�e�L�X�g�t�@�C���ɏ���ۑ�����
//============================================================================
void CObjectX::SaveInfoTxt(fstream & WritingFile)
{
	WritingFile << "POS = " << fixed << setprecision(3)<< m_Pos.x << " " << 
		fixed << setprecision(3) << m_Pos.y << " " << 
		fixed << setprecision(3) << m_Pos.z << " " << endl;//�ʒu
	WritingFile << "ROT = " << fixed << setprecision(3) << m_Rot.x << " " <<
		fixed << setprecision(3) << m_Rot.y << " " <<
		fixed << setprecision(3) << m_Rot.z << " " << endl;//����
	WritingFile << "SCALE = " << fixed << setprecision(3) << m_Scale.x << " " <<
		fixed << setprecision(3) << m_Scale.y << " " <<
		fixed << setprecision(3) << m_Scale.z << " " << endl;//�g�嗦
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