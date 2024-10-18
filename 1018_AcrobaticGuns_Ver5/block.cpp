//==========================================================================================================
//
//７月１２日：全ギミックを実装する[block.cpp]
//Author:ShinaTaiyo
//
//==========================================================================================================

//=========================
//インクルード
//=========================
#include "block.h"
#include "manager.h"
#include "objectXInfo.h"
#include "calculation.h"
#include "stagemanager.h"
#include "player.h"
#include "game.h"
#include "edit.h"
#include "input.h"
#include "debugproc.h"
#include <stdio.h>
#include <string.h>
//=================================================

//=========================
//静的メンバ
//=========================
int CBlock::m_nNumFile = CBlock::BLOCKTYPE_MAX;                                              //ファイル数を格納する
const float CBlock::m_fBLOCKCORRECTIONCOLLISION = 3.0f;      //1,2,3,4,5,6,7,8,9,10,11,12,13 
const char* CBlock::m_BLOCK_FILENAME[CBlock::BLOCKTYPE_MAX] =
{ "data\\MODEL\\Block\\Block00_Normal.x", 
};                                                  //ブロックのXファイルへのポインタ
//========================================================

//=========================
//コンストラクタ
//=========================
CBlock::CBlock() : CObjectX(2),m_bCollision(false),m_type(BLOCKTYPE00_NORMAL)
{

}
//=================================================

//=========================
//デストラクタ
//=========================
CBlock::~CBlock()
{

}
//=================================================

//=========================
//初期化処理
//=========================
HRESULT CBlock::Init()
{
	//===========================
	//Xオブジェクト初期化
	//===========================
	CObjectX::Init();
	//=========================================
	m_type = BLOCKTYPE00_NORMAL;   //ブロックの種類
	m_bCollision = true;           //当たり判定をするかどうか
	return S_OK;
}
//=================================================

//=========================
//終了処理
//=========================
void CBlock::Uninit()
{
	CObjectX::Uninit();//Xオブジェクト終了処理
}
//=================================================

//=========================
//更新処理
//=========================
void CBlock::Update()
{
	GravityProcess();

	//========================================
    //Xオブジェクトの更新処理
    //========================================
	CObjectX::Update();
	//===========================================================================================

	Collision();//接触判定を行う
}
//=================================================

//===================================================
//描画処理
//===================================================
void CBlock::Draw()
{
	CObjectX::Draw();
}
//===========================================================================================

//===================================================
//死亡フラグを設定
//===================================================
void CBlock::SetDeath()
{
	CObject::SetDeath();
}
//===========================================================================================

//==================================
//ブロック生成処理
//==================================
CBlock* CBlock::Create(BLOCKTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	CBlock* pBlock = DBG_NEW CBlock;                                                               //ブロックを生成
	bool bSuccess = pBlock->CObject::GetCreateSuccess();
	int nIdx = 0;//モデル情報のインデックス
	if (bSuccess == true)
	{
		if (pBlock != nullptr)
		{
			pBlock->Init();                               //初期化処理
			pBlock->SetUseGravity(false,1.0f);            //重力を設定する
			pBlock->SetRot(rot);                          //向きを設定
			pBlock->SetUseDeath(true);                    //死亡フラグを発動するかどうかを設定する
			pBlock->m_type = type;                        //ブロックの種類
			pBlock->SetLife(nLife);                       //ブロックの体力
			pBlock->SetMaxLife(nLife);                    //ブロックの体力
			pBlock->SetAutoSubLife(false);                //自動的に体力を減らすかどうか
			pBlock->SetSupportPos(pos);                   //設置位置
			pBlock->CObjectX::SetPos(pos);                //オブジェクトXの位置を設定
			pBlock->SetPosOld(pos);                       //1f前の位置を設定
			pBlock->CObject::SetType(CObject::TYPE_BLOCK);//オブジェクトの種類を決める
			pBlock->SetScale(scale);                      //オブジェクトの拡大率
			pBlock->SetFormarScale(scale);                //元の拡大率を設定する
			nIdx = CManager::GetObjectXInfo()->Regist(m_BLOCK_FILENAME[(int)(type)]);
			pBlock->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
				CManager::GetObjectXInfo()->GetTexture(nIdx),
				CManager::GetObjectXInfo()->GetColorValue(nIdx));                     //モデル情報を割り当てる

			pBlock->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BLOCK);               //オブジェクトXのタイプを設定
			pBlock->CObjectX::SetTypeNum((int)(type));                                //オブジェクトXごとのタイプ番号を設定
			pBlock->SetSize();                                                        //Xオブジェクトのサイズを設定する
			pBlock->SetManagerType((int)(CStageManager::MANAGEROBJECT_BLOCK));        //マネージャーで呼び出す時の種類を設定

		}
	}
	else
	{//オブジェクトに空きがなかったので破棄する
		delete pBlock;
		pBlock = nullptr;
		return nullptr;
	}
	return pBlock;
}
//================================================================================================

