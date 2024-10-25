//====================================================
//
//�P�O���Q�S���F�U���N���X����������[attack.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _BGMODEL_H_  
#define _BGMODEL_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "objectX.h"
//==========================================

//===========================================
//�w�i���f���N���X
//===========================================
class CBgModel : public CObjectX
{
public:
	typedef enum
	{
		TYPE_BILL_00 = 0,//�r��
		TYPE_TREE_00,    //��
		TYPE_MAX
	}BGMODELTYPE;

	CBgModel();                  //�R���X�g���N�^
	~CBgModel();                 //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static CBgModel* Create(BGMODELTYPE bgModelType,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale);//��������

	//==========================================================
	//�G�f�B�^�֌W
	//==========================================================
	//�֐�
	void SaveInfoTxt(fstream& WritingFile) override;  //�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	static void LoadInfoTxt(fstream& LoadingFile, vector<CObject*>& VecSaveManager, string& Buff);  //�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�   
	CObject* ManagerChengeObject(bool bAim) override; //�X�e�[�W�}�l�[�W���[�ɕύX�����I�u�W�F�N�g��n��
	CObject* ManagerSaveObject() override;             //�X�e�[�W�}�l�[�W���[�ɍ��̃I�u�W�F�N�g��ۑ�����
	//=================================================================================================================
protected:
	void SetBgModelType(BGMODELTYPE AttackType) { m_Type = AttackType; }//�w�i���f���̎�ނ�ݒ肷��
	const BGMODELTYPE& GetAttackType() const { return m_Type; }         //�w�i���f���̎�ނ��擾����
	static const string BGMODEL_FILENAME[BGMODELTYPE::TYPE_MAX];        //�w�i���f���̃t�@�C���� 
private:
	BGMODELTYPE m_Type;//�^�C�v
};
//==================================================================================================================================================
#endif