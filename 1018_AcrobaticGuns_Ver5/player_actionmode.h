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

////============================================
////スーパークラス
////============================================
//
////プレイヤーアクションモードクラス
//class CPlayer_ActionMode
//{
//public:
//	CPlayer_ActionMode();        //コンストラクタ
//	virtual ~CPlayer_ActionMode(); //デストラクタ
//	virtual void Attack(CPlayer* pPlayer) {};           //攻撃
//	virtual void Move(CPlayer* pPlayer) {};             //移動量
//	virtual void Collision(CPlayer* pPlayer) {};        //当たり判定処理
//
//protected:
//	void PlayerControl(CPlayer* pPlayer);               //プレイヤーを動かす処理
//private:
//};
////======================================================================================
//
////============================================
////アクションモードクラス
////============================================
//
////プレイヤーショットクラス
//class CPlayerShot : public CPlayer_ActionMode
//{
//public:
//	CPlayerShot();//コンストラクタ
//	~CPlayerShot() override;//デストラクタ
//	void Attack(CPlayer* pPlayer) override;       //攻撃
//	void Move(CPlayer* pPlayer) override;         //移動
//	void Collision(CPlayer* pPlayer) override;    //当たり判定処理
//private:
//};
//
////プレイヤーダイブクラス
//class CPlayerDive : public CPlayer_ActionMode
//{
//public:
//	CPlayerDive(); //コンストラクタ
//	~CPlayerDive() override;//デストラクタ
//	void Move(CPlayer* pPlayer) override;   //移動
//	void Collision(CPlayer* pPlayer) override;    //当たり判定処理
//private:
//	bool m_bCollision;//判定したかどうかを取得
//	bool m_bActiveDive;//ダイブ中かどうか
//};
//======================================================================================

//==============================================
//移動クラス
//==============================================

//スーパークラス
class CPlayerMove
{
public:
	CPlayerMove();                             //コンストラクタ
	virtual ~CPlayerMove();                    //デストラクタ
	virtual void MoveProcess(CPlayer* pPlayer);//移動処理
};

//通常移動クラス
class CPlayerMove_Normal : public CPlayerMove
{
public:
	CPlayerMove_Normal();                    //コンストラクタ
	~CPlayerMove_Normal() override;                   //デストラクタ
	void MoveProcess(CPlayer* pPlayer) override;//移動処理
};

//ダイブ移動クラス
class CPlayerMove_Dive : public CPlayerMove
{
public:
	CPlayerMove_Dive();                      //コンストラクタ
	~CPlayerMove_Dive() override;                     //デストラクタ
	void MoveProcess(CPlayer* pPlayer) override;//移動処理
	void SetDiveMove(D3DXVECTOR3 Move) { m_DiveMove = Move; }//ダイブ時の移動量を設定
private:
	D3DXVECTOR3 m_DiveMove;//ダイブの移動量
};

//ダイブ準備クラス
class CPlayerMove_PrepDive : public CPlayerMove
{
public:
	CPlayerMove_PrepDive();                      //コンストラクタ
	~CPlayerMove_PrepDive() override;            //デストラクタ
	void MoveProcess(CPlayer* pPlayer) override; //移動処理
};

//移動できないクラス
class CPlayerMove_Dont : public CPlayerMove
{
public:
	CPlayerMove_Dont();//コンストラクタ
	~CPlayerMove_Dont() override;//デストラクタ
	void MoveProcess(CPlayer* pPlayer) override;//移動処理
private:
};
//======================================================================================



//==============================================
//攻撃クラス
//==============================================

//スーパークラス
class CPlayerAttack
{
public:
	CPlayerAttack() {};//コンストラクタ
	virtual ~CPlayerAttack() {};//デストラクタ
	virtual void AttackProcess(CPlayer* pPlayer) {};//攻撃処理
};

//射撃クラス
class CPlayerAttack_Shot : public CPlayerAttack
{
public:
	CPlayerAttack_Shot();//コンストラクタ
	~CPlayerAttack_Shot() override;//デストラクタ
	void AttackProcess(CPlayer* pPlayer) override;//攻撃処理
};

//ダイブクラス
class CPlayerAttack_Dive : public CPlayerAttack
{
public:
	CPlayerAttack_Dive();//コンストラクタ
	~CPlayerAttack_Dive();//デストラクタ
	void AttackProcess(CPlayer* pPlayer) override;//攻撃処理
};

//攻撃できないクラス
class CPlayerAttack_Dont : public CPlayerAttack
{
public:
	CPlayerAttack_Dont();//コンストラクタ
	~CPlayerAttack_Dont() override;//デストラクタ
	void AttackProcess(CPlayer* pPlayer) override;//攻撃処理
};
//======================================================================================


#endif