//===============================================================
//ブロックを消す処理
//===============================================================
void CBlock::ReleaseBlock()
{
	Release();
}
//=========================================================================================================================

//===============================================================
//ブロックのタイプを取得する
//===============================================================
CBlock::BLOCKTYPE CBlock::GetType()
{
	return m_type;
}
//=========================================================================================================================

//===============================================================
//ブロックとの当たり判定を行う
//===============================================================
void CBlock::Collision()
{
	//==================================
	//オブジェクトXから情報を取得
	//==================================
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //判定対象の位置を取得
	D3DXVECTOR3 ComparisonPosOld = NULL_VECTOR3;               //判定対象の1f前の位置を取得
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //判定対象の最大頂点を取得
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //判定対象の最小頂点を取得
	D3DXVECTOR3 MySize = NULL_VECTOR3;
	D3DXVECTOR3 BlockPos = GetPos();                          //ブロックの位置を取得
	D3DXVECTOR3 BlockVtxMax = GetVtxMax();                    //ブロックの最大頂点を取得
	D3DXVECTOR3 BlockVtxMin = GetVtxMin();                    //ブロックの最小頂点を取得
	//===========================================================

	//オブジェクト取得
	CObject* pObj = nullptr;
	//==================================
	//ブロックとの当たり判定
	//==================================
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
		while (pObj != nullptr)
		{
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextObject();

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();

			//オブジェクトの種類が敵だったら
			if (type == CObject::TYPE_PLAYER || type == CObject::TYPE_ENEMY || type == CObject::TYPE_BOSS)
			{
				ComparisonPos.x = (float)(floor(((CObjectX*)pObj)->GetPos().x));                   //位置を取得
				ComparisonPos.y = (float)(floor(((CObjectX*)pObj)->GetPos().y));                   //位置を取得
				ComparisonPos.z = (float)(floor(((CObjectX*)pObj)->GetPos().z));                   //位置を取得]

				ComparisonVtxMax.x = (float)(floor(((CObjectX*)pObj)->GetVtxMax().x));             //最大頂点を取得
				ComparisonVtxMax.y = (float)(floor(((CObjectX*)pObj)->GetVtxMax().y));             //最大頂点を取得
				ComparisonVtxMax.z = (float)(floor(((CObjectX*)pObj)->GetVtxMax().z));             //最大頂点を取得

				ComparisonVtxMin.x = (float)(floor(((CObjectX*)pObj)->GetVtxMin().x));             //最小頂点を取得
				ComparisonVtxMin.y = (float)(floor(((CObjectX*)pObj)->GetVtxMin().y));             //最小頂点を取得
				ComparisonVtxMin.z = (float)(floor(((CObjectX*)pObj)->GetVtxMin().z));             //最小頂点を取得

				ComparisonPosOld.x = (float)(floor(((CObjectX*)pObj)->GetPosOld().x));             //1f前の位置を取得
				ComparisonPosOld.y = (float)(floor(((CObjectX*)pObj)->GetPosOld().y));             //1f前の位置を取得
				ComparisonPosOld.z = (float)(floor(((CObjectX*)pObj)->GetPosOld().z));             //1f前の位置を取得

				MySize.x = (float)(floor(((CObjectX*)pObj)->GetSize().x));
				MySize.y = (float)(floor(((CObjectX*)pObj)->GetSize().y));
				MySize.z = (float)(floor(((CObjectX*)pObj)->GetSize().z));
				//((CObjectX*)pObj)->SetLiftLanding(false);                     //リフトに乗っていない状態に初期化
				//上
				if (ComparisonPos.x + ComparisonVtxMax.x > BlockPos.x + BlockVtxMin.x
					&& ComparisonPos.x + ComparisonVtxMin.x < BlockPos.x + BlockVtxMax.x
					&& ComparisonPos.y + ComparisonVtxMin.y - m_fBLOCKCORRECTIONCOLLISION < BlockPos.y + BlockVtxMax.y
					&& ComparisonPosOld.y + ComparisonVtxMin.y + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.y + BlockVtxMax.y
					&& ComparisonPos.z + ComparisonVtxMax.z > BlockPos.z + BlockVtxMin.z
					&& ComparisonPos.z + ComparisonVtxMin.z < BlockPos.z + BlockVtxMax.z)
				{//対象の下端がモデルの+Yに当たった時の処理

				}
				if (ComparisonPos.x + ComparisonVtxMin.x - m_fBLOCKCORRECTIONCOLLISION <= BlockPos.x + BlockVtxMax.x &&
					ComparisonPos.x + ComparisonVtxMax.x + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.x + BlockVtxMin.x &&
					ComparisonPos.y + ComparisonVtxMin.y <= BlockPos.y + BlockVtxMax.y &&
					ComparisonPos.y + ComparisonVtxMax.y >= BlockPos.y + BlockVtxMin.y &&
					ComparisonPos.z + ComparisonVtxMin.z - m_fBLOCKCORRECTIONCOLLISION <= BlockPos.z + BlockVtxMax.z &&
					ComparisonPos.z + ComparisonVtxMax.z + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.z + BlockVtxMin.z)
				{//ブロックに当たったら

				}
			}
			//リストを次に進める
			pObj = pNext;
		}
	}

}
//=========================================================================================================================

