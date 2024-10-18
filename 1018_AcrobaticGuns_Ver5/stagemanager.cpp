//==========================================================
//
//７月１７日：オブジェクトのリスト管理を実装[stagemanager.cpp]
//Author:ShinaTaiyo
//
//==========================================================

//================================
//インクルード
//================================
#include "stagemanager.h"
#include "objectX.h"
#include "objectXInfo.h"
#include "input.h"
#include "manager.h"
#include "object.h"
#include "camera.h"
#include "debugproc.h"
#include <stdio.h>
#include <string.h>
#include "calculation.h"
//==========================================================

//================================
//前方宣言
//================================
const char* CStageManager::m_apWORLDMAP_TXT[CStageManager::WORLDTYPE_MAX] =
{
	"data\\TEXTFILE\\Map\\EasyMap.txt",
	"data\\TEXTFILE\\Map\\NormalMap.txt",
	"data\\TEXTFILE\\Map\\BossRushMap.txt",
};

//================================
//コンストラクタ
//================================
CStageManager::CStageManager() : m_nWorldIndex(0),m_pBg3D(nullptr)
{
	for (int nCnt = 0; nCnt < m_nMAXMANAGEROBJECT; nCnt++)
	{
		m_apObjectList[nCnt] = nullptr;
	}

	for (int nCnt = 0; nCnt < m_nMAX_MAP; nCnt++)
	{
		for (int nCntWord = 0; nCntWord < m_nMAX_WORD; nCntWord++)
		{
			m_aMapFilePass[nCnt][nCntWord] = {};
		}
	}
}
//==========================================================

//================================
//デストラクタ
//================================
CStageManager::~CStageManager()
{

}
//==========================================================

//================================
//初期化処理
//================================
HRESULT CStageManager::Init()
{
	//===========================
	//メンバ変数
	//===========================
	m_nMapIndex = 0;                                     //マップのインデックス
	m_nMapNum = 0;                                       //マップの総数
	m_Scale = ONE_VECTOR3;                               //拡大率
	m_Rot = NULL_VECTOR3;                                //向き
	m_Pos = NULL_VECTOR3;                                //位置
	m_SaveBeforeChoosePos = NULL_VECTOR3;                //選択処理をする前の位置を記憶する
 	m_pManagerObject = nullptr;                          //マネージャーに表示するオブジェクト
	m_nIndexObject = 0;                                  //オブジェクト番号
	m_ManagerMode = MANAGERMODE_ALREADYSTAGE;            //現在のステージマネーシャーのモード

	//=========================
	//選択系
	//=========================
	m_pChooseObject = nullptr;                          //選択オブジェクトへの
	m_nIndexChooseObject = 0;                           //選択しているオブジェクトの番号
	m_bChooseObject = false;                            //オブジェクトを選択しているかどうか
	m_bMakeMapMode = false;                             //マップ制作モードかどうか
	//=======================================================================================
#ifdef _DEBUG
#endif // _DEBUG

	//===========================
	//移動モードを決める
	//===========================
	m_MoveMode = MOVEMODE00_XY;     //移動方法の種類
	m_FocusType = FOCUSTYPE_NORMAL;//カメラが追う位置の種類
	m_bUseSizeMove = false;//現在のオブジェクトのサイズ分移動するかどうか
	//=======================================================================================

	//===========================
	//オブジェクトX初期化処理
	//===========================
	CObject::Init();
	//=======================================================================================

	//======================================
	//マップのファイルパスをロードする
	//======================================
	LoadMapFilePass(WORLDTYPE00_EASY);
	return S_OK;
}
//==========================================================

//================================
//終了処理
//================================
void CStageManager::Uninit()
{
	StageManagerObjectReleaseAll();
     
}
//==========================================================

//================================
//別枠の終了処理
//================================
void CStageManager::ExtraUninit()
{

}
//==========================================================

//================================
//更新処理
//================================
void CStageManager::Update()
{

#ifdef _DEBUG

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		if (m_bChooseObject == false)
		{
			//===============================================
			//ステージマネージャーの色を変える
			//===============================================
			ColorChenge();
			//===========================================================================================

			//===============================================
			//体力を設定する
			//===============================================
			LifeSet();
			//===========================================================================================

			//===============================================
			//ステージマネージャーの位置を移動する
			//===============================================
			MoveManager();
			//===========================================================================================

			//===============================================
			//現在のブロックの種類を変更する
			//===============================================
			TypeChenge();
			//===========================================================================================

			//===============================================
			//拡大率をリセットする処理
			//===============================================
			ResetScale();
			//===========================================================================================

			//===============================================
			//ブロックを設定する
			//===============================================
			SetObjectX();
			//===========================================================================================

			////===============================================
			////ブロックを消す処理
			////===============================================
			//DeleteObjectX();
			////===========================================================================================

			//===============================================
			//マップを変える処理
			//===============================================
			MapChenge();
			//===========================================================================================

			//===============================================
			//オブジェクト全消去処理
			//===============================================
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_RSHIFT) == true)
			{
				StageManagerObjectReleaseAll();
			}
			//===========================================================================================
		}

		//===============================================
		//オブジェクト選択処理
		//===============================================
		ChooseObject();
		//===========================================================================================

		//===============================================
		//情報表示処理
		//===============================================
		DispInfo();
		//===========================================================================================

		//===============================================
		//オブジェクト更新
		//===============================================
		CObject::Update();
		//===========================================================================================
	}

#endif // _DEBUG
}
//==========================================================

