//====================================================================================================================
//
//９月１３日：当たり判定用のクラスを作る[collision.cpp]
//Author:ShinaTaiyo
//
//====================================================================================================================

//================================================================
//インクルード
//================================================================
#include "collision.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"
#include "camera.h"
//====================================================================================================================

//================================================================
//コンストラクタ
//================================================================
CCollision::CCollision()
{

}
//====================================================================================================================

//================================================================
//デストラクタ
//================================================================
CCollision::~CCollision()
{

}
//====================================================================================================================

//================================================================
//正方形の当たり判定を取る
//================================================================
bool CCollision::CollisionSquare(D3DXVECTOR3 MyPos, D3DXVECTOR3 MyVtxMax, D3DXVECTOR3 MyVtxMin,
	D3DXVECTOR3 ComparisonPos, D3DXVECTOR3 ComparisonVtxMax, D3DXVECTOR3 ComparisonVtxMin)
{
	if (MyPos.x + MyVtxMin.x <= ComparisonPos.x + ComparisonVtxMax.x &&
		MyPos.x + MyVtxMax.x >= ComparisonPos.x + ComparisonVtxMin.x &&
		MyPos.y + MyVtxMin.y <= ComparisonPos.y + ComparisonVtxMax.y &&
		MyPos.y + MyVtxMax.y >= ComparisonPos.y + ComparisonVtxMin.y &&
		MyPos.z + MyVtxMin.z <= ComparisonPos.z + ComparisonVtxMax.z &&
		MyPos.z + MyVtxMax.z >= ComparisonPos.z + ComparisonVtxMin.z)
	{
		return true;
	}
	return false;
}
//====================================================================================================================

//================================================================
//正方形の当たり判定を取る
//================================================================
bool CCollision::CollisionBall(D3DXVECTOR3 MySenterPos, D3DXVECTOR3 MySize,
	D3DXVECTOR3 ComparisonSenterPos, D3DXVECTOR3 ComparisonSize)
{
	float fResultMySize = 0.0f;
	float fResultComprisonSize = 0.0f;
	if (MySize.x > MySize.y)
	{//サイズがXの方が大きい
		fResultMySize = MySize.x;
	}
	else
	{//サイズがYの方が大きい
		fResultMySize = MySize.y;
	}

	if (ComparisonSize.x > ComparisonSize.y)
	{//サイズがXの方が大きい
		fResultComprisonSize = ComparisonSize.x;
	}
	else
	{//サイズがYの方が大きい
		fResultComprisonSize = ComparisonSize.y;
	}

	float fCheckLength = fResultMySize / 2 + fResultComprisonSize / 2;

	//中心点からの距離　＋　モデルの端までの距離
	float fLength = CCalculation::CalculationLength(MySenterPos, ComparisonSenterPos);

	if (fLength < fCheckLength)//二つの半径の和より距離が小さければ
	{
		return true;
	}

	return false;
}
//====================================================================================================================

