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

	//=================================
	//描画情報構造体
	//=================================
	struct DrawInfo
	{
		//影
		bool bUseShadow = true;//影を使用するかどうか!

		//描画を使用するかどうか
		bool bUseDraw = true;//!

		//カリングするかどうか
		bool bUseCulling = false;

		//色
		int nChengeColorTime = 0;                    //色を変える時間!
		bool bColorChenge = false;                       //色を変えているかどうか!

		//ワールド変換行列
		D3DXMATRIX mtxWorld = {};                     //マトリックスワールド!
		D3DXMATRIX* pMtxParent = nullptr;             //親マトリックス 
		D3DXCOLOR Color = { 1.0f,1.0f,1.0f,1.0f };    //現在の統一の色合い

		//影
		void SetUseShadow(bool bUse) { bUseShadow = bUse; }
		const bool& GetUseDraw() const { return bUseDraw; }

		//描画するかどうか
		void SetUseDraw(bool bUse) { bUseDraw = bUse; }

		//カリングするかどうか
		void SetUseCulling(bool bUse) { bUseCulling = bUse; }

        //ワールド変換行列
		void SetMtxParent(D3DXMATRIX* mtx) {pMtxParent = mtx; }              //親マトリックスを取得                   
		D3DXMATRIX& GetMatrixWorld() { return mtxWorld; }                     //ワールド変換行列の取得
		//================================================================================================================================================
	};
	//============================================================================================================

	struct PosInfo
	{
		//==========================================================
        //位置
        //==========================================================
		D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };                              //位置!
		D3DXVECTOR3 SupportPos = { 0.0f,0.0f,0.0f };                       //最初に設置された位置!
		D3DXVECTOR3 PosOld = { 0.0f,0.0f,0.0f };                           //1f前の位置!
		D3DXVECTOR3 SenterPos = { 0.0f,0.0f,0.0f };                        //中心点の位置
		//=================================================================================================================
    
	    //位置
		void SetPos(D3DXVECTOR3 CopyPos) { Pos = CopyPos; }                                           //位置の設定
		const D3DXVECTOR3& GetPos() const { return Pos; }                                     //位置の取得

		//1f前の位置
		void SetPosOld(D3DXVECTOR3 CopyPosOld) { PosOld = CopyPosOld; }                               //1f前の位置の設定
		const D3DXVECTOR3& GetPosOld() const { return PosOld; }                                            //1f前の位置を設定

		//初期位置
		void SetSupportPos(D3DXVECTOR3 CopySupportPos) { SupportPos = CopySupportPos; }               //設置位置を設定する
		D3DXVECTOR3& GetSupportPos() { return SupportPos; }                                  //設置位置を取得する

		//中心点の位置
		D3DXVECTOR3& GetSenterPos() { return SenterPos; }                                     //中心点を取得する
		//============================================================================================================
	};

	struct RotInfo
	{
		//変数
		D3DXVECTOR3 Rot = {0.0f,0.0f,0.0f};                              //向き!
		D3DXVECTOR3 AddRot = { 0.0f,0.0f,0.0f};//加算する向き!
		bool bUseAddRot = false;   //向きの加算を使用するかどうか!

		//関数
		void SetRot(D3DXVECTOR3 CopyRot) { Rot = CopyRot; }                                           //向きの設定
		D3DXVECTOR3& GetRot() { return Rot; }                                                 //向きの取得
		void SetUseAddRot(bool bUse, D3DXVECTOR3 CopyRot) { bUseAddRot = bUse; AddRot = CopyRot; }
		bool& GetUseAddRot() { return bUseAddRot; }//向きの加算を使用しているかどうか
		D3DXVECTOR3& GetAddRot() { return AddRot; }//向きの加算量を取得
	};

	struct SizeInfo
	{
		//*変数

		//頂点の最小値
		D3DXVECTOR3 VtxMin = { 0.0f,0.0f,0.0f };                           //それぞれの座標のモデルの位置の最小値!
		D3DXVECTOR3 OriginVtxMin = { 0.0f,0.0f,0.0f };                     //元々の最小値!

		//頂点の最大値
		D3DXVECTOR3 VtxMax = { 0.0f,0.0f,0.0f };                           //それぞれの座標のモデルの位置の最大値!
		D3DXVECTOR3 OriginVtxMax = { 0.0f,0.0f,0.0f };                     //元々の最大値!

		//サイズ
		D3DXVECTOR3 Size = { 0.0f,0.0f,0.0f };                             //サイズ!

		//XZのサイズを変える
		bool bSwapVtxXZ = false;

		//変数
		D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f };                            //大きさ!
		D3DXVECTOR3 FormarScale = { 1.0f,1.0f,1.0f };                      //元の拡大率!
		D3DXVECTOR3 MultiScale = { 0.0f,0.0f,0.0f };                       //乗算拡大率
		D3DXVECTOR3 AddScale = { 0.0f,0.0f,0.0f };                         //加算する拡大率を設定!
		bool bUseMultiScale = false;                                       //拡大率を乗算するかどうか
		bool bUseAddScaling = false;                                       //拡大率を使用するかどうか!


		//*関数
		D3DXVECTOR3& GetVtxMax() { return VtxMax; }                                          //各頂点最大値の取得
		void SetVtxMax(D3DXVECTOR3 CopyVtxMax) { VtxMax = CopyVtxMax; }
		D3DXVECTOR3& GetOriginVtxMax() { return OriginVtxMax; }                              //元の各頂点最大値の取得
		void SetOriginVtxMax(D3DXVECTOR3 CopyVtxMax) { OriginVtxMax = CopyVtxMax; }                   //元の最大頂点の設定

		//最小頂点
		D3DXVECTOR3& GetVtxMin() { return VtxMin; }                                          //各頂点最小値の取得
		void SetVtxMin(D3DXVECTOR3 CopyVtxMin) { VtxMin = CopyVtxMin; }
		D3DXVECTOR3& GetOriginVtxMin() { return OriginVtxMin; }                              //元の各頂点最小値の取得
		void SetOriginVtxMin(D3DXVECTOR3 CopyVtxMin) { OriginVtxMin = CopyVtxMin; }                   //元の最小頂点の設定

		D3DXVECTOR3 GetSize() { return Size; }                                                //Xオブジェクトのサイズを取得

		//最大頂点と最小頂点をスワップ
		void ActiveSwapVtxMaxMin()
		{
			if (bSwapVtxXZ == true)
			{
				swap(OriginVtxMax.x, OriginVtxMax.z);
				swap(OriginVtxMin.x, OriginVtxMin.z);
			}
		}
		//関数
		void SetScale(D3DXVECTOR3 CopyScale) { Scale = CopyScale; }                                  //拡大率の設定
		D3DXVECTOR3& GetScale() { return Scale; }                                            //拡大率の取得

		//元の拡大率
		void SetFormarScale(D3DXVECTOR3 CopyScale) { FormarScale = CopyScale; }                      //元の拡大率を設定
		D3DXVECTOR3& GetFormarScale() { return FormarScale; }                                //元の拡大率を取得

		//加算拡大率
		void SetUseAddScale(D3DXVECTOR3 CopyAddScale, bool bUse);                                  //拡大率の加算を使用するかどうか

		//乗算拡大率
		void SetUseMultiScale(bool bUse, D3DXVECTOR3 CopyScale) { bUseMultiScale = bUse; MultiScale = CopyScale; }

		void SetUseSwapVtxXZ(bool bUse) { bSwapVtxXZ = bUse; }
		const bool& GetUseSwapVtxXZ()const { return bSwapVtxXZ; }
	};



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
	
	//クォータニオン
	void SetAxis(float fAxis) { m_fAxis = fAxis; }//回転角を設定
	const float& GetAxis() const { return m_fAxis; }//回転角を取得 
	void SetVecAxis(D3DXVECTOR3 VecAxis) { m_VecAxis = VecAxis; }//回転軸を設定
	const D3DXVECTOR3& GetVecAxis() const { return m_VecAxis; }  //回転軸を取得

	//============================================================================================================

	//==========================================================
	//拡大率
	//==========================================================
	//============================================================================================================

	//==========================================================
	//サイズ
	//==========================================================
	void CalculateSenterPos();
	void SetSize();                                                                         //サイズを設定する 
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
	//モデル情報
	//==========================================================
	CObjectX::ObjectXInfo& GetObjectXInfo() { return m_ObjectXInfo; }
	//================================================================================================================================================

	//===================================
	//描画
	//===================================
	//狙ったマテリアルにテクスチャ割り当て（ポインタのポインタになってるから大丈夫）
	void SetLockOnMatBindTexture(int nNumMat, LPDIRECT3DTEXTURE9 pTexture);

	//色合い
	void SetColor(D3DXCOLOR col, int nColChengeTime, bool bChoose, bool bSetAlpha);


	//=================================================================================================================

	//===================================
	//各構造体
	//===================================

	//描画情報の取得
	DrawInfo& GetDrawInfo() { return m_DrawInfo; }

	//位置情報の取得
	PosInfo& GetPosInfo() { return m_PosInfo; }

	//向き情報の取得
	RotInfo& GetRotInfo() { return m_RotInfo; }

	//サイズ情報の取得
	SizeInfo& GetSizeInfo() { return m_SizeInfo; }
	//=================================================================================================================

private:
	static bool s_bCOMMON_DRAWSHADOW;

	void DrawShadow();                                                       //影を描画する

	//==========================================================
	//クォータニオン
	//==========================================================
    float m_fAxis;//クォータニオンの回転角
	D3DXVECTOR3 m_VecAxis;//クォータニオンの回転軸
	//==========================================================================================================================================================

	//===============================================
	//モデル情報
	//===============================================
	ObjectXInfo m_ObjectXInfo;                      //モデル情報の構造体!
	//===========================================================================================================================================================
	
	//===================================
	//描画関係
	//===================================
	void SetFormarColor();                     //元の色合いに戻す
	//=================================================================================================================

	//===================================
	//モデルインデックス
	//===================================
	int m_nIndexObjectX;
	//==================================================================================================================
	
	//===================================
	//各構造体
	//===================================
	DrawInfo m_DrawInfo;   //描画情報
	PosInfo m_PosInfo;     //位置情報
	RotInfo m_RotInfo;     //向き情報
	SizeInfo m_SizeInfo;   //サイズ情報
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