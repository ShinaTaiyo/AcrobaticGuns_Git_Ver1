//=======================================================================================
//
//�U���S���F���f����`�悷��[objectX.h]
//Author:ShinaTaiyo
//
//=======================================================================================
#ifndef _OBJECTXALIVE_H_
#define _OBJECTXALIVE_H_

//==============================================
//�C���N���[�h
//==============================================
#include "main.h"
#include "objectXMove.h"
//=======================================================================================

//=======================================================
//�I�u�W�F�N�gX�A���C�u�N���X
//=======================================================
class CObjectXAlive : public CObjectXMove
{
public:
	CObjectXAlive(int nPriority = 2);                                      //�R���X�g���N�^
	~CObjectXAlive() override;                                             //�f�X�g���N�^
	HRESULT Init() override;                                               //����������
	void Uninit() override;                                                //�I������
	void Update() override;                                                //�X�V����
	void Draw() override;                                                  //�`�揈��	
	//==========================================================
	//����֌W
	//==========================================================

	//�����`�̉����o������

	//X
	void SetExtrusionCollisionSquareX(bool bSuccess) { m_bExtrusionCollisionSquareX = bSuccess;}
	const bool & GetExtrusionCollisionSquareX() const { return m_bExtrusionCollisionSquareX; }

	//Y
	void SetExtrusionCollisionSquareY(bool bSuccess) { m_bExtrusionCollisionSquareY = bSuccess;}
	const bool & GetExtrusionCollisionSquareY() const { return m_bExtrusionCollisionSquareY; }

	//Z
	void SetExtrusionCollisionSquareZ(bool bSuccess) { m_bExtrusionCollisionSquareZ = bSuccess;}
	const bool &  GetExtrusionCollisionSquareZ() const { return m_bExtrusionCollisionSquareZ; }
	//============================================================================================================

	//======================================================
	//�̗͊֌W
	//======================================================

	//�̗�
	void SetLife(int nLife) { m_nLife = nLife; }                    //�̗͂�ݒ肷��
	int& GetLife() { return m_nLife; }                              //�̗͂��擾����

	//�ő�̗�
	void SetMaxLife(int nMaxLife) { m_nMaxLife = nMaxLife; }        //�ő�̗͂�ݒ肷��
	int& GetMaxLife() { return m_nMaxLife; }                        //�ő�̗͂��擾����

	//�q�b�g�X�g�b�v
	void SetHitStopTime(int nHitStopTime) { m_nHitStopTime = nHitStopTime; }//�q�b�g�X�g�b�v���Ԃ�ݒ肷��
	bool& GetHitStop() { return m_bHitStop; }                       //�q�b�g�X�g�b�v��Ԃ��ǂ������擾����

	//�����I�ɑ̗͂����炷
	void SetAutoSubLife(bool bUse) { m_bAutoSubLife = bUse; }       //�̗͂������I�Ɍ��炷���ǂ�����ݒ�
	int& GetHitStopTime() { return m_nHitStopTime; }                //�q�b�g�X�g�b�v���Ԃ��擾����

	//�_���[�W��^����
	void SetDamage(int nDamage, int nHitStopTime);          //�_���[�W��^����

	//�񕜂���
	void SetHeal(int nHeal, D3DXCOLOR col, float fWidth, float fHeight);                  //�񕜗ʂ�^����
	//=================================================================================================================

	//==========================================================
	//�G�f�B�^�֌W
	//==========================================================
	//�֐�
	void SaveInfoTxt(fstream& WritingFile) override;//�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	void ManagerChooseControlInfo() override;              //�X�e�[�W�}�l�[�W���[�����𑀍삷��
	//=================================================================================================================
protected:
private:
	//===================================
	//�̗͊֌W
	//===================================

	//�q�b�g�X�g�b�v
	bool m_bHitStop;                                //�q�b�g�X�g�b�v!
	int m_nHitStopTime;                             //�q�b�g�X�g�b�v����!
	void HitStopProcess();                          //�q�b�g�X�g�b�v�̏���

	//�����I�ɑ̗͂����炷���ǂ���
	bool m_bAutoSubLife;                            //�����I�ɑ̗͂����炵�����邩�ǂ���!

	//�̗�
	int m_nLife;                                    //�̗�!
	int m_nMaxLife;                                 //�ő�̗�!

	//�F
	bool m_bUseRatioLifeAlpha;                      //�̗͂̊����œ����x��ς��邩�ǂ���!
	//============================================================================================================

	//===================================
	//����֌W
	//===================================

	//�����`�̉����o������
	bool m_bExtrusionCollisionSquareX;//X
	bool m_bExtrusionCollisionSquareY;//Y
	bool m_bExtrusionCollisionSquareZ;//Z
	//============================================================================================================

	//===================================
	//�G�f�B�^�֌W
	//===================================
	void EditLife();//�̗͂�ύX����
	//============================================================================================================
};
//============================================================================================================
#endif
