//====================================================
//
//８月２９日：計算用のクラスを作る[calculation.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _CALCULATION_H_  
#define _CALCULATION_H_

//======================
//インクルード
//======================
#include "main.h"
//==========================================

//==========================================

//==========================================
//計算クラス
//==========================================
class CCalculation
{
public:
    //==============================
	//移動方向列挙型
	//==============================
	typedef enum
	{
		MOVEAIM_XY = 0,//XY方向に動く
		MOVEAIM_XZ,    //XZ方向に動く
		MOVEAIM_ZY,    //ZY方向に動く
		MOVEAIM_MAX
	}MOVEAIM;
	//=============================================================================================================

	//==============================
	//プロトタイプ宣言
	//==============================
	CCalculation();                   //コンストラクタ
	~CCalculation();                  //デストラクタ
	static float CalculationLength(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos);//2点の距離を計算する
	static float CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos);//２次元の目的方向への角度を求める
	static float CalculationParabola(float fLength, float fGravity, float fSpeed,D3DXVECTOR3 Pos,D3DXVECTOR3 PurposePos);//距離、重力、速度をもとに２点の放物線移動を計算する
	static float CalculationRandVecXY();//ランダムな２次元ベクトルを返す
	static void CalculationCollectionRot2D(float& fMyRot, float fRotAim, float fDecayRot);//２次元方向の向きの補正を行う
	static bool CaluclationMove(bool bUseStick,D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim,float & fRot);//移動に使う処理の角度を求める
	static D3DXVECTOR3 Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed);  //目的への移動量を求める

	// スクリーン座標をワールド座標に変換
	static D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout,
		int Sx,  // スクリーンX座標
		int Sy,  // スクリーンY座標
		float fZ,  // 射影空間でのZ値（0～1）
		int Screen_w,
		int Screen_h,
		D3DXMATRIX* View,
		D3DXMATRIX* Prj
	);

	// XZ平面とスクリーン座標の交点算出関数
	static D3DXVECTOR3* CalcScreenToXZ(
		D3DXVECTOR3* pout,
		int Sx,
		int Sy,
		int Screen_w,
		int Screen_h,
		D3DXMATRIX* View,
		D3DXMATRIX* Prj
	);

	//レイと球の衝突判定
	static bool CalcRaySphere(
		D3DXVECTOR3 LayPos,//レイの支点
		D3DXVECTOR3 LayVec,//レイの方向ベクトル
		D3DXVECTOR3 SphereSenterPos,//球の中心点
		float r,                    //球の半径
		D3DXVECTOR3 & CollisionStartPos,//レイの衝突開始位置
		D3DXVECTOR3 & CollisoinEndPos   //レイの衝突終了位置
	);
	//目的の物体とレイの向きが一致しているかどうかを判定
	static bool CalcMatchRay(D3DXVECTOR3 AimPos, float fSx, float fSy, int nScreen_w, int nScreen_h, D3DXMATRIX* View, D3DXMATRIX * Prj);

	static int CalculationDigit(int nNum);//桁数を計算する
	//=============================================================================================================
private:
};
#endif
