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
int CBlock::m_nNumFile = static_cast<int>(CBlock::BLOCKTYPE::MAX);                                              //ファイル数を格納する
const float CBlock::m_fBLOCKCORRECTIONCOLLISION = 3.0f;      //1,2,3,4,5,6,7,8,9,10,11,12,13 
const char* CBlock::m_BLOCK_FILENAME[static_cast<int>(CBlock::BLOCKTYPE::MAX)] =
{ "data\\MODEL\\Block\\Block00_Normal.x",
  "data\\MODEL\\Block\\Block01_Water.x", 
  "data\\MODEL\\Block\\Renga_000.x", 
};                                                  //ブロックのXファイルへのポインタ
//========================================================

//=========================
//コンストラクタ
//=========================
CBlock::CBlock(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectXAlive(nPri,bUseintPri,type,ObjType),
m_bCollision(false),m_type(BLOCKTYPE::NORMAL)
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
	CObjectXAlive::Init();
	//=========================================
	m_type = BLOCKTYPE::NORMAL;   //ブロックの種類
	m_bCollision = true;           //当たり判定をするかどうか
	return S_OK;
}
//=================================================

//=========================
//終了処理
//=========================
void CBlock::Uninit()
{
	CObjectXAlive::Uninit();//Xオブジェクト終了処理
}
//=================================================

//=========================
//更新処理
//=========================
void CBlock::Update()
{
	//========================================
    //Xオブジェクトの更新処理
    //========================================
	CObjectXAlive::Update();
	//===========================================================================================

	Collision();//接触判定を行う
}
//=================================================