//================================
//描画処理
//================================
void CStageManager::Draw()
{
#ifdef _DEBUG

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
        //オブジェクト描画
	    CObject::Draw();
	}
#endif // _DEBUG

}
//============================================================================================================

//======================================================
//死亡フラグを設定
//======================================================
void CStageManager::SetDeath()
{
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(true);
		m_pManagerObject->SetDeath();
		m_pManagerObject = nullptr;
	}

	CObject::SetDeath();
}
//============================================================================================================

//============================================
//マップをロードする処理
//============================================
void CStageManager::LoadMapTxt(int nMapNum)
{
	//==================
	//変数
	//==================
	char aString[128] = {};
	m_nMapIndex = nMapNum;//マップのインデックスを設定
	FILE* pFile = nullptr;//ファイルポインタを宣言

	//=================================================
	//現在のモードによってロードするファイルを変える
	//=================================================
	if (m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		pFile = fopen(&m_aMapFilePass[m_nMapIndex][0], "r");
	}
	else
	{
		pFile = fopen(SAVE_TXT, "r");
	}
	//=====================================================================================================================

	int nManagerType = 0;                  //マネージャーオブジェクトの種類
	int nType = 0;                         //オブジェクトごとの種類
	int nRotType = 0;                      //向きの種類
	int nSubType = 0;                      //サブタイプ
	D3DXVECTOR3 pos = NULL_VECTOR3;        //位置
	D3DXVECTOR3 WarpPos = NULL_VECTOR3;
	int nWarpMap = 0;//ワープ先のマップ番号
	int nLife = 0;//体力
	D3DXVECTOR3 Scale = NULL_VECTOR3;      //拡大率
	D3DXVECTOR3 Rot = NULL_VECTOR3;        //向き
	CObject* pObject = nullptr;            //オブジェクトへのポインタ
	//======================================================
	//ReleaseAll();
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		StageManagerObjectReleaseAll();
	}


	if (pFile != nullptr)
	{
		while (1)
		{

			fscanf(pFile, "%s", &aString[0]);

			if (aString[0] == '#')
			{
				fgets(&aString[0], 100, pFile);//行をスキップ
			}
			else if (strcmp(&aString[0], "SETOBJECT") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);
					if (aString[0] == '#')
					{
						fgets(&aString[0], 100, pFile);//行をスキップ
					}
					else if (strcmp(&aString[0], "MANAGERTYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%d", &nManagerType);//マネージャーオブジェクトのタイプ取得
					}
					else if (strcmp(&aString[0], "TYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%d", &nType);       //オブジェクトごとのタイプ取得
					}
					else if (strcmp(&aString[0], "ROTTYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%d", &nRotType);    //オブジェクトごとのタイプ取得
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%f", &pos.x);       //X座標読み取り
						fscanf(pFile, "%f", &pos.y);       //Y座標読み取り
						fscanf(pFile, "%f", &pos.z);       //Z座標読み取り
					}
					else if (strcmp(&aString[0], "SCALE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%f", &Scale.x);       //X座標読み取り
						fscanf(pFile, "%f", &Scale.y);       //Y座標読み取り
						fscanf(pFile, "%f", &Scale.z);       //Z座標読み取り
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%f", &Rot.x);       //X座標読み取り
						fscanf(pFile, "%f", &Rot.y);       //Y座標読み取り
						fscanf(pFile, "%f", &Rot.z);       //Z座標読み取り
					}
					else if (strcmp(&aString[0], "LIFE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%d", &nLife);       //Z座標読み取り
					}
					else if (strcmp(&aString[0], "SUBTYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%d", &nSubType);    //サブタイプ
					}
					else if (strcmp(&aString[0], "END_SETOBJECT") == 0)
					{//Xオブジェクトを設定
						if (CScene::GetMode() == CScene::MODE_EDIT)

						{//エディットモードなら、オブジェクトをリストに保存し管理する
						}
						break;
					}

				}
			}
			else if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{
				break;
				fclose(pFile);
			}
		}
	}
}
//============================================================================================================

//===========================================
//マップのファイルパスをロードする
//===========================================
void CStageManager::LoadMapFilePass(WORLDTYPE type)
{
	char aString[512] = {};
	//===============================================================================================

	m_nMapNum = 0;//マップ総数を初期化

	FILE* pFileTxt = nullptr;
	pFileTxt = fopen(m_apWORLDMAP_TXT[type], "r");

	if (pFileTxt != nullptr)
	{
		while (1)
		{
			fscanf(pFileTxt, "%s", &aString[0]);
			if (aString[0] == '#')
			{
				fgets(&aString[0], 100, pFileTxt);//行をスキップ
			}
			else if (strcmp(&aString[0], "FILENAME") == 0)
			{
				fscanf(pFileTxt, "%s", &aString[0]);//イコール読み取り用
				fscanf(pFileTxt, "%s", &m_aMapFilePass[m_nMapNum][0]);//ファイル名読み取り用
				m_nMapNum++;
			}
			else if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{
				fclose(pFileTxt);
				break;
			}
		}

	}

}
//============================================================================================================