//================================================================
//斜めの当たり判定XY
//================================================================
bool CCollision::RectAngleCollisionXY(D3DXVECTOR3 MyPos, D3DXVECTOR3 MyVtxMax, D3DXVECTOR3 MyVtxMin, D3DXVECTOR3 Rot,
	D3DXVECTOR3 ComparisonPos, D3DXVECTOR3 ComparisonVtxMax, D3DXVECTOR3 ComparisonVtxMin)
{
	D3DXVECTOR2 FourVtxPos[4] = {};   //４頂点の位置
	D3DXVECTOR2 FourVtxRotPos[4] = {};//回転した４頂点の位置
	D3DXVECTOR2 FourComparisonVtxPos[4] = {};    //比較用４頂点
	bool bCollision = true;

	//=====================================
	//比較対象の四角形の４頂点
	//=====================================
	FourComparisonVtxPos[0].x = ComparisonPos.x + ComparisonVtxMin.x;
	FourComparisonVtxPos[0].y = ComparisonPos.y + ComparisonVtxMax.y;

	FourComparisonVtxPos[1].x = ComparisonPos.x + ComparisonVtxMax.x;
	FourComparisonVtxPos[1].y = ComparisonPos.y + ComparisonVtxMax.y;

	FourComparisonVtxPos[2].x = ComparisonPos.x + ComparisonVtxMin.x;
	FourComparisonVtxPos[2].y = ComparisonPos.y + ComparisonVtxMin.y;

	FourComparisonVtxPos[3].x = ComparisonPos.x + ComparisonVtxMax.x;
	FourComparisonVtxPos[3].y = ComparisonPos.y + ComparisonVtxMin.y;

	//=============================================================================================================================================================================

	//=====================================
	//攻撃の四角形の４頂点
	//=====================================

	//左上頂点
	FourVtxPos[0] = D3DXVECTOR2(MyPos.x + MyVtxMin.x, MyPos.y + MyVtxMax.y);

	//右上頂点
	FourVtxPos[1] = D3DXVECTOR2(MyPos.x + MyVtxMax.x, MyPos.y + MyVtxMax.y);

	//左下頂点
	FourVtxPos[2] = D3DXVECTOR2(MyPos.x + MyVtxMin.x, MyPos.y + MyVtxMin.y);

	//右下頂点
	FourVtxPos[3] = D3DXVECTOR2(MyPos.x + MyVtxMax.x, MyPos.y + MyVtxMin.y);

	//=============================================================================================================================================================================

	//==================================
	//中心点と４頂点の距離を求める
	//==================================
	float fLength[4] = {};
	for (int nCnVtx = 0; nCnVtx < 4; nCnVtx++)
	{
		fLength[nCnVtx] = sqrtf(powf((FourVtxPos[nCnVtx].x - MyPos.x), 2) + powf((FourVtxPos[nCnVtx].y - MyPos.y), 2));
	}
	//=============================================================================================================================================================================

	//==================================
	//中心点から４頂点の角度を求める
	//==================================
	float fVaim[4] = {};
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		fVaim[nCntVtx] = atan2f(FourVtxPos[nCntVtx].x - MyPos.x, FourVtxPos[nCntVtx].y - MyPos.y);
	}
	//=============================================================================================================================================================================

	//==================================
	//回転頂点の位置を求める
	//==================================
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		//判定を合わせるため、Rotに-を付ける
		FourVtxRotPos[nCntVtx] = D3DXVECTOR2(sinf(-Rot.z + fVaim[nCntVtx]) * fLength[nCntVtx] + MyPos.x, cosf(-Rot.z + fVaim[nCntVtx]) * fLength[nCntVtx] + MyPos.y);
	}
	//=============================================================================================================================================================================

	//=====================================================
	//二つの四角形のそれぞれの辺の方向を求める
	//=====================================================
	float fNormalizeAim[4] = {};
	fNormalizeAim[0] = sqrtf(powf((FourVtxRotPos[0].x - FourVtxRotPos[1].x), 2) + powf((FourVtxRotPos[0].y - FourVtxRotPos[1].y), 2));
	fNormalizeAim[1] = sqrtf(powf((FourVtxRotPos[1].x - FourVtxRotPos[3].x), 2) + powf((FourVtxRotPos[1].y - FourVtxRotPos[3].y), 2));
	fNormalizeAim[2] = sqrtf(powf((FourComparisonVtxPos[0].x - FourComparisonVtxPos[1].x), 2) + powf((FourComparisonVtxPos[0].y - FourComparisonVtxPos[1].y), 2));
	fNormalizeAim[3] = sqrtf(powf((FourComparisonVtxPos[1].x - FourComparisonVtxPos[3].x), 2) + powf((FourComparisonVtxPos[1].y - FourComparisonVtxPos[3].y), 2));

	//正規化ベクトルを求める
	D3DXVECTOR2 sideDirs[] =
	{
		(FourVtxRotPos[0] - FourVtxRotPos[1]) / fNormalizeAim[0], (FourVtxRotPos[1] - FourVtxRotPos[3]) / fNormalizeAim[1],
		(FourComparisonVtxPos[0] - FourComparisonVtxPos[1]) / fNormalizeAim[2], (FourComparisonVtxPos[1] - FourComparisonVtxPos[3]) / fNormalizeAim[3]
	};
	//==============================================================================================================================================================================


	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		float minA = D3DXVec2Dot(&FourVtxRotPos[0], &sideDirs[nCnt]);
		float maxA = minA;
		float minB = D3DXVec2Dot(&FourComparisonVtxPos[0], &sideDirs[nCnt]);
		float maxB = minB;

		for (int i = 1; i < 4; i++)
		{
			float projA = D3DXVec2Dot(&FourVtxRotPos[i], &sideDirs[nCnt]);

			if (projA < minA) { minA = projA; }
			else if (projA > maxA) { maxA = projA; }

			float projB = D3DXVec2Dot(&FourComparisonVtxPos[i], &sideDirs[nCnt]);

			if (projB < minB) { minB = projB; }
			else if (projB > maxB) { maxB = projB; }
		}

		if (maxB < minA || maxA < minB)
		{
			//return false;
			bCollision = false;
		}
	}

	//==================================================================
	//Zの範囲内にいるかどうか
	//==================================================================
	if (ComparisonPos.z + ComparisonVtxMax.z >= MyPos.z + MyVtxMin.z &&
		ComparisonPos.z + ComparisonVtxMin.z <= MyPos.z + MyVtxMax.z && bCollision == true)
	{
		return true;
	}
	else
	{
	   return false;
	}
	//======================================================================================================
}

