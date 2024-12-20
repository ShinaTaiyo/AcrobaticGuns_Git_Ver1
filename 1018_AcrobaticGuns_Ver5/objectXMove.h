//=======================================================================================
//
//�U���S���F���f����`�悷��[objectX.h]
//Author:ShinaTaiyo
//
//=======================================================================================
#ifndef _OBJECTXMOVE_H_
#define _OBJECTXMOVE_H_

//==============================================
//�C���N���[�h
//==============================================
#include "main.h"
#include "objectX.h"
//=======================================================================================

//=======================================================
//�I�u�W�F�N�gX�N���X
//=======================================================
class CObjectXMove : public CObjectX
{
public:
	CObjectXMove(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                                                       //�R���X�g���N�^
	~CObjectXMove() override;                                             //�f�X�g���N�^
	HRESULT Init() override;                                              //����������
	void Uninit() override;                                               //�I������
	void Update() override;                                               //�X�V����
	void Draw() override;                                                 //�`�揈��	

    //==========================================================
	//�ړ���
	//==========================================================
	//�ړ���
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }                                       //�ړ��ʂ�ݒ肷��
	const D3DXVECTOR3& GetMove() const { return m_Move; }                                   //�ړ��ʂ��擾����

	void SetAddMove(D3DXVECTOR3 Move) { m_AddMove = Move; }//���Z�ړ��ʂ�ݒ�
	const D3DXVECTOR3& GetAddMove() const { return m_AddMove; }//���Z�ړ��ʂ��擾

	//����
	void SetUseInteria(bool bUseInteria, float fInertia) { m_bUseInteria = bUseInteria; m_fInertia = fInertia; }                   //�����������邩�ǂ���
	float& GetInertia() { return m_fInertia; }                                              //�������擾����
	void SetUndoInertia() { m_fInertia = m_fNORMAL_INERTIA; }                               //�����̑傫�������ɖ߂�
	bool& GetUseInteria() { return m_bUseInteria; }                                         //�������|���邩�ǂ������擾

	//�d��
	void SetUseGravity(bool bUse, float fGravityPower) { m_bUseGravity = bUse; m_fGravityPower = fGravityPower; }//�d�͂�ݒ肷��
	const bool& GetUseGravity() const { return m_bUseGravity; }                                                  //�d�͂��g�p���邩�ǂ�����ݒ�

	//��Z����
	void SetUseMultiSpeed(bool bUse, D3DXVECTOR3 MultiSpeed) { m_MultiSpeed = MultiSpeed; m_bUseMultiSpeed = bUse; }//��Z�������g�p���邩�ǂ���

	//���Z���x
	void SetUseAddSpeed(bool bUse, D3DXVECTOR3 AddSpeed) { m_bUseAddSpeed = bUse; m_AddSpeed = AddSpeed; }//�������g�p���邩�ǂ���
	//============================================================================================================

	//===================================
	//�ʒu�X�V�֌W
	//===================================
	void SetUseUpdatePos(bool bUse) { m_bUseUpdatePos = bUse; }//�ʒu�̍X�V���g�p���邩�ǂ���
	void SetAdjustPos(bool bUse) { m_bAdjustPos = bUse; }      //�ʒu�̕␳�����邩�ǂ���
	//=================================================================================================================

	//==========================================================
	//�G�f�B�^�֌W
	//==========================================================
	//�֐�
	void SaveInfoTxt(fstream& WritingFile) override;//�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	void ManagerChooseControlInfo() override;              //�X�e�[�W�}�l�[�W���[�����𑀍삷��
	//=================================================================================================================

	//==========================================================
	//�ÓI�����o�擾
	//==========================================================
	static const float GetNormalGravity() { return m_fNORMAL_GRAVITY; }
	static const float GetNormalInertia() { return m_fNORMAL_INERTIA; }
	//=================================================================================================================
protected:
private:
	//===============================================
	//�ÓI�����o
	//===============================================
	static constexpr float m_fNORMAL_INERTIA = 0.5f;//���ʂ̊���
	static constexpr float m_fNORMAL_GRAVITY = 1.0f;//���ʂ̏d��
	//=================================================================================================================
	
	//==========================================================
	//�ړ���
	//==========================================================
	
	void GravityProcess();                          //�d�͂̏���
	void UpdatePos();                               //�ʒu���X�V������

	//�ړ���
	D3DXVECTOR3 m_Move;                             //�ړ���!
	D3DXVECTOR3 m_AddMove;                          //���Z�ړ���
	//����
	float m_fInertia;                               //����!
	bool m_bUseInteria;                             //�����������邩�ǂ���!
	//�d��
	bool m_bUseGravity;                             //�d�͂��g�p���邩�ǂ���!
	float m_fGravityPower;                          //�d�͂̑傫��!
	//��Z����
	bool m_bUseMultiSpeed;                                  //��Z���������邩�ǂ���!
	D3DXVECTOR3 m_MultiSpeed;                               //��Z�����x!
	//����
	bool m_bUseAddSpeed;                                    //�������g�p���邩�ǂ���!
	D3DXVECTOR3 m_AddSpeed;                                 //�����x

	//�ʒu�̍X�V�����邩�ǂ���
	bool m_bUseUpdatePos;                           //�ʒu�̍X�V���g�p���邩�ǂ���!

	//�ʒu�̕␳�����邩�ǂ���
	bool m_bAdjustPos;

	//=================================================================================================================

	//==========================================================
	//�ړ���
	//==========================================================
	//=================================================================================================================
};
//============================================================================================================
#endif
