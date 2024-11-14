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
#include "debugproc.h"
#include "particle.h"
#include "attack.h"
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
		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::UP) == true)
		{
			fMoveZ = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::DOWN) == true)
		{
			fMoveZ = -1.0f;
		}
		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RIGHT) == true)
		{
			fMoveX = 1.0f;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true || CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LEFT) == true)
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
		bMove = CManager::GetInputJoypad()->GetLStickPress(8,0.0f);
	}
	if (bMove == true)
	{//移動状態なら
		//カメラを基準に向きを決める
		if (bUseStick == true)
		{
			fRot = fCameraRot + CManager::GetInputJoypad()->GetLStickAimRot() + D3DX_PI;
		}
		else
		{
			fRot = atan2f(fMoveX, fMoveZ) + fCameraRot + D3DX_PI;
		}
		switch (MoveAim)
		{
		case MOVEAIM_XY:
			Move.x = sinf(fRot) * fSpeed;
			Move.y = cosf(fRot) * fSpeed;
			break;
		case MOVEAIM_XZ:
			Move.x = sinf(fRot - D3DX_PI) * fSpeed;
			Move.z = cosf(fRot - D3DX_PI) * fSpeed;
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
	D3DXVECTOR3 VecAim = D3DXVECTOR3(0.0f,0.0f,0.0f);       //それぞれの方向のベクトル
	D3DXVECTOR3 ResultMove = D3DXVECTOR3(0.0f,0.0f,0.0f);   //結果の移動量
	float fVLaim = 0.0f;                     //総合ベクトル

	//==========================
	//３次元ベクトルを取る
	//==========================

	//それぞれの方向のベクトルを求める
	VecAim.x = AimPos.x - MyPos.x;
	VecAim.y = AimPos.y - MyPos.y;
	VecAim.z = AimPos.z - MyPos.z;

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
//虹色を計算する
//=========================================================
D3DXCOLOR CCalculation::CalRaibowColor()
{
	D3DXCOLOR RaibowColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	RaibowColor.r = float(rand() % 100 + 1) / 100;
	RaibowColor.g = float(rand() % 100 + 1) / 100;
	RaibowColor.b = float(rand() % 100 + 1) / 100;
	return RaibowColor;
}
//===========================================================================================================

//=========================================================
// スクリーン座標をワールド座標に変換
//=========================================================
D3DXVECTOR3* CCalculation::CalcScreenToWorld(D3DXVECTOR3* pout, float Sx, float Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	// 各行列の逆行列を算出
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);//ビューマトリックスとの逆光列
	D3DXMatrixInverse(&InvPrj, NULL, Prj);  //プロジェクションマトリックスとの逆光列
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	//自分
	D3DXVECTOR3 MyPos = D3DXVECTOR3(Sx,Sy,fZ);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;//ワールド座標を求める
	D3DXVec3TransformCoord(pout, &MyPos, &tmp);     //位置を求める
	return pout;
}
//===========================================================================================================

//=========================================================
// ワールド座標をスクリーン座標に変換する
//=========================================================
D3DXVECTOR3 CCalculation::CalcWorldToScreenNoViewport(D3DXVECTOR3 worldPos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float screenWidth, float screenHeight)
{
	//ワールド座標をクリップ座標に変換
	D3DXVECTOR4 ClipSpacePos;
	D3DXMATRIX mtxTrans;

	mtxTrans = viewMatrix * projectionMatrix;
	D3DXVec3Transform(&ClipSpacePos, &worldPos, &mtxTrans);

	//透視除算（クリップ座標からNDC空間へ）
	if (ClipSpacePos.w != 0.0f)
	{
		ClipSpacePos.x /= ClipSpacePos.w;
		ClipSpacePos.y /= ClipSpacePos.w;
		ClipSpacePos.z /= ClipSpacePos.w;
	}

	//スクリーン座標へ変換
	D3DXVECTOR3 ScreenPos;
	ScreenPos.x = (ClipSpacePos.x * 0.5f + 0.5f) * screenWidth;
	ScreenPos.y = (1.0f - (ClipSpacePos.y * 0.5f + 0.5f)) * screenHeight;
	ScreenPos.z = ClipSpacePos.z;//深度値（０～１）の範囲

	return ScreenPos;
}
//===========================================================================================================

//=========================================================
// XZ平面とスクリーン座標の交点算出関数
//=========================================================
D3DXVECTOR3* CCalculation::CalcScreenToXZ(D3DXVECTOR3* pout,float Sx,float Sy, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	bool bCross = false;

	D3DXVECTOR3 Pos1 = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);//（椎名）多分カメラの位置
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj); //（椎名）多分描画範囲の一番奥の位置
	ray = farpos - nearpos;

	D3DXVec3Normalize(&ray, &ray);

	nearpos *= -1;
	// 床との交差が起きている場合は交点を
	// 起きていない場合は遠くの壁との交点を出力
	//if (ray.y <= 0) {
	//	// 床交点
	//	float Lray = D3DXVec3Dot(&ray, &Pos1);
	//	float LP0 = D3DXVec3Dot(&nearpos, &Pos1);
	//	*pout = nearpos + (LP0 / Lray) * ray;
	//	bCross = true;
	//	//CManager::GetDebugProc()->PrintDebugProc("Lray:%f\n", Lray);
	//	//CManager::GetDebugProc()->PrintDebugProc("LP0:%f\n",LP0);
	//}
	//else {
	* pout = farpos;
	bCross = false;
	//}
	//CManager::GetDebugProc()->PrintDebugProc("レイの向き：%f %f %f\n", ray.x, ray.y, ray.z);
	//CManager::GetDebugProc()->PrintDebugProc("レイが床と交差しているかどうか：%d\n", bCross);

	return pout;
}
//===========================================================================================================

