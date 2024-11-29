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
#include "debugproc.h"
#include "wire.h"
//=========================================================================================================================

//=============================================================
//�ÓI�����o�錾
//=============================================================
CPlayer* CGame::m_pPlayer = nullptr;
CStageManager* CGame::m_pStageManager = nullptr;
CPhaseManager* CGame::m_pPhaseManager = nullptr;
int CGame::s_nPhaseNum = 0;
//=========================================================================================================================

//=============================================================
//�R���X�g���N�^
//=============================================================
CGame::CGame()
{
	m_pPlayer = nullptr;
	m_pStageManager = nullptr;
	m_pPhaseManager = nullptr;
	s_nPhaseNum = 0;
	bStartFade = false;
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
	//CEnemy::Create(CEnemy::ENEMYTYPE::NORMAL, 100, D3DXVECTOR3(-200.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(1.0f,1.0f,1.0f) * 2);

	CBg3D * pBg3D = CBg3D::Create(CBg3D::BG3DTYPE::GLASS, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 1200.0f, 1200.0f));

	//m_pPhaseManager = CPhaseManager::Create();//�t�F�[�Y�}�l�[�W���[�𐶐�

	m_pStageManager = CStageManager::Create();//�X�e�[�W�}�l�[�W���[�𐶐�

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

	//============================================
	//�t�F�[�Y�}�l�[�W���[�̔j��
	//============================================
	if (m_pPhaseManager != nullptr)
	{
		m_pPhaseManager->SetUseDeath(true);
		m_pPhaseManager->SetDeath();
		m_pPhaseManager = nullptr;
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
	if (CEnemy::GetNumEnemy() <= 0)
	{
		s_nPhaseNum++;

		if (s_nPhaseNum == 1)
		{
			CShotWeakEnemy::Create(CShotWeakEnemy::SHOTWEAKENEMYTYPE::NORMAL, 100, 1, D3DXVECTOR3(0.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(12.0f, 12.0f, 12.0f));
			CDiveWeakEnemy::Create(CDiveWeakEnemy::DIVEWEAKENEMYTYPE::NORMAL, 100, 1, D3DXVECTOR3(350.0f, 0.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(12.0f, 12.0f, 12.0f));
		}
		else if (s_nPhaseNum == 2)
		{
			CShotWeakEnemy::Create(CShotWeakEnemy::SHOTWEAKENEMYTYPE::NORMAL, 100, 2, D3DXVECTOR3(500.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(12.0f, 17.0f, 12.0f));
			CShotWeakEnemy::Create(CShotWeakEnemy::SHOTWEAKENEMYTYPE::NORMAL, 100, 2, D3DXVECTOR3(0.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(12.0f, 12.0f, 12.0f));
			CDiveWeakEnemy::Create(CDiveWeakEnemy::DIVEWEAKENEMYTYPE::NORMAL, 100, 2, D3DXVECTOR3(-250.0f, 0.0f,450.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(12.0f, 17.0f, 12.0f));
		}
		else
		{
			if (bStartFade == false)
			{
				bStartFade = true;
				CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
			}
		}
	}


#ifdef _DEBUG
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	}

	CManager::GetDebugProc()->PrintDebugProc("���݂̓G�̐��G%d\n", CEnemy::GetNumEnemy());
#else
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	}
#endif // !_DEBUG
}
//=========================================================================================================================