//================================================================
//斜めの当たり判定XZ
//================================================================
bool CCollision::RectAngleCollisionXZ(CObjectX* pMyObj, CObjectX* pComObj)
{
	D3DXVECTOR2 FourVtxPos[4] = {};   //４頂点の位置
	D3DXVECTOR2 FourVtxRotPos[4] = {};//回転した４頂点の位置
	D3DXVECTOR2 FourComparisonVtxPos[4] = {};    //比較用４頂点
	bool bCollision = true;
	D3DXVECTOR3 MyPos = pMyObj->GetPos();
	D3DXVECTOR3 MyVtxMin = pMyObj->GetVtxMin();
	D3DXVECTOR3 MyVtxMax = pMyObj->GetVtxMax();
	D3DXVECTOR3 MyRot = pMyObj->GetRot();
	D3DXVECTOR3 ComparisonPos = pComObj->GetPos();
	D3DXVECTOR3 ComparisonVtxMin = pComObj->GetVtxMin();
	D3DXVECTOR3 ComparisonVtxMax = pComObj->GetVtxMax();

	//=====================================
	//比較対象の四角形の４頂点
	//=====================================
	FourComparisonVtxPos[0].x = ComparisonPos.x + ComparisonVtxMin.x;
	FourComparisonVtxPos[0].y = ComparisonPos.z + ComparisonVtxMax.z;

	FourComparisonVtxPos[1].x = ComparisonPos.x + ComparisonVtxMax.x;
	FourComparisonVtxPos[1].y = ComparisonPos.z + ComparisonVtxMax.z;

	FourComparisonVtxPos[2].x = ComparisonPos.x + ComparisonVtxMin.x;
	FourComparisonVtxPos[2].y = ComparisonPos.z + ComparisonVtxMin.z;

	FourComparisonVtxPos[3].x = ComparisonPos.x + ComparisonVtxMax.x;
	FourComparisonVtxPos[3].y = ComparisonPos.z + ComparisonVtxMin.z;

	//=============================================================================================================================================================================

	//=====================================
	//攻撃の四角形の４頂点
	//=====================================

	//左上頂点
	FourVtxPos[0] = D3DXVECTOR2(MyPos.x + MyVtxMin.x, MyPos.z + MyVtxMax.z);

	//右上頂点
	FourVtxPos[1] = D3DXVECTOR2(MyPos.x + MyVtxMax.x, MyPos.z + MyVtxMax.z);

	//左下頂点
	FourVtxPos[2] = D3DXVECTOR2(MyPos.x + MyVtxMin.x, MyPos.z + MyVtxMin.z);

	//右下頂点
	FourVtxPos[3] = D3DXVECTOR2(MyPos.x + MyVtxMax.x, MyPos.z + MyVtxMin.z);

	//=============================================================================================================================================================================

	//==================================
	//中心点と４頂点の距離を求める
	//==================================
	float fLength[4] = {};
	for (int nCnVtx = 0; nCnVtx < 4; nCnVtx++)
	{
		fLength[nCnVtx] = sqrtf(powf((FourVtxPos[nCnVtx].x - MyPos.x), 2) + powf((FourVtxPos[nCnVtx].y - MyPos.z), 2));
	}
	//=============================================================================================================================================================================

	//==================================
	//中心点から４頂点の角度を求める
	//==================================
	float fVaim[4] = {};
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		fVaim[nCntVtx] = atan2f(FourVtxPos[nCntVtx].x - MyPos.x, FourVtxPos[nCntVtx].y - MyPos.z);
	}
	//=============================================================================================================================================================================

	//==================================
	//回転頂点の位置を求める
	//==================================
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		//判定を合わせるため、Rotに-を付ける
		FourVtxRotPos[nCntVtx] = D3DXVECTOR2(sinf(MyRot.y + fVaim[nCntVtx]) * fLength[nCntVtx] + MyPos.x, cosf(MyRot.y + fVaim[nCntVtx]) * fLength[nCntVtx] + MyPos.z);
	}
	//=============================================================================================================================================================================

	//=====================================================
	//二つの四角形のそれぞれの辺の方向を求める
	//=====================================================
	float fNormalizeAim[4] = {};
	fNormalizeAim[0] = sqrtf(powf((FourVtxRotPos[0].x - FourVtxRotPos[1].x), 2) + powf((FourVtxRotPos[0].y - FourVtxRotPos[1].y), 2));
	fNormalizeAim[1] = sqrtf(powf((FourVtxRotPos[1].x - FourVtxRotPos[3].x), 2) + powf((FourVtxRotPos[1].y - FourVtxRotPos[3].y), 2));
	fNormalizeAim[2] = sqrtf(powf((FourComparisonVtxPos[0].x - FourComparisonVtxPos[1].x), 2) + powf((FourComparisonVtxPos[0].y - FourComparisonVtxPos[1].y), 2));
	fNormalizeAim[3] = sqrtf(powf((FourComparisonVtxPos[1].x - FourComparisonVtxPos[3].x), 2) + powf((FourComparisonVtxPos[1].y - FourComparisonVtxPos[3].y), 2));

	//正規化ベクトルを求める
	D3DXVECTOR2 sideDirs[] =
	{
		(FourVtxRotPos[0] - FourVtxRotPos[1]) / fNormalizeAim[0], (FourVtxRotPos[1] - FourVtxRotPos[3]) / fNormalizeAim[1],
		(FourComparisonVtxPos[0] - FourComparisonVtxPos[1]) / fNormalizeAim[2], (FourComparisonVtxPos[1] - FourComparisonVtxPos[3]) / fNormalizeAim[3]
	};
	//==============================================================================================================================================================================


	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		float minA = D3DXVec2Dot(&FourVtxRotPos[0], &sideDirs[nCnt]);
		float maxA = minA;
		float minB = D3DXVec2Dot(&FourComparisonVtxPos[0], &sideDirs[nCnt]);
		float maxB = minB;

		for (int i = 1; i < 4; i++)
		{
			float projA = D3DXVec2Dot(&FourVtxRotPos[i], &sideDirs[nCnt]);

			if (projA < minA) { minA = projA; }
			else if (projA > maxA) { maxA = projA; }

			float projB = D3DXVec2Dot(&FourComparisonVtxPos[i], &sideDirs[nCnt]);

			if (projB < minB) { minB = projB; }
			else if (projB > maxB) { maxB = projB; }
		}

		if (maxB < minA || maxA < minB)
		{
			//return false;
			bCollision = false;
		}
	}

	//==================================================================
	//Zの範囲内にいるかどうか
	//==================================================================
	if (ComparisonPos.y + ComparisonVtxMax.y >= MyPos.y + MyVtxMin.y &&
		ComparisonPos.y + ComparisonVtxMin.y <= MyPos.y + MyVtxMax.y && bCollision == true)
	{
		return true;
	}
	else
	{
		return false;
	}
	//======================================================================================================
}
//====================================================================================================================

