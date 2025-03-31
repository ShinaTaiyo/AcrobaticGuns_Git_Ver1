//=============================================================================================================================
//
//�Q�O�Q�T�N�R���R�P���F�|�[�Y�}�l�[�W���[�����[pausemanager.cpp]
//Author:ShinaTaiyo
//
//=============================================================================================================================

//================================================================
//�C���N���[�h
//================================================================
#include "pausemanager.h"
#include "input.h"
#include "manager.h"
#include "debugtext.h"
#include "fade.h"
#include "game.h"
//=============================================================================================================================

//================================================================
//�ÓI�����o�錾
//================================================================

//=============================================================================================================================

//================================================================
//�R���X�g���N�^
//================================================================
CPauseManager::CPauseManager(int nPriority, bool bUseintPriority, TYPE Type, OBJECTTYPE ObjType)
{

}
//=============================================================================================================================

//================================================================
//�f�X�g���N�^
//================================================================
CPauseManager::~CPauseManager()
{

}
//=============================================================================================================================

//================================================================
//����������
//================================================================
HRESULT CPauseManager::Init()
{
	CObject::Init();//����������

	m_pCommandUi[static_cast<int>(COMMANDTYPE::CONTINUE)] = CUi::Create(CUi::UITYPE::CONTINUETEXT_000,true,CObject2D::POLYGONTYPE::SENTERROLLING,
		200.0f, 100.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//�R���e�B�j���[�R�}���h����
	m_pCommandUi[static_cast<int>(COMMANDTYPE::EXIT)] = CUi::Create(CUi::UITYPE::EXITTEXT_000,true, CObject2D::POLYGONTYPE::SENTERROLLING,
		200.0f, 100.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//EXIT�R�}���h����

	for (int nCnt = 0; nCnt < static_cast<int>(COMMANDTYPE::MAX); nCnt++)
	{//�S�R�}���h���ʐݒ�
		m_pCommandUi[nCnt]->SetUseDeath(false);//�S�ẴR�}���h�̎��S�t���O���g�p���Ȃ�
		m_pCommandUi[nCnt]->SetIsStopUpdatePause(false);//�|�[�Y���ɍX�V���~�߂Ȃ�
	}

	return S_OK;
}
//=============================================================================================================================

//================================================================
//�I������
//================================================================
void CPauseManager::Uninit()
{
	CObject::Uninit();//�I������
}
//=============================================================================================================================

//================================================================
//�X�V����
//================================================================
void CPauseManager::Update()
{
	ChooseCommand();       //�R�}���h�I������
	ShowSelectedCommand(); //�I�������R�}���h��\�����鏈��
	ExecuteCommand();      //�R�}���h��������
	CObject::Update();//�X�V����
}
//=============================================================================================================================

//================================================================
//�`�揈��
//================================================================
void CPauseManager::Draw()
{
	CObject::Draw();//�`�揈��
}
//=============================================================================================================================

//================================================================
//���S�t���O�ݒ菈��
//================================================================
void CPauseManager::SetDeath()
{
	if (GetUseDeath())
	{//���S�t���O���g�p����Ȃ�
		for (int nCnt = 0; nCnt < static_cast<int>(COMMANDTYPE::MAX); nCnt++)
		{//�O�R�}���h���S�t���O�ݒ菈��
			if (m_pCommandUi[nCnt] != nullptr)
			{//�|�C���^�����݂��Ă�����
				m_pCommandUi[nCnt]->SetUseDeath(true); //�S�ẴR�}���h�̎��S�t���O���g�p����
				m_pCommandUi[nCnt]->SetDeath();        //���S�t���O��ݒ肷��
			}
		}
	}
	CObject::SetDeath();//���S�t���O�ݒ菈��
}
//=============================================================================================================================

//================================================================
//��������
//================================================================
CPauseManager* CPauseManager::Create()
{
	CDebugText* pDebugText = CManager::GetDebugText();     //�f�o�b�O�\�������擾
	CPauseManager* pPauseManager = DBG_NEW CPauseManager();//����
	pPauseManager->Init();                                 //����������
	pDebugText->PrintDebugText("�|�[�Y�}�l�[�W���[�𐶐��I\n");
	return pPauseManager;
}
//=============================================================================================================================

//================================================================
//�R�}���h�I������
//================================================================
void CPauseManager::ChooseCommand()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //�L�[���͏����擾
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //�W���C�p�b�h���͏����擾
	CDebugText* pDebugText = CManager::GetDebugText();             //�f�o�b�O�\�������擾
	int nNowChooseCoommand = m_nChooseCommandNum;                  //���ݑI��ł���R�}���h�ԍ����i�[
	if (pInputKeyboard->GetTrigger(DIK_S) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::DOWN))
	{
		nNowChooseCoommand++;//���̃R�}���h�ɃC���N�������g
	}
	if (pInputKeyboard->GetTrigger(DIK_W) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::UP))
	{
		nNowChooseCoommand--;//�O�̃R�}���h�ɃC���N�������g
	}
	nNowChooseCoommand = (nNowChooseCoommand + static_cast<int>(COMMANDTYPE::MAX)) % static_cast<int>(COMMANDTYPE::MAX);// �͈͐���
	m_nChooseCommandNum = nNowChooseCoommand;//�X�V

	pDebugText->PrintDebugText("�I�����Ă���|�[�Y�R�}���h�F%d\n", m_nChooseCommandNum);
}
//=============================================================================================================================

//================================================================
//�I�������R�}���h��\�����鏈��
//================================================================
void CPauseManager::ShowSelectedCommand()
{
	for (int nCnt = 0; nCnt < static_cast<int>(COMMANDTYPE::MAX); nCnt++)
	{
		if (nCnt == m_nChooseCommandNum)
		{
			m_pCommandUi[nCnt]->SetColor(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),false,1.0f);//�I�𒆂̔ԍ��̃R�}���h�̐F�����ʏ�ɂ���
		}
		else
		{
			m_pCommandUi[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), false, 1.0f);//�I�����Ă��Ȃ��R�}���h�̐F�𔖂�����
		}
	}
}
//=============================================================================================================================

//================================================================
//�I�������R�}���h�𔭓����鏈��
//================================================================
void CPauseManager::ExecuteCommand()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //�L�[���͏����擾
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //�W���C�p�b�h���͏����擾
	CDebugText* pDebugText = CManager::GetDebugText();             //�f�o�b�O�\�������擾
	CSceneFade* pSceneFade = CManager::GetSceneFade();             //�V�[���J�ڗp�t�F�[�h�����擾
	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::A))
	{//ENTER�L�[�A���̓W���C�p�b�h��A�{�^��
		switch (m_nChooseCommandNum)
		{
		case static_cast<int>(COMMANDTYPE::CONTINUE)://�I���R�}���h���u�R���e�B�j���[�v��������
			CGame::SetChengePause();//�|�[�Y��ԕύX�t���O��ON�ɂ��A�|�[�Y���I������			
			break;
		case static_cast<int>(COMMANDTYPE::EXIT)://�I���R�}���h���uEXIT(�^�C�g���ɖ߂�)�v��������
			pSceneFade->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_TITLE);//�^�C�g����ʂ֑J�ڂ���
			break;
		default:
			break;
		}
	}
}
//=============================================================================================================================