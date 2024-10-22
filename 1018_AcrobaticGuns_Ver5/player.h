//====================================================
//
//７月４日：チャージ攻撃を実装する[player.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _PLAYER_H_  
#define _PLAYER_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
#include "objectXAlive.h"
//==========================================

//======================
//前方宣言
//======================
class CInputKeyboard;
class CInputJoypad;
class CGauge;
class CNumber;
class CScore;
//==========================================

//===========================================
//プレイヤー3Dクラス
//===========================================
class CPlayer : public CObjectXAlive
{
public:
	CPlayer();                  //コンストラクタ
	~CPlayer();                 //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void ExtraUninit() override;//別枠の終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static void Unload();       //モデルの情報の破棄
	static CPlayer * Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 move,D3DXVECTOR3 Scale);
private:
	void MoveProcess();//プレイヤーの移動処理
	float m_fRotAim;//目的の向き	
};

#endif