//===================================================
//描画処理
//===================================================
void CBlock::Draw()
{
	CObjectXAlive::Draw();
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
CBlock* CBlock::Create(BLOCKTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, bool bSwapVtxXZ)
{
	CBlock* pBlock = DBG_NEW CBlock;                                                               //ブロックを生成
	bool bSuccess = pBlock->CObject::GetCreateSuccess();
	int nIdx = 0;//モデル情報のインデックス
	if (bSuccess == true)
	{
		if (pBlock != nullptr)
		{
			pBlock->Init();                               //初期化処理
			pBlock->SetRot(rot);                          //向きを設定
			pBlock->SetUseSwapVtxXZ(bSwapVtxXZ);          //頂点XZを入れ替える
			pBlock->SetUseDeath(true);                    //死亡フラグを発動するかどうかを設定する
			pBlock->m_type = type;                        //ブロックの種類
			pBlock->SetLife(nLife);                       //ブロックの体力
			pBlock->SetMaxLife(nLife);                    //ブロックの体力
			pBlock->SetAutoSubLife(false);                //自動的に体力を減らすかどうか
			pBlock->SetSupportPos(pos);                   //設置位置
			pBlock->CObjectX::SetPos(pos);                //オブジェクトXの位置を設定
			pBlock->SetPosOld(pos);                       //1f前の位置を設定
			pBlock->CObject::SetType(CObject::TYPE::BLOCK);//オブジェクトの種類を決める
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
			pBlock->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::BLOCK);           //マネージャーで呼び出す時の種類を設定
			pBlock->ActiveSwapVtxMaxMin();
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
	D3DXVECTOR3 ComparisonPos = D3DXVECTOR3(0.0f,0.0f,0.0f);                  //判定対象の位置を取得
	D3DXVECTOR3 ComparisonPosOld = D3DXVECTOR3(0.0f,0.0f,0.0f);               //判定対象の1f前の位置を取得
	D3DXVECTOR3 ComparisonVtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f);               //判定対象の最大頂点を取得
	D3DXVECTOR3 ComparisonVtxMin = D3DXVECTOR3(0.0f,0.0f,0.0f);               //判定対象の最小頂点を取得
	D3DXVECTOR3 MySize = D3DXVECTOR3(0.0f,0.0f,0.0f);
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
			if (type == CObject::TYPE::PLAYER || type == CObject::TYPE::ENEMY)
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
void CBlock::CollisionSquare(CObjectXAlive* pObjX)
{
	//D3DXVECTOR3 ComparisonPos = D3DXVECTOR3(0.0f,0.0f,0.0f);        //比較用位置
	//D3DXVECTOR3 ComparisonPosOld = D3DXVECTOR3(0.0f,0.0f,0.0f);     //比較用1f前の位置
	//D3DXVECTOR3 ComparisonVtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f);     //比較用最高頂点
	//D3DXVECTOR3 ComparisonVtxMin = D3DXVECTOR3(0.0f,0.0f,0.0f);     //比較用最低頂点
	//D3DXVECTOR3 ComparisonSize = D3DXVECTOR3(0.0f,0.0f,0.0f);       //比較サイズ
	//D3DXVECTOR3 ComparisonMove = D3DXVECTOR3(0.0f,0.0f,0.0f);       //比較移動量
	////==========================================================================================================
	//bool& bIsLanding = pObjX->GetLanding();
	//bool bIsLandingOld = pObjX->GetLanding();//1f前の着地状態
	//bIsLanding = false;
	//bool& bIsWalling = pObjX->GetWalling();          //壁にくっついているかどうか
	//bIsWalling = false;

	//bool bCollision = true;//当たり判定をするかどうか

	//D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f,0.0f,0.0f);

	////====================================================================
	////判定開始
	////====================================================================
	//for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	//{
	//	CObject * pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
	//	while (pObj != nullptr)
	//	{
	//		//次のオブジェクトを格納
	//		CObject* pNext = pObj->GetNextObject();

	//		//種類の取得（敵なら当たり判定）
	//		CObject::TYPE type = pObj->GetType();
	//		if (type == CObject::TYPE_BLOCK)
	//		{
	//			CBlock* pBlock = (CBlock*)pObj;

	//			float fLength = CCalculation::CalculationLength(pObjX->GetPos(), pBlock->GetPos());
	//			Scale = pBlock->GetScale();
	//			if (Scale.x >= Scale.y - 0.1f && Scale.x <= Scale.y + 0.1f)
	//			{//拡大率がほとんど同じなら
	//				if (fLength > pBlock->GetSize().x * 3.0f)
	//				{
	//					bCollision = false;
	//				}
	//				else
	//				{
	//					bCollision = true;
	//				}
	//			}
	//			else
	//			{
	//				bCollision = true;
	//			}

	//			if (pBlock->m_bCollision == true && bCollision == true)
	//			{
	//				if (bIsLandingOld == true)
	//				{
	//					//上下
	//					pBlock->ExtrusionCollisionY(pObjX, pBlock);

	//					//左右
	//					pBlock->ExtrusionCollisionX(pObjX, pBlock);
	//					
	//				}
	//				else
	//				{
	//					//左右
	//					pBlock->ExtrusionCollisionX(pObjX, pBlock);

	//					//上下
	//					pBlock->ExtrusionCollisionY(pObjX, pBlock);
	//				}
	//			}
	//		}
	//		//リストを次に進める
	//		pObj = pNext;
	//	}
	//}
}
//=========================================================================================================================

//===============================================================
//指定下ブロックの上に乗っている場合、位置を補正する
//===============================================================
void CBlock::LandingCorrection(D3DXVECTOR3& Pos, CObject* pSaveObj,D3DXVECTOR3 VtxMin)
{
	CObject::TYPE type = (CObject::TYPE)(0);
	D3DXVECTOR3 ComparisonPos = D3DXVECTOR3(0.0f,0.0f,0.0f);        //比較用位置
	D3DXVECTOR3 ComparisonPosOld = D3DXVECTOR3(0.0f,0.0f,0.0f);     //比較用1f前の位置
	D3DXVECTOR3 ComparisonVtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f);     //比較用最高頂点
	D3DXVECTOR3 ComparisonVtxMin = D3DXVECTOR3(0.0f,0.0f,0.0f);     //比較用最低頂点
	D3DXVECTOR3 ComparisonSize = D3DXVECTOR3(0.0f,0.0f,0.0f);       //サイズ
	D3DXVECTOR3 ComparisonMove = D3DXVECTOR3(0.0f,0.0f,0.0f);

	D3DXVECTOR3 MyVtxMin = D3DXVECTOR3(0.0f,0.0f,0.0f);
	MyVtxMin.x = (float)(floor(VtxMin.x));
	MyVtxMin.y = (float)(floor(VtxMin.y));
	MyVtxMin.z = (float)(floor(VtxMin.z));

	if (pSaveObj != nullptr)
	{
		//種類の取得
		type = pSaveObj->GetType();
		if (type == CObject::TYPE::BLOCK)
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
//必要な情報をテキストファイルに保存する
//=======================================================================
void CBlock::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETBLOCK" << endl;
	WritingFile << "TYPE = " << static_cast<int>(m_type);
	switch (m_type)
	{
	case BLOCKTYPE::NORMAL:
		WritingFile << " # NORMALBLOCK" << endl;
		break;
	case BLOCKTYPE::WATER:
		WritingFile << " # WATERBLOCK" << endl;
		break;
	default:
		break;
	}

	CObjectXAlive::SaveInfoTxt(WritingFile);

	WritingFile << "END_SETBLOCK" << endl;
}
//=========================================================================================================================

//=======================================================================
//ステージマネージャーに変更したオブジェクトを返す
//=======================================================================
CObject* CBlock::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_type);
	BLOCKTYPE NewType = BLOCKTYPE::NORMAL;

	//=======================================
	//種類を変える
    //=======================================
	if (bAim == true)
	{
		nNewType++;
	}
	else
	{
		nNewType--;
	}
	if (nNewType >= static_cast<int>(BLOCKTYPE::MAX))
	{
		nNewType = static_cast<int>(BLOCKTYPE::NORMAL);
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(BLOCKTYPE::MAX) - 1;
	}
	//======================================================================================

	//=======================================
	//設定する
	//=======================================
	NewType = BLOCKTYPE(nNewType);
	//======================================================================================

	//=======================================
	//死亡フラグを設定する
	//=======================================
	SetUseDeath(true);
	SetDeath();
	//======================================================================================

	return CBlock::Create(NewType, GetMaxLife(), GetPos(), GetRot(), GetScale(),GetUseSwapVtxXZ());//生成したオブジェクトを返す
}
//=========================================================================================================================

