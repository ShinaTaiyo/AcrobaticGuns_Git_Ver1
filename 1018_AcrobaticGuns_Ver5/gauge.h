//====================================================
//
//�U���P�U���F�v���C���[�̃Q�[�W������[gauge.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _GAUGE_H_  
#define _GAUGE_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
//==========================================

class CDamageGauge;
//==========================================

//==========================================
//�Q�[�W�N���X
//==========================================
class CGauge : public CObject2D
{
public:
	//===========================
	//�Q�[�W�̎��
	//===========================
	enum class GAUGETYPE
	{
		PLAYERHP = 0, //�v���C���[�̗̑�
		MAX
	};
	//======================================================================
	CGauge(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::GAUGE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);                   //�R���X�g���N�^
	~CGauge() override;         //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	virtual void AddParam(int nParam);//�p�����[�^�𑝉�����
	void SubParam(int nParam) { m_nParam -= nParam; }            //�p�����[�^����������@
	void SetDeath() override;   //���S�t���O��ݒ�
	int& GetParam() { return m_nParam; }//�p�����[�^���擾
	int GetMaxParam() { return m_nMaxParam; }//�ő�p�����[�^���擾
	virtual void SetParam(int nParam);
	void SetMaxParam(int nParam) { m_nMaxParam = nParam; }//�ő�p�����[�^��ݒ�
	void SetGaugeType(GAUGETYPE type) { m_type = type; }
	static CGauge* Create(GAUGETYPE type, int nParam, float fWidth, float fHeight, D3DXVECTOR3 pos);//�Q�[�W�𐶐�

	//====================================
	//�I�v�V����
	//====================================
	void SetShake(float fPower, int nTime) { m_fShakePower = fPower; m_nShakeTime = nTime;}
protected:
private:
	int m_nParam;               //�Q�[�W�ŕ\�L���錻�݂̃p�����[�^
	int m_nParamOld;            //��O�̃p�����[�^
	GAUGETYPE m_type;           //�Q�[�W�̎��
	int m_nMaxParam;            //�Q�[�W�ŕ\�L����p�����[�^�̍ő�l

	float m_fShakePower;        //�V�F�C�N�������
	int m_nShakeTime;           //�V�F�C�N�����鎞��
};
//=======================================================================================================================
#endif