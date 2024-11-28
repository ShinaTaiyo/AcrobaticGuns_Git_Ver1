//====================================================
//
//�P�O���Q�X���F�G�����[enemy.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _ENEMY_H_  
#define _ENEMY_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "objectXAlive.h"
#include "lockon.h"
//==========================================

//===========================================
//�G�l�~�[�N���X
//===========================================
class CEnemy : public CObjectXAlive
{
public:
	enum class ENEMYTYPE
	{
		SHOTWEAK = 0,
		DIVEWEAK,
		MAX
	};

	CEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                   //�R���X�g���N�^
	~CEnemy();                  //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	//static CEnemy* Create(ENEMYTYPE Type,int nLife,D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 Scale);

	//==========================================================
    //�G�f�B�^�֌W
    //==========================================================
    //�֐�
	void SaveInfoTxt(fstream& WritingFile) override;  //�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�   
	//CObject* ManagerChengeObject(bool bAim) override; //�X�e�[�W�}�l�[�W���[�ɕύX�����I�u�W�F�N�g��n��
	CObject* ManagerSaveObject() override;             //�X�e�[�W�}�l�[�W���[�ɍ��̃I�u�W�F�N�g��ۑ�����
	//=================================================================================================================
protected:
	//================================================
    //�ÓI�����o
    //================================================
	//static const string m_aENEMY_FILENAME[static_cast<int>(ENEMYTYPE::MAX)];//�G�̃��f���t�@�C����
    //===============================================================================================

	//================================================
	//�v���g�^�C�v�錾
	//================================================

	//===============================================================================================

	//================================================
	//�ϐ��錾
	//================================================
	void SetEnemyType(ENEMYTYPE type) { m_Type = type; }//�G�̎�ނ�ݒ�
	//===============================================================================================
private:
	//================================================
	//�ÓI�����o
	//================================================

	//===============================================================================================

	//================================================
	//�ϐ��錾
	//================================================
	ENEMYTYPE m_Type;//�G�̎��
	//===============================================================================================

	//================================================
	//�v���g�^�C�v�錾
	//================================================
	void CollisionProcess();//�����蔻����s��
	//===============================================================================================
};

//===========================================
//�ˌ��Ɏア�G�l�~�[�N���X
//===========================================
class CShotWeakEnemy : public CEnemy
{
public:
	enum class SHOTWEAKENEMYTYPE
	{
		NORMAL = 0,
		MAX
	};

	CShotWeakEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                   //�R���X�g���N�^
	~CShotWeakEnemy();                  //�f�X�g���N�^
	HRESULT Init() override;            //����������
	void Uninit() override;             //�I������
	void Update() override;             //�X�V����
	void Draw() override;               //�`�揈��
	void SetDeath() override;           //���S�t���O��ݒ�
	static CShotWeakEnemy* Create(SHOTWEAKENEMYTYPE Type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);

	//==========================================================
    //�G�f�B�^�֌W
    //==========================================================
    //�֐�
	void SaveInfoTxt(fstream& WritingFile) override;  //�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�   
	CObject* ManagerChengeObject(bool bAim) override; //�X�e�[�W�}�l�[�W���[�ɕύX�����I�u�W�F�N�g��n��
	CObject* ManagerSaveObject() override;             //�X�e�[�W�}�l�[�W���[�ɍ��̃I�u�W�F�N�g��ۑ�����
	//=================================================================================================================
protected:
private:
	//================================================
	//�ÓI�����o
	//================================================
	static const string s_aSHOTWEAKENEMY_FILENAME[static_cast<int>(SHOTWEAKENEMYTYPE::MAX)];//�G�̃��f���t�@�C����
	//===============================================================================================

	//================================================
	//�ϐ��錾
	//================================================
	SHOTWEAKENEMYTYPE m_ShotWeakEnemyType;//�G�̎��
	//===============================================================================================

	//================================================
	//�v���g�^�C�v�錾
	//================================================

	//===============================================================================================
};

//===========================================
//�_�C�u�Ɏア�G�l�~�[�N���X
//===========================================
class CDiveWeakEnemy : public CEnemy
{
public:
	enum class DIVEWEAKENEMYTYPE
	{
		NORMAL = 0,
		MAX
	};

	CDiveWeakEnemy(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ENEMY, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                   //�R���X�g���N�^
	~CDiveWeakEnemy();                  //�f�X�g���N�^
	HRESULT Init() override;            //����������
	void Uninit() override;             //�I������
	void Update() override;             //�X�V����
	void Draw() override;               //�`�揈��
	void SetDeath() override;           //���S�t���O��ݒ�
	static CDiveWeakEnemy* Create(DIVEWEAKENEMYTYPE Type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);

	//==========================================================
    //�G�f�B�^�֌W
    //==========================================================
    //�֐�
	void SaveInfoTxt(fstream& WritingFile) override;  //�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	static void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff);  //�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�   
	CObject* ManagerChengeObject(bool bAim) override; //�X�e�[�W�}�l�[�W���[�ɕύX�����I�u�W�F�N�g��n��
	CObject* ManagerSaveObject() override;             //�X�e�[�W�}�l�[�W���[�ɍ��̃I�u�W�F�N�g��ۑ�����
	//=================================================================================================================
protected:
private:
	//================================================
	//�ÓI�����o
	//================================================
	static const string s_aDIVEWEAKENEMY_FILENAME[static_cast<int>(DIVEWEAKENEMYTYPE::MAX)];//�G�̃��f���t�@�C����
	//===============================================================================================

	//================================================
	//�ϐ��錾
	//================================================
	DIVEWEAKENEMYTYPE m_DiveWeakEnemyType;//�G�̎��
	//===============================================================================================

	//================================================
	//�v���g�^�C�v�錾
	//================================================

	//===============================================================================================
};



#endif
