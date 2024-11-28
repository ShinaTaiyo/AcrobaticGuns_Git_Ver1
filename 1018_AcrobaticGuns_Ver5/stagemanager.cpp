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
#include "block.h"
#include "input.h"
#include "manager.h"
#include "object.h"
#include "camera.h"
#include "calculation.h"
#include "debugproc.h"
#include "bgModel.h"
#include <stdio.h>
#include <string.h>
//==========================================================

//================================
//静的メンバ宣言
//================================
const char* CStageManager::m_apWORLDMAP_TXT[static_cast<int>(CStageManager::WORLDTYPE::MAX)] =
{
	"data\\TEXTFILE\\Map\\EasyMap.txt",
	"data\\TEXTFILE\\Map\\NormalMap.txt",
	"data\\TEXTFILE\\Map\\BossRushMap.txt",
};

const string CStageManager::m_aSAVE_FILENAME = "data\\TEXTFILE\\Ver2\\Practice.txt";

//================================
//コンストラクタ
//================================
CStageManager::CStageManager(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri,bUseintPri,type,ObjType),
m_nWorldIndex(0),m_pBg3D(nullptr), m_StgObjList(),m_SaveScale(D3DXVECTOR3(1.0f,1.0f,1.0f)),m_SavePos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_SaveRot(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_SaveBeforeChoosePos(D3DXVECTOR3(0.0f,0.0f,0.0f)), m_nMapIndex(0), m_nMapNum(0), m_pChooseObject(nullptr),
m_ManagerMode(MANAGERMODE::ALREADYSTAGE),m_bChooseObject(false),m_bMakeMapMode(false),m_bUseSizeMove(false),m_StgObjIt(),m_pState(nullptr)
{

}
//==========================================================

//================================
//デストラクタ
//================================
CStageManager::~CStageManager()
{
	m_StgObjList.clear();//vectorの中身をクリア（忘れたとき対策）
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

	//=========================
	//選択系
	//=========================
	m_pChooseObject = nullptr;                          //選択オブジェクトへの
	//=======================================================================================
#ifdef _DEBUG
#endif // _DEBUG

	//===========================
	//移動モードを決める
	//===========================
	m_bUseSizeMove = false;//現在のオブジェクトのサイズ分移動するかどうか
	//=======================================================================================

	m_pState = DBG_NEW CStageManagerState_NewObject();

	//===========================
	//オブジェクトリスト
	//===========================
	//m_VecObjList = {};//オブジェクトのVector型のリスト
	//=======================================================================================

	////===========================
	////オブジェクトX初期化処理
	////===========================
	//if (CScene::GetMode() == CScene::MODE_EDIT)
	//{
	//	m_pManagerObject = CBlock::Create(CBlock::BLOCKTYPE00_NORMAL, 1, D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(1.0f,1.0f,1.0f));
	//	m_pManagerObject->SetUseDeath(false);//死亡フラグをオフにする
	//}
	////=======================================================================================
	CObject::Init();

	LoadMapTxt(0);
	return S_OK;
}
//==========================================================

//================================
//終了処理
//================================
void CStageManager::Uninit()
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
		//マップを変える処理
		MapChenge();

		//処理
		if (m_pState != nullptr)
		{
			m_pState->Process(this);
		}
		//===========================================================================================

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
	m_StgObjList.clear();//vectorの中身をクリア

	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	CObject::SetDeath();
}
//============================================================================================================

//============================================
//マップをロードする処理
//============================================
void CStageManager::LoadMapTxt(int nMapNum)
{

	//vectorに保存した情報をリセットする
	m_StgObjList.clear();

	fstream ReadingFile;//読み取り用ファイル
	string Reading_Buff;//読み取り用ファイルの文字列

	ReadingFile.open(m_aSAVE_FILENAME, ios::in);//読み取りモードで開く

	while (!ReadingFile.eof())
	{
		ReadingFile >> Reading_Buff;

		if (Reading_Buff == "SETBLOCK")
		{
			CBlock::LoadInfoTxt(ReadingFile, m_StgObjList,Reading_Buff);
		}
		else if (Reading_Buff == "SETBGMODEL")
		{
			CBgModel::LoadInfoTxt(ReadingFile, m_StgObjList, Reading_Buff);
		}
	}

	ReadingFile.close();//ファイルを閉じる
}
//============================================================================================================

