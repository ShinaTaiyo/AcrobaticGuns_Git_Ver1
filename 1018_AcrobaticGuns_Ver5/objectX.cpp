//===========================================================================================
//
//６月４日：モデルを描画する[objectX.cpp]
//Author:ShinaTaiyo
//
//===========================================================================================

//===============================
//インクルード
//===============================
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "objectX.h"
#include "calculation.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "debugproc.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
//===========================================================================================

//================================================
//コンストラクタ
//================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority), m_bUseDraw(true),m_ObjectXInfo(),m_bColorChenge(false),m_bUseAddRot(false),
m_bUseAddScaling(false),m_bUseShadow(false),m_nChengeColorTime(0),m_nIndexObjectX(0),m_nManagerType(0),m_nObjXType(OBJECTXTYPE_BLOCK), 
m_nTypeNum(0), m_bUseMultiScale(false), m_MultiScale(NULL_VECTOR3),m_bUseCulling(false), m_Pos(NULL_VECTOR3), m_SupportPos(NULL_VECTOR3),
m_PosOld(NULL_VECTOR3),m_Rot(NULL_VECTOR3),m_Scale(NULL_VECTOR3),m_FormarScale(NULL_VECTOR3),m_Size(NULL_VECTOR3),m_VtxMin(NULL_VECTOR3),
m_OriginVtxMin(NULL_VECTOR3),m_VtxMax(NULL_VECTOR3),m_OriginVtxMax(NULL_VECTOR3),m_mtxWorld(),m_AddRot(NULL_VECTOR3),m_SenterPos(NULL_VECTOR3),
m_AddScale(NULL_VECTOR3)
{
	SetObjectType(CObject::OBJECTTYPE::OBJECTTYPE_X);
}
//================================================================================================================================================

//================================================
//デストラクタ
//================================================
CObjectX::~CObjectX()
{

}
//================================================================================================================================================

//================================================
//初期化処理
//================================================
HRESULT CObjectX::Init()
{
	//==========================
	//変数の初期化
	//==========================
	m_mtxWorld = {};                                   //ワールドマトリックス
	//==================================================================================

	//===========================================
	//影関係
	//===========================================
	m_bUseShadow = true;
	//==================================================================================

	m_bUseAddScaling = false;                 //拡大率の加算を使用するかどうか
	m_AddScale = NULL_VECTOR3;                //拡大率の加算量    

	CObject::Init();

	return S_OK;
}
//================================================================================================================================================

//================================================
//終了処理
//================================================
void CObjectX::Uninit()
{
	if (m_ObjectXInfo.Diffuse != nullptr)
	{
		delete[] m_ObjectXInfo.Diffuse;//色合いの動的メモリを開放
		m_ObjectXInfo.Diffuse = nullptr;
	}

	if (m_ObjectXInfo.pTexture != nullptr)
	{
		delete[] m_ObjectXInfo.pTexture;//テクスチャの動的メモリを解放
		m_ObjectXInfo.pTexture = nullptr;
	}

	if (m_ObjectXInfo.FormarDiffuse != nullptr)
	{
		delete[] m_ObjectXInfo.FormarDiffuse;//色合いの動的メモリを開放
		m_ObjectXInfo.FormarDiffuse = nullptr;
	}

	if (m_ObjectXInfo.pMesh != nullptr)
	{//メッシュへのポインタをnullptrに
		m_ObjectXInfo.pMesh = nullptr;
	}

	if (m_ObjectXInfo.pBuffMat != nullptr)
	{//マテリアルへのポインタをnullptrに
		m_ObjectXInfo.pBuffMat = nullptr;
	}

	CObject::Uninit();
}
//================================================================================================================================================

