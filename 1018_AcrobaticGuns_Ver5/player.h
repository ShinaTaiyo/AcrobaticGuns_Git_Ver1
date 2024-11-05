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
#include "objectXAlive.h"
#include "lockon.h"
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
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CPlayer * Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 move,D3DXVECTOR3 Scale);
private:
	//================================================
    //静的メンバ
    //================================================
	static constexpr float m_fNORMALATTACK_SPEED = 10.0f;//通常攻撃の移動量を設定
	//===============================================================================================

	//================================================
	//変数宣言
	//================================================
	float m_fRotAim;   //目的の向き
	CLockon* m_pLockOn;//ロックオンカーソルへのポインタ
	//===============================================================================================


	//================================================
	//プロトタイプ宣言
	//================================================

	//=============================
	//基本形
	//=============================
	void MoveProcess();//プレイヤーの移動処理
	void AttackProcress();//攻撃処理
	void NormalAttackProcess();//通常攻撃処理
	//================================================

	//=============================
	//通常攻撃処理
	//=============================
	D3DXVECTOR3 CalcRay(D3DXVECTOR3 & NearPos);//レイを測る
	D3DXVECTOR3 & CalcAttackMove(const D3DXVECTOR3& ShotPos,const D3DXVECTOR3 & NearPos,const D3DXVECTOR3& Ray,D3DXVECTOR3 & Move);
	void AttackStart(const D3DXVECTOR3& ShotPos, const D3DXVECTOR3& Move, const D3DXVECTOR3& Rot);

	//判定
	void CollisionBlock();//ブロックとの当たり判定処理

	//ロックオンコントローラー
	void LockOnProcess();//ロックオンの処理
	void LockOnMove();   //ロックオンを動かす


	//===============================================================================================
};

#endif
