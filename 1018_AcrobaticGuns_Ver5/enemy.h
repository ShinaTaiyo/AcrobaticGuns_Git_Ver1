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
#include "aimodel.h"
//==========================================

//===========================================
//�O���錾
//===========================================
class CEnemyMove;
class CAttackEnemy;

//===========================================
//�G�l�~�[�N���X
//===========================================
class CEnemy : public CObjectXAlive
{
public:
	struct MoveAiInfo
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Rot;
		D3DXVECTOR3 Scale;
	};

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
	void ManagerChooseControlInfo() override;          //�X�e�[�W�}�l�[�W���[���瑀�삷��
	void SetPhaseNum(int nNum) { m_nPhaseNum = nNum; } //�t�F�[�Y�ԍ���ݒ肷��
	const int& GetPhaseNum() const { return m_nPhaseNum; }//�t�F�[�Y�ԍ����擾����   
 	//=================================================================================================================

	//==========================================================
	//�ړ�AI
	//==========================================================
	void SetVecMoveAiInfo(vector<CAIModel*>& vec);
	vector<CAIModel*>& GetVecAiModelInfo() { return m_VecMoveAi; }
	void AIMoveProcess();//AI�ړ�����
	virtual void BattleMoveProcess();//�o�g���ړ�����
	void ChengeMove(CEnemyMove* pEnemyMove);//�ړ���Ԃ�ς���
	void RayCollision();                               //���C���I�u�W�F�N�g�ɓ������Ă��邩�ǂ���
	//=================================================================================================================

	//==========================================================
	//�o�g���֌W
	//==========================================================
	const int GetCntTime() const { return m_nCntTime; }

	//���m�˒�
	void SetSensingRange(float fRange) { m_fSensingRange = fRange; }
	const float & GetSensingRange() const { return m_fSensingRange; }

	//�ʏ�ړ����x
	void SetNormalSpeed(float fSpeed) { m_fNormalSpeed = fSpeed; }
	const float& GetNormalSpeed() const { return m_fNormalSpeed; }

	//�p�^�[��


	//=================================================================================================================

	static int GetNumEnemy() { return m_nNumEnemy; }
protected:
	//struct  MoveAiInfo
	//{
	//	D3DXVECTOR3 Pos;
	//	D3DXVECTOR3 Rot;
	//	D3DXVECTOR3 Scale;
	//};

	//struct PhaseSaveInfo
	//{
	//	D3DXVECTOR3 Pos;     //�ʒu
	//	D3DXVECTOR3 Rot;     //����
	//	D3DXVECTOR3 Scale;   //�g�嗦
	//	int nLife;           //�̗�
	//	ENEMYTYPE EnemyType; //�G�^�C�v
	//	int nTypeNum;        //�N���X���Ƃ̃^�C�v
	//	int nPhaseNum;       //�t�F�[�Y�ԍ���ݒ�

	//	vector<MoveAiInfo> VecMoveAi;//�ړ�AI��Vector
	//};
	////================================================
 //   //�ÓI�����o
 //   //================================================
	//static list<PhaseSaveInfo>
 //   //===============================================================================================

	struct Pattern
	{
		int nPattern;
		int nSubPattern;
		int nPatternTime;
		bool bAction;
	};

	//================================================
	//�ϐ��錾
	//================================================
	void SetEnemyType(ENEMYTYPE type) { m_Type = type; }//�G�̎�ނ�ݒ�
	//===============================================================================================
	
	//================================================
	//�v���g�^�C�v�錾
	//================================================
	void SetPattern(int nPattern) { m_Pattern.nPattern = nPattern; }
	const int& GetPattern() const { return m_Pattern.nPattern; }

	void SetSubPattern(int nSubPattern) { m_Pattern.nSubPattern = nSubPattern; }
	const int& GetSubpattern() const { return m_Pattern.nSubPattern; }

	void SetPatternTime(int nPatternTime) { m_Pattern.nPatternTime = nPatternTime; }
	const int& GetPatternTime() const { return m_Pattern.nPatternTime; }

	void SetAction(bool bAction) { m_Pattern.bAction = bAction; }
	const bool& GetAction() const { return m_Pattern.bAction; }

	void SetUseCollisionDetection(bool bUse) { m_bCollisoinDetection = bUse; }

	const bool& GetCollisionDetection() const { return m_bActivateCollisionDetection; }
	//===============================================================================================

