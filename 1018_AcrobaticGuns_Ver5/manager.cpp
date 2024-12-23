//===================================================
//
//クラスで基盤づくり[Manager.cpp]
//Author;ShinaTaiyo
//
//===================================================

//=======================
//インクルード
//=======================
#include "object.h"
#include "bg.h"
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "object3d.h"
#include "texture.h"
#include "eventmanager.h"
#include "objectXInfo.h"
#include "main.h"
#include "fade.h"
#include "text.h"
//===================================================

//=======================
//静的メンバ
//=======================
CRenderer* CManager::m_pRenderer = nullptr;
CInputKeyboard* CManager::m_pInputKeyboard = nullptr;
CInputJoypad* CManager::m_pInputJoypad = nullptr;
CSound* CManager::m_pSound = nullptr;
CCamera* CManager::m_pCamera = nullptr;
CLight* CManager::m_pLight = nullptr;
CTexture* CManager::m_pTexture = nullptr;
CObjectXInfo* CManager::m_pObjectXInfo = nullptr;
CScene* CManager::m_pScene = nullptr;
CSceneFade* CManager::m_pSceneFade = nullptr;
CText* CManager::m_pText = nullptr;
CDebugProc* CManager::m_pDebugProc = nullptr;
CInputMouse* CManager::m_pInputMouse = nullptr;
//===================================================

//=======================
//コンストラクタ
//=======================
CManager::CManager()
{

}
//===================================================

//=======================
//デストラクタ
//=======================
CManager::~CManager()
{

}

//================================
//初期化処理
//================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//============================
	//レンダラーの生成
	//============================
	m_pRenderer = DBG_NEW CRenderer;
	m_pRenderer->Init(hWnd, bWindow);

	//======================================================

	//============================
	//キー入力情報の生成
	//============================
	m_pInputKeyboard = DBG_NEW CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	//======================================================

	//============================
	//ジョイパッド入力情報の生成
	//============================
	m_pInputJoypad = DBG_NEW CInputJoypad;
	m_pInputJoypad->Init(hInstance, hWnd);
	//======================================================

	//===================================
	//マウス入力情報の設定
	//===================================
	m_pInputMouse = DBG_NEW CInputMouse;
	m_pInputMouse->Init(hInstance, hWnd);
	//======================================================

	//============================
	//サウンドの生成
	//============================
	m_pSound = DBG_NEW CSound;
	m_pSound->InitSound(hWnd);
	//======================================================

	//============================
	//カメラの生成
	//============================
	m_pCamera = DBG_NEW CCamera;
	m_pCamera->Init();
	//======================================================

	//============================
	//ライトの生成
	//============================
	m_pLight = DBG_NEW CLight;
	m_pLight->Init();
	//======================================================

	//============================
	//テクスチャクラスの生成
	//============================
	m_pTexture = DBG_NEW CTexture;
	//======================================================


	//===============================
	//オブジェクトX情報クラスを生成
    //===============================
	m_pObjectXInfo = DBG_NEW CObjectXInfo;
	//======================================================

	//===============================
	//テキストクラスを生成
	//===============================
	m_pText = DBG_NEW CText;
	m_pText->Init();
	//======================================================

	//===================================
	//デバッグプロシージャクラスを生成
	//===================================
	m_pDebugProc = DBG_NEW CDebugProc;
	m_pDebugProc->Init();
	//======================================================

	//===================================
	//イベントマネージャークラスを生成
	//===================================
	//=====================================================

	//===================================
	//モードの設定
	//===================================
	m_pSceneFade = CSceneFade::Create();
	SetMode(CScene::MODE_TITLE);
	m_pSceneFade->SetSceneFade(CFade::FADEMODE_OUT, CScene::MODE_TITLE);
	//=====================================================

	//============================
	//マップ情報を設定
	//============================

	return S_OK;
}
//================================================================================

//=======================
//終了処理
//=======================
void CManager::Uninit()
{
	//m_pStageManager->SaveMapTxt();//マップ情報をセーブする（それをしてから破棄する処理を開始）
	//CObject::ReleaseAll();    //オブジェクトの開放
	//================================
	//シーン
	//================================
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	//================================
	//レンダラーの開放
	//================================
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
	//===============================================

	//================================
	//キー入力情報の開放
	//================================
	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}
	//===============================================

	//================================
	//ジョイパッド入力情報の開放
	//================================
	if (m_pInputJoypad != nullptr)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = nullptr;
	}
	//===============================================
	
	//================================
	//マウス入力情報の開放
	//================================
	if (m_pInputMouse != nullptr)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}
	//===============================================

	//================================
	//サウンドの開放
	//================================
	if (m_pSound != nullptr)
	{
		m_pSound->UninitSound();
		delete m_pSound;
		m_pSound = nullptr;
	}
	//===============================================

	//================================
	//カメラ
	//================================
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}
	//===============================================

	//================================
	//ライト
	//================================
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}
	//===============================================
	
	//================================
	//テクスチャ
	//================================
	if (m_pTexture != nullptr)
	{
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = nullptr;
	}
	//===============================================

	//================================
	//オブジェクトX情報
	//================================
	if (m_pObjectXInfo != nullptr)
	{
		m_pObjectXInfo->Unload();
		delete m_pObjectXInfo;
		m_pObjectXInfo = nullptr;
	}
	//===============================================

	//================================
	//フェード情報
	//================================
	if (m_pSceneFade != nullptr)
	{
		m_pSceneFade->SetUseDeath(true);
		m_pSceneFade->SetDeath();
		m_pSceneFade = nullptr;
	}
	//===============================================

	//================================
	//テキスト情報
	//================================
	if (m_pText != nullptr)
	{
		m_pText->Uninit();
		delete m_pText;
		m_pText = nullptr;
	}
	//===============================================

	//================================
	//デバッグプロシージャ情報
	//================================
	if (m_pDebugProc != nullptr)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}
	//===============================================

	//最後に残ったオブジェクトをここで全部破棄
	CObject::ReleaseProcess();//リストの破棄をする処理
}
//===================================================

//=======================
//更新処理
//=======================
void CManager::Update()
{
	m_pRenderer->Update();     //レンダラー
	m_pInputKeyboard->Update();//キー入力
	m_pInputJoypad->Update();  //ジョイパッド入力
	m_pInputMouse->Update();   //マウス入力
	m_pCamera->Update();       //カメラ
	m_pLight->Update();        //ライト
#ifdef _DEBUG
	m_pDebugProc->Update();    //デバッグプロシージャ
#endif // _DEBUG
	if (m_pScene != nullptr)
	{
		m_pScene->Update();        //シーンの更新処理
	}
}
//===================================================

//=======================
//描画処理
//=======================
void CManager::Draw()
{
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();        //シーンの更新処理
	}

	m_pRenderer->Draw();         //レンダラー
}
//===================================================

//=================================
//レンダラークラスを取得する
//=================================
CRenderer* CManager::GetRenderer()
{
	return m_pRenderer;
}
//===================================================

//=================================
//キーボードクラスを取得する
//=================================
CInputKeyboard* CManager::GetInputKeyboard()
{
	return m_pInputKeyboard;
}
//===================================================

//=================================
//ジョイパッドクラスを取得する
//=================================
CInputJoypad* CManager::GetInputJoypad()
{
	return m_pInputJoypad;
}
//====================================================

//===========================================
//モード
//===========================================
void CManager::SetMode(CScene::MODE mode)
{

	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}
	
	//生成
	m_pScene = CScene::Create(mode);
}
