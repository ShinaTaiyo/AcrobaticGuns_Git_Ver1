//====================================================
//
//１０月２４日：攻撃クラスを実装する[attack.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _ATTACK_H_  
#define _ATTACK_H_

//======================
//インクルード
//======================
#include "main.h"
#include "objectXAlive.h"
//==========================================

//===========================================
//攻撃クラス
//===========================================
class CAttack : public CObjectXAlive
{
public:
	enum class ATTACKTYPE
	{
		BULLET = 0,
		EXPLOSION,
		MAX
	};

	CAttack(int nPri = 2);                  //コンストラクタ
	~CAttack();                 //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CAttack* Create(ATTACKTYPE AttackType, int nLife,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale);//生成処理
protected:
	void SetAttackType(ATTACKTYPE AttackType) { m_Type = AttackType;}//攻撃の種類を設定する
	const ATTACKTYPE & GetAttackType() const { return m_Type; }      //攻撃の種類を取得する
	static const string ATTACK_FILENAME[static_cast<int>(ATTACKTYPE::MAX)];//攻撃モデルのファイル名 
private:
	ATTACKTYPE m_Type;//タイプ
};
//==================================================================================================================================================

//===========================================
//プレイヤー攻撃クラス
//===========================================
class CAttackPlayer : public CAttack
{
public:
	CAttackPlayer(int nPri = 2);      //コンストラクタ
	~CAttackPlayer();                 //デストラクタ
	HRESULT Init() override;          //初期化処理
	void Uninit() override;           //終了処理
	void Update() override;           //更新処理
	void Draw() override;             //描画処理
	void SetDeath() override;         //死亡フラグを設定
	static CAttackPlayer* Create(ATTACKTYPE AttackType, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale);//生成処理
private:
};
//==================================================================================================================================================

//===========================================
//敵攻撃クラス
//===========================================
class CAttackEnemy : public CAttack
{
public:
	CAttackEnemy();                  //コンストラクタ
	~CAttackEnemy();                 //デストラクタ
	HRESULT Init() override;          //初期化処理
	void Uninit() override;           //終了処理
	void Update() override;           //更新処理
	void Draw() override;             //描画処理
	void SetDeath() override;         //死亡フラグを設定
	static CAttackEnemy* Create(ATTACKTYPE AttackType, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale);//生成処理
private:

};
//==================================================================================================================================================
#endif
