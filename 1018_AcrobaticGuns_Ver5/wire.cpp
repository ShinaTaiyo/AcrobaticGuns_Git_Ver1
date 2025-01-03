//===================================================================================================================
//
//１１月１７日：ワイヤーを作る[wire.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================================

//===============================================================
//インクルード
//===============================================================
#include "wire.h"
#include "texture.h"
#include "renderer.h"
#include "player.h"
#include "manager.h"
#include "particle.h"
#include "debugproc.h"
#include "calculation.h"
//===================================================================================================================

//===============================================================
//デストラクタ
//===============================================================
const string CWire::s_WIRE_FILENAME[static_cast<int>(CWire::WIRETYPE::MAX)] =
{
	"data\\TEXTURE\\Wire\\LakeField_000.png"
};
//===================================================================================================================

//===============================================================
//コンストラクタ
//===============================================================
CWire::CWire(WIRETYPE WireType, float fRadius, float fHeight,int nNumDivsionXZ, 
	D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, 
	int nNumDivisionY, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CMeshCylinder(fRadius,fHeight,nNumDivsionXZ,nNumDivisionY,
		Pos,Rot,
		nPri,bUseintPri,type,ObjType),m_Type(WireType),m_bUseUpdate(true),m_pWireHead(nullptr),m_pPlayer(nullptr),m_VecMtxCircle()
{
	for (int nCnt = 0; nCnt < nNumDivsionXZ + 1; nCnt++)
	{
		CirclePosInfo Info = { D3DXVECTOR3(0.0f,0.0f,0.0f),{} };
		m_VecMtxCircle.push_back(Info);
	}
}
//===================================================================================================================

//===============================================================
//デストラクタ
//===============================================================
CWire::~CWire()
{
	m_VecMtxCircle.clear();        //メモリ初期化
	m_VecMtxCircle.shrink_to_fit();//メモリ開放
}
//===================================================================================================================

//===============================================================
//初期化処理
//===============================================================
HRESULT CWire::Init()
{
	CMeshCylinder::Init();

	//ワイヤーヘッドを生成
	m_pWireHead = CWireHead::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 120);
	m_pWireHead->SetAutoSubLife(false);
	m_pWireHead->SetUseDeath(false);
	m_pWireHead->SetUseDraw(true);

	return S_OK;
}
//===================================================================================================================

//===============================================================
//終了処理
//===============================================================
void CWire::Uninit()
{
	CMeshCylinder::Uninit();

	if (m_pPlayer != nullptr)
	{
		m_pPlayer = nullptr;
	}
}
//===================================================================================================================

//===============================================================
//更新処理
//===============================================================
void CWire::Update()
{
	CMeshCylinder::Update();
	const D3DXVECTOR3& Pos = GetPos();
	//const D3DXVECTOR3* pSenterPos = GetSenterPos();
	const int& nNumDivisionXZ = GetNumDivisionXZ();//XZ分割数
	const int& nNumDivisionY = GetNumDivisionY();  //Y分割数
	const int& nNumVtx = GetNumVtx();              //頂点数
	const int& nNumIdx = GetNumIdx();              //インデックス数
	const float& fRadius = GetRadius();            //半径
	const float& fHeight = GetHeight();            //高さ

	float fLength = CCalculation::CalculationLength(m_pPlayer->GetPos(), m_pWireHead->GetPos());
	SetHeight(fLength);
	//CManager::GetDebugProc()->PrintDebugProc("距離：%f\n", fLength);

	D3DXVECTOR3 RotMove = m_pWireHead->GetPos() - m_pPlayer->GetPos();
	D3DXVec3Normalize(&RotMove, &RotMove);
	VERTEX_3D* pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBufferPointer();
	//頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_bUseUpdate == true)
	{
		int nCntArray = 0;
		float fRatioXZ = 0.0f;
		float fRatioY = 0.0f;
		D3DXVECTOR3 LastSenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 MeasureNor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//法線計算用
		for (int nCntVtxY = 0; nCntVtxY < nNumDivisionY; nCntVtxY++)
		{//Y方向のUVはそのまま使う
			fRatioY = (1.0f / (nNumDivisionY - 1)) * nCntVtxY;
			for (int nCntVtxXZ = 0; nCntVtxXZ < nNumDivisionXZ + 1; nCntVtxXZ++)
			{//X方向のUVは重なる頂点があるので、+ 1
				fRatioXZ = (1.0f / (nNumDivisionXZ)) * nCntVtxXZ;
				if (nCntVtxY == 0 && nCntVtxXZ == 0)
				{//最初の周なので上面の中心点を設定する
					pVtx[nCntArray].pos = m_pWireHead->GetPos();//Y方向分割数3つの場合、底面 = 1
					nCntArray++;
				}

				if (nCntVtxY == 0)
				{//最初の周で基準点を決める（9,8,7,6,5,4,3,2,1)
					pVtx[nCntArray].pos = m_VecMtxCircle[nCntVtxXZ].Pos;
				}
				else
				{//基準点に対して軌跡風に頂点を代入していく(18 = 27)
					D3DXVECTOR3 AdjustPos = RotMove * fLength * fRatioY;
					pVtx[nCntArray].pos = pVtx[1 + nCntVtxXZ].pos - AdjustPos;
				}
				//配列カウント
				nCntArray++;

				if (nCntArray < 0 || nCntArray >= nNumVtx)
				{//配列外アクセスチェック
					assert(false);
				}
				if (nCntVtxY == nNumDivisionY - 1 && nCntVtxXZ == nNumDivisionXZ)
				{//最後
					pVtx[nCntArray].pos = m_pPlayer->GetPos();//底面の中心に位置を設定
				}
			}
		}
	}

	//頂点バッファをアンロックする 
	pVtxBuff->Unlock();

}
//===================================================================================================================

