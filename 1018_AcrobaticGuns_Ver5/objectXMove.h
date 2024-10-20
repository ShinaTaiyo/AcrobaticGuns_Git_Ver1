//=======================================================================================
//
//６月４日：モデルを描画する[objectX.h]
//Author:ShinaTaiyo
//
//=======================================================================================
#ifndef _OBJECTXMOVE_H_
#define _OBJECTXMOVE_H_

//==============================================
//インクルード
//==============================================
#include "main.h"
#include "objectX.h"
//=======================================================================================

//=======================================================
//オブジェクトXクラス
//=======================================================
class CObjectXMove : public CObjectX
{
public:
	CObjectXMove();                                                       //コンストラクタ
	~CObjectXMove() override;                                             //デストラクタ
	HRESULT Init() override;                                              //初期化処理
	void Uninit() override;                                               //終了処理
	void Update() override;                                               //更新処理
	void Draw() override;                                                 //描画処理	

    //==========================================================
	//移動量
	//==========================================================
	//移動量
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }                                       //移動量を設定する
	D3DXVECTOR3& GetMove() { return m_Move; }                                               //移動量を取得する

	//慣性
	void SetUseInteria(bool bUseInteria) { m_bUseInteria = bUseInteria; }                   //慣性をかけるかどうか
	float& GetInertia() { return m_fInertia; }                                              //慣性を取得する
	void SetInertia(float fInertia) { m_fInertia = fInertia; }                              //慣性を設定する
	void SetUndoInertia() { m_fInertia = m_fNORMAL_INERTIA; }                               //慣性の大きさを元に戻す
	bool& GetUseInteria() { return m_bUseInteria; }                                         //慣性を掛けるかどうかを取得

	//重力
	void SetUseGravity(bool bUse, float fGravityPower) { m_bUseGravity = bUse; m_fGravityPower = fGravityPower; }//重力を設定する
	const bool& GetUseGravity() const { return m_bUseGravity; }                                                  //重力を使用するかどうかを設定

	//乗算加速
	void SetUseMultiSpeed(bool bUse, D3DXVECTOR3 MultiSpeed) { m_MultiSpeed = MultiSpeed; m_bUseMultiSpeed = bUse; }//乗算加速を使用するかどうか

	//加算速度
	void SetUseAddSpeed(bool bUse, D3DXVECTOR3 AddSpeed) { m_bUseAddSpeed = bUse; m_AddSpeed = AddSpeed; }//加速を使用するかどうか
	//============================================================================================================

	//===================================
	//位置更新関係
	//===================================
	void SetUseUpdatePos(bool bUse) { m_bUseUpdatePos = bUse; }//位置の更新を使用するかどうか
	//=================================================================================================================
protected:
	static const float GetNormalGravity() { return m_fNORMAL_GRAVITY; }//基本の重力を取得する
	virtual void GravityProcess();                  //重力の処理
private:
	//===============================================
	//静的メンバ
	//===============================================
	static constexpr float m_fNORMAL_INERTIA = 0.5f;//普通の慣性
	static constexpr float m_fNORMAL_GRAVITY = 1.0f;//普通の重力
	//=================================================================================================================
	
	//==========================================================
	//移動量
	//==========================================================
	
	//移動量
	D3DXVECTOR3 m_Move;                             //移動量!
	//慣性
	float m_fInertia;                               //慣性!
	bool m_bUseInteria;                             //慣性をかけるかどうか!
	//重力
	bool m_bUseGravity;                             //重力を使用するかどうか!
	float m_fGravityPower;                          //重力の大きさ!
	//乗算加速
	bool m_bUseMultiSpeed;                                  //乗算加速をするかどうか!
	D3DXVECTOR3 m_MultiSpeed;                               //乗算加速度!
	//加速
	bool m_bUseAddSpeed;                                    //加速を使用するかどうか!
	D3DXVECTOR3 m_AddSpeed;                                 //加速度

	//位置の更新をするかどうか
	bool m_bUseUpdatePos;                           //位置の更新を使用するかどうか!
	//=================================================================================================================

	//==========================================================
	//移動量
	//==========================================================
	void UpdatePos();                               //位置を更新させる

};
//============================================================================================================
#endif
