//====================================================
//
//�P�O���Q�S���F�U���N���X����������[attack.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _ATTACK_H_  
#define _ATTACK_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object.h"
#include "objectXAlive.h"
//==========================================

//===========================================
//�U���N���X
//===========================================
class CAttack : public CObjectXAlive
{
public:
	enum class ATTACKTYPE
	{
		BULLET = 0,
		EXPLOSION,
		MAX
	};

	struct HitStop
	{
		int nCntTime;
		const int nSetTime;
	};
	CAttack(int nPower,int nSetHitStopTime,int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ATTACK, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                  //�R���X�g���N�^
	~CAttack();                 //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	void SetCollisionRelease(bool bUse) { m_bCollisionRelease = bUse; }
	const HitStop & GetHitStop() const { return m_HitStop; }
	const int& GetPower() const { return m_nPower; }
protected:
	void SetAttackType(ATTACKTYPE AttackType) { m_Type = AttackType;}//�U���̎�ނ�ݒ肷��
	const ATTACKTYPE & GetAttackType() const { return m_Type; }      //�U���̎�ނ��擾����
	static const string ATTACK_FILENAME[static_cast<int>(ATTACKTYPE::MAX)];//�U�����f���̃t�@�C���� 
private:
	virtual void Collision();//�����蔻����s������
	int m_nPower;     //�U����

	bool m_bCollisionRelease;//�Փˎ��ɏ������ǂ���

	HitStop m_HitStop;//�q�b�g�X�g�b�v
	ATTACKTYPE m_Type;//�^�C�v
};
//==================================================================================================================================================

//===========================================
//�v���C���[�U���N���X
//===========================================
class CAttackPlayer : public CAttack
{
public:
	CAttackPlayer(int nPower,int nSetHitStopTime);      //�R���X�g���N�^
	~CAttackPlayer() override;                 //�f�X�g���N�^
	HRESULT Init() override;          //����������
	void Uninit() override;           //�I������
	void Update() override;           //�X�V����
	void Draw() override;             //�`�揈��
	void SetDeath() override;         //���S�t���O��ݒ�
	static CAttackPlayer* Create(ATTACKTYPE AttackType,int nPower,int nSetHitStopTime,int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale);//��������
private:
};
//==================================================================================================================================================

//===========================================
//�G�U���N���X
//===========================================
class CAttackEnemy : public CAttack
{
public:
	CAttackEnemy(int nPower,int nSetHitStopTime);         //�R���X�g���N�^
	~CAttackEnemy() override;                  //�f�X�g���N�^
	HRESULT Init() override;          //����������
	void Uninit() override;           //�I������
	void Update() override;           //�X�V����
	void Draw() override;             //�`�揈��
	void SetDeath() override;         //���S�t���O��ݒ�
	static CAttackEnemy* Create(ATTACKTYPE AttackType, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale);//��������
private:

};
//==================================================================================================================================================
#endif