//=======================================================================
//テキストファイルから読み込んだ情報を保存する処理
//=======================================================================
void CBlock::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	int nType = 0;//種類
	int nLife = 0;//体力
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f,0.0f,0.0f);//移動量
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f); //位置
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f,0.0f,0.0f);//拡大率
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f,0.0f,0.0f); //向き
	BLOCKTYPE Type = {};//ブロックの種類
	bool bSwapVtxXZ = false;
	while (Buff != "END_SETBLOCK")
	{
		LoadingFile >> Buff;//単語を読み込む
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "TYPE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nType;      //種類
		}
		else if (Buff == "LIFE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> nLife;      //体力
		}
		else if (Buff == "MOVE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Move.x;      //移動量X
			LoadingFile >> Move.y;      //移動量Y
			LoadingFile >> Move.z;      //移動量Z
		}
		else if (Buff == "POS")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Pos.x;      //位置X
			LoadingFile >> Pos.y;      //位置Y
			LoadingFile >> Pos.z;      //位置Z
		}
		else if (Buff == "ROT")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Rot.x;      //位置X
			LoadingFile >> Rot.y;      //位置Y
			LoadingFile >> Rot.z;      //位置Z
		}
		else if (Buff == "SCALE")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> Scale.x;      //拡大率X
			LoadingFile >> Scale.y;      //拡大率Y
			LoadingFile >> Scale.z;      //拡大率Z
		}
		else if (Buff == "SWAPVTXXZ")
		{
			LoadingFile >> Buff;//イコール
			LoadingFile >> bSwapVtxXZ;
		}
	}
	Type = BLOCKTYPE(nType);

	listSaveManager.push_back(CBlock::Create(Type, nLife, Pos, Rot, Scale, bSwapVtxXZ));//vectorに情報を保存する
}
//=========================================================================================================================

