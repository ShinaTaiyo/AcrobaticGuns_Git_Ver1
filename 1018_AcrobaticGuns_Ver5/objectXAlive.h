//=======================================================================================
//
//６月４日：モデルを描画する[objectX.h]
//Author:ShinaTaiyo
//
//=======================================================================================
#ifndef _OBJECTXALIVE_H_
#define _OBJECTXALIVE_H_

//==============================================
//インクルード
//==============================================
#include "main.h"
#include "objectXMove.h"
//=======================================================================================

//=======================================================
//オブジェクトXアライブクラス
//=======================================================
class CObjectXAlive : public CObjectXMove
{
public:
	CObjectXAlive(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                                      //コンストラクタ
	~CObjectXAlive() override;                                             //デストラクタ
	HRESULT Init() override;                                               //初期化処理
	void Uninit() override;                                                //終了処理
	void Update() override;                                                //更新処理
	void Draw() override;                                                  //描画処理	
	//==========================================================
	//判定関係
	//==========================================================

	//正方形の押し出し判定

	//X
	void SetExtrusionCollisionSquareX(bool bSuccess) { m_bExtrusionCollisionSquareX = bSuccess;}
	const bool & GetExtrusionCollisionSquareX() const { return m_bExtrusionCollisionSquareX; }

	//Y
	void SetExtrusionCollisionSquareY(bool bSuccess) { m_bExtrusionCollisionSquareY = bSuccess;}
	const bool & GetExtrusionCollisionSquareY() const { return m_bExtrusionCollisionSquareY; }

	//Z
	void SetExtrusionCollisionSquareZ(bool bSuccess) { m_bExtrusionCollisionSquareZ = bSuccess;}
	const bool &  GetExtrusionCollisionSquareZ() const { return m_bExtrusionCollisionSquareZ; }

	//地面にいるかどうか
	void SetIsLanding(bool bLanding) { m_bIsLanding = bLanding;}
	const bool& GetLanding() const { return m_bIsLanding; }

	//============================================================================================================

	//======================================================
	//体力関係
	//======================================================

	//体力
	void SetLife(int nLife) { m_nLife = nLife; }                    //体力を設定する
	int& GetLife() { return m_nLife; }                              //体力を取得する

	//最大体力
	void SetMaxLife(int nMaxLife) { m_nMaxLife = nMaxLife; }        //最大体力を設定する
	int& GetMaxLife() { return m_nMaxLife; }                        //最大体力を取得する

	//ヒットストップ
	void SetHitStopTime(int nHitStopTime) { m_nHitStopTime = nHitStopTime; }//ヒットストップ時間を設定する
	bool& GetHitStop() { return m_bHitStop; }                       //ヒットストップ状態かどうかを取得する

	//自動的に体力を減らす
	void SetAutoSubLife(bool bUse) { m_bAutoSubLife = bUse; }       //体力を自動的に減らすかどうかを設定
	int& GetHitStopTime() { return m_nHitStopTime; }                //ヒットストップ時間を取得する

	//体力によって透明度を変えるかどうか
	void SetUseRatioLifeAlpha(bool bUse) { m_bUseRatioLifeAlpha = bUse; }

	//ダメージを与える
	virtual void SetDamage(int nDamage, int nHitStopTime);          //ダメージを与える

	//回復する
	void SetHeal(int nHeal, D3DXCOLOR col, float fWidth, float fHeight);                  //回復量を与える

	//体力０で自動的に破棄するかどうか
	void SetAutoDeath(bool bUse) { m_bAutoDeath = bUse; }
	const bool& GetAutoDeath() const { return m_bAutoDeath; }
	//=================================================================================================================

	//==========================================================
	//エディタ関係
	//==========================================================
	//関数
	void SaveInfoTxt(fstream& WritingFile) override;//テキストファイルに情報を保存するための関数
	void ManagerChooseControlInfo() override;              //ステージマネージャーが情報を操作する
	//=================================================================================================================
protected:
private:
	//===================================
	//体力関係
	//===================================

	//ヒットストップ
	bool m_bHitStop;                                //ヒットストップ!
	int m_nHitStopTime;                             //ヒットストップ時間!
	void HitStopProcess();                          //ヒットストップの処理

	//自動的に体力を減らすかどうか
	bool m_bAutoSubLife;                            //自動的に体力を減らし続けるかどうか!

	//体力
	int m_nLife;                                    //体力!
	int m_nMaxLife;                                 //最大体力!

	//色
	bool m_bUseRatioLifeAlpha;                      //体力の割合で透明度を変えるかどうか!

	//死亡
	bool m_bAutoDeath;                             //体力０で自動的に破棄されるかどうか
	//============================================================================================================

	//===================================
	//判定関係
	//===================================

	//正方形の押し出し判定
	bool m_bExtrusionCollisionSquareX;//X
	bool m_bExtrusionCollisionSquareY;//Y
	bool m_bExtrusionCollisionSquareZ;//Z

	//地面にいるかどうか
	bool m_bIsLanding;

	//============================================================================================================

	//===================================
	//エディタ関係
	//===================================
	void EditLife();//体力を変更する
	//============================================================================================================
};
//============================================================================================================
#endif