//===============================================================
//ブロックとの当たり判定を行う
//===============================================================
void CBlock::CollisionSquare(CObjectX* pObjX)
{
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;        //比較用位置
	D3DXVECTOR3 ComparisonPosOld = NULL_VECTOR3;     //比較用1f前の位置
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;     //比較用最高頂点
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;     //比較用最低頂点
	D3DXVECTOR3 ComparisonSize = NULL_VECTOR3;       //比較サイズ
	D3DXVECTOR3 ComparisonMove = NULL_VECTOR3;       //比較移動量
	//==========================================================================================================
	bool& bIsLanding = pObjX->GetLanding();
	bool bIsLandingOld = pObjX->GetLanding();//1f前の着地状態
	bIsLanding = false;
	bool& bIsWalling = pObjX->GetWalling();          //壁にくっついているかどうか
	bIsWalling = false;

	bool bCollision = true;//当たり判定をするかどうか

	D3DXVECTOR3 Scale = NULL_VECTOR3;

	//====================================================================
	//判定開始
	//====================================================================
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		CObject * pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
		while (pObj != nullptr)
		{
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextObject();

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();
			if (type == CObject::TYPE_BLOCK)
			{
				CBlock* pBlock = (CBlock*)pObj;

				float fLength = CCalculation::CalculationLength(pObjX->GetPos(), pBlock->GetPos());
				Scale = pBlock->GetScale();
				if (Scale.x >= Scale.y - 0.1f && Scale.x <= Scale.y + 0.1f)
				{//拡大率がほとんど同じなら
					if (fLength > pBlock->GetSize().x * 3.0f)
					{
						bCollision = false;
					}
					else
					{
						bCollision = true;
					}
				}
				else
				{
					bCollision = true;
				}

				if (pBlock->m_bCollision == true && bCollision == true)
				{
					if (bIsLandingOld == true)
					{
						//上下
						pBlock->ExtrusionCollisionY(pObjX, pBlock);

						//左右
						pBlock->ExtrusionCollisionX(pObjX, pBlock);
						
					}
					else
					{
						//左右
						pBlock->ExtrusionCollisionX(pObjX, pBlock);

						//上下
						pBlock->ExtrusionCollisionY(pObjX, pBlock);
					}
				}
			}
			//リストを次に進める
			pObj = pNext;
		}
	}
}
//=========================================================================================================================

