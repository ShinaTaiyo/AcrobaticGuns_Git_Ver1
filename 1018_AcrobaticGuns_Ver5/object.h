//====================================================
//
//�V���P�U���F�I�u�W�F�N�g�̃��X�g�Ǘ�[Object.cpp]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _OBJECT_H_  
#define _OBJECT_H_

//======================
//�C���N���[�h
//======================
#include "main.h"

//==========================================

//==========================================
//�I�u�W�F�N�g�N���X
//==========================================
class CObject
{
public:
	//===========================
	//�I�u�W�F�N�g��ޗ񋓌^
	//===========================
	typedef enum
	{
		TYPE_NONE,            //�^�C�v�Ȃ�
		TYPE_PLAYER,          //�v���C���[3
		TYPE_ENEMY,           //�G2
		TYPE_BULLET,          //�e4
		TYPE_BLOCK,           //�u���b�N0
		TYPE_BG,              //�w�i
		TYPE_FIELD,           //��
		TYPE_STAGEMANAGER,    //�X�e�[�W�}�l�[�W���[
		TYPE_FADE,            //�t�F�[�h4
		TYPE_ATTACK,          //�U��
		TYPE_BGMODEL,         //�w�i���f��
		TYPE_UI3D,            //UI3D
		TYPE_LOCKON,          //���b�N�I��
		TYPE_PARTICLE,        //�p�[�e�B�N��
		TYPE_MAX
	}TYPE;
	//===========================================

	//====================================================
	//�X�e�[�W�}�l�[�W���[�Ǘ��I�u�W�F�N�g�^�C�v�񋓌^
	//====================================================
	typedef enum
	{
		MANAGEROBJECTTYPE_NONE = 0,//�Ȃ�
		MANAGEROBJECTTYPE_BLOCK,   //�u���b�N
		MANAGEROBJECTTYPE_BGMODEL, //�w�i���f��
		MANAGEROBJECTTYPE_MAX
	}MANAGEROBJECTTYPE;
	//===========================================

	//===========================
	//�I�u�W�F�N�g���ޗ񋓌^
	//===========================
	enum class OBJECTTYPE
	{
		OBJECTTYPE_2D = 0,
		OBJECTTYPE_3D,
		OBJECTTYPE_BILLBOARD,
		OBJECTTYPE_X,
		OBJECTTYPE_MAX
	};
	//===========================================

	CObject(int nPriority = 2);//�`��D��ݒ�(�T���Q�W��New!�F�f�t�H���g�����i�Ăяo�����Ɉ����ݒ肵�Ȃ���΂R���������ď��������
	virtual ~CObject();        //�f�X�g���N�^
	virtual HRESULT Init() = 0;//����������
	virtual void Uninit() = 0; //�I������
	virtual void ExtraUninit();//�ʘg�̏I������
	virtual void Update() = 0; //�X�V����
	virtual void Draw() = 0;   //�`�揈��
	bool GetUseDeath() { return m_bUseDeath; }//���S�t���O�𔭓����邩���߂�t���O���擾
	static void ReleaseAll();  //�S�I�u�W�F�N�g�J��
	static void UpdateAll();   //�S�I�u�W�F�N�g�X�V
	static void DrawAll();     //�S�I�u�W�F�N�g�`��
	virtual void SetDeath();                                              //���S�t���O��ݒ�
	//=====================
	//�擾�n
	//=====================
	static int GetNumAll();                              //�S�I�u�W�F�N�g�̑������擾����
	bool GetCreateSuccess() { return m_bCreateSuccess; } //�I�u�W�F�N�g�̐����ɐ����������ǂ���
	int GetId() { return m_nID; }                        //���݂�ID���擾����
	static CObject* GetCObject(int nIdx,int nPriority);  //�I�u�W�F�N�g�擾
	TYPE GetType();                                      //�^�C�v�擾
	//==============================================================================================
	void SetType(TYPE type);                             //�^�C�v�ݒ�
	void SetStageManagerObjNum(int nNum) { m_nStageManagerObjNum = nNum; }            //�X�e�[�W�}�l�[�W���[�ŊǗ�����Ă���I�u�W�F�N�g�̔ԍ���ݒ�
	int GetStageManagerObjNum() { return m_nStageManagerObjNum; }                     //�X�e�[�W�}�l�[�W���[�ŊǗ�����Ă���I�u�W�F�N�g�̔ԍ����擾����
	static const int m_nMAXOBJECT = 1024;                                             //�I�u�W�F�N�g�ő吔
	static const int m_nMAXPRIORITY = 10;                                              //�`�揇�ő吔
	static void ReleaseProcess();                                                     //���X�g�̔j�������鏈��
	void Release();                                                                   //�������g�̊J��
	void SetUseDeath(bool bDeath) { m_bUseDeath = bDeath; }                           //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
	static void SetReleaseAll() { m_bActivationReleaseAll = true; }                   //ReleaseAll�𔭓�������t���O��ݒ肷��
	void SetObjectType(OBJECTTYPE type) { m_ObjectType = type; }                      //�I�u�W�F�N�g�̕��ނ����߂�
	OBJECTTYPE GetObjectType() { return m_ObjectType; }                               //�I�u�W�F�N�g�̕��ނ��擾����
	CObject* GetNextObject() {return m_pNext; }                                       //���̃I�u�W�F�N�g���擾����
	CObject* GetPrevObject() { return m_pPrev; }                                      //�O�̃I�u�W�F�N�g���擾����

