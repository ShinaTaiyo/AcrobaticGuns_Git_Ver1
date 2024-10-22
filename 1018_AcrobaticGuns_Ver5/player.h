//====================================================
//
//�V���S���F�`���[�W�U������������[player.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _PLAYER_H_  
#define _PLAYER_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
#include "objectXAlive.h"
//==========================================

//======================
//�O���錾
//======================
class CInputKeyboard;
class CInputJoypad;
class CGauge;
class CNumber;
class CScore;
//==========================================

//===========================================
//�v���C���[3D�N���X
//===========================================
class CPlayer : public CObjectXAlive
{
public:
	CPlayer();                  //�R���X�g���N�^
	~CPlayer();                 //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void ExtraUninit() override;//�ʘg�̏I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static void Unload();       //���f���̏��̔j��
	static CPlayer * Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 move,D3DXVECTOR3 Scale);
private:
	void MoveProcess();//�v���C���[�̈ړ�����
	float m_fRotAim;//�ړI�̌���	
};

#endif