//================================================
//更新処理
//================================================
void CObjectX::Update()
{
	//==============================================
	//中心点を求める
	//==============================================
	CalculateSenterPos();

	//==================================
	//拡大率の加算がONになっていたら
	//==================================
	if (m_bUseAddScaling == true)
	{
		m_Scale += m_AddScale;
	}
	//==========================================================

	//==================================
	//拡大率の乗算がONになっていたら
	//==================================
	if (m_bUseMultiScale == true)
	{
		m_Scale.x *= m_MultiScale.x;
		m_Scale.y *= m_MultiScale.y;
		m_Scale.z *= m_MultiScale.z;
	}
	//==========================================================

	//常に拡大率を参照して最大最小頂点を求める
	m_VtxMax.x = m_OriginVtxMax.x * m_Scale.x;
	m_VtxMax.y = m_OriginVtxMax.y * m_Scale.y;
	m_VtxMax.z = m_OriginVtxMax.z * m_Scale.z;
	m_VtxMin.x = m_OriginVtxMin.x * m_Scale.x;
	m_VtxMin.y = m_OriginVtxMin.y * m_Scale.y;
	m_VtxMin.z = m_OriginVtxMin.z * m_Scale.z;

	m_Size = m_VtxMax - m_VtxMin;

	if (m_bColorChenge == true)
	{
		m_nChengeColorTime--;
	}

	if (m_nChengeColorTime <= 0 && m_bColorChenge == true )
	{
		m_nChengeColorTime = 0;
		SetFormarColor();//元の色合いに戻す
		m_bColorChenge = false;
	}

	if (m_bUseAddRot == true)
	{//向きの加算処理
		m_Rot += m_AddRot;
	}

	if (GetStageManagerChoose() == true)
	{//オブジェクトXの情報を表示

	}

	CObject::Update();
}
//================================================================================================================================================

//================================================
//描画処理
//================================================
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //デバイスへのポインタと取得
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;                  //計算用マトリックス
	D3DMATERIAL9 matDef;                                              //現在のマテリアル保存用
	D3DXMATERIAL* pMat;                                               //マテリアルデータへのポインタ

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//大きさを反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_ObjectXInfo.Diffuse[0].a > 0.0f && m_Pos.y + m_VtxMax.y >= 0.0f && m_bUseShadow == true)
	{
		//影の描画
		DrawShadow();
	}
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//=======================================
	//描画の調整
	//=======================================
	//アルファテストを有効(アルファ値が０より大きい場合に描画する）
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	if (m_bUseCulling == true)
	{
		//両面を描画する
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
	//================================================================================================================

	//マテリアルへのポインタを取得
	pMat = (D3DXMATERIAL*)m_ObjectXInfo.pBuffMat->GetBufferPointer();

	if (m_bUseDraw == true)
	{
		//==========================================================================
		//マテリアルの数分、テクスチャを読み込む。
		//==========================================================================
		for (int nCntMat = 0; nCntMat < (int)m_ObjectXInfo.dwNumMat; nCntMat++)
		{
			//色合いの設定
			pMat[nCntMat].MatD3D.Diffuse = m_ObjectXInfo.Diffuse[nCntMat];

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャを設定する
			pDevice->SetTexture(0, m_ObjectXInfo.pTexture[nCntMat]);

			//モデル（パーツ）の描画
			m_ObjectXInfo.pMesh->DrawSubset(nCntMat);
		}
		//================================================================================================================
	}
	
	//=======================================
	//描画の調整を元に戻す
	//=======================================
	//アルファテスト無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);


	//片面だけ描画する
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//================================================================================================================

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	CObject::Draw();

}
//================================================================================================================================================

//==========================================================================
//拡大率の加算を設定
//==========================================================================
void CObjectX::SetUseAddScale(D3DXVECTOR3 AddScale, bool bUse)
{
	m_bUseAddScaling = bUse;
	m_AddScale = AddScale;
}
//===================================================================================================================================================

//================================================
//モデル情報を割り当てる
//================================================
void CObjectX::BindObjectXInfo(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPDIRECT3DTEXTURE9* pTexture, D3DCOLORVALUE* pDiffuse)
{
	m_ObjectXInfo.pMesh = pMesh;
	m_ObjectXInfo.pBuffMat = pBuffMat;
	m_ObjectXInfo.dwNumMat = dwNumMat;
	m_ObjectXInfo.pTexture = DBG_NEW LPDIRECT3DTEXTURE9[int(dwNumMat)];
	m_ObjectXInfo.Diffuse = DBG_NEW D3DCOLORVALUE[int(dwNumMat)];
	m_ObjectXInfo.FormarDiffuse = DBG_NEW D3DCOLORVALUE[int(dwNumMat)];
	for (int nCnt = 0; nCnt < (int)(dwNumMat); nCnt++)
	{
		m_ObjectXInfo.pTexture[nCnt] = pTexture[nCnt];
		m_ObjectXInfo.Diffuse[nCnt] = pDiffuse[nCnt];
		m_ObjectXInfo.FormarDiffuse[nCnt] = pDiffuse[nCnt];
	}
}
//================================================================================================================================================

