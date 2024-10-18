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

	CObjectX(int nPriority = 3);                                                           //コンストラクタ
	~CObjectX() override;                                                 //デストラクタ
	HRESULT Init() override;                                              //初期化処理
	void Uninit() override;                                               //終了処理
	void Update() override;                                               //更新処理
	void Draw() override;                                                 //描画処理
	void BindObjectXInfo(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPDIRECT3DTEXTURE9* pTexture,D3DCOLORVALUE * pDiffuse);//モデル情報を割り当てる
	//==========================================================
	//位置
	//==========================================================
	
	//位置
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; }                                           //位置の設定
	D3DXVECTOR3& GetPos() { return m_Pos; }                                                 //位置の取得

	//1f前の位置
	void SetPosOld(D3DXVECTOR3 PosOld) { m_PosOld = PosOld; }                               //1f前の位置の設定
	D3DXVECTOR3& GetPosOld() { return m_PosOld; }                                            //1f前の位置を設定

	//初期位置
	void SetSupportPos(D3DXVECTOR3 SupportPos) { m_SupportPos = SupportPos; }               //設置位置を設定する
	D3DXVECTOR3 & GetSupportPos() { return m_SupportPos; }                                  //設置位置を取得する

	//中心点の位置
	D3DXVECTOR3& GetSenterPos() { return m_SenterPos; }                                     //中心点を取得する
	//============================================================================================================

	//==========================================================
	//移動量
	//==========================================================
	//移動量
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }                                       //移動量を設定する
	D3DXVECTOR3& GetMove() { return m_Move; }                                               //移動量を取得する

	//慣性
	void SetUseInteria(bool bUseInteria) { m_bUseInteria = bUseInteria; }                   //慣性をかけるかどうか
	float& GetInertia() { return m_fInertia; }                                              //慣性を取得する
	void SetInertia(float fInertia) { m_fInertia = fInertia; }                              //慣性を設定する
	void SetUndoInertia() { m_fInertia = m_fNORMAL_INERTIA; }                               //慣性の大きさを元に戻す
	bool& GetUseInteria() { return m_bUseInteria; }                                         //慣性を掛けるかどうかを取得

	//重力
	void SetUseGravity(bool bUse, float fGravityPower) { m_bUseGravity = bUse; m_fGravityPower = fGravityPower; }//重力を設定する
	bool& GetUseGravity() { return m_bUseGravity; }                                         //重力を使用するかどうかを設定

	//乗算加速
	void SetUseMultiSpeed(bool bUse, D3DXVECTOR3 MultiSpeed) { m_MultiSpeed = MultiSpeed; m_bUseMultiSpeed = bUse; }//乗算加速を使用するかどうか

	//加算速度
	void SetUseAddSpeed(bool bUse, D3DXVECTOR3 AddSpeed) { m_bUseAddSpeed = bUse; m_AddSpeed = AddSpeed; }//加速を使用するかどうか
	//============================================================================================================

	//==========================================================
	//向き
	//==========================================================
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; }                                           //向きの設定
	D3DXVECTOR3 &GetRot() { return m_Rot; }                                                 //向きの取得
	void SetUseAddRot(bool bUse, D3DXVECTOR3 Rot) { m_bUseAddRot = bUse; m_AddRot = Rot;}
	bool& GetUseAddRot() { return m_bUseAddRot; }//向きの加算を使用しているかどうか
	D3DXVECTOR3& GetAddRot() { return m_AddRot; }//向きの加算量を取得
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
	D3DXVECTOR3 & GetOriginVtxMax() { return m_OriginVtxMax; }                              //元の各頂点最大値の取得

	//最小頂点
	D3DXVECTOR3 & GetVtxMin() { return m_VtxMin; }                                          //各頂点最小値の取得
	D3DXVECTOR3 & GetOriginVtxMin() { return m_OriginVtxMin; }                              //元の各頂点最小値の取得

	//サイズ
	void SetSize();                                                                         //サイズを設定する 
	D3DXVECTOR3 GetSize() { return m_Size; }                                                //Xオブジェクトのサイズを取得
	//============================================================================================================

	//==========================================================
	//エディタ
	//==========================================================

	//マネージャー番号
	void SetManagerType(int nManagerType) { m_nManagerType = nManagerType; }//マネージャーで呼び出される際のタイプ番号を設定する
	int GetManagerType() { return m_nManagerType; }//マネージャーで呼び出される際のタイプ番号

	//オブジェクトXの種類
	void SetObjXType(OBJECTXTYPE ObjXType) { m_nObjXType = ObjXType; }                      //オブジェクトXの分類を設定
	OBJECTXTYPE GetObjXType() { return m_nObjXType; }                                       //オブジェクトXのタイプを取得する

	//タイプ番号
	void SetTypeNum(int nTypeNum) { m_nTypeNum = nTypeNum; }                                //オブジェクトごとのタイプ番号を設定
	int GetTypeNum() { return m_nTypeNum;}                                                  //オブジェクトごとのタイプ番号を設定

	//情報表示処理＆操作
	virtual void DispInfo();                        //情報表示処理

	//================================================================================================================================================

	//==========================================================
	//ワールド変換行列
	//==========================================================
	D3DXMATRIX &GetMatrixWorld() { return m_mtxWorld; }                                     //ワールド変換行列の取得
	//================================================================================================================================================

	//==========================================================
	//モデル情報
	//==========================================================
	CObjectX::ObjectXInfo& GetObjectXInfo() { return m_ObjectXInfo; }
	//================================================================================================================================================

	//==========================================================
	//判定
	//==========================================================
	
	//地面にいるか
	void SetLanding(bool bLanding) { m_bIsLanding = bLanding; }                             //地面にいるかどうかを設定
	bool& GetLanding() { return m_bIsLanding; }                                             //地面にいるかどうかを取得

	//壁にいるか
	void SetWalling(bool bWalling) { m_bIsWalling = bWalling; }                             //壁にくっついているかどうかを設定
	bool& GetWalling() { return m_bIsWalling; }                                             //壁にくっついているかどうかを取得

	//ジャンプしているか
	void SetJumping(bool bJumping) { m_bIsJumping = bJumping; }                             //ジャンプしているかどうかを設定
	bool& GetJumping() { return m_bIsJumping; }                                             //ジャンプしているかどうかを取得

    //======================================================
	//体力関係
	//======================================================

	//体力
	void SetLife(int nLife) { m_nLife = nLife; }                    //体力を設定する
	int& GetLife() { return m_nLife; }                              //体力を取得する

	//最大体力
	void SetMaxLife(int nMaxLife) { m_nMaxLife = nMaxLife; }        //最大体力を設定する
	int& GetMaxLife() { return m_nMaxLife; }                        //最大体力を取得する

	//ヒットストップ
	void SetHitStopTime(int nHitStopTime) { m_nHitStopTime = nHitStopTime; }//ヒットストップ時間を設定する
	bool& GetHitStop() { return m_bHitStop; }                       //ヒットストップ状態かどうかを取得する

	//自動的に体力を減らす
	void SetAutoSubLife(bool bUse) { m_bAutoSubLife = bUse; }       //体力を自動的に減らすかどうかを設定
	int& GetHitStopTime() { return m_nHitStopTime; }                //ヒットストップ時間を取得する

	//ダメージを与える
	virtual void SetDamage(int nDamage, int nHitStopTime);          //ダメージを与える

	//回復する
	virtual void SetHeal(int nHeal,D3DXCOLOR col,float fWidth,float fHeight);                  //回復量を与える
	//=================================================================================================================

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
	
	//===================================
	//位置更新関係
	//===================================
	void SetUseUpdatePos(bool bUse) { m_bUseUpdatePos = bUse; }//位置の更新を使用するかどうか
	//=================================================================================================================
