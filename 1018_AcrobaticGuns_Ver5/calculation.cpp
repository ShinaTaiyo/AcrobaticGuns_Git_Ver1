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
	if (fMyRot > D3DX_PI * 0.5f + fCameraRot && fRotAim < -D3DX_PI * 0.5f + fCameraRot)
	{
		fRotDiff += D3DX_PI * 2;
	}
	if (fMyRot < -D3DX_PI * 0.5f + fCameraRot && fRotAim > D3DX_PI * 0.5f + fCameraRot)
	{
		fRotDiff -= D3DX_PI * 2;
	}

	//徐々に目的の向きへ合わせていく
	fMyRot += fRotDiff * fDecayRot;

	//向きの調整（値を3.14～-3.14の中に固定したいので・・・）
	if (fMyRot > D3DX_PI + fCameraRot)
	{//3.14→-3.14にする
		fMyRot -= D3DX_PI * 2;
	}
	else if (fMyRot < -D3DX_PI + fCameraRot)
	{//-3.14→3.14にする
		fMyRot += D3DX_PI * 2;
	}
}
//===========================================================================================================

//=========================================================
//移動方向への角度を計算する
//=========================================================
bool CCalculation::CaluclationMove(D3DXVECTOR3& Pos, float fSpeed, MOVEAIM MoveAim, float& fRot)
{
	float fCameraRot = CManager::GetCamera()->GetRot().y;
	float fMoveX = 0.0f;                                            //X方向の移動量
	float fMoveZ = 0.0f;                                            //Z方向の移動量
	bool bMove = false;                                             //移動しているかどうか 
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


	if (bMove == true)
	{//移動状態なら
		fRot = atan2f(fMoveX, fMoveZ) + fCameraRot;
		switch (MoveAim)
		{
		case MOVEAIM_XY:
			Pos.x += sinf(fRot) * fSpeed;
			Pos.y += cosf(fRot) * fSpeed;
			break;
		case MOVEAIM_XZ:
			Pos.x += sinf(fRot) * fSpeed;
			Pos.z += cosf(fRot) * fSpeed;
			break;
		case MOVEAIM_ZY:
			Pos.z += sinf(fRot) * fSpeed;
			Pos.y += cosf(fRot) * fSpeed;
			break;
		default:
			break;
		}
	}

	return bMove;
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