//============================================
//マップをセーブする処理
//============================================
void CStageManager::SaveMapTxt(int nMapNum)
{
	D3DXVECTOR3 Pos = NULL_VECTOR3;
	D3DXVECTOR3 WarpPos = NULL_VECTOR3;
	D3DXVECTOR3 Scale = NULL_VECTOR3;
	D3DXVECTOR3 Rot = NULL_VECTOR3;
	int nWarpMap = 0;//ワープ先のマップ番号
	int nManagerType;//オブジェクトXのタイプ
	int nType = 0;//オブジェクトXごとのタイプ
	D3DXVECTOR3 move = NULL_VECTOR3;
	float fWidth = 0.0f;
	float fHeight = 0.0f;
	int nLife = 0;        //体力
	m_nMapIndex = nMapNum;
	int nSubType = 0;     //サブタイプ
	FILE* pFile = nullptr;
	CObject* pObj = nullptr;//オブジェクト取得用

	//=================================================
	//現在のモードによってセーブするファイルを変える
	//=================================================
	if (m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		pFile = fopen(&m_aMapFilePass[m_nMapIndex][0], "w");
	}
	else
	{
		pFile = fopen(SAVE_TXT, "w");
	}
	//=====================================================================================================================

	if (pFile != nullptr)
	{
		pObj = GetTopStageManagerObject();//トップオブジェクトを取得

		while (pObj != nullptr)
		{
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextStageManagerObject();

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();

			//==============================================
			//オブジェクト情報を書き出す
			//==============================================
			nLife = ((CObjectX*)pObj)->GetLife();
			nManagerType = ((CObjectX*)pObj)->GetManagerType(); //Xオブジェクトのタイプを取得
			nType = ((CObjectX*)pObj)->GetTypeNum();            //Xオブジェクトごとのタイプを取得
			Pos = ((CObjectX*)pObj)->GetSupportPos();           //位置を書き出す
			Scale = ((CObjectX*)pObj)->GetScale();				//拡大率を書き出す
			Rot = ((CObjectX*)pObj)->GetRot();					//向きを書き出す
			fprintf(pFile, "SETOBJECT\n");
			fprintf(pFile, "MANAGERTYPE = %d\n", nManagerType);
			fprintf(pFile, "TYPE = %d\n", nType);
			fprintf(pFile, "POS = %.3f %.3f %.3f\n", Pos.x, Pos.y, Pos.z);
			fprintf(pFile, "SCALE = %.3f %.3f %.3f\n", Scale.x, Scale.y, Scale.z);
			fprintf(pFile, "ROT = %.3f %.3f %.3f\n", Rot.x, Rot.y, Rot.z);
			fprintf(pFile, "LIFE = %d\n", nLife);
			fprintf(pFile, "SUBTYPE = %d\n", nSubType);
			fprintf(pFile, "END_SETOBJECT\n\n");
		}
		//========================================================================
		fprintf(pFile, "END_SCRIPT");
		fclose(pFile);
	}
	//================================================================================

}
//======================================================================================================================

//==========================================================
//マップをバイナリファイルから読み込む
//==========================================================
void CStageManager::LoadMapBin(int nMapNum)
{
	//======================================
	//変数宣言
	//======================================
	D3DXVECTOR3 Pos = NULL_VECTOR3;
	D3DXVECTOR3 Scale = NULL_VECTOR3;
	D3DXVECTOR3 Rot = NULL_VECTOR3;
	int nType = 0;            //オブジェクトXごとのタイプ
	int nManagerType = 0;        //オブジェクトXのタイプ
	int nRotType = 0;         //向きのタイプ
	D3DXVECTOR3 move = NULL_VECTOR3;
	float fWidth = 0.0f;
	float fHeight = 0.0f;
	int nData = 0;
	char aString[512] = {};
	char aStringMap[100] = {};
	CObject* pObject = nullptr;
	//===============================================================================================
	StageManagerObjectReleaseAll();
	FILE* pFile = nullptr;
	pFile = fopen(&m_aMapFilePass[nMapNum][0], "rb");
	//pFile = fopen(&m_aMapFilePass[0][0], "rb");
	if (pFile != nullptr)
	{
		//==============================================
		//オブジェクトの情報を読み込む
		//==============================================
		fread(&m_nIndexObject, sizeof(int), 1, pFile);
		if (m_nIndexObject > 0)
		{
			for (int nCnt = 0; nCnt < m_nIndexObject; nCnt++)
			{//オブジェクトを設定
				fread(&nManagerType, sizeof(int), 1, pFile);         //Xオブジェクトのタイプを読み込む
				fread(&nType, sizeof(int), 1, pFile);             //Xオブジェクトごとのタイプを読み込む
				fread(&nRotType, sizeof(int), 1, pFile);          //向きの種類を読み込む
				fread(&Pos, sizeof(D3DXVECTOR3), 1, pFile);       //位置を読み込む
				fread(&Scale, sizeof(D3DXVECTOR3), 1, pFile);     //拡大率を読み込む
				fread(&Rot, sizeof(D3DXVECTOR3), 1, pFile);       //向きを読み込む

				//switch (nManagerType)
				//{
				//	case (int)(MANAGEROBJECT_BLOCK) :
				//		pObject = CBlock::Create(CBlock::BLOCKTYPE(nType), 10, Pos, Rot, Scale, (CObjectX::ROTTYPE)(nRotType), m_nSetSubType);//生成処理
				//		break;
				//		case (int)(MANAGEROBJECT_MODEL) :
				//			pObject = CModel::Create((CModel::MODELTYPE)(nType), Pos, Rot, NULL_VECTOR3, Scale, (CObjectX::ROTTYPE)(nRotType));//生成処理
				//			break;
				//				case (int)(MANAGEROBJECT_ENEMY) :
				//					pObject = CEnemy::Create((CEnemy::ENEMYTYPE)(nType), 10, Pos, NULL_VECTOR3, Scale,NULL_VECTOR3,NULL_VECTOR3,ONE_VECTOR3);//生成処理
				//					break;
				//					case (int)(MANAGEROBJECT_ITEM) :
				//						pObject = CItem::Create((CItem::ITEMTYPE)(nType), Pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL);//生成処理
				//						break;
				//						case (int)(MANAGEROBJECT_MARKER) :
				//							pObject = CMarker::Create((CMarker::MARKERTYPE)(nType),Pos,Scale, NULL_VECTOR3, 0);
				//							break;
				//							case (int)(MANAGEROBJECT_BOSS) :
				//								pObject = CBoss::SetCreateBoss((CBoss::BOSSTYPE)(nType),10, Pos, Scale);
				//								break;


				//				default:
				//					break;
				//}

				if (pObject != nullptr)
				{
					m_apObjectList[nCnt] = pObject;
				}
			}
		}
		//========================================================================
		fclose(pFile);
	}
}
	//======================================================================================================================