//================================================================
//点がAABBの中に入っているかどうか
//================================================================
bool CCollision::IsPointInsideAABB(const D3DXVECTOR3& Point, CObjectX* pComObjX)
{
	if (Point.x >= pComObjX->GetPos().x + pComObjX->GetVtxMin().x &&
		Point.x <= pComObjX->GetPos().x + pComObjX->GetVtxMax().x &&
		Point.y >= pComObjX->GetPos().y + pComObjX->GetVtxMin().y &&
		Point.y <= pComObjX->GetPos().y + pComObjX->GetVtxMax().y &&
		Point.z >= pComObjX->GetPos().z + pComObjX->GetVtxMin().z &&
		Point.z <= pComObjX->GetPos().z + pComObjX->GetVtxMax().z)
	{
		CManager::GetDebugProc()->PrintDebugProc("レイの支点とAABBの当たり判定が無効！位置：%f %f %f\n", pComObjX->GetPos().x, pComObjX->GetPos().y, pComObjX->GetPos().z);
		CManager::GetDebugProc()->PrintDebugProc("最大頂点：%f %f %f\n", pComObjX->GetVtxMax().x, pComObjX->GetVtxMax().y, pComObjX->GetVtxMax().z);
		CManager::GetDebugProc()->PrintDebugProc("最小頂点：%f %f %f\n", pComObjX->GetVtxMin().x, pComObjX->GetVtxMin().y, pComObjX->GetVtxMin().z);
		return true;
	}
	return false;
}
//====================================================================================================================