//===========================================
//マップのファイルパスをロードする
//===========================================
void CStageManager::LoadMapFilePass(WORLDTYPE type)
{
	//char aString[512] = {};
	////===============================================================================================

	//m_nMapNum = 0;//マップ総数を初期化

	//FILE* pFileTxt = nullptr;
	//pFileTxt = fopen(m_apWORLDMAP_TXT[type], "r");

	//if (pFileTxt != nullptr)
	//{
	//	while (1)
	//	{
	//		fscanf(pFileTxt, "%s", &aString[0]);
	//		if (aString[0] == '#')
	//		{
	//			fgets(&aString[0], 100, pFileTxt);//行をスキップ
	//		}
	//		else if (strcmp(&aString[0], "FILENAME") == 0)
	//		{
	//			fscanf(pFileTxt, "%s", &aString[0]);//イコール読み取り用
	//			fscanf(pFileTxt, "%s", &m_aMapFilePass[m_nMapNum][0]);//ファイル名読み取り用
	//			m_nMapNum++;
	//		}
	//		else if (strcmp(&aString[0], "END_SCRIPT") == 0)
	//		{
	//			fclose(pFileTxt);
	//			break;
	//		}
	//	}

	//}

}
//============================================================================================================

//============================================
//マップをセーブする処理
//============================================
void CStageManager::SaveMapTxt(int nMapNum)
{

	fstream WritingFile;    //ファイル
	string Writing_Buff;    //文字列
	CObject::TYPE Type = {};//オブジェクト種類

	WritingFile.open(m_aSAVE_FILENAME, ios::out);//読み取りモードでファイルを開く	

	//ファイルに情報を保存する
	for (list<CObject*>::iterator it = m_StgObjList.begin(); it != m_StgObjList.end();++it)
	{//末尾まで繰り返す
		if (&it != nullptr)
		{
			Type = (*it)->GetType();
			(*it)->SaveInfoTxt(WritingFile);

			WritingFile << endl << endl;//改行処理
		}
	}

	WritingFile.close();//ファイルを閉じる

	m_StgObjList.clear();//Vectorの中身をクリアする
	ReleaseAll();        //全ての死亡フラグを発動
}
//======================================================================================================================

//==========================================================
//マップをバイナリファイルから読み込む
//==========================================================
void CStageManager::LoadMapBin(int nMapNum)
{
	////======================================
	////変数宣言
	////======================================
	//D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//int nType = 0;            //オブジェクトXごとのタイプ
	//int nManagerType = 0;        //オブジェクトXのタイプ
	//int nRotType = 0;         //向きのタイプ
	//D3DXVECTOR3 move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//float fWidth = 0.0f;
	//float fHeight = 0.0f;
	//int nData = 0;
	//char aString[512] = {};
	//char aStringMap[100] = {};
	//CObject* pObject = nullptr;
	////===============================================================================================
	//StageManagerObjectReleaseAll();
	//FILE* pFile = nullptr;
	//pFile = fopen(&m_aMapFilePass[nMapNum][0], "rb");
	////pFile = fopen(&m_aMapFilePass[0][0], "rb");
	//if (pFile != nullptr)
	//{
	//	//==============================================
	//	//オブジェクトの情報を読み込む
	//	//==============================================
	//	fread(&m_nIndexObject, sizeof(int), 1, pFile);
	//	if (m_nIndexObject > 0)
	//	{
	//		for (int nCnt = 0; nCnt < m_nIndexObject; nCnt++)
	//		{//オブジェクトを設定
	//			fread(&nManagerType, sizeof(int), 1, pFile);         //Xオブジェクトのタイプを読み込む
	//			fread(&nType, sizeof(int), 1, pFile);             //Xオブジェクトごとのタイプを読み込む
	//			fread(&nRotType, sizeof(int), 1, pFile);          //向きの種類を読み込む
	//			fread(&Pos, sizeof(D3DXVECTOR3), 1, pFile);       //位置を読み込む
	//			fread(&Scale, sizeof(D3DXVECTOR3), 1, pFile);     //拡大率を読み込む
	//			fread(&Rot, sizeof(D3DXVECTOR3), 1, pFile);       //向きを読み込む

	//			//switch (nManagerType)
	//			//{
	//			//	case (int)(MANAGEROBJECT_BLOCK) :
	//			//		pObject = CBlock::Create(CBlock::BLOCKTYPE(nType), 10, Pos, Rot, Scale, (CObjectX::ROTTYPE)(nRotType), m_nSetSubType);//生成処理
	//			//		break;
	//			//		case (int)(MANAGEROBJECT_MODEL) :
	//			//			pObject = CModel::Create((CModel::MODELTYPE)(nType), Pos, Rot, D3DXVECTOR3(0.0f,0.0f,0.0f), Scale, (CObjectX::ROTTYPE)(nRotType));//生成処理
	//			//			break;
	//			//				case (int)(MANAGEROBJECT_ENEMY) :
	//			//					pObject = CEnemy::Create((CEnemy::ENEMYTYPE)(nType), 10, Pos, D3DXVECTOR3(0.0f,0.0f,0.0f), Scale,D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f));//生成処理
	//			//					break;
	//			//					case (int)(MANAGEROBJECT_ITEM) :
	//			//						pObject = CItem::Create((CItem::ITEMTYPE)(nType), Pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL);//生成処理
	//			//						break;
	//			//						case (int)(MANAGEROBJECT_MARKER) :
	//			//							pObject = CMarker::Create((CMarker::MARKERTYPE)(nType),Pos,Scale, D3DXVECTOR3(0.0f,0.0f,0.0f), 0);
	//			//							break;
	//			//							case (int)(MANAGEROBJECT_BOSS) :
	//			//								pObject = CBoss::SetCreateBoss((CBoss::BOSSTYPE)(nType),10, Pos, Scale);
	//			//								break;


	//			//				default:
	//			//					break;
	//			//}

	//			//if (pObject != nullptr)
	//			//{
	//			//	m_apObjectList[nCnt] = pObject;
	//			//}
	//		}
	//	}
	//	//========================================================================
	//	fclose(pFile);
	//}
}
	//======================================================================================================================