	//==============================
	//�X�e�[�W�}�l�[�W���[�֌W
	//==============================
	CObject* GetNextStageManagerObject() { return m_pStageManagerObj_Next; }          //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���I�u�W�F�N�g�̎��̃I�u�W�F�N�g���擾����
	CObject* GetPrevStageManagerObject() { return m_pStageManagerObj_Prev; }          //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���I�u�W�F�N�g�̑O�̃I�u�W�F�N�g���擾����
	static CObject* GetTopStageManagerObject() { return m_pStageManagerObj_Top; }     //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���I�u�W�F�N�g�̃g�b�v�I�u�W�F�N�g���擾����
	static CObject* GetCurStageManagerObject() { return m_pStageManagerObj_Cur; }     //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���I�u�W�F�N�g�̃J�����g�I�u�W�F�N�g���擾����
	void SetStageManagerObj();                                                        //�X�e�[�W�}�l�[�W���[�ŊǗ�����I�u�W�F�N�g��ݒ�

	static int GetTotalStageManagerObjNum() { return m_nNumStageManagerObject; }      //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���I�u�W�F�N�g�̑������擾
	static void StageManagerObjectReleaseAll();                                            //�S�Ẵ}�l�[�W���[�Ǘ��̃I�u�W�F�N�g������

	//���ޗp
	void SetManagerObjectType(MANAGEROBJECTTYPE Type) { m_ManagerObjectType = Type; }     //�X�e�[�W�}�l�[�W���[�Ǘ��^�C�v��ݒ�
	const MANAGEROBJECTTYPE & GetManagerObjectType() const { return m_ManagerObjectType; }  //�X�e�[�W�}�l�[�W���[�Ǘ��^�C�v���擾
	//==============================================================================================

	//==============================
	//���X�g�֌W
	//==============================
	static CObject* GetTopObject(int nPriority){ return m_pTop[nPriority];}           //�g�b�v�I�u�W�F�N�g���擾����
	static CObject* GetCurrentObject(int nPriority) { return m_pCur[nPriority];}      //�J�����g�I�u�W�F�N�g���擾����
	void SetStageManagerChoose(bool bChoose) { m_bStageManagerChooseState = bChoose; }//�X�e�[�W�}�l�[�W���[���I�����Ă��邩�ǂ���
	bool GetStageManagerChoose() { return m_bStageManagerChooseState; }               //�X�e�[�W�}�l�[�W���[���I�����Ă��邩�ǂ������擾����
	//==============================================================================================

	//=================================================
	//�t���[���֌W
	//=================================================
	int GetCntFrame() { return m_nCntFrame; }                                         //�o�����Ă���̃t���[�������J�E���g����
	//================================================================================================