//==========================================================
//マップをバイナリファイルに書き出す
//==========================================================
void CStageManager::SaveMapBin()
{
	//======================================
	//変数宣言
	//======================================
	D3DXVECTOR3 Pos = NULL_VECTOR3;
	D3DXVECTOR3 Scale = NULL_VECTOR3;
	D3DXVECTOR3 Rot = NULL_VECTOR3;
	int nType = 0;             //オブジェクトXごとのタイプ
	int nManagerType = 0;             //オブジェクトXのタイプ
	D3DXVECTOR3 move = NULL_VECTOR3;
	float fWidth = 0.0f;
	float fHeight = 0.0f;
	FILE* pFile = fopen(SAVE_BIN, "wb");
	//===============================================================================================
	if (pFile != nullptr)
	{

		fwrite(&m_nIndexObject, sizeof(int),1, pFile);
		//==============================================
        //ブロックのXオブジェクトの情報を書き出す
        //==============================================
		for (int nCnt = 0; nCnt < m_nIndexObject; nCnt++)
		{//ブロックを設定
			if (m_apObjectList[nCnt] != nullptr)
			{
				nManagerType = ((CObjectX*)m_apObjectList[nCnt])->GetManagerType();  //Xオブジェクトのタイプを取得
				nType = (int)(((CObjectX*)m_apObjectList[nCnt])->GetTypeNum());       //Xオブジェクトごとのタイプを取得
				Pos = ((CObjectX*)m_apObjectList[nCnt])->GetPos();                    //位置を書き出す
				Scale = ((CObjectX*)m_apObjectList[nCnt])->GetScale();				//拡大率を書き出す
				Rot = ((CObjectX*)m_apObjectList[nCnt])->GetRot();					//向きを書き出す

				fwrite(&nManagerType, sizeof(int), 1, pFile);              //Xオブジェクトのタイプを書き出す
				fwrite(&nType, sizeof(int), 1, pFile);                  //Xオブジェクトごとのタイプを書き出す
				fwrite(&Pos, sizeof(D3DXVECTOR3), 1, pFile);            //位置を書き出す
				fwrite(&Scale, sizeof(D3DXVECTOR3), 1, pFile);          //拡大率を書き出す
				fwrite(&Rot, sizeof(D3DXVECTOR3), 1, pFile);            //向きを書き出す
			}
		}
		//========================================================================
		fclose(pFile);
	}
}
//======================================================================================================================

//==============================================
//ステージマネージャー生成処理
//==============================================
CStageManager* CStageManager::Create()
{
	CStageManager* pStageManager = DBG_NEW CStageManager;                                                                        //プレイヤーを生成

	bool bSuccess = pStageManager->CObject::GetCreateSuccess();
	if (bSuccess == true)
	{//空きがあったので生成開始
		if (pStageManager != nullptr)
		{
			pStageManager->Init();                                                                                           //初期化処理
			pStageManager->SetUseDeath(false);                                                                        //死亡フラグを発動するかどうかを設定する
			pStageManager->CObject::SetType(CObject::TYPE_STAGEMANAGER);                                                     //オブジェクトの種類を決める   
		}
	}
	else
	{//オブジェクトに空きがなかったので破棄する
		delete pStageManager;
		pStageManager = nullptr;
	}

	return pStageManager;
}
//================================================================================

//============================================
//ステージマネージャーを移動させる処理
//============================================
void CStageManager::MoveManager()
{
	//=========================================
	//変数宣言
	//=========================================
	D3DXVECTOR3 MoveSize = ((CObjectX*)m_pManagerObject)->GetSize();//移動するサイズを求める
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();//キーボードクラスの取得
	float fMoveX = 0.0f;                                          //X方向の移動量
	float fMoveZ = 0.0f;                                          //Z方向の移動量
	bool bMove = false;                                           //移動しているかどうか
	//===========================================================================================================================

	//=================================
	//移動タイプを切り替える
	//=================================
	if (pInputKeyboard->GetTrigger(DIK_MINUS) == true)
	{
		if (m_MoveMode == MOVEMODE00_XY)
		{
			m_MoveMode = MOVEMODE01_XZ;
		}
		else if (m_MoveMode == MOVEMODE01_XZ)
		{
			m_MoveMode = MOVEMODE00_XY;
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_C) == true)
	{//サイズ分動かすかどうか
		m_bUseSizeMove = m_bUseSizeMove ? false : true;
	}

	//==================================================================================================

	//=================================
	//位置を戻す
	//=================================
	if (pInputKeyboard->GetTrigger(DIK_PERIOD))
	{
		m_Pos.z = 0.0f;
		m_Pos.y = 0.0f;
	}
	//==================================================================================================

	//============================
	// 移動ボタンを押していたら
	//============================
	CCalculation::CaluclationMove(m_Pos, 5.0f, CCalculation::MOVEAIM_XZ,m_Rot.y);

	//位置を設定する
	((CObjectX*)m_pManagerObject)->SetPos(m_Pos);
	//==================================================================================================

}
//===========================================================================================