//=======================================================================
//ステージマネージャーにセーブする
//=======================================================================
CObject* CBlock::ManagerSaveObject()
{
	return CBlock::Create(m_type, GetMaxLife(), GetPos(), GetRot(), GetScale(),GetUseSwapVtxXZ());//生成したオブジェクトを返す
}
//=========================================================================================================================

//=======================================================================
//X方向の押し出し判定を行う
//=======================================================================
void CBlock::ExtrusionCollisionX(CObjectXAlive* pMyObjX, CBlock* pBlock)
{
	//D3DXVECTOR3 MyPos = pMyObjX->GetPos();
	//const D3DXVECTOR3& Pos = pMyObjX->GetPos();              //位置を取得
	//const D3DXVECTOR3& PosOld = pMyObjX->GetPosOld();        //1f前の位置を取得
	//D3DXVECTOR3 MyVtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f);             //自分自身の最大頂点
	//D3DXVECTOR3 MyVtxMin = D3DXVECTOR3(0.0f,0.0f,0.0f);             //自分自身の最小頂点

	//D3DXVECTOR3 ComPos = pBlock->GetPos();             //ブロックの位置
	//D3DXVECTOR3 ComVtxMax = pBlock->GetVtxMax();       //ブロックの最大頂点
	//D3DXVECTOR3 ComVtxMin = pBlock->GetVtxMin();       //ブロックの最小頂点

	//bool& bIsWalling = pMyObjX->GetWalling();          //壁にくっついているかどうか
	////押し出しに使う値の小数点を切り捨てる（微小な差で失敗しないため）
	//MyVtxMax.x = (float)(floor(pMyObjX->GetVtxMax().x));
	//MyVtxMax.y = (float)(floor(pMyObjX->GetVtxMax().y));
	//MyVtxMax.z = (float)(floor(pMyObjX->GetVtxMax().z));

	//MyVtxMin.x = (float)(floor(pMyObjX->GetVtxMin().x));
	//MyVtxMin.y = (float)(floor(pMyObjX->GetVtxMin().y));
	//MyVtxMin.z = (float)(floor(pMyObjX->GetVtxMin().z));

	//ComPos.x = float(floor(pBlock->GetPos().x));
	//ComPos.y = float(floor(pBlock->GetPos().y));
	//ComPos.z = float(floor(pBlock->GetPos().z));

	//ComVtxMax.x = (float)(floor(ComVtxMax.x));
	//ComVtxMax.y = (float)(floor(ComVtxMax.y));
	//ComVtxMax.z = (float)(floor(ComVtxMax.z));

	//ComVtxMin.x = (float)(floor(ComVtxMin.x));
	//ComVtxMin.y = (float)(floor(ComVtxMin.y));
	//ComVtxMin.z = (float)(floor(ComVtxMin.z));
	//// 					//左
	//if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
	//	&& PosOld.x + MyVtxMax.x <= ComPos.x + ComVtxMin.x
	//	&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
	//	&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
	//	&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
	//	&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	//{//対象がブロックの-Xに当たった時の処理
	//	//Pos.x = ComPos.x + ComVtxMin.x - MyVtxMax.x;
	//	bIsWalling = true;
	// 
	//}

	////右
	//else if (Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
	//	&& PosOld.x + MyVtxMin.x >= ComPos.x + ComVtxMax.x
	//	&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
	//	&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
	//	&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
	//	&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	//{//対象がブロックの+Xに当たった時の処理
	//	//Pos.x = ComPos.x + ComVtxMax.x - MyVtxMin.x;
	//	bIsWalling = true;
	//}
}
//======================================================================================================================================

