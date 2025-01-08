//===========================================================================
//
//�Q�O�Q�T�N�P���W���F���[�V�������Ǘ��N���X����[motionData.h]
//Author:ShinaTaiyo
//
//===========================================================================

//=====================================
//�C���N���[�h
//=====================================
#include "main.h"
#include "objectX.h"
#include "modelparts.h"
//========================================================================

//=====================================
//�N���X
//=====================================
class CCharacter : public CObjectX
{
public:
	struct KEY
	{
		D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };//�ʒu
		D3DXVECTOR3 Rot = { 0.0f,0.0f,0.0f };//����
	};

	struct KEYSET
	{
		int nFrame = 0;    //�t���[����
		vector<KEY> VecKey;//�e���f���p�[�c�̃L�[���
	};

	struct MOTION
	{
		bool bLoop = false;       //���[�v�����邩�ǂ���
		int nNumKey = 0;          //�L�[�̑���
		vector<KEYSET> VecKeySet; //�L�[�Z�b�g���
	};

	struct CHARACTERMOTIONDATA
	{
		string FileName = "";//���[�V�����t�@�C����
		vector<MOTION> VecMotion;//���[�V�������
	};

	CCharacter(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                                                           //�R���X�g���N�^
	~CCharacter() override; //�f�X�g���N�^
	HRESULT Init() override;//����������
	void Uninit() override; //�I������
	void Update() override; //�X�V����
	void Draw() override;   //�`�揈��
	void SetDeath() override;//���S�t���O�ݒ菈��

	static int Regist(string String,CCharacter * pCharacter);//���[�V�������̃t�@�C���p�X���w�肵�A���[�V�������̃C���f�b�N�X�ƃ��f���p�[�c���擾
private:
	//=======================
	//�ÓI�����o
	//=======================
	static vector<CHARACTERMOTIONDATA> s_VecMotionInfo;//���[�V�������̃x�N�^�[
	//===================================================================================

	//=======================
	//�\����
	//=======================
	struct NowMotionInfo
	{
		int nNowMotion = 0;//���݂̃��[�V�����ԍ�
		int nNextMotion = 0;//���̃��[�V����
		int nMotionOld = 0;//1f�O�̃��[�V����

		int nCntFrame = 0; //���݂̃t���[����
		int nCntKey = 0;   //���݂̃L�[���J�E���g����
	};
	//===================================================================================

	//=======================
	//�ϐ�
	//=======================
	int m_nIdxCharacter;//�L�����N�^�[���[�V�������̃C���f�b�N�X
	NowMotionInfo m_NowMotionInfo;//���[�V������ԏ��
	vector <CModelParts*> m_VecModelParts;//���f���p�[�c�̃x�N�^�[
	//===================================================================================

	//=======================
	//�֐�
	//=======================
	void MotionProcess();//���[�V�����������s��
	void MotionInfoChengeProcess();//���[�V���������X�V���鏈��

	//�ÓI�����o
	static void SaveMotion(string MotionFileName);//���[�V��������ۑ�����
	static void LoadModelParts(string MotionFileName, CCharacter* pCharacter);//���f���p�[�c��ǂݍ���
	//===================================================================================
};
//============================================================================================================
#pragma once