//=========================================================
// レイと球の衝突判定
//=========================================================
bool CCalculation::CalcRaySphere(D3DXVECTOR3 LayPos, D3DXVECTOR3 LayVec, D3DXVECTOR3 SphereSenterPos, float r, D3DXVECTOR3& CollisionStartPos, D3DXVECTOR3& CollisoinEndPos)
{
	SphereSenterPos.x = SphereSenterPos.x - LayPos.x;
	SphereSenterPos.y = SphereSenterPos.y - LayPos.y;
	SphereSenterPos.z = SphereSenterPos.z - LayPos.z;

	float A = LayVec.x * LayVec.x + LayVec.y * LayVec.y + LayVec.z * LayVec.z;
	float B = LayVec.x * SphereSenterPos.x + LayVec.y * SphereSenterPos.y + LayVec.z * SphereSenterPos.z;
	float C = SphereSenterPos.x * SphereSenterPos.x + SphereSenterPos.y * SphereSenterPos.y + SphereSenterPos.z * SphereSenterPos.z - r * r;
	
	CManager::GetDebugProc()->PrintDebugProc("A = %f\n", A);
	CManager::GetDebugProc()->PrintDebugProc("B = %f\n", B);
	CManager::GetDebugProc()->PrintDebugProc("C = %f\n", C);


	if (A == 0.0f)
		return false; // レイの長さが0

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // 衝突していない

	CManager::GetDebugProc()->PrintDebugProc("s = %f\n", s);

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	CManager::GetDebugProc()->PrintDebugProc("a1 = %f\n", a1);
	CManager::GetDebugProc()->PrintDebugProc("a2 = %f\n", a2);

	if (a1 < 0.0f || a2 < 0.0f)
		return false; // レイの反対で衝突

	//衝突開始位置を求める
	CollisionStartPos.x = LayPos.x + a1 * LayVec.x;
	CollisionStartPos.y = LayPos.y + a1 * LayVec.y;
	CollisionStartPos.z = LayPos.z + a1 * LayVec.z;
	CManager::GetDebugProc()->PrintDebugProc("レイ貫通開始位置： %f %f %f\n",CollisionStartPos.x,CollisionStartPos.y,CollisionStartPos.z);

	//衝突終了位置を求める
	CollisoinEndPos.x = LayPos.x + a2 * LayVec.x;
	CollisoinEndPos.y = LayPos.y + a2 * LayVec.y;
	CollisoinEndPos.z = LayPos.z + a2 * LayVec.z;
	CManager::GetDebugProc()->PrintDebugProc("レイ貫通終了位置： %f %f %f\n", CollisoinEndPos.x, CollisoinEndPos.y, CollisoinEndPos.z);

	return true;
}
//===========================================================================================================