//=======================================================================
//Y方向の押し出し判定を行う
//=======================================================================
void CBlock::ExtrusionCollisionY(CObjectXAlive* pMyObjX, CBlock* pBlock)
{
	//D3DXVECTOR3 MyVtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f);             //自分自身の最大頂点
	//D3DXVECTOR3 MyVtxMin = D3DXVECTOR3(0.0f,0.0f,0.0f);             //自分自身の最小頂点

	//D3DXVECTOR3 ComPos = pBlock->GetPos();             //ブロックの位置
	//D3DXVECTOR3 ComVtxMax = pBlock->GetVtxMax();       //ブロックの最大頂点
	//D3DXVECTOR3 ComVtxMin = pBlock->GetVtxMin();       //ブロックの最小頂点[
	//const D3DXVECTOR3& ComMove = pBlock->GetMove();           //ブロックの移動量
	//const D3DXVECTOR3& Pos = pMyObjX->GetPos();              //位置を取得
	//const D3DXVECTOR3& Move = pMyObjX->GetMove();
	//D3DXVECTOR3 PosOld = pMyObjX->GetPosOld();        //1f前の位置を取得

	//bool& bIsLanding = pMyObjX->GetLanding();          //地面にいるかどうか

	//bool& bIsJumping = pMyObjX->GetJumping();          //ジャンプしているかどうか

	////押し出し失敗対策
	//MyVtxMax.x = (float)(floor(pMyObjX->GetVtxMax().x));
	//MyVtxMax.y = (float)(floor(pMyObjX->GetVtxMax().y));
	//MyVtxMax.z = (float)(floor(pMyObjX->GetVtxMax().z));

	//MyVtxMin.x = (float)(floor(pMyObjX->GetVtxMin().x));
	//MyVtxMin.y = (float)(floor(pMyObjX->GetVtxMin().y));
	//MyVtxMin.z = (float)(floor(pMyObjX->GetVtxMin().z));

	//ComPos.x = float(floor(pBlock->GetPos().x));
	//ComPos.y = float(floor(pBlock->GetPos().y));
	//ComPos.z = float(floor(pBlock->GetPos().z));

	//

	//ComVtxMax.x = (float)(floor(ComVtxMax.x));
	//ComVtxMax.y = (float)(floor(ComVtxMax.y));
	//ComVtxMax.z = (float)(floor(ComVtxMax.z));

	//ComVtxMin.x = (float)(floor(ComVtxMin.x));
	//ComVtxMin.y = (float)(floor(ComVtxMin.y));
	//ComVtxMin.z = (float)(floor(ComVtxMin.z));

	////上
	//if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
	//	&& Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
	//	&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
	//	&& PosOld.y + MyVtxMin.y - Move.y >= ComPos.y + ComVtxMax.y
	//	&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
	//	&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	//{//対象の下端がモデルの+Yに当たった時の処理
	//	float fPosY = fabsf(MyVtxMin.y);
	//	//Pos.y = ComPos.y + ComVtxMax.y + fPosY;
	//	pMyObjX->SetPos(D3DXVECTOR3(Pos.x, Pos.y + ComVtxMax.y + fPosY, Pos.z));
	//	pMyObjX->SetPos(Pos + ComMove);
	//	bIsLanding = true;
	//}
	////下
	//else if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
	//	&& Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
	//	&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
	//	&& PosOld.y + MyVtxMax.y <= ComPos.y + ComVtxMin.y
	//	&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
	//	&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	//{//対象の下端がモデルの+Yに当たった時の処理
	//	//Pos.y = ComPos.y + ComVtxMin.y - MyVtxMax.y;
	//	pMyObjX->SetPos(D3DXVECTOR3(Pos.x,ComPos.y + ComVtxMin.y - MyVtxMax.y, Pos.z));
	//	bIsJumping = false;

	//	if (pMyObjX->GetType() == CObject::TYPE_PLAYER)
	//	{
	//		//Move.y = 0.0f;
	//		pMyObjX->SetMove(D3DXVECTOR3(Move.x, 0.0f, Move.z));
	//	}
	//}
}
//======================================================================================================================================