//================================================
//色合いを設定する
//================================================
void CObjectX::SetColor(D3DXCOLOR col, int nColChengeTime, bool bChoose, bool bSetAlpha)
{
	m_bColorChenge = true;
	m_nChengeColorTime = nColChengeTime;

	if (bChoose == true)
	{
		if (bSetAlpha == false)
		{
			for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
			{
				m_ObjectXInfo.Diffuse[nCnt].r = col.r;
				m_ObjectXInfo.Diffuse[nCnt].g = col.g;
				m_ObjectXInfo.Diffuse[nCnt].b = col.b;
			}
		}
		else
		{
			for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
			{
				m_ObjectXInfo.Diffuse[nCnt].a = col.a;
			}
		}
	}
	else
	{
		for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
		{
			m_ObjectXInfo.Diffuse[nCnt] = col;
		}
	}
}
//================================================================================================================================================

//================================================
//中心点を求める
//================================================
void CObjectX::CalculateSenterPos()
{
	m_SenterPos = m_Pos;
	m_SenterPos.y += (m_VtxMax.y + m_VtxMin.y) / 2;
}
//================================================================================================================================================

//================================================
//元の色合いに戻す
//================================================
void CObjectX::SetFormarColor()
{
	for (int nCnt = 0; nCnt < (int)(m_ObjectXInfo.dwNumMat); nCnt++)
	{
		m_ObjectXInfo.Diffuse[nCnt] = m_ObjectXInfo.FormarDiffuse[nCnt];
	}
}
//================================================================================================================================================

//================================================
//サイズを設定する
//================================================
void CObjectX::SetSize()
{
	BYTE* pVtxBuff;//頂点バッファへのポインタ

	//頂点数の取得
	int nNumVtx = m_ObjectXInfo.pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	DWORD sizeFVF = D3DXGetFVFVertexSize(m_ObjectXInfo.pMesh->GetFVF());

	//頂点バッファのロック
    m_ObjectXInfo.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//====================================================
		//頂点座標を比較してモデルの最小値最大値を取得
		//====================================================
		if (vtx.x > m_OriginVtxMax.x)
		{
			m_OriginVtxMax.x = vtx.x;
		}
		else if (vtx.y > m_OriginVtxMax.y)
		{//今回読み込んだ頂点が、一番大きい頂点より大きい場合
			m_OriginVtxMax.y = vtx.y;
		}
		else if (vtx.z > m_OriginVtxMax.z)
		{//今回読み込んだ頂点が、一番大きい頂点より大きい場合
			m_OriginVtxMax.z = vtx.z;
		}
		else if (vtx.x < m_OriginVtxMin.x)
		{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
			m_OriginVtxMin.x = vtx.x;
		}
		else if (vtx.y < m_OriginVtxMin.y)
		{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
			m_OriginVtxMin.y = vtx.y;
		}
		else if (vtx.z < m_OriginVtxMin.z)
		{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
			m_OriginVtxMin.z = vtx.z;
		}
		//=============================================================================================================

		//頂点フォーマットのサイズ文ポインタを進める
		pVtxBuff += sizeFVF;

	}
	//頂点バッファのアンロック
	m_ObjectXInfo.pMesh->UnlockVertexBuffer();

	//==========================================================
	//上の処理で出した最小最大の頂点を拡大率で計算し直す
	//==========================================================
	m_VtxMax.x = m_OriginVtxMax.x * m_Scale.x;
	m_VtxMax.y = m_OriginVtxMax.y * m_Scale.y;
	m_VtxMax.z = m_OriginVtxMax.z * m_Scale.z;
	m_VtxMin.x = m_OriginVtxMin.x * m_Scale.x;
	m_VtxMin.y = m_OriginVtxMin.y * m_Scale.y;
	m_VtxMin.z = m_OriginVtxMin.z * m_Scale.z;
	//================================================================================================================================================

	//==========================================================
	//最大最小を参照してサイズを設定する
	//==========================================================
	m_Size = m_VtxMax - m_VtxMin;
	//================================================================================================================================================

}
//================================================================================================================================================

