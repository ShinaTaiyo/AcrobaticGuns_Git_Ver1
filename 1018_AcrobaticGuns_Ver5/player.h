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
#include "objectXAlive.h"
#include "lockon.h"
#include "meshorbit.h"
#include "ui.h"
#include "player_actionmode.h"
#include "wire.h"
//==========================================

//===========================================
//�v���C���[3D�N���X
//===========================================
class CPlayer : public CObjectXAlive
{
public:
	CPlayer(CPlayerMove * pPlayerMove,CPlayerAttack * pPlayerAttack,CPlayerEffect * pPlayerEffect,CPlayerWireShot * pPlayer,
		int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::PLAYER, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                  //�R���X�g���N�^
	~CPlayer();                 //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static CPlayer * Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 move,D3DXVECTOR3 Scale);
	CLockon* GetLockOn() { return m_pLockOn; }//���b�N�I���̃C���X�^���X���擾

	//================================================
	//�����蔻��
	//================================================
	void SetSuccessCollision(bool bSuccess) { m_bCollision = bSuccess; }//�����蔻�肪�����������ǂ�����ݒ�
	const bool& GetCollisionSuccess() const { return m_bCollision; }//�����蔻�肪�����������ǂ������擾
	//===============================================================================================
	
	//================================================
	//���b�V��
	//================================================
	CMeshOrbit* GetMeshOrbit() { return m_pMeshOrbit; }//�O�Ղ��擾
	CWire* GetWire() { return m_pWire; }//���C���[���擾
	//===============================================================================================

	//================================================
	//���[�h�`�F���W
	//================================================
	void ChengeMoveMode(CPlayerMove* pPlayerMove);//�ړ����[�h��ς���
	void ChengeAttackMode(CPlayerAttack* pPlayerAttack);//�U�����[�h��ς���
	void ChengeEffectMode(CPlayerEffect* pPlayerEffect);//�G�t�F�N�g���[�h��ς���
	void ChengeWireShotMode(CPlayerWireShot* pPlayerWireShot);//���C���[���˃��[�h��ς���
	CPlayerWireShot* GetWireShotState() { return m_pWireShot; }//���C���[���ˏ�Ԃ��擾
	//===============================================================================================
private:
	//================================================
	//�A�N�V�������[�h�񋓌^
	//================================================
	enum class ACTIONMODE
	{
		SHOT = 0,
		DIVE,
		MAX
	};
	//===============================================================================================

	//================================================
    //�ÓI�����o
    //================================================
	static constexpr float m_fNORMALATTACK_SPEED = 10.0f;//�ʏ�U���̈ړ��ʂ�ݒ�
	//===============================================================================================

	//================================================
	//�ϐ��錾
	//================================================
	float m_fRotAim;                    //�ړI�̌���
	bool m_bCollision;                  //�����蔻�肪�����������ǂ���

	//���N���X�C���X�^���X
	CLockon* m_pLockOn;                 //���b�N�I���J�[�\���ւ̃|�C���^
	ACTIONMODE m_NowActionMode;         //���݂̃A�N�V�������[�h
	CUi * m_pModeDisp;                  //���[�h�\��UI
	CMeshOrbit* m_pMeshOrbit;           //�O��
	CWire* m_pWire;                     //���C���[
	
	//�s�����
	CPlayerMove* m_pMove;               //�ړ�����
	CPlayerAttack* m_pAttack;           //�U������
	CPlayerEffect* m_pEffect;           //�G�t�F�N�g����
	CPlayerWireShot* m_pWireShot;       //���C���[�V���b�g���
	//===============================================================================================


	//================================================
	//�v���g�^�C�v�錾
	//================================================

	//=============================
	//�ʒu�n
	//=============================
	void AdjustRot();//������������
	void AdjustPos();//�ʒu�𒲐�����
	//================================================

	//=============================
	//���[�h�`�F���W
	//=============================
	void ActionModeChenge();//�A�N�V�������[�h��ύX����
	//==============================================================================================

	//=============================
	//����n
	//=============================
	void CollisionProcess();//�����蔻�菈���S��
	void CollisionBlock();//�u���b�N�Ƃ̓����蔻�菈��
	//===============================================================================================

	//=============================
	//�W�����v�֌W
	//=============================
	void JumpProcess();     //�W�����v����
	//===============================================================================================
};

#endif
