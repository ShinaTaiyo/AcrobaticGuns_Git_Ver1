//====================================================
//
//�X���P�T���F�{�X�o���C�x���g�����[eventmanager.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _EVENTMANAGER_H_  
#define _EVENTMANAGER_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object.h"
#include "ui.h"
//====================================================

//======================
//�O���錾
//======================
class CEventManager;
//==========================================

//=====================================================
//�C�x���g�X�e�[�g
//=====================================================

//�X�[�p�[�N���X
class CNowEvent
{
public:
	CNowEvent() {};//�R���X�g���N�^
	virtual ~CNowEvent() {};//�f�X�g���N�^
	virtual void Process(CEventManager* pEventManager) {};//����
};

//�t�F�[�Y�ڍs�N���X
class CNowEvent_NextPhase : public CNowEvent
{
public:
	CNowEvent_NextPhase(int nPhaseNum,float fValueWidth,float fValueHeight);//�R���X�g���N�^
	~CNowEvent_NextPhase() override;//�f�X�g���N�^
	void Process(CEventManager* pEventManager) override;//����
private:
	CUi* m_PhaseText;//�t�F�[�Y�̕���
};

//===================================================================================================

//==========================================
//�C�x���g�}�l�[�W���[�N���X
//==========================================
class CEventManager : public CObject
{
public:
	//===================================================
    //�C�x���g�񋓌^
    //===================================================
	enum class EVENTTYPE
	{
		NONE = 0, //�Ȃ�
		NEXTPHASE,//�t�F�[�Y������
		MAX
	};
	//=========================================================================================

	//====================================================
    //�C�x���g�i�s���\����
    //====================================================
	struct EventProgressInfo
	{
		int nEventPattern;     //�C�x���g�̃p�^�[��
		int nCntEventTime;     //�C�x���g�̎��Ԃ𑪂�
		bool bEventProgress;   //�C�x���g���i�s�����ǂ���
		EVENTTYPE EventType;//���݂̃C�x���g

		void NextPattern();      //�p�^�[�������ɐi�߂�
		void ResetPattern();     //�p�^�[�������Z�b�g����
	};
	//=========================================================================================
	CEventManager(CNowEvent * pNowEvent, int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::EVENTMANAGER, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);
	~CEventManager();        //�f�X�g���N�^
	HRESULT Init() override;          //����������
	void Uninit() override;           //�I������
	void Update() override;           //�X�V����
	void Draw() override;             //�`�揈��
	void SetDeath() override;         //���S�t���O�ݒ菈��

	void ChengeEvent(CNowEvent* pNowEvent);//�C�x���g��ς���
	void SetNextPhaseEvent(int nNowPhase,float fValueWidth,float fValueHeight);

	static CEventManager* Create(CNowEvent * pNowEvent);//��������
	EventProgressInfo & GetEventProgressInfo(){ return m_EventProgressInfo; }
private:
	EventProgressInfo m_EventProgressInfo;//�C�x���g�i�s���Ǘ�����ϐ�
	CNowEvent* m_pNowEvent;               //���݂̃C�x���g�̏��
};

#endif