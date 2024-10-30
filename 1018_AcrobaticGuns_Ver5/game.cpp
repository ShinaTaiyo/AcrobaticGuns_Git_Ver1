//=========================================================================================================================
//
//�V���P�P���F�I�u�W�F�N�g��Release���@�̕ύX[game.cpp]
//Author:ShinaTaiyo
//
//=========================================================================================================================

//=============================================================
//�C���N���[�h
//=============================================================
#include "game.h"
#include "bg.h"
#include "manager.h"
#include "sound.h"
#include "fade.h"
#include "camera.h"
#include "field.h"
#include "enemy.h"
#include "input.h"
//=========================================================================================================================

//=============================================================
//�ÓI�����o�錾
//=============================================================
CPlayer* CGame::m_pPlayer = nullptr;
CStageManager* CGame::m_pStageManager = nullptr;
//=========================================================================================================================

//=============================================================
//�R���X�g���N�^
//=============================================================
CGame::CGame()
{
	m_pPlayer = nullptr;
	m_pStageManager = nullptr;
}
//=========================================================================================================================

//=============================================================
//�f�X�g���N�^
//=============================================================
CGame::~CGame()
{

}
//=========================================================================================================================

//=============================================================
//����������
//=============================================================
HRESULT CGame::Init()
{
	CScene::Init();//�V�[������������

	m_pPlayer = CPlayer::Create(NULL_VECTOR3, NULL_VECTOR3, NULL_VECTOR3, ONE_VECTOR3);
	m_pPlayer->SetUseDeath(false);

	CEnemy::Create(CEnemy::ENEMYTYPE::NORMAL, 100, D3DXVECTOR3(0.0f, 200.0f, 200.0f), NULL_VECTOR3, ONE_VECTOR3 * 4);
	CEnemy::Create(CEnemy::ENEMYTYPE::NORMAL, 100, D3DXVECTOR3(200.0f, 0.0f, 200.0f), NULL_VECTOR3, ONE_VECTOR3 * 2);
	CEnemy::Create(CEnemy::ENEMYTYPE::NORMAL, 100, D3DXVECTOR3(-200.0f, 0.0f, 200.0f), NULL_VECTOR3, ONE_VECTOR3 * 2);

	m_pStageManager = CStageManager::Create();

	CField::Create(NULL_VECTOR3, NULL_VECTOR3, 2000.0f, 2000.0f,CField::FIELDTYPE00_NORMAL);
	return S_OK;
}
//=========================================================================================================================

//=============================================================
//�I������
//=============================================================
void CGame::Uninit()
{
	//============================================
	//�v���C���[�̔j��
	//============================================
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->SetUseDeath(true);
		m_pPlayer->SetDeath();
		m_pPlayer = nullptr;
	}
	//=====================================================================

	//============================================
	//�X�e�[�W�}�l�[�W���[�̔j��
	//============================================
	if (m_pStageManager != nullptr)
	{
		m_pStageManager->SetUseDeath(true);
		m_pStageManager->SetDeath();
		m_pStageManager = nullptr;
	}
	//=====================================================================


	CManager::GetSound()->StopSound();

	CScene::Uninit();//�V�[���I������
}
//=========================================================================================================================

//=============================================================
//�X�V����
//=============================================================
void CGame::Update()
{
#ifdef _DEBUG
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	}
#else
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	}
#endif // !_DEBUG
}
//=========================================================================================================================