//=================================================
//体力を設定する
//=================================================
void CStageManager::LifeSet()
{
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_F8) == true)
	//{
	//	m_nSetLife += 10;
	//}
	//else if (CManager::GetInputKeyboard()->GetTrigger(DIK_F7) == true)
	//{
	//	m_nSetLife -= 10;
	//}
}
//===========================================================================================

//=================================================
//選択ブロックの種類を変える
//=================================================
void CStageManager::TypeChenge()
{
	//=========================================
    //変数宣言
    //=========================================

	////それぞれのオブジェクトタイプのファイル数を取得する
	//int nNumFile[MANAGEROBJECT_MAX] = { CBlock::GetNumFile(),
	//	CModel::GetNumFile(),
	//    CEnemy::GetNumFile(),
	//    CItem::GetNumFile(),
	//    CMarker::GetNumFile(),
	//    CBoss::GetNumFile(),
	//    CTrap::GetNumFile(),
	//    CSignBoard::GetNumFile()};

	//const int* pNumSubType = nullptr;//それぞれのオブジェクトのサブタイプを取得する

	////=====================================================================
 //   //オブジェクトXの種類を移動する
 //   //=====================================================================
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
	//{
	//	m_nManagerType++;
	//	m_nSetSubType = 0;//サブタイプ番号をリセット
	//	if (m_nManagerType >= (int)(MANAGEROBJECT_MAX))
	//	{
	//		m_nManagerType = 0;
	//	}
	//	m_nType = 0;//オブジェクト別タイプ番号を０に戻す
	//	ChengeObject();//オブジェクトの種類を変える
	//}
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	//{
	//	m_nManagerType--;
	//	m_nSetSubType = 0;//サブタイプ番号をリセット
	//	if (m_nManagerType < 0)
	//	{
	//		m_nManagerType = (int)(MANAGEROBJECT_MAX) - 1;
	//	}
	//	m_nType = 0;//オブジェクト別タイプ番号を０に戻す
	//	ChengeObject();//オブジェクトの種類を変える
	//}
	////=====================================================================================================================================

	////=====================================================================
	////オブジェクトごとの種類を移動する
	////=====================================================================
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	//{
	//	m_nType++;
	//	if (m_nType >= nNumFile[m_nManagerType])
	//	{
	//		m_nType = 0;
	//	}
	//	m_nSetSubType = 0;//サブタイプ番号をリセット
	//	ChengeObject();//オブジェクトの種類を変える
	//}
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
	//{
	//	m_nType--;
	//	m_nSetSubType = 0;//サブタイプ番号をリセット
	//	if (m_nType < 0)
	//	{
	//		m_nType = nNumFile[m_nManagerType] - 1;
	//	}
	//	ChengeObject();//オブジェクトの種類を変える
	//}
	////=====================================================================================================================================

	////=====================================================================
	////サブタイプを設定する
	////=====================================================================

	////サブタイプ取得
	//switch (m_nManagerType)
	//{
	//case MANAGEROBJECT_BLOCK:
	//	pNumSubType = CBlock::GetSubTypeNum();
	//	break;
	//case MANAGEROBJECT_TRAP:
	//	pNumSubType = CTrap::GetSubTypeNum();
	//	break;
	//case MANAGEROBJECT_SIGNBOARD:
	//	pNumSubType = CSignBoard::GetSubTypeNum();
	//	break;
	//default:
	//	pNumSubType = nullptr;
	//	m_nSetSubType = 0;//サブタイプ番号をリセット
	//	break;
	//}

	//if (pNumSubType != nullptr)
	//{//サブタイプを設定する
	//	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9) == true)
	//	{
	//		m_nSetSubType++;
	//		if (m_nSetSubType >= pNumSubType[m_nType])
	//		{
	//			m_nSetSubType = 0;
	//		}
	//	}
	//	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F10) == true)
	//	{
	//		m_nSetSubType--;
	//		if (m_nSetSubType < 0)
	//		{
	//			m_nSetSubType = nNumFile[m_nType] - 1;
	//		}
	//	}
	//}


	//=====================================================================================================================================

	//=====================================================================
	//拡大率を変える
	//=====================================================================
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//Y軸縮小
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
		{
			m_Scale.x -= 1.0f;
		}
	}
	else
	{//Y軸拡大
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
		{
			m_Scale.x += 1.0f;
		}
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//Y軸縮小
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
		{
			m_Scale.y -= 1.0f;
		}
	}
	else
	{//Y軸拡大
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
		{
			m_Scale.y += 1.0f;
		}
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//Z軸縮小
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
		{
			m_Scale.z -= 1.0f;
		}
	}
	else
	{//Z軸拡大
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
		{
			m_Scale.z += 1.0f;
		}
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//等比縮小
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{
			m_Scale -= ONE_VECTOR3;
		}
	}
	else
	{//等比拡大
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{
			m_Scale += ONE_VECTOR3;
		}
	}

	((CObjectX*)m_pManagerObject)->SetScale(m_Scale);
	//=====================================================================================================================================
}
//===========================================================================================

