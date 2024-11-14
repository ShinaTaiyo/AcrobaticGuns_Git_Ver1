//===================================================================================================
//
//�X���P�O���F�^�C�g������������[ui.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================

//====================================================
//�C���N���[�h
//====================================================
#include "ui.h"
#include "texture.h"
#include "renderer.h"
#include "manager.h"
#include "sound.h"
//===================================================================================================

//====================================================
//�ÓI�����o�錾
//====================================================
const string CUi::UI_FILENAME[int(CUi::UITYPE::MAX)] =
{
	"data\\TEXTURE\\UI\\LockOn_001.png",
	"data\\TEXTURE\\UI\\ActionModeWindow_000.png",
	"data\\TEXTURE\\UI\\Gun_000.png",
	"data\\TEXTURE\\UI\\Dive_000.png",
};//�e�N�X�`���t�@�C����

//====================================================
//�R���X�g���N�^
//====================================================
CUi::CUi(int nPriority) : CObject2D(nPriority),m_MoveType(UIMOVETYPE_NORMAL),m_Type(UITYPE::LOCKON),m_bUseUiEffect(false),m_nSetUiEffectLife(0),m_SetUiEffectColor(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
{

}
//===================================================================================================

//====================================================
//�f�X�g���N�^
//====================================================
CUi::~CUi()
{

}
//===================================================================================================

//====================================================
//����������
//====================================================
HRESULT CUi::Init()
{
	CObject2D::Init();
	return S_OK;
}
//===================================================================================================

//====================================================
//�I������
//====================================================
void CUi::Uninit()
{
	CObject2D::Uninit();
}
//===================================================================================================

//====================================================
//�X�V����
//====================================================
void CUi::Update()
{
	CObject2D::Update();

	if (m_bUseUiEffect == true)
	{
		CUiEffect::Create(m_Type, GetPolygonType(), GetWidth(), GetHeight(), m_nSetUiEffectLife, GetPos(), D3DXVECTOR3(0.0f,0.0f,0.0f), GetRot(), m_SetUiEffectColor);
	}
}
//===================================================================================================

//====================================================
//�`�揈��
//====================================================
void CUi::Draw()
{
	CObject2D::Draw();
}
//===================================================================================================

//====================================================
//���S�t���O�ݒ菈��
//====================================================
void CUi::SetDeath()
{
	CObject::SetDeath();
}
//===================================================================================================

//====================================================
//��������
//====================================================
CUi* CUi::Create(UITYPE type, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight, int nLife, bool bUseLife, D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, D3DXCOLOR col)
{
	CUi* pUi = DBG_NEW CUi;   //�e�𐶐�
	bool bSuccess = pUi->CObject::GetCreateSuccess();
	CTexture* pTextureClass = CManager::GetTexture();           //�e�N�X�`���N���X���擾
	if (bSuccess == true)
	{
		if (pUi != nullptr)
		{
			pUi->SetUiType(type);                  //�w�i�̎�ނ�ݒ�
			pUi->SetUseLife(bUseLife, nLife, nLife);//�̗͐ݒ�
			pUi->Init();                                                     //����������
			pUi->SetMove(Move);//�ړ���
			pUi->SetRot(Rot);//����
			pUi->SetPos(Pos);//�ʒu
			pUi->SetSupportPos(Pos);//�x�_�ƂȂ�ʒu
			pUi->SetUseScale(true);//�g�嗦���g�p����
			pUi->CObject2D::SetInfo(1, 1, fWidth, fHeight, col, PolygonType, true);//�|���S���ƃe�N�X�`������ݒ�
			pUi->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pUi->SetTextureIndex(pTextureClass->Regist(UI_FILENAME[int(type)]));
			pUi->CObject2D::BindTexture(pTextureClass->GetAddress(pUi->GetTextureIndex()));
			pUi->CObject::SetType(CObject::TYPE::UI);//�I�u�W�F�N�̎�ނ����߂�
		}
	}
	else
	{
		delete pUi;
		pUi = nullptr;
		return nullptr;
	}

	return pUi;
}
//===================================================================================================

//====================================================
//��������
//====================================================
void CUi::SetUiType(UITYPE type)
{
	m_Type = type;
	CTexture* pTextureClass = CManager::GetTexture();           //�e�N�X�`���N���X���擾
    SetTextureIndex(pTextureClass->Regist(UI_FILENAME[int(type)]));
    CObject2D::BindTexture(pTextureClass->GetAddress(GetTextureIndex()));

}
//===================================================================================================

//=======================================<<<<<<<<UI�G�t�F�N�g�N���X>>>>>>>>=========================================================================


//====================================================
//�R���X�g���N�^
//====================================================
CUiEffect::CUiEffect() : CUi(5)
{

}
//===================================================================================================

//====================================================
//�f�X�g���N�^
//====================================================
CUiEffect::~CUiEffect()
{
}
//===================================================================================================

//====================================================
//����������
//====================================================
HRESULT CUiEffect::Init()
{
	CUi::Init();
	return S_OK;
}
//===================================================================================================

//====================================================
//�I������
//====================================================
void CUiEffect::Uninit()
{
	CUi::Uninit();
}
//===================================================================================================

//====================================================
//�X�V����
//====================================================
void CUiEffect::Update()
{
	CUi::Update();
}
//===================================================================================================

//====================================================
//�`�揈��
//====================================================
void CUiEffect::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	//a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	CUi::Draw();
	//a�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//===================================================================================================

//====================================================
//���S�t���O�ݒ菈��
//====================================================
void CUiEffect::SetDeath()
{
	CUi::SetDeath();
}
//===================================================================================================

//====================================================
//��������
//====================================================
CUiEffect* CUiEffect::Create(UITYPE type, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight, int nLife, D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, D3DXCOLOR col)
{
	CUiEffect* pUiEffect = DBG_NEW CUiEffect;   //�e�𐶐�
	bool bSuccess = pUiEffect->CObject::GetCreateSuccess();
	CTexture* pTextureClass = CManager::GetTexture();           //�e�N�X�`���N���X���擾
	if (bSuccess == true)
	{
		if (pUiEffect != nullptr)
		{
			pUiEffect->SetUiType(type);                  //�w�i�̎�ނ�ݒ�
			pUiEffect->Init();                                                     //����������
			pUiEffect->SetMove(Move);//�ړ���
			pUiEffect->SetRot(Rot);//����
			pUiEffect->SetPos(Pos);//�ʒu
			pUiEffect->SetUseLife(true, nLife, nLife);//�̗͂��g�p����
			pUiEffect->SetSupportPos(Pos);//�x�_�ƂȂ�ʒu
			pUiEffect->SetUseLifeRatioColor(true);//�̗͂ɉ����ĐF������ς���
			pUiEffect->SetUseScale(true);//�g�嗦���g�p����
			pUiEffect->CObject2D::SetInfo(1, 1, fWidth, fHeight, col, PolygonType, true);//�|���S���ƃe�N�X�`������ݒ�
			pUiEffect->SetUseDeath(true);                 //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pUiEffect->SetTextureIndex(pTextureClass->Regist(UI_FILENAME[int(type)]));
			pUiEffect->CObject2D::BindTexture(pTextureClass->GetAddress(pUiEffect->GetTextureIndex()));
			pUiEffect->CObject::SetType(CObject::TYPE::UI);//�I�u�W�F�N�̎�ނ����߂�
		}
	}
	else
	{
		delete pUiEffect;
		pUiEffect = nullptr;
		return nullptr;
	}

	return pUiEffect;
}
//===================================================================================================