private:
	//================================================
	//�ÓI�����o
	//================================================
	static int m_nNumEnemy;//�G�̑���
	//===============================================================================================

	//================================================
	//�ϐ��錾
	//================================================
	ENEMYTYPE m_Type;//�G�̎��
	vector<CAIModel*> m_VecMoveAi;//�ړ�AI���f��
	D3DXVECTOR3 m_MoveAiSavePos;//�ړ�AI�̕ۑ�����ʒu
	int m_nIdxMoveAi;           //�ړ�AI�̈ړ��ʒu�̔z����w�肷��
	int m_nPhaseNum;            //�t�F�[�Y�ԍ�
	CEnemyMove* m_pEnemyMove;   //�G�̈ړ��X�e�[�g
	int m_nCntTime;             //���Ԃ𑪂�
	float m_fRotMove;

	float m_fSensingRange;//���m�˒�
	float m_fNormalSpeed; //�ʏ�ړ����x
	
	Pattern m_Pattern;    //�p�^�[��

	bool m_bCollisoinDetection;//�Փ˔�����s�����ǂ���
	bool m_bActivateCollisionDetection;//�Փ˔��蔭��
	//===============================================================================================

	//================================================
	//�v���g�^�C�v�錾
	//================================================
	void CollisionProcess();//�����蔻����s��
	void SetMoveAiPoint();  //�ړ�AI�̔z�u������
	void PhaseNumDecision();//�t�F�[�Y�ԍ���ݒ肷��
	virtual void AttackProcess();//�U������
	void CollisionDetectionProcess();//�Փ˔��菈��

	//*�X�e�[�^�X�G�f�B�b�g
	void EditNormalSpeed();//�ړ����x
	void EditSensingRange();//���G�͈�
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
	static CShotWeakEnemy* Create(SHOTWEAKENEMYTYPE Type, int nLife,int nPhaseNum,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);

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
	static const float s_fATTACKSTART_LENGTH;
	static const float s_fNORMAL_SENSIINGRANGE;
	static const int s_nATTACK_COOLTIME;//�U���̃N�[���^�C��
	//===============================================================================================

	//================================================
	//�ϐ��錾
	//================================================
	SHOTWEAKENEMYTYPE m_ShotWeakEnemyType;//�G�̎��
	CAttackEnemy* m_pMagicSword;//���@��
	D3DXVECTOR3 m_SaveAimPos;//�ړI�̈ʒu�ۑ��p
	int m_nAttackCoolTime;//�U���̃N�[���^�C��
	//===============================================================================================

	//================================================
	//�v���g�^�C�v�錾
	//================================================

	//*�ړ�AI
	void BattleMoveProcess() override;//�o�g���ړ�����
	void AttackProcess() override;
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
	static CDiveWeakEnemy* Create(DIVEWEAKENEMYTYPE Type, int nLife,int nPhaseNum,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);

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
	static const int s_nATTACK_FREQUENCY;//�U���p�x
	static const float s_fSENSINGRANGE;  //���m�˒�
	static const float s_fNORMAL_SPEED;//�ʏ�̑��x
	//===============================================================================================

	//================================================
	//�ϐ��錾
	//================================================
	DIVEWEAKENEMYTYPE m_DiveWeakEnemyType;//�G�̎��

	//===============================================================================================

	//================================================
	//�v���g�^�C�v�錾
	//================================================
	
	//*�ړ�AI
	void BattleMoveProcess() override;//�o�g���ړ�����

	//�U������
	void AttackProcess() override;

	//===============================================================================================
};

//**************************************************************************************
//�G�̏�ԃN���X
//**************************************************************************************

//================================
//�ړ��N���X
//================================

//�X�[�p�[�N���X
class CEnemyMove
{
public:
	CEnemyMove();//�R���X�g���N�^
	virtual ~CEnemyMove();//�f�X�g���N�^
	virtual void Process(CEnemy* pEnemy);//����
};

//�ړ��^�C�v�FAI
class CEnemyMove_AI : public CEnemyMove
{
public:
	CEnemyMove_AI();//�R���X�g���N�^
	~CEnemyMove_AI() override;//�f�X�g���N�^
	void Process(CEnemy* pEnemy) override;
};

//�ړ��^�C�v�F�o�g��
class CEnemyMove_Battle : public CEnemyMove
{
public:
	CEnemyMove_Battle();//�R���X�g���N�^
	~CEnemyMove_Battle() override;//�f�X�g���N�^
	void Process(CEnemy* pEnemy) override;
};

//�ړ��^�C�v�F�Ȃ�
class CEnemyMove_None : public CEnemyMove
{
public:
	CEnemyMove_None();//�R���X�g���N�^
	~CEnemyMove_None() override;//�f�X�g���N�^
	void Process(CEnemy* pEnemy) override;
};

//===================================================================================================================================

#endif