//==========================================================
//マップをバイナリファイルに書き出す
//==========================================================
void CStageManager::SaveMapBin()
{
	////======================================
	////変数宣言
	////======================================
	//D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//int nType = 0;             //オブジェクトXごとのタイプ
	//int nManagerType = 0;             //オブジェクトXのタイプ
	//D3DXVECTOR3 move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//float fWidth = 0.0f;
	//float fHeight = 0.0f;
	//FILE* pFile = fopen(SAVE_BIN, "wb");
	////===============================================================================================
	//if (pFile != nullptr)
	//{

	//	fwrite(&m_nIndexObject, sizeof(int),1, pFile);
	//	//==============================================
 //       //ブロックのXオブジェクトの情報を書き出す
 //       //==============================================
	//	for (int nCnt = 0; nCnt < m_nIndexObject; nCnt++)
	//	{//ブロックを設定
	//		if (m_apObjectList[nCnt] != nullptr)
	//		{
	//			nManagerType = ((CObjectX*)m_apObjectList[nCnt])->GetManagerType();  //Xオブジェクトのタイプを取得
	//			nType = (int)(((CObjectX*)m_apObjectList[nCnt])->GetTypeNum());       //Xオブジェクトごとのタイプを取得
	//			Pos = ((CObjectX*)m_apObjectList[nCnt])->GetPos();                    //位置を書き出す
	//			Scale = ((CObjectX*)m_apObjectList[nCnt])->GetScale();				//拡大率を書き出す
	//			Rot = ((CObjectX*)m_apObjectList[nCnt])->GetRot();					//向きを書き出す

	//			fwrite(&nManagerType, sizeof(int), 1, pFile);              //Xオブジェクトのタイプを書き出す
	//			fwrite(&nType, sizeof(int), 1, pFile);                  //Xオブジェクトごとのタイプを書き出す
	//			fwrite(&Pos, sizeof(D3DXVECTOR3), 1, pFile);            //位置を書き出す
	//			fwrite(&Scale, sizeof(D3DXVECTOR3), 1, pFile);          //拡大率を書き出す
	//			fwrite(&Rot, sizeof(D3DXVECTOR3), 1, pFile);            //向きを書き出す
	//		}
	//	}
	//	//========================================================================
	//	fclose(pFile);
	//}
}
//======================================================================================================================

