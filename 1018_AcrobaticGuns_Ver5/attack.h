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
#include "object.h"
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
		MAGICSWORD,
		MAX
	};

	enum class COLLISIONTYPE
	{
		NONE = 0,
		SQUARE,
		RECTANGLE_XZ,
		MAX
	};

	enum class TARGETTYPE
	{
		NONE = 0,
		PLAYER,
		ENEMY,
		MAX
	};

	struct HitStop
	{
		int nCntTime;
		const int nSetTime;
	};
	CAttack(int nPower,int nSetHitStopTime,int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ATTACK, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                  //コンストラクタ
	~CAttack();                 //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	void SetCollisionRelease(bool bUse) { m_bCollisionRelease = bUse; }
	const HitStop & GetHitStop() const { return m_HitStop; }
	const int& GetPower() const { return m_nPower; }
	const bool& GetCollisionRelease() const{ return m_bCollisionRelease; }

	//判定タイプを設定する
	void SetCollisionType(COLLISIONTYPE Type) { m_CollisionType = Type; }
	const COLLISIONTYPE GetCollisionType() const { return m_CollisionType; }

	//ターゲットタイプを設定する
	void SetTargetType(TARGETTYPE Type) { m_TargetType = Type; }
	const TARGETTYPE GetTargetType() const { return m_TargetType; }

	//生きているオブジェクト以外にも当たるかどうかを設定
	void SetHitOtherThanLibing(bool bHit) { m_bHitOtherThanLiving = bHit; }
	const bool& GetHitOtherThanLibing() const { return m_bHitOtherThanLiving; }

	//当たり判定を攻撃クラスに任せるかどうか
	void SetAutoCollision(bool bAuto) { m_bAutoCollision = bAuto; }
	const bool& GetAutoCollision() { return m_bAutoCollision; }


protected:
	void SetAttackType(ATTACKTYPE AttackType) { m_Type = AttackType;}//攻撃の種類を設定する
	const ATTACKTYPE & GetAttackType() const { return m_Type; }      //攻撃の種類を取得する
	static const string ATTACK_FILENAME[static_cast<int>(ATTACKTYPE::MAX)];//攻撃モデルのファイル名 
	const bool& GetCollisionSuccess() const { return m_bCollisionSuccess; }
private:

	//================================================
	//変数宣言
	//================================================
	int m_nPower;     //攻撃力

	bool m_bCollisionRelease;//衝突時に消すかどうか

	bool m_bHitOtherThanLiving;//生きているオブジェクト以外にも当たるかどうか
	bool m_bAutoCollision;     //当たり判定を攻撃クラスに任せるかどうか

	bool m_bCollisionSuccess; //判定が成功したかどうか

	HitStop m_HitStop;//ヒットストップ
	ATTACKTYPE m_Type;//タイプ
	COLLISIONTYPE m_CollisionType;//判定タイプ
	TARGETTYPE m_TargetType;//狙うオブジェクトの種類
	//==========================================================================================

	//================================================
	//プロトタイプ宣言
	//================================================
	void Collision();//当たり判定を行う処理
	void CollisionProcess(bool& bCollision,bool & bNowCollision,CObjectXAlive * pObjX);
	//==========================================================================================
};
//==================================================================================================================================================

//===========================================
//プレイヤー攻撃クラス
//===========================================
class CAttackPlayer : public CAttack
{
public:
	CAttackPlayer(int nPower,int nSetHitStopTime);      //コンストラクタ
	~CAttackPlayer() override;                 //デストラクタ
	HRESULT Init() override;          //初期化処理
	void Uninit() override;           //終了処理
	void Update() override;           //更新処理
	void Draw() override;             //描画処理
	void SetDeath() override;         //死亡フラグを設定
	static CAttackPlayer* Create(ATTACKTYPE AttackType,TARGETTYPE TargetType,COLLISIONTYPE CollisionType,bool bHitOtherThanLiving,bool bAutoCollision,int nPower,int nSetHitStopTime,int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale);//生成処理
private:
};
//==================================================================================================================================================

//===========================================
//敵攻撃クラス
//===========================================
class CAttackEnemy : public CAttack
{
public:
	CAttackEnemy(int nPower,int nSetHitStopTime);         //コンストラクタ
	~CAttackEnemy() override;                  //デストラクタ
	HRESULT Init() override;          //初期化処理
	void Uninit() override;           //終了処理
	void Update() override;           //更新処理
	void Draw() override;             //描画処理
	void SetDeath() override;         //死亡フラグを設定
	static CAttackEnemy* Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,bool bHitOtherThanLiving,bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale);//生成処理
private:

};
//==================================================================================================================================================
#endif
