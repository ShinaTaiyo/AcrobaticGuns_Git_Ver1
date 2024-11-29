//==========================================================================================
//
//�P�P���Q�X���F�t�F�[�Y�}�l�[�W���[�����[phasemanager.h]
//Author:ShinaTaiyo
//
//==========================================================================================

#ifndef _PHASEMANAGER_H_  
#define _PHASEMANAGER_H_

//============================
//�C���N���[�h
//============================
#include "main.h"
#include "object.h"
#include "enemy.h"

//========================================================

//======================
//�O���錾
//======================

//============================
//�t�F�[�Y�Ǘ��N���X
//============================
class CPhaseManager : public CObject
{
public:
	CPhaseManager(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::PHASEMANAGER, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);  //�R���X�g���N�^
	~CPhaseManager(); //�f�X�g���N�^
	HRESULT Init() override;   //����������
	void Uninit() override;     //�I������
	void Update() override;    //�X�V����
	void Draw() override;      //�`�揈��
	void SetDeath() override;  //���S�t���O��ݒ�
	static CPhaseManager * Create();           //��������
	void PushPhaseInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, int nLife, int nEnemyType, int nTypeNum, int nPhaseNum, vector<CEnemy::MoveAiInfo> VecMoveAi);        //��������
private:
	struct PhaseSaveInfo
	{
		D3DXVECTOR3 Pos;     //�ʒu
		D3DXVECTOR3 Rot;     //����
		D3DXVECTOR3 Scale;   //�g�嗦
		int nLife;           //�̗�
		int nEnemyType;      //�G�^�C�v
		int nTypeNum;        //�N���X���Ƃ̃^�C�v
		int nPhaseNum;       //�t�F�[�Y�ԍ���ݒ�

		vector<CEnemy::MoveAiInfo> VecMoveAi;//�ړ�AI��Vector
	};
	static list<PhaseSaveInfo>s_PhaseList;  //�t�F�[�Y���̃��X�g
	static int s_nNowPhase;                 //���݂̃t�F�[�Y�ԍ�
	void AdvancePhase();//���̃t�F�[�Y�Ɉڍs���鏈��
};
#endif