protected:
	static const float GetNormalGravity() { return m_fNORMAL_GRAVITY; }//基本の重力を取得する
	virtual void GravityProcess();                  //重力の処理
private:
	//===============================================
	//静的メンバ
	//===============================================
	static constexpr float m_fNORMAL_INERTIA = 0.5f;//普通の慣性
	static constexpr float m_fNORMAL_GRAVITY = 1.0f;//普通の重力
	//=================================================================================================================

	void DrawShadow();                                                       //影を描画する

	//==========================================================
	//位置
	//==========================================================
	D3DXVECTOR3 m_Pos;                              //位置!
	D3DXVECTOR3 m_SupportPos;                       //最初に設置された位置!
	D3DXVECTOR3 m_PosOld;                           //1f前の位置!
	D3DXVECTOR3 m_SenterPos;                        //中心点の位置
	bool m_bUseUpdatePos;                           //位置の更新を使用するかどうか!
	void UpdatePos();                               //位置を更新させる
	void CalculateSenterPos();
	//=================================================================================================================

	//==========================================================
	//移動量
	//==========================================================
	//移動量
	D3DXVECTOR3 m_Move;                             //移動量!
	//慣性
	float m_fInertia;                               //慣性!
	bool m_bUseInteria;                             //慣性をかけるかどうか!
	//重力
	bool m_bUseGravity;                             //重力を使用するかどうか!
	float m_fGravityPower;                          //重力の大きさ!
	//乗算加速
	bool m_bUseMultiSpeed;                                  //乗算加速をするかどうか!
	D3DXVECTOR3 m_MultiSpeed;                               //乗算加速度!
	//加速
	bool m_bUseAddSpeed;                                    //加速を使用するかどうか!
	D3DXVECTOR3 m_AddSpeed;                                 //加速度
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
	//体力関係
	//===================================
	
	//ヒットストップ
	bool m_bHitStop;                                //ヒットストップ!
	int m_nHitStopTime;                             //ヒットストップ時間!
	void HitStopProcess();            //ヒットストップの処理

	//自動的に体力を減らすかどうか
	bool m_bAutoSubLife;                            //自動的に体力を減らし続けるかどうか!

	//体力
	int m_nLife;                                    //体力!
	int m_nMaxLife;                                 //最大体力!
	//============================================================================================================

	//===================================
	//判定関係
    //===================================
	bool m_bIsLanding;                              //地面にいるかどうか!
	bool m_bIsWalling;                              //壁にくっついているかどうか!
	bool m_bIsJumping;                              //ジャンプしているかどうか!
	//============================================================================================================

	//===================================
	//描画関係
	//===================================

	//影
	bool m_bUseShadow;//影を使用するかどうか!

	//描画を使用するかどうか
	bool m_bUseDraw;//!

	//カリングするかどうか
	bool m_bUseCulling;

	//色
	bool m_bUseRatioLifeAlpha;                 //体力の割合で透明度を変えるかどうか!
	int m_nChengeColorTime;                    //色を変える時間!
	bool m_bColorChenge;                       //色を変えているかどうか!
	void SetFormarColor();                     //元の色合いに戻す 

	//ワールド変換行列
	D3DXMATRIX m_mtxWorld;                      //マトリックスワールド!
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
	//=================================================================================================================

};
//============================================================================================================
#endif