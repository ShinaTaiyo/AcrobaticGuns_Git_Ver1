//===============================================================================
//
//６月１７日：画面遷移を実装する[game.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _GAME_H_  
#define _GAME_H_


//========================================
//インクルード
//========================================
#include "scene.h"
#include "stagemanager.h"
#include "player.h"
#include "main.h"
#include "phasemanager.h"
//===============================================================================

//========================================
//ゲームクラス
//========================================
class CGame : public CScene
{
public:
	CGame();                                                               //コンストラクタ
	~CGame() override;                                                     //デストラクタ
	HRESULT Init() override;                                               //初期化処理
	void Uninit() override;                                                //終了処理
	void Update() override;                                                //更新処理
	static CPlayer* GetPlayer() { return m_pPlayer; }                      //プレイヤーを取得する
	static CPhaseManager* GetPhaseManager() { return m_pPhaseManager; }    //フェーズマネージャーを取得する
private:
	static CPlayer* m_pPlayer;                                             //プレイヤー
	static CStageManager* m_pStageManager;                                 //ステージマネージャー
	static CPhaseManager* m_pPhaseManager;                                 //フェーズマネージャー
	static int s_nPhaseNum;                                                //フェーズ番号
	bool bStartFade;
};
#endif