//===================================================================================================================
//エディタから拡大率を変更する
//===================================================================================================================
void CObjectX::ChengeEditScale()
{
	D3DXVECTOR3& Scale = GetScale();//拡大率

	ChengeEditScaleX();

	ChengeEditScaleY();

	ChengeEditScaleZ();

	CManager::GetDebugProc()->PrintDebugProc("拡大率(RTYキー) %f %f %f\n", Scale.x,Scale.y,Scale.z);
}
//================================================================================================================================================

//===================================================================================================================
//エディタから拡大率Xを変更する
//===================================================================================================================
void CObjectX::ChengeEditScaleX()
{
	D3DXVECTOR3& Scale = GetScale();                                //拡大率

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_R) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_R) == true)
		{
			Scale.x += 0.1f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
			{
				Scale.x -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
		{
			Scale.x += 0.1f;
		}
	}

}
//================================================================================================================================================

//===================================================================================================================
//エディタから拡大率Yを変更する
//===================================================================================================================
void CObjectX::ChengeEditScaleY()
{
	D3DXVECTOR3& Scale = GetScale();                                //拡大率

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_T) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_T) == true)
		{
			Scale.y += 0.1f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
			{
				Scale.y -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
		{
			Scale.y += 0.1f;
		}
	}
}
//================================================================================================================================================

//===================================================================================================================
//エディタから拡大率Zを変更する
//===================================================================================================================
void CObjectX::ChengeEditScaleZ()
{
	D3DXVECTOR3& Scale = GetScale();                                //拡大率
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//Lコントロールキーを押しながら
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
		{
			Scale.z += 0.1f;
		}
	}
	else
	{//Lコントロールキーを押していない
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//シフトキーを押しながら・・・
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
			{
				Scale.z -= 0.1f;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
		{
			Scale.z += 0.1f;
		}
	}

}
//================================================================================================================================================

//===================================================================================================================
//位置を移動させる
//===================================================================================================================
void CObjectX::ChengeEditPos()
{
	float fMoveX = 0.0f;                                            //X方向の移動量
	float fMoveZ = 0.0f;                                            //Z方向の移動量
	bool bMove = false;                                             //移動しているかどうか 
	D3DXVECTOR3 Move = NULL_VECTOR3;
	SetColor(D3DXCOLOR(1.0f,0.0f,0.0f,0.5f),3,true,true);           //色を半透明にする

	//===========================
	//位置を支点に固定
	//===========================
	m_Pos = m_SupportPos;
	//========================================================================================

	//===========================
	//位置を変更
	//===========================
	CCalculation::CaluclationMove(Move, 5.0f, CCalculation::MOVEAIM_XZ, m_Rot.y);

	////===========================
	////RTボタンを押していたら
	////===========================
	//if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	//{//Lコントロールキーを押しながら
	//	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	//	{//シフトキーを押しながら・・・
	//		if (CManager::GetInputKeyboard()->GetPress(DIK_F) == true)
	//		{
	//			m_Rot.z -= 0.01f;
	//		}
	//	}
	//	else if (CManager::GetInputKeyboard()->GetPress(DIK_G) == true)
	//	{
	//		m_Rot.z += 0.01f;
	//	}
	//}
	//else
	//{//Lコントロールキーを押していない
	//	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	//	{//シフトキーを押しながら・・・
	//		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F) == true)
	//		{
	//			m_Rot.z -= 0.01f;
	//		}
	//	}
	//	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_G) == true)
	//	{
	//		m_Rot.z += 0.01f;
	//	}
	//}

	//支点も一緒に移動
	m_Pos += Move;
	m_SupportPos = m_Pos;
	CManager::GetDebugProc()->PrintDebugProc("支点位置(矢印キー) %f %f %f\n", m_SupportPos.x, m_SupportPos.y, m_SupportPos.z);
	CManager::GetDebugProc()->PrintDebugProc("向きZ(FGキー) %f\n", m_Rot.z);
	//================================================================================================================================================

}
//================================================================================================================================================

