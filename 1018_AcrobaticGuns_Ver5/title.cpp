//=========================================================================================================================
//
//６月１７日：画面遷移を実装する[title.cpp]
//Author:ShinaTaiyo
//
//=========================================================================================================================

//=============================================================
//インクルード
//=============================================================
#include "title.h"
#include "bg3d.h"
#include "bgModel.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "fade.h"
#include "attack.h"
#include "camera.h"
#include "input.h"
#include "debugproc.h"
//=========================================================================================================================

//=============================================================
//静的メンバ宣言
//=============================================================
CPlayer* CTitle::m_pPlayer = nullptr;
CBg3D* CTitle::m_pBg3D = nullptr;

//=============================================================
//コンストラクタ
//=============================================================
CTitle::CTitle() : m_bMoveSwitch(true)
{
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI , 0.0f), D3DXVECTOR3(0.0f, -5.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pPlayer->SetUseGravity(false, CObjectXMove::GetNormalGravity());
	m_pPlayer->SetUseInteria(false, CObjectXMove::GetNormalInertia());
	m_pPlayer->SetUseUpdatePos(true);

	m_pBg3D = CBg3D::Create(CBg3D::BG3DTYPE::SKY, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f));
	m_pBg3D->SetUseDeath(false);

	CManager::GetCamera()->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f));

	m_pTitleLogo = CUi::Create(CUi::UITYPE::TITLELOGO_000, CObject2D::POLYGONTYPE::SENTERROLLING, 1000.0f, 500.0f, 10, false, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 200.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pTitleLogo->SetUseDeath(false);
}
//=========================================================================================================================

//=============================================================
//デストラクタ
//=============================================================
CTitle::~CTitle()
{

}
//=========================================================================================================================

//=============================================================
//初期化処理
//=============================================================
HRESULT CTitle::Init()
{
	CScene::Init();//シーン初期化処理

	//タイトル背景を生成

	CObjectX::SetCommonDraw(false);
	return S_OK;
}
//=========================================================================================================================

//=============================================================
//終了処理
//=============================================================
void CTitle::Uninit()
{
	CScene::Uninit();//シーン終了処理

	if (m_pPlayer != nullptr)
	{
		m_pPlayer->SetUseDeath(true);
		m_pPlayer->SetDeath();
		m_pPlayer = nullptr;
	}

	if (m_pBg3D != nullptr)
	{
		m_pBg3D->SetUseDeath(true);
		m_pBg3D->SetDeath();
		m_pBg3D = nullptr;
	}

	if (m_pTitleLogo != nullptr)
	{
		m_pTitleLogo->SetUseDeath(true);
		m_pTitleLogo->SetDeath();
		m_pTitleLogo = nullptr;
	}

	CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_NOESIS);//タイトルbgmを止める
}
//=========================================================================================================================

//=============================================================
//更新処理
//=============================================================
void CTitle::Update()
{
	CScene::Update();//シーン更新処理

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_GAME);
	}

	if (m_pPlayer != nullptr && m_pBg3D != nullptr)
	{
		m_pBg3D->GetPosInfo().SetPos(m_pPlayer->GetPosInfo().GetPos());

		PlayerProcess();
	}
}
//=========================================================================================================================

//=============================================================
//描画処理
//=============================================================
void CTitle::Draw()
{
	CScene::Draw();//シーン描画処理
}
//=========================================================================================================================

//=============================================================
//プレイヤーの処理
//=============================================================
void CTitle::PlayerProcess()
{
	if (m_bMoveSwitch == false)
	{
		m_pPlayer->SetMove(D3DXVECTOR3(m_pPlayer->GetMove().x,m_pPlayer->GetMove().y + 0.1f,m_pPlayer->GetMove().z));

		if (m_pPlayer->GetMove().y >= 5.0f)
		{
			m_bMoveSwitch = m_bMoveSwitch ? false : true;
		}

	}
	else
	{
		m_pPlayer->SetMove(D3DXVECTOR3(m_pPlayer->GetMove().x, m_pPlayer->GetMove().y - 0.1f, m_pPlayer->GetMove().z));

		if (m_pPlayer->GetMove().y <= -5.0f)
		{
			m_bMoveSwitch = m_bMoveSwitch ? false : true;
		}
	}

	float fPosX = static_cast<float>(rand() % 1500 - 750);
	float fPosY = static_cast<float>(rand() % 1500 - 750);

	CAttackPlayer::Create(CAttack::ATTACKTYPE::BULLET, CAttack::TARGETTYPE::ENEMY, CAttack::COLLISIONTYPE::SQUARE, false, true, 3, 0,300,D3DXVECTOR3(fPosX,fPosY,3000.0f),
		D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,-20.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}
//=========================================================================================================================