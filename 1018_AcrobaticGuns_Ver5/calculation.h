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
	static bool CaluclationMove(D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim,float & fRot);//移動に使う処理の角度を求める
	static D3DXVECTOR3 Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed);  //目的への移動量を求める
	static int CalculationDigit(int nNum);//桁数を計算する
	//=============================================================================================================
private:
};
#endif