//================================================================
//正方形の押し出し判定
//================================================================
bool CCollision::ExtrusionCollisionSquare(D3DXVECTOR3& MyPos, bool& bCollisionX, bool& bCollisionY, bool& bCollisionZ,
	const D3DXVECTOR3 MyMove, const D3DXVECTOR3 MyPosOld, const D3DXVECTOR3 MyVtxMax, const D3DXVECTOR3 MyVtxMin,
	const D3DXVECTOR3 ComPos, const D3DXVECTOR3 ComVtxMax, const D3DXVECTOR3 ComVtxMin,
	const bool bCollisionXOld, const bool bCollisionYOld, const bool bCollisionZOld, bool& bIsLanding)
{

	//それぞれのbCollisionがすでにtrueの場合は、値を返さない

	if (bCollisionYOld == true)
	{
		bCollisionY = ExtrusionCollisionSquareY(MyPos, MyMove, MyPosOld,
			MyVtxMax, MyVtxMin, ComPos,
			ComVtxMax, ComVtxMin, bCollisionY, bIsLanding);

		if (bCollisionXOld == true)
		{
			bCollisionX = ExtrusionCollisionSquareX(MyPos, MyMove, MyPosOld,
				MyVtxMax, MyVtxMin, ComPos,
				ComVtxMax, ComVtxMin, bCollisionX);

			bCollisionZ = ExtrusionCollisionSquareZ(MyPos, MyMove, MyPosOld,
				MyVtxMax, MyVtxMin, ComPos,
				ComVtxMax, ComVtxMin, bCollisionZ);
		}
		else
		{
			bCollisionZ = ExtrusionCollisionSquareZ(MyPos, MyMove, MyPosOld,
				MyVtxMax, MyVtxMin, ComPos,
				ComVtxMax, ComVtxMin, bCollisionZ);

			bCollisionX = ExtrusionCollisionSquareX(MyPos, MyMove, MyPosOld,
				MyVtxMax, MyVtxMin, ComPos,
				ComVtxMax, ComVtxMin, bCollisionX);
		}
	}
	else if (bCollisionXOld == true)
	{
		bCollisionX = ExtrusionCollisionSquareX(MyPos, MyMove, MyPosOld,
			MyVtxMax, MyVtxMin, ComPos,
			ComVtxMax, ComVtxMin, bCollisionX);

		bCollisionZ = ExtrusionCollisionSquareZ(MyPos, MyMove, MyPosOld,
			MyVtxMax, MyVtxMin, ComPos,
			ComVtxMax, ComVtxMin, bCollisionZ);

		bCollisionY = ExtrusionCollisionSquareY(MyPos, MyMove, MyPosOld,
			MyVtxMax, MyVtxMin, ComPos,
			ComVtxMax, ComVtxMin, bCollisionY, bIsLanding);
	}
	else if (bCollisionZOld == true)
	{
		bCollisionZ = ExtrusionCollisionSquareZ(MyPos, MyMove, MyPosOld,
			MyVtxMax, MyVtxMin, ComPos,
			ComVtxMax, ComVtxMin, bCollisionZ);

		bCollisionX = ExtrusionCollisionSquareX(MyPos, MyMove, MyPosOld,
			MyVtxMax, MyVtxMin, ComPos,
			ComVtxMax, ComVtxMin, bCollisionX);

		bCollisionY = ExtrusionCollisionSquareY(MyPos, MyMove, MyPosOld,
			MyVtxMax, MyVtxMin, ComPos,
			ComVtxMax, ComVtxMin, bCollisionY, bIsLanding);

	}
	else
	{
		bCollisionZ = ExtrusionCollisionSquareZ(MyPos, MyMove, MyPosOld,
			MyVtxMax, MyVtxMin, ComPos,
			ComVtxMax, ComVtxMin, bCollisionZ);

		bCollisionX = ExtrusionCollisionSquareX(MyPos, MyMove, MyPosOld,
			MyVtxMax, MyVtxMin, ComPos,
			ComVtxMax, ComVtxMin, bCollisionX);

		bCollisionY = ExtrusionCollisionSquareY(MyPos, MyMove, MyPosOld,
			MyVtxMax, MyVtxMin, ComPos,
			ComVtxMax, ComVtxMin, bCollisionY, bIsLanding);
	}

	if (bCollisionX == true || bCollisionY == true || bCollisionZ == true)
	{
		return true;
	}
	return false;
}
//====================================================================================================================

