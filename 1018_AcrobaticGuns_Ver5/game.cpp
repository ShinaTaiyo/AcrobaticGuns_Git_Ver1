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
#include "bg3d.h"
#include "enemy.h"
#include "input.h"
#include "wire.h"
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

	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f,0.0f,-300.0f),D3DXVECTOR3(0.0f,D3DX_PI,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(1.0f,1.0f,1.0f));
	m_pPlayer->SetUseDeath(false);
	m_pPlayer->SetVecAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	CShotWeakEnemy::Create(CShotWeakEnemy::SHOTWEAKENEMYTYPE::NORMAL, 100, D3DXVECTOR3(0.0f, 200.0f, 200.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(1.0f,1.0f,1.0f) * 4);
	CDiveWeakEnemy::Create(CDiveWeakEnemy::DIVEWEAKENEMYTYPE::NORMAL, 100, D3DXVECTOR3(200.0f,0.0f, 200.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(1.0f,1.0f,1.0f) * 4);
	//CEnemy::Create(CEnemy::ENEMYTYPE::NORMAL, 100, D3DXVECTOR3(-200.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(1.0f,1.0f,1.0f) * 2);

	CBg3D * pBg3D = CBg3D::Create(CBg3D::BG3DTYPE::GLASS, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 1200.0f, 1200.0f));

	m_pStageManager = CStageManager::Create();

	CField::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 2000.0f, 2000.0f,CField::FIELDTYPE00_NORMAL);
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
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	}
#else
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	}
#endif // !_DEBUG
}
//=========================================================================================================================