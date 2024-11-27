//==========================================================================================
//
//�V���P�V���F�z�u�c�[�������[stagemanager.h]
//Author:ShinaTaiyo
//
//==========================================================================================

#ifndef _STAGEMANAGER_H_  
#define _STAGEMANAGER_H_

//============================
//�C���N���[�h
//============================
#include "main.h"
#include "object.h"
//========================================================

//============================
//�O���錾
//============================
class CObjectXInfo;
class CBg3D;
//======================
//�}�N����`
//======================
#define SAVE_BIN "data\\BINARYFILE\\Save.bin"
#define SAVE_TXT "data\\TEXTFILE\\stagemanager\\Save.txt"
//============================
//�X�e�[�W�Ǘ��N���X
//============================
class CStageManager : public CObject
{
public:
	enum class FOCUSTYPE
	{
		FOCUSTYPE_NORMAL = 0,//�J�������ǂ��ʒu������
		FOCUSTYPE_WARPPOS,   //�J���������[�v��̈ʒu��ǂ�
	};

	enum class WORLDTYPE
	{
		EASY = 0,
		NORMAL,
		BOSSRUSH,
		MAX
	};

	CStageManager(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::STAGEMANAGER, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);  //�R���X�g���N�^
	~CStageManager(); //�f�X�g���N�^
	HRESULT Init() override;   //����������
	void Uninit() override;     //�I������
	void Update() override;    //�X�V����
	void Draw() override;      //�`�揈��
	void SetDeath() override;  //���S�t���O��ݒ�
	void LoadMapTxt(int nMapNum);               //�}�b�v���e�L�X�g�t�@�C�����烍�[�h����
	void LoadMapFilePass(WORLDTYPE type);           //�}�b�v�̃t�@�C���p�X�����[�h����
	void SaveMapTxt(int nMapNum);               //�}�b�v���e�L�X�g�t�@�C���ɃZ�[�u����
	void LoadMapBin(int nMapNum);               //�}�b�v���o�C�i���t�@�C�����烍�[�h����
	void SaveMapBin();                          //�}�b�v���o�C�i���t�@�C���ɃZ�[�u����
	int GetMapIndex() { return m_nMapIndex; }   //���݂̃}�b�v�ԍ����擾����

	CObject* GetStageManagerObject() { return m_pManagerObject; }//����I�u�W�F�N�g�̎擾

	static CStageManager* Create();             //��������

	static const int m_nMAXMANAGEROBJECT = 1024;//�Ǘ�����u���b�N�̍ő吔
private:

	enum class MANAGERMODE
	{
		ALREADYSTAGE = 0,//���ɂ���X�e�[�W��ҏW���郂�[�h
		NEWSTAGE,        //�V�����X�e�[�W����郂�[�h
		MAX
	};

	static const int m_nMAX_MAP = 20;
	static const int m_nMAX_WORD = 126;
	static const char* m_apWORLDMAP_TXT[static_cast<int>(WORLDTYPE::MAX)];
	static const string m_aSAVE_FILENAME;//�ۑ�����t�@�C����

	void TypeChenge();                          //�I�u�W�F�N�gX�̎�ނ�ς���
	void SetObjectX();                          //�I�u�W�F�N�gX��ݒ肷��
	void DeleteManagerObject();                       //�I�u�W�F�N�gX������
	void ChengeObject(CObject::MANAGEROBJECTTYPE ManagerObjectType);                        //�I�u�W�F�N�g�̎�ނ�����
	void ReleaseObject();                       //�I�u�W�F�N�g�������[�X����
	void MapChenge();                           //�}�b�v��ς���
	void DispInfo();                            //���݂̃}�b�v�G�f�B�^�̏���\��
	void ChooseObject();                        //�I�u�W�F�N�g�I������

	//====================
	//��{�n
	//====================
	D3DXVECTOR3 m_SaveRot;                          //����
	D3DXVECTOR3 m_SavePos;                          //�ʒu
	D3DXVECTOR3 m_SaveScale;                        //�g�嗦
	D3DXVECTOR3 m_SaveBeforeChoosePos;          //�I������������O�̂���ʒu
	CObject* m_pManagerObject;                  //�F�X�Ȕh���N���X�ɃL���X�g����I�u�W�F�N�g
	MANAGERMODE m_ManagerMode;                  //���݂̃X�e�[�W�}�l�[�V���[�̃��[�h
	char m_aMapFilePass[m_nMAX_MAP][m_nMAX_WORD];//�}�b�v�̃t�@�C���p�X
	//========================================================================================

	//====================
	//�ԍ�
	//====================
	int m_nMapIndex;                            //�}�b�v�̃C���f�b�N�X
	int m_nWorldIndex;                          //���[���h�̃C���f�b�N�X
	int m_nMapNum;                              //�}�b�v�̑���
	//========================================================================================
	
	//====================
	//�I���n
	//====================
	CObject* m_pChooseObject;                   //�I���I�u�W�F�N�g
	bool m_bChooseObject;                       //�I�u�W�F�N�g��I�����Ă��邩�ǂ���
	//========================================================================================

	//===================
	//���[�h�֌W
	//===================
	bool m_bMakeMapMode;                        //�}�b�v����郂�[�h
	bool m_bUseSizeMove;//���݂̃I�u�W�F�N�g�̃T�C�Y���ړ����邩�ǂ���
	//========================================================================================

	//===================
	//�g�嗦�֌W
	//===================
	void ResetScale();//�g�嗦�����Z�b�g���鏈��
	//========================================================================================

	//===================
	//�I�u�W�F�N�g���X�g
	//===================
	list <CObject*> m_StgObjList;//�I�u�W�F�N�g�̃��X�g
	list <CObject*>::iterator m_StgObjIt;//�X�e�[�W�I�u�W�F�N�g���X�g�ւ̃C�e���[�^
	//========================================================================================

	//===================
	//3D�w�i
	//===================
	CBg3D* m_pBg3D;//�w�i�ւ̃|�C���^
	//========================================================================================
};
#endif