//================================================================
//正方形の押し出し判定X
//================================================================
bool CCollision::ExtrusionCollisionSquareX(D3DXVECTOR3& MyPos, const D3DXVECTOR3 MyMove, const D3DXVECTOR3 MyPosOld, const D3DXVECTOR3 MyVtxMax, const D3DXVECTOR3 MyVtxMin,
	const D3DXVECTOR3 ComPos, const D3DXVECTOR3 ComVtxMax, const D3DXVECTOR3 ComVtxMin, const bool bCollisionX)
{
	if (MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPosOld.x + MyVtxMax.x - MyMove.x <= ComPos.x + ComVtxMin.x
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象の右端がブロックの左端に当たった時の処理
		MyPos.x = ComPos.x + ComVtxMin.x - MyVtxMax.x - 0.1f;
		return true;
	}
	else if (MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPosOld.x + MyVtxMin.x - MyMove.x >= ComPos.x + ComVtxMax.x
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象の左端がブロックの右端に当たった時の処理
		MyPos.x = ComPos.x + ComVtxMax.x - MyVtxMin.x + 0.1f;
		return true;
	}
	return bCollisionX;
}
//====================================================================================================================

//================================================================
//正方形の押し出し判定Y
//================================================================
bool CCollision::ExtrusionCollisionSquareY(D3DXVECTOR3& MyPos, const D3DXVECTOR3 MyMove, const D3DXVECTOR3 MyPosOld, const D3DXVECTOR3 MyVtxMax, const D3DXVECTOR3 MyVtxMin,
	const D3DXVECTOR3 ComPos, const D3DXVECTOR3 ComVtxMax, const D3DXVECTOR3 ComVtxMin, const bool bCollisionY, bool& bIsLanding)
{
	//上
	if (MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPosOld.y + MyVtxMin.y - MyMove.y >= ComPos.y + ComVtxMax.y
		&& MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象の下端がモデルの上端に当たった時の処理
		float fPosY = fabsf(MyVtxMin.y);
		MyPos.y = ComPos.y + ComVtxMax.y + fPosY + 0.1f;
		bIsLanding = true;
		return true;
	}
	//下
	else if (MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPosOld.y + MyVtxMax.y <= ComPos.y + ComVtxMin.y
		&& MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象の上端がモデルの下端に当たった時の処理
		MyPos.y = ComPos.y + ComVtxMin.y - MyVtxMax.y;
		return true;
	}

	return bCollisionY;
}
//====================================================================================================================