//===============================================================
//描画処理
//===============================================================
void CWire::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス
	D3DXMATRIX& mtxWorld = GetMtxWorld();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBufferPointer();
	LPDIRECT3DINDEXBUFFER9 pIdxBuff = GetIdxBufferPointer();
	LPDIRECT3DTEXTURE9 pTexture = GetTexture();
	const int& nNumPolygon = GetNumPolygon();
	const int& nNumVtx = GetNumVtx();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,0.0f,0.0f,0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//========================================================================================
	//円状に点を配置するためのワールド座標を求める
	//========================================================================================
	if (m_pWireHead != nullptr && GetUseDraw() == true)
	{
		D3DXVECTOR3 PosZero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//ワールド座標代入処理で使用する
		D3DXVECTOR3 WireHeadRot = m_pWireHead->GetRot();//ワイヤーヘッドの向き
		D3DXVECTOR3 WireHeadPos = m_pWireHead->GetPos();//ワイヤーヘッドの位置
		int Size = m_VecMtxCircle.size();

		for (int nCnt = 0; nCnt < Size; nCnt++)
		{
			D3DXMATRIX mtxSenter;
			D3DXMatrixIdentity(&mtxSenter);

			//割合を求める
			float fRatio = (2.0f / (Size - 1)) * nCnt;

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&m_VecMtxCircle[nCnt].WorldMtx);

			//向きを反映（ワイヤーヘッドが向いている方向に）
			D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);
			D3DXMatrixMultiply(&m_VecMtxCircle[nCnt].WorldMtx, &m_VecMtxCircle[nCnt].WorldMtx, &mtxRot);

			//位置を反映（ワイヤーヘッドを中心に円状に）
			D3DXMatrixTranslation(&mtxTrans,
				sinf(D3DX_PI * fRatio) * GetRadius(),
				0.0f,
				cosf(D3DX_PI * fRatio) * GetRadius());
			D3DXMatrixMultiply(&m_VecMtxCircle[nCnt].WorldMtx, &m_VecMtxCircle[nCnt].WorldMtx, &mtxTrans);

			//ワイヤーヘッドとワールド変換行列を掛け合わせる
			D3DXMatrixMultiply(&m_VecMtxCircle[nCnt].WorldMtx, &m_VecMtxCircle[nCnt].WorldMtx,&m_pWireHead->GetMatrixWorld());

			//ワールド座標を代入
			D3DXVec3TransformCoord(&m_VecMtxCircle[nCnt].Pos, &PosZero, &m_VecMtxCircle[nCnt].WorldMtx);
		}
	}
	//=======================================================================================================================

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//両面を描画する
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture);

	if (GetUseDraw() == true)
	{
		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, nNumVtx, 0, nNumPolygon);
	}
	//片面だけ描画する
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//===================================================================================================================

//===============================================================
//死亡フラグ設定処理
//===============================================================
void CWire::SetDeath()
{
	if (m_pWireHead != nullptr)
	{
		m_pWireHead->SetUseDeath(true);
		m_pWireHead->SetDeath();
		m_pWireHead = nullptr;
	}

	CObject::SetDeath();
}
//===================================================================================================================

//===============================================================
//生成処理
//===============================================================
CWire* CWire::Create(WIRETYPE Type, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, float fRadius, float fHeight, int nNumDivisionXZ, int nNumDivisionY)
{
	CTexture* pTexture = CManager::GetTexture();
	CWire* pWire = DBG_NEW CWire(Type, fRadius, fHeight, nNumDivisionXZ, Pos, Rot, nNumDivisionY);
	pWire->Init();//初期化処理
	pWire->SetTextureIndex(pTexture->Regist(s_WIRE_FILENAME[static_cast<int>(Type)]));
	pWire->BindTexture(pTexture->GetAddress(pWire->GetTextureIndex()));

	return pWire;
}
//===================================================================================================================