//==================================================
//Xオブジェクト設定処理
//==================================================
void CStageManager::SetObjectX()
{
	CObject* pObject = nullptr;
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{
		//switch (m_nManagerType)
		//{
		//	case (int)(MANAGEROBJECT_BLOCK) :
		//    pObject = CBlock::Create((CBlock::BLOCKTYPE)(m_nType),m_nSetLife, m_Pos, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType), m_nSetSubType);
		//    break;
		//    case (int)(MANAGEROBJECT_MODEL) :
		//    pObject = CModel::Create((CModel::MODELTYPE)(m_nType),m_Pos, NULL_VECTOR3,NULL_VECTOR3,m_Scale,(CObjectX::ROTTYPE)(m_nRotType));
		//    break;
		//	case (int)(MANAGEROBJECT_ENEMY) :
		//	pObject = CEnemy::Create((CEnemy::ENEMYTYPE)(m_nType), m_nSetLife,m_Pos,NULL_VECTOR3,m_Scale, m_Pos,NULL_VECTOR3,ONE_VECTOR3);
		//	break;
		//	case (int)(MANAGEROBJECT_ITEM) :
		//	pObject = CItem::Create((CItem::ITEMTYPE)(m_nType), m_Pos, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType));//生成処理
		//	break;
		//	case (int)(MANAGEROBJECT_MARKER) :
		//	pObject = CMarker::Create((CMarker::MARKERTYPE)(m_nType),m_Pos,m_Scale, m_WarpPos, m_nWarpMapNum);
		//	break;
		//	case (int)(MANAGEROBJECT_BOSS) :
		//	pObject = CBoss::SetCreateBoss((CBoss::BOSSTYPE)(m_nType),m_nSetLife, m_Pos, m_Scale);
		//	break;
		//	case (int)(MANAGEROBJECT_TRAP) :
		//	pObject = CTrap::Create((CTrap::TRAPTYPE)(m_nType), m_Pos, NULL_VECTOR3, m_Scale, CObjectX::ROTTYPE_NORMAL,m_nSetSubType);//生成処理
		//	break;
		//	case (int)(MANAGEROBJECT_SIGNBOARD) :
		//		pObject = CSignBoard::Create(CSignBoard::SIGNBOARDTYPE(m_nType), CSignBoard::SIGNBOARDMANUALTYPE(m_nSetSubType),
		//			m_Pos, m_Scale, NULL_VECTOR3);
		//	break;
		//    default:
		//    break;
		//}
		//if (pObject != nullptr && m_nIndexObject < m_nMAXOBJECT)
		//{//オブジェクト情報をリストに保存
		//	m_apObjectList[m_nIndexObject] = pObject;

		pObject->SetStageManagerObj();//ステージマネージャー管理オブジェクトに設定する
		m_nIndexObject++;//インデックスを増やす

		//}

	}

}
//===========================================================================================

//==================================================
//Xオブジェクト消去処理
//==================================================
void CStageManager::DeleteObjectX()
{
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_DELETE) == true)
	//{
	//	if (m_apObjectList[m_nIndexObject - 1] != nullptr && m_nIndexObject >= 1)
	//	{
	//		if (((CObjectX*)m_apObjectList[m_nIndexObject - 1])->GetObjXType() == CObjectX::OBJECTXTYPE_MARKER)
	//		{
	//			((CMarker*)m_apObjectList[m_nIndexObject - 1])->DeleteNum();
	//		}
	//		m_apObjectList[m_nIndexObject - 1]->SetDeath();    //ブロックリストの動的確保メモリを開放
	//		m_apObjectList[m_nIndexObject - 1] = nullptr;          //ブロックリストのポインタを初期化
	//		m_nIndexObject--;                                     //インデックスをデクリメント
	//	}
	//}
}
//=======================================================================================================================

//====================================================
//設置オブジェクトの種類を変える
//====================================================
void CStageManager::ChengeObject()
{
	//オブジェクトをリリースする
	ReleaseObject();

	//switch (m_nManagerType)
	//{
	//	case (int)(MANAGEROBJECT_BLOCK) :
	//		m_pManagerObject = CBlock::Create((CBlock::BLOCKTYPE)(m_nType), 10, m_Pos, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType), m_nSetSubType);
	//		break;
	//		case (int)(MANAGEROBJECT_MODEL) :
	//			m_pManagerObject = CModel::Create((CModel::MODELTYPE)(m_nType), m_Pos, NULL_VECTOR3, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType));
	//			break;
	//			case (int)(MANAGEROBJECT_ENEMY) :
	//				m_pManagerObject = CEnemy::Create((CEnemy::ENEMYTYPE)(m_nType), 10, m_Pos, NULL_VECTOR3, m_Scale, m_Pos, NULL_VECTOR3, ONE_VECTOR3);
	//				break;
	//				case (int)(MANAGEROBJECT_ITEM) :
	//					m_pManagerObject = CItem::Create((CItem::ITEMTYPE)(m_nType), m_Pos, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType));//生成処理
	//					break;
	//					case (int)(MANAGEROBJECT_MARKER) :
	//						m_pManagerObject = CMarker::Create((CMarker::MARKERTYPE)(m_nType), m_Pos, m_Scale, NULL_VECTOR3, 0);
	//						break;
	//						case (int)(MANAGEROBJECT_BOSS) :
	//							m_pManagerObject = CBoss::SetCreateBoss((CBoss::BOSSTYPE)(m_nType), m_nSetLife, m_Pos, m_Scale);
	//							break;
	//							case (int)(MANAGEROBJECT_TRAP) :
	//								m_pManagerObject = CTrap::Create((CTrap::TRAPTYPE)(m_nType), m_Pos, NULL_VECTOR3, m_Scale, CObjectX::ROTTYPE_NORMAL, m_nSetSubType);//生成処理
	//								break;
	//								case (int)(MANAGEROBJECT_SIGNBOARD) :
	//									m_pManagerObject = CSignBoard::Create(CSignBoard::SIGNBOARDTYPE(m_nType), CSignBoard::SIGNBOARDMANUALTYPE(m_nSetSubType),
	//										m_Pos, m_Scale, NULL_VECTOR3);
	//									break;

	//								default:
	//									break;
	//}
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(false);
	}
}
//=======================================================================================================================