	//=================================================
	//�G�f�B�^�֌W
	//=================================================
	virtual void SaveInfoTxt(fstream& WritingFile);   //�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	virtual void ManagerChooseControlInfo();          //��񑀍�
	virtual CObject * ManagerChengeObject(bool bAim); //�X�e�[�W�}�l�[�W���[�ɕύX�����I�u�W�F�N�g��n��
	virtual CObject * ManagerSaveObject();            //�X�e�[�W�}�l�[�W���[�ɍ��̃I�u�W�F�N�g��ۑ�����
	//================================================================================================

	//=================================================
	//�T�E���h�֌W
	//=================================================
	void SetUseInitialSound(bool bUse) { m_bUseInitialSound = bUse; }//���������ɃT�E���h���Đ����邩�ǂ���
	bool GetUseInitialSound() { return m_bUseInitialSound; }         //���������ɃT�E���h�������������邩�ǂ������擾
	//================================================================================================
protected:
private:
	void ReleaseStageManagerObj();                             //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���I�u�W�F�N�g��j��
	static CObject* m_apObject[m_nMAXPRIORITY][m_nMAXOBJECT];//�I�u�W�F�N�g�Ǘ�
	static bool m_bActivationReleaseAll;                     //ReleaseAll�𔭓����邩�ǂ���

	//===============================================
	//���X�g�Ǘ�
	//===============================================
	static CObject* m_pTop[m_nMAXPRIORITY];                  //�擪�̃I�u�W�F�N�g�̃|�C���^
	static CObject* m_pCur[m_nMAXPRIORITY];                  //���݁i�Ō���j�̃|�C���^

	CObject* m_pNext;                                        //���̃I�u�W�F�N�g�ւ̃|�C���^!
	CObject* m_pPrev;                                        //�O�̃I�u�W�F�N�g�ւ̃|�C���^!

	bool m_bDeath;                                           //���S�t���O!
	bool m_bUseDeath;                                        //���S�t���O�𔭓����邩�ǂ���!
	//===============================================================================================

	//===============================================
	//�X�e�[�W�}�l�[�W���[�֌W
	//===============================================
	static CObject* m_pStageManagerObj_Top;                  //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���擪�I�u�W�F�N�g�ւ̃|�C���^
	static CObject* m_pStageManagerObj_Cur;                  //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă��Ō���I�u�W�F�N�g�ւ̃|�C���^
	static int m_nNumStageManagerObject;                     //�X�e�[�W�}�l�[�W���[�ŊǗ�����Ă���I�u�W�F�N�g�̐�

	CObject* m_pStageManagerObj_Next;                        //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă��鎟�̃I�u�W�F�N�g�ւ̃|�C���^!
	CObject* m_pStageManagerObj_Prev;                        //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���O�̃I�u�W�F�N�g�ւ̃|�C���^!
	bool m_bIsStageManagerObj;                               //�X�e�[�W�}�l�[�W���[�ŊǗ�����Ă���I�u�W�F�N�g���ǂ���!
	int m_nStageManagerObjNum;                               //�X�e�[�W�}�l�[�W���[�ŊǗ�����Ă���I�u�W�F�N�g�̔ԍ�!
	bool m_bStageManagerChooseState;                         //�X�e�[�W�}�l�[�W���[���I�����Ă��邩�ǂ���!

	//���ޗp
	MANAGEROBJECTTYPE m_ManagerObjectType;                   //�Ǘ��ԍ����ʗp

	//================================================================================================

	static int m_nNumAll;                                    //�I�u�W�F�N�g����

    //=================================================
	//��{�n
	//=================================================
	TYPE m_type;                                             //�I�u�W�F�N�g���!
	OBJECTTYPE m_ObjectType;                                 //�I�u�W�F�N�g����!
	int m_nID;                                               //�������g��ID!
	int m_nPriority;                                         //�`�揇�𑗂�!
	bool m_bCreateSuccess;                                   //�I�u�W�F�N�g�̐����������������ǂ���!
	//================================================================================================

	//=================================================
	//�t���[���֌W
	//=================================================
	int m_nCntFrame;                                         //�o�����Ă���̃t���[�������J�E���g����!
	//================================================================================================

	//=================================================
	//�T�E���h�֌W
	//=================================================
	bool m_bUseInitialSound;//���������ɃT�E���h���Đ����邩�ǂ���!
	//================================================================================================
};

#endif