//=========================================================
//目的の位置と狙っている位置とのレイが一致しているかどうかを判定
//=========================================================
bool CCalculation::CalcMatchRay(D3DXVECTOR3 AimPos, float fSx, float fSy, int nScreen_w, int nScreen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	D3DXVECTOR3 nearpos1;
	D3DXVECTOR3 farpos1;
	D3DXVECTOR3 ray1;
	bool bCross = false;

	D3DXVECTOR3 Pos1 = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	CalcScreenToWorld(&nearpos1,fSx,fSy, 0.0f, nScreen_w, nScreen_h, View, Prj);//（椎名）多分カメラの位置
	CalcScreenToWorld(&farpos1, fSx,fSy, 1.0f, nScreen_w, nScreen_h, View, Prj); //（椎名）多分描画範囲の一番奥の位置
	ray1 = farpos1 - nearpos1;

	D3DXVec3Normalize(&ray1, &ray1);

	D3DXVECTOR3 farpos2;
	D3DXVECTOR3 ray2;

	ray2 = AimPos - nearpos1;//目的の位置とカメラの位置のレイを求める

	D3DXVec3Normalize(&ray2, &ray2);

	CManager::GetDebugProc()->PrintDebugProc("ray1:%f %f %f\n", ray1.x, ray1.y, ray1.z);
	CManager::GetDebugProc()->PrintDebugProc("ray2:%f %f %f\n", ray2.x, ray2.y, ray2.z);

	if (ray1.x >= ray2.x - 0.05f && ray1.x <= ray2.x + 0.05f &&
		ray1.y >= ray2.y - 0.05f && ray1.y <= ray2.y + 0.05f &&
		ray1.z >= ray2.z - 0.05f && ray1.z <= ray2.z + 0.05f)
	{
		CManager::GetDebugProc()->PrintDebugProc("レイの方向が一致\n");
		return true;
	}
	return false;
}
//===========================================================================================================

//=========================================================
//点と辺の距離を求める
//=========================================================
float CCalculation::pointLineDistance(float cx, float cy, float x1, float y1, float x2, float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;

	// 円の中心から辺の端点までのベクトルを計算
	float t = ((cx - x1) * dx + (cy - y1) * dy) / (dx * dx + dy * dy);

	// tの範囲を[0, 1]に制限し、円の中心に最も近い点を見つける
	t = (std::max)(0.0f, (std::min)(1.0f, t));

	// 最短距離を計算
	float closestX = x1 + t * dx;
	float closestY = y1 + t * dy;
	float distanceX = cx - closestX;
	float distanceY = cy - closestY;

	return std::sqrt(distanceX * distanceX + distanceY * distanceY);
}
//===========================================================================================================

//=========================================================
//正方形と円の当たり判定（2D)
//=========================================================
bool CCalculation::checkCollisionCircleRectangle(float cx, float cy, float r, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	// 四角形の各辺に対して円との距離を計算
	if (pointLineDistance(cx, cy, x1, y1, x2, y2) <= r) return true;
	if (pointLineDistance(cx, cy, x2, y2, x3, y3) <= r) return true;
	if (pointLineDistance(cx, cy, x3, y3, x4, y4) <= r) return true;
	if (pointLineDistance(cx, cy, x4, y4, x1, y1) <= r) return true;

	// どの辺とも当たっていない場合はfalse
	return false;
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