//==========================================================
//マップを変更する
//==========================================================
void CStageManager::MapChenge()
{
	//===============================================================
    //モードを変える
    //===============================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F4) == true)
	{
	     SaveMapTxt(m_nMapIndex);//現在のマップを保存する

		if (m_ManagerMode == MANAGERMODE::ALREADYSTAGE)
		{
			m_ManagerMode = MANAGERMODE::NEWSTAGE;
		}
		else
		{
			m_ManagerMode = MANAGERMODE::ALREADYSTAGE;
		}


		LoadMapTxt(m_nMapIndex);
	}
	//=======================================================================================================

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F3) == true && m_ManagerMode == MANAGERMODE::ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//現在のマップ情報をセーブする
		m_nMapIndex++;
		if (m_nMapIndex >= m_nMapNum)
		{
			m_nMapIndex = 0;
		}

		LoadMapTxt(m_nMapIndex);//番号のマップを読み込む
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_F2) == true && m_ManagerMode == MANAGERMODE::ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//現在のマップ情報をセーブする
		m_nMapIndex--;
		if (m_nMapIndex < 0)
		{
			m_nMapIndex = m_nMapNum - 1;
		}
		LoadMapTxt(m_nMapIndex);//番号のマップを読み込む
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
			pStageManager->CObject::SetType(CObject::TYPE::STAGEMANAGER);                                                     //オブジェクトの種類を決める   
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
//
////=================================================
////選択オブジェクトの種類を変える
////=================================================
//void CStageManager::TypeChenge()
//{
//	bool bChengeFlag = false;
//	int nNumType = m_pManagerObject->GetManagerObjectType();
//
//	CObject::MANAGEROBJECTTYPE ManagerObjectType = CObject::MANAGEROBJECTTYPE_NONE;
//	//=====================================================================
//    //オブジェクトXの種類を変更する
//    //=====================================================================
//	if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
//	{
//		nNumType++;
//		bChengeFlag = true;//発動
//	}
//	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
//	{
//		nNumType--;
//		bChengeFlag = true;//発動
//	}
//
//	if (bChengeFlag == true)
//	{
//		if (nNumType >= CObject::MANAGEROBJECTTYPE_MAX)
//		{
//			nNumType = CObject::MANAGEROBJECTTYPE_NONE + 1;
//		}
//		if (nNumType < 0)
//		{
//			nNumType = CObject::MANAGEROBJECTTYPE_MAX - 1;
//		}
//
//		ManagerObjectType = CObject::MANAGEROBJECTTYPE(nNumType);
//
//		ChengeObject(ManagerObjectType);//オブジェクトの種類を変える
//
//	}
//	//=====================================================================================================================================
//
//	//=====================================================================
//	//オブジェクトごとの種類を変更する
//	//=====================================================================
//	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
//	{
//		m_pManagerObject = m_pManagerObject->ManagerChengeObject(true);
//	}
//	if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
//	{
//		m_pManagerObject = m_pManagerObject->ManagerChengeObject(false);
//	}
//	//=====================================================================================================================================
//}
////===========================================================================================

////==================================================
////Xオブジェクト設定処理
////==================================================
//void CStageManager::SetObjectX()
//{
//	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
//	{//オブジェクトをVectorの先頭に保存する
//		m_StgObjList.push_back(m_pManagerObject->ManagerSaveObject());
//	}
//
//}
////===========================================================================================

////==================================================
////Xオブジェクト消去処理
////==================================================
//void CStageManager::DeleteManagerObject()
//{
//	if (CManager::GetInputKeyboard()->GetTrigger(DIK_BACKSPACE) == true)
//	{
//		if (m_StgObjList.size() > 0)
//		{
//			auto it = m_StgObjList.end();//配列マックスー１
//
//			(*it)->SetUseDeath(true);
//			(*it)->SetDeath();
//
//			m_StgObjList.pop_back();//末尾の要素を削除する
//		}
//	}
//}
////=======================================================================================================================

////====================================================
////設置オブジェクトの種類を変える
////====================================================
//void CStageManager::ChengeObject(CObject::MANAGEROBJECTTYPE ManagerObjectType)
//{	
//	//オブジェクトを破棄する
//	ReleaseObject();
//
//	int nObjectType = ManagerObjectType;
//
//	if (nObjectType >= CObject::MANAGEROBJECTTYPE_MAX)
//	{
//		nObjectType = CObject::MANAGEROBJECTTYPE_NONE + 1;
//	}
//	else if (nObjectType < CObject::MANAGEROBJECTTYPE_NONE + 1)
//	{
//		nObjectType = CObject::MANAGEROBJECTTYPE_MAX - 1;
//	}
//
//	ManagerObjectType = MANAGEROBJECTTYPE(nObjectType);
//
//	//オブジェクトの種類を変える
//	switch (ManagerObjectType)
//	{
//	case  CObject::MANAGEROBJECTTYPE_BLOCK:
//		m_pManagerObject = CBlock::Create(CBlock::BLOCKTYPE00_NORMAL, 10, m_SavePos,m_SaveRot,m_SaveScale);
//		break;
//	case CObject::MANAGEROBJECTTYPE_BGMODEL:
//		m_pManagerObject = CBgModel::Create(CBgModel::BGMODELTYPE::BILL_00, m_SavePos, m_SaveRot, m_SaveScale);
//		break;
//		default:
//		break;
//	}
//	if (m_pManagerObject != nullptr)
//	{//今選んでいるオブジェクトだけは破棄されない
//		m_pManagerObject->SetUseDeath(false);
//	}
//}
////=======================================================================================================================