//==================================================================
//オブジェクトをリリースする
//==================================================================
void CStageManager::ReleaseObject()
{
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(true);
		m_pManagerObject->SetDeath();
		m_pManagerObject = nullptr;
	}

}

//================================================================
//色合いを設定する
//================================================================
void CStageManager::ColorChenge()
{

}
//=======================================================================================================================

//===========================================================
//マップを変える
//===========================================================
void CStageManager::MapChenge()
{
	//===============================================================
	//モードを変える
	//===============================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F4) == true)
	{
		SaveMapTxt(m_nMapIndex);//現在のマップを保存する

		if (m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
		{
			m_ManagerMode = MANAGERMODE_NEWSTAGE;
		}
		else
		{
			m_ManagerMode = MANAGERMODE_ALREADYSTAGE;
		}

		LoadMapTxt(m_nMapIndex);
	}
	//=======================================================================================================

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F3) == true && m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//現在のマップ情報をセーブする
		m_nMapIndex++;
		if (m_nMapIndex >= m_nMapNum)
		{
			m_nMapIndex = 0;
		}

		LoadMapTxt(m_nMapIndex);//番号のマップを読み込む
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_F2) == true && m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//現在のマップ情報をセーブする
		m_nMapIndex--;
		if (m_nMapIndex < 0)
		{
			m_nMapIndex = m_nMapNum - 1;
		}
		LoadMapTxt(m_nMapIndex);//番号のマップを読み込む
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_P) == true && m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//現在のマップ情報をセーブする

		m_nMapIndex = 0;//ワールドを変更するのでマップ番号を初期化
		m_nMapNum = 0;  //マップ総数を初期化

		m_nWorldIndex++;//次のワールドを選択

		if (m_nWorldIndex == WORLDTYPE_MAX)
		{
			m_nWorldIndex = 0;
		}

		//マップ情報をロードする
		LoadMapFilePass(WORLDTYPE(m_nWorldIndex));

		//マップを読み込む
		LoadMapTxt(m_nMapIndex);//番号のマップを読み込む
	}

}
//=======================================================================================================================

//===========================================================
//情報表示処理
//===========================================================
void CStageManager::DispInfo()
{
	char aChooseString[32] = {};//選択状態かどうかを記述
	char aCameraTypeName[CCamera::CAMERATYPE_MAX][100] = {};
	char aMapModeString[32] = {};
	//マップのカメラモード名
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BIRD][0], "バードビュー");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BESIDE][0], "ビサイドビュー");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_SIDE][0], "サイドビュー");


	if (m_bChooseObject == false)
	{
		strcpy(&aChooseString[0], "選択していない");
	}
	else
	{
		strcpy(&aChooseString[0], "選択中・・・");
	}

	if (m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		strcpy(&aMapModeString[0], "既存のステージを編集");
	}
	else
	{
		strcpy(&aMapModeString[0], "新しいステージを編集");
	}

	CManager::GetDebugProc()->PrintDebugProc("//=================================\n");
	CManager::GetDebugProc()->PrintDebugProc("//マップエディタの情報\n");
	CManager::GetDebugProc()->PrintDebugProc("//=================================\n");
	CManager::GetDebugProc()->PrintDebugProc("ステージマネージャー管理オブジェクトの総数：%d\n",GetTotalStageManagerObjNum());
	CManager::GetDebugProc()->PrintDebugProc("現在のワールド：%s\n",&m_apWORLDMAP_TXT[m_nWorldIndex][0]);
	CManager::GetDebugProc()->PrintDebugProc("現在のマップ番号(F2、F3で変更）：%d\n", m_nMapIndex);
	CManager::GetDebugProc()->PrintDebugProc("現在のマップファイルパス：%s\n",&m_aMapFilePass[m_nMapIndex][0]);
	CManager::GetDebugProc()->PrintDebugProc("オブジェクトの位置： %f %f %f\n",m_Pos.x,m_Pos.y,m_Pos.z);
	CManager::GetDebugProc()->PrintDebugProc("オブジェクトの拡大率リセット：V\n");
	CManager::GetDebugProc()->PrintDebugProc("現在のマップモード（F4）：%s\n",&aMapModeString[0]);
	CManager::GetDebugProc()->PrintDebugProc("オブジェクトを選択（L）：%s\n",&aChooseString[0]);
	CManager::GetDebugProc()->PrintDebugProc("選択オブジェクト変更：（N、M）\n");
	CManager::GetDebugProc()->PrintDebugProc("選択オブジェクト消去：（DEL）\n");
	CManager::GetDebugProc()->PrintDebugProc("移動方法を変更：ALTキー\n");
	CManager::GetDebugProc()->PrintDebugProc("移動方向変更：（マイナスキー）\n");
	CManager::GetDebugProc()->PrintDebugProc("Z座標を原点に戻す：（ピリオドキー）\n");
	CManager::GetDebugProc()->PrintDebugProc("全管理オブジェクト消去：（LSHIFTキー）\n");
	CManager::GetDebugProc()->PrintDebugProc("//=================================\n");

}
//=======================================================================================================================

