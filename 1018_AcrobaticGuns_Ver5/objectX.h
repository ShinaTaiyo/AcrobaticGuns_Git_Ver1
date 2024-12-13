//=======================================================================================
//
//６月４日：モデルを描画する[objectX.h]
//Author:ShinaTaiyo
//
//=======================================================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//==============================================
//インクルード
//==============================================
#include "main.h"
#include "object.h"
//=======================================================================================

//=======================================================
//オブジェクトXクラス
//=======================================================
class CObjectX : public CObject
{
public:
	static const int m_nMAX_MAT = 100;                                                      //マテリアルの最大数
	typedef enum
	{
		OBJECTXTYPE_BLOCK = 0,        //ブロック
		OBJECTXTYPE_MODEL,            //モデル
		OBJECTXTYPE_PLAYER,           //プレイヤー
		OBJECTXTYPE_ENEMY,            //敵
		OBJECTXTYPE_ITEM,             //アイテム
		OBJECTXTYPE_MARKER,           //マーカー
		OBJECTXTYPE_ATTACK,           //攻撃
		OBJECTXTYPE_BOSS,             //ボス
		OBJECTXTYPE_TRAP,             //トラップ
		OBJECTXTYPE_SIGNBOARD,        //立て看板
		OBJECTXTYPE_AIMODEL,          //AIモデル
		OBJECTXTYPE_MAX
	}OBJECTXTYPE;

	typedef enum
	{
		ORIGINPOSTYPE_SENTER = 0,
		ORIGINPOSTYPE_DOWN,
	    ORIGINPOSTYPE_MAX
	}ORIGINPOSTYPE;

	typedef enum
	{
		COLORTYPE_RED = 0,
		COLORTYPE_GREEN,
		COLORTYPE_BRUE,
		COLORTYPE_ALPHA,
		COLORTYPE_MAX
	}COLORTYPE;

	//=================================
    //モデル情報構造体
    //=================================
	typedef struct
	{
		LPD3DXMESH pMesh;                           //メッシュ（頂点情報）へのポインタ（モデルの種類数分）
		LPD3DXBUFFER pBuffMat;                      //マテリアルへのポインタ
		DWORD dwNumMat;                             //マテリアルの数
		LPDIRECT3DTEXTURE9* pTexture;               //テクスチャへのポインタ
		D3DCOLORVALUE* Diffuse;          //色合い
		D3DCOLORVALUE* FormarDiffuse;    //元の色合い
	}ObjectXInfo;
	//============================================================================================================

	CObjectX(int nPri = 0, bool bUseintPri = false,CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                                                           //コンストラクタ
	~CObjectX() override;                                                 //デストラクタ
	HRESULT Init() override;                                              //初期化処理
	void Uninit() override;                                               //終了処理
	void Update() override;                                               //更新処理
	void Draw() override;                                                 //描画処理
	void BindObjectXInfo(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPDIRECT3DTEXTURE9* pTexture,D3DCOLORVALUE * pDiffuse);//モデル情報を割り当てる

	//==========================================================
	//静的メンバ宣言
	//==========================================================
	static void SetCommonDraw(bool bDraw) { s_bCOMMON_DRAWSHADOW = bDraw; }

	//==========================================================
	//位置
	//==========================================================
	
	//位置
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; }                                           //位置の設定
	const D3DXVECTOR3& GetPos() const { return m_Pos; }                                     //位置の取得

	//1f前の位置
	void SetPosOld(D3DXVECTOR3 PosOld) { m_PosOld = PosOld; }                               //1f前の位置の設定
	const D3DXVECTOR3& GetPosOld() const { return m_PosOld; }                                            //1f前の位置を設定

	//初期位置
	void SetSupportPos(D3DXVECTOR3 SupportPos) { m_SupportPos = SupportPos; }               //設置位置を設定する
	D3DXVECTOR3 & GetSupportPos() { return m_SupportPos; }                                  //設置位置を取得する

	//中心点の位置
	D3DXVECTOR3& GetSenterPos() { return m_SenterPos; }                                     //中心点を取得する
	//============================================================================================================
	
