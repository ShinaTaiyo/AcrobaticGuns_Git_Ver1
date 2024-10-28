//===========================================================================================================
//
//８月２９日：計算用のクラスを作る[calculation.cpp]
//Author:ShinaTaiyo
//
//===========================================================================================================

//=========================================================
//インクルード
//=========================================================
#include "calculation.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "input.h"
//===========================================================================================================

//=========================================================
//コンストラクタ
//=========================================================
CCalculation::CCalculation()
{

}
//===========================================================================================================

//=========================================================
//デストラクタ
//=========================================================
CCalculation::~CCalculation()
{

}
//===========================================================================================================

//=========================================================
//距離を計算する
//=========================================================
float CCalculation::CalculationLength(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fLength = 0.0f;
	fLength = sqrtf(powf(PurposePos.x - Pos.x, 2) + powf(PurposePos.y - Pos.y, 2) + powf(PurposePos.z - Pos.z, 2));
	return fLength;
}
//==========================================================================================================

//=========================================================
//XYベクトルを計算する
//=========================================================
float CCalculation::CalculationXYaim(D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fVXaim = PurposePos.x - Pos.x;
	float fVYaim = PurposePos.y - Pos.y;
	float fVLaim = atan2f(fVXaim, fVYaim);
	return fVLaim;
}
//==========================================================================================================

//=========================================================
//放物線の角度を計算する
//=========================================================
float CCalculation::CalculationParabola(float fLength, float fGravity, float fSpeed, D3DXVECTOR3 Pos, D3DXVECTOR3 PurposePos)
{
	float fRot = 0.0f;//角度

	float fXG = fLength * fGravity;//距離×重力
	float fV = fSpeed;             //速度
	float fV2 = powf(fV, 2);       //速度２条

	fRot = asinf(fXG / fV2);
	fRot /= 2;

	if (PurposePos.x < Pos.x)
	{//角度が絶対値なので、目的の位置が自分の位置よりマイナス方向にある場合、負の値にする
		fRot *= -1;
	}

	return fRot;
}
//===========================================================================================================

//=========================================================
//XY方向の乱数ベクトルを用意する
//=========================================================
float CCalculation::CalculationRandVecXY()
{
	float fRandRot = float(rand() % 628 + 1) / 100;
	return fRandRot;
}
//===========================================================================================================

//=========================================================
//2D方向の向きの補正を行う
//=========================================================
void CCalculation::CalculationCollectionRot2D(float& fMyRot, float fRotAim, float fDecayRot)
{
	float fRotDiff = 0.0f;//向きの差分
	float fCameraRot = CManager::GetCamera()->GetRot().y;
	fRotDiff = fRotAim - fMyRot;

	//向きの差分を求める（3.14と-3.14の境界線をまたぎそうになったら補正する)
	//if (fMyRot > D3DX_PI * 0.5f + fCameraRot && fRotAim < -D3DX_PI * 0.5f + fCameraRot)//1.57以上 -1.57未満
	//{
	//	fRotDiff += D3DX_PI * 2 + fCameraRot;
	//}
	//if (fMyRot < -D3DX_PI * 0.5f + fCameraRot && fRotAim > D3DX_PI * 0.5f + fCameraRot)
	//{
	//	fRotDiff -= D3DX_PI * 2 + fCameraRot;
	//}

	//向きの差分の調整
	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= D3DX_PI * 2;
	}
	else if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += D3DX_PI * 2;
	}


	//徐々に目的の向きへ合わせていく
	fMyRot += fRotDiff * fDecayRot;

	//向きの調整（カメラを基準に値を3.14～-3.14の中に固定したいので・・・）
	if (fMyRot >= D3DX_PI + fCameraRot)
	{//3.14→-3.14にする
		fMyRot -= D3DX_PI * 2;
	}
	else if (fMyRot <= -D3DX_PI + fCameraRot)
	 
	{//-3.14→3.14にする
		fMyRot += D3DX_PI * 2;
	}
}
//===========================================================================================================

//=========================================================
//移動方向への角度を計算する
//=========================================================
bool CCalculation::CaluclationMove(bool bUseStick, D3DXVECTOR3& Move, float fSpeed, MOVEAIM MoveAim, float& fRot)
{
	float fCameraRot = CManager::GetCamera()->GetRot().y;
	float fMoveX = 0.0f;                                            //X方向の移動量
	float fMoveZ = 0.0f;                                            //Z方向の移動量
	bool bMove = true;                                             //移動しているかどうか 

	if (bUseStick == false)
	{
		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_UP) == true)
		{
			fMoveZ = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_DOWN) == true)
		{
			fMoveZ = -1.0f;
		}
		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_RIGHT) == true)
		{
			fMoveX = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_LEFT) == true)
		{
			fMoveX = -1.0f;
		}

		if (fMoveX != 0.0f || fMoveZ != 0.0f)
		{
			bMove = true;//移動状態
		}
		else
		{
			bMove = false;//待機状態
		}
	}
	else
	{
		bMove = CManager::GetInputJoypad()->GetLStickPress(8);
	}
	if (bMove == true)
	{//移動状態なら

		if (bUseStick == false)
		{//WASDのボタン入力を基準に向きを決める
			fRot = atan2f(fMoveX, fMoveZ);
		}
		else
		{
			//カメラを基準に向きを決める
			fRot = fCameraRot + CManager::GetInputJoypad()->GetLStickAimRot();
		}
		switch (MoveAim)
		{
		case MOVEAIM_XY:
			Move.x = sinf(fRot) * fSpeed;
			Move.y = cosf(fRot) * fSpeed;
			break;
		case MOVEAIM_XZ:
			Move.x = sinf(fRot) * fSpeed;
			Move.z = cosf(fRot) * fSpeed;
			break;
		case MOVEAIM_ZY:
			Move.z = sinf(fRot) * fSpeed;
			Move.y = cosf(fRot) * fSpeed;
			break;
		default:
			break;
		}
	}

	return bMove;
}
//===========================================================================================================


//=========================================================
//目的の位置への移動量を計算する
//=========================================================
D3DXVECTOR3 CCalculation::Calculation3DVec(D3DXVECTOR3 MyPos, D3DXVECTOR3 AimPos, float fSpeed)

{
	D3DXVECTOR3 VecAim = NULL_VECTOR3;       //それぞれの方向のベクトル
	D3DXVECTOR3 ResultMove = NULL_VECTOR3;   //結果の移動量
	float fVLaim = 0.0f;                     //総合ベクトル

	//==========================
	//３次元ベクトルを取る
	//==========================

	//それぞれの方向のベクトルを求める
	VecAim.x = MyPos.x - AimPos.x;
	VecAim.y = MyPos.y - AimPos.y;
	VecAim.z = MyPos.z - AimPos.z;

	//総合ベクトルを求める
	fVLaim = sqrtf(powf(VecAim.x,2) + powf(VecAim.y,2) + powf(VecAim.z,2));

	//目的の位置への移動量を求める
	ResultMove.x = VecAim.x / fVLaim * fSpeed;
	ResultMove.y = VecAim.y / fVLaim * fSpeed;
	ResultMove.z = VecAim.z / fVLaim * fSpeed;

	return ResultMove;
}
//===========================================================================================================

//=========================================================
//桁数を計算する
//=========================================================
int CCalculation::CalculationDigit(int nNum)
{
	int nDigit = 0;
	while (nNum != 0) {
		nNum /= 10;
		nDigit++;
	}
	return nDigit;
}
//===========================================================================================================