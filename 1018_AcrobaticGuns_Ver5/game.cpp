//=========================================================================================================================
//
//７月１１日：オブジェクトのRelease方法の変更[game.cpp]
//Author:ShinaTaiyo
//
//=========================================================================================================================

//=============================================================
//インクルード
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
#include "attack.h"
#include "debugproc.h"
#include "calculation.h"
#include "particle.h"
#include "wire.h"
//=========================================================================================================================

//=============================================================
//静的メンバ宣言
//=============================================================
CPlayer* CGame::m_pPlayer = nullptr;
CStageManager* CGame::m_pStageManager = nullptr;
CPhaseManager* CGame::m_pPhaseManager = nullptr;
int CGame::s_nPhaseNum = 0;
//=========================================================================================================================

//=============================================================
//コンストラクタ
//=============================================================
CGame::CGame() : m_pBgModel(nullptr)
{
	m_pPlayer = nullptr;
	m_pStageManager = nullptr;
	m_pPhaseManager = nullptr;
	s_nPhaseNum = 0;
	bStartFade = false;
}
//=========================================================================================================================

//=============================================================
//デストラクタ
//=============================================================
CGame::~CGame()
{

}
//=========================================================================================================================

//=============================================================
//初期化処理
//=============================================================
HRESULT CGame::Init()
{
	CScene::Init();//シーン初期化処理

	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f,0.0f,-300.0f),D3DXVECTOR3(0.0f,D3DX_PI,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(1.0f,1.0f,1.0f));
	m_pPlayer->SetUseDeath(false);
	m_pPlayer->SetVecAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//CEnemy::Create(CEnemy::ENEMYTYPE::NORMAL, 100, D3DXVECTOR3(-200.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(1.0f,1.0f,1.0f) * 2);

	CBg3D * pBg3D = CBg3D::Create(CBg3D::BG3DTYPE::GLASS, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 1200.0f, 1200.0f));

	m_pPhaseManager = CPhaseManager::Create();//フェーズマネージャーを生成

	m_pStageManager = CStageManager::Create();//ステージマネージャーを生成

	m_pBgModel = CBgModel::Create(CBgModel::BGMODELTYPE::TREE_00, D3DXVECTOR3(200.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 2.0f, 2.0f));

	CField::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), 2000.0f, 2000.0f,CField::FIELDTYPE00_NORMAL);
	return S_OK;
}
//=========================================================================================================================

//=============================================================
//終了処理
//=============================================================
void CGame::Uninit()
{
	//============================================
	//プレイヤーの破棄
	//============================================
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->SetUseDeath(true);
		m_pPlayer->SetDeath();
		m_pPlayer = nullptr;
	}
	//=====================================================================

	//============================================
	//ステージマネージャーの破棄
	//============================================
	if (m_pStageManager != nullptr)
	{
		m_pStageManager->SetUseDeath(true);
		m_pStageManager->SetDeath();
		m_pStageManager = nullptr;
	}
	//=====================================================================

	//============================================
	//フェーズマネージャーの破棄
	//============================================
	if (m_pPhaseManager != nullptr)
	{
		m_pPhaseManager->SetUseDeath(true);
		m_pPhaseManager->SetDeath();
		m_pPhaseManager = nullptr;
	}
	//=====================================================================

	if (m_pBgModel != nullptr)
	{
		m_pBgModel->SetUseDeath(true);
		m_pBgModel->SetDeath();
		m_pBgModel = nullptr;
	}

	CManager::GetSound()->StopSound();

	CScene::Uninit();//シーン終了処理
}
//=========================================================================================================================

//=============================================================
//更新処理
//=============================================================
void CGame::Update()
{
	//D3DXVECTOR3 Vec1 = D3DXVECTOR3(sinf(m_pPlayer->GetRot().y + D3DX_PI * 0.7f) * 10.0f, 0.0f, cosf(m_pPlayer->GetRot().y + D3DX_PI * 0.7f) * 10.0f);
	//D3DXVECTOR3 Vec2 = D3DXVECTOR3(sinf(m_pPlayer->GetRot().y - D3DX_PI * 0.7f) * 10.0f, 0.0f, cosf(m_pPlayer->GetRot().y - D3DX_PI * 0.7f) * 10.0f);
	////D3DXVECTOR3 Vec1Move = CCalculation::Calculation3DVec(m_pPlayer->GetPos(), Vec1, 10.0f);
	////CAttackPlayer::Create(CAttack::ATTACKTYPE::BULLET, 10, 1, 100, m_pPlayer->GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	////	Vec1 * 10.0f, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	////CAttackPlayer::Create(CAttack::ATTACKTYPE::BULLET, 10, 1, 100, m_pPlayer->GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	////	Vec2 * 10.0f, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	//CManager::GetDebugProc()->PrintDebugProc("プレイヤーの向き：%f %f %f\n", m_pPlayer->GetRot().x, m_pPlayer->GetRot().y, m_pPlayer->GetRot().z);

	//float fRot = 0.0f;
	//fRot = CCalculation::GetAngleBetweenVectors(Vec1, Vec2);
	//CManager::GetDebugProc()->PrintDebugProc("二つのベクトルがなす角：%f\n", fRot);
#ifdef _DEBUG
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	}

	CManager::GetDebugProc()->PrintDebugProc("現在の敵の数；%d\n", CEnemy::GetNumEnemy());
#else
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
	}
#endif // !_DEBUG
}
//=========================================================================================================================