	//==========================================================
	//向き
	//==========================================================
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; }                                           //向きの設定
	D3DXVECTOR3 &GetRot() { return m_Rot; }                                                 //向きの取得
	void SetUseAddRot(bool bUse, D3DXVECTOR3 Rot) { m_bUseAddRot = bUse; m_AddRot = Rot;}
	bool& GetUseAddRot() { return m_bUseAddRot; }//向きの加算を使用しているかどうか
	D3DXVECTOR3& GetAddRot() { return m_AddRot; }//向きの加算量を取得

	//クォータニオン
	void SetAxis(float fAxis) { m_fAxis = fAxis; }//回転角を設定
	const float& GetAxis() const { return m_fAxis; }//回転角を取得 
	void SetVecAxis(D3DXVECTOR3 VecAxis) { m_VecAxis = VecAxis; }//回転軸を設定
	const D3DXVECTOR3& GetVecAxis() const { return m_VecAxis; }  //回転軸を取得

	//============================================================================================================

	//==========================================================
	//拡大率
	//==========================================================
	
	//拡大率
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }                                   //拡大率の設定
	D3DXVECTOR3 & GetScale() { return m_Scale; }                                            //拡大率の取得

	//元の拡大率
	void SetFormarScale(D3DXVECTOR3 Scale) { m_FormarScale = Scale; }                       //元の拡大率を設定
	D3DXVECTOR3& GetFormarScale() { return m_FormarScale; }                                 //元の拡大率を取得

	//加算拡大率
	void SetUseAddScale(D3DXVECTOR3 AddScale, bool bUse);              //拡大率の加算を使用するかどうか

	//乗算拡大率
	void SetUseMultiScale(bool bUse, D3DXVECTOR3 Scale) { m_bUseMultiScale = bUse; m_MultiScale = Scale; }
	//============================================================================================================

	//==========================================================
	//サイズ
	//==========================================================

	//最大頂点
	D3DXVECTOR3 & GetVtxMax() { return m_VtxMax; }                                          //各頂点最大値の取得
	void SetVtxMax(D3DXVECTOR3 VtxMax) { m_VtxMax = VtxMax; }
	D3DXVECTOR3 & GetOriginVtxMax() { return m_OriginVtxMax; }                              //元の各頂点最大値の取得
	void SetOriginVtxMax(D3DXVECTOR3 VtxMax) { m_OriginVtxMax = VtxMax; }                   //元の最大頂点の設定

	//最小頂点
	D3DXVECTOR3 & GetVtxMin() { return m_VtxMin; }                                          //各頂点最小値の取得
	void SetVtxMin(D3DXVECTOR3 VtxMin) { m_VtxMin = VtxMin; }
	D3DXVECTOR3 & GetOriginVtxMin() { return m_OriginVtxMin; }                              //元の各頂点最小値の取得
	void SetOriginVtxMin(D3DXVECTOR3 VtxMin) { m_OriginVtxMin = VtxMin; }                   //元の最小頂点の設定

	//サイズ
	void SetSize();                                                                         //サイズを設定する 
	D3DXVECTOR3 GetSize() { return m_Size; }                                                //Xオブジェクトのサイズを取得

    //最大頂点と最小頂点をスワップ
	void ActiveSwapVtxMaxMin()
	{
		if (m_bSwapVtxXZ == true)
		{
			swap(m_OriginVtxMax.x, m_OriginVtxMax.z);
			swap(m_OriginVtxMin.x, m_OriginVtxMin.z);
		}
	}
	void SetUseSwapVtxXZ(bool bUse) { m_bSwapVtxXZ = bUse; }
	const bool& GetUseSwapVtxXZ()const{ return m_bSwapVtxXZ; }
	//============================================================================================================

	//==========================================================
	//エディタ
	//==========================================================

	//オブジェクトXの種類
	void SetObjXType(OBJECTXTYPE ObjXType) { m_nObjXType = ObjXType; }                      //オブジェクトXの分類を設定
	OBJECTXTYPE GetObjXType() { return m_nObjXType; }                                       //オブジェクトXのタイプを取得する

	//タイプ番号
	void SetTypeNum(int nTypeNum) { m_nTypeNum = nTypeNum; }                                //オブジェクトごとのタイプ番号を設定
	int GetTypeNum() { return m_nTypeNum;}                                                  //オブジェクトごとのタイプ番号を設定

	//情報表示処理＆操作
	void ManagerChooseControlInfo() override;                                               //ステージマネージャーが情報を操作する

	void SaveInfoTxt(fstream& WritingFile) override;//テキストファイルに情報を保存するための関数

	//================================================================================================================================================

	//==========================================================
	//ワールド変換行列
	//==========================================================
	void SetMtxParent(D3DXMATRIX* mtx) { m_pMtxParent = mtx; }              //親マトリックスを取得                   
	D3DXMATRIX &GetMatrixWorld() { return m_mtxWorld; }                     //ワールド変換行列の取得
	//================================================================================================================================================

	//==========================================================
	//モデル情報
	//==========================================================
	CObjectX::ObjectXInfo& GetObjectXInfo() { return m_ObjectXInfo; }
	//================================================================================================================================================

	//===================================
	//描画
	//===================================
	
	//影
	void SetUseShadow(bool bUse) { m_bUseShadow = bUse; }

	//描画するかどうか
	void SetUseDraw(bool bUse) { m_bUseDraw = bUse; }

	//カリングするかどうか
	void SetUseCulling(bool bUse) { m_bUseCulling = bUse; }

	//狙ったマテリアルにテクスチャ割り当て（ポインタのポインタになってるから大丈夫）
	void SetLockOnMatBindTexture(int nNumMat, LPDIRECT3DTEXTURE9 pTexture);

	//色合い
	void SetColor(D3DXCOLOR col, int nColChengeTime,bool bChoose,bool bSetAlpha);
	//=================================================================================================================
