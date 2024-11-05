//====================================================
//
//�P�O���Q�W���F�_���������ɒe�𔭎˂�����[lockon.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _LOCKON_H_  
#define _LOCKON_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
#include "lockon.h"
//==========================================

//==========================================
//���b�N�I���N���X
//==========================================
class CLockon : public CObject2D
{
public:
	CLockon();     //�`�揇�ݒ�p�R���X�g���N�^
	~CLockon() override;       //�f�X�g���N�^
	HRESULT Init() override;     //����������
	void Uninit() override;      //�I������
	void Update() override;      //�X�V����
	void Draw() override;        //�`�揈��
	void SetDeath() override;    //���S�t���O�ݒ菈��
	static CLockon* Create(D3DXVECTOR3 Pos,CObject2D::POLYGONTYPE PolygonType,float fWidth,float fHeight,D3DXCOLOR col);//��������

	const D3DXVECTOR3& GetLockOnPos() const { return m_LockOnPos; }//�_���Ă���ʒu���擾����
	const D3DXVECTOR3& GetNowRay() const { return m_NowRay; }      //���݂̃��C���擾����
	const D3DXVECTOR3& GetFrontPos() const { return m_FrontPos; }  //��O�̈ʒu���擾����
private:
	//=======================================
	//�ÓI�����o
	//=======================================
	static constexpr float m_fNORMAL_LOCKONMOVE = 15.0f;
	//==========================================================================================================

	//=======================================
	//�ϐ�
	//=======================================
	D3DXVECTOR3 m_LockOnPos;//�_���Ă���ʒu
	D3DXVECTOR3 m_NowRay;   //���݂̃��C
	D3DXVECTOR3 m_FrontPos; //��O�̈ʒu
	//==========================================================================================================

	//=======================================
	//�v���g�^�C�v�錾
	//=======================================
	void MoveProcess();//�ړ�����
	void SearchProcess();//�J�[�\���̐�ɂ���I�u�W�F�N�g���T�[�`����
	void CalcRay();//���C���v�Z����
	//==========================================================================================================


};

#endif