//===============================================================
//指定下ブロックの上に乗っている場合、位置を補正する
//===============================================================
void CBlock::LandingCorrection(D3DXVECTOR3& Pos, CObject* pSaveObj,D3DXVECTOR3 VtxMin)
{
	CObject::TYPE type = (CObject::TYPE)(0);
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;        //比較用位置
	D3DXVECTOR3 ComparisonPosOld = NULL_VECTOR3;     //比較用1f前の位置
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;     //比較用最高頂点
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;     //比較用最低頂点
	D3DXVECTOR3 ComparisonSize = NULL_VECTOR3;       //サイズ
	D3DXVECTOR3 ComparisonMove = NULL_VECTOR3;

	D3DXVECTOR3 MyVtxMin = NULL_VECTOR3;
	MyVtxMin.x = (float)(floor(VtxMin.x));
	MyVtxMin.y = (float)(floor(VtxMin.y));
	MyVtxMin.z = (float)(floor(VtxMin.z));

	if (pSaveObj != nullptr)
	{
		//種類の取得
		type = pSaveObj->GetType();
		if (type == CObject::TYPE_BLOCK)
		{
			CBlock* pBlock = (CBlock*)pSaveObj;

			ComparisonPos.x = (float)(floor(pBlock->GetPos().x));
			ComparisonPos.y = (float)(floor(pBlock->GetPos().y));
			ComparisonPos.z = (float)(floor(pBlock->GetPos().z));

			ComparisonPosOld.x = (float)(floor(pBlock->GetPosOld().x));
			ComparisonPosOld.y = (float)(floor(pBlock->GetPosOld().y));
			ComparisonPosOld.z = (float)(floor(pBlock->GetPosOld().z));

			ComparisonVtxMax.x = (float)(floor(pBlock->GetVtxMax().x));
			ComparisonVtxMax.y = (float)(floor(pBlock->GetVtxMax().y));
			ComparisonVtxMax.z = (float)(floor(pBlock->GetVtxMax().z));

			ComparisonVtxMin.x = (float)(floor(pBlock->GetVtxMin().x));
			ComparisonVtxMin.y = (float)(floor(pBlock->GetVtxMin().y));
			ComparisonVtxMin.z = (float)(floor(pBlock->GetVtxMin().z));

			ComparisonSize.x = (float)(floor(pBlock->GetSize().x));
			ComparisonSize.y = (float)(floor(pBlock->GetSize().y));
			ComparisonSize.z = (float)(floor(pBlock->GetSize().z));
			 

			float fPosY = fabsf(MyVtxMin.y);
			Pos.y = ComparisonPos.y + ComparisonVtxMax.y + fPosY;
		}
	}

}
//=========================================================================================================================

//=======================================================================
//X方向の押し出し判定を行う
//=======================================================================
void CBlock::ExtrusionCollisionX(CObjectX* pMyObjX, CBlock* pBlock)
{
	D3DXVECTOR3 MyPos = pMyObjX->GetPos();
	D3DXVECTOR3& Pos = pMyObjX->GetPos();              //位置を取得
	D3DXVECTOR3& PosOld = pMyObjX->GetPosOld();        //1f前の位置を取得
	D3DXVECTOR3 MyVtxMax = NULL_VECTOR3;             //自分自身の最大頂点
	D3DXVECTOR3 MyVtxMin = NULL_VECTOR3;             //自分自身の最小頂点

	D3DXVECTOR3 ComPos = pBlock->GetPos();             //ブロックの位置
	D3DXVECTOR3 ComVtxMax = pBlock->GetVtxMax();       //ブロックの最大頂点
	D3DXVECTOR3 ComVtxMin = pBlock->GetVtxMin();       //ブロックの最小頂点
	D3DXVECTOR3 ComMove = pBlock->GetMove();           //ブロックの最小頂点

	bool& bIsWalling = pMyObjX->GetWalling();          //壁にくっついているかどうか
	//押し出しに使う値の小数点を切り捨てる（微小な差で失敗しないため）
	MyVtxMax.x = (float)(floor(pMyObjX->GetVtxMax().x));
	MyVtxMax.y = (float)(floor(pMyObjX->GetVtxMax().y));
	MyVtxMax.z = (float)(floor(pMyObjX->GetVtxMax().z));

	MyVtxMin.x = (float)(floor(pMyObjX->GetVtxMin().x));
	MyVtxMin.y = (float)(floor(pMyObjX->GetVtxMin().y));
	MyVtxMin.z = (float)(floor(pMyObjX->GetVtxMin().z));

	ComPos.x = float(floor(pBlock->GetPos().x));
	ComPos.y = float(floor(pBlock->GetPos().y));
	ComPos.z = float(floor(pBlock->GetPos().z));

	ComVtxMax.x = (float)(floor(ComVtxMax.x));
	ComVtxMax.y = (float)(floor(ComVtxMax.y));
	ComVtxMax.z = (float)(floor(ComVtxMax.z));

	ComVtxMin.x = (float)(floor(ComVtxMin.x));
	ComVtxMin.y = (float)(floor(ComVtxMin.y));
	ComVtxMin.z = (float)(floor(ComVtxMin.z));
	// 					//左
	if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& PosOld.x + MyVtxMax.x <= ComPos.x + ComVtxMin.x
		&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象がブロックの-Xに当たった時の処理
		Pos.x = ComPos.x + ComVtxMin.x - MyVtxMax.x;
		bIsWalling = true;
	}

	//右
	else if (Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& PosOld.x + MyVtxMin.x >= ComPos.x + ComVtxMax.x
		&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象がブロックの+Xに当たった時の処理
		Pos.x = ComPos.x + ComVtxMax.x - MyVtxMin.x;
		bIsWalling = true;
	}
}
//======================================================================================================================================

