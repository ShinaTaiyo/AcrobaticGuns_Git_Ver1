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

	CAttack(int nPri = 2);                  //�R���X�g���N�^
	~CAttack();                 //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static CAttack* Create(ATTACKTYPE AttackType, int nLife,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale);//��������
protected:
	void SetAttackType(ATTACKTYPE AttackType) { m_Type = AttackType;}//�U���̎�ނ�ݒ肷��
	const ATTACKTYPE & GetAttackType() const { return m_Type; }      //�U���̎�ނ��擾����
	static const string ATTACK_FILENAME[static_cast<int>(ATTACKTYPE::MAX)];//�U�����f���̃t�@�C���� 
private:
	ATTACKTYPE m_Type;//�^�C�v
};
//==================================================================================================================================================

//===========================================
//�v���C���[�U���N���X
//===========================================
class CAttackPlayer : public CAttack
{
public:
	CAttackPlayer(int nPri = 2);      //�R���X�g���N�^
	~CAttackPlayer();                 //�f�X�g���N�^
	HRESULT Init() override;          //����������
	void Uninit() override;           //�I������
	void Update() override;           //�X�V����
	void Draw() override;             //�`�揈��
	void SetDeath() override;         //���S�t���O��ݒ�
	static CAttackPlayer* Create(ATTACKTYPE AttackType, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale);//��������
private:
};
//==================================================================================================================================================

//===========================================
//�G�U���N���X
//===========================================
class CAttackEnemy : public CAttack
{
public:
	CAttackEnemy();                  //�R���X�g���N�^
	~CAttackEnemy();                 //�f�X�g���N�^
	HRESULT Init() override;          //����������
	void Uninit() override;           //�I������
	void Update() override;           //�X�V����
	void Draw() override;             //�`�揈��
	void SetDeath() override;         //���S�t���O��ݒ�
	static CAttackEnemy* Create(ATTACKTYPE AttackType, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale);//��������
private:

};
//==================================================================================================================================================
#endif
