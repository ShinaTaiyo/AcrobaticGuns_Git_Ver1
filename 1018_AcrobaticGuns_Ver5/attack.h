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
		MAGICSWORD,
		MAX
	};

	enum class COLLISIONTYPE
	{
		NONE = 0,
		SQUARE,
		RECTANGLE_XZ,
		MAX
	};

	enum class TARGETTYPE
	{
		NONE = 0,
		PLAYER,
		ENEMY,
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
	const bool& GetCollisionRelease() const{ return m_bCollisionRelease; }

	//����^�C�v��ݒ肷��
	void SetCollisionType(COLLISIONTYPE Type) { m_CollisionType = Type; }
	const COLLISIONTYPE GetCollisionType() const { return m_CollisionType; }

	//�^�[�Q�b�g�^�C�v��ݒ肷��
	void SetTargetType(TARGETTYPE Type) { m_TargetType = Type; }
	const TARGETTYPE GetTargetType() const { return m_TargetType; }

	//�����Ă���I�u�W�F�N�g�ȊO�ɂ������邩�ǂ�����ݒ�
	void SetHitOtherThanLibing(bool bHit) { m_bHitOtherThanLiving = bHit; }
	const bool& GetHitOtherThanLibing() const { return m_bHitOtherThanLiving; }

	//�����蔻����U���N���X�ɔC���邩�ǂ���
	void SetAutoCollision(bool bAuto) { m_bAutoCollision = bAuto; }
	const bool& GetAutoCollision() { return m_bAutoCollision; }


protected:
	void SetAttackType(ATTACKTYPE AttackType) { m_Type = AttackType;}//�U���̎�ނ�ݒ肷��
	const ATTACKTYPE & GetAttackType() const { return m_Type; }      //�U���̎�ނ��擾����
	static const string ATTACK_FILENAME[static_cast<int>(ATTACKTYPE::MAX)];//�U�����f���̃t�@�C���� 
	const bool& GetCollisionSuccess() const { return m_bCollisionSuccess; }
private:

	//================================================
	//�ϐ��錾
	//================================================
	int m_nPower;     //�U����

	bool m_bCollisionRelease;//�Փˎ��ɏ������ǂ���

	bool m_bHitOtherThanLiving;//�����Ă���I�u�W�F�N�g�ȊO�ɂ������邩�ǂ���
	bool m_bAutoCollision;     //�����蔻����U���N���X�ɔC���邩�ǂ���

	bool m_bCollisionSuccess; //���肪�����������ǂ���

	HitStop m_HitStop;//�q�b�g�X�g�b�v
	ATTACKTYPE m_Type;//�^�C�v
	COLLISIONTYPE m_CollisionType;//����^�C�v
	TARGETTYPE m_TargetType;//�_���I�u�W�F�N�g�̎��
	//==========================================================================================

	//================================================
	//�v���g�^�C�v�錾
	//================================================
	void Collision();//�����蔻����s������
	void CollisionProcess(bool& bCollision,bool & bNowCollision,CObjectXAlive * pObjX);
	//==========================================================================================
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
	static CAttackPlayer* Create(ATTACKTYPE AttackType,TARGETTYPE TargetType,COLLISIONTYPE CollisionType,bool bHitOtherThanLiving,bool bAutoCollision,int nPower,int nSetHitStopTime,int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale);//��������
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
	static CAttackEnemy* Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,bool bHitOtherThanLiving,bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale);//��������
private:

};
//==================================================================================================================================================
#endif
