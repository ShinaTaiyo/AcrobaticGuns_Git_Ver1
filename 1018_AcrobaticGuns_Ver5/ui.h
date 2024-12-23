//=====================================================================================
//
//�X���P�O���F�^�C�g�������[ui.h]
//Author:ShinaTaiyo
//
//=====================================================================================1

#ifndef _UI_H_  
#define _UI_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
#include "number.h"
//==========================================

//======================
//�O���錾
//======================
class CObject2D;
class CUi;

//==========================================
//UI�X�e�[�g
//==========================================
class CUiState
{
public:
	CUiState();                 //�R���X�g���N�^
	virtual ~CUiState();        //�f�X�g���N�^
	virtual void Process(CUi* pUi);//����         
};
//=======================================================================================

//==========================================
//UI�X�e�[�g�F�����ێ�
//==========================================
class CUiState_Numeric : public CUiState
{
public:
	CUiState_Numeric(CUi* pUi, int nValue, float fWidth, float fHeight);//�R���X�g���N�^
	~CUiState_Numeric() override;//�f�X�g���N�^
	void Process(CUi* pUi) override;//����
private:
	vector<CNumber*> m_VecNum;//�����̃x�N�^�[
	int m_nValue;              //�l
};
//=======================================================================================

//==========================================
//UI�N���X
//==========================================
class CUi : public CObject2D
{
public:
	enum class UITYPE
	{
		LOCKON = 0,      //���b�N�I��
		ACTIONMODEWINDOW,//�A�N�V�������[�h�E�C���h�E
	    ACTIONMODE_GUN,  //�V���b�g���[�hUI
		ACTIONMODE_DIVE, //�_�C�u���[�hUI
		PHASETEXT,       //�t�F�[�Y����
		STAGETEXT,       //�X�e�[�W����
	    TARGET_000,      //�^�[�Q�b�g
		MAX
	};

	typedef enum
	{
		UIMOVETYPE_NORMAL = 0,//���ʂ̓���
		UIMOVETYPE_FLOATING,  //���V������
		UIMOVETYPE_MAX
	}UIMOVETYPE;

	CUi(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::UI, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);                      //�R���X�g���N�^
	~CUi() override;             //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static CUi * Create(UITYPE type,CObject2D::POLYGONTYPE PolygonType,float fWidth, float fHeight,int nLife,bool bUseLife,D3DXVECTOR3 Pos,D3DXVECTOR3 Move,D3DXVECTOR3 Rot,D3DXCOLOR col);//�w�i�̐���
	void SetUiType(UITYPE type);
	void SetUseUiEffect(bool bUse, int nSetEffectLife, D3DXCOLOR col) { m_bUseUiEffect = bUse; m_nSetUiEffectLife = nSetEffectLife; m_SetUiEffectColor = col; }
	UITYPE GetUiType() { return m_Type; }//UI�̎�ނ��擾
	void SetNumericState(int nValue, float fWidth, float fHeight);//������Ԃ�ݒ肷��
protected:
	static const string UI_FILENAME[int(UITYPE::MAX)];//UI�̃e�N�X�`���t�@�C����
private:
	UITYPE m_Type;                 //�w�i�̎��
	UIMOVETYPE m_MoveType;         //UI�̓������̎��
	bool m_bUseUiEffect;           //UI�ŃG�t�F�N�g���o�����ǂ���

	D3DXCOLOR m_SetUiEffectColor;  //UI�ŏo���G�t�F�N�g�̐F�����ݒ�p
	int m_nSetUiEffectLife;        //UI�̃G�t�F�N�g�̗̑͐ݒ�p
	CUiState* m_pUiState;          //UI�̏��
};
//=======================================================================================

//==========================================
//UI�G�t�F�N�g�N���X
//==========================================
class CUiEffect : public CUi
{
public:
	CUiEffect();                //�R���X�g���N�^
	virtual ~CUiEffect();       //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static CUiEffect* Create(UITYPE type, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight,int nLife,D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, D3DXCOLOR col);//�w�i�̐���
private:
};
//=======================================================================================
#endif