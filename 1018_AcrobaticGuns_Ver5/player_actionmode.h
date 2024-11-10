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

//============================================
//�X�[�p�[�N���X
//============================================

//�v���C���[�A�N�V�������[�h�N���X
class CPlayer_ActionMode : public CObject2D
{
public:
	CPlayer_ActionMode();        //�R���X�g���N�^
	~CPlayer_ActionMode() override; //�f�X�g���N�^
	HRESULT Init() override;        //����������
	void Uninit() override;         //�I������
	void Update() override;         //�X�V����
	void Draw() override;           //�`�揈��
	void SetDeath() override;       //���S�t���O�ݒ菈��
	virtual void Attack(CPlayer* pPlayer) {};           //�U��
	virtual void Move(CPlayer* pPlayer) {};             //�ړ���
private:
};
//======================================================================================

//============================================
//�A�N�V�������[�h�N���X
//============================================

//�v���C���[�V���b�g�N���X
class CPlayerShot : public CPlayer_ActionMode
{
public:
	CPlayerShot();//�R���X�g���N�^
	~CPlayerShot() override;//�f�X�g���N�^
	HRESULT Init() override;//����������
	void Uninit() override; //�I������
	void Update() override; //�X�V����
	void Draw() override;   //�`�揈��
	void SetDeath() override;//���S�t���O�ݒ菈��
	static CPlayerShot* Create(const CObject2D::POLYGONTYPE PolygonType,D3DXCOLOR col,const D3DXVECTOR3 Pos, const float fWidth, const float fHeight);
	void Attack(CPlayer* pPlayer) override; //�U��
private:
};

//�v���C���[�_�C�u�N���X
class CPlayerDive : public CPlayer_ActionMode
{
public:
	CPlayerDive(); //�R���X�g���N�^
	~CPlayerDive() override;//�f�X�g���N�^
	HRESULT Init() override;//����������
	void Uninit() override; //�I������
	void Update() override; //�X�V����
	void Draw() override;   //�`�揈��
	void SetDeath() override;//���S�t���O�ݒ菈��
	static CPlayerDive* Create(const CObject2D::POLYGONTYPE PolygonType, D3DXCOLOR col, const D3DXVECTOR3 Pos, const float fWidth, const float fHeight);
	void Move(CPlayer* pPlayer) override;   //�ړ�
};
//======================================================================================

#endif