//=======================================================================
//Y方向の押し出し判定を行う
//=======================================================================
void CBlock::ExtrusionCollisionY(CObjectX* pMyObjX, CBlock* pBlock)
{
	D3DXVECTOR3 MyVtxMax = NULL_VECTOR3;             //自分自身の最大頂点
	D3DXVECTOR3 MyVtxMin = NULL_VECTOR3;             //自分自身の最小頂点

	D3DXVECTOR3 ComPos = pBlock->GetPos();             //ブロックの位置
	D3DXVECTOR3 ComVtxMax = pBlock->GetVtxMax();       //ブロックの最大頂点
	D3DXVECTOR3 ComVtxMin = pBlock->GetVtxMin();       //ブロックの最小頂点[
	D3DXVECTOR3 ComMove = pBlock->GetMove();           //ブロックの最小頂点[

	D3DXVECTOR3& Pos = pMyObjX->GetPos();              //位置を取得
	D3DXVECTOR3& Move = pMyObjX->GetMove();             //移動量を取得
	D3DXVECTOR3 PosOld = pMyObjX->GetPosOld();        //1f前の位置を取得

	bool& bIsLanding = pMyObjX->GetLanding();          //地面にいるかどうか

	bool& bIsJumping = pMyObjX->GetJumping();          //ジャンプしているかどうか

	//押し出し失敗対策
	MyVtxMax.x = (float)(floor(pMyObjX->GetVtxMax().x));
	MyVtxMax.y = (float)(floor(pMyObjX->GetVtxMax().y));
	MyVtxMax.z = (float)(floor(pMyObjX->GetVtxMax().z));

	MyVtxMin.x = (float)(floor(pMyObjX->GetVtxMin().x));
	MyVtxMin.y = (float)(floor(pMyObjX->GetVtxMin().y));
	MyVtxMin.z = (float)(floor(pMyObjX->GetVtxMin().z));

	ComPos.x = float(floor(pBlock->GetPos().x));
	ComPos.y = float(floor(pBlock->GetPos().y));
	ComPos.z = float(floor(pBlock->GetPos().z));

	ComVtxMax.x = (float)(floor(ComVtxMax.x));
	ComVtxMax.y = (float)(floor(ComVtxMax.y));
	ComVtxMax.z = (float)(floor(ComVtxMax.z));

	ComVtxMin.x = (float)(floor(ComVtxMin.x));
	ComVtxMin.y = (float)(floor(ComVtxMin.y));
	ComVtxMin.z = (float)(floor(ComVtxMin.z));

	//上
	if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& PosOld.y + MyVtxMin.y - Move.y >= ComPos.y + ComVtxMax.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象の下端がモデルの+Yに当たった時の処理
		float fPosY = fabsf(MyVtxMin.y);
		Pos.y = ComPos.y + ComVtxMax.y + fPosY;
		Pos += ComMove;

		bIsLanding = true;
	}
	//下
	else if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& PosOld.y + MyVtxMax.y <= ComPos.y + ComVtxMin.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象の下端がモデルの+Yに当たった時の処理
		Pos.y = ComPos.y + ComVtxMin.y - MyVtxMax.y;
		bIsJumping = false;

		if (pMyObjX->GetType() == CObject::TYPE_PLAYER)
		{
			Move.y = 0.0f;
		}
	}
}
//======================================================================================================================================