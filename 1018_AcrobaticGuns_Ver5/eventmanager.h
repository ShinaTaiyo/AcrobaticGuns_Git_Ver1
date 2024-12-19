//====================================================
//
//９月１５日：ボス出現イベントを作る[eventmanager.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _EVENTMANAGER_H_  
#define _EVENTMANAGER_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object.h"
#include "ui.h"
//====================================================

//======================
//前方宣言
//======================
class CEventManager;
//==========================================

//=====================================================
//イベントステート
//=====================================================

//スーパークラス
class CNowEvent
{
public:
	CNowEvent() {};//コンストラクタ
	virtual ~CNowEvent() {};//デストラクタ
	virtual void Process(CEventManager* pEventManager) {};//処理
};

//フェーズ移行クラス
class CNowEvent_NextPhase : public CNowEvent
{
public:
	CNowEvent_NextPhase();//コンストラクタ
	~CNowEvent_NextPhase() override;//デストラクタ
	void Process(CEventManager* pEventManager) override;//処理
private:
	CUi* m_PhaseText;//フェーズの文字
};

//===================================================================================================

//==========================================
//イベントマネージャークラス
//==========================================
class CEventManager : public CObject
{
public:
	//===================================================
    //イベント列挙型
    //===================================================
	enum class EVENTTYPE
	{
		NONE = 0, //なし
		NEXTPHASE,//フェーズを次へ
		MAX
	};
	//=========================================================================================

	//====================================================
    //イベント進行情報構造体
    //====================================================
	struct EventProgressInfo
	{
		int nEventPattern;     //イベントのパターン
		int nCntEventTime;     //イベントの時間を測る
		bool bEventProgress;   //イベントが進行中かどうか
		EVENTTYPE EventType;//現在のイベント

		void NextPattern();      //パターンを次に進める
		void ResetPattern();     //パターンをリセットする
	};
	//=========================================================================================
	CEventManager(CNowEvent * pNowEvent);
	~CEventManager();        //デストラクタ
	HRESULT Init() override;          //初期化処理
	void Uninit() override;           //終了処理
	void Update() override;           //更新処理
	void Draw() override;             //描画処理
	void SetDeath() override;         //死亡フラグ設定処理

	void ChengeEvent(CNowEvent* pNowEvent);//イベントを変える

	static CEventManager* Create(CNowEvent * pNowEvent);//生成処理
	EventProgressInfo & GetEventProgressInfo(){ return m_EventProgressInfo; }
private:
	EventProgressInfo m_EventProgressInfo;//イベント進行を管理する変数
	CNowEvent* m_pNowEvent;               //現在のイベントの状態
};

#endif
