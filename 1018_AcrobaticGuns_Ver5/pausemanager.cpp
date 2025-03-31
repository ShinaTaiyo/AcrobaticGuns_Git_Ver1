//=============================================================================================================================
//
//２０２５年３月３１日：ポーズマネージャーを作る[pausemanager.cpp]
//Author:ShinaTaiyo
//
//=============================================================================================================================

//================================================================
//インクルード
//================================================================
#include "pausemanager.h"
#include "input.h"
#include "manager.h"
#include "debugtext.h"
#include "fade.h"
#include "game.h"
//=============================================================================================================================

//================================================================
//静的メンバ宣言
//================================================================

//=============================================================================================================================

//================================================================
//コンストラクタ
//================================================================
CPauseManager::CPauseManager(int nPriority, bool bUseintPriority, TYPE Type, OBJECTTYPE ObjType)
{

}
//=============================================================================================================================

//================================================================
//デストラクタ
//================================================================
CPauseManager::~CPauseManager()
{

}
//=============================================================================================================================

//================================================================
//初期化処理
//================================================================
HRESULT CPauseManager::Init()
{
	CObject::Init();//初期化処理

	m_pCommandUi[static_cast<int>(COMMANDTYPE::CONTINUE)] = CUi::Create(CUi::UITYPE::CONTINUETEXT_000,true,CObject2D::POLYGONTYPE::SENTERROLLING,
		200.0f, 100.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//コンティニューコマンド生成
	m_pCommandUi[static_cast<int>(COMMANDTYPE::EXIT)] = CUi::Create(CUi::UITYPE::EXITTEXT_000,true, CObject2D::POLYGONTYPE::SENTERROLLING,
		200.0f, 100.0f, 1, false, D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//EXITコマンド生成

	for (int nCnt = 0; nCnt < static_cast<int>(COMMANDTYPE::MAX); nCnt++)
	{//全コマンド共通設定
		m_pCommandUi[nCnt]->SetUseDeath(false);//全てのコマンドの死亡フラグを使用しない
		m_pCommandUi[nCnt]->SetIsStopUpdatePause(false);//ポーズ中に更新を止めない
	}

	return S_OK;
}
//=============================================================================================================================

//================================================================
//終了処理
//================================================================
void CPauseManager::Uninit()
{
	CObject::Uninit();//終了処理
}
//=============================================================================================================================

//================================================================
//更新処理
//================================================================
void CPauseManager::Update()
{
	ChooseCommand();       //コマンド選択処理
	ShowSelectedCommand(); //選択したコマンドを表示する処理
	ExecuteCommand();      //コマンド発動処理
	CObject::Update();//更新処理
}
//=============================================================================================================================

//================================================================
//描画処理
//================================================================
void CPauseManager::Draw()
{
	CObject::Draw();//描画処理
}
//=============================================================================================================================

//================================================================
//死亡フラグ設定処理
//================================================================
void CPauseManager::SetDeath()
{
	if (GetUseDeath())
	{//死亡フラグを使用するなら
		for (int nCnt = 0; nCnt < static_cast<int>(COMMANDTYPE::MAX); nCnt++)
		{//前コマンド死亡フラグ設定処理
			if (m_pCommandUi[nCnt] != nullptr)
			{//ポインタが存在していたら
				m_pCommandUi[nCnt]->SetUseDeath(true); //全てのコマンドの死亡フラグを使用する
				m_pCommandUi[nCnt]->SetDeath();        //死亡フラグを設定する
			}
		}
	}
	CObject::SetDeath();//死亡フラグ設定処理
}
//=============================================================================================================================

//================================================================
//生成処理
//================================================================
CPauseManager* CPauseManager::Create()
{
	CDebugText* pDebugText = CManager::GetDebugText();     //デバッグ表示情報を取得
	CPauseManager* pPauseManager = DBG_NEW CPauseManager();//生成
	pPauseManager->Init();                                 //初期化処理
	pDebugText->PrintDebugText("ポーズマネージャーを生成！\n");
	return pPauseManager;
}
//=============================================================================================================================

//================================================================
//コマンド選択処理
//================================================================
void CPauseManager::ChooseCommand()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //キー入力情報を取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //ジョイパッド入力情報を取得
	CDebugText* pDebugText = CManager::GetDebugText();             //デバッグ表示情報を取得
	int nNowChooseCoommand = m_nChooseCommandNum;                  //現在選んでいるコマンド番号を格納
	if (pInputKeyboard->GetTrigger(DIK_S) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::DOWN))
	{
		nNowChooseCoommand++;//次のコマンドにインクリメント
	}
	if (pInputKeyboard->GetTrigger(DIK_W) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::UP))
	{
		nNowChooseCoommand--;//前のコマンドにインクリメント
	}
	nNowChooseCoommand = (nNowChooseCoommand + static_cast<int>(COMMANDTYPE::MAX)) % static_cast<int>(COMMANDTYPE::MAX);// 範囲制限
	m_nChooseCommandNum = nNowChooseCoommand;//更新

	pDebugText->PrintDebugText("選択しているポーズコマンド：%d\n", m_nChooseCommandNum);
}
//=============================================================================================================================

//================================================================
//選択したコマンドを表示する処理
//================================================================
void CPauseManager::ShowSelectedCommand()
{
	for (int nCnt = 0; nCnt < static_cast<int>(COMMANDTYPE::MAX); nCnt++)
	{
		if (nCnt == m_nChooseCommandNum)
		{
			m_pCommandUi[nCnt]->SetColor(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),false,1.0f);//選択中の番号のコマンドの色だけ通常にする
		}
		else
		{
			m_pCommandUi[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), false, 1.0f);//選択していないコマンドの色を薄くする
		}
	}
}
//=============================================================================================================================

//================================================================
//選択したコマンドを発動する処理
//================================================================
void CPauseManager::ExecuteCommand()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard(); //キー入力情報を取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //ジョイパッド入力情報を取得
	CDebugText* pDebugText = CManager::GetDebugText();             //デバッグ表示情報を取得
	CSceneFade* pSceneFade = CManager::GetSceneFade();             //シーン遷移用フェード情報を取得
	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::A))
	{//ENTERキー、又はジョイパッドのAボタン
		switch (m_nChooseCommandNum)
		{
		case static_cast<int>(COMMANDTYPE::CONTINUE)://選択コマンドが「コンティニュー」だったら
			CGame::SetChengePause();//ポーズ状態変更フラグをONにし、ポーズを終了する			
			break;
		case static_cast<int>(COMMANDTYPE::EXIT)://選択コマンドが「EXIT(タイトルに戻る)」だったら
			pSceneFade->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_TITLE);//タイトル画面へ遷移する
			break;
		default:
			break;
		}
	}
}
//=============================================================================================================================