//============================================================================
//ステージマネージャーが情報を操作する
//============================================================================
void CObjectX::ManagerChooseControlInfo()
{
	ChengeEditPos();//位置を変える

	ChengeEditScale();//拡大率を変える
}
//================================================================================================================================================

//============================================================================
//テキストファイルに情報を保存する
//============================================================================
void CObjectX::SaveInfoTxt(fstream & WritingFile)
{
	WritingFile << "POS = " << fixed << setprecision(3)<< m_Pos.x << " " << 
		fixed << setprecision(3) << m_Pos.y << " " << 
		fixed << setprecision(3) << m_Pos.z << " " << endl;//位置
	WritingFile << "ROT = " << fixed << setprecision(3) << m_Rot.x << " " <<
		fixed << setprecision(3) << m_Rot.y << " " <<
		fixed << setprecision(3) << m_Rot.z << " " << endl;//向き
	WritingFile << "SCALE = " << fixed << setprecision(3) << m_Scale.x << " " <<
		fixed << setprecision(3) << m_Scale.y << " " <<
		fixed << setprecision(3) << m_Scale.z << " " << endl;//拡大率
}
//================================================================================================================================================

//============================================================================
//狙ったマテリアル番号のテクスチャを割り当てる
//============================================================================
void CObjectX::SetLockOnMatBindTexture(int nNumMat, LPDIRECT3DTEXTURE9 pTexture)
{
	m_ObjectXInfo.pTexture[nNumMat] = pTexture;
}
//================================================================================================================================================

//============================================================================
//影を描画する
//============================================================================
void CObjectX::DrawShadow()
{
	D3DXMATRIX mtxShadow,mtxScale,mtxRot,mtxTrans;                  //計算用マトリックス
	D3DXPLANE plane;                       //プレーン
	D3DXVECTOR4 vecLight;                  //ライト逆方向
	D3DXVECTOR3 ShadowPos, ShadowNor;      //平面上の一転と
	D3DXMATERIAL* pMat;                                               //マテリアルデータへのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //デバイスへのポインタと取得
	D3DMATERIAL9 matDef;                                              //現在のマテリアル保存用

    //影のマトリックスを初期化
	D3DXMatrixIdentity(&mtxShadow);

	//ライトの逆方向ベクトルを設定
	vecLight = D3DXVECTOR4(-100.0f, 300.0f,300.0f, 0.0f);
	ShadowPos = NULL_VECTOR3;
	ShadowNor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//法線と平面上の一点から平面情報を作成
	D3DXPlaneFromPointNormal(&plane, &ShadowPos, &ShadowNor);

	//ライトの平面情報から影行列を作成
	D3DXMatrixShadow(&mtxShadow, &vecLight, &plane);

	//大きさを反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxScale);

	////向きを反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot,m_Rot.y, m_Rot.x, m_Rot.z);
	//D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x,0.0f, m_Pos.z);
	D3DXMatrixMultiply(&mtxShadow, &mtxShadow, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//=======================================
    //描画の調整
    //=======================================

    //法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	//================================================================================================================

	//マテリアルへのポインタを取得
	pMat = (D3DXMATERIAL*)m_ObjectXInfo.pBuffMat->GetBufferPointer();

	//==========================================================================
	//マテリアルの数分、テクスチャを読み込む。
	//==========================================================================
	for (int nCntMat = 0; nCntMat < (int)m_ObjectXInfo.dwNumMat; nCntMat++)
	{
		//色合いの設定
		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャを設定する
		pDevice->SetTexture(0,NULL);

		//モデル（パーツ）の描画
		m_ObjectXInfo.pMesh->DrawSubset(nCntMat);
	}
	//================================================================================================================

	//=======================================
	//描画の調整を元に戻す
	//=======================================

	//法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	//================================================================================================================

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
//================================================================================================================================================