////==================================================================
////オブジェクトをリリースする
////==================================================================
//void CStageManager::ReleaseObject()
//{
//	//最低限の情報を保存する
//	switch (m_pManagerObject->GetObjectType())
//	{
//	case CObject::OBJECTTYPE::OBJECTTYPE_X:
//		m_SavePos = ((CObjectX*)m_pManagerObject)->GetPos();
//		m_SaveRot = ((CObjectX*)m_pManagerObject)->GetRot();
//		m_SaveScale = ((CObjectX*)m_pManagerObject)->GetScale();
//		break;
//	default:
//		break;
//	}
//	if (m_pManagerObject != nullptr)
//	{
//		m_pManagerObject->SetUseDeath(true);
//		m_pManagerObject->SetDeath();
//		m_pManagerObject = nullptr;
//	}
//
//}
////=======================================================================================================================

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

	if (m_ManagerMode == MANAGERMODE::ALREADYSTAGE)
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
	CManager::GetDebugProc()->PrintDebugProc("現在のステージマネージャー管理オブジェクトの数：%d\n", m_StgObjList.size());
	CManager::GetDebugProc()->PrintDebugProc("現在のワールド：%s\n",&m_apWORLDMAP_TXT[m_nWorldIndex][0]);
	CManager::GetDebugProc()->PrintDebugProc("現在のマップ番号(F2、F3で変更）：%d\n", m_nMapIndex);
	CManager::GetDebugProc()->PrintDebugProc("現在のマップモード（F4）：%s\n",&aMapModeString[0]);
	CManager::GetDebugProc()->PrintDebugProc("：末尾のオブジェクトを消去：BACKSPACE\n");
	CManager::GetDebugProc()->PrintDebugProc("//=================================\n");

}
//=======================================================================================================================

//===========================================================
//オブジェクト選択処理
//===========================================================
void CStageManager::ChooseObject()
{
	
}
//=======================================================================================================================

//===========================================================
//状態を変える
//===========================================================
void CStageManager::StateChenge(CStageManagerState* pStageManagerState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pStageManagerState;
}
//=======================================================================================================================

//===========================================================
//拡大率をリセットする処理
//===========================================================
void CStageManager::ResetScale()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_V) == true)
	{
		m_SaveScale = D3DXVECTOR3(1.0f,1.0f,1.0f);
	}
}
//=======================================================================================================================

//***********************************************************************************************************************
//新しいオブジェクト配置状態クラス
//***********************************************************************************************************************

//===========================================================
//コンストラクタ
//===========================================================
CStageManagerState_NewObject::CStageManagerState_NewObject() : m_pManagerObject(nullptr)
{
	m_pManagerObject = CBlock::Create(CBlock::BLOCKTYPE00_NORMAL, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pManagerObject->SetUseDeath(false);
}
//=======================================================================================================================

//===========================================================
//デストラクタ
//===========================================================
CStageManagerState_NewObject::~CStageManagerState_NewObject()
{
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(true);
		m_pManagerObject->SetDeath();
		m_pManagerObject = nullptr;
	}
}
//=======================================================================================================================

//===========================================================
//処理
//===========================================================
void CStageManagerState_NewObject::Process(CStageManager* pStageManager)
{
	//ステージマネージャーから情報にアクセスして操作する
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->ManagerChooseControlInfo();

		//現在のブロックの種類を変更する
		TypeChenge(pStageManager);

		//ブロックを設定する
		SetObjectX(pStageManager);

		//オブジェクトを消す
		DeleteManagerObject(pStageManager);
	}
}
//=======================================================================================================================