private:
	static bool s_bCOMMON_DRAWSHADOW;

	void DrawShadow();                                                       //影を描画する

	//==========================================================
	//クォータニオン
	//==========================================================
    float m_fAxis;//クォータニオンの回転角
	D3DXVECTOR3 m_VecAxis;//クォータニオンの回転軸

	//==========================================================
	//位置
	//==========================================================
	D3DXVECTOR3 m_Pos;                              //位置!
	D3DXVECTOR3 m_SupportPos;                       //最初に設置された位置!
	D3DXVECTOR3 m_PosOld;                           //1f前の位置!
	D3DXVECTOR3 m_SenterPos;                        //中心点の位置
	void CalculateSenterPos();
	//=================================================================================================================

	//==========================================================
	//向き
	//==========================================================
	//向き
	D3DXVECTOR3 m_Rot;                              //向き!

	//加算向き
	D3DXVECTOR3 m_AddRot;//加算する向き!
	bool m_bUseAddRot = false;   //向きの加算を使用するかどうか!
	//=================================================================================================================

	//==========================================================
	//拡大率
	//==========================================================
	//拡大率
	D3DXVECTOR3 m_Scale;                            //大きさ!

	//元の拡大率
	D3DXVECTOR3 m_FormarScale;                      //元の拡大率!

	//乗算拡大率
	D3DXVECTOR3 m_MultiScale;//!
	bool m_bUseMultiScale;//!

	//加算拡大率
	bool m_bUseAddScaling;                                  //拡大率を使用するかどうか!
	D3DXVECTOR3 m_AddScale;                                 //加算する拡大率を設定!
	//=================================================================================================================

	//==========================================================
	//サイズ
	//==========================================================

	//頂点の最小値
	D3DXVECTOR3 m_VtxMin;                           //それぞれの座標のモデルの位置の最小値!
	D3DXVECTOR3 m_OriginVtxMin;                     //元々の最小値!

	//頂点の最大値
	D3DXVECTOR3 m_VtxMax;                           //それぞれの座標のモデルの位置の最大値!
	D3DXVECTOR3 m_OriginVtxMax;                     //元々の最大値!

	//サイズ
	D3DXVECTOR3 m_Size;                             //サイズ!
	//==========================================================================================================================================================

	//===============================================
	//モデル情報
	//===============================================
	ObjectXInfo m_ObjectXInfo;                      //モデル情報の構造体!
	//===========================================================================================================================================================
	
	//===================================
	//描画関係
	//===================================

	//影
	bool m_bUseShadow;//影を使用するかどうか!

	//描画を使用するかどうか
	bool m_bUseDraw;//!

	//カリングするかどうか
	bool m_bUseCulling;

	//最大頂点と最小頂点をチェンジするかどうか
	bool m_bSwapVtxXZ;

	//色
	int m_nChengeColorTime;                    //色を変える時間!
	bool m_bColorChenge;                       //色を変えているかどうか!
	void SetFormarColor();                     //元の色合いに戻す 

	//ワールド変換行列
	D3DXMATRIX m_mtxWorld;                     //マトリックスワールド!
	D3DXMATRIX * m_pMtxParent;                   //親マトリックス 
	//=================================================================================================================

	//===================================
	//モデルインデックス
	//===================================
	int m_nIndexObjectX;
	//==================================================================================================================
	
	//===================================
	//エディタ関係
	//===================================
	int m_nManagerType;                             //マネージャーで呼び出される時のタイプ番号!
	int m_nTypeNum;                                 //モデルごとのタイプ番号!
	OBJECTXTYPE m_nObjXType;                        //オブジェクトXの分類!

	void ChengeEditScale();             //拡大率を変更する
	void ChengeEditScaleX();
	void ChengeEditScaleY();
	void ChengeEditScaleZ();
	void ChengeEditPos();               //位置を移動する
	void ChengeEditSwapVtxXZ();         //最大頂点と最小頂点を変えるかどうか
	//=================================================================================================================

};
//============================================================================================================
#endif