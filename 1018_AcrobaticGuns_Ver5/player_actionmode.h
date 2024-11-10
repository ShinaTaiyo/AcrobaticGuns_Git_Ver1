//======================================================================================
//
//１１月１０日：プレイヤーアクションモードクラスを作る[player_actionmode.h]
//Author:ShinaTaiyo
//
//======================================================================================

//============================================
//２重インクルード防止
//============================================
#ifndef _PLAYER_ACTIONMODE_H_
#define _PLAYER_ACTIONMODE_H_
//======================================================================================

//============================================
//インクルード
//============================================
#include "main.h"
#include "object2d.h"
//======================================================================================

//============================================
//前方宣言
//============================================
class CPlayer;

//============================================
//スーパークラス
//============================================

//プレイヤーアクションモードクラス
class CPlayer_ActionMode : public CObject2D
{
public:
	CPlayer_ActionMode();        //コンストラクタ
	~CPlayer_ActionMode() override; //デストラクタ
	HRESULT Init() override;        //初期化処理
	void Uninit() override;         //終了処理
	void Update() override;         //更新処理
	void Draw() override;           //描画処理
	void SetDeath() override;       //死亡フラグ設定処理
	virtual void Attack(CPlayer* pPlayer) {};           //攻撃
	virtual void Move(CPlayer* pPlayer) {};             //移動量
private:
};
//======================================================================================

//============================================
//アクションモードクラス
//============================================

//プレイヤーショットクラス
class CPlayerShot : public CPlayer_ActionMode
{
public:
	CPlayerShot();//コンストラクタ
	~CPlayerShot() override;//デストラクタ
	HRESULT Init() override;//初期化処理
	void Uninit() override; //終了処理
	void Update() override; //更新処理
	void Draw() override;   //描画処理
	void SetDeath() override;//死亡フラグ設定処理
	static CPlayerShot* Create(const CObject2D::POLYGONTYPE PolygonType,D3DXCOLOR col,const D3DXVECTOR3 Pos, const float fWidth, const float fHeight);
	void Attack(CPlayer* pPlayer) override; //攻撃
private:
};

//プレイヤーダイブクラス
class CPlayerDive : public CPlayer_ActionMode
{
public:
	CPlayerDive(); //コンストラクタ
	~CPlayerDive() override;//デストラクタ
	HRESULT Init() override;//初期化処理
	void Uninit() override; //終了処理
	void Update() override; //更新処理
	void Draw() override;   //描画処理
	void SetDeath() override;//死亡フラグ設定処理
	static CPlayerDive* Create(const CObject2D::POLYGONTYPE PolygonType, D3DXCOLOR col, const D3DXVECTOR3 Pos, const float fWidth, const float fHeight);
	void Move(CPlayer* pPlayer) override;   //移動
};
//======================================================================================

#endif