//===========================================================
//オブジェクトの種類を変える
//===========================================================
void CStageManagerState_NewObject::TypeChenge(CStageManager* pStageManager)
{
	CObject* pObj = nullptr;
	//=====================================================================
	//オブジェクトごとの種類を変更する
	//=====================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{
		pObj = m_pManagerObject->ManagerChengeObject(true);
		m_pManagerObject = pObj;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
	{
		pObj = m_pManagerObject->ManagerChengeObject(false);
		m_pManagerObject = pObj;
	}
	//=====================================================================================================================================

	//オブジェクトごとのタイプを変える
	ChengeObject(pStageManager);
}
//=======================================================================================================================

//===========================================================
//オブジェクトを設定する
//===========================================================
void CStageManagerState_NewObject::SetObjectX(CStageManager* pStageManager)
{
	list <CObject*>& StgObjList = pStageManager->GetStgObjList();
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{//オブジェクトをVectorの先頭に保存する
		StgObjList.push_back(m_pManagerObject->ManagerSaveObject());
	}
}
//=======================================================================================================================

//===========================================================
//末尾のオブジェクトを消去する
//===========================================================
void CStageManagerState_NewObject::DeleteManagerObject(CStageManager* pStageManager)
{
	list <CObject*>& StgObjList = pStageManager->GetStgObjList();

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_BACKSPACE) == true)
	{
		if (StgObjList.size() > 0)
		{
			auto it = StgObjList.end()--;//配列マックスー１

			(*it)->SetUseDeath(true);
			(*it)->SetDeath();

			StgObjList.pop_back();//末尾の要素を削除する
		}
	}
}
//=======================================================================================================================

//===========================================================
//オブジェクトの変更を行う
//===========================================================
void CStageManagerState_NewObject::ChengeObject(CStageManager* pStageManager)
{
	bool bChengeFlag = false;
	int nNumType = m_pManagerObject->GetManagerObjectType();
	CObject::MANAGEROBJECTTYPE ManagerObjectType = CObject::MANAGEROBJECTTYPE_NONE;

	//=====================================================================
	//オブジェクトXの種類を変更する
	//=====================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
	{
		nNumType++;
		bChengeFlag = true;//発動
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{
		nNumType--;
		bChengeFlag = true;//発動
	}

	if (bChengeFlag == true)
	{
		ReleaseObject(pStageManager);
		if (nNumType >= CObject::MANAGEROBJECTTYPE_MAX)
		{
			nNumType = CObject::MANAGEROBJECTTYPE_NONE + 1;
		}
		if (nNumType < 0)
		{
			nNumType = CObject::MANAGEROBJECTTYPE_MAX - 1;
		}

		ManagerObjectType = CObject::MANAGEROBJECTTYPE(nNumType);
	}
	//=====================================================================================================================================

	//オブジェクトの種類を変える
	switch (ManagerObjectType)
	{
	case  CObject::MANAGEROBJECTTYPE_BLOCK:
		m_pManagerObject = CBlock::Create(CBlock::BLOCKTYPE00_NORMAL, 10, pStageManager->GetSavePos(),pStageManager->GetSaveRot(),pStageManager->GetSaveScale());
		break;
	case CObject::MANAGEROBJECTTYPE_BGMODEL:
		m_pManagerObject = CBgModel::Create(CBgModel::BGMODELTYPE::BILL_00, pStageManager->GetSavePos(), pStageManager->GetSaveRot(), pStageManager->GetSaveScale());
		break;
	default:
		break;
	}
	if (m_pManagerObject != nullptr)
	{//今選んでいるオブジェクトだけは破棄されない
		m_pManagerObject->SetUseDeath(false);
	}
}
//=======================================================================================================================

//===========================================================
//末尾のオブジェクトの破棄を行う
//===========================================================
void CStageManagerState_NewObject::ReleaseObject(CStageManager* pStageManager)
{
	//最低限の情報を保存する
	switch (m_pManagerObject->GetObjectType())
	{
	case CObject::OBJECTTYPE::OBJECTTYPE_X:
		pStageManager->SetSavePos(static_cast<CObjectX*>(m_pManagerObject)->GetPos());
		pStageManager->SetSaveRot(static_cast<CObjectX*>(m_pManagerObject)->GetRot());
		pStageManager->SetSaveScale(static_cast<CObjectX*>(m_pManagerObject)->GetScale());
		break;
	default:
		break;
	}
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(true);
		m_pManagerObject->SetDeath();
		m_pManagerObject = nullptr;
	}
}
//=======================================================================================================================