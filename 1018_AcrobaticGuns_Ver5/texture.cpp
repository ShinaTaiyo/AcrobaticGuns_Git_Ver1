//==================================================================================================
//
//６月６日：テクスチャ管理クラスを作成する[texture.cpp]
//Author:ShinaTaiyo
//
//==================================================================================================

//=====================================================
//インクルード
//=====================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include <iostream>
//==================================================================================================

//=====================================================
//静的メンバ初期化
//=====================================================
int CTexture::m_nNumAll = 0;//テクスチャ総数

//=====================================================
//コンストラクタ
//=====================================================
CTexture::CTexture() : m_pTextureList(),m_aTextureName()
{
	//=====================================================
	//テクスチャの名前の初期化
	//=====================================================
	for (int nCntObjType = 0; nCntObjType < m_nMAXTEXTURE; nCntObjType++)
	{
		m_aTextureName[nCntObjType] = {};
	}
	//==============================================================================================

	//=====================================================
	//テクスチャリストの初期化
	//=====================================================
	for (int nCntTexture = 0; nCntTexture < m_nMAXTEXTURE; nCntTexture++)
	{
		m_pTextureList[nCntTexture] = {};
	}
	//==================================================================================================
}
//==================================================================================================

//=====================================================
//デストラクタ
//=====================================================
CTexture::~CTexture()
{

}
//==================================================================================================

//=====================================================
//テクスチャのアンロード処理
//=====================================================
void CTexture::Unload()
{
	for (int nCntTexture = 0; nCntTexture < m_nMAXTEXTURE; nCntTexture++)
	{
		if (m_pTextureList[nCntTexture] != nullptr)
		{
			m_pTextureList[nCntTexture]->Release();
			m_pTextureList[nCntTexture] = nullptr;
		}
	}
}
//==================================================================================================

//=====================================================
//テクスチャの登録処理
//=====================================================
int CTexture::Regist(std::string pTextureName)
{
	int nIdx = 0;//テクスチャ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスの取得
	for (int nCnt = 0; nCnt < m_nMAXTEXTURE; nCnt++)
	{
		if (m_pTextureList[nCnt] == nullptr)
		{//まだ登録されていない場合
			D3DXCreateTextureFromFile(pDevice,
				pTextureName.c_str(),
				&m_pTextureList[nCnt]);

			m_aTextureName[nCnt] = pTextureName;
			nIdx = nCnt;//ID設定
			m_nNumAll++;//テクスチャ総数
			break;
		}
		else if (m_aTextureName[nCnt] == pTextureName)
		{//既に生成されているテクスチャのファイルパスと一致した
			nIdx = nCnt;//ID設定（同じパスを使いまわす)
			break;
		}
	}

	return nIdx;//IDを返す
}
//==================================================================================================

//=====================================================
//テクスチャの取得処理
//=====================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (m_pTextureList[nIdx] != nullptr)
	{//テクスチャファイルが存在していたら
		return m_pTextureList[nIdx];
	}

	return nullptr;
}
//==================================================================================================