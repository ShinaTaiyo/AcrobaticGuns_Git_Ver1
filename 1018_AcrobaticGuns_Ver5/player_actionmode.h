//======================================================================================
//
//�P�P���P�O���F�v���C���[�A�N�V�������[�h�N���X�����[player_actionmode.h]
//Author:ShinaTaiyo
//
//======================================================================================

//============================================
//�Q�d�C���N���[�h�h�~
//============================================
#ifndef _PLAYER_ACTIONMODE_H_
#define _PLAYER_ACTIONMODE_H_
//======================================================================================

//============================================
//�C���N���[�h
//============================================
#include "main.h"
#include "object2d.h"
//======================================================================================

//============================================
//�O���錾
//============================================
class CPlayer;

////============================================
////�X�[�p�[�N���X
////============================================
//
////�v���C���[�A�N�V�������[�h�N���X
//class CPlayer_ActionMode
//{
//public:
//	CPlayer_ActionMode();        //�R���X�g���N�^
//	virtual ~CPlayer_ActionMode(); //�f�X�g���N�^
//	virtual void Attack(CPlayer* pPlayer) {};           //�U��
//	virtual void Move(CPlayer* pPlayer) {};             //�ړ���
//	virtual void Collision(CPlayer* pPlayer) {};        //�����蔻�菈��
//
//protected:
//	void PlayerControl(CPlayer* pPlayer);               //�v���C���[�𓮂�������
//private:
//};
////======================================================================================
//
////============================================
////�A�N�V�������[�h�N���X
////============================================
//
////�v���C���[�V���b�g�N���X
//class CPlayerShot : public CPlayer_ActionMode
//{
//public:
//	CPlayerShot();//�R���X�g���N�^
//	~CPlayerShot() override;//�f�X�g���N�^
//	void Attack(CPlayer* pPlayer) override;       //�U��
//	void Move(CPlayer* pPlayer) override;         //�ړ�
//	void Collision(CPlayer* pPlayer) override;    //�����蔻�菈��
//private:
//};
//
////�v���C���[�_�C�u�N���X
//class CPlayerDive : public CPlayer_ActionMode
//{
//public:
//	CPlayerDive(); //�R���X�g���N�^
//	~CPlayerDive() override;//�f�X�g���N�^
//	void Move(CPlayer* pPlayer) override;   //�ړ�
//	void Collision(CPlayer* pPlayer) override;    //�����蔻�菈��
//private:
//	bool m_bCollision;//���肵�����ǂ������擾
//	bool m_bActiveDive;//�_�C�u�����ǂ���
//};
//======================================================================================

//==============================================
//�ړ��N���X
//==============================================

//�X�[�p�[�N���X
class CPlayerMove
{
public:
	CPlayerMove();                             //�R���X�g���N�^
	virtual ~CPlayerMove();                    //�f�X�g���N�^
	virtual void MoveProcess(CPlayer* pPlayer);//�ړ�����
};

//�ʏ�ړ��N���X
class CPlayerMove_Normal : public CPlayerMove
{
public:
	CPlayerMove_Normal();                    //�R���X�g���N�^
	~CPlayerMove_Normal() override;                   //�f�X�g���N�^
	void MoveProcess(CPlayer* pPlayer) override;//�ړ�����
};

//�_�C�u�ړ��N���X
class CPlayerMove_Dive : public CPlayerMove
{
public:
	CPlayerMove_Dive();                      //�R���X�g���N�^
	~CPlayerMove_Dive() override;                     //�f�X�g���N�^
	void MoveProcess(CPlayer* pPlayer) override;//�ړ�����
	void SetDiveMove(D3DXVECTOR3 Move) { m_DiveMove = Move; }//�_�C�u���̈ړ��ʂ�ݒ�
private:
	D3DXVECTOR3 m_DiveMove;//�_�C�u�̈ړ���
};

//�_�C�u�����N���X
class CPlayerMove_PrepDive : public CPlayerMove
{
public:
	CPlayerMove_PrepDive();                      //�R���X�g���N�^
	~CPlayerMove_PrepDive() override;            //�f�X�g���N�^
	void MoveProcess(CPlayer* pPlayer) override; //�ړ�����
};

//�ړ��ł��Ȃ��N���X
class CPlayerMove_Dont : public CPlayerMove
{
public:
	CPlayerMove_Dont();//�R���X�g���N�^
	~CPlayerMove_Dont() override;//�f�X�g���N�^
	void MoveProcess(CPlayer* pPlayer) override;//�ړ�����
private:
};
//======================================================================================



//==============================================
//�U���N���X
//==============================================

//�X�[�p�[�N���X
class CPlayerAttack
{
public:
	CPlayerAttack() {};//�R���X�g���N�^
	virtual ~CPlayerAttack() {};//�f�X�g���N�^
	virtual void AttackProcess(CPlayer* pPlayer) {};//�U������
};

//�ˌ��N���X
class CPlayerAttack_Shot : public CPlayerAttack
{
public:
	CPlayerAttack_Shot();//�R���X�g���N�^
	~CPlayerAttack_Shot() override;//�f�X�g���N�^
	void AttackProcess(CPlayer* pPlayer) override;//�U������
};

//�_�C�u�N���X
class CPlayerAttack_Dive : public CPlayerAttack
{
public:
	CPlayerAttack_Dive();//�R���X�g���N�^
	~CPlayerAttack_Dive();//�f�X�g���N�^
	void AttackProcess(CPlayer* pPlayer) override;//�U������
};

//�U���ł��Ȃ��N���X
class CPlayerAttack_Dont : public CPlayerAttack
{
public:
	CPlayerAttack_Dont();//�R���X�g���N�^
	~CPlayerAttack_Dont() override;//�f�X�g���N�^
	void AttackProcess(CPlayer* pPlayer) override;//�U������
};
//======================================================================================


#endif