//================================================================
//正方形の押し出し判定Z
//================================================================
bool CCollision::ExtrusionCollisionSquareZ(D3DXVECTOR3& MyPos, const D3DXVECTOR3 MyMove, const D3DXVECTOR3 MyPosOld, const D3DXVECTOR3 MyVtxMax, const D3DXVECTOR3 MyVtxMin,
	const D3DXVECTOR3 ComPos, const D3DXVECTOR3 ComVtxMax, const D3DXVECTOR3 ComVtxMin, const bool bCollisionZ)
{
	if (MyPos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& MyPosOld.z + MyVtxMax.z - MyMove.z <= ComPos.z + ComVtxMin.z
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x)
	{//対象の右端がブロックの左端に当たった時の処理
		MyPos.z = ComPos.z + ComVtxMin.z - MyVtxMax.z - 0.1f;
		return true;
	}
	else if (MyPos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z
		&& MyPosOld.z + MyVtxMin.z - MyMove.z >= ComPos.z + ComVtxMax.z
		&& MyPos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& MyPos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& MyPos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& MyPos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x)
	{//対象の左端がブロックの右端に当たった時の処理
		MyPos.z = ComPos.z + ComVtxMax.z - MyVtxMin.z + 0.1f;
		return true;
	}
	return bCollisionZ;
}
//====================================================================================================================

//================================================================
//レイとAABBの当たり判定
//================================================================
bool CCollision::RayIntersectsAABB(const D3DXVECTOR3& rayOrigin, const D3DXVECTOR3& rayDir, const D3DXVECTOR3& boxMin, const D3DXVECTOR3& boxMax)
{
	float tMin = 0.0f;
	float tMax = std::numeric_limits<float>::infinity();

	for (int i = 0; i < 3; i++) {
		if (rayDir[i] != 0) {
			float t1 = (boxMin[i] - rayOrigin[i]) / rayDir[i];
			float t2 = (boxMax[i] - rayOrigin[i]) / rayDir[i];
			if (t1 > t2) std::swap(t1, t2);

			tMin = (std::max)(tMin, t1);
			tMax = (std::min)(tMax, t2);

			if (tMin > tMax) {
				return false; // No intersection
			}
		}
		else if (rayOrigin[i] < boxMin[i] || rayOrigin[i] > boxMax[i]) {
			return false; // No intersection if the ray is parallel and outside the box
		}
	}

	return true; // The ray intersects the AABB}
}
//====================================================================================================================

//================================================================
//レイとAABBの当たり判定、判定箇所も求める
//================================================================
bool CCollision::RayIntersectsAABBCollisionPos(const D3DXVECTOR3& origin,const D3DXVECTOR3& direction, const D3DXVECTOR3& min, const D3DXVECTOR3& max,
	D3DXVECTOR3& CollisionPos)
{
	D3DXVECTOR3 CopyDirection = direction;

	if (origin.x >= min.x && origin.x <= max.x &&
		origin.y >= min.y && origin.y <= max.y &&
		origin.z >= min.z && origin.z <= max.z)
	{//レイの起点がボックスの内側にある場合
		return false;
	}

	if (CopyDirection.x == 0.0f)
	{
		CopyDirection.x = 0.001f;//０除算を回避
	}
	if (CopyDirection.y == 0.0f)
	{
		CopyDirection.y = 0.001f;//０除算を回避
	}
	if (CopyDirection.z == 0.0f)
	{
		CopyDirection.z = 0.001f;//０除算を回避
	}

	//＊それぞれの軸の距離を求め、minやmaxにその単位ベクトルの方向なら、どれくらいの距離を進めば到達できるかを求める。
	float tmin = (min.x - origin.x) / CopyDirection.x;
	float tmax = (max.x - origin.x) / CopyDirection.x;

	float t = 0.0f;

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (min.y - origin.y) / CopyDirection.y;
	float tymax = (max.y - origin.y) / CopyDirection.y;

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax)) return false;

	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	float tzmin = (min.z - origin.z) / CopyDirection.z;
	float tzmax = (max.z - origin.z) / CopyDirection.z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax)) return false;

	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;

	t = tmin;

	if (t < 0.0f) return false;

	//衝突したことが確定したので、衝突位置を求める（tには、レイがAABBとの衝突点の最小距離が入っている）
	CollisionPos = D3DXVECTOR3(origin.x + CopyDirection.x * t,
		origin.y + CopyDirection.y * t,
		origin.z + CopyDirection.z * t);
	return true;
}
//====================================================================================================================