//===========================================================
//オブジェクト選択処理
//===========================================================
void CStageManager::ChooseObject()
{
	bool bChoose = false;                              //オブジェクトを選択しているかどうか
	int nChooseIdxObject = 0;                          //現在選択しているオブジェクトの番号

	if (GetTotalStageManagerObjNum() <= 0)
	{//ステージマネージャー管理オブジェクトがなくなった瞬間に選択状態を解除する。
		m_bChooseObject = false;
		m_pChooseObject = nullptr;
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_L) == true && GetTotalStageManagerObjNum() > 0)
	{
		m_bChooseObject = m_bChooseObject ? false : true;

		if (m_bChooseObject == false)
		{
			m_pChooseObject->SetStageManagerChoose(false);     //選択しているオブジェクトの選択状態を解除
			m_Pos = m_SaveBeforeChoosePos;                     //保存していた位置に戻す
		}
		else
		{
			m_SaveBeforeChoosePos = m_Pos;                     //選択状態を発動する前のオブジェクトの位置を記憶する
			m_pChooseObject = GetCurStageManagerObject();      //最新のオブジェクトを選択
			m_pChooseObject->SetStageManagerChoose(false);     //選択しているオブジェクトの選択状態を解除
		}
	}
	
	if (m_pChooseObject != nullptr)
	{//選択オブジェクトが存在していたら
		if (m_bChooseObject == true)
		{
			//選択オブジェクト変更
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_N) == true)
			{
				m_pChooseObject->SetStageManagerChoose(false);//現在選んでいるオブジェクトの選択状態を解除

				if (m_pChooseObject->GetPrevStageManagerObject() != nullptr)
				{//前のオブジェクトがあったら
					m_pChooseObject = m_pChooseObject->GetPrevStageManagerObject();//前のオブジェクトを選択
				}
				else
				{//前のオブジェクトがない
					m_pChooseObject = GetCurStageManagerObject();//最新オブジェクトを選択
				}
			}
			else if (CManager::GetInputKeyboard()->GetTrigger(DIK_M) == true)
			{
				m_pChooseObject->SetStageManagerChoose(false);//現在選んでいるオブジェクトの選択状態を解除

				if (m_pChooseObject->GetNextStageManagerObject() != nullptr)
				{//次のオブジェクトがある
					m_pChooseObject = m_pChooseObject->GetNextStageManagerObject();//次のオブジェクトを選択
				}
				else
				{//次のオブジェクトがない
					m_pChooseObject = GetTopStageManagerObject();//トップオブジェクトを選択
				}
			}

			if (m_pChooseObject != nullptr && m_nIndexObject >= 1)
			{//オブジェクトを選択状態にする
				m_Pos = ((CObjectX*)m_pChooseObject)->GetPos();//選択オブジェクトの位置に移動する
				m_pChooseObject->SetStageManagerChoose(true);
			}

			if (CManager::GetInputKeyboard()->GetTrigger(DIK_DELETE) == true)
			{//選択オブジェクトを破棄する
				m_pChooseObject->SetUseDeath(true);
				m_pChooseObject->SetDeath();

				//選択オブジェクトを変更
				if (m_pChooseObject->GetNextStageManagerObject() != nullptr)
				{//次のオブジェクトがある
					m_pChooseObject = m_pChooseObject->GetNextStageManagerObject();//次のオブジェクトを選択
				}
				else
				{//次のオブジェクトがない
					m_pChooseObject = GetTopStageManagerObject();//トップオブジェクトを選択
				}
			}
		}
	}
}
//=======================================================================================================================

//===========================================================
//サイズ分動かすときの処理
//===========================================================
D3DXVECTOR3 CStageManager::SizeMoveProcess(float fMoveX, float fMoveY,D3DXVECTOR3 Size)
{
	D3DXVECTOR3 ResultSize = NULL_VECTOR3;

	ResultSize.x = float(floor(Size.x));
	ResultSize.y = float(floor(Size.y));
	ResultSize.z = float(floor(Size.z));

	D3DXVECTOR3 Move = NULL_VECTOR3;
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_DOWN) == true ||
		CManager::GetInputKeyboard()->GetTrigger(DIK_UP) == true ||
		CManager::GetInputKeyboard()->GetTrigger(DIK_RIGHT) == true ||
		CManager::GetInputKeyboard()->GetTrigger(DIK_LEFT) == true)
	{
		if (m_MoveMode == MOVEMODE00_XY)
		{
			if (fMoveX != 0.0f)
			{
				Move.x = sinf(atan2f(fMoveX, 0.0f)) * ResultSize.x;
			}
			else if (fMoveY != 0.0f)
			{
				Move.y = cosf(atan2f(0.0f, fMoveY)) * ResultSize.x;
			}
		}
	}


	return Move;
}
//=======================================================================================================================

//===========================================================
//拡大率をリセットする処理
//===========================================================
void CStageManager::ResetScale()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_V) == true)
	{
		m_Scale = ONE_VECTOR3